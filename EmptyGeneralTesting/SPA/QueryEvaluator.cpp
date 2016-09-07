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

/*
Evaluate QueryTree
*/
void QueryEvaluator::evaluate(QueryTable queryTable) {
	try {

		// do the evaluation (Basic Query Evaluator BQE) 
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

	// FOLLOW RELATIONSHIP :
	if (type == FOLLOWS) {
		// both are statements number : Follows(3,4)
		if (argOne.getIntegerValue() > 0 && argTwo.getIntegerValue() > 0) {
			suchThatRelObject.setResultsBoolean(pkb->is(FOLLOWS, argOne.getIntegerValue(), argTwo.getIntegerValue()));
		}
		// arg1 is synonym, and arg2 is integer : Follows(s,3);Follows(a,4)
		else if (argOne.getIsSynonym() && argTwo.getIntegerValue() > 0) {
			std::vector<StmtNumber> statements = pkb->getStmtsByStmt(argTwo.getIntegerValue(), FOLLOWS);     // Get stmts that arg2(integer) follows
			// filter by synonym by finding in Synonym Table
			// populate the 'statements' with the current entity statements (set intersection)

		}
		// arg1 is integer, and arg1 is synonym : Follows(3,s);Follows(4,a)
		else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym()) {
			std::vector<StmtNumber> statements = pkb->getStmtsByStmt(argOne.getIntegerValue(), FOLLOWED_BY); // Get stmts that arg1(integer) is followed by
			// filter by synonym by finding in Synonym Table
			// populate the 'statements' with the current entity statements (set intersection)
		}
		// both arguments are a synonym
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym()) {

		}

		return suchThatRelObject;

	}
	else if (type == MODIFIES) {

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
