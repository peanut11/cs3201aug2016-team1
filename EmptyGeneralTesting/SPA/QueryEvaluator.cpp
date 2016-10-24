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

std::vector<StringToken> QueryEvaluator::to_tokens(std::string str) {
    // Note: QueryPreProcessor should store std::vector<StringToken>
    StringTokenizer st = StringTokenizer(str, DelimiterMode::QUERY_PREPROCESSOR);
    std::vector<StringToken> tokens;

    while (st.hasMoreTokens()) {
        tokens.push_back(st.nextToken());
    }

    return tokens;
}

QueryEvaluator* QueryEvaluator::getInstance() {
    if (_instance == nullptr) {
        _instance = new QueryEvaluator();
    }
    return _instance;
}

QueryEvaluator::QueryEvaluator() {
    pkb = PKB::getInstance();
    synonymTable = SynonymTable::getInstance();
    resultManager = new ResultGridManager();
	queryOptimizer = new QueryOptimization();
}

QueryOptimization* QueryEvaluator::getQueryOptimizer() {
	return this->queryOptimizer;
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

VarName QueryEvaluator::to_var_name(VarIndex varIndex) {
	// return PKB::getInstance()->getVarName(varIndex);              // Original
	return QueryEvaluator::getInstance()->pkb->getVarName(varIndex); // DummyPKB
}

ProcName QueryEvaluator::to_proc_name(ProcIndex procIndex) {
	// return PKB::getInstance()->getVarName(varIndex);              // Original
	return QueryEvaluator::getInstance()->pkb->getProcName(procIndex); // DummyPKB
}

std::vector<std::string> QueryEvaluator::evaluate(QueryTable queryTable) {
    // Get new instance for new query
    delete resultManager;
	delete queryOptimizer;
    resultManager = new ResultGridManager();
	queryOptimizer = new QueryOptimization();

    //try {
        // Get select object and all clause objects
		ClauseSelectObject select = queryTable.getResult().getClauseSelectObjectList()[0];
		//ClauseSelectObject select = queryTable.getSelect();
        //std::vector<ClausePatternObject*> patterns = queryTable.getPatterns();
        //std::vector<ClauseSuchThatObject*> suchThats = queryTable.getSuchThats();
        //std::vector<ClauseWithObject*> withs = queryTable.getWiths();

        // Populate result grids
        populateResultGrids();

		// Group queries into optimized groups
		std::vector<GroupObject> groupQueries = queryOptimizer->beginOptimize(SynonymGroup::getInstance(), queryTable);
		bool relationshipHolds = true;

		// Evaluate the optimized groups
		for (std::vector<GroupObject>::iterator it = groupQueries.begin(); it != groupQueries.end(); it++) {
			// Get list of clauses in group
			std::vector<ClauseObject*> clauses = it->getClauseObjectList();

			// Iterate the clauses in each group
			int clauseIndex = 0;
			for (ClauseObject* obj : clauses) {
				ClauseType::ClauseType clauseType = obj->getClauseType();
				bool isStopEvaluation = false;

				// Check if group is BOOLEAN
				if (it->getGroupType() == GroupType::BOOLEAN) {
					isStopEvaluation = true;
				}

				// Check if group need to stop evaluation when got results
				if (it->getGroupType() == GroupType::NOT_RELATED || it->getGroupType() == GroupType::NOT_RELATED_CONTAIN_AFFECTS) {
					isStopEvaluation = (clauseIndex == clauses.size() - 1);
				}

				// Evaluate such that clause
				if (clauseType == ClauseType::SUCH_THAT) {
					ClauseSuchThatObject* childObj = dynamic_cast<ClauseSuchThatObject*>(obj);
					relationshipHolds = evaluateSuchThat(childObj, isStopEvaluation)->getResultsBoolean();
				}
				// Evaluate with clause
				else if (clauseType == ClauseType::WITH) {
					ClauseWithObject* childObj = dynamic_cast<ClauseWithObject*>(obj);
					relationshipHolds = evaluateWith(childObj, isStopEvaluation)->getResultsBoolean();
				}
				// Evaluate pattern clause
				else if (clauseType == ClauseType::PATTERN) {
					ClausePatternObject* childObj = dynamic_cast<ClausePatternObject*>(obj);
					relationshipHolds = evaluatePattern(childObj, isStopEvaluation)->getResultsBoolean();
				}
				// Stop evaluation if relationship is false
				if (relationshipHolds == false) {
					break;
				}

				clauseIndex++;
			}
			// Stop evaluation if relationship is false
			if (relationshipHolds == false) {
				break;
			}
		}

		/*
        for (int i = 0; i < 2; i++) {
            // Iterate the such that clauses vectors and evaluate them
            for (std::vector<ClauseSuchThatObject*>::iterator it = suchThats.begin(); it != suchThats.end(); it++) {
                //ClauseSuchThatObject suchThatObject = evaluateSuchThat(*it);
                //relationshipHolds = relationshipHolds && suchThatObject.getResultsBoolean();
            }

            // Iterate the with clauses vectors and evaluate them
            for (std::vector<ClauseWithObject*>::iterator it = withs.begin(); it != withs.end(); it++) {
                //ClauseWithObject withObject = evaluateWith(*it);
                //relationshipHolds = relationshipHolds && withObject->getResultsBoolean();
            }

            // Iterate the pattern clauses vectors and evaluate them
            for (std::vector<ClausePatternObject*>::iterator it = patterns.begin(); it != patterns.end(); it++) {
               // ClausePatternObject patternObject = evaluatePattern(*it);
               // relationshipHolds = relationshipHolds && patternObject.getResultsBoolean();
            }
        }
		*/

        // Evaluate results by constraint of select object
        return evaluateSelect(select, relationshipHolds);

   // } catch (std::runtime_error e) { // there's error
        //throw e.what();
		//return std::vector<std::string>(); // return empty list 
   // }
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
            EntityType type = it->getType();
            StmtSet stmts = pkb->getStmtsByType(type);
            resultManager->initialiseSynonym(syn, stmts);
        }
    }
    return resultManager;
}

ClauseSuchThatObject* QueryEvaluator::evaluateSuchThat(ClauseSuchThatObject* suchThatRelObject, bool isStopEvaluation) {
    RelationshipType type = suchThatRelObject->getRelationshipType();
    ClauseSuchThatArgObject* argOne = suchThatRelObject->getArgsOne();
    ClauseSuchThatArgObject* argTwo = suchThatRelObject->getArgsTwo();
    // FOLLOW / FOLLOWS_STAR / PARENT / PARENT_STAR / NEXT / NEXT_STAR relationship
    if (type == FOLLOWS || type == FOLLOWS_STAR || type == PARENT || type == PARENT_STAR || type == NEXT || type == NEXT_STAR) {
        // Both are statement numbers: Follows(3,4)
        if (argOne->getIntegerValue() > 0 && argTwo->getIntegerValue() > 0) {
            suchThatRelObject->setResultsBoolean(pkb->is(type, argOne->getIntegerValue(), argTwo->getIntegerValue()));
        }
        // arg1 is synonym, arg2 is integer: Follows(s,3); Follows(a,4)
        else if (argOne->getIsSynonym() && argTwo->getIntegerValue() > 0) {
            // Store results
			std::set<StmtNumber> statements = pkb->getStmtsByStmt(argTwo->getIntegerValue(), type);     // Get stmts that arg2(integer) follows
			
			if (statements.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), statements);
			}
			/*
			// Retrieve current statements																							
			std::set<StmtNumber> s = resultManager->getValuesForSynonym(argOne->getStringValue());

			// Get results
			std::set<StmtNumber> evaluatedS;
            StmtNumber stmt = argTwo->getIntegerValue();
			for (StmtSetIterator i = s.begin(); i != s.end(); i++) {
				if (pkb->is(type, *i, stmt)) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedS.insert(*i);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}
 
			// Check if relationship holds/have results
            if (evaluatedS.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedS);
            }
			*/
            
        }
        // arg1 is integer, and arg1 is synonym: Follows(3,s);Follows(4,a)
        else if (argOne->getIntegerValue() > 0 && argTwo->getIsSynonym()) {
            // Store results
	        std::set<StmtNumber> statements = pkb->getStmtsByStmt(type, argOne->getIntegerValue()); // Get stmts that arg1(integer) is followed by
			
			if (statements.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
				// Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), statements);
			}
 /*
			// Retrieve current statements			
			std::set<StmtNumber> s = resultManager->getValuesForSynonym(argTwo->getStringValue());

			// Get results
			std::set<StmtNumber> evaluatedS;
            StmtNumber stmt = argOne->getIntegerValue();
			for (StmtSetIterator i = s.begin(); i != s.end(); i++) {
				if (pkb->is(type, stmt, *i)) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedS.insert(*i);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
            if (evaluatedS.size() > 0) {
                // Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), evaluatedS);
            }
*/
        }
        // arg1 is underscore & arg2 is integer: Follows(_,10);
        else if (argOne->getIsSynonym() == false && argOne->getStringValue() == "_" && argTwo->getIntegerValue() > 0) {
            // Store results
            std::set<StmtNumber> statements = pkb->getStmtsByStmt(argTwo->getIntegerValue(), type);     // Get stmts that arg2(integer) follows

            // Check if relationship holds
            if (statements.size() > 0) {
                suchThatRelObject->setResultsBoolean(true);
            }
        }
        // arg1 is underscore & arg2 is synonym: Follows(_,s);
        else if (argOne->getIsSynonym() == false && argOne->getStringValue() == "_" && argTwo->getIsSynonym()) {
			// Set "_" to retrieve all statements and also get all statements of synonym
			std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(argTwo->getStringValue());
			
			std::set<StmtNumber> evaluatedS;
			for (StmtSetIterator k = currentStatements.begin(); k != currentStatements.end(); k++) {
				std::set<StmtNumber> validStatements = pkb->getStmtsByStmt(*k, type);
				if (validStatements.size() > 0) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedS.insert(*k);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedS.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), evaluatedS);
			}
/*			
			// Set "_" to retrieve all statements and also get all statements of synonym
            std::set<StmtNumber> statements = pkb->getAllStmts();
            std::set<StmtNumber> evaluatedS;

            // Get existing results from results table
            std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(argTwo->getStringValue());

            // Check if any synonym statements follows any statements
            for (StmtSetIterator i = statements.begin(); i != statements.end(); i++) {
                for (StmtSetIterator j = currentStatements.begin(); j != currentStatements.end(); j++) {
                    if (pkb->is(type, *i, *j)) {
                        suchThatRelObject->setResultsBoolean(true);
                        evaluatedS.insert(*j);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
                    }
                }
            }

            // Check if relationship holds/have results
            if (evaluatedS.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), evaluatedS);
            }
*/
        }
        // arg1 is integer & arg2 is underscore: Follows(3,_);
        else if (argOne->getIntegerValue() > 0 && argTwo->getIsSynonym() == false && argTwo->getStringValue() == "_") {
            // Store results
            std::set<StmtNumber> statements = pkb->getStmtsByStmt(type, argOne->getIntegerValue());     // Get stmts that arg2(integer) follows

            // Check if relationship holds
            if (statements.size() > 0) {
                suchThatRelObject->setResultsBoolean(true);
            }
        }
        // arg1 is synonym & arg2 is underscore: Follows(s,_);
        else if (argOne->getIsSynonym() && argTwo->getIsSynonym() == false && argTwo->getStringValue() == "_") {
			// Set "_" to retrieve all statements and also get all statements of synonym
			std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(argOne->getStringValue());

			std::set<StmtNumber> evaluatedS;
			for (StmtSetIterator k = currentStatements.begin(); k != currentStatements.end(); k++) {
				std::set<StmtNumber> validStatements = pkb->getStmtsByStmt(type, *k);
				if (validStatements.size() > 0) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedS.insert(*k);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedS.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedS);
			}
/*			
			// Set "_" to retrieve all statements and also get all statements of synonym
            std::set<StmtNumber> statements = pkb->getAllStmts();
            std::set<StmtNumber> evaluatedS;

            // Get existing results from results table
            std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(argOne->getStringValue());

            // Check if any synonym statements follows any statements
            for (StmtSetIterator cs = currentStatements.begin(); cs != currentStatements.end(); cs++) {
                for (StmtSetIterator s = statements.begin(); s != statements.end(); s++) {
                    if (pkb->is(type, *cs, *s)) {
                        suchThatRelObject->setResultsBoolean(true);
                        evaluatedS.insert(*cs);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
                    }
                }
            }

            // Check if relationship holds/have results
            if (evaluatedS.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedS);
            }
			*/
        }
        // Both arguments are a synonym: Follows(s1,s2); Follow(a,s1);
        else if (argOne->getIsSynonym() && argTwo->getIsSynonym()) {
			// Get current statements
			std::tuple<SynonymString, SynonymString> synonymTuple(argOne->getStringValue(), argTwo->getStringValue());
			std::set<StmtNumber> statements1 = resultManager->getValuesForSynonym(argOne->getStringValue());
			std::set<StmtNumber> statements2 = resultManager->getValuesForSynonym(argTwo->getStringValue());
			ValueTupleSet evaluatedTupleStatements;
			
			for (StmtSetIterator k = statements1.begin(); k != statements1.end(); k++) {
				std::set<StmtNumber> retrievedStatements = pkb->getStmtsByStmt(type, *k);
				for (StmtSetIterator m = retrievedStatements.begin(); m != retrievedStatements.end(); m++) {
					if (statements2.count(*m)) {
						std::tuple<StmtNumber, StmtNumber> validTuple(*k, *m);
						evaluatedTupleStatements.insert(validTuple);
						suchThatRelObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedTupleStatements.size() > 0) {
				// Update tuple with evaluation results
				resultManager->updateSynonymTuple(synonymTuple, evaluatedTupleStatements);
			}
/*
			// Get current tuple synonyms 
			std::tuple<SynonymString, SynonymString> synonymTuple(argOne->getStringValue(), argTwo->getStringValue());
			std::set<ValueTuple> tupleStatements = resultManager->getValuesForSynonymTuple(synonymTuple);

			// Obtain evaluation results for tuple
			ValueTupleSet evaluatedTupleStatements;
			for (std::set<ValueTuple>::iterator t1 = tupleStatements.begin(); t1 != tupleStatements.end(); t1++) {
				if (pkb->is(type, std::get<0>(*t1), std::get<1>(*t1))) {
					evaluatedTupleStatements.insert(*t1);
					suchThatRelObject->setResultsBoolean(true);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedTupleStatements.size() > 0) {
				// Update tuple with evaluation results
				resultManager->updateSynonymTuple(synonymTuple, evaluatedTupleStatements);
			}	
			*/
/*
			// Retrieve current statements
			std::set<StmtNumber> s1s = resultManager->getValuesForSynonym(argOne->getStringValue());
			std::set<StmtNumber> s2s = resultManager->getValuesForSynonym(argTwo->getStringValue());
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
                suchThatRelObject->setResultsBoolean(true);
            }

            // Update the results table
            resultManager->updateSynonym(argOne->getStringValue(), updatedStatements1);
            resultManager->updateSynonym(argTwo->getStringValue(), updatedStatements2);
*/
        }
		// arg1 is underscore & arg2 is underscore: Follows(_,_);
		else if (argOne->getIsSynonym() == false && argOne->getStringValue() == "_" && argTwo->getIsSynonym() == false && argTwo->getStringValue() == "_") {
			// Retrieve wild card statements
			std::set<StmtNumber> statements1 = pkb->getAllStmts();
			std::set<StmtNumber> statements2 = pkb->getAllStmts();

			// Obtain evaluation results
			for (StmtSetIterator s1s = statements1.begin(); s1s != statements1.end(); s1s++) {
				for (StmtSetIterator s2s = statements2.begin(); s2s != statements2.end(); s2s++) {
					if (pkb->is(type, *s1s, *s2s)) {
						suchThatRelObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
					}
				}
			}
		}
    }

	// MODIFIES / USES relationship:
    else if (type == MODIFIES || type == USES) {
        // If left arg is 'statement number', right arg is "x" (Modifies(3,"x"))
        if (argOne->getIntegerValue() > 0 && argTwo->getIsSynonym() == false && argTwo->getStringValue() != "_") {
            suchThatRelObject->setResultsBoolean(pkb->is(type, argOne->getIntegerValue(), pkb->getVarIndex(argTwo->getStringValue())));
        }
        // If left arg is 'statement number', right arg is a synonym (Modifies(3,v))
        else if (argOne->getIntegerValue() > 0 && argTwo->getIsSynonym()) {
		/*
			// Retrieve current variables																							
			std::set<VarIndex> v = resultManager->getValuesForSynonym(argTwo->getStringValue());

			// Get results
			std::set<VarIndex> evaluatedV;
            StmtNumber stmt = argOne->getIntegerValue();
			for (StmtSetIterator i = v.begin(); i != v.end(); i++) {
				if (pkb->is(type, stmt, *i)) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedV.insert(*i);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedV.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), evaluatedV);
			}
*/

			// Get results
            std::set<VarIndex> variableIndexes = pkb->getVarsByStmt(argOne->getIntegerValue(), type);

			if (variableIndexes.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
				// Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), variableIndexes);
			}

        }
        // If left arg is 'statement number', right arg is "_" (Modifies(3,_);
        else if (argOne->getIntegerValue() > 0 && argTwo->getIsSynonym() == false && argTwo->getStringValue() == "_") {
            // Store results
            std::set<VarIndex> variables = pkb->getVarsByStmt(argOne->getIntegerValue(), type);     // Get vars that arg1(integer) follows

            // Check if relationship holds
            if (variables.size() > 0) {
                suchThatRelObject->setResultsBoolean(true);
            }
        }
        // If left arg is synonym, right arg is "x" (Modifies(s,"x")); Modifies(a,"x")
        else if (argOne->getIsSynonym() && argTwo->getIsSynonym() == false && argTwo->getStringValue() != "_") {
/*
			// Retrieve current statements																							
			std::set<StmtNumber> s = resultManager->getValuesForSynonym(argOne->getStringValue());

			// Get results
			std::set<StmtNumber> evaluatedS;
            VarIndex var = pkb->getVarIndex(argTwo->getStringValue());
			for (StmtSetIterator i = s.begin(); i != s.end(); i++) {
				if (pkb->is(type, *i, var)) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedS.insert(*i);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}
			
			// Check if relationship holds/have results
			if (evaluatedS.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedS);
			}
			*/

			
			// Get all statements that modifies variable
			VarIndex varIndex = pkb->getVarIndex(argTwo->getStringValue());
			std::set<StmtNumber> statements = pkb->getStmtsByVar(type, varIndex);

            // Check if relationship holds/have results
            if (statements.size() > 0) {
                suchThatRelObject->setResultsBoolean(true);
				// Update the synonym table
				resultManager->updateSynonym(argOne->getStringValue(), statements);
            }

            

        }
        // If left arg is synonym, right arg is "_" (Modifies(s,_);
        else if (argOne->getIsSynonym() && argTwo->getIsSynonym() == false && argTwo->getStringValue() == "_") {
			// Set "_" to retrieve all statements and also get all statements of synonym
			std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(argOne->getStringValue());

			std::set<VarIndex> evaluatedV;
			for (StmtSetIterator k = currentStatements.begin(); k != currentStatements.end(); k++) {
				// Store results
				std::set<VarIndex> variables = pkb->getVarsByStmt(*k, type);
				if (variables.size() > 0) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedV.insert(*k);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedV.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedV);
			}
/*
			// Set "_" to retrieve all statements and also get all statements of synonym
            std::set<VarIndex> variables = pkb->getAllVarIndex();
            std::set<StmtNumber> evaluatedS;

            // Get existing results from results table
            std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(argOne->getStringValue());

            // Check if any synonym statements follows any statements
            for (StmtSetIterator cs = currentStatements.begin(); cs != currentStatements.end(); cs++) {
                for (VarIndexSetIterator s = variables.begin(); s != variables.end(); s++) {
                    if (pkb->is(type, *cs, *s)) {
                        evaluatedS.insert(*cs);
						suchThatRelObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
                    }
                }
            }

            // Check if relationship holds/have results
            if (evaluatedS.size() > 0) {
                // Update the results table
				 resultManager->updateSynonym(argOne->getStringValue(), evaluatedS);
            }
 */
        }
        // Both args are synonym (Modifies(a,v));Modifies(s,v)
        else if (argOne->getIsSynonym() && argTwo->getIsSynonym()) {
			// Get current statements
			std::tuple<SynonymString, SynonymString> synonymTuple(argOne->getStringValue(), argTwo->getStringValue());
			std::set<StmtNumber> statements = resultManager->getValuesForSynonym(argOne->getStringValue());
			std::set<VarIndex> variables = resultManager->getValuesForSynonym(argTwo->getStringValue());
			ValueTupleSet evaluatedTupleStatements;

			for (StmtSetIterator k = statements.begin(); k != statements.end(); k++) {
				std::set<VarIndex> variableIndexes = pkb->getVarsByStmt(*k, type);
				for (StmtSetIterator m = variableIndexes.begin(); m != variableIndexes.end(); m++) {
					if (variables.count(*m)) {
						std::tuple<StmtNumber, VarIndex> validTuple(*k, *m);
						evaluatedTupleStatements.insert(validTuple);
						suchThatRelObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedTupleStatements.size() > 0) {
				// Update tuple with evaluation results
				resultManager->updateSynonymTuple(synonymTuple, evaluatedTupleStatements);
			}
/*
			std::set<StmtNumber> test = resultManager->getValuesForSynonym(argOne->getStringValue());
			std::set<VarIndex> test1 = resultManager->getValuesForSynonym(argTwo->getStringValue());
			std::tuple<SynonymString, SynonymString> testTuple (argOne->getStringValue(), argTwo->getStringValue());
			ValueTupleSet testTupleStatements;
			for (StmtSetIterator s1s = test.begin(); s1s != test.end(); s1s++) {
				for (StmtSetIterator s2s = test1.begin(); s2s != test1.end(); s2s++) {
					if (pkb->is(type, *s1s, *s2s)) {
						std::tuple<StmtNumber, VarIndex> validTuple = { *s1s, *s2s };
						testTupleStatements.insert(validTuple);
						suchThatRelObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
					}
				}
			}

			// Check if relationship holds/have results
			if (testTupleStatements.size() > 0) {
				// Update tuple with evaluation results
				resultManager->updateSynonymTuple(testTuple, testTupleStatements);
			}
*/
			/*
			// Get current tuple synonyms 
			SynonymString firstSynonym = argOne->getStringValue();
			SynonymString secondSynonym = argTwo->getStringValue();
			std::tuple<SynonymString, SynonymString> synonymTuple(firstSynonym, secondSynonym);
			std::set<ValueTuple> tupleStatements = resultManager->getValuesForSynonymTuple(synonymTuple);

			// Obtain evaluation results for tuple
			ValueTupleSet evaluatedTupleStatements;
			for (std::set<ValueTuple>::iterator t1 = tupleStatements.begin(); t1 != tupleStatements.end(); t1++) {
				if (pkb->is(type, std::get<0>(*t1), std::get<1>(*t1))) {
					evaluatedTupleStatements.insert(*t1);
					suchThatRelObject->setResultsBoolean(true);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedTupleStatements.size() > 0) {
				// Update tuple with evaluation results
				resultManager->updateSynonymTuple(synonymTuple, evaluatedTupleStatements);
			}
			*/
/*            
			// Retrieve current statements
            std::set<StmtNumber> s1s = resultManager->getValuesForSynonym(argOne->getStringValue());
            std::set<VarIndex> v1s = resultManager->getValuesForSynonym(argTwo->getStringValue());

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
                suchThatRelObject->setResultsBoolean(true);
            }

            // Update the results table
            resultManager->updateSynonym(argOne->getStringValue(), updatedStatements);
            resultManager->updateSynonym(argTwo->getStringValue(), updatedVariables);
*/
        }
    }

	// MODIFIES_P / USES_P relationship:
	else if (type == MODIFIES_P || type == USES_P) {
		// If left arg is 'procedure name', right arg is "x" (Modifies("Giraffe","x"))
		if (argOne->getIsSynonym() == false && argOne->getStringValue() != "_" && argTwo->getIsSynonym() == false && argTwo->getStringValue() != "_") {
			suchThatRelObject->setResultsBoolean(pkb->is(type, pkb->getProcIndex(argOne->getStringValue()), pkb->getVarIndex(argTwo->getStringValue())));
		}
		// If left arg is 'procedure name', right arg is a synonym (Modifies("Giraffe",v))
		else if (argOne->getIsSynonym() == false && argOne->getStringValue() != "_" && argTwo->getIsSynonym()) {
			// Get all statements that modifies variable
			ProcIndex procIndex = pkb->getProcIndex(argOne->getStringValue());
			std::set<VarIndex> variables = pkb->getVarsByProc(procIndex, type);

			// Check if relationship holds/have results
			if (variables.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
				// Update the synonym table
				resultManager->updateSynonym(argTwo->getStringValue(), variables);
			}
/*			
			// Retrieve current variables																							
			std::set<VarIndex> v = resultManager->getValuesForSynonym(argTwo->getStringValue());

			// Get results
			std::set<VarIndex> evaluatedV;
            ProcIndex proc = pkb->getProcIndex(argOne->getStringValue());
			for (StmtSetIterator i = v.begin(); i != v.end(); i++) {
				if (pkb->is(type, proc, *i)) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedV.insert(*i);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedV.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), evaluatedV);
			}
*/
		}
		// If left arg is 'procedure name', right arg is "_" (Modifies("Giraffe",_);
		else if (argOne->getIsSynonym() == false && argOne->getStringValue() != "_" && argTwo->getIsSynonym() == false && argTwo->getStringValue() == "_") {
			// Store results
			std::set<VarIndex> variables = pkb->getVarsByProc(pkb->getProcIndex(argOne->getStringValue()), type);     // Get vars that arg1(integer) follows

			// Check if relationship holds
			if (variables.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
			}
		}
		// If left arg is synonym, right arg is "x" (Modifies(p,"x")); 
		else if (argOne->getIsSynonym() && argTwo->getIsSynonym() == false && argTwo->getStringValue() != "_") {
			// Get all statements that modifies variable
			VarIndex varIndex = pkb->getVarIndex(argTwo->getStringValue());
			std::set<ProcIndex> procedures = pkb->getProcsByVar(type, varIndex);

			// Check if relationship holds/have results
			if (procedures.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
				// Update the synonym table
				resultManager->updateSynonym(argOne->getStringValue(), procedures);
			}
/*			
			// Retrieve current statements																							
			std::set<ProcIndex> p = resultManager->getValuesForSynonym(argOne->getStringValue());

			// Get results
			std::set<ProcIndex> evaluatedP;
            VarIndex var = pkb->getVarIndex(argTwo->getStringValue());
			for (StmtSetIterator i = p.begin(); i != p.end(); i++) {
				if (pkb->is(type, *i, var)) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedP.insert(*i);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}
			// Check if relationship holds/have results
			if (evaluatedP.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedP);
			}	
*/
		}
		// If left arg is synonym, right arg is "_" (Modifies(p,_);
		else if (argOne->getIsSynonym() && argTwo->getIsSynonym() == false && argTwo->getStringValue() == "_") {
			// Set "_" to retrieve all statements and also get all statements of synonym
			std::set<ProcIndex> currentProcedures = resultManager->getValuesForSynonym(argOne->getStringValue());

			std::set<VarIndex> evaluatedV;
			for (StmtSetIterator k = currentProcedures.begin(); k != currentProcedures.end(); k++) {
				// Store results
				std::set<VarIndex> variables = pkb->getVarsByProc(*k, type);
				if (variables.size() > 0) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedV.insert(*k);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedV.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedV);
			}
			
/*
			// Set "_" to retrieve all statements and also get all statements of synonym
			std::set<VarIndex> variables = pkb->getAllVarIndex();
			std::set<ProcIndex> evaluatedP;

			// Get existing results from results table
			std::set<ProcIndex> currentProcedures = resultManager->getValuesForSynonym(argOne->getStringValue());

			// Check if any synonym statements modifies/uses
			for (StmtSetIterator cp = currentProcedures.begin(); cp != currentProcedures.end(); cp++) {
				for (VarIndexSetIterator s = variables.begin(); s != variables.end(); s++) {
					if (pkb->is(type, *cp, *s)) {
						evaluatedP.insert(*cp);
						suchThatRelObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedP.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedP);
			}
*/

		}
		// Both args are synonym (Modifies(p,v));
		else if (argOne->getIsSynonym() && argTwo->getIsSynonym()) {
			// Get current statements
			std::tuple<SynonymString, SynonymString> synonymTuple(argOne->getStringValue(), argTwo->getStringValue());
			std::set<ProcIndex> procedures = resultManager->getValuesForSynonym(argOne->getStringValue());
			std::set<VarIndex> variables = resultManager->getValuesForSynonym(argTwo->getStringValue());
			ValueTupleSet evaluatedTupleStatements;

			for (StmtSetIterator k = procedures.begin(); k != procedures.end(); k++) {
				std::set<VarIndex> retrievedVariables = pkb->getVarsByProc(*k, type);
				for (StmtSetIterator m = retrievedVariables.begin(); m != retrievedVariables.end(); m++) {
					if (variables.count(*m)) {
						std::tuple<ProcIndex, VarIndex> validTuple(*k, *m);
						evaluatedTupleStatements.insert(validTuple);
						suchThatRelObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedTupleStatements.size() > 0) {
				// Update tuple with evaluation results
				resultManager->updateSynonymTuple(synonymTuple, evaluatedTupleStatements);
			}
			
			
/*			
			// Get current tuple synonyms 
			std::tuple<SynonymString, SynonymString> synonymTuple(argOne->getStringValue(), argTwo->getStringValue());
			std::set<ValueTuple> tupleStatements = resultManager->getValuesForSynonymTuple(synonymTuple);

			// Obtain evaluation results for tuple
			ValueTupleSet evaluatedTupleStatements;
			for (std::set<ValueTuple>::iterator t1 = tupleStatements.begin(); t1 != tupleStatements.end(); t1++) {
				if (pkb->is(type, std::get<0>(*t1), std::get<1>(*t1))) {
					evaluatedTupleStatements.insert(*t1);
					suchThatRelObject->setResultsBoolean(true);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedTupleStatements.size() > 0) {
				// Update tuple with evaluation results
				resultManager->updateSynonymTuple(synonymTuple, evaluatedTupleStatements);
			}
			*/
		}
	}
	
	// CALLS / CALLS_STAR relationship:
	else if (type == CALLS || type == CALLS_STAR) {
		// Both are procedure names : Calls("Giraffe","Panda")
		if (argOne->getIsSynonym() == false && argOne->getStringValue() != "_" && argTwo->getIsSynonym() == false && argTwo->getStringValue() != "_") {
			suchThatRelObject->setResultsBoolean(pkb->is(type, pkb->getProcIndex(argOne->getStringValue()), pkb->getProcIndex(argTwo->getStringValue())));
		}
		// arg1 is synonym, arg2 is proc name: Calls(p1,"Panda")
		else if (argOne->getIsSynonym() && argTwo->getIsSynonym() == false && argTwo->getStringValue() != "_") {
			// Store results
			ProcIndex procIndex = pkb->getProcIndex(argTwo->getStringValue());
			std::set<ProcIndex> procedures = pkb->getProcsByProc(procIndex, type);     // Get stmts that arg2(integer) follows

			if (procedures.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), procedures);
			}

/*
			// Retrieve current statements																							
			std::set<ProcIndex> p = resultManager->getValuesForSynonym(argOne->getStringValue());

			// Get results
			std::set<ProcIndex> evaluatedP;
            ProcIndex proc = pkb->getProcIndex(argTwo->getStringValue());
			for (StmtSetIterator i = p.begin(); i != p.end(); i++) {
				if (pkb->is(type, *i, proc)) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedP.insert(*i);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}
			// Check if relationship holds/have results
			if (evaluatedP.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedP);
			}
*/
/*
			// Store results
			std::set<ProcIndex> procedures = pkb->getProcsByProc(argTwo->getStringValue(), type);

			 // Check if relationship holds/have results
			if (procedures.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
			}

			// Update the results table
			resultManager->updateSynonym(argOne->getStringValue(), procedures);
*/
		}
		// arg1 is integer, and arg1 is synonym: Calls("Giraffe", p1)
		else if (argOne->getIsSynonym() == false && argOne->getStringValue() != "_" && argTwo->getIsSynonym()) {
			// Store results
			ProcIndex procIndex = pkb->getProcIndex(argOne->getStringValue());
			std::set<ProcIndex> procedures = pkb->getProcsByProc(type, procIndex);     // Get stmts that arg2(integer) follows

			if (procedures.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
				// Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), procedures);
			}
	/*
			// Retrieve current statements																							
			std::set<ProcIndex> p = resultManager->getValuesForSynonym(argTwo->getStringValue());

			// Get results
			std::set<ProcIndex> evaluatedP;
            ProcIndex proc = pkb->getProcIndex(argOne->getStringValue());
			for (StmtSetIterator i = p.begin(); i != p.end(); i++) {
				if (pkb->is(type, proc, *i)) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedP.insert(*i);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}
			// Check if relationship holds/have results
			if (evaluatedP.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), evaluatedP);
			}
*/
/*
			// Store results
			std::set<ProcIndex> procedures = pkb->getProcsByProc(type, argTwo->getStringValue());

			if (procedures.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
			}

			// Update the results table
			resultManager->updateSynonym(argTwo->getStringValue(), procedures);
*/
		}
		// arg1 is underscore & arg2 is integer: Calls(_,"Giraffe")
		else if (argOne->getIsSynonym() == false && argOne->getStringValue() == "_" && argTwo->getIsSynonym() == false && argTwo->getStringValue() != "_") {
			// Store results
			std::set<ProcIndex> procedures = pkb->getProcsByProc(pkb->getProcIndex(argTwo->getStringValue()), type);

			if (procedures.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
			}
		}
		// arg1 is underscore & arg2 is synonym: Calls(_,p1)
		else if (argOne->getIsSynonym() == false && argOne->getStringValue() == "_" && argTwo->getIsSynonym()) {
			// Set "_" to retrieve all statements and also get all statements of synonym
			std::set<ProcIndex> currentProcedures = resultManager->getValuesForSynonym(argTwo->getStringValue());

			std::set<ProcIndex> evaluatedP;
			for (StmtSetIterator k = currentProcedures.begin(); k != currentProcedures.end(); k++) {
				// Store results
				std::set<ProcIndex> procedures = pkb->getProcsByProc(*k, type);
				if (procedures.size() > 0) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedP.insert(*k);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedP.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), evaluatedP);
			}
			
/*			
			// Retrieve all procedures name for "_"
			std::set<ProcIndex> procedures = pkb->getAllProcIndex();
			std::set<ProcIndex> evaluatedP;

			// Get existing results from results table
			std::set<ProcIndex> currentProcedures = resultManager->getValuesForSynonym(argTwo->getStringValue());

			// Check if any synonym statements follows any statements
			for (StmtSetIterator i = procedures.begin(); i != procedures.end(); i++) {
				for (StmtSetIterator j = currentProcedures.begin(); j != currentProcedures.end(); j++) {
					if (pkb->is(type, *i, *j)) {
						suchThatRelObject->setResultsBoolean(true);
						evaluatedP.insert(*j);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
					}
				}
			}
			// Check if relationship holds/have results
			if (evaluatedP.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argTwo->getStringValue(), evaluatedP);
			}
			
			*/
		}
		// arg1 is proc name & arg2 is underscore: Calls("Giraffe",_)
		else if (argOne->getIntegerValue() > 0 && argTwo->getIsSynonym() == false && argTwo->getStringValue() == "_") {
			// Store results
			std::set<ProcIndex> procedures = pkb->getProcsByProc(type, pkb->getProcIndex(argOne->getStringValue()));    

			if (procedures.size() > 0) {
				suchThatRelObject->setResultsBoolean(true);
			}

		}
		// arg1 is synonym & arg2 is underscore: Calls(p1,_)
		else if (argOne->getIsSynonym() && argTwo->getIsSynonym() == false && argTwo->getStringValue() == "_") {
			// Set "_" to retrieve all statements and also get all statements of synonym
			std::set<ProcIndex> currentProcedures = resultManager->getValuesForSynonym(argOne->getStringValue());

			std::set<ProcIndex> evaluatedP;
			for (StmtSetIterator k = currentProcedures.begin(); k != currentProcedures.end(); k++) {
				// Store results
				std::set<ProcIndex> procedures = pkb->getProcsByProc(type, *k);
				if (procedures.size() > 0) {
					suchThatRelObject->setResultsBoolean(true);
					evaluatedP.insert(*k);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedP.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedP);
			}
/*		
			
			// Set "_" to retrieve all statements and also get all statements of synonym
			std::set<ProcIndex> procedures = pkb->getAllProcIndex();
			std::set<StmtNumber> evaluatedP;

			// Get existing results from results table
			std::set<ProcIndex> currentProcedures = resultManager->getValuesForSynonym(argOne->getStringValue());

			// Check if any synonym statements follows any statements
			for (StmtSetIterator cs = currentProcedures.begin(); cs != currentProcedures.end(); cs++) {
				for (StmtSetIterator s = procedures.begin(); s != procedures.end(); s++) {
					if (pkb->is(type, *cs, *s)) {
						suchThatRelObject->setResultsBoolean(true);
						evaluatedP.insert(*cs);
						if (isStopEvaluation) {
						return suchThatRelObject;
						}
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedP.size() > 0) {
				// Update the results table
				resultManager->updateSynonym(argOne->getStringValue(), evaluatedP);
			}

			
			*/
		}
		// Both arguments are a synonym: Calls(p1,p2)
		else if (argOne->getIsSynonym() && argTwo->getIsSynonym()) {
			// Get current statements
			std::tuple<SynonymString, SynonymString> synonymTuple(argOne->getStringValue(), argTwo->getStringValue());
			std::set<ProcIndex> procedures1 = resultManager->getValuesForSynonym(argOne->getStringValue());
			std::set<ProcIndex> procedures2 = resultManager->getValuesForSynonym(argTwo->getStringValue());
			ValueTupleSet evaluatedTupleStatements;

			for (StmtSetIterator k = procedures1.begin(); k != procedures1.end(); k++) {
				std::set<ProcIndex> retrievedProcedures = pkb->getProcsByProc(type, *k);
				for (StmtSetIterator m = retrievedProcedures.begin(); m != retrievedProcedures.end(); m++) {
					if (procedures2.count(*m)) {
						std::tuple<ProcIndex, ProcIndex> validTuple(*k, *m);
						evaluatedTupleStatements.insert(validTuple);
						suchThatRelObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
					}
				}
			}

			// Check if relationship holds/have results
			if (evaluatedTupleStatements.size() > 0) {
				// Update tuple with evaluation results
				resultManager->updateSynonymTuple(synonymTuple, evaluatedTupleStatements);
			}
			
/*			
			// Get current tuple synonyms 
			std::tuple<SynonymString, SynonymString> synonymTuple(argOne->getStringValue(), argTwo->getStringValue());
			std::set<ValueTuple> tupleProcedures = resultManager->getValuesForSynonymTuple(synonymTuple);

			// Obtain evaluation results for tuple
			ValueTupleSet evaluatedTupleProcedures;
			for (std::set<ValueTuple>::iterator t1 = tupleProcedures.begin(); t1 != tupleProcedures.end(); t1++) {
				if (pkb->is(type, std::get<0>(*t1), std::get<1>(*t1))) {
					evaluatedTupleProcedures.insert(*t1);
					suchThatRelObject->setResultsBoolean(true);
					if (isStopEvaluation) {
						return suchThatRelObject;
					}
				}
			}
			// Check if relationship holds/have results
			if (evaluatedTupleProcedures.size() > 0) {
				// Update tuple with evaluation results
				resultManager->updateSynonymTuple(synonymTuple, evaluatedTupleProcedures);
			}
*/
/*			
			// Retrieve current statements
			std::set<ProcIndex> p1s = resultManager->getValuesForSynonym(argOne->getStringValue());
			std::set<ProcIndex> p2s = resultManager->getValuesForSynonym(argTwo->getStringValue());

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
				suchThatRelObject->setResultsBoolean(true);
			}

			// Update the results table
			resultManager->updateSynonym(argOne->getStringValue(), updatedProcedures1);
			resultManager->updateSynonym(argTwo->getStringValue(), updatedProcedures2);
*/
		}
		// arg1 is underscore & arg2 is underscore: Calls(_,_);
		else if (argOne->getIsSynonym() == false && argOne->getStringValue() == "_" && argTwo->getIsSynonym() == false && argTwo->getStringValue() == "_") {
			// Retrieve wild card procedure names
			std::set<ProcIndex> procedures1 = pkb->getAllProcIndex();
			std::set<ProcIndex> procedures2 = pkb->getAllProcIndex();

			// Obtain evaluation results
			for (StmtSetIterator s1s = procedures1.begin(); s1s != procedures1.end(); s1s++) {
				for (StmtSetIterator s2s = procedures2.begin(); s2s != procedures2.end(); s2s++) {
					if (pkb->is(type, *s1s, *s2s)) {
						suchThatRelObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return suchThatRelObject;
						}
					}
				}
			}
	
		}
	}
    
	return suchThatRelObject;
}

ClauseWithObject* QueryEvaluator::evaluateWith(ClauseWithObject* withObject, bool isStopEvaluation) {
	ClauseWithRefObject* leftObj = withObject->getRefObject1();
	ClauseWithRefObject* rightObj = withObject->getRefObject2();

	// left side is = synonym.attrName (attrRef)
	if (leftObj->getRefType() == ATTRREF) {
		// left side is = synonynm.procName 
		if (leftObj->getAttrType() == AttrType::PROC_NAME) {
			// if synonym = procedure -> p.procName
			if (leftObj->getEntityType() == PROCEDURE) {
				// right side is IDENTIFIER; p.procName = "First"
				if (rightObj->getRefType() == IDENTIFIER) {
					// Check if procedure name exists
					if (pkb->isProcExist(rightObj->getStringValue())) {
						// Get Procedure Index
						ProcIndex procIndex = pkb->getProcIndex(rightObj->getStringValue());

						// Get current results for procedure synonym
						std::set<ProcIndex> procedures = resultManager->getValuesForSynonym(leftObj->getSynonym());

						// Check if contain procedure identifier
						std::set<ProcIndex> evaluatedProcedures;
						if (procedures.count(procIndex)) {
							evaluatedProcedures.insert(procIndex);
						}

						// Check relationship holds
						if (evaluatedProcedures.size() > 0) {
							withObject->setResultsBoolean(true);
							// Update the result table
							resultManager->updateSynonym(leftObj->getSynonym(), evaluatedProcedures);
						}					
					}
				}
				// right side is ATTRREF; p.procName = synonym.attrName
				else if (rightObj->getRefType() == ATTRREF) {
					// right side is procName; p1.procName = synonym.procName;
					if (rightObj->getAttrType() == AttrType::PROC_NAME) {
						// if right synonym = procedure -> p1.procName = p2.procName
						if (rightObj->getEntityType() == PROCEDURE) {
							// Get current results
							std::set<ProcIndex> procedures1 = resultManager->getValuesForSynonym(leftObj->getSynonym());
							std::set<ProcIndex> procedures2 = resultManager->getValuesForSynonym(rightObj->getSynonym());

							// Intersect both sets
							std::set<ProcIndex> evaluatedProcedures;
							set_intersection(procedures1.begin(), procedures1.end(), procedures2.begin(), procedures2.end(),
								std::inserter(evaluatedProcedures, evaluatedProcedures.begin()));

							// Check relationships holds
							if (evaluatedProcedures.size() > 0) {
								withObject->setResultsBoolean(true);
								// Update the result table
								resultManager->updateSynonym(leftObj->getSynonym(), evaluatedProcedures);
							}						
						}
						// if right synonym = call -> p1.procName = call.procName
						else if (rightObj->getEntityType() == CALL) {
							// Get current results
							std::set<ProcIndex> procedures = resultManager->getValuesForSynonym(leftObj->getSynonym());
							std::set<StmtNumber> callStatements = resultManager->getValuesForSynonym(rightObj->getSynonym());

							// get all the procedures each call statement calls
							std::set<ProcIndex> callProcedureIndexs;
							for (StmtSetIterator s1 = callStatements.begin(); s1 != callStatements.end(); s1++) {
								callProcedureIndexs.insert(pkb->getProcByCall(*s1));
							}

							// Intersect both sets
							std::set<ProcIndex> evaluatedProcedures;
							set_intersection(procedures.begin(), procedures.end(), callProcedureIndexs.begin(), callProcedureIndexs.end(),
								std::inserter(evaluatedProcedures, evaluatedProcedures.begin()));

							// Check relationships holds
							if (evaluatedProcedures.size() > 0) {
								withObject->setResultsBoolean(true);
								// Update the result table
								resultManager->updateSynonym(leftObj->getSynonym(), evaluatedProcedures);
							}					
						}
					}
					// right side is varName; p1.procName = synonym.varName
					else if (rightObj->getAttrType() == AttrType::VAR_NAME) {
						// if right synonym = variable -> p1.procName = v.varName
						if (rightObj->getEntityType() == VARIABLE) {
							// get all the results for p synonym and v synonym
							std::set<ProcIndex> procedures = resultManager->getValuesForSynonym(leftObj->getSynonym());
							std::set<VarIndex> variables = resultManager->getValuesForSynonym(rightObj->getSynonym());

							// get all the procedure names for p synonym
							std::set<ProcName> procedureNames;
							for (StmtSetIterator p1 = procedures.begin(); p1 != procedures.end(); p1++) {
								procedureNames.insert(pkb->getProcName(*p1));
							}

							// get all the variable names and check whether it exists inside p synonym
							// if exist, add the procedure index of the name inside
							std::set<ProcIndex> evaluatedProcedures({});
							for (StmtSetIterator v1 = variables.begin(); v1 != variables.end(); v1++) {
								if (procedureNames.count(pkb->getVarName(*v1))) {
									evaluatedProcedures.insert(pkb->getProcIndex(pkb->getVarName(*v1)));
								}
							}

							// Check relationships holds
							if (evaluatedProcedures.size() > 0) {
								withObject->setResultsBoolean(true);
								// Update the result table
								resultManager->updateSynonym(leftObj->getSynonym(), evaluatedProcedures);
							}					
						}
					}
				}
			}
			// if synonym = call -> c.procName
			else if (leftObj->getEntityType() == CALL) {
				// right side is IDENTIFIER; c.procName = "First"
				if (rightObj->getRefType() == IDENTIFIER) {
					// Check if procedure name exists
					if (pkb->isProcExist(rightObj->getStringValue())) {
						// Get Procedure Index
						ProcIndex procIndex = pkb->getProcIndex(rightObj->getStringValue());

						// get the call statements for call synonym first
						std::set<StmtNumber> callStatements = resultManager->getValuesForSynonym(leftObj->getSynonym());

						// get all the procedures index each call statement calls
						// compare with the identifier procedure index
						std::set<StmtNumber> evaluatedCallStatements;
						for (StmtSetIterator s1 = callStatements.begin(); s1 != callStatements.end(); s1++) {
							if (pkb->getProcByCall(*s1) == procIndex) {
								evaluatedCallStatements.insert(*s1);
							}
						}

						// Check if relationship holds
						if (evaluatedCallStatements.size() > 0) {
							withObject->setResultsBoolean(true);
							// Update the result table
							resultManager->updateSynonym(leftObj->getSynonym(), evaluatedCallStatements);
						}						
					}
				}
				// right side is ATTRREF; c.procName = synonym.attrName
				else if (rightObj->getRefType() == ATTRREF) {
					// right side is procName; c.procName = synonym.procName;
					if (rightObj->getAttrType() == AttrType::PROC_NAME) {
						// if right synonym = procedure; c.procName = p.procName
						if (rightObj->getEntityType() == PROCEDURE) {
							// get all the results for call and procedure synonym
							std::set<StmtNumber> callStatements = resultManager->getValuesForSynonym(leftObj->getSynonym());
							std::set<ProcIndex> procedures = resultManager->getValuesForSynonym(rightObj->getSynonym());

							// get all the procedures index each call statement calls
							// compare with the identifier procedure index
							std::set<ProcIndex> callProceduresIndex;
							for (StmtSetIterator s1 = callStatements.begin(); s1 != callStatements.end(); s1++) {
								callProceduresIndex.insert(pkb->getProcByCall(*s1));
							}

							// Intersect two sets x2 and get the final procedure indexes
							std::set<ProcIndex> evaluatedProceduresIndex;
							set_intersection(procedures.begin(), procedures.end(), callProceduresIndex.begin(), callProceduresIndex.end(),
								std::inserter(evaluatedProceduresIndex, evaluatedProceduresIndex.begin()));

							// Check the current call statements which has the final procedure index
							std::set<StmtNumber> evaluatedCallStatements;
							for (StmtSetIterator s1 = callStatements.begin(); s1 != callStatements.end(); s1++) {
								if (evaluatedProceduresIndex.count(pkb->getProcByCall(*s1))) {
									evaluatedCallStatements.insert(*s1);
								}
							}

							// Check relationships holds
							if (evaluatedCallStatements.size() > 0) {
								withObject->setResultsBoolean(true);
								// Update the result table
								resultManager->updateSynonym(leftObj->getSynonym(), evaluatedCallStatements);
							}							
						}
						// if right synonym = call -> c1.procName = c2.procName
						else if (rightObj->getEntityType() == CALL) {
							// get the call statements all
							std::set<StmtNumber> callStatements1 = resultManager->getValuesForSynonym(leftObj->getSynonym());
							std::set<StmtNumber> callStatements2 = resultManager->getValuesForSynonym(rightObj->getSynonym());

							// get all the procedures each call statement calls
							std::set<ProcIndex> procedures1;
							for (StmtSetIterator s1 = callStatements1.begin(); s1 != callStatements1.end(); s1++) {
								procedures1.insert(pkb->getProcByCall(*s1));
							}

							// get all the procedures each call statement calls
							std::set<ProcIndex> procedures2;
							for (StmtSetIterator s2 = callStatements2.begin(); s2 != callStatements2.end(); s2++) {
								procedures2.insert(pkb->getProcByCall(*s2));
							}

							// Intersect two sets x2 and get the final procedure indexes
							std::set<ProcIndex> evaluatedProceduresIndex;
							set_intersection(procedures1.begin(), procedures1.end(), procedures2.begin(), procedures2.end(),
								std::inserter(evaluatedProceduresIndex, evaluatedProceduresIndex.begin()));

							// Check the current call statements which has the final procedure index
							std::set<StmtNumber> evaluatedCallStatements;
							for (StmtSetIterator s1 = callStatements1.begin(); s1 != callStatements1.end(); s1++) {
								if (evaluatedProceduresIndex.count(pkb->getProcByCall(*s1))) {
									evaluatedCallStatements.insert(*s1);
								}
							}
							// Check relationships holds
							if (evaluatedCallStatements.size() > 0) {
								withObject->setResultsBoolean(true);
								// Update the result table
								resultManager->updateSynonym(leftObj->getSynonym(), evaluatedCallStatements);
							}						
						}
					}
					// right side is varName; c.procName = synonym.varName
					else if (rightObj->getAttrType() == AttrType::VAR_NAME) {
						// right side is variable synonym; c.procName = v.varName;
						if (rightObj->getEntityType() == VARIABLE) {
							// get all the results for call synonym and variable synonym
							std::set<StmtNumber> callStatements = resultManager->getValuesForSynonym(leftObj->getSynonym());
							std::set<VarIndex> variables = resultManager->getValuesForSynonym(rightObj->getSynonym());

							// get all the procedure names for call synonym
							std::set<ProcName> procedureNames;
							for (StmtSetIterator s1 = callStatements.begin(); s1 != callStatements.end(); s1++) {
								procedureNames.insert(pkb->getProcName(pkb->getProcByCall(*s1)));
							}

							// get all the variable names and check whether it exists inside p synonym
							// if exist, add the procedure index of the name inside
							std::set<ProcIndex> evaluatedProcedures;
							for (StmtSetIterator v1 = variables.begin(); v1 != variables.end(); v1++) {
								if (procedureNames.count(pkb->getVarName(*v1))) {
									evaluatedProcedures.insert(pkb->getProcIndex(pkb->getVarName(*v1)));
								}
							}

							// Check the current call statements which has the final procedure index
							std::set<StmtNumber> evaluatedCallStatements;
							for (StmtSetIterator s1 = callStatements.begin(); s1 != callStatements.end(); s1++) {
								if (evaluatedProcedures.count(pkb->getProcByCall(*s1))) {
									evaluatedCallStatements.insert(*s1);
								}
							}

							// Check relationships holds
							if (evaluatedCallStatements.size() > 0) {
								withObject->setResultsBoolean(true);
								// Update the result table
								resultManager->updateSynonym(leftObj->getSynonym(), evaluatedCallStatements);
							}							
						}
					}
				}
			}
		}
		// left side is = synonym.stmt#
		else if (leftObj->getAttrType() == AttrType::STMT_NO) {
			// right side is INTEGER; s.stmt# = 3
			if (rightObj->getRefType() == INTEGER) {
				// Check if statement line exists
				if (pkb->isStmtExist(rightObj->getIntegerValue())) {
					// Store statement
					StmtNumber stmtNumber = rightObj->getIntegerValue();

					// Get current results for left synonym
					std::set<StmtNumber> statements = resultManager->getValuesForSynonym(leftObj->getSynonym());

					// Check if statement line exist
					std::set<StmtNumber> evaluatedStatement;
					if (statements.count(stmtNumber)) {
						evaluatedStatement.insert(stmtNumber);

					}

					// Check relationship if holds
					if (evaluatedStatement.size() > 0) {
						withObject->setResultsBoolean(true);
						// Update the result table
						resultManager->updateSynonym(leftObj->getSynonym(), evaluatedStatement);
					}					
				}
			}
			// right side is ATTRREF; s.stmt# = synonym.attrName
			else if (rightObj->getRefType() == ATTRREF) {
				// right side is value; s.stmt# = synonym.value
				if (rightObj->getAttrType() == AttrType::VALUE) {
					// right side is constant synonym; s.stmt# = c.value
					if (rightObj->getEntityType() == CONSTANT) {
						// Get current results
						std::set<StmtNumber> statements = resultManager->getValuesForSynonym(leftObj->getSynonym());
						std::set<Constant> constants = resultManager->getValuesForSynonym(rightObj->getSynonym());

						// Intersect both sets
						std::set<StmtNumber> evaluatedStatements;
						set_intersection(statements.begin(), statements.end(), constants.begin(), constants.end(),
							std::inserter(evaluatedStatements, evaluatedStatements.begin()));

						// Check relationship is true
						if (evaluatedStatements.size() > 0) {
							withObject->setResultsBoolean(true);
							// Update the result table
							resultManager->updateSynonym(leftObj->getSynonym(), evaluatedStatements);
						}					
					}
				}
				// right side is stmt#; s.stmt# = a.stmt#
				else if (rightObj->getAttrType() == AttrType::STMT_NO) {
					std::set<StmtNumber> statements1 = resultManager->getValuesForSynonym(leftObj->getSynonym());
					std::set<StmtNumber> statements2 = resultManager->getValuesForSynonym(rightObj->getSynonym());

					// Intersect both sets
					std::set<StmtNumber> evaluatedStatements;
					set_intersection(statements1.begin(), statements1.end(), statements2.begin(), statements2.end(),
						std::inserter(evaluatedStatements, evaluatedStatements.begin()));

					// Check relationship is true
					if (evaluatedStatements.size() > 0) {
						withObject->setResultsBoolean(true);
						// Update the result table
						resultManager->updateSynonym(leftObj->getSynonym(), evaluatedStatements);
					}					
				}
			}
		}
		// left side is = synonym.varName
		else if (leftObj->getAttrType() == AttrType::VAR_NAME) {
			// right side is IDENTIFIER; v.varName = "x"
			if (rightObj->getRefType() == IDENTIFIER) {
				// Check variable exists
				if (pkb->isVarExist(rightObj->getStringValue())) {
					// Store variable name
					VarName varName = rightObj->getStringValue();

					// Get current variable synonym results
					std::set<VarIndex> variables = resultManager->getValuesForSynonym(leftObj->getSynonym());

					// Check if variable synonym results contain identifier
					std::set<VarIndex> evaluatedVariables;
					if (variables.count(pkb->getVarIndex(varName))) {
						evaluatedVariables.insert(pkb->getVarIndex(varName));
					}

					// Check if relationship holds
					if (evaluatedVariables.size() > 0) {
						withObject->setResultsBoolean(true);
						// Update the result table
						resultManager->updateSynonym(leftObj->getSynonym(), evaluatedVariables);
					}
				}
			}
			// right side is ATTRREF; v.varName = synonym.attrName
			else if (rightObj->getRefType() == ATTRREF) {
				// right side is procName; v.varName = p.procName;
				if (rightObj->getAttrType() == AttrType::PROC_NAME) {
					// Get current results index
					std::set<VarIndex> variables = resultManager->getValuesForSynonym(leftObj->getSynonym());
					std::set<ProcIndex> procedures = resultManager->getValuesForSynonym(rightObj->getSynonym());

					// Get current variables names
					std::set<VarName> variablesNames;
					for (StmtSetIterator v1 = variables.begin(); v1 != variables.end(); v1++) {
						variablesNames.insert(pkb->getVarName(*v1));
					}

					// Get current procedures names
					std::set<ProcName> proceduresNames;
					for (StmtSetIterator p1 = procedures.begin(); p1 != procedures.end(); p1++) {
						proceduresNames.insert(pkb->getProcName(*p1));
					}

					// Intersect two sets x2
					std::set<VarName> evaluatedVariablesNames;
					set_intersection(variablesNames.begin(), variablesNames.end(), proceduresNames.begin(), proceduresNames.end(),
						std::inserter(evaluatedVariablesNames, evaluatedVariablesNames.begin()));


					// Check the current variables index which has the final variable names
					std::set<VarIndex> evaluatedVariables;
					for (StmtSetIterator v1 = variables.begin(); v1 != variables.end(); v1++) {
						if (evaluatedVariablesNames.count(pkb->getVarName(*v1))) {
							evaluatedVariables.insert(*v1);
						}
					}

					// Check relationships holds
					if (evaluatedVariables.size() > 0) {
						withObject->setResultsBoolean(true);
						// Update the result table
						resultManager->updateSynonym(leftObj->getSynonym(), evaluatedVariables);
					}					
				}
				// right side is varName; v1.varName = v2.varName;
				else if (rightObj->getAttrType() == AttrType::VAR_NAME) {
					std::set<VarIndex> variables1 = resultManager->getValuesForSynonym(leftObj->getSynonym());
					std::set<VarIndex> variables2 = resultManager->getValuesForSynonym(rightObj->getSynonym());

					// Intersect both sets
					std::set<VarIndex> evaluatedVariables;
					set_intersection(variables1.begin(), variables1.end(), variables2.begin(), variables2.end(),
						std::inserter(evaluatedVariables, evaluatedVariables.begin()));

					// Check relationship is true
					if (evaluatedVariables.size() > 0) {
						withObject->setResultsBoolean(true);
						// Update the result table
						resultManager->updateSynonym(leftObj->getSynonym(), evaluatedVariables);
					}					
				}
			}
		}
		// left side is = synonym.value
		else if (leftObj->getAttrType() == AttrType::VALUE) {
			// right side is INTEGER; c.value = 3
			if (rightObj->getRefType() == INTEGER) {
				// Store integer
				Constant constant = rightObj->getIntegerValue();

				// Get current results
				std::set<Constant> constants = resultManager->getValuesForSynonym(leftObj->getSynonym());

				// Check if integer exist in results
				std::set<Constant> evaluatedConstants;
				if (constants.count(constant)) {
					evaluatedConstants.insert(constant);

				}

				// Check relationship if holds
				if (evaluatedConstants.size() > 0) {
					withObject->setResultsBoolean(true);
					// Update the result table
					resultManager->updateSynonym(leftObj->getSynonym(), evaluatedConstants);
				}				
			}
			// right side is ATTRREF; c.value = synonym.attrName
			else if (rightObj->getRefType() == ATTRREF) {
				// right side is value; c1.value = synonym.value
				if (rightObj->getAttrType() == AttrType::VALUE) {
					// right side is constant synonym; c1.value = c2.value
					if (rightObj->getEntityType() == CONSTANT) {
						// Get current results
						std::set<Constant> constants1 = resultManager->getValuesForSynonym(leftObj->getSynonym());
						std::set<Constant> constants2 = resultManager->getValuesForSynonym(rightObj->getSynonym());

						// Intersect both sets
						std::set<Constant> evaluatedConstants;
						set_intersection(constants1.begin(), constants1.end(), constants2.begin(), constants2.end(),
							std::inserter(evaluatedConstants, evaluatedConstants.begin()));

						// Check relationship is true
						if (evaluatedConstants.size() > 0) {
							withObject->setResultsBoolean(true);
							// Update the result table
							resultManager->updateSynonym(leftObj->getSynonym(), evaluatedConstants);
						}					
					}
				}
				// right side is stmt#; c.value = s.stmt#
				else if (rightObj->getAttrType() == AttrType::STMT_NO) {
					// Get current results
					std::set<Constant> constants = resultManager->getValuesForSynonym(leftObj->getSynonym());
					std::set<StmtNumber> statements = resultManager->getValuesForSynonym(rightObj->getSynonym());

					// Intersect both sets
					std::set<Constant> evaluatedConstants;
					set_intersection(constants.begin(), constants.end(), statements.begin(), statements.end(),
						std::inserter(evaluatedConstants, evaluatedConstants.begin()));

					// Check relationship is true
					if (evaluatedConstants.size() > 0) {
						withObject->setResultsBoolean(true);
						// Update the result table
						resultManager->updateSynonym(leftObj->getSynonym(), evaluatedConstants);
					}					
				}
			}
		}
	}

	// left side is = synonym (prog_line)
	else if (leftObj->getRefType() == SYNONYM) {
		// right side is INTEGER; n = 10;
		if (rightObj->getRefType() == INTEGER) {
			// Check if statement line exists
			if (pkb->isStmtExist(rightObj->getIntegerValue())) {
				// Store integer
				StmtNumber stmtNumber = rightObj->getIntegerValue();

				// Get current results
				std::set<StmtNumber> lines = resultManager->getValuesForSynonym(leftObj->getStringValue());

				// Check if current lines results contain integer
				std::set<StmtNumber> evaluatedLines;
				if (lines.count(stmtNumber)) {
					evaluatedLines.insert(stmtNumber);
				}

				// Check relationship is true
				if (evaluatedLines.size() > 0) {
					withObject->setResultsBoolean(true);
					// Update the result table
					resultManager->updateSynonym(leftObj->getStringValue(), evaluatedLines);
				}			
			}
		}
		// right side is SYNONYM; n = n1;
		else if (rightObj->getRefType() == SYNONYM) {
			// Get current results
			std::set<StmtNumber> lines1 = resultManager->getValuesForSynonym(leftObj->getStringValue());
			std::set<StmtNumber> lines2 = resultManager->getValuesForSynonym(rightObj->getStringValue());

			// Intersect both sets
			std::set<Constant> evaluatedLines;
			set_intersection(lines1.begin(), lines1.end(), lines2.begin(), lines2.end(),
				std::inserter(evaluatedLines, evaluatedLines.begin()));

			// Check relationship is true
			if (evaluatedLines.size() > 0) {
				withObject->setResultsBoolean(true);
				// Update the results table
				resultManager->updateSynonym(leftObj->getStringValue(), evaluatedLines);
			}		
		}
		// right side is ATTRREF; n = synonym.attrName
		else if (rightObj->getRefType() == ATTRREF) {
			// right side is stmt#; n = s.stmt#
			if (rightObj->getAttrType() == AttrType::STMT_NO) {
				// Get current results
				std::set<StmtNumber> lines = resultManager->getValuesForSynonym(leftObj->getStringValue());
				std::set<StmtNumber> statements = resultManager->getValuesForSynonym(rightObj->getSynonym());

				// Intersect both sets
				std::set<Constant> evaluatedLines;
				set_intersection(lines.begin(), lines.end(), statements.begin(), statements.end(),
					std::inserter(evaluatedLines, evaluatedLines.begin()));

				// Check relationship is true
				if (evaluatedLines.size() > 0) {
					withObject->setResultsBoolean(true);
					// Update the results table
					resultManager->updateSynonym(leftObj->getStringValue(), evaluatedLines);
				}				
			}
			// right side is value; n = c.value;
			else if (rightObj->getAttrType() == AttrType::VALUE) {
				// Get current results
				std::set<StmtNumber> lines = resultManager->getValuesForSynonym(leftObj->getStringValue());
				std::set<Constant> constants = resultManager->getValuesForSynonym(rightObj->getSynonym());

				// Intersect both sets
				std::set<Constant> evaluatedLines;
				set_intersection(lines.begin(), lines.end(), constants.begin(), constants.end(),
					std::inserter(evaluatedLines, evaluatedLines.begin()));

				// Check relationship is true
				if (evaluatedLines.size() > 0) {
					withObject->setResultsBoolean(true);
					// Update the results table
					resultManager->updateSynonym(leftObj->getStringValue(), evaluatedLines);
				}				
			}
		}
	}
	
	return withObject;
}

ClausePatternObject* QueryEvaluator::evaluatePattern(ClausePatternObject* patternObject, bool isStopEvaluation) {
    EntityType patternType = patternObject->getPatternType();
    EntityType firstArgType = patternObject->getFirstArgumentType();
    std::string patternSynonymArg = patternObject->getPatternSynonymArgument();
    std::string firstArg = patternObject->getFirstArgument();
    std::string secondArg = patternObject->getSecondArgument();
    std::string thirdArg = patternObject->getThirdArgument();
    bool isFirstArgSynonym = patternObject->getIsFirstArgSynonym();

    // ASSIGN pattern:
    if (patternType == ASSIGN) {
        // pattern a(<firstArg>,_)
        if (secondArg == "_") {
            // pattern a(v,_)
            if (isFirstArgSynonym) {
				// Get current tuple synonyms 
				std::tuple<SynonymString, SynonymString> synonymTuple(patternSynonymArg, firstArg);
				std::set<ValueTuple> tuplePatterns = resultManager->getValuesForSynonymTuple(synonymTuple);

				// Obtain evaluation results for tuple
				ValueTupleSet evaluatedTuplePatterns;
				for (std::set<ValueTuple>::iterator t1 = tuplePatterns.begin(); t1 != tuplePatterns.end(); t1++) {
					if (pkb->is(MODIFIES, std::get<0>(*t1), std::get<1>(*t1))) {
						evaluatedTuplePatterns.insert(*t1);
						patternObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return patternObject;
						}
					}
				}
				// Check if relationship holds/have results
				if (evaluatedTuplePatterns.size() > 0) {
					// Update tuple with evaluation results
					resultManager->updateSynonymTuple(synonymTuple, evaluatedTuplePatterns);
				}			
				/*
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
                    patternObject->setResultsBoolean(true);
                }

                // Update the results table
                resultManager->updateSynonym(patternSynonymArg, updatedStatements);
                resultManager->updateSynonym(firstArg, updatedVariables);
				*/
            }
            // pattern a(_,_)
            else if (firstArg == "_") {
                // Check if theres any statements number inside
                if (resultManager->getValuesForSynonym(patternSynonymArg).size() > 0) {
                    patternObject->setResultsBoolean(true);
                }
            }
            // pattern a("x",_)
            else if (firstArgType == VARIABLE) {
                // Retrieve existing pattern synonym statements
                std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(patternSynonymArg);

                // Evaluated true statements
                std::set<StmtNumber> evaluatedS;

                // Check all existing pattern synonym statements if it modifies the 'variable'
                VarIndex var = pkb->getVarIndex(firstArg);
                for (StmtSetIterator i = currentStatements.begin(); i != currentStatements.end(); i++) {
                    // Check if the existing statement modifies the 'variable'
                    if (pkb->is(MODIFIES, *i, var)) {
                        evaluatedS.insert(*i);
						patternObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return patternObject;
						}
                    }
                }
				// Check if relationship holds/have results
				if (evaluatedS.size() > 0) {
					// Update the results table
					resultManager->updateSynonym(patternSynonymArg, evaluatedS);
				}             
            }
        }
        // Means Pattern must be Pattern a(<firstArg>,"_<constant/variable>_")	
        else {
            // pattern a(v,"_<constant/variable>_")
            if (isFirstArgSynonym) {
				// Get current tuple synonyms 
				std::tuple<SynonymString, SynonymString> synonymTuple(patternSynonymArg, firstArg);
				std::set<ValueTuple> tuplePatterns = resultManager->getValuesForSynonymTuple(synonymTuple);

				// Obtain evaluation results for tuple
				ValueTupleSet evaluatedTuplePatterns;
				for (std::set<ValueTuple>::iterator t1 = tuplePatterns.begin(); t1 != tuplePatterns.end(); t1++) {
					if (pkb->is(MODIFIES, std::get<0>(*t1), std::get<1>(*t1))) {
						if (pkb->isAssignContainsPattern(std::get<0>(*t1), to_tokens(secondArg))) {
							evaluatedTuplePatterns.insert(*t1);
							patternObject->setResultsBoolean(true);
							if (isStopEvaluation) {
								return patternObject;
							}							
						}

					}
				}
				// Check if relationship holds/have results
				if (evaluatedTuplePatterns.size() > 0) {
					// Update tuple with evaluation results
					resultManager->updateSynonymTuple(synonymTuple, evaluatedTuplePatterns);
				}
				
 /*               
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
                            if (pkb->isAssignContainsPattern(*cs, to_tokens(secondArg))) {
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
                    patternObject->setResultsBoolean(true);
                }

                // Update the results table
                resultManager->updateSynonym(patternSynonymArg, updatedStatements);
                resultManager->updateSynonym(firstArg, updatedVariables);
*/
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
                            if (pkb->isAssignContainsPattern(*cs, to_tokens(secondArg))) {
                                evaluatedPatternSynonymStatements.insert(*cs);
								patternObject->setResultsBoolean(true);
								if (isStopEvaluation) {
									return patternObject;
								}
                                break;
                            }
                        }
                    }
                }
				// Check if relationship holds/have results
				if (evaluatedPatternSynonymStatements.size() > 0) {
					// Update the results table
					resultManager->updateSynonym(patternSynonymArg, evaluatedPatternSynonymStatements);
				}               
            }
            // pattern a("x","_<constant/variable>_")
            else if (firstArgType == VARIABLE) {
                // Retrieve existing pattern synonym statements
                std::set<StmtNumber> currentStatements = resultManager->getValuesForSynonym(patternSynonymArg);

                // Evaluated true statements
                std::set<StmtNumber> evaluatedS;

                // Check all existing pattern synonym statements if it modifies the 'variable'
                VarIndex var = pkb->getVarIndex(firstArg);
                for (StmtSetIterator i = currentStatements.begin(); i != currentStatements.end(); i++) {
                    // Check if the existing statement modifies the 'variable'
                    if (pkb->is(MODIFIES, *i, var)) {
                        // If yes, check if this statement uses the second argument subexpression
                        if (pkb->isAssignContainsPattern(*i, to_tokens(secondArg))) {
                            evaluatedS.insert(*i);
							patternObject->setResultsBoolean(true);
							if (isStopEvaluation) {
								return patternObject;
							}
                        }
                    }
                }
				// Check if relationship holds/have results
				if (evaluatedS.size() > 0) {
					// Update the results table
					resultManager->updateSynonym(patternSynonymArg, evaluatedS);
				}
                
            }
        }
    }
	// WHILE pattern:
	else if (patternType == WHILE) {
		// Pattern w(<first arg>,_)
		if (secondArg == "_") {
			// Pattern w(v,_)
			if (isFirstArgSynonym) {
				// Get current tuple synonyms 
				std::tuple<SynonymString, SynonymString> synonymTuple(patternSynonymArg, firstArg);
				std::set<ValueTuple> tuplePatterns = resultManager->getValuesForSynonymTuple(synonymTuple);

				// Obtain evaluation results for tuple
				ValueTupleSet evaluatedTuplePatterns;
				for (std::set<ValueTuple>::iterator t1 = tuplePatterns.begin(); t1 != tuplePatterns.end(); t1++) {
					if (pkb->isWhilePattern(std::get<0>(*t1), std::get<1>(*t1))) {
						evaluatedTuplePatterns.insert(*t1);
						patternObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return patternObject;
						}
					}
				}
				// Check if relationship holds/have results
				if (evaluatedTuplePatterns.size() > 0) {
					// Update tuple with evaluation results
					resultManager->updateSynonymTuple(synonymTuple, evaluatedTuplePatterns);
				}				
/*
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
					patternObject->setResultsBoolean(true);
				}

				// Update the results table
				resultManager->updateSynonym(patternSynonymArg, updatedStatements);
				resultManager->updateSynonym(firstArg, updatedVariables);
*/
			} 
			// Pattern w(_,_)
			else if (firstArg == "_") {
				// Check if theres any statements number inside
				if (resultManager->getValuesForSynonym(patternSynonymArg).size() > 0) {
					patternObject->setResultsBoolean(true);
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
						patternObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return patternObject;
						}
					}
				}

				// Check if relationship holds/have results
				if (evaluatedS.size() > 0) {
					// Update the results table
					resultManager->updateSynonym(patternSynonymArg, evaluatedS);
				}				
			}
		}
	}
	// IF pattern:
	else if (patternType == IF) {
		// pattern if(<first arg>,_,_)
		if (secondArg == "_" && thirdArg == "_") {
			// pattern if(v,_,_)
			if (isFirstArgSynonym) {
				// Get current tuple synonyms 
				std::tuple<SynonymString, SynonymString> synonymTuple(patternSynonymArg, firstArg);
				std::set<ValueTuple> tuplePatterns = resultManager->getValuesForSynonymTuple(synonymTuple);

				// Obtain evaluation results for tuple
				ValueTupleSet evaluatedTuplePatterns;
				for (std::set<ValueTuple>::iterator t1 = tuplePatterns.begin(); t1 != tuplePatterns.end(); t1++) {
					if (pkb->isIfPattern(std::get<0>(*t1), std::get<1>(*t1))) {
						evaluatedTuplePatterns.insert(*t1);
						patternObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return patternObject;
						}
					}
				}
				// Check if relationship holds/have results
				if (evaluatedTuplePatterns.size() > 0) {
					// Update tuple with evaluation results
					resultManager->updateSynonymTuple(synonymTuple, evaluatedTuplePatterns);
				}				
/*
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
					patternObject->setResultsBoolean(true);
				}

				// Update the results table
				resultManager->updateSynonym(patternSynonymArg, updatedStatements);
				resultManager->updateSynonym(firstArg, updatedVariables);
*/
			}
			// pattern if(_,_,_)
			else if (firstArg == "_") {
				// Check if theres if statements inside source program
				if (resultManager->getValuesForSynonym(patternSynonymArg).size() > 0) {
					patternObject->setResultsBoolean(true);
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
						patternObject->setResultsBoolean(true);
						if (isStopEvaluation) {
							return patternObject;
						}
					}
				}
				// Check if relationship holds/have results
				if (evaluatedS.size() > 0) {
					// Update the results table
					resultManager->updateSynonym(patternSynonymArg, evaluatedS);
				}				
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
            } 
			else if (ClauseSelectObject.getEntityType() == PROCEDURE) {
				std::set<ProcIndex> setResults3 = resultManager->getValuesForSynonym(ClauseSelectObject.getSynonymString());
				std::vector<std::string> vectorResults3;
				std::transform(setResults3.begin(), setResults3.end(), std::back_inserter(vectorResults3), to_proc_name);
				return vectorResults3;
			}
			else {
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
