#pragma once

#include <filesystem>
#include <iostream>
#include <format>
#include <vector>
#include <math.h>
#include "tools/tls.h"
#include "SourceData.h"

#define MAX_FIND_ITERATIONS 3
#define PRINT_ALL 0

namespace fs = std::filesystem;

class SourceDataManager
{
public:
	SourceDataManager();
	void createData(const fs::path& path, const std::vector<std::string>& functions);

private:
	tls::LogManager log;
	fs::path rootPath;
	std::vector<std::string> funcNames;
	std::vector<SourceData> data;
	std::vector<std::string> extensions = { ".c", ".C", ".cpp", ".CPP" };


	fs::path findFile(std::string file);
	void parseFile(const fs::path& p, SourceData& element);
	bool containsStr(const std::string& line, const std::string& str);
	std::string getContentInBrackets(const std::string& line);
};

