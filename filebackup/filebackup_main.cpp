/* \file	filebackup_main
	\author	Nick Komarnicki
	\date	September 21, 2018
	\brief	Backup program to take user provided folder(or current working directory)
	and backup all .c, .cpp, .h, or .hpp files to specified folder or default folder of c:\backup
*/
#include "filebackup_main.hpp"

/**
Main method for filebackup_main
@param argc - argument count from command line
@param argv - command line args
*/
int main(int argc, char * argv[])
{
	std::cout << "filebackup (c) 2018, Nick Komarnicki\n\n";
	ProgramParams params;
	getInput(argc, argv, params);
	runProgram(params);
}//end main

 /**
 Processes command line args and stores them into program paramaters object
 @param argc - number of command line args
 @param argv - the command line arguments
 @param params - the paramaters to follow in order to do the collecting
 */
void getInput(int argc, char *argv[], ProgramParams & params)
{
	std::string backupDirTrim;

	std::string errorMsg = "";
	params.sourceDir = std::experimental::filesystem::current_path();
	params.backupDir = "c:\backup";
	if (argc <= 1)
	{
		params.backupDir = "c:\backup";
		params.defaultRun = true;
	}
	else if (argc == 2)
	{
		if (argv[1][2] != '\\')
		{
			errorMsg = argv[1];
			errorMsg += " is not a valid path";
			printError(errorMsg);
		}
		params.sourceDir = argv[1];
		params.oneArgRun = true;
		if (!std::experimental::filesystem::exists(params.sourceDir))
		{
			errorMsg += params.sourceDir.string() + " does not exist to backup.\n";
			printError(errorMsg);
		}
	}
	else if (argc == 3)
	{
		params.sourceDir = argv[1];
		if (!std::experimental::filesystem::exists(params.sourceDir))
		{
			errorMsg += params.sourceDir.string() + " does not exist to backup.\n";
			printError(errorMsg);
		}

		backupDirTrim = argv[2];
		backupDirTrim.erase(backupDirTrim.begin() + backupDirTrim.find('\\'), backupDirTrim.end());
		params.backupDir = backupDirTrim;
		if (!std::experimental::filesystem::exists(params.backupDir))
		{
			errorMsg = argv[2];
			errorMsg += " is not a valid path";
			printError(errorMsg);
		}

		params.backupDir = argv[2];
		params.twoArgRun = true;
	}
	else
	{
		errorMsg = "Invalid number of args. Please enter in 0 - 2 arguments";
		printError(errorMsg);
	}
	
	if (params.sourceDir.string().at(params.sourceDir.string().size() - 1) == '\\'
		|| params.backupDir.string().at(params.backupDir.string().size() - 1) == '\\')
	{
		errorMsg = "Invalid input. Cannot end directory in a \\";
		printError(errorMsg);
	}
}//end getInput

/**
 Displays error message to console and quits program
 @param msg - the string message to display
 */
void printError(std::string const & msg)
{
	std::cerr << "Error: " << msg << std::endl;
	std::exit(EXIT_FAILURE);
}//end printError

/**
 Handles running the program based on the provided parameters
 @param params - the paramaters to follow in order to do the collecting
 */
void runProgram(ProgramParams& params)
{
	if (params.defaultRun)
	{
		FileManager backupFiles;
		backupFiles.backup();
	}
	else if (params.oneArgRun)
	{
		FileManager backupFiles(params.sourceDir);
		FileManager backupFilesCopy(backupFiles);
	}
	else if (params.twoArgRun)
	{
		FileManager backupFiles(params.sourceDir, params.backupDir);
		backupFiles.backup();
	}
}//end runProgram