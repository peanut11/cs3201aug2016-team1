#pragma once

#include<stdio.h>
#include <iostream>
#include "PKB.h"

class QueryEvaluator {

	static QueryEvaluator *_instance;

public: 
	/*
	Singleton
	*/
	static QueryEvaluator *getInstance()
	{
		if (!_instance)
			_instance = new QueryEvaluator;
		return _instance;
	}

	/*
	Evaluate QueryTree
	*/
	void evaluate() {
		try {
			// do the evaluation (Basic Query Evaluator BQE) 
		}
		catch (std::runtime_error e) {
			throw e.what();
		}
	}

};