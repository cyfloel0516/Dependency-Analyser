#include "TypeDetector.h"
// class definition detector
bool ClassDefinition::test(ASTNode*& node)
{
	if (node->type == "class" || node->type == "enum" || node->type == "struct")
	{
		return true;
	}
	return false;
}

TypeDefinition ClassDefinition::getData(ASTNode*& node)
{
	// To get the real name of class
	auto tokens = utils::split(node->getFullName(), ' ');
	string realName = tokens[tokens.size() - 2];
	auto type = TypeDefinition(realName, node->type, "", "", node->getFullName());
	return type;
}


// typedef declaration detector
bool TypeDefDefinition::test(ASTNode *& node)
{
	if (node->type == "declaration" && node->name.find_first_of("typedef") == 0) {
		return true;
	}
	return false;
}

TypeDefinition TypeDefDefinition::getData(ASTNode *& node)
{
	// To get the real name of typedef
	auto wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');
	auto realName = tokens[tokens.size() - 2];
	auto type = TypeDefinition(realName, node->type, "", "", node->getFullName());
	return type;
}

bool AliasDefinition::test(ASTNode *& node)
{
	if (node->type == "declaration" && node->name.find_first_of("using") == 0 && node->name.find_first_of("=") != string::npos) {
		return true;
	}
	return false;
}

TypeDefinition AliasDefinition::getData(ASTNode *& node)
{
	auto wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');
	auto equalIndex = std::find(tokens.begin(), tokens.end(), "=");
	
	auto realName = *(equalIndex - 1);

	auto type = TypeDefinition(realName, node->type, "", "", node->getFullName());
	return type;
}

bool GlobalFunctionDefinition::isLambda(ASTNode *& node)
{
	auto wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');
	if (std::find(tokens.begin(), tokens.end(), "function") != tokens.end()) {
		return true;
	}
	return false;
}

bool GlobalFunctionDefinition::test(ASTNode *& node)
{
	if (node->type == "function" && node->parent != nullptr && node->parent->type == "namespace") {
		return true;
	}
	return false;
}

TypeDefinition GlobalFunctionDefinition::getData(ASTNode *& node)
{
	string realName;
	auto wholeName = node->getFullName();
	auto tokens = utils::split(wholeName, ' ');
	if (isLambda(node)) {
		auto equalIndex = std::find(tokens.begin(), tokens.end(), "=");
		realName = *(equalIndex - 1);
	}
	else {
		auto parenIndex = std::find(tokens.begin(), tokens.end(), "(");
		realName = *(parenIndex - 1);
	}
	auto type = TypeDefinition(realName, node->type, "", "", node->getFullName());
	return type;
}
