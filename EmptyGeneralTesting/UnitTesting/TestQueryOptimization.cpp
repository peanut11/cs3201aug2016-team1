#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryValidator.h"
#include "../SPA/QueryProcessor.h"
#include "../SPA/QueryOptimization.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

	TEST_CLASS(TestQueryOptimization) {

		TEST_METHOD(TestQueryOptimization_Check_Boolean) {
			//QueryValidator *validator = QueryValidator::getInstance();
			ClauseSuchThatObject suchThatObj;


			QueryOptimization optimize = QueryOptimization();

			// Follows(2, 4)
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS, 
				ClauseSuchThatArgObject(EntityType::CONSTANT, "", 2, false), 
				ClauseSuchThatArgObject(EntityType::CONSTANT, "", 4, false));
			Assert::IsTrue(optimize.isClauseReturnBoolean(suchThatObj));

			// Follows(_, _)
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS,
				ClauseSuchThatArgObject(EntityType::WILDCARD, "", 0, false),
				ClauseSuchThatArgObject(EntityType::WILDCARD, "", 0, false));
			Assert::IsTrue(optimize.isClauseReturnBoolean(suchThatObj));

			// Follows(_, 4)
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS,
				ClauseSuchThatArgObject(EntityType::WILDCARD, "", 0, false),
				ClauseSuchThatArgObject(EntityType::CONSTANT, "", 4, false));
			Assert::IsTrue(optimize.isClauseReturnBoolean(suchThatObj));

			// Follows(2, _)
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS,
				ClauseSuchThatArgObject(EntityType::CONSTANT, "", 2, false),
				ClauseSuchThatArgObject(EntityType::WILDCARD, "", 0, false));
			Assert::IsTrue(optimize.isClauseReturnBoolean(suchThatObj));

			// Uses(5, "y")
			suchThatObj = ClauseSuchThatObject(RelationshipType::USES,
				ClauseSuchThatArgObject(EntityType::CONSTANT, "", 5, false),
				ClauseSuchThatArgObject(EntityType::VARIABLE, "y", 0, false));
			Assert::IsTrue(optimize.isClauseReturnBoolean(suchThatObj));

			// pattern a1(_, _)
			suchThatObj = ClauseSuchThatObject(RelationshipType::USES,
				ClauseSuchThatArgObject(EntityType::CONSTANT, "", 5, false),
				ClauseSuchThatArgObject(EntityType::VARIABLE, "y", 0, false));
			Assert::IsTrue(optimize.isClauseReturnBoolean(suchThatObj));



			// Failure, contains synonym
			// Follows(p, _)
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS,
				ClauseSuchThatArgObject(EntityType::PROCEDURE, "p", 0, true),
				ClauseSuchThatArgObject(EntityType::WILDCARD, "", 0, false));
			Assert::IsFalse(optimize.isClauseReturnBoolean(suchThatObj));

			// Uses(s1, "y")
			suchThatObj = ClauseSuchThatObject(RelationshipType::USES,
				ClauseSuchThatArgObject(EntityType::STMT, "s1", 0, true),
				ClauseSuchThatArgObject(EntityType::VARIABLE, "y", 0, false));
			Assert::IsFalse(optimize.isClauseReturnBoolean(suchThatObj));

			// Uses(p, "y")
			suchThatObj = ClauseSuchThatObject(RelationshipType::USES,
				ClauseSuchThatArgObject(EntityType::PROCEDURE, "p", 0, true),
				ClauseSuchThatArgObject(EntityType::VARIABLE, "y", 0, false));
			Assert::IsFalse(optimize.isClauseReturnBoolean(suchThatObj));


		}

		TEST_METHOD(TestQueryOptimization_Affect_Result) {
			ClauseSuchThatObject suchThatObj;
			ClausePatternObject patternObj;
			ClauseWithObject withObj;

			ClauseSelectObject selectObj1 = ClauseSelectObject(EntityType::STMT, "s1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj2 = ClauseSelectObject(EntityType::PROCEDURE, "p", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj3 = ClauseSelectObject(EntityType::VARIABLE, "var1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj4 = ClauseSelectObject(EntityType::VARIABLE, "a1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj5 = ClauseSelectObject(EntityType::VARIABLE, "w1", AttrType::AttrType::INVALID, false);


			QueryTable mQueryTable = QueryTable();
			mQueryTable.insertSelectObject(selectObj1);
			mQueryTable.insertSelectObject(selectObj2);
			mQueryTable.insertSelectObject(selectObj3);
			mQueryTable.insertSelectObject(selectObj4);
			mQueryTable.insertSelectObject(selectObj5);

			QueryOptimization optimize = QueryOptimization();

			// Follows(p, _)
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS,
				ClauseSuchThatArgObject(EntityType::PROCEDURE, "p", 0, true),
				ClauseSuchThatArgObject(EntityType::WILDCARD, "", 0, false));
			Assert::IsTrue(optimize.isClauseAffectsResult(mQueryTable, suchThatObj));

			// Uses(s1, var1)
			suchThatObj = ClauseSuchThatObject(RelationshipType::USES,
				ClauseSuchThatArgObject(EntityType::STMT, "s1", 0, true),
				ClauseSuchThatArgObject(EntityType::VARIABLE, "var1", 0, true));
			Assert::IsTrue(optimize.isClauseAffectsResult(mQueryTable, suchThatObj));

			// Pattern a1(_,_)
			patternObj = ClausePatternObject(EntityType::ASSIGN, 
				"a1", EntityType::WILDCARD, false, "_", "_");
			Assert::IsTrue(optimize.isClauseAffectsResult(mQueryTable, patternObj));

			// Pattern w1(_,_,_)
			patternObj = ClausePatternObject(EntityType::WHILE,
				"w1", EntityType::WILDCARD, false, "_", "_", "_");
			Assert::IsTrue(optimize.isClauseAffectsResult(mQueryTable, patternObj));

			// Pattern ifstmt(var1,_)
			patternObj = ClausePatternObject(EntityType::IF,
				"ifstmt", EntityType::VARIABLE, true, "var1", "_");
			Assert::IsTrue(optimize.isClauseAffectsResult(mQueryTable, patternObj));

			// Pattern ifstmt(var1,_"x+y+z"_)
			patternObj = ClausePatternObject(EntityType::IF,
				"ifstmt", EntityType::VARIABLE, true, "var1", "_\"x+y+z\"_");
			Assert::IsTrue(optimize.isClauseAffectsResult(mQueryTable, patternObj));

			// with a1.stmt#=20 - either left or right contains related synonym will consider true
			withObj = ClauseWithObject(
				ClauseWithRefObject(WithRefType::SYNONYM, "a1", AttrType::AttrType::STMT_NO), 
				ClauseWithRefObject(WithRefType::INTEGER, 20));
			Assert::IsTrue(optimize.isClauseAffectsResult(mQueryTable, withObj));

			// with a1.stmt#=const.value - either left or right contains related synonym will consider true
			withObj = ClauseWithObject(
				ClauseWithRefObject(WithRefType::SYNONYM, "a1", AttrType::AttrType::STMT_NO),
				ClauseWithRefObject(WithRefType::SYNONYM, "const", AttrType::AttrType::VALUE));
			Assert::IsTrue(optimize.isClauseAffectsResult(mQueryTable, withObj));



			// Failure, such that clauses not related to result clauses
			// Affects(a10, a12)
			suchThatObj = ClauseSuchThatObject(RelationshipType::AFFECTS,
				ClauseSuchThatArgObject(EntityType::ASSIGN, "a10", 0, true),
				ClauseSuchThatArgObject(EntityType::ASSIGN, "a12", 0, true));
			Assert::IsFalse(optimize.isClauseAffectsResult(mQueryTable, suchThatObj));

			// Failure, pattern clauses not related to result clauses
			// pattern ifstmt(_, _)
			patternObj = ClausePatternObject(EntityType::IF,
				"ifstmt", EntityType::WILDCARD, false, "_", "_");
			Assert::IsFalse(optimize.isClauseAffectsResult(mQueryTable, patternObj));
			
			// pattern ifstmt("x", "x+y")
			patternObj = ClausePatternObject(EntityType::IF,
				"ifstmt", EntityType::VARIABLE, false, "x", "x+y");
			Assert::IsFalse(optimize.isClauseAffectsResult(mQueryTable, patternObj));

			// Failure with clauses not related to result clauses
			// with a2.stmt#=20
			withObj = ClauseWithObject(
				ClauseWithRefObject(WithRefType::SYNONYM, "a2", AttrType::AttrType::STMT_NO),
				ClauseWithRefObject(WithRefType::INTEGER, 20));
			Assert::IsFalse(optimize.isClauseAffectsResult(mQueryTable, withObj));

			// with a2.stmt#=const.value
			withObj = ClauseWithObject(
				ClauseWithRefObject(WithRefType::SYNONYM, "a2", AttrType::AttrType::STMT_NO),
				ClauseWithRefObject(WithRefType::SYNONYM, "const", AttrType::AttrType::VALUE));
			Assert::IsFalse(optimize.isClauseAffectsResult(mQueryTable, withObj));


		}

		TEST_METHOD(TestQueryOptimization_Such_That_Clause) {
			ClauseSuchThatObject suchThatObj;
			ClausePatternObject patternObj;
			ClauseWithObject withObj;

			ClauseSelectObject selectObj1 = ClauseSelectObject(EntityType::STMT, "s1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj2 = ClauseSelectObject(EntityType::PROCEDURE, "p", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj3 = ClauseSelectObject(EntityType::VARIABLE, "var1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj4 = ClauseSelectObject(EntityType::VARIABLE, "a1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj5 = ClauseSelectObject(EntityType::VARIABLE, "w1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj6 = ClauseSelectObject(EntityType::PROGRAM_LINE, "n1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj7 = ClauseSelectObject(EntityType::ASSIGN, "a1", AttrType::AttrType::INVALID, false);


			QueryTable mQueryTable = QueryTable();
			mQueryTable.insertSelectObject(selectObj1);
			mQueryTable.insertSelectObject(selectObj2);
			mQueryTable.insertSelectObject(selectObj3);
			mQueryTable.insertSelectObject(selectObj4);
			mQueryTable.insertSelectObject(selectObj5);
			mQueryTable.insertSelectObject(selectObj6);
			mQueryTable.insertSelectObject(selectObj7);

			QueryOptimization optimize = QueryOptimization();

			// Follows(3, 4) => expect type BOOLEAN
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS,
				ClauseSuchThatArgObject(EntityType::CONSTANT, "", 3, false),
				ClauseSuchThatArgObject(EntityType::CONSTANT, "", 4, false));
			Assert::IsTrue(GroupType::GroupType::BOOLEAN == optimize.getGroupType(mQueryTable, suchThatObj));
		
			// Follows(_, _) => expect type BOOLEAN
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS,
				ClauseSuchThatArgObject(EntityType::WILDCARD, "_", 0, false),
				ClauseSuchThatArgObject(EntityType::WILDCARD, "_", 0, false));
			Assert::IsTrue(GroupType::GroupType::BOOLEAN == optimize.getGroupType(mQueryTable, suchThatObj));

			// Modifies(a3, v3) => expect type NOT_RELATED
			suchThatObj = ClauseSuchThatObject(RelationshipType::MODIFIES,
				ClauseSuchThatArgObject(EntityType::ASSIGN, "a3", 0, true),
				ClauseSuchThatArgObject(EntityType::VARIABLE, "v3", 0, true));
			Assert::IsTrue(GroupType::GroupType::NOT_RELATED == optimize.getGroupType(mQueryTable, suchThatObj));

			// Follows(s1, _) => expect type RELATED
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS,
				ClauseSuchThatArgObject(EntityType::STMT, "s1", 0, true),
				ClauseSuchThatArgObject(EntityType::WILDCARD, "_", 0, false));
			Assert::IsTrue(GroupType::GroupType::RELATED == optimize.getGroupType(mQueryTable, suchThatObj));

			// Follows(_, s1) => expect type RELATED
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS,
				ClauseSuchThatArgObject(EntityType::WILDCARD, "_", 0, false),
				ClauseSuchThatArgObject(EntityType::STMT, "s1", 0, true));
			Assert::IsTrue(GroupType::GroupType::RELATED == optimize.getGroupType(mQueryTable, suchThatObj));

			// Follows(s1, s2) => expect type RELATED
			suchThatObj = ClauseSuchThatObject(RelationshipType::FOLLOWS,
				ClauseSuchThatArgObject(EntityType::STMT, "s1", 0, true),
				ClauseSuchThatArgObject(EntityType::STMT, "s2", 0, true));
			Assert::IsTrue(GroupType::GroupType::RELATED == optimize.getGroupType(mQueryTable, suchThatObj));

			// Next(n1, n2) => expect type OTHERS
			suchThatObj = ClauseSuchThatObject(RelationshipType::NEXT,
				ClauseSuchThatArgObject(EntityType::PROGRAM_LINE, "n1", 0, true),
				ClauseSuchThatArgObject(EntityType::PROGRAM_LINE, "n2", 0, true));
			Assert::IsTrue(GroupType::GroupType::OTHERS == optimize.getGroupType(mQueryTable, suchThatObj));

			// Next(_, _) => expect type BOOLEAN
			suchThatObj = ClauseSuchThatObject(RelationshipType::NEXT,
				ClauseSuchThatArgObject(EntityType::WILDCARD, "_", 0, false),
				ClauseSuchThatArgObject(EntityType::WILDCARD, "_", 0, false));
			Assert::IsTrue(GroupType::GroupType::BOOLEAN == optimize.getGroupType(mQueryTable, suchThatObj));

			// Next(n10, n11) => expect type NOT_RELATED
			suchThatObj = ClauseSuchThatObject(RelationshipType::NEXT,
				ClauseSuchThatArgObject(EntityType::PROGRAM_LINE, "n10", 0, true),
				ClauseSuchThatArgObject(EntityType::PROGRAM_LINE, "n11", 0, true));
			Assert::IsTrue(GroupType::GroupType::NOT_RELATED == optimize.getGroupType(mQueryTable, suchThatObj));


			// Next*(n1, n2) => expect type OTHERS
			suchThatObj = ClauseSuchThatObject(RelationshipType::NEXT_STAR,
				ClauseSuchThatArgObject(EntityType::PROGRAM_LINE, "n1", 0, true),
				ClauseSuchThatArgObject(EntityType::PROGRAM_LINE, "n2", 0, true));
			Assert::IsTrue(GroupType::GroupType::OTHERS == optimize.getGroupType(mQueryTable, suchThatObj));

			// Affects(a1, a2) => expect type OTHERS
			suchThatObj = ClauseSuchThatObject(RelationshipType::AFFECTS,
				ClauseSuchThatArgObject(EntityType::ASSIGN, "a1", 0, true),
				ClauseSuchThatArgObject(EntityType::ASSIGN, "a2", 0, true));
			Assert::IsTrue(GroupType::GroupType::OTHERS == optimize.getGroupType(mQueryTable, suchThatObj));

			// Affects*(a1, a2) => expect type OTHERS
			suchThatObj = ClauseSuchThatObject(RelationshipType::AFFECTS_STAR,
				ClauseSuchThatArgObject(EntityType::ASSIGN, "a1", 0, true),
				ClauseSuchThatArgObject(EntityType::ASSIGN, "a2", 0, true));
			Assert::IsTrue(GroupType::GroupType::OTHERS == optimize.getGroupType(mQueryTable, suchThatObj));


		}

		TEST_METHOD(TestQueryOptimization_Pattern_Clause) {
			ClauseSuchThatObject suchThatObj;
			ClausePatternObject patternObj;
			ClauseWithObject withObj;

			ClauseSelectObject selectObj1 = ClauseSelectObject(EntityType::STMT, "s1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj2 = ClauseSelectObject(EntityType::PROCEDURE, "p", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj3 = ClauseSelectObject(EntityType::VARIABLE, "var1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj4 = ClauseSelectObject(EntityType::VARIABLE, "a1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj5 = ClauseSelectObject(EntityType::VARIABLE, "w1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj6 = ClauseSelectObject(EntityType::PROGRAM_LINE, "n1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj7 = ClauseSelectObject(EntityType::ASSIGN, "a1", AttrType::AttrType::INVALID, false);


			QueryTable mQueryTable = QueryTable();
			mQueryTable.insertSelectObject(selectObj1);
			mQueryTable.insertSelectObject(selectObj2);
			mQueryTable.insertSelectObject(selectObj3);
			mQueryTable.insertSelectObject(selectObj4);
			mQueryTable.insertSelectObject(selectObj5);
			mQueryTable.insertSelectObject(selectObj6);
			mQueryTable.insertSelectObject(selectObj7);

			QueryOptimization optimize = QueryOptimization();

			// Pattern a10(_,_) => expect type BOOLEAN
			patternObj = ClausePatternObject(EntityType::ASSIGN,
				"a10", EntityType::WILDCARD, false, "_", "_");
			Assert::IsTrue(GroupType::GroupType::BOOLEAN == optimize.getGroupType(mQueryTable, patternObj));
			
			// Pattern a10(var2,_) => expect type NOT_RELATED
			patternObj = ClausePatternObject(EntityType::ASSIGN,
				"a10", EntityType::VARIABLE, true, "var2", "_");
			Assert::IsTrue(GroupType::GroupType::NOT_RELATED == optimize.getGroupType(mQueryTable, patternObj));

			// Pattern a1(_,_) => expect type RELATED
			patternObj = ClausePatternObject(EntityType::ASSIGN,
				"a1", EntityType::WILDCARD, false, "_", "_");
			Assert::IsTrue(GroupType::GroupType::RELATED == optimize.getGroupType(mQueryTable, patternObj));

		}

		TEST_METHOD(TestQueryOptimization_With_Clause) {
			ClauseSuchThatObject suchThatObj;
			ClausePatternObject patternObj;
			ClauseWithObject withObj;

			ClauseSelectObject selectObj1 = ClauseSelectObject(EntityType::STMT, "s1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj2 = ClauseSelectObject(EntityType::PROCEDURE, "p", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj3 = ClauseSelectObject(EntityType::VARIABLE, "var1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj4 = ClauseSelectObject(EntityType::VARIABLE, "a1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj5 = ClauseSelectObject(EntityType::VARIABLE, "w1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj6 = ClauseSelectObject(EntityType::PROGRAM_LINE, "n1", AttrType::AttrType::INVALID, false);
			ClauseSelectObject selectObj7 = ClauseSelectObject(EntityType::ASSIGN, "a1", AttrType::AttrType::INVALID, false);


			QueryTable mQueryTable = QueryTable();
			mQueryTable.insertSelectObject(selectObj1);
			mQueryTable.insertSelectObject(selectObj2);
			mQueryTable.insertSelectObject(selectObj3);
			mQueryTable.insertSelectObject(selectObj4);
			mQueryTable.insertSelectObject(selectObj5);
			mQueryTable.insertSelectObject(selectObj6);
			mQueryTable.insertSelectObject(selectObj7);

			QueryOptimization optimize = QueryOptimization();

			// with a2.stmt#=const.value  => expect type WITH_NOT_RELATED
			withObj = ClauseWithObject(
				ClauseWithRefObject(WithRefType::SYNONYM, "a2", AttrType::AttrType::STMT_NO),
				ClauseWithRefObject(WithRefType::SYNONYM, "const", AttrType::AttrType::VALUE));
			Assert::IsTrue(GroupType::GroupType::WITH_NOT_RELATED == optimize.getGroupType(mQueryTable, withObj));

			// with a2.stmt#=10  => expect type WITH_NOT_RELATED
			withObj = ClauseWithObject(
				ClauseWithRefObject(WithRefType::SYNONYM, "a2", AttrType::AttrType::STMT_NO),
				ClauseWithRefObject(WithRefType::INTEGER, 10));
			Assert::IsTrue(GroupType::GroupType::WITH_NOT_RELATED == optimize.getGroupType(mQueryTable, withObj));

			// with a1.stmt#=const.value  => expect type RELATED
			withObj = ClauseWithObject(
				ClauseWithRefObject(WithRefType::SYNONYM, "a1", AttrType::AttrType::STMT_NO),
				ClauseWithRefObject(WithRefType::SYNONYM, "const", AttrType::AttrType::VALUE));
			Assert::IsTrue(GroupType::GroupType::RELATED == optimize.getGroupType(mQueryTable, withObj));


		}
	};

	

}