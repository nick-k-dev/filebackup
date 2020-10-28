#pragma once
/* \file	filebackup_main
	\author	Nick Komarnicki
	\date	September 21, 2018
	\brief	Header file for filebackup_main
*/

#include <iostream>
#include <string>
#include "ProgramParams.hpp"
#include "FileManager.hpp"

void getInput(int, char *[], ProgramParams&);
void printError(std::string const &);
void runProgram(ProgramParams&);