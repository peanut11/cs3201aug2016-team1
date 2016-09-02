// Maintained by: Ngoc Khanh
//
// Accessed by:
// - Parser
//
// Constructs:
// - AssignNode
#include "ProgramConverter.h"
#include "StringTokenizer.h"

ProgramConverter::ProgramConverter() {

}

void ProgramConverter::convert(std::string program, AssignmentTree& asTree, VarTable& varTable) {
	StringTokenizer tokenizer = StringTokenizer(program, PROGRAM_VALIDATOR);
}