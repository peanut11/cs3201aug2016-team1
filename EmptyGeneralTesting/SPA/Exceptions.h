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

const std::invalid_argument INCORRECT_API_ERROR("Incorrect use of API");
const std::invalid_argument INVALID_STMT_RELATION("Invalid stmt-stmt relation");
const std::invalid_argument INVALID_VAR_RELATION("Invalid var-stmt relation");
const std::invalid_argument NOT_ASSIGN_ERROR("Statement is not an assignment");

const std::length_error UNEXPECTED_STMT_ERROR("StmtType not recorded in order");

const std::out_of_range INVALID_VAR_INDEX("VarIndex does not exist");
