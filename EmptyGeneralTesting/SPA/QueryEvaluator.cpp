// Maintained by: Joseph
//
// Accessed by:
// - QueryProcessor
//
// Uses:
// QueryTable
// ClauseObject
// ClauseSelectObject

#include "QueryEvaluator.h"

QueryEvaluator* QueryEvaluator::_instance = nullptr;

QueryEvaluator* QueryEvaluator::getInstance() {
    if (_instance == nullptr) {
        _instance = new QueryEvaluator();
    }
    return _instance;
}

VarName QueryEvaluator::to_var_name(VarIndex varIndex) {
    // return PKB::getInstance()->getVarName(varIndex);              // Original
    return QueryEvaluator::getInstance()->pkb->getVarName(varIndex); // DummyPKB
}

QueryEvaluator::QueryEvaluator() {
    pkb = PKB::getInstance();
    synonymTable = SynonymTable::getInstance();
    resultManager = new ResultGridManager();
}

PKB* QueryEvaluator::getPKB() {
    return this->pkb;
}

void QueryEvaluator::setPKB(PKB *pkb) {
    this->pkb = pkb;
}

std::set<VarName> QueryEvaluator::getValuesForSynonym(SynonymString syn) {
    std::set<VarIndex> indexSet = resultManager->getValuesForSynonym(syn);
    std::set<VarName> nameSet;
    std::transform(indexSet.begin(), indexSet.end(), std::inserter(nameSet, nameSet.begin()), to_var_name);
    return nameSet;
}

std::vector<std::string> QueryEvaluator::evaluate(QueryTable queryTable) {
    // Get new instance for new query
    delete resultManager;
    resultManager = new ResultGridManager();

    try {
        // Get select object and all clause objects
        ClauseSelectObject select = queryTable.getSelect();
        std::vector<ClausePatternObject> patterns = queryTable.getPatterns();
        std::vector<ClauseSuchThatObject> suchThats = queryTable.getSuchThats();
        std::vector<ClauseWithObject> withs = queryTable.getWiths();

        // Populate result grids
        populateResultGrids();

        // Boolean status on relationships holds
        bool relationshipHolds = true;
/*
		// group queries into optimized groups
		std::vector<std::vector<ClauseObject>> groupQueries = QueryOptimizer.beginGroup(queryTable);

		// evaluate the optimized groups
		for (int i = 0; i < groupQueries.size(); i++) {
			// check what group type

			// check whether group need to optimized

			// evaluate this group

			// this group : check what clause is this

			// this group : evaluate this clause

		}
*/
        for (int i = 0; i < 2; i++) {
            // Iterate the such that clauses vectors and evaluate them
            for (std::vector<ClauseSuchThatObject>::iterator it = suchThats.begin(); it != suchThats.end(); it++) {
                ClauseSuchThatObject suchThatObject = evaluateSuchThat(*it);
                relationshipHolds = relationshipHolds && suchThatObject.getResultsBoolean();
            }

            // Iterate the with clauses vectors and evaluate them
            for (std::vector<ClauseWithObject>::iterator it = withs.begin(); it != withs.end(); it++) {
                ClauseWithObject withObject = evaluateWith(*it);
                relationshipHolds = relationshipHolds && withObject.getResultsBoolean();
            }

            // Iterate the pattern clauses vectors and evaluate them
            for (std::vector<ClausePatternObject>::iterator it = patterns.begin(); it != patterns.end(); it++) {
                ClausePatternObject patternObject = evaluatePattern(*it);
                relationshipHolds = relationshipHolds && patternObject.getResultsBoolean();
            }
        }

        // Evaluate results by constraint of select object
        return evaluateSelect(select, relationshipHolds);

    } catch (std::runtime_error e) {
        throw e.what();
    }
}

ResultGridManager* QueryEvaluator::populateResultGrids() {
    // Get synonym table & list of synonyms
    std::vector<SynonymObject> synonymObjects = synonymTable->getObjects();

    // Populate result grids
    for (std::vector<SynonymObject>::iterator it = synonymObjects.begin(); it != synonymObjects.end(); it++) {
        SynonymString syn = it->getSynonym();
        if (it->getType() == VARIABLE) {
            resultManager->initialiseSynonym(syn, pkb->getAllVarIndex());
        } else {
            resultManager->initialiseSynonym(syn, pkb->getStmtsByType(it->getType()));
        }
    }
    return resultManager;
}

ClauseSuchThatObject QueryEvaluator::evaluateSuchThat(ClauseSuchThatObject suchThatRelObject) {
    RelationshipType type = suchThatRelObject.getRelationshipType();
    ClauseSuchThatArgObject argOne = suchThatRelObject.getArgsOne();
    ClauseSuchThatArgObject argTwo = suchThatRelObject.getArgsTwo();
    bool relationshipHolds = true;

    // FOLLOW / FOLLOWS_STAR / PARENT / PARENT_STAR / NEXT / NEXT_STAR relationship
    if (type == FOLLOWS || type == FOLLOWS_STAR || type == PARENT || type == PARENT_STAR || type == NEXT || type == NEXT_STAR) {
        // Both are statement numbers: Follows(3,4)
        if (argOne.getIntegerValue() > 0 && argTwo.getIntegerValue() > 0) {
            suchThatRelObject.setResultsBoolean(pkb->is(type, argOne.getIntegerValue(), argTwo.getIntegerValue()));
        }
        // arg1 is synonym, arg2 is integer: Follows(s,3); Follows(a,4)
        else if (argOne.getIsSynonym() && argTwo.getIntegerValue() > 0) {
            // Store results
            std::set<StmtNumber> statements = pkb->getStmtsByStmt(argTwo.getIntegerValue(), type);     // Get stmts that arg2(integer) follows

            // Check if relationship holds/have results
            if (statements.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }

            // Update the results table
            resultManager->updateSynonym(argOne.getStringValue(), statements);
        }
        // arg1 is integer, and arg1 is synonym: Follows(3,s);Follows(4,a)
        else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym()) {
            // Store results
            std::set<StmtNumber> statements = pkb->getStmtsByStmt(type, argOne.getIntegerValue()); // Get stmts that arg1(integer) is followed by

            // Check if relationship holds/have results
            if (statements.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }

            // Update the results table
            resultManager->updateSynonym(argTwo.getStringValue(), statements);
        }
        // arg1 is underscore & arg2 is integer: Follows(_,10);
        else if (argOne.getIsSynonym() == false && argOne.getStringValue() == "_" && argTwo.getIntegerValue() > 0) {
            // Store results
            std::set<StmtNumber> statements = pkb->getStmtsByStmt(argTwo.getIntegerValue(), type);     // Get stmts that arg2(integer) follows

            // Check if relationship holds
            if (statements.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }
        }
        // arg1 is underscore & arg2 is synonym: Follows(_,s);
        else if (argOne.getIsSynonym() == false && argOne.getStringValue() == "_" && argTwo.getIsSynonym()) {
            // Set "_" to retrieve all statements and also get all statements of synonym
            std::set<StmtNumber> statements = pkb->getAllStmts();
            std::set<StmtNumber> evaluatedS;

            // Get existing results from results table
            std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(argTwo.getStringValue());

            // Check if any synonym statements follows any statements
            for (StmtSetIterator i = statements.begin(); i != statements.end(); i++) {
                for (StmtSetIterator j = currentStatements.begin(); j != currentStatements.end(); j++) {
                    if (pkb->is(type, *i, *j)) {
                        suchThatRelObject.setResultsBoolean(true);
                        evaluatedS.insert(*j);
                    }
                }
            }

            // Check if relationship holds/have results
            if (evaluatedS.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }

            // Update the results table
            resultManager->updateSynonym(argTwo.getStringValue(), evaluatedS);

        }
        // arg1 is integer & arg2 is underscore: Follows(3,_);
        else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
            // Store results
            std::set<StmtNumber> statements = pkb->getStmtsByStmt(type, argOne.getIntegerValue());     // Get stmts that arg2(integer) follows

            // Check if relationship holds
            if (statements.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }
        }
        // arg1 is synonym & arg2 is underscore: Follows(s,_);
        else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
            // Set "_" to retrieve all statements and also get all statements of synonym
            std::set<StmtNumber> statements = pkb->getAllStmts();
            std::set<StmtNumber> evaluatedS;

            // Get existing results from results table
            std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(argOne.getStringValue());

            // Check if any synonym statements follows any statements
            for (StmtSetIterator cs = currentStatements.begin(); cs != currentStatements.end(); cs++) {
                for (StmtSetIterator s = statements.begin(); s != statements.end(); s++) {
                    if (pkb->is(type, *cs, *s)) {
                        suchThatRelObject.setResultsBoolean(true);
                        evaluatedS.insert(*cs);
                    }
                }
            }

            // Check if relationship holds/have results
            if (evaluatedS.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }

            // Update the results table
            resultManager->updateSynonym(argOne.getStringValue(), evaluatedS);
        }
        // Both arguments are a synonym: Follows(s1,s2); Follow(a,s1);
        else if (argOne.getIsSynonym() && argTwo.getIsSynonym()) {
            // Retrieve current statements
            std::set<StmtNumber> s1s = resultManager->getValuesForSynonym(argOne.getStringValue());
            std::set<StmtNumber> s2s = resultManager->getValuesForSynonym(argTwo.getStringValue());

            std::set<StmtNumber> evaluatedS1s;
            std::set<StmtNumber> evaluatedS2s;

            // Obtain evaluation results
            for (StmtSetIterator s1 = s1s.begin(); s1 != s1s.end(); s1++) {
                for (StmtSetIterator s2 = s2s.begin(); s2 != s2s.end(); s2++) {
                    if (pkb->is(type, *s1, *s2)) {
                        evaluatedS1s.insert(*s1);
                        evaluatedS2s.insert(*s2);
                    }
                }
            }

            // Intersect two sets x2
            std::set<StmtNumber> updatedStatements1;
            set_intersection(evaluatedS1s.begin(), evaluatedS1s.end(), s1s.begin(), s1s.end(),
                             std::inserter(updatedStatements1, updatedStatements1.begin()));

            std::set<StmtNumber> updatedStatements2;
            set_intersection(evaluatedS2s.begin(), evaluatedS2s.end(), s2s.begin(), s2s.end(),
                             std::inserter(updatedStatements2, updatedStatements2.begin()));

            // Check if relationship holds/have results
            if (updatedStatements1.size() > 0 || updatedStatements2.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }

            // Update the results table
            resultManager->updateSynonym(argOne.getStringValue(), updatedStatements1);
            resultManager->updateSynonym(argTwo.getStringValue(), updatedStatements2);
        }
		// arg1 is underscore & arg2 is underscore: Follows(_,_);
		else if (argOne.getIsSynonym() == false && argOne.getStringValue() == "_" && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
			// Retrieve wild card statements
			std::set<StmtNumber> statements1 = pkb->getAllStmts();
			std::set<StmtNumber> statements2 = pkb->getAllStmts();

			// Obtain evaluation results
			for (StmtSetIterator s1s = statements1.begin(); s1s != statements1.end(); s1s++) {
				for (StmtSetIterator s2s = statements2.begin(); s2s != statements2.end(); s2s++) {
					if (pkb->is(type, *s1s, *s2s)) {
						suchThatRelObject.setResultsBoolean(true);
						break;
					}
				}
				if (suchThatRelObject.getResultsBoolean() == true) {
					break;
				}
			}
		}
    }

    // Iteration 1: only allow 'statement' at left instead of 'procedure' too
    // MODIFIES / USES relationship:

    else if (type == MODIFIES || type == USES) {
        // If left arg is 'statement number', right arg is "x" (Modifies(3,"x"))
        if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym() == false && argTwo.getStringValue() != "_") {
            suchThatRelObject.setResultsBoolean(pkb->is(type, argOne.getIntegerValue(), pkb->getVarIndex(argTwo.getStringValue())));
        }
        // If left arg is 'statement number', right arg is a synonym (Modifies(3,v))
        else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym()) {
            // Get results
            std::set<VarIndex> variableIndexes = pkb->getVarsByStmt(argOne.getIntegerValue(), type);

            // Check if relationship holds/have results
            if (variableIndexes.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }

            // Update the results table
            resultManager->updateSynonym(argTwo.getStringValue(), variableIndexes);
        }
        // If left arg is 'statement number', right arg is "_" (Modifies(3,_);
        else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
            // Store results
            std::set<VarIndex> variables = pkb->getVarsByStmt(argOne.getIntegerValue(), type);     // Get vars that arg1(integer) follows

            // Check if relationship holds
            if (variables.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }
        }
        // If left arg is synonym, right arg is "x" (Modifies(s,"x")); Modifies(a,"x")
        else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue() != "_") {
            // Get all statements that modifies variable
			std::set<StmtNumber> statements = pkb->getStmtsByVar(type, argTwo.getStringValue());

            // Check if relationship holds/have results
            if (statements.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }

            // Update the synonym table
            resultManager->updateSynonym(argOne.getStringValue(), statements);
        }
        // If left arg is synonym, right arg is "_" (Modifies(s,_);
        else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
            // Set "_" to retrieve all statements and also get all statements of synonym
            std::set<VarIndex> variables = pkb->getAllVarIndex();
            std::set<StmtNumber> evaluatedS;

            // Get existing results from results table
            std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(argOne.getStringValue());

            // Check if any synonym statements follows any statements
            for (StmtSetIterator cs = currentStatements.begin(); cs != currentStatements.end(); cs++) {
                for (VarIndexSetIterator s = variables.begin(); s != variables.end(); s++) {
                    if (pkb->is(type, *cs, *s)) {
                        evaluatedS.insert(*cs);
                    }
                }
            }

            // Check if relationship holds/have results
            if (evaluatedS.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }

            // Update the results table
            resultManager->updateSynonym(argOne.getStringValue(), evaluatedS);
        }
        // Both args are synonym (Modifies(a,v));Modifies(s,v)
        else if (argOne.getIsSynonym() && argTwo.getIsSynonym()) {
            // Retrieve current statements
            std::set<StmtNumber> s1s = resultManager->getValuesForSynonym(argOne.getStringValue());
            std::set<VarIndex> v1s = resultManager->getValuesForSynonym(argTwo.getStringValue());

            std::set<StmtNumber> evaluatedS1s;
            std::set<VarIndex> evaluatedV1s;

            // Obtain evaluation results
            for (StmtSetIterator s1 = s1s.begin(); s1 != s1s.end(); s1++) {
                for (VarIndexSetIterator v1 = v1s.begin(); v1 != v1s.end(); v1++) {
                    if (pkb->is(type, *s1, *v1)) {
                        evaluatedS1s.insert(*s1);
                        evaluatedV1s.insert(*v1);
                    }
                }
            }

            // Intersect two sets x2
            std::set<StmtNumber> updatedStatements;
            set_intersection(evaluatedS1s.begin(), evaluatedS1s.end(), s1s.begin(), s1s.end(),
                             std::inserter(updatedStatements, updatedStatements.begin()));

            std::set<VarIndex> updatedVariables;
            set_intersection(evaluatedV1s.begin(), evaluatedV1s.end(), v1s.begin(), v1s.end(),
                             std::inserter(updatedVariables, updatedVariables.begin()));

            // Check if relationship holds/have results
            if (updatedStatements.size() > 0 || updatedVariables.size() > 0) {
                suchThatRelObject.setResultsBoolean(true);
            }

            // Update the results table
            resultManager->updateSynonym(argOne.getStringValue(), updatedStatements);
            resultManager->updateSynonym(argTwo.getStringValue(), updatedVariables);
        }
    }

	else if (type == CALLS || type == CALLS_STAR) {
		// Both are procedure names : Calls("Giraffe","Panda")
		if (argOne.getIsSynonym() == false && argOne.getStringValue() != "_" && argTwo.getIsSynonym() == false && argTwo.getStringValue() != "_") {
//			suchThatRelObject.setResultsBoolean(pkb->is(type, argOne.getStringValue(), argTwo.getStringValue()));
		}
		// arg1 is synonym, arg2 is proc name: Calls(p1,"Panda")
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue() != "_") {
			// Store results
			std::set<ProcIndex> procedures = pkb->getProcsByProc(argTwo.getStringValue(), type);

			 // Check if relationship holds/have results
			if (procedures.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// Update the results table
			resultManager->updateSynonym(argOne.getStringValue(), procedures);
		}
		// arg1 is integer, and arg1 is synonym: Calls("Giraffe", p1)
		else if (argOne.getIsSynonym() == false && argOne.getStringValue() != "_" && argTwo.getIsSynonym()) {
			// Store results
			std::set<ProcIndex> procedures = pkb->getProcsByProc(type, argTwo.getStringValue());

			if (procedures.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// Update the results table
			resultManager->updateSynonym(argTwo.getStringValue(), procedures);
		}
		// arg1 is underscore & arg2 is integer: Calls(_,"Giraffe")
		else if (argOne.getIsSynonym() == false && argOne.getStringValue() == "_" && argTwo.getIsSynonym() == false && argTwo.getStringValue() != "_") {
			// Store results
			std::set<ProcIndex> procedures = pkb->getProcsByProc(argTwo.getStringValue(), type);

			if (procedures.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}
		}
		// arg1 is underscore & arg2 is synonym: Calls(_,p1)
		else if (argOne.getIsSynonym() == false && argOne.getStringValue() == "_" && argTwo.getIsSynonym()) {
			// Retrieve all procedures name for "_"
/*			std::set<ProcIndex> procedures = pkb->getAllProcIndex();
			std::set<ProcIndex> evaluatedP;

			// Get existing results from results table
			std::set<ProcIndex> currentProcedures = resultManager->getValuesForSynonym(argTwo.getStringValue());

			// Check if any synonym statements follows any statements
			for (StmtSetIterator i = procedures.begin(); i != procedures.end(); i++) {
				for (StmtSetIterator j = currentProcedures.begin(); j != currentProcedures.end(); j++) {
					if (pkb->is(type, *i, *j)) {
						suchThatRelObject.setResultsBoolean(true);
						evaluatedP.insert(*j);
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedP.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// Update the results table
			resultManager->updateSynonym(argTwo.getStringValue(), evaluatedP);
*/
		}
		// arg1 is proc name & arg2 is underscore: Calls("Giraffe",_)
		else if (argOne.getIntegerValue() > 0 && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
			// Store results
			std::set<ProcIndex> procedures = pkb->getProcsByProc(type, argOne.getStringValue());    

			if (procedures.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

		}
		// arg1 is synonym & arg2 is underscore: Calls(p1,_)
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
			// Set "_" to retrieve all statements and also get all statements of synonym
/*			std::set<ProcIndex> procedures = pkb->getAllProcIndex();
			std::set<StmtNumber> evaluatedP;

			// Get existing results from results table
			std::set<ProcIndex> currentProcedures = resultManager->getValuesForSynonym(argOne.getStringValue());

			// Check if any synonym statements follows any statements
			for (StmtSetIterator cs = currentProcedures.begin(); cs != currentProcedures.end(); cs++) {
				for (StmtSetIterator s = procedures.begin(); s != procedures.end(); s++) {
					if (pkb->is(type, *cs, *s)) {
						suchThatRelObject.setResultsBoolean(true);
						evaluatedP.insert(*cs);
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedP.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// Update the results table
			resultManager->updateSynonym(argOne.getStringValue(), evaluatedP);
*/
		}
		// Both arguments are a synonym: Calls(p1,p2)
		else if (argOne.getIsSynonym() && argTwo.getIsSynonym()) {
			// Retrieve current statements
			std::set<ProcIndex> p1s = resultManager->getValuesForSynonym(argOne.getStringValue());
			std::set<ProcIndex> p2s = resultManager->getValuesForSynonym(argTwo.getStringValue());

			std::set<StmtNumber> evaluatedP1s;
			std::set<StmtNumber> evaluatedP2s;

			// Obtain evaluation results
			for (StmtSetIterator s1 = p1s.begin(); s1 != p1s.end(); s1++) {
				for (StmtSetIterator s2 = p2s.begin(); s2 != p2s.end(); s2++) {
					if (pkb->is(type, *s1, *s2)) {
						evaluatedP1s.insert(*s1);
						evaluatedP2s.insert(*s2);
					}
				}
			}

			// Intersect two sets x2
			std::set<StmtNumber> updatedProcedures1;
			set_intersection(evaluatedP1s.begin(), evaluatedP1s.end(), p1s.begin(), p1s.end(),
				std::inserter(updatedProcedures1, updatedProcedures1.begin()));

			std::set<StmtNumber> updatedProcedures2;
			set_intersection(evaluatedP2s.begin(), evaluatedP2s.end(), p2s.begin(), p2s.end(),
				std::inserter(updatedProcedures2, updatedProcedures2.begin()));

			// Check if relationship holds/have results
			if (updatedProcedures1.size() > 0 || updatedProcedures2.size() > 0) {
				suchThatRelObject.setResultsBoolean(true);
			}

			// Update the results table
			resultManager->updateSynonym(argOne.getStringValue(), updatedProcedures1);
			resultManager->updateSynonym(argTwo.getStringValue(), updatedProcedures2);
		}
		// arg1 is underscore & arg2 is underscore: Calls(_,_);
		else if (argOne.getIsSynonym() == false && argOne.getStringValue() == "_" && argTwo.getIsSynonym() == false && argTwo.getStringValue() == "_") {
			// Retrieve wild card procedure names
/*			std::set<ProcIndex> procedures1 = pkb->getAllProcIndex();
			std::set<ProcIndex> procedures2 = pkb->getAllProcIndex();

			// Obtain evaluation results
			for (StmtSetIterator s1s = procedures1.begin(); s1s != procedures1.end(); s1s++) {
				for (StmtSetIterator s2s = procedures2.begin(); s2s != procedures2.end(); s2s++) {
					if (pkb->is(type, *s1s, *s2s)) {
						suchThatRelObject.setResultsBoolean(true);
						break;
					}
				}
				if (suchThatRelObject.getResultsBoolean() == true) {
					break;
				}
			}
	*/
		}
	}
    
	return suchThatRelObject;
}

ClauseWithObject QueryEvaluator::evaluateWith(ClauseWithObject withObject) {
	ClauseWithRefObject leftObj = withObject.getRefObject1();
	ClauseWithRefObject rightObj = withObject.getRefObject2();
	
	// left side is = synonym.attrName (attrRef)
	if (leftObj.getRefType() == ATTRREF) {
		// left side is = synonynm.procName 
		if (leftObj.getAttrType() == AttrType::PROC_NAME) {
			// if synonym = procedure -> p.procName
			if (leftObj.getEntityType() == PROCEDURE) {
				// right side is IDENTIFIER; p.procName = "First"
				if (rightObj.getRefType() == IDENTIFIER) {
					// Check & Get if procedure name exists
					ProcIndex procIndex = pkb->getProcIndex(rightObj.getStringValue());

					// Find all procedures that calls "First"

					// Check if relationship holds/have results

					// Update the results table
				}
				// right side is ATTRREF; p.procName = synonym.attrName
				else if (rightObj.getRefType() == ATTRREF) {
					// right side is procName; p1.procName = synonym.procName;
					if (rightObj.getAttrType() == AttrType::PROC_NAME) {
						// if right synonym = procedure -> p1.procName = p2.procName
						if (rightObj.getEntityType() == PROCEDURE) {

						}
						// if right synonym = call -> p1.procName = call.procName
						else if (rightObj.getEntityType() == CALL) {

						}
					}
					// right side is varName; p1.procName = synonym.varName
					else if (rightObj.getAttrType() == AttrType::VAR_NAME) {

					}
				}
			}
			// if synonym = call -> constraint call statements to those that calls IDENTIFIER
			else if (leftObj.getEntityType() == CALL) {

			}
		}
		// left side is = synonym.stmt#
		else if (leftObj.getAttrType() == AttrType::STMT_NO) {
			// right side is INTEGER; s.stmt# = 3
			if (rightObj.getRefType() == INTEGER) {

			}
			// right side is ATTRREF; s.stmt# = synonym.attrName
			else if (rightObj.getRefType() == ATTRREF) {
				// right side is value; s.stmt# = c.value
				if (rightObj.getAttrType() == AttrType::VALUE) {

				}
				// right side is stmt#; s.stmt# = a.stmt#
				else if (rightObj.getAttrType() == AttrType::STMT_NO) {

				}
			}
		}
		// left side is = synonym.varName
		else if (leftObj.getAttrType() == AttrType::VAR_NAME) {
			// right side is IDENTIFIER; v.varName = "x"
			if (rightObj.getRefType() == IDENTIFIER) {

			}
			// right side is ATTRREF; v.varName = synonym.attrName
			else if (rightObj.getRefType() == ATTRREF) {
				// right side is procName; v.varName = p.procName;
				if (rightObj.getAttrType() == AttrType::PROC_NAME) {

				}
				// right side is varName; v1.varName = v2.varName;
				else if (rightObj.getAttrType() == AttrType::VAR_NAME) {

				}
			}
		}
		// left side is = synonym.value
		else if (leftObj.getAttrType() == AttrType::VALUE) {
			// right side is INTEGER; c.value = 3
			if (rightObj.getRefType() == INTEGER) {

			}
			else if (rightObj.getRefType() == ATTRREF) {
				// right side is value; c1.value = c2.value
				if (rightObj.getAttrType() == AttrType::VALUE) {

				}
				// right side is stmt#; c.value = s.stmt#
				else if (rightObj.getAttrType() == AttrType::STMT_NO) {

				}
			}
		}
	}

	// left side is = synonym (prog_line)
	else if (leftObj.getRefType() == SYNONYM) {
		// right side is INTEGER; n = 10;
		if (rightObj.getRefType() == INTEGER) {

		} 
		// right side is SYNONYM; n = n1;
		else if (rightObj.getRefType() == SYNONYM) {
			std::set<StmtNumber> statements = resultManager->getValuesForSynonym(rightObj.getStringValue());

			if (statements.size() > 0) {
				withObject.setResultsBoolean(true);
			}

			// Update the results table
			resultManager->updateSynonym(leftObj.getStringValue(), statements);

		}
		// right side is ATTRREF; n = synonym.attrName
		else if (rightObj.getRefType() == ATTRREF) {
			// right side is stmt#; n = s.stmt# OR right side is value; n = c.value;
			if (rightObj.getAttrType() == AttrType::STMT_NO || rightObj.getAttrType() == AttrType::VALUE) {
				// Get existing statements/values
				std::set<StmtNumber> statements = resultManager->getValuesForSynonym(rightObj.getSynonym());

				if (statements.size() > 0) {
					withObject.setResultsBoolean(true);
				}

				// Update the results table
				resultManager->updateSynonym(leftObj.getStringValue(), statements);
			} 
		}
	}

	return ClauseWithObject();
}

ClausePatternObject QueryEvaluator::evaluatePattern(ClausePatternObject patternObject) {
    EntityType patternType = patternObject.getPatternType();
    EntityType firstArgType = patternObject.getFirstArgumentType();
    std::string patternSynonymArg = patternObject.getPatternSynonymArgument();
    std::string firstArg = patternObject.getFirstArgument();
    std::string secondArg = patternObject.getSecondArgument();
    std::string thirdArg = patternObject.getThirdArgument();
    bool isFirstArgSynonym = patternObject.getIsFirstArgSynonym();
    bool relationshipHolds = true;

    // ASSIGN pattern:
    if (patternType == ASSIGN) {
        // pattern a(<firstArg>,_)
        if (secondArg == "_") {
            // pattern a(v,_)
            if (isFirstArgSynonym) {
                // Retrieve existing pattern synonym statements & first arg synonym variables
                std::set<StmtNumber> patternSynonymStatements = resultManager->getValuesForSynonym(patternSynonymArg);
                std::set<VarIndex> firstArgSynonymVariables = resultManager->getValuesForSynonym(firstArg);

                // Evaluated true statements
                std::set<StmtNumber> evaluatedPatternSynonymStatements;
                std::set<VarIndex> evaluatedfirstArgSynonymVariables;

                // Check all existing pattern synonym statements if it modifies the 'variable'
                for (StmtSetIterator cs = patternSynonymStatements.begin(); cs != patternSynonymStatements.end(); cs++) {
                    for (VarIndexSetIterator s = firstArgSynonymVariables.begin(); s != firstArgSynonymVariables.end(); s++) {
                        if (pkb->is(MODIFIES, *cs, *s)) {
                            evaluatedPatternSynonymStatements.insert(*cs);
                            evaluatedfirstArgSynonymVariables.insert(*s);
                            break;
                        }
                    }
                }

                // Intersect two sets x2
                std::set<StmtNumber> updatedStatements;
                set_intersection(evaluatedPatternSynonymStatements.begin(), evaluatedPatternSynonymStatements.end(), patternSynonymStatements.begin(), patternSynonymStatements.end(),
                                 std::inserter(updatedStatements, updatedStatements.begin()));

                std::set<VarIndex> updatedVariables;
                set_intersection(evaluatedfirstArgSynonymVariables.begin(), evaluatedfirstArgSynonymVariables.end(), firstArgSynonymVariables.begin(), firstArgSynonymVariables.end(),
                                 std::inserter(updatedVariables, updatedVariables.begin()));

                // Check if relationship holds/have results
                if (updatedStatements.size() > 0 || updatedVariables.size() > 0) {
                    patternObject.setResultsBoolean(true);
                }

                // Update the results table
                resultManager->updateSynonym(patternSynonymArg, updatedStatements);
                resultManager->updateSynonym(firstArg, updatedVariables);
            }
            // pattern a(_,_)
            else if (firstArg == "_") {
                // Check if theres any statements number inside
                if (resultManager->getValuesForSynonym(patternSynonymArg).size() > 0) {
                    patternObject.setResultsBoolean(true);
                }
            }
            // pattern a("x",_)
            else if (firstArgType == VARIABLE) {
                // Retrieve existing pattern synonym statements
                std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(patternSynonymArg);

                // Evaluated true statements
                std::set<StmtNumber> evaluatedS;

                // Check all existing pattern synonym statements if it modifies the 'variable'
                for (StmtSetIterator i = currentStatements.begin(); i != currentStatements.end(); i++) {
                    // Check if the existing statement modifies the 'variable'
                    if (pkb->is(MODIFIES, *i, pkb->getVarIndex(firstArg))) {
                        evaluatedS.insert(*i);
                    }
                }

                // Check if relationship holds/have results
                if (evaluatedS.size() > 0) {
                    patternObject.setResultsBoolean(true);
                }

                // Update the results table
                resultManager->updateSynonym(patternSynonymArg, evaluatedS);
            }
        }
        // Means Pattern must be Pattern a(<firstArg>,"_<constant/variable>_")	
        else {
            // pattern a(v,"_<constant/variable>_")
            if (isFirstArgSynonym) {
                // Retrieve existing pattern synonym statements & first arg synonym variables
                std::set<StmtNumber> patternSynonymStatements = resultManager->getValuesForSynonym(patternSynonymArg);
                std::set<VarIndex> firstArgSynonymVariables = resultManager->getValuesForSynonym(firstArg);

                // Evaluated true statements
                std::set<StmtNumber> evaluatedPatternSynonymStatements;
                std::set<VarIndex> evaluatedfirstArgSynonymVariables;

                // Check all existing pattern synonym statements if it modifies the 'variable'
                for (StmtSetIterator cs = patternSynonymStatements.begin(); cs != patternSynonymStatements.end(); cs++) {
                    for (VarIndexSetIterator s = firstArgSynonymVariables.begin(); s != firstArgSynonymVariables.end(); s++) {
                        if (pkb->is(MODIFIES, *cs, *s)) {
                            if (pkb->isAssignHasSubexpr(*cs, secondArg)) {
                                evaluatedPatternSynonymStatements.insert(*cs);
                                evaluatedfirstArgSynonymVariables.insert(*s);
                                break;
                            }
                        }
                    }
                }

                // Intersect two sets x2
                std::set<StmtNumber> updatedStatements;
                set_intersection(evaluatedPatternSynonymStatements.begin(), evaluatedPatternSynonymStatements.end(), patternSynonymStatements.begin(), patternSynonymStatements.end(),
                                 std::inserter(updatedStatements, updatedStatements.begin()));

                std::set<VarIndex> updatedVariables;
                set_intersection(evaluatedfirstArgSynonymVariables.begin(), evaluatedfirstArgSynonymVariables.end(), firstArgSynonymVariables.begin(), firstArgSynonymVariables.end(),
                                 std::inserter(updatedVariables, updatedVariables.begin()));

                // Check if relationship holds/have results
                if (updatedStatements.size() > 0 || updatedVariables.size() > 0) {
                    patternObject.setResultsBoolean(true);
                }

                // Update the results table
                resultManager->updateSynonym(patternSynonymArg, updatedStatements);
                resultManager->updateSynonym(firstArg, updatedVariables);
            }
            // pattern a(_,"_<constant/variable>_")
            else if (firstArg == "_") {
                // Retrieve existing pattern synonym statements & first arg synonym variables
                std::set<StmtNumber> patternSynonymStatements = resultManager->getValuesForSynonym(patternSynonymArg);
                std::set<VarIndex> firstArgSynonymVariables = pkb->getAllVarIndex();

                // Evaluated true statements
                std::set<StmtNumber> evaluatedPatternSynonymStatements;
                std::set<VarIndex> evaluatedfirstArgSynonymVariables;

                // Check all existing pattern synonym statements if it modifies the 'variable'
                for (StmtSetIterator cs = patternSynonymStatements.begin(); cs != patternSynonymStatements.end(); cs++) {
                    for (VarIndexSetIterator s = firstArgSynonymVariables.begin(); s != firstArgSynonymVariables.end(); s++) {
                        if (pkb->is(MODIFIES, *cs, *s)) {
                            if (pkb->isAssignHasSubexpr(*cs, secondArg)) {
                                evaluatedPatternSynonymStatements.insert(*cs);
                                break;
                            }
                        }
                    }
                }

                // Check if relationship holds/have results
                if (evaluatedPatternSynonymStatements.size() > 0) {
                    patternObject.setResultsBoolean(true);
                }

                // Update the results table
                resultManager->updateSynonym(patternSynonymArg, evaluatedPatternSynonymStatements);
            }
            // pattern a("x","_<constant/variable>_")
            else if (firstArgType == VARIABLE) {
                // Retrieve existing pattern synonym statements
                std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(patternSynonymArg);

                // Evaluated true statements
                std::set<StmtNumber> evaluatedS;

                // Check all existing pattern synonym statements if it modifies the 'variable'
                for (StmtSetIterator i = currentStatements.begin(); i != currentStatements.end(); i++) {
                    // Check if the existing statement modifies the 'variable'
                    if (pkb->is(MODIFIES, *i, pkb->getVarIndex(firstArg))) {
                        // If yes, check if this statement uses the second argument subexpression
                        if (pkb->isAssignHasSubexpr(*i, secondArg)) {
                            evaluatedS.insert(*i);
                        }
                    }
                }

                // Check if relationship holds/have results
                if (evaluatedS.size() > 0) {
                    patternObject.setResultsBoolean(true);
                }

                // Update the results table
                resultManager->updateSynonym(patternSynonymArg, evaluatedS);
            }
        }
    }
	// WHILE pattern:
	else if (patternType == WHILE) {
		// Pattern w(<first arg>,_)
		if (secondArg == "_") {
			// Pattern w(v,_)
			if (isFirstArgSynonym) {
				// Retrieve existing pattern synonym statements & first arg synonym variable
				std::set<StmtNumber> patternSynonymStatements = resultManager->getValuesForSynonym(patternSynonymArg);
				std::set<VarIndex> firstArgSynonymVariables = resultManager->getValuesForSynonym(firstArg);

				// Evaluated true statements
				std::set<StmtNumber> evaluatedPatternSynonymStatements;
				std::set<VarIndex> evaluatedfirstArgSynonymVariables;

				// Check all existing pattern synonym statements if while statement uses the 'variable' as control variable
				for (StmtSetIterator cs = patternSynonymStatements.begin(); cs != patternSynonymStatements.end(); cs++) {
					for (VarIndexSetIterator s = firstArgSynonymVariables.begin(); s != firstArgSynonymVariables.end(); s++) {
						if (pkb->isWhilePattern(*cs, *s)) {
							evaluatedPatternSynonymStatements.insert(*cs);
							evaluatedfirstArgSynonymVariables.insert(*s);
							break;
						}
					}
				}

				// Intersect two sets x2
				std::set<StmtNumber> updatedStatements;
				set_intersection(evaluatedPatternSynonymStatements.begin(), evaluatedPatternSynonymStatements.end(), patternSynonymStatements.begin(), patternSynonymStatements.end(),
					std::inserter(updatedStatements, updatedStatements.begin()));

				std::set<VarIndex> updatedVariables;
				set_intersection(evaluatedfirstArgSynonymVariables.begin(), evaluatedfirstArgSynonymVariables.end(), firstArgSynonymVariables.begin(), firstArgSynonymVariables.end(),
					std::inserter(updatedVariables, updatedVariables.begin()));

				// Check if relationship holds/have results
				if (updatedStatements.size() > 0 || updatedVariables.size() > 0) {
					patternObject.setResultsBoolean(true);
				}

				// Update the results table
				resultManager->updateSynonym(patternSynonymArg, updatedStatements);
				resultManager->updateSynonym(firstArg, updatedVariables);
			} 
			// Pattern w(_,_)
			else if (firstArg == "_") {
				// Check if theres any statements number inside
				if (resultManager->getValuesForSynonym(patternSynonymArg).size() > 0) {
					patternObject.setResultsBoolean(true);
				}
			}
			// Pattern w("i",_)
			else if (firstArgType == VARIABLE) {
				// Retrieve existing pattern synonym statements
				std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(patternSynonymArg);

				// Evaluated true statements
				std::set<StmtNumber> evaluatedS;

				// Check all existing pattern synonym statements if it uses the 'variable' as control variable
				for (StmtSetIterator i = currentStatements.begin(); i != currentStatements.end(); i++) {
					// Check if the existing statement uses the 'variable' as control variable
					if (pkb->isWhilePattern(*i, pkb->getVarIndex(firstArg))) {
						evaluatedS.insert(*i);
					}
				}

				// Check if relationship holds/have results
				if (evaluatedS.size() > 0) {
					patternObject.setResultsBoolean(true);
				}

				// Update the results table
				resultManager->updateSynonym(patternSynonymArg, evaluatedS);
			}
		}
	}
	// IF pattern:
	else if (patternType == IF) {
		// pattern if(<first arg>,_,_)
		if (secondArg == "_" && thirdArg == "_") {
			// pattern if(v,_,_)
			if (isFirstArgSynonym) {
				// Retrieve existing pattern synonym statements & first arg synonym variable
				std::set<StmtNumber> patternSynonymStatements = resultManager->getValuesForSynonym(patternSynonymArg);
				std::set<VarIndex> firstArgSynonymVariables = resultManager->getValuesForSynonym(firstArg);

				// Evaluated true statements
				std::set<StmtNumber> evaluatedPatternSynonymStatements;
				std::set<VarIndex> evaluatedfirstArgSynonymVariables;

				// Check all existing pattern synonym statements if while statement uses the 'variable' as control variable
				for (StmtSetIterator cs = patternSynonymStatements.begin(); cs != patternSynonymStatements.end(); cs++) {
					for (VarIndexSetIterator s = firstArgSynonymVariables.begin(); s != firstArgSynonymVariables.end(); s++) {
						if (pkb->isIfPattern(*cs, *s)) {
							evaluatedPatternSynonymStatements.insert(*cs);
							evaluatedfirstArgSynonymVariables.insert(*s);
							break;
						}
					}
				}

				// Intersect two sets x2
				std::set<StmtNumber> updatedStatements;
				set_intersection(evaluatedPatternSynonymStatements.begin(), evaluatedPatternSynonymStatements.end(), patternSynonymStatements.begin(), patternSynonymStatements.end(),
					std::inserter(updatedStatements, updatedStatements.begin()));

				std::set<VarIndex> updatedVariables;
				set_intersection(evaluatedfirstArgSynonymVariables.begin(), evaluatedfirstArgSynonymVariables.end(), firstArgSynonymVariables.begin(), firstArgSynonymVariables.end(),
					std::inserter(updatedVariables, updatedVariables.begin()));

				// Check if relationship holds/have results
				if (updatedStatements.size() > 0 || updatedVariables.size() > 0) {
					patternObject.setResultsBoolean(true);
				}

				// Update the results table
				resultManager->updateSynonym(patternSynonymArg, updatedStatements);
				resultManager->updateSynonym(firstArg, updatedVariables);
			}
			// pattern if(_,_,_)
			else if (firstArg == "_") {
				// Check if theres if statements inside source program
				if (resultManager->getValuesForSynonym(patternSynonymArg).size() > 0) {
					patternObject.setResultsBoolean(true);
				}
			}
			// pattern if("x",_,_)
			else if (firstArgType == VARIABLE) {
				// Retrieve existing pattern synonym statements
				std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(patternSynonymArg);

				// Evaluated true statements
				std::set<StmtNumber> evaluatedS;

				// Check all existing pattern synonym statements if it uses the 'variable' as control variable
				for (StmtSetIterator i = currentStatements.begin(); i != currentStatements.end(); i++) {
					// Check if the existing statement uses the 'variable' as control variable
					if (pkb->isIfPattern(*i, pkb->getVarIndex(firstArg))) {
						evaluatedS.insert(*i);
					}
				}

				// Check if relationship holds/have results
				if (evaluatedS.size() > 0) {
					patternObject.setResultsBoolean(true);
				}

				// Update the results table
				resultManager->updateSynonym(patternSynonymArg, evaluatedS);
			}
		}
	}
  
	return patternObject;
}

std::vector<std::string> QueryEvaluator::evaluateSelect(ClauseSelectObject ClauseSelectObject, bool relationshipHolds) {
    // Constraint results by SelectObj
    std::vector<std::string> results;

    if (relationshipHolds) {
        // If its Select BOOLEAN
        if (ClauseSelectObject.getBoolean()) {
            // Output: TRUE
            results.push_back("true");
        }
        // Else then it must be a synonym
        else {
            // Iteration 1: only if the entity is VARIABLE, then return string (variable names)
            if (ClauseSelectObject.getEntityType() == VARIABLE) {
                std::set<VarIndex> setResults1 = resultManager->getValuesForSynonym(ClauseSelectObject.getSynonymString());
                std::vector<std::string> vectorResults1;
                std::transform(setResults1.begin(), setResults1.end(), std::back_inserter(vectorResults1), to_var_name);
                return vectorResults1;
            } else {
                std::set<StmtNumber> setResults2 = resultManager->getValuesForSynonym(ClauseSelectObject.getSynonymString());
                std::vector<StmtNumber> vectorStmtNumbers(setResults2.begin(), setResults2.end());
                std::vector<std::string> vectorResults2;
                for (std::vector<StmtNumber>::iterator it = vectorStmtNumbers.begin(); it != vectorStmtNumbers.end(); it++) {
                    vectorResults2.push_back(std::to_string(*it));
                }
                return vectorResults2;
            }
        }
    } else {
        if (ClauseSelectObject.getBoolean()) {
            // Output: FALSE
            results.push_back("false");
        }
    }

    return results;
}
