// FileSearch_Test.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <windows.h>

#include "../FileSystem_Windows/FileSystemSearchHelper.h"
#include "../FileSystem_Windows/FileSystem.h"

using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int main(int argc, char* argv[])
{
	std::string userInput;
	if (argc < 3) {
		cout << "please input folder path and search pattern" << endl;
		return 1;
	}
	else if (argc == 3) {
		//Run test for a single file
		std::string folderPath = FileSystem::Path::getFullFileSpec(argv[1]);
		std::string pattern = std::string(argv[2]);
		auto fileInfo = FileSystem::FileInfo(folderPath);
		if (!fileInfo.isDirectory() || !FileSystem::File::exists(folderPath)) {
			cout << "Please input the correct folder name!" << endl;
			return 1;
		}
		else {
			system("CLS");
			std::string userInput;
			SetConsoleTextAttribute(hConsole, 10);
			std::string searchPath = std::string(argv[1]);
			std::string searchPattern = std::string(argv[2]);
			std::cout << "This test will show you the ability to search files and directories in a folder." << endl;

			if (!FileSystem::Directory::exists(searchPath)) {
				std::wcout << "The directory is not existed!!!";
				std::getline(std::cin, userInput);
				return 1;
			}

			FileSystem::Directory::setCurrentDirectory(searchPath);
			auto files = FileSystem::Directory::getFiles(FileSystem::Directory::getCurrentDirectory(), searchPattern);
			auto directories = FileSystem::Directory::getDirectories(FileSystem::Directory::getCurrentDirectory(), searchPattern);

			cout << endl;
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "Begin searching in " << searchPath << " with pattern " << searchPattern << "..." << endl;
			std::cout << "======================================================================" << endl;
			std::cout << "Matched directories in this folder: " << endl;
			std::for_each(directories.begin(), directories.end(), [](std::string& path) {
				std::cout << FileSystem::Path::getFullFileSpec(path) << endl;
			});

			std::cout << endl << "======================================================================" << endl;
			std::cout << "Matched files in this folder: " << endl;
			std::for_each(files.begin(), files.end(), [](std::string& path) {
				std::cout << FileSystem::Path::getFullFileSpec(path) << endl;
			});

			std::cout << "======================================================================" << endl;
			std::cout << "Press enter to exit..." << endl;
			std::getline(std::cin, userInput);
		}
	}
	return 1;
}

