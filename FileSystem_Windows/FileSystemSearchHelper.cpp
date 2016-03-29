/*
* FileSystemSearchHelper.cpp: implementation of class FileSystemSearchHelper.
* version: 1.0
* Languange: C++
* Application: Visual Studio 2015
* Author: Junkai Yang
*/

#include "FileSystemSearchHelper.h"
#include <iostream>
#include <algorithm>

std::vector<std::string> FileSystemSearchHelper::searchFiles(std::string rootPath, std::string pattern) {
	std::vector<std::string> result;
	auto entry = FileSystem::Path::getFullFileSpec(FileSystem::Directory::getCurrentDirectory());
	FileSystem::Directory::setCurrentDirectory(".");
	rootPath = FileSystem::Path::getFullFileSpec(rootPath);
	if (!FileSystem::Directory::exists(rootPath))
	{
		return result;
	}
	std::function<void(std::string root, std::string pattern, std::vector<std::string>& results)> walkFolder;
	// This lambda is use to search folder recursively
	walkFolder = [&](std::string root, std::string pattern, std::vector<std::string>& results) {
		auto fullPath = FileSystem::Path::getFullFileSpec(root);
		FileSystem::Directory::setCurrentDirectory(fullPath);

		auto files = FileSystem::Directory::getFiles(fullPath, pattern);
		std::for_each(files.begin(), files.end(), [&](std::string& path) {
			results.push_back(FileSystem::Path::getFullFileSpec(path));
		});

		auto directories = FileSystem::Directory::getDirectories(fullPath, "*");
		auto nowPath = FileSystem::Path::getFullFileSpec(".");
		auto parentPath = FileSystem::Path::getFullFileSpec("..");
		std::for_each(directories.begin(), directories.end(), [&](std::string& path) {
			//Need to exclude self and parent folder
			path = FileSystem::Path::getFullFileSpec(path);
			if (path != nowPath && path != parentPath && path != root)
				walkFolder(path, pattern, results);
		});
	};

	//Begin to search using lambda function
	walkFolder(rootPath, pattern, result);
	FileSystem::Directory::setCurrentDirectory(entry);
	return result;
}

std::vector<std::string> FileSystemSearchHelper::searchDirectories(std::string rootPath, std::string pattern) {
	std::vector<std::string> result;
	//Need to exclude self and parent folder
	auto p = FileSystem::Path::getFullFileSpec(".");
	FileSystem::Directory::setCurrentDirectory(".");
	auto entry = FileSystem::Path::getFullFileSpec(FileSystem::Directory::getCurrentDirectory());
	rootPath = FileSystem::Path::getFullFileSpec(rootPath);
	if (!FileSystem::Directory::exists(rootPath))
	{
		return result;
	}
	std::function<void(std::string root, std::string pattern, std::vector<std::string>& results)> walkFolder;

	// This lambda is use to search folder recursively
	walkFolder = [&](std::string root, std::string pattern, std::vector<std::string>& results) {
		auto fullPath = FileSystem::Path::getFullFileSpec(root);
		FileSystem::Directory::setCurrentDirectory(fullPath);

		auto directories = FileSystem::Directory::getDirectories(fullPath, "*");
		auto nowPath = FileSystem::Path::getFullFileSpec(".");
		auto parentPath = FileSystem::Path::getFullFileSpec("..");

		auto files = FileSystem::Directory::getDirectories(fullPath, pattern);
		std::for_each(files.begin(), files.end(), [&](std::string& path) {
			//Need to exclude self and parent folder
			path = FileSystem::Path::getFullFileSpec(path);
			if (path != nowPath && path != parentPath && path != root)
				results.push_back(FileSystem::Path::getFullFileSpec(path));
		});

		std::for_each(directories.begin(), directories.end(), [&](std::string& path) {
			//Need to exclude self and parent folder
			path = FileSystem::Path::getFullFileSpec(path);
			if (path != nowPath && path != parentPath && path != root)
				walkFolder(path, pattern, results);
		});
	};

	//Begin to search using lambda function
	walkFolder(rootPath, pattern, result);
	FileSystem::Directory::setCurrentDirectory(entry);
	return result;
}

//----< test stub >--------------------------------------------
#ifdef FILESEARCH_TEST
int main() {
	std::vector<std::string> files = FileSystemSearchHelper::searchFiles("../Parser", "*.*");
	std::cout << "Begin searching in " << "../Parser" << " with pattern " << "*.*" << "..." << endl;
	std::cout << "======================================================================" << endl;
	std::cout << "Matched files in this folder: " << endl;
	std::for_each(files.begin(), files.end(), [](std::string& path) {
		std::cout << FileSystem::Path::getFullFileSpec(path) << endl;
	});
	std::cout << "======================================================================" << endl;
	std::cout << "======================================================================" << endl;

	files = FileSystemSearchHelper::searchDirectories("../Parser", "*");
	std::cout << "Begin searching in " << "../Parser" << " with pattern " << "*.*" << "..." << endl;
	std::cout << "======================================================================" << endl;
	std::cout << "Matched directories in this folder: " << endl;
	std::for_each(files.begin(), files.end(), [](std::string& path) {
		std::cout << FileSystem::Path::getFullFileSpec(path) << endl;
	});
	std::cout << "======================================================================" << endl;
	std::cout << "======================================================================" << endl;
}
#endif