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
	if (_instance == nullptr) 
		_instance = new QueryEvaluator;
	_instance->mPKB = PKB::getInstance();
	_instance->mSynonymTable = SynonymTable::getInstance();
	
	return _instance;
}

PKB *QueryEvaluator::getPKB()
{
	return this->mPKB;
}

SynonymTable *QueryEvaluator::getSynonymTable()
{
	return this->mSynonymTable;
}

void *QueryEvaluator::setPKB(PKB *pkb)
{
	return this->mPKB = pkb;
}

/*
Evaluate QueryTree
*/
std::vector <std::string> QueryEvaluator::evaluate(QueryTable queryTable) {
	try {

		// gets all the clauses and select objects
		std::vector<ClauseSuchThatObject> suchThats = queryTable.getSuchThats();
		std::vector<ClauseWithObject> withs = queryTable.getWiths();
		std::vector<ClausePatternObject> patterns = queryTable.getPatterns();
		SelectObject select = queryTable.getSelect();

		// boolean status on relationships holds
		bool relationshipHolds = true;

		populateResultTable(getSynonymTable());

		// iterate the such that clauses vectors and evaluate them
		for (std::vector<ClauseSuchThatObject>::iterator it = suchThats.begin(); it != suchThats.end(); it++) {
			ClauseSuchThatObject suchThatObject = evaluateSuchThat(*it);
			relationshipHolds = relationshipHolds && suchThatObject.getResultsBoolean();
		}

		// iterate the with clauses vectors and evaluate them
		for (std::vector<ClauseWithObject>::iterator it = withs.begin(); it != withs.end(); it++) {
			ClauseWithObject withObject = evaluateWith(*it);
			relationshipHolds = relationshipHolds && withObject.getResultsBoolean();
		}

		// iterate the pattern clauses vectors and evaluate them
		for (std::vector<ClausePatternObject>::iterator it = patterns.begin(); it != patterns.end(); it++) {
			ClausePatternObject patternObject = evaluatePattern(*it);
			relationshipHolds = relationshipHolds && patternObject.getResultsBoolean();
		}

		// evaluate results by constraint of select object
		return evaluateSelect(select, relationshipHolds);

	}
	catch (std::runtime_error e) {
		throw e.what();
	}
}

ResultsTable *QueryEvaluator::populateResultTable(SynonymTable *synonymTable)
{
	// get Synonym Table & list of synonyms
	SynonymTable *mSynonymTable = synonymTable;
	std::vector<SynonymObject> synonymObjects = mSynonymTable->getObjects();

	// populate Results Table 
	for (std::vector<SynonymObject>::iterator it = synonymObjects.begin(); it != synonymObjects.end(); it++) {
		ResultsObject resultsObject(it->getSynonym());
		resultsTable.insert(resultsObject);
		if (it->getType() == VARIABLE) {
			resultsTable.insertSet(it->getSynonym(), getPKB()->getAllVarNames());
		}
		else {
			resultsTable.insertSet(it->getSynonym(), getPKB()->getStmtsByType(it->getType()));
		}

	}

	return &resultsTable;
}

ClauseSuchThatObject QueryEvaluator::evaluateSuchThat(ClauseSuchThatObject suchThatRelObject)
{
	RelationshipType type = suchThatRelObject.getRelationshipType();
	ClauseSuchThatArgObject argOne = suchThatRelObject.getArgsOne();
	ClauseSuchThatArgObject argTwo = suchThatRelObject.getArgsTwo();
	bool relationshipHolds = true;
	PKB *pkb = getPKB();

	// FOLLOW/FOLLOWS_STAR/PARENT/PARENT_STAR RELATIFONSHIP :
	if (type == FOLLOWS || type == FOLLOWS_STAR || type == PARENT || type == PARENT_STAR) {
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
			std::set<StmtNumber> evaluatedS;

			// get existing results from results table
			std::set<StmtNumber> currentStatements = resultsTable.getSetInt(argTwo.getStringValue());

			// check if any synonym statements follows any statements
			for (StmtSetIterator i = statements.begin(); i != statements.end(); i++) {
				for (StmtSetIterator j = currentStatements.begin(); j != currentStatements.end(); j++) {
					if (pkb->is(type, *i, *j)) {
						suchThatRelObject.setResultsBoolean(true);
						evaluatedS.insert(*j);
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
			// retrieve current statements
			std::set<StmtNumber> s1s = resultsTable.getSetInt(argOne.getStringValue());
			std::set<StmtNumber> s2s = resultsTable.getSetInt(argTwo.getStringValue());

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

			// intersect two sets x2
			std::set<StmtNumber> updatedStatements1;
			set_intersection(evaluatedS1s.begin(), evaluatedS1s.end(), s1s.begin(), s1s.end(),
				std::inserter(updatedStatements1, updatedStatements1.begin()));
			
			std::set<StmtNumber> updatedStatements2;
			set_intersection(evaluatedS2s.begin(), evaluatedS2s.end(), s2s.begin(), s2s.end(),
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
			std::set<VarName> variables = pkb->getAllVarNames();
			std::set<StmtNumber> evaluatedS;

			// get existing results from results table
			std::set<StmtNumber> currentStatements = resultsTable.getSetInt(argOne.getStringValue());

			// check if any synonym statements follows any statements
			for (StmtSetIterator cs = currentStatements.begin(); cs != currentStatements.end(); cs++) {
				for (VarNameSetIterator s = variables.begin(); s != variables.end(); s++) {
					if (pkb->is(type, *cs, pkb->getVarIndex(*s))) {
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
			// retrieve current statements
			std::set<StmtNumber> s1s = resultsTable.getSetInt(argOne.getStringValue());
			std::set<VarName> v1s = resultsTable.getSetString(argTwo.getStringValue());

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

			// intersect two sets x2
			std::set<StmtNumber> updatedStatements;
			set_intersection(evaluatedS1s.begin(), evaluatedS1s.end(), s1s.begin(), s1s.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			std::set<VarName> updatedVariables;
			set_intersection(evaluatedV1s.begin(), evaluatedV1s.end(), v1s.begin(), v1s.end(),
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
	EntityType patternType = patternObject.getPatternType();
	EntityType firstArgType = patternObject.getFirstArgumentType();
	std::string patternSynonymArg = patternObject.getPatternSynonymArgument();
	std::string firstArg = patternObject.getFirstArgument();
	std::string secondArg = patternObject.getSecondArgument();
	std::string thirdArg = patternObject.getThirdArgument();
	bool isFirstArgSynonym = patternObject.getIsFirstArgSynonym();
	bool relationshipHolds = true;
	PKB *pkb = getPKB();

	// ASSIGN PATTERN :
	if (patternType == ASSIGN) {
		// Pattern a(<firstArg>,_)
		if (secondArg == "_") {
			// Pattern a(v,_)
			if (isFirstArgSynonym) {
				// retrieve existing pattern synonym statements & first arg synonym variables
				std::set<StmtNumber> patternSynonymStatements = resultsTable.getSetInt(patternSynonymArg);
				std::set<VarName> firstArgSynonymVariables = resultsTable.getSetString(firstArg);

				// evaluated true statements
				std::set<StmtNumber> evaluatedPatternSynonymStatements;
				std::set<VarName> evaluatedfirstArgSynonymVariables;

				// check all existing pattern synonym statements if it modifies the 'variable'
				for (StmtSetIterator cs = patternSynonymStatements.begin(); cs != patternSynonymStatements.end(); cs++) {
					for (VarNameSetIterator s = firstArgSynonymVariables.begin(); s != firstArgSynonymVariables.end(); s++) {
						if (pkb->is(MODIFIES, *cs, pkb->getVarIndex(*s))) {
							evaluatedPatternSynonymStatements.insert(*cs);
							evaluatedfirstArgSynonymVariables.insert(*s);
							break;
						}
					}
				}

				// intersect two sets x2
				std::set<StmtNumber> updatedStatements;
				set_intersection(evaluatedPatternSynonymStatements.begin(), evaluatedPatternSynonymStatements.end(), patternSynonymStatements.begin(), patternSynonymStatements.end(),
					std::inserter(updatedStatements, updatedStatements.begin()));

				std::set<VarName> updatedVariables;
				set_intersection(evaluatedfirstArgSynonymVariables.begin(), evaluatedfirstArgSynonymVariables.end(), firstArgSynonymVariables.begin(), firstArgSynonymVariables.end(),
					std::inserter(updatedVariables, updatedVariables.begin()));

				// check if relationship holds/have results
				if (updatedStatements.size() > 0 || updatedVariables.size() > 0) {
					patternObject.setResultsBoolean(true);
				}

				// update the results table
				resultsTable.replaceSet(patternSynonymArg, updatedStatements);
				resultsTable.replaceSet(firstArg, updatedVariables);

			}
			// Pattern a(_,_)
			else if (firstArg == "_") {
				// check if theres any statements number inside
				if (resultsTable.getSetInt(patternSynonymArg).size() > 0) {
					patternObject.setResultsBoolean(true);
				}
			}
			// Pattern a("x",_)
			else if (firstArgType == VARIABLE) {
				// retrieve existing pattern synonym statements
				std::set<StmtNumber> currentStatements = resultsTable.getSetInt(patternSynonymArg);

				// evaluated true statements
				std::set<StmtNumber> evaluatedS;

				// check all existing pattern synonym statements if it modifies the 'variable'
				for (StmtSetIterator i = currentStatements.begin(); i != currentStatements.end(); i++) {
					// check if the existing statement modifies the 'variable'
					if (pkb->is(MODIFIES, *i, pkb->getVarIndex(firstArg))) {
						evaluatedS.insert(*i);
					}
				}

				// intersect sets
				std::set<StmtNumber> updatedStatements;
				set_intersection(evaluatedS.begin(), evaluatedS.end(), currentStatements.begin(), currentStatements.end(),
					std::inserter(updatedStatements, updatedStatements.begin()));

				// check if relationship holds/have results
				if (updatedStatements.size() > 0) {
					patternObject.setResultsBoolean(true);
				}

				// update the results table
				resultsTable.replaceSet(patternSynonymArg, updatedStatements);
			}
		} 
		// means Pattern must be Pattern a(<firstArg>,"_<constant/variable>_")	
		else { 		
			// Pattern a(v,"_<constant/variable>_")
			if (isFirstArgSynonym) {
				// retrieve existing pattern synonym statements & first arg synonym variables
				std::set<StmtNumber> patternSynonymStatements = resultsTable.getSetInt(patternSynonymArg);
				std::set<VarName> firstArgSynonymVariables = resultsTable.getSetString(firstArg);

				// evaluated true statements
				std::set<StmtNumber> evaluatedPatternSynonymStatements;
				std::set<VarName> evaluatedfirstArgSynonymVariables;

				// check all existing pattern synonym statements if it modifies the 'variable'
				for (StmtSetIterator cs = patternSynonymStatements.begin(); cs != patternSynonymStatements.end(); cs++) {
					for (VarNameSetIterator s = firstArgSynonymVariables.begin(); s != firstArgSynonymVariables.end(); s++) {
						if (pkb->is(MODIFIES, *cs, pkb->getVarIndex(*s))) {
							if (pkb->isAssignHasSubexpr(*cs, secondArg)) {
								evaluatedPatternSynonymStatements.insert(*cs);
								evaluatedfirstArgSynonymVariables.insert(*s);
								break;
							}
						}
					}
				}

				// intersect two sets x2
				std::set<StmtNumber> updatedStatements;
				set_intersection(evaluatedPatternSynonymStatements.begin(), evaluatedPatternSynonymStatements.end(), patternSynonymStatements.begin(), patternSynonymStatements.end(),
					std::inserter(updatedStatements, updatedStatements.begin()));

				std::set<VarName> updatedVariables;
				set_intersection(evaluatedfirstArgSynonymVariables.begin(), evaluatedfirstArgSynonymVariables.end(), firstArgSynonymVariables.begin(), firstArgSynonymVariables.end(),
					std::inserter(updatedVariables, updatedVariables.begin()));

				// check if relationship holds/have results
				if (updatedStatements.size() > 0 || updatedVariables.size() > 0) {
					patternObject.setResultsBoolean(true);
				}

				// update the results table
				resultsTable.replaceSet(patternSynonymArg, updatedStatements);
				resultsTable.replaceSet(firstArg, updatedVariables);

			}
			// Pattern a(_,"_<constant/variable>_")
			else if (firstArg == "_") {
				// retrieve existing pattern synonym statements & first arg synonym variables
				std::set<StmtNumber> patternSynonymStatements = resultsTable.getSetInt(patternSynonymArg);
				std::set<VarName> firstArgSynonymVariables = pkb->getAllVarNames();

				// evaluated true statements
				std::set<StmtNumber> evaluatedPatternSynonymStatements;
				std::set<VarName> evaluatedfirstArgSynonymVariables;

				// check all existing pattern synonym statements if it modifies the 'variable'
				for (StmtSetIterator cs = patternSynonymStatements.begin(); cs != patternSynonymStatements.end(); cs++) {
					for (VarNameSetIterator s = firstArgSynonymVariables.begin(); s != firstArgSynonymVariables.end(); s++) {
						if (pkb->is(MODIFIES, *cs, pkb->getVarIndex(*s))) {							
							if (pkb->isAssignHasSubexpr(*cs, secondArg)) {
								evaluatedPatternSynonymStatements.insert(*cs);
								break;
							}
							
						}
					}
				}

				// intersect sets 
				std::set<StmtNumber> updatedStatements;
				set_intersection(evaluatedPatternSynonymStatements.begin(), evaluatedPatternSynonymStatements.end(), patternSynonymStatements.begin(), patternSynonymStatements.end(),
					std::inserter(updatedStatements, updatedStatements.begin()));

				// check if relationship holds/have results
				if (updatedStatements.size() > 0 ) {
					patternObject.setResultsBoolean(true);
				}

				// update the results table
				resultsTable.replaceSet(patternSynonymArg, updatedStatements);

			}
			// Pattern a("x","_<constant/variable>_")
			else if (firstArgType == VARIABLE) {
				// retrieve existing pattern synonym statements
				std::set<StmtNumber> currentStatements = resultsTable.getSetInt(patternSynonymArg);

				// evaluated true statements
				std::set<StmtNumber> evaluatedS;

				// check all existing pattern synonym statements if it modifies the 'variable'
				for (StmtSetIterator i = currentStatements.begin(); i != currentStatements.end(); i++) {
					// check if the existing statement modifies the 'variable'
					if (pkb->is(MODIFIES, *i, pkb->getVarIndex(firstArg))) {
						// if yes, check if this statement uses the second argument subexpression
						if (pkb->isAssignHasSubexpr(*i, secondArg)) {
							evaluatedS.insert(*i);
						}		
					}
				}

				// intersect sets
				std::set<StmtNumber> updatedStatements;
				set_intersection(evaluatedS.begin(), evaluatedS.end(), currentStatements.begin(), currentStatements.end(),
					std::inserter(updatedStatements, updatedStatements.begin()));

				// check if relationship holds/have results
				if (updatedStatements.size() > 0) {
					patternObject.setResultsBoolean(true);
				}

				// update the results table
				resultsTable.replaceSet(patternSynonymArg, updatedStatements);

			}
		}
	}
	return patternObject;
}


std::vector<std::string> QueryEvaluator::evaluateSelect(SelectObject selectObject, bool relationshipHolds)
{
	// constraint results by SelectObj
	std::vector<std::string> results;

	if (relationshipHolds) {
		// if its Select BOOLEAN
		if (selectObject.getBoolean()) {
			// output : TRUE
			results.push_back("true");
		}
		// else then it must be a synonym
		else {
			// iteration 1 : only if the entity is VARIABLE, then return string (variable names)
			if (selectObject.getEntityType() == VARIABLE) {
				std::set<VarName> setResults1 = resultsTable.getSetString(selectObject.getSynonymString());
				std::vector<std::string> vectorResults1(setResults1.begin(), setResults1.end());
				return vectorResults1;
			}
			else {
				std::set<StmtNumber> setResults2 = resultsTable.getSetInt(selectObject.getSynonymString());
				std::vector<StmtNumber> vectorStmtNumbers(setResults2.begin(), setResults2.end());
				std::vector<std::string> vectorResults2;
				for (std::vector<StmtNumber>::iterator it = vectorStmtNumbers.begin(); it != vectorStmtNumbers.end(); ++it) {
					vectorResults2.push_back(std::to_string(*it));
				}
				return vectorResults2;
			}
		}
	}
	else {
		if (selectObject.getBoolean()) {
			// output : FALSE
			results.push_back("false");
		}
	}

	return results;
}