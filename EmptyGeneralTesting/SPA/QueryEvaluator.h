#pragma once

#include<stdio.h>
#include <iostream>

class QueryEvaluator {

	static QueryEvaluator *_instance;

public:
	static QueryEvaluator *getInstance();
	void evaluate();

};