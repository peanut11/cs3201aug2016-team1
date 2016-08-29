// Maintained by: Kai Lin & Ngoc Khanh
using namespace std;

class TNode
{

public:
	enum TNodeType { nodeTypeProgram, nodeTypeProcedure, nodeTypeStmtLstDefault, 
		nodeTypeStmtLstThen, nodeTypeStmtLstElse, nodeTypeAssign, nodeTypeWhile, 
		nodeTypeVariable, nodeTypeConstant, nodeTypePlus };

	TNode(TNodeType nodeType);	
	// NodeType: procedure, variable, constant
	TNode(TNodeType nodeType, int nameIndex);

	TNodeType getNodeType();
	int getNameIndex();
private:
	TNodeType mNodeType;
	int mNameIndex;
};
