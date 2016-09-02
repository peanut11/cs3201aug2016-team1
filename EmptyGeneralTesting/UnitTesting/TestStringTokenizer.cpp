#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/StringTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestStringTokenizer) {
public:

	TEST_METHOD(TestNextToken_ProcedureFirst) {
		std::string str = "";
		str.append("procedure First {").append("\n");
		str.append("x = 2;").append("\n");
		str.append("z = 3;").append("\n");
		str.append("call Second; }");

		StringTokenizer st = StringTokenizer(str, DelimiterMode::PARSER);

		Assert::AreEqual(st.nextToken(), std::string("procedure"));
		Assert::AreEqual(st.nextToken(), std::string("First"));
		Assert::AreEqual(st.nextToken(), std::string("{"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("x"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("2"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("z"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("3"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("call"));
		Assert::AreEqual(st.nextToken(), std::string("Second"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("}"));

		Assert::IsFalse(st.hasMoreTokens());
	}

	TEST_METHOD(TestNextToken_ProcedureSecond) {
		std::string str = "";
		str.append("procedure Second {").append("\n");
		str.append("x = 0;").append("\n");
		str.append("i = 5;").append("\n");
		str.append("while i {").append("\n");
		str.append("x = x + 2*y;").append("\n");
		str.append("call Third;").append("\n");
		str.append("i = i - 1; }").append("\n");
		str.append("if x then{").append("\n");
		str.append("x = x + 1; }").append("\n");
		str.append("else {").append("\n");
		str.append("z = 1; }").append("\n");
		str.append("z = z + x + i;").append("\n");
		str.append("y = z + 2;").append("\n");
		str.append("x = x * y + z; }");

		StringTokenizer st = StringTokenizer(str, DelimiterMode::PARSER);

		Assert::AreEqual(st.nextToken(), std::string("procedure"));
		Assert::AreEqual(st.nextToken(), std::string("Second"));
		Assert::AreEqual(st.nextToken(), std::string("{"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("x"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("0"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("i"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("5"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("while"));
		Assert::AreEqual(st.nextToken(), std::string("i"));
		Assert::AreEqual(st.nextToken(), std::string("{"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("x"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("x"));
		Assert::AreEqual(st.nextToken(), std::string("+"));
		Assert::AreEqual(st.nextToken(), std::string("2"));
		Assert::AreEqual(st.nextToken(), std::string("*"));
		Assert::AreEqual(st.nextToken(), std::string("y"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("call"));
		Assert::AreEqual(st.nextToken(), std::string("Third"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("i"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("i"));
		Assert::AreEqual(st.nextToken(), std::string("-"));
		Assert::AreEqual(st.nextToken(), std::string("1"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("}"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("if"));
		Assert::AreEqual(st.nextToken(), std::string("x"));
		Assert::AreEqual(st.nextToken(), std::string("then"));
		Assert::AreEqual(st.nextToken(), std::string("{"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("x"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("x"));
		Assert::AreEqual(st.nextToken(), std::string("+"));
		Assert::AreEqual(st.nextToken(), std::string("1"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("}"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("else"));
		Assert::AreEqual(st.nextToken(), std::string("{"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("z"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("1"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("}"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("z"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("z"));
		Assert::AreEqual(st.nextToken(), std::string("+"));
		Assert::AreEqual(st.nextToken(), std::string("x"));
		Assert::AreEqual(st.nextToken(), std::string("+"));
		Assert::AreEqual(st.nextToken(), std::string("i"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("y"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("z"));
		Assert::AreEqual(st.nextToken(), std::string("+"));
		Assert::AreEqual(st.nextToken(), std::string("2"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("\n"));

		Assert::AreEqual(st.nextToken(), std::string("x"));
		Assert::AreEqual(st.nextToken(), std::string("="));
		Assert::AreEqual(st.nextToken(), std::string("x"));
		Assert::AreEqual(st.nextToken(), std::string("*"));
		Assert::AreEqual(st.nextToken(), std::string("y"));
		Assert::AreEqual(st.nextToken(), std::string("+"));
		Assert::AreEqual(st.nextToken(), std::string("z"));
		Assert::AreEqual(st.nextToken(), std::string(";"));
		Assert::AreEqual(st.nextToken(), std::string("}"));
		
		Assert::IsFalse(st.hasMoreTokens());
	}

	};
}