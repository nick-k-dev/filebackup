#pragma once
/* \file	FileManager.hpp
	\author	Nick Komarnicki
	\date	September 21, 2018
	\brief	Class to hold the file information for backing up.
*/

#include <filesystem>
#include <map>
#include <vector>
#include <regex>
#include <iostream>
#include <iomanip>
#include <locale>

class FileManager
{
public:

	typedef std::experimental::filesystem::path							pathT;
	typedef unsigned long long											bigUInt;
	typedef std::map<pathT, std::vector<std::pair<pathT, bigUInt>>>		fileContainer;

	//Constructors
	FileManager();
	FileManager(pathT sd);
	FileManager(pathT sd, pathT bd);
	
	FileManager(FileManager const & oldFM);

	FileManager& operator = (FileManager& rhs);

	//Remove the move constructor and move = operator
	FileManager(FileManager const &&) = delete;
	FileManager& operator = (FileManager const &&) = delete;

private:
	pathT sourceDir;
	pathT backupDir;
	bigUInt totalBackupFileSizes;
	bigUInt totalBackupFiles;
	bigUInt largestSize;

	fileContainer fileStructure;
	std::map<pathT, std::pair<char, bigUInt>> backupInfo;
	std::regex ext;
	std::string::size_type parentPathLength;

public:
	void scanDirectories();
	void backup();
	void report();
	void calculateInfo();
}; //end class