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
    std::vector<ProcName>         procRefTable; 
    std::map<VarName, VarIndex>   varRefMap;
    std::vector<VarName>          varRefTable;

    std::map<StmtNumber, ProcIndex> callToProcMap;   
    std::vector<StmtSet>            procToCallTable; 

    std::set<Constant>            constants;
    std::vector<VarIndex>         controlVars;
    std::vector<PostfixExpr>      postfixExprs;
    std::vector<ProcRow>          procTable;
    std::vector<StmtSet>          procToStmtTable;
    std::vector<StmtSet>          stmtByTypeTable;
    std::vector<StmtRow>          stmtTable;
    std::vector<EntityType>       stmtTypeTable;
    std::vector<ProcIndex>        stmtToProcTable;
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
    bool putStmtForStmt(StmtNumber stmtA, RelationshipType rel, StmtNumber stmtB); // Follows(stmtA, stmtB)
    bool putStmtProc(StmtNumber stmt, ProcName procNameContainingStmt);
    bool putStmtTypeForStmt(StmtNumber stmt, EntityType stmtType);
    bool putVarForStmt(StmtNumber stmt, RelationshipType modifiesOrUses, VarName varName);
	bool putVarForProc(ProcName procA, RelationshipType modifiesOrUses, VarName varName);
	bool putStmtCallProc(StmtNumber stmt, ProcName procCalled);
	bool putStmtLst(StmtNumber stmt);

    // API used by QP and DE
    virtual bool is(RelationshipType rel, ProcStmtIndex stmtOrProcIndex, ProcStmtVarIndex item);
	virtual bool isAssignExactPattern(StmtNumber, InfixExpr);
    virtual bool isAssignContainsPattern(StmtNumber, InfixExpr);
    virtual bool isIfPattern(StmtNumber ifStmt, VarIndex varIndex);
    virtual bool isWhilePattern(StmtNumber whileStmt, VarIndex varIndex);
    virtual bool isProcExist(ProcName procName);
    virtual bool isStmtExist(StmtNumber stmt);
    virtual bool isVarExist(VarName varName);

    virtual StmtNumber           getStmtTableSize();
	virtual StmtNumber			 getProcTableSize();
    virtual ProcIndex            getProcByCall(StmtNumber callStmt);
    virtual ProcIndex            getProcByStmt(StmtNumber stmt);
    virtual EntityType           getStmtTypeForStmt(StmtNumber stmt);
    virtual std::set<ProcName>   getAllProcNames();
	virtual std::set<ProcIndex>	 getAllProcIndex();
    virtual std::set<Constant>   getAllConstantValues();
    virtual std::set<StmtNumber> getAllStmts();
    virtual std::set<VarIndex>   getAllVarIndex();
    virtual std::set<VarName>    getAllVarNames();

    // Get procedures
    virtual std::set<ProcIndex>  getProcsByProc(ProcIndex procIndex, RelationshipType callsOrStar); // Gets x in Calls(x, 2)
    virtual std::set<ProcIndex>  getProcsByProc(RelationshipType calls, ProcIndex procIndex); // Gets x in Calls(2, x)
	virtual std::set<ProcIndex>  getProcsByVar(RelationshipType modifiesOrUses, VarIndex varIndex);

    // Get statements
    virtual std::set<StmtNumber> getCallsByProc(ProcIndex procIndex);
    virtual std::set<StmtNumber> getStmtsByType(EntityType stmtType);
    virtual std::set<StmtNumber> getStmtsByProc(ProcIndex procIndex);
    virtual std::set<StmtNumber> getStmtsByStmt(StmtNumber stmt, RelationshipType stmtRel); // Gets x in Follows(x, 2)
    virtual std::set<StmtNumber> getStmtsByStmt(RelationshipType followsOrParent, StmtNumber stmt); // Gets x in Follows(1, x)
    virtual std::set<StmtNumber> getStmtsByVar(RelationshipType modifiesOrUses, VarIndex varIndex);

    // Get variables
    virtual std::set<VarIndex>   getVarsByProc(ProcIndex procIndex, RelationshipType modifiesOrUses);
    virtual std::set<VarIndex>   getVarsByStmt(StmtNumber stmt, RelationshipType modifiesOrUses);

    // Warning: this one is public for testing purpose
    PostfixExpr infixToPostfix(InfixExpr);
};
