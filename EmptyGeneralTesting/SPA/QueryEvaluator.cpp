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
#include "QueryTable.h"
#include "ClauseObject.h"
#include "SelectObject.h"
#include "PKB.h"
#include "SynonymTable.h"
#include <set>
#include <iterator>

QueryEvaluator *QueryEvaluator::_instance;

/*
Singleton
*/
QueryEvaluator *QueryEvaluator::getInstance()
{
	if (!_instance)
//		_instance = new QueryEvaluator;
	return _instance;
}

PKB * QueryEvaluator::getPKB()
{
	return this->mPKB;
}

SynonymTable * QueryEvaluator::getSynonymTable()
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
		std::vector<SuchThatRelObject> suchThats = queryTable.getSuchThats();
		std::vector<WithObject> withs = queryTable.getWiths();
		std::vector<PatternObject> patterns = queryTable.getPatterns();
		SelectObject select = queryTable.getSelect();

		// iterate the clauses vectors and evaluate them
		for (std::vector<SuchThatRelObject>::iterator it = suchThats.begin(); it < suchThats.end(); it++) {
			*it = evaluateSuchThat(*it);
		}
		// iterate the clauses vectors and evaluate them
		for (std::vector<WithObject>::iterator it = withs.begin(); it < withs.end(); it++) {
			evaluateWith(*it);
		}
		// iterate the clauses vectors and evaluate them
		for (std::vector<PatternObject>::iterator it = patterns.begin(); it < patterns.end(); it++) {
			evaluatePattern(*it);
		}
	}
	catch (std::runtime_error e) {
		throw e.what();
	}
}

SuchThatRelObject QueryEvaluator::evaluateSuchThat(SuchThatRelObject suchThatRelObject)
{
	RelationshipType type = suchThatRelObject.getRelationshipType();
	SuchThatArgObject argOne = suchThatRelObject.getArgsOne();
	SuchThatArgObject argTwo = suchThatRelObject.getArgsTwo();

	// get PKB
	PKB *pkb = getPKB();

	// get Synonym Table
	SynonymTable *mSynonymTable = getSynonymTable();

	// populate Synonym Table 
	for (std::vector<SynonymObject>::iterator it = mSynonymTable->getObjects().begin(); it < mSynonymTable->getObjects().end(); it++) {
		mSynonymTable->insertSet(it->getSynonym(), pkb->getStmtsByType(it->getType()));
	}

	// FOLLOW RELATIONSHIP :
	if (type == FOLLOWS) {
		// both are statements number : Follows(3,4)
		if (argOne.getIntegerValue() > 0 && argTwo.getIntegerValue() > 0) {
			suchThatRelObject.setResultsBoolean(pkb->is(FOLLOWS, argOne.getIntegerValue(), argTwo.getIntegerValue()));
		}
		// arg1 is synonym, and arg2 is integer : Follows(s,3);Follows(a,4)
		else if (argOne.getIsSynonym() && argTwo.getIntegerValue() > 0) {
			// store results
			std::set<int> statements = pkb->getStmtsByStmt(argTwo.getIntegerValue(), FOLLOWS);     // Get stmts that arg2(integer) follows
			
			// get existing results from synonym table
			std::set<int> currentStatements = mSynonymTable->getSetInt(argOne.getStringValue());

			// intersect two sets
			std::set<int> updatedStatements;
			set_intersection(statements.begin(), statements.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// update the synonym table
			mSynonymTable->replaceSet(argOne.getStringValue(), updatedStatements);

		}
		// arg1 is integer, and arg1 is synonym : Follows(3,s);Follows(4,a)
		else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym()) {
			// store results
			std::vector<StmtNumber> statements = pkb->getStmtsByStmt(argOne.getIntegerValue(), FOLLOWED_BY); // Get stmts that arg1(integer) is followed by

			// get existing results from synonym table
			std::set<int> currentStatements = mSynonymTable->getSetInt(argTwo.getStringValue());

			// intersect two sets
			std::set<int> updatedStatements;
			set_intersection(statements.begin(), statements.end(), currentStatements.begin(), currentStatements.end(),
				std::inserter(updatedStatements, updatedStatements.begin()));

			// update the synonym table
			mSynonymTable->replaceSet(argTwo.getStringValue(), updatedStatements);
		}
		// both arguments are a synonym
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym()) {
			// get all the statements for each synonym entity type
			std::vector<StmtNumber> s1 = pkb->getStmtsByType(argOne.getEntityType());
			std::vector<StmtNumber> s2 = pkb->getStmtsByType(argTwo.getEntityType());
			std::vector<StmtNumber> evaluatedS1;
			std::vector<StmtNumber> evaluatedS2;

			// obtain evaluation results
			for (int i = 0; i < s1.size(); i++) {
				for (int j = 0; j < s2.size(); j++) {
					if (pkb->is(FOLLOWS, s1[i], s2[j])) {
						evaluatedS1.push_back(s1[i]);
						evaluatedS2.push_back(s2[j]);
					}
				}
			}

			// retrieve current statements
			std::set<int> currentStatements1 = mSynonymTable->getSetInt(argOne.getStringValue());
			std::set<int> currentStatements2 = mSynonymTable->getSetInt(argTwo.getStringValue());

			// intersect two sets x2
			std::set<int> updatedStatements1;
			set_intersection(evaluatedS1.begin(), evaluatedS1.end(), currentStatements1.begin(), currentStatements1.end(),
				std::inserter(updatedStatements1, updatedStatements1.begin()));
			
			std::set<int> updatedStatements2;
			set_intersection(evaluatedS2.begin(), evaluatedS2.end(), currentStatements2.begin(), currentStatements2.end(),
				std::inserter(updatedStatements2, updatedStatements2.begin()));

			// update the synonm table
			mSynonymTable->replaceSet(argOne.getStringValue(), updatedStatements1);
			mSynonymTable->replaceSet(argTwo.getStringValue(), updatedStatements2);
		}

		return suchThatRelObject;

	}
	// iteration 1 : only allow 'statement' at left instead of 'procedure' too
	else if (type == MODIFIES) {
		// if left arg is 'statement number', right arg is "x" (Modifies(3,"x"))
		if (argOne.getIntegerValue() > 0 &&  argTwo.getIsSynonym == false && argTwo.getStringValue().length > 0) {
				
		}
	}
}

void QueryEvaluator::evaluateWith(WithObject withObject)
{
}

void QueryEvaluator::evaluatePattern(PatternObject patternObject)
{
}

void QueryEvaluator::evaluateSelect(SelectObject selectObject)
{
}
