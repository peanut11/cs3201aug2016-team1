// Maintained by: Ngoc Khanh
//
// Accessed by:
// - Parser
//
// Constructs:
// - AssignTNode
#include "ProgramConverter.h"
#include "StringTokenizer.h"

ProgramConverter::ProgramConverter() {

}

void ProgramConverter::convert(std::string program, AbstractSyntaxTree& asTree, VarTable& varTable) {
	StringTokenizer tokenizer = StringTokenizer(program, PROGRAM_VALIDATOR);
}