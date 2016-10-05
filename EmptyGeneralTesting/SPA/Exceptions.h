#pragma once

#include <stdexcept>
#include "RelationshipType.h"

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

class Exceptions : public std::logic_error {
private:
    typedef const Exceptions E;

public:
    // QueryValidator
    static E invalid_expression(std::string expression) {
        return E(invalid_argument, "Invalid expression found near " + expression);
    }

    static E invalid_relationship(RelationshipType type) {
        return E(invalid_argument, "Invalid query near relationship " + to_string(type));
    }

    static E invalid_relationship_argument(RelationshipType type, std::string argument) {
        return E(invalid_argument, "Invalid argument for relationship " + to_string(type) + " near " + argument);
    }

    static E invalid_relationship_same_synonym_arguments(RelationshipType type, std::string argument1, std::string argument2) {
        return E(invalid_argument, "Invalid relationship " + to_string(type) + " due to same synonym for both argument arg1 = " + argument1 + " and arg2 = " + argument2);
    }

    static E invalid_query_syntax(std::string syntax) {
        return E(invalid_argument, "Invalid syntax near " + syntax);
    }

    static E invalid_pattern(std::string syntax) {
        return E(invalid_argument, "Invalid pattern near " + syntax);
    }

    static E invalid_pattern_missing_syntax() {
        return E(invalid_argument, "Invalid pattern, missing syntax");
    }

    static E invalid_pattern_type_syntax() {
        return E(invalid_argument, "Invalid pattern syntax type");
    }

    static E invalid_pattern_argument() {
        return E(invalid_argument, "Invalid pattern argument(s)");
    }

    static E invalid_pattern_argument(std::string arugment) {
        return E(invalid_argument, "Invalid pattern argument near " + arugment);
    }

    static E invalid_pattern_expression(std::string expression) {
        return E(invalid_argument, "Invalid pattern expression near " + expression);
    }

    static E invalid_attribute_format() {
        return E(invalid_argument, "Invalid attribute format");
    }

    static E invalid_attribute_missing_synonym() {
        return E(invalid_argument, "Invalid attribute missing synonym");
    }

    static E invalid_attribute_syntax(std::string syntax) {
        return E(invalid_argument, "Invalid attribute syntax near " + syntax);
    }

    static E invalid_attribute_name(std::string syntax) {
        return E(invalid_argument, "Invalid attribute name near " + syntax);
    }

    static E invalid_attribute_value_single_synonym(std::string syntax) {
        return E(invalid_argument, "Invalid attribute single synonym near " + syntax);
    }

    static E invalid_attribute_value(std::string value) {
        return E(invalid_argument, "Invalid attribute value near" + value);
    }

    static E exceed_common_synonym_count() {
        return E(length_error, "Exceed number of common synonym between clauses");
    }

    // StringTokenizer
    static E invalid_character(std::string character) {
        return E(invalid_argument, "Invalid character: " + character);
    }

    // SynonymTable
    static E duplicate_object_synonym(std::string synonym) {
        return E(invalid_argument, "SynonymTable insert duplicate object synonym: " + synonym);
    }

public:
    const char* what() { return msg.c_str(); }
    const char* type() { return error_type.c_str(); }

private:
    std::string error_type;
    std::string msg;

    enum ErrorType { domain_error, invalid_argument, length_error, out_of_range };

    Exceptions(ErrorType type, std::string message) : logic_error(std::string()), msg(message) {
        switch (type) {
        case domain_error:     error_type = "domain_error";
        case invalid_argument: error_type = "invalid_argument";
        case length_error:     error_type = "length_error";
        case out_of_range:     error_type = "out_of_range";
        }
    }

    static std::string to_string(RelationshipType type) {
        switch (type) {
        case MODIFIES:
            return "modifies";
        case USES:
            return "uses";
        case CALLS:
            return "calls";
        case CALLS_STAR:
            return "calls*";
        case FOLLOWS:
            return "follows";
        case FOLLOWS_STAR:
            return "follows*";
        case PARENT_STAR:
            return "parent*";
        case NEXT:
            return "next";
        case NEXT_STAR:
            return "next*";
        case AFFECTS:
            return "affects";
        case AFFECTS_STAR:
            return "affects*";
        default:
            return "undefined";
        }
    }
};

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
    const std::length_error EMPTY_PROGRAM_ERROR("Program empty, or file not found");

    // ResultGrid
    const std::invalid_argument INCORRECT_GRID_API("mergeGrid(other) for no result");
    const std::invalid_argument INVALID_TUPLE_POSITION("Tuple position must be 0/1");

    // ResultGridManager
    const std::invalid_argument INVALID_SYNONYM_ERROR("Synonym does not exist");

    // SynonymTable
    const std::out_of_range INVALID_OBJECT_INDEX("getObject index is out of vector size");
};
