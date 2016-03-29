#include "TypeDetector.h"

bool ClassDefinition::test(ASTNode*& node)
{
	if (node->type == "class" || node->type == "enum" || node->type == "struct")
	{
		return true;
	}
	return false;
}

std::pair<std::string, std::string> ClassDefinition::getData(ASTNode*& node)
{
	return std::make_pair(node->type, node->name);
}
