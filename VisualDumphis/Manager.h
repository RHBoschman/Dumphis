#pragma once

#include <iostream>
#include <filesystem>
#include <format>
#include "tools/tls.h"
#include "DumpCDBManager.h"
#include "CDBDataManager.h"

/* TODO
* - Verify project path input
* - Find and parse cdb_data.h
* - Create data with CDBDataManger (as CDBData objects)
*/

namespace fs = std::filesystem;

class Manager
{
public:
	Manager(void);
	void start(void);

private:
	DumpCDBManager dumpcdbRead;
	CDBDataManager cdbdataRead;
	tls::LogManager log;
	tls::Gui gui;
	fs::path projectPath;
	fs::path snapshotPath;
	std::string file_dumpcdb = "dumpcdb.csv";
	std::string file_cdb_data = "global\\include\\cdb_data.h";

	void askForInput(void);
	bool verifyInput(const std::string& proj, const std::string& file);
	void readFiles(void);
	bool doesFileExist(const fs::path& root, const std::string& relativePath);
};

