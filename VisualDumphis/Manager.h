#pragma once

#include <iostream>
#include <filesystem>
#include <format>
#include "tools/tls.h"
#include "DumpCDBManager.h"

namespace fs = std::filesystem;

class Manager
{
public:
	Manager(void);
	void start(void);

private:
	tls::LogManager log;
	tls::Gui gui;
	fs::path projectPath;
	fs::path snapshotPath;
	std::string file_dumpcdb = "dumpcdb.csv";
	fs::path path_dumpcdb;


	void askForInput(void);
	bool verifyInput(const std::string& proj, const std::string& file);
	void readFiles(void);
};

