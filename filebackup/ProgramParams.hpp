#pragma once
/* \file	ProgramParams.hpp
	\author	Nick Komarnicki
	\date	September 21, 2018
	\brief	Struct to hold the parameters of the program
*/

#include <filesystem>

struct ProgramParams
{
	ProgramParams() : defaultRun(false), oneArgRun(false), twoArgRun(false) {}
	std::experimental::filesystem::path sourceDir;
	std::experimental::filesystem::path backupDir;
	bool defaultRun;
	bool oneArgRun;
	bool twoArgRun;
};