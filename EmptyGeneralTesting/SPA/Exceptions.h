#pragma once

#include <stdexcept>

/*
std::runtime_error
 - std::range_error
 - std::overflow_error
 - std::underflow_error
 - std::system_error

std::logic_error
 - std::domain_error
 - std::invalid_argument
 - std::length_error
 - std::out_of_range
 - std::future_error
*/

namespace Exception {
    // PKB
    const std::invalid_argument INCORRECT_PKB_API("API only for FOLLOWS & PARENT");
    const std::invalid_argument INVALID_STMT_TYPE("Only ASSIGN, CALL, IF, WHILE");
    const std::invalid_argument INVALID_STMT_STMT_RELATION("Invalid stmt-stmt relation");
    const std::invalid_argument INVALID_VAR_STMT_RELATION("Invalid var-stmt relation");
	const std::invalid_argument INVALID_VAR_PROC_RELATION("Invalid var-proc relation");
	const std::invalid_argument INVALID_PROC_PROC_RELATION("Invalid proc-proc relation");
    const std::invalid_argument NOT_ASSIGN_ERROR("Statement is not an assignment");
	const std::invalid_argument NOT_WHILE_OR_IF_ERROR("Statement is not a while or if statement");
    const std::length_error UNEXPECTED_STMT_ERROR("StmtType not recorded in order");
    const std::out_of_range INVALID_VAR_INDEX("VarIndex does not exist");
	const std::out_of_range INVALID_PROC_INDEX("ProcIndex does not exist");

    // ProgramValidator
    const std::domain_error INVALID_CALL_MADE("Call to procedure that does not exist"); 
    const std::domain_error INVALID_SIMPLE_SYNTAX("Invalid SIMPLE source code");
    const std::invalid_argument RECURSIVE_CALL_ERROR("Procedure is called recursively");
    const std::invalid_argument DUPLICATE_PROCEDURE_NAME("Procedure was registered");

    // ResultGrid
    const std::invalid_argument INCORRECT_GRID_API("mergeGrid(other) for no result");
    const std::invalid_argument INVALID_TUPLE_POSITION("Tuple position must be 0/1");

    // ResultGridManager
    const std::invalid_argument INVALID_SYNONYM_ERROR("Synonym does not exist");

    // SynonymTable
    const std::out_of_range INVALID_OBJECT_INDEX("getObject index is out of vector size");
};
