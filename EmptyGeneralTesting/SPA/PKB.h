// Maintained by: Kai Lin

#pragma once

#include <stack>
#include <set>
#include <string>
#include <vector>

#include "EntityType.h"
#include "Exceptions.h"
#include "RelationshipType.h"
#include "Types.h"

class PKB {
private:
    static PKB* theOne;

    std::map<ProcName, ProcIndex> procRefMap;
    std::map<VarName, VarIndex>   varRefMap;

    std::set<Constant>            constants;
    std::vector<VarIndex>         controlVars;
    std::vector<PostfixExpr>      postfixExprs;
    std::vector<ProcName>         procRefTable;
    std::vector<ProcRow>          procTable;
    std::vector<StmtSet>          procToStmtTable;
    std::vector<StmtSet>          stmtByTypeTable;
    std::vector<StmtRow>          stmtTable;
    std::vector<EntityType>       stmtTypeTable;
    std::vector<ProcIndex>        stmtToProcTable;
    std::vector<VarName>          varRefTable;
    std::vector<VarRow>           varTable;

public:
    static PKB* getInstance();
    void clear();
    PKB();

    // Indexing
    virtual ProcIndex getProcIndex(ProcName procName);
    virtual ProcName  getProcName(ProcIndex procIndex);
    virtual VarIndex  getVarIndex(VarName varName);
    virtual VarName   getVarName(VarIndex varIndex);

    // API used by Parser and DE
    bool putConstant(Constant constant); 
    bool putControlVarForStmt(StmtNumber ifOrWhile, VarName varName);
    bool putExprForStmt(StmtNumber, PostfixExpr);
    bool putProcForProc(ProcIndex procA, RelationshipType callsOrStar, ProcName procB);
    bool putStmtForStmt(StmtNumber stmtA, RelationshipType rel, StmtNumber stmtB);
    bool putStmtProc(StmtNumber stmt, ProcName procNameContainingStmt);
    bool putStmtTypeForStmt(StmtNumber stmt, EntityType stmtType);
    bool putVarForStmt(StmtNumber stmt, RelationshipType modifiesOrUses, VarName varName);

    // API used by QP and DE
    /*
    Follows( 1, 2) -> bool result = is(FOLLOWS,1,2);
    Follows(s1, 2) -> std::vector<StmtNumber> s1 = getStmtsByStmt(2,FOLLOWS);     // Get stmts that stmt 2 follows
    Follows( 1,s2) -> std::vector<StmtNumber> s2 = getStmtsByStmt(1,FOLLOWED_BY); // Get stmts that stmt 1 is followed by
    Follows(s1,s2) -> std::vector<StmtNumber> s1 = s2 = getAllStmts();
    Follows( a,s2) -> std::vector<StmtNumber>  a = getStmtsByType(ASSIGN);
                      std::vector<StmtNumber> s2 = getAllStmts();
                      for (int i = 0; i < a.size(); i++) {
                        for (int j = 0; j < s2.size(); j++) {
                          if (is(FOLLOWS,a,s2)) {
                            if (query == "Select a") {
                              results.append(a);
                            } else if (query == "Select s2") {
                              results.append(s2);
                            }
                          }
                        }
                      }
    */
    virtual bool isAssignHasExpr(StmtNumber, StringToken);
    virtual bool isAssignHasSubexpr(StmtNumber, StringToken);
    virtual bool is(RelationshipType rel, StmtNumber stmt, ProcStmtVarIndex item);
    virtual bool isAssignExactPattern(StmtNumber, InfixExpr);
    virtual bool isAssignContainsPattern(StmtNumber, InfixExpr);
    virtual bool isIfPattern(StmtNumber ifStmt, VarIndex varIndex);
    virtual bool isWhilePattern(StmtNumber whileStmt, VarIndex varIndex);
    bool isVarExist(VarName varName);

    virtual StmtNumber           getStmtTableSize();
    virtual ProcIndex            getProcByStmt(StmtNumber stmt);
    virtual EntityType           getStmtTypeForStmt(StmtNumber stmt);
    virtual std::set<ProcName>   getAllProcNames();
    virtual std::set<Constant>   getAllConstantValues();
    virtual std::set<StmtNumber> getAllStmts();
    virtual std::set<VarIndex>   getAllVarIndex();
    virtual std::set<VarName>    getAllVarNames();

    // Get procedures
    virtual std::set<ProcIndex>  getProcsByProc(ProcName procName, RelationshipType callsOrStar);
    virtual std::set<ProcIndex>  getProcsByProc(RelationshipType calls, ProcName procName); // Check for inverse
    virtual std::set<ProcIndex>  getProcsByVar(RelationshipType modifiesOrUses, VarName varName);

    // Get statements
    virtual std::set<StmtNumber> getStmtsByType(EntityType stmtType);
    virtual std::set<StmtNumber> getStmtsByProc(ProcName procName);
    virtual std::set<StmtNumber> getStmtsByProc(ProcIndex procIndex);
    virtual std::set<StmtNumber> getStmtsByStmt(StmtNumber stmt, RelationshipType stmtRel);
    virtual std::set<StmtNumber> getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt);
    virtual std::set<StmtNumber> getStmtsByVar(RelationshipType modifiesOrUses, VarName varName);
    virtual std::set<StmtNumber> getStmtsByVar(RelationshipType modifiesOrUses, VarIndex varIndex);

    // Get variables
    virtual std::set<VarIndex>   getVarsByProc(ProcName procName, RelationshipType modifiesOrUses);
    virtual std::set<VarIndex>   getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses);

    // Warning: this one is public for testing purpose
    PostfixExpr infixToPostfix(InfixExpr);
};
