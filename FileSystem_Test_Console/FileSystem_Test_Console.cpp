// FileSystem_Test_Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "../FileSystem_Windows/FileSystem.h"


int main()
{
	FileSystem::Directory::setCurrentDirectory("..");
	auto files = FileSystem::Directory::getDirectories(FileSystem::Directory::getCurrentDirectory(), "File*");

	for (auto i = 0; i < files.size(); i++) {
		std::cout << files[i] << std::endl;
	}
	
	std::string input;
	std::getline(std::cin, input);
}

