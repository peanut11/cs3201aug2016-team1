#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/QueryTable.h"
#include "../SPA/AttrType.h"
#include "../SPA/ClauseSelectObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestQueryTable) {

public:
	TEST_METHOD(TestQueryTable_Getters) {
		QueryTable queryTable;
		std::vector<ClauseSuchThatObject*> suchThats;
		std::vector<ClauseWithObject> withs;
		std::vector<ClausePatternObject> patterns;
		ClauseSuchThatObject* st1 = new ClauseSuchThatObject(MODIFIES, new ClauseSuchThatArgObject(), new ClauseSuchThatArgObject());
		ClauseSuchThatObject* st2 = new ClauseSuchThatObject(USES, new ClauseSuchThatArgObject(), new ClauseSuchThatArgObject());
		ClauseWithRefObject wr1(ATTRREF, "", AttrType::PROC_NAME);
		ClauseWithRefObject wr2(SYNONYM, "", AttrType::PROC_NAME);
		ClauseWithRefObject wr3(IDENTIFIER, "", AttrType::PROC_NAME);
		ClauseWithRefObject wr4(INTEGER, "", AttrType::PROC_NAME);
		ClauseWithObject w1(wr1, wr2);
		ClauseWithObject w2(wr3, wr4);
		ClausePatternObject p1(ASSIGN, "ab", EntityType::INVALID, false, "ab", "ab"); 
		ClausePatternObject p2(PROCEDURE, "ab", EntityType::INVALID, false, "ab", "ab");
		suchThats.push_back(st1);
		suchThats.push_back(st2);
		withs.push_back(w1);
		withs.push_back(w2);
		patterns.push_back(p1);
		patterns.push_back(p2);
		queryTable.insertSuchThatObject(st1);
		queryTable.insertSuchThatObject(st2);
		queryTable.insertWithObject(w1);
		queryTable.insertWithObject(w2);
		queryTable.insertPatternObject(p1);
		queryTable.insertPatternObject(p2);

		bool statusSuchThat = true;
		for (int i = 0; i < 2; i++) {
			if (queryTable.getSuchThats()[i]->getRelationshipType() != suchThats[i]->getRelationshipType()) {
				statusSuchThat = false;
			}
		}

		Assert::IsTrue(statusSuchThat);

		bool statusWith = true;
		for (int i = 0; i < 2; i++) {
			if (queryTable.getWiths()[i].getRefObject1().getAttrType() != withs[i].getRefObject1().getAttrType()) {
				statusWith = false;
			}
		}

		Assert::IsTrue(statusWith);

		bool statusPattern = true;
		for (int i = 0; i < 2; i++) {
			if (queryTable.getPatterns()[i].getPatternType() != patterns[i].getPatternType()) {
				statusPattern = false;
			}
		}

		Assert::IsTrue(statusPattern);
	}
	};

}