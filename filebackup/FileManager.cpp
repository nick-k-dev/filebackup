#include "FileManager.hpp"
/* \file	FileManager.cpp
	\author	Nick Komarnicki
	\date	September 21, 2018
	\brief	Implementation of FileManager class to hold the file information for backing up.
*/

//Constructors
FileManager::FileManager() : sourceDir(std::experimental::filesystem::current_path()), backupDir("c:\\backup"),
ext("\\.(c|cpp|h|hpp|)"), parentPathLength(0), totalBackupFileSizes(0), totalBackupFiles(0), largestSize(0)
{
	scanDirectories();
}

FileManager::FileManager(pathT sd) : sourceDir(sd), backupDir("c:\\backup"),
ext("\\.(c|cpp|h|hpp|)"), parentPathLength(0), totalBackupFileSizes(0), totalBackupFiles(0), largestSize(0)
{
	scanDirectories();
}

FileManager::FileManager(pathT sd, pathT bd) : sourceDir(sd), backupDir(bd),
ext("\\.(c|cpp|h|hpp|)"), parentPathLength(0), totalBackupFileSizes(0), totalBackupFiles(0), largestSize(0)
{
	scanDirectories();
}

//Copy constructor
FileManager::FileManager(FileManager const & oldFM) : sourceDir(oldFM.sourceDir), backupDir(oldFM.backupDir),
ext(oldFM.ext), parentPathLength(oldFM.parentPathLength), totalBackupFileSizes(0),
totalBackupFiles(0), largestSize(0)
{
	for (auto & folders : oldFM.fileStructure)
	{
		fileStructure[folders.first] = folders.second;
	}
	backup();
}

FileManager& FileManager::operator = (FileManager& rhs)
{
	sourceDir = rhs.sourceDir;
	backupDir = rhs.backupDir;
	ext = rhs.ext;
	parentPathLength = rhs.parentPathLength;
	totalBackupFileSizes = 0;
	totalBackupFiles = 0;
	largestSize = 0;
	return *this;
}


/**
 Scans the folders for the file types and stores them
 */
void FileManager::scanDirectories()
{
	//Get length of parent path to remove later
	parentPathLength = FileManager::sourceDir.string().size();

	//Scan all folders and store paths with proper extensions
	pathT currentFolder;
	for (std::experimental::filesystem::recursive_directory_iterator dir(sourceDir), endDir; dir != endDir; ++dir)
	{
		try
		{
			if (exists(dir->path()))
			{
				if (is_directory(dir->status()))
					currentFolder = *dir;
				else
					currentFolder = dir->path().parent_path();

				if (is_regular_file(dir->status()))
				{
					if (std::regex_match(dir->path().extension().string(), ext))
					{
						fileStructure[currentFolder].push_back(std::make_pair(dir->path(), std::experimental::filesystem::file_size(dir->path())));
					}
				}//end if
			}//end outer if
		}
		catch (std::experimental::filesystem::filesystem_error& ex)
		{
			std::cerr << ex.what() << std::endl;
			exit(EXIT_FAILURE);
		}//end try catch
	}//end for
}//end scanDirectories

/**
 Backs up the files to the back up folder to the provided backup folder
 */
void FileManager::backup()
{
	//Create directories
	for (auto & folders : fileStructure)
	{
		std::string temp = folders.first.string();
		std::string newPathString = backupDir.string();
		newPathString += temp.substr(parentPathLength, temp.size());
		pathT newPath = newPathString;
		if (!std::experimental::filesystem::exists(newPath))
			std::experimental::filesystem::create_directories(newPath);

		//Copy files to new path
		for (auto & file : folders.second)
		{
			std::string temp2 = file.first.string();
			std::string newPathString2 = backupDir.string();
			newPathString2 += temp2.substr(parentPathLength, temp2.size());
			pathT newPath2 = newPathString2;
			if (!std::experimental::filesystem::exists(newPath2))
			{
				std::experimental::filesystem::copy(file.first, newPath2);
				backupInfo[newPath2].first = '+';
			}
			else
			{
				backupInfo[newPath2].first = '-';
			}
			backupInfo[newPath2].second = std::experimental::filesystem::file_size(newPath2);
		}
	}
	report();
}//end backup

/**
 Reports to the console which files were backed up
 */
void FileManager::report()
{
	std::cout.imbue(std::locale(""));

	//Get size info and columns for printing
	calculateInfo();

	//Print report
	std::cout << "Root Directory: " << sourceDir << std::endl;
	std::cout << "Backup Directory: " << backupDir << std::endl;

	for (auto reportInfo : backupInfo)
	{
		std::cout << std::left << reportInfo.second.first << " " << std::left << std::setw(largestSize) << reportInfo.first  << std::right << "Size:  " << reportInfo.second.second << std::endl;
	}

	std::cout << std::endl;
	largestSize -= 12;
	largestSize -= std::to_string(totalBackupFiles).size();
	std::cout << std::left << "Backup Total: " << totalBackupFiles << std::string(unsigned int(largestSize), ' ') << std::right << "Size:  " << totalBackupFileSizes << std:: endl;
}//end report

/**
 Calculates size info and column width for reporting
 */
void FileManager::calculateInfo()
{
	bigUInt lastLargest = largestSize;
	for (auto reportInfo : backupInfo)
	{
		lastLargest = reportInfo.first.string().size();
		if (lastLargest >= largestSize)
			largestSize = lastLargest;
		if (reportInfo.second.first == '+')
		{
			totalBackupFileSizes += reportInfo.second.second; 
			++totalBackupFiles;
		}
	}
	largestSize += 3;
}//end calculateInfo