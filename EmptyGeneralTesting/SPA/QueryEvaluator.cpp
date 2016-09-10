// Maintained by: Joseph
//
// Accessed by:
// - QueryProcessor
//
// Uses:
// QueryTable
// ClauseObject
// SelectObject

#include "QueryEvaluator.h"
#include "ResultsTable.h"

QueryEvaluator* QueryEvaluator::_instance = nullptr;

/*
Singleton
*/
QueryEvaluator* QueryEvaluator::getInstance()
{
	if (_instance == nullptr) {
		_instance = new QueryEvaluator;
	}

	return _instance;
}

PKB* QueryEvaluator::getPKB()
{
	return this->mPKB;
}

SynonymTable* QueryEvaluator::getSynonymTable()
{
	return this->mSynonymTable;
}

/*
Evaluate QueryTree
*/
void QueryEvaluator::evaluate(QueryTable queryTable) {
	try {

		// gets all the clauses and select objects
		std::vector<ClauseSuchThatObject> suchThats = queryTable.getSuchThats();
		std::vector<ClauseWithObject> withs = queryTable.getWiths();
		std::vector<ClausePatternObject> patterns = queryTable.getPatterns();
		SelectObject select = queryTable.getSelect();

		// boolean status on relationships holds
		bool relationshipHolds = true;

		// get Synonym Table & list of synonyms
		SynonymTable *mSynonymTable = getSynonymTable();
		std::vector<SynonymObject> synonymObjects = mSynonymTable->getObjects();

		// populate Results Table 
		for (std::vector<SynonymObject>::iterator it = synonymObjects.begin(); it != synonymObjects.end(); it++) {
			ResultsObject resultsObject(it->getSynonym());
			resultsTable.insert(resultsObject);
			resultsTable.insertSet(it->getSynonym(), getPKB()->getStmtsByType(it->getType()));
		}

		// iterate the such that clauses vectors and evaluate them
		std::vector<ClauseSuchThatObject> evaluatedSTs;
		for (std::vector<ClauseSuchThatObject>::iterator it = suchThats.begin(); it != suchThats.end(); it++) {
			ClauseSuchThatObject suchThatObject = evaluateSuchThat(*it);
			evaluatedSTs.push_back(suchThatObject);
			relationshipHolds = relationshipHolds && suchThatObject.getResultsBoolean();
		}
		suchThats = std::vector<ClauseSuchThatObject>(evaluatedSTs.begin(), evaluatedSTs.end());

		// iterate the with clauses vectors and evaluate them
		std::vector<ClauseWithObject> evaluatedWs;
		for (std::vector<ClauseWithObject>::iterator it = withs.begin(); it != withs.end(); it++) {
			ClauseWithObject withObject = evaluateWith(*it);
			evaluatedWs.push_back(withObject);
			relationshipHolds = relationshipHolds && withObject.getResultsBoolean();
		}
		withs = std::vector<ClauseWithObject>(evaluatedWs.begin(), evaluatedWs.end());

		// iterate the pattern clauses vectors and evaluate them
		std::vector<ClausePatternObject> evaluatedPs;
		for (std::vector<ClausePatternObject>::iterator it = patterns.begin(); it != patterns.end(); it++) {
			ClausePatternObject patternObject = evaluatePattern(*it);
			evaluatedPs.push_back(patternObject);
			relationshipHolds = relationshipHolds && patternObject.getResultsBoolean();
		}
		patterns = std::vector<ClausePatternObject>(evaluatedPs.begin(), evaluatedPs.end());

		// evaluate results by constraint of select object
		evaluateSelect(select, relationshipHolds);

	}
	catch (std::runtime_error e) {
		throw e.what();
	}
}

ClauseSuchThatObject QueryEvaluator::evaluateSuchThat(ClauseSuchThatObject suchThatRelObject)
{
	RelationshipType type = suchThatRelObject.getRelationshipType();
	ClauseSuchThatArgObject argOne = suchThatRelObject.getArgsOne();
	ClauseSuchThatArgObject argTwo = suchThatRelObject.getArgsTwo();
	bool relationshipHolds = true;
	PKB *pkb = getPKB();

	// FOLLOW/FOLLOWS_STAR/PARENT/PARENTSTAR RELATIFONSHIP :
	if (type == FOLLOWS || type == FOLLOWS_STAR || type == PARENT || type == PARENTSTAR) {
		// both are statements number : Follows(3,4)
		if (argOne.getIntegerValue() > 0 && argTwo.getIntegerValue() > 0) {
			suchThatRelObject.setResultsBoolean(pkb->is(type, argOne.getIntegerValue(), argTwo.getIntegerValue()));
		}
		// arg1 is synonym, and arg2 is integer : Follows(s,3);Follows(a,4)
		else if (argOne.getIsSynonym() && argTwo.getIntegerValue() > 0) {
			// store results
			std::set<StmtNumber> statements = pkb->getStmtsByStmt(argTwo.getIntegerValue(), type);     // Get stmts that arg2(integer) follows
			
			// get existing results from results table
			std::set<StmtNumber> currentStatements = resultsTable.getSetInt(argOne.getStringValue());

			// intersect two sets
			std::set<StmtNumber> updatedStatements;
			set_intersection(statements.begin(), statements.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// check if relationship holds/have results
			if (updatedStatements.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// update the results table
			resultsTable.replaceSet(argOne.getStringValue(), updatedStatements);
		}
		// arg1 is integer, and arg1 is synonym : Follows(3,s);Follows(4,a)
		else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym()) {
			// store results
			std::set<StmtNumber> statements = pkb->getStmtsByStmt(type, argOne.getIntegerValue()); // Get stmts that arg1(integer) is followed by
			
		   // get existing results from results table
			std::set<StmtNumber> currentStatements = resultsTable.getSetInt(argTwo.getStringValue());

			// intersect two sets
			std::set<StmtNumber> updatedStatements;
			set_intersection(statements.begin(), statements.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// check if relationship holds/have results
			if (updatedStatements.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// update the results table
			resultsTable.replaceSet(argTwo.getStringValue(), updatedStatements);

		}
		// arg1 is underscore & arg2 is integer : Follows(_,10);
		else if (argOne.getIsSynonym() == false && argOne.getStringValue() == "_" && argTwo.getIntegerValue() > 0){			
			// store results
			std::set<StmtNumber> statements = pkb->getStmtsByStmt(argTwo.getIntegerValue(), type);     // Get stmts that arg2(integer) follows

			// check if relationship holds
			if (statements.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

		}
		// arg1 is underscore & arg2 is synonym : Follows(_,s);
		else if (argOne.getIsSynonym() == false && argOne.getStringValue() == "_" && argTwo.getIsSynonym()) {
			// set "_" to retrieve all statements and also get all statements of synonym
			std::set<StmtNumber> statements = pkb->getAllStmts();
			std::set<StmtNumber> s = pkb->getStmtsByType(argTwo.getEntityType());
			std::set<StmtNumber> evaluatedS;

			// get existing results from results table
			std::set<StmtNumber> currentStatements = resultsTable.getSetInt(argTwo.getStringValue());

			// check if any synonym statements follows any statements
			for (StmtSetIterator i = statements.begin(); i != statements.end(); i++) {
				for (StmtSetIterator j = currentStatements.begin(); j != currentStatements.end(); j++) {
					if (pkb->is(type, *i, *j)) {
						suchThatRelObject.setResultsBoolean(true);
						evaluatedS.insert(*i);
					}
				}
			}

			// intersect the results set
			std::set<StmtNumber> updatedStatements;
			set_intersection(evaluatedS.begin(), evaluatedS.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// check if relationship holds/have results
			if (updatedStatements.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// update the results table
			resultsTable.replaceSet(argTwo.getStringValue(), updatedStatements);

		}
		// arg1 is integer & arg2 is underscore : Follows(3,_);
		else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
			// store results
			std::set<StmtNumber> statements = pkb->getStmtsByStmt(type, argOne.getIntegerValue());     // Get stmts that arg2(integer) follows
			
			// check if relationship holds
			if (statements.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}
		}
		// arg1 is synonym & arg2 is underscore : Follows(s,_);
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {			
			// set "_" to retrieve all statements and also get all statements of synonym
			std::set<StmtNumber> s = pkb->getStmtsByType(argOne.getEntityType());
			std::set<StmtNumber> statements = pkb->getAllStmts();
			std::set<StmtNumber> evaluatedS;

			// get existing results from results table
			std::set<StmtNumber> currentStatements = resultsTable.getSetInt(argOne.getStringValue());

			// check if any synonym statements follows any statements
			for (StmtSetIterator cs = currentStatements.begin(); cs != currentStatements.end(); cs++) {
				for (StmtSetIterator s = statements.begin(); s != statements.end(); s++) {
					if (pkb->is(type, *cs, *s)) {
						suchThatRelObject.setResultsBoolean(true);
						evaluatedS.insert(*cs);
					}
				}
			}

			// intersect the results set
			std::set<StmtNumber> updatedStatements;
			set_intersection(evaluatedS.begin(), evaluatedS.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// check if relationship holds/have results
			if (updatedStatements.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// update the results table
			resultsTable.replaceSet(argOne.getStringValue(), updatedStatements);

		}
		// both arguments are a synonym : Follows(s1,s2); Follow(a,s1);
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym()) {
			// get all the statements for each synonym entity type
			std::set<StmtNumber> s1s = pkb->getStmtsByType(argOne.getEntityType());
			std::set<StmtNumber> s2s = pkb->getStmtsByType(argTwo.getEntityType());
			std::set<StmtNumber> evaluatedS1s;
			std::set<StmtNumber> evaluatedS2s;

			// obtain evaluation results
			for (StmtSetIterator s1 = s1s.begin(); s1 != s1s.end(); s1++) {
				for (StmtSetIterator s2 = s2s.begin(); s2 != s2s.end(); s2++) {
					if (pkb->is(type, *s1, *s2)) {
						evaluatedS1s.insert(*s1);
						evaluatedS2s.insert(*s2);
					}
				}
			}

			// retrieve current statements
			std::set<StmtNumber> currentStatements1 = resultsTable.getSetInt(argOne.getStringValue());
			std::set<StmtNumber> currentStatements2 = resultsTable.getSetInt(argTwo.getStringValue());

			// intersect two sets x2
			std::set<StmtNumber> updatedStatements1;
			set_intersection(evaluatedS1s.begin(), evaluatedS1s.end(), currentStatements1.begin(), currentStatements1.end(),
				std::inserter(updatedStatements1, updatedStatements1.begin()));
			
			std::set<StmtNumber> updatedStatements2;
			set_intersection(evaluatedS2s.begin(), evaluatedS2s.end(), currentStatements2.begin(), currentStatements2.end(),
				std::inserter(updatedStatements2, updatedStatements2.begin()));

			// check if relationship holds/have results
			if (updatedStatements1.size() > 0 || updatedStatements2.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// update the results table
			resultsTable.replaceSet(argOne.getStringValue(), updatedStatements1);
			resultsTable.replaceSet(argTwo.getStringValue(), updatedStatements2);

		}
	}

	// iteration 1 : only allow 'statement' at left instead of 'procedure' too
	// MODIFIES/USES RELATIONSHIP :

	else if (type == MODIFIES || type == USES) {
		// if left arg is 'statement number', right arg is "x" (Modifies(3,"x"))
		if (argOne.getIntegerValue() > 0 &&  argTwo.getIsSynonym() == false && argTwo.getStringValue() != "_") {
			suchThatRelObject.setResultsBoolean(pkb->is(type, argOne.getIntegerValue(), pkb->getVarIndex(argTwo.getStringValue())));
		}
		// if left arg is 'statement number', right arg is a synonym (Modifies(3,v))
		else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym()) {
			// get results
			std::set<VarIndex> variableIndexes = pkb->getVarsByStmt(argOne.getIntegerValue(), type);
			
			// convert results into set<varName>
			std::set<VarName> variables;
			for (std::set<VarIndex>::iterator it = variableIndexes.begin(); it != variableIndexes.end(); it++) {
				variables.insert(pkb->getVarName(*it));
			}

			// get existing results from the results table
			std::set<VarName> currentVariables = resultsTable.getSetString(argTwo.getStringValue());

			// intersect two sets
			std::set<VarName> updatedVariables;
			set_intersection(variables.begin(), variables.end(), currentVariables.begin(), currentVariables.end(),
				std::inserter(updatedVariables, updatedVariables.begin()));

			// check if relationship holds/have results
			if (updatedVariables.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// update the results table
			resultsTable.replaceSet(argTwo.getStringValue(), updatedVariables);

		}
		//if left arg is 'statement number', right arg is "_" (Modifies(3,_);
		else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
			// store results
			std::set<VarIndex> variables = pkb->getVarsByStmt(argOne.getIntegerValue(), type);     // Get vars that arg1(integer) follows
																									   
			// check if relationship holds
			if (variables.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}
		}
		// if left arg is synonym, right arg is "x" (Modifies(s,"x")); Modifies(a,"x")
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue() != "_") {
			// get all statements that modifies variable
			std::set<StmtNumber> statements = pkb->getStmtsByVar(type, argTwo.getStringValue());

			// get existing results from results table
			std::set<StmtNumber> currentStatements = resultsTable.getSetInt(argOne.getStringValue());

			// intersect two sets
			std::set<StmtNumber> updatedStatements;
			set_intersection(statements.begin(), statements.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// check if relationship holds/have results
			if (updatedStatements.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// update the synonym table
			resultsTable.replaceSet(argOne.getStringValue(), updatedStatements);

		}
		//if left arg is synonym, right arg is "_" (Modifies(s,_);
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
			// set "_" to retrieve all statements and also get all statements of synonym
			std::set<StmtNumber> s = pkb->getStmtsByType(argOne.getEntityType());
			std::set<VarName> variables = pkb->getAllVarNames();
			std::set<StmtNumber> evaluatedS;

			// get existing results from results table
			std::set<StmtNumber> currentStatements = resultsTable.getSetInt(argOne.getStringValue());

			// check if any synonym statements follows any statements
			for (StmtSetIterator cs = currentStatements.begin(); cs != currentStatements.end(); cs++) {
				for (VarNameSetIterator s = variables.begin(); s != variables.end(); s++) {
					if (pkb->is(type, *cs, pkb->getVarIndex(*s))) {
						suchThatRelObject.setResultsBoolean(true);
						evaluatedS.insert(*cs);
					}
				}
			}

			// intersect the results set
			std::set<StmtNumber> updatedStatements;
			set_intersection(evaluatedS.begin(), evaluatedS.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// check if relationship holds/have results
			if (updatedStatements.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// update the results table
			resultsTable.replaceSet(argOne.getStringValue(), updatedStatements);
		}
		// both args are synonym (Modifies(a,v));Modifies(s,v)
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym()) {
			// get all the statements for each synonym entity type
			std::set<StmtNumber> s1s = pkb->getStmtsByType(argOne.getEntityType());
			std::set<VarName> v1s = pkb->getAllVarNames();
			std::set<StmtNumber> evaluatedS1s;
			std::set<VarName> evaluatedV1s;

			// obtain evaluation results
			for (StmtSetIterator s1 = s1s.begin(); s1 != s1s.end(); s1++) {
				for (VarNameSetIterator v1 = v1s.begin(); v1 != v1s.end(); v1++) {
					if (pkb->is(type, *s1, pkb->getVarIndex(*v1))) {
						evaluatedS1s.insert(*s1);
						evaluatedV1s.insert(*v1);
					}
				}
			}

			// retrieve current statements
			std::set<StmtNumber> currentStatements = resultsTable.getSetInt(argOne.getStringValue());
			std::set<VarName> currentVariables = resultsTable.getSetString(argTwo.getStringValue());

			// intersect two sets x2
			std::set<StmtNumber> updatedStatements;
			set_intersection(evaluatedS1s.begin(), evaluatedS1s.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			std::set<VarName> updatedVariables;
			set_intersection(evaluatedV1s.begin(), evaluatedV1s.end(), currentVariables.begin(), currentVariables.end(),
				std::inserter(updatedVariables, updatedVariables.begin()));

			// check if relationship holds/have results
			if (updatedStatements.size() > 0 || updatedVariables.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// update the results table
			resultsTable.replaceSet(argOne.getStringValue(), updatedStatements);
			resultsTable.replaceSet(argTwo.getStringValue(), updatedVariables);

		}
	}

	return suchThatRelObject;
}

ClauseWithObject QueryEvaluator::evaluateWith(ClauseWithObject withObject)
{
	return ClauseWithObject();
}


ClausePatternObject QueryEvaluator::evaluatePattern(ClausePatternObject patternObject)
{
	return ClausePatternObject();
}


bool QueryEvaluator::evaluateSelect(SelectObject selectObject, bool relationshipHolds)
{
	// constraint results by SelectObj
	if (relationshipHolds) {
		// if its Select BOOLEAN
		if (selectObject.getBoolean()) {
			// output : TRUE
			return true;
		}
		// else then it must be a synonym
		else {
			// TODO: Define resultsTable
			// resultsTable.get(select.getStringValue());
			return true;
		}
	}
	else {
		if (selectObject.getBoolean()) {
			// output : FALSE
		}
		return false;
	}
}