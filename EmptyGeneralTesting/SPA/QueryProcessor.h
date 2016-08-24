#pragma once
#include<stdio.h>
#include <iostream>

#include "QueryEvaluator.h"

class QueryProcessor {

	static QueryProcessor *_instance;

	QueryEvaluator *mEvaluator;
	

	void init() {
		mEvaluator = QueryEvaluator::getInstance();
		// do other initialization here
	}

public:
	/*
	Singleton
	*/
	static QueryProcessor *getInstance()
	{
		if (!_instance)
			_instance = new QueryProcessor;
		return _instance;
	}

	/*
	Evaluate QueryTree
	*/
	void evaluate() {

		try {
			// call QueryEvaulator and evaluate QueryTree
			this->mEvaluator->evaluate();
		}
		catch (std::runtime_error e) {
			std::cout << e.what() << std::endl;
		}

		
	}

	


};