
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

QueryEvaluator* QueryEvaluator::_instance = nullptr;

/*
Singleton
*/
QueryEvaluator* QueryEvaluator::getInstance()
{
	if (_instance == nullptr) {
		_instance = new QueryEvaluator();
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

		// obtain queryTable structure 
		this->queryTable = queryTable;

		// gets all the clauses and select objects
		std::set<ClauseSuchThatObject> suchThats = queryTable.getSuchThats();
//		std::set<WithObject> withs = queryTable.getWiths();
//		std::set<PatternObject> patterns = queryTable.getPatterns();
		SelectObject select = queryTable.getSelect();

		// boolean status on relationships holds
		bool relationshipHolds = true;

		// iterate the clauses vectors and evaluate them
		std::vector<ClauseSuchThatObject> evaluatedSTs;
		for (std::set<ClauseSuchThatObject>::iterator it = suchThats.begin(); it != suchThats.end(); it++) {
			ClauseSuchThatObject evaluatedST = evaluateSuchThat(*it);
//			evaluatedSTs.push_back(evaluatedST);
//			relationshipHolds = relationshipHolds && evaluatedST.getResultsBoolean();
		}
//		suchThats = std::set<ClauseSuchThatObject>(evaluatedSTs.begin(), evaluatedSTs.end());
		// iterate the clauses vectors and evaluate them
/*		for (std::set<WithObject>::iterator it = withs.begin(); it != withs.end(); it++) {
			evaluateWith(*it);
			relationshipHolds = relationshipHolds && it->getResultsBoolean();
		}
		// iterate the clauses vectors and evaluate them
		for (std::set<PatternObject>::const_iterator it = patterns.begin(); it != patterns.end(); it++) {
			evaluatePattern(*it);
			relationshipHolds = relationshipHolds && it->getResultsBoolean();
		}
		// constraint results by SelectObj
		if (relationshipHolds) {
			// if its Select BOOLEAN
			if (select.getBoolean()) {
				// output : true
			}
			// then it must be a synonym
			else {
				// TODO: Define resultsTable
				// resultsTable.get(select.getStringValue());
			}
		}
		else {
			// output : invalid/none
		}
*/
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

	// get PKB
	PKB *pkb = getPKB();

	// get Synonym Table
	SynonymTable *mSynonymTable = getSynonymTable();

	// populate Synonym Table 
	for (std::vector<SynonymObject>::iterator it = mSynonymTable->getObjects().begin(); it < mSynonymTable->getObjects().end(); it++) {
		mSynonymTable->insertSet(it->getSynonym(), pkb->getStmtsByType(it->getType()));
	}

	// FOLLOW/FOLLOW*/PARENT/PARENT* RELATIONSHIP :

	if (type == FOLLOWS || type == FOLLOWSSTAR || type == PARENT || type == PARENTSTAR) {
		// both are statements number : Follows(3,4)
		if (argOne.getIntegerValue() > 0 && argTwo.getIntegerValue() > 0) {
			suchThatRelObject.setResultsBoolean(pkb->is(type, argOne.getIntegerValue(), argTwo.getIntegerValue()));
		}
		// arg1 is synonym, and arg2 is integer : Follows(s,3);Follows(a,4)
		else if (argOne.getIsSynonym() && argTwo.getIntegerValue() > 0) {
			// store results
			std::set<StmtNumber> statements = pkb->getStmtsByStmt(argTwo.getIntegerValue(), type);     // Get stmts that arg2(integer) follows
			
			// get existing results from synonym table
			std::set<StmtNumber> currentStatements = mSynonymTable->getSetInt(argOne.getStringValue());

			// intersect two sets
			std::set<StmtNumber> updatedStatements;
			set_intersection(statements.begin(), statements.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// check if relationship holds/have results
			if (updatedStatements.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// update the synonym table
			mSynonymTable->replaceSet(argOne.getStringValue(), updatedStatements);
		}
		// arg1 is integer, and arg1 is synonym : Follows(3,s);Follows(4,a)
		else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym()) {
			// store results
//			std::set<StmtNumber> statements = pkb->getStmtsByStmt(argOne.getIntegerValue(), FOLLOWED_BY); // Get stmts that arg1(integer) is followed by
			std::set<StmtNumber> statements = pkb->getStmtsByStmt(type, argOne.getIntegerValue()); // Get stmts that arg1(integer) is followed by
			// get existing results from synonym table
			std::set<StmtNumber> currentStatements = mSynonymTable->getSetInt(argTwo.getStringValue());

/*
			// intersect two sets
			std::set<StmtNumber> updatedStatements;
			set_intersection(statements.begin(), statements.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// update the synonym table
			mSynonymTable->replaceSet(argTwo.getStringValue(), updatedStatements);
*/
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

			// get existing results from synonym table
			std::set<StmtNumber> currentStatements = mSynonymTable->getSetInt(argTwo.getStringValue());

			// check if any synonym statements follows any statements
			for (StmtSetIterator i = statements.begin(); i != statements.end(); i++) {
				for (StmtSetIterator j = currentStatements.begin(); j != currentStatements.end(); j++) {
					if (pkb->is(type, *i, *j)) {
						suchThatRelObject.setResultsBoolean(true);
						evaluatedS.insert(*i);
					}
				}
			}

/*
			// intersect the synonym set
			std::set<StmtNumber> updatedStatements;
			set_intersection(evaluatedS.begin(), evaluatedS.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// update the synonm table
			mSynonymTable->replaceSet(argTwo.getStringValue(), updatedStatements);
*/
		}
		// arg1 is integer & arg2 is underscore : Follows(3,_);
		else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
			// store results
//			std::set<StmtNumber> statements = pkb->getStmtsByStmt(argOne.getIntegerValue(), FOLLOWED_BY);     // Get stmts that arg2(integer) follows
			std::set<StmtNumber> statements = pkb->getStmtsByStmt(type, argOne.getIntegerValue());     // Get stmts that arg2(integer) follows
			// check if relationship holds
			if (statements.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}
		}
		// arg1 is synonym & arg2 is underscore : Follows(s,_);
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
			
			// set "_" to retrieve all statements and also get all statements of synonym
			std::set<StmtNumber> statements = pkb->getAllStmts();
			std::set<StmtNumber> s = pkb->getStmtsByType(argOne.getEntityType());
			std::set<StmtNumber> evaluatedS;

			// get existing results from synonym table
			std::set<StmtNumber> currentStatements = mSynonymTable->getSetInt(argOne.getStringValue());

			// check if any synonym statements follows any statements
			for (StmtSetIterator cs = currentStatements.begin(); cs != currentStatements.end(); cs++) {
				for (StmtSetIterator s = statements.begin(); s != statements.end(); s++) {
					if (pkb->is(type, *cs, *s)) {
						suchThatRelObject.setResultsBoolean(true);
						evaluatedS.insert(*cs);
					}
				}
			}
/*
			// intersect the synonym set
			std::set<StmtNumber> updatedStatements;
			set_intersection(evaluatedS.begin(), evaluatedS.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// update the synonm table
			mSynonymTable->replaceSet(argOne.getStringValue(), updatedStatements);
*/
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
			std::set<StmtNumber> currentStatements1 = mSynonymTable->getSetInt(argOne.getStringValue());
			std::set<StmtNumber> currentStatements2 = mSynonymTable->getSetInt(argTwo.getStringValue());
/*
			// intersect two sets x2
			std::set<StmtNumber> updatedStatements1;
			set_intersection(evaluatedS1s.begin(), evaluatedS1s.end(), currentStatements1.begin(), currentStatements1.end(),
				std::inserter(updatedStatements1, updatedStatements1.begin()));
			
			std::set<StmtNumber> updatedStatements2;
			set_intersection(evaluatedS2s.begin(), evaluatedS2s.end(), currentStatements2.begin(), currentStatements2.end(),
				std::inserter(updatedStatements2, updatedStatements2.begin()));

			// update the synonm table
			mSynonymTable->replaceSet(argOne.getStringValue(), updatedStatements1);
			mSynonymTable->replaceSet(argTwo.getStringValue(), updatedStatements2);
*/
		}

		return suchThatRelObject;

	}

	// iteration 1 : only allow 'statement' at left instead of 'procedure' too
	// MODIFIES/USES RELATIONSHIP :

	else if (type == MODIFIES || type == USES) {
		// if left arg is 'statement number', right arg is "x" (Modifies(3,"x"))
		if (argOne.getIntegerValue() > 0 &&  argTwo.getIsSynonym() == false && argTwo.getStringValue().length() > 0) {
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

			// get existing results from the synonym table
			std::set<VarName> currentVariables = mSynonymTable->getSetString(argTwo.getStringValue());

/*
			// intersect two sets
			std::set<VarName> updatedVariables;
			set_intersection(variables.begin(), variables.end(), currentVariables.begin(), currentVariables.end(),
				std::inserter(updatedVariables, updatedVariables.begin()));

			// update the synonym table
			mSynonymTable->replaceSet(argOne.getIntegerValue(), updatedVariables);
*/
		}
		// if left arg is synonym, right arg is "x" (Modifies(s,"x")); Modifies(a,"x")
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue().length() > 0) {
			// get all statements that modifies synonym
			std::set<StmtNumber> statements = pkb->getStmtsByVar(type, "x");

			// get existing results from synonym table
			std::set<StmtNumber> currentStatements = mSynonymTable->getSetInt(argOne.getStringValue());

/*
			// intersect two sets
			std::set<VarName> updatedStatements;
			set_intersection(statements.begin(), statements.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// update the synonym table
			mSynonymTable->replaceSet(argOne.getStringValue(), updatedStatements);
*/
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
			std::set<StmtNumber> currentStatements = mSynonymTable->getSetInt(argOne.getStringValue());
			std::set<VarName> currentVariables = mSynonymTable->getSetString(argTwo.getStringValue());

/*
			// intersect two sets x2
			std::set<StmtNumber> updatedStatements;
			set_intersection(evaluatedS1s.begin(), evaluatedS1s.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			std::set<StmtNumber> updatedVariables;
			set_intersection(evaluatedV1s.begin(), evaluatedV1s.end(), currentVariables.begin(), currentVariables.end(),
				std::inserter(updatedVariables, updatedVariables.begin()));

			// update the synonm table
			mSynonymTable->replaceSet(argOne.getStringValue(), updatedStatements);
			mSynonymTable->replaceSet(argTwo.getStringValue(), updatedVariables);
*/
		}

		return suchThatRelObject;
	}
	/*	else if (type == PARENT) {
	// both are statements number : Parent(3,4)
	if (argOne.getIntegerValue() > 0 && argTwo.getIntegerValue() > 0) {
	}
	// arg1 is synonym, and arg2 is integer : Parent(s,3);Parent(a,4)
	else if (argOne.getIsSynonym() && argTwo.getIntegerValue() > 0) {

	}
	// arg1 is integer, and arg1 is synonym : Parent(3,s);Parent(4,a)
	else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym()) {

	}
	// arg1 is underscore & arg2 is integer : Parent(_,10);
	else if (argOne.getIsSynonym() == false && argOne.getStringValue() == "_" && argTwo.getIntegerValue() > 0) {

	}
	// arg1 is underscore & arg2 is synonym : Parent(_,s);
	else if (argOne.getIsSynonym() == false && argOne.getStringValue() == "_" && argTwo.getIsSynonym()) {

	}
	// arg1 is integer & arg2 is underscore : Parent(3,_);
	else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {

	}
	// arg1 is synonym & arg2 is underscore : Parent(s,_);
	else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {

	}
	// both arguments are a synonym : Parent(s1,s2);Parent(a,s1);
	else if (argOne.getIsSynonym() && argTwo.getIsSynonym()) {

	}
	}
	*/
/*	else if (type == USES) {
		// if left arg is 'statement number', right arg is "x" (Uses(3,"x"))
		if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym() == false && argTwo.getStringValue().length() > 0) {

		}
		// if left arg is 'statement number', right arg is a synonym (Uses(3,v))
		else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym()) {

		}
		// if left arg is synonym, right arg is "x" (Uses(s,"x")); Uses(a,"x")
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue().length() > 0) {

		}
		// both args are synonym (Uses(a,v));Uses(s,v)
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym()) {

		}
	}
*/
}
/*
void QueryEvaluator::evaluateWith(WithObject withObject)
{
}


void QueryEvaluator::evaluatePattern(PatternObject patternObject)
{
}
*/

void QueryEvaluator::evaluateSelect(SelectObject selectObject)
{
}
