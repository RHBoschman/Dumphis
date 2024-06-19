#pragma once

#include <filesystem>
#include <iostream>
#include <format>
#include <vector>
#include <math.h>
#include "tools/tls.h"
#include "SourceData.h"
#include "AbstractDataManager.h"

#define MAX_FIND_ITERATIONS 3
#define PRINT_ALL 0

namespace fs = std::filesystem;

class SourceDataManager : public AbstractDataManager {
public:
	SourceDataManager();
	void createData(const std::string& path) override; // Needed to make sure class is not abstract
	void createSourceData(const fs::path& path, const std::vector<std::string>& functions);

private:
	tls::LogManager log;
	fs::path rootPath;
	std::vector<std::string> funcNames;
	std::vector<SourceData> data;
	std::vector<std::string> extensionsC = { ".c", ".C", ".cpp", ".CPP" };
	std::vector<std::string> extensionsH = { ".h", ".H", ".hpp", ".HPP" };


	fs::path findFile(std::string file, std::vector<std::string>& exts);
	void parseCFile(const fs::path& p, SourceData& element);
	void parseHFile(const fs::path& p, SourceData& element);
	bool containsStr(const std::string& line, const std::string& str);
	std::string getContentInBrackets(const std::string& line);
	int findDefineIndex(const std::string& str);
	bool isComment(const std::string& line);
};

