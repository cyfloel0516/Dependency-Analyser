#pragma once
/*
* This is a detector interface for type analysis.
* The implementation of this interface test if a ASTnode is 
* a particular type definition and return the result.
* 
* Author: Junkai Yang
*/

#include "../AstTree/ASTNode.h"
#include <complex>
class ITypeDetector {
public:
	virtual bool test(ASTNode*& node) = 0;
	// Return type and name as the result.
	virtual std::pair<std::string, std::string> getData(ASTNode*& node) = 0;
};

class ClassDefinition: public ITypeDetector{
	// Inherited via ITypeDetector
	virtual bool test(ASTNode*& node) override;
	virtual std::pair<std::string, std::string> getData(ASTNode*& node) override;
};