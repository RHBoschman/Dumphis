#include "SourceDataManager.h"


// TODO:  
// -  Check whether #define is in /* */ block in parseHFile() ?? maybe ignore this for now
// -  Check output because indexing is still not quite right

SourceDataManager::SourceDataManager() : log("SourceDataManager") {}
void SourceDataManager::createData(const std::string& path) {}

void SourceDataManager::createSourceData(const fs::path& path, const std::vector<std::string>& functions) {
	rootPath = path;
	funcNames = functions;

	if (!fs::exists(path) || !fs::is_directory(path)) {
		log.logFatal(std::format("{} is not the path to ctl directory. Application will terminate", rootPath.string()));
		exit(0);
	}

	// Find source data by parsing .c/.cpp files
	log.logInfo("Searching in source files...");
	log.logInfo(std::format("Number of functions: {}", functions.size()));
	log.logInfo("Searching on extensions: ");
	for (std::string e : extensionsC) {
		log.logNoNl(e);
		log.logNoNl(" ");
	}

	int n_found = 0, n_notFound = 0, total;
	for (size_t i = 0; i < funcNames.size(); i++) {
		SourceData element;
		element.setFuncIndex(i);

		// Find func.c / .C / .cpp / .CPP
		std::string CFileName = funcNames[i];
		fs::path CFilePath = findFile(CFileName, extensionsC);
		if (!CFilePath.empty()) {
			n_found++;
			element.setFoundFunction();
			element.setFilePath(CFilePath.string());
			parseCFile(CFilePath, element);
		}
		else {
			log.logError(std::format("Could not find a source file for {}", CFileName));
			n_notFound++;
		}

		// If no file is found, the SourceData element stays empty 
		// (except for the function index). But it IS added because
		// every function should have some info. 
		try {
			data.push_back(element);
		}
		catch (std::exception& e) {
			log.logFatal(std::format("Error while pushing data: {}. Application will terminate", e.what()));
			exit(0);
		}
	}

	log.logInfo(std::format("Found {} files", n_found));
	log.logInfo(std::format("Could not find {} files", n_notFound));
	total = n_found + n_notFound;
	float loss = ((float)n_notFound / (float)total) * 100.0f;
	log.logInfo(std::format("Total: {}, loss: {}%", total, std::round(loss)));


	// Find cmnd indexes by parsing .h files
	log.logInfo("Searching in header files...");
	log.logInfo(std::format("Number of functions: {}", functions.size()));
	log.logInfo("Searching on extensions: ");
	for (std::string e : extensionsH) {
		log.logNoNl(e);
		log.logNoNl(" ");
	}

	n_found = 0, n_notFound = 0, total = 0;
	for (size_t i = 0; i < data.size(); i++) {
		// Find func.h / .H / .hpp / .HPP

		if (data[i].hasCMNDs()) {
			std::string HFileName = funcNames[data[i].getFuncIndex()];
			fs::path HFilePath = findFile(HFileName, extensionsH);

			if (!HFilePath.empty()) {
				n_found++;
				parseHFile(HFilePath, data[i]);
			}
			else {
				log.logError(std::format("Could not find a source file for {}", HFileName));
				n_notFound++;
			}
		}
	}

	log.logInfo(std::format("Found {} files", n_found));
	log.logInfo(std::format("Could not find {} files", n_notFound));
	total = n_found + n_notFound;
	loss = ((float)n_notFound / (float)total) * 100.0f;
	log.logInfo(std::format("Total: {}, loss: {}%", total, std::round(loss)));

	// Check if all cmnd indexes are found
	int n_cmndFound = 0, n_cmndNotFound = 0;
	for (SourceData e : data) {
		std::vector<CMND> cmnds = e.getCMNDs();
		for (CMND f : cmnds) {
			if (f.CmndIndex > -1)
				n_cmndFound++;
			else
				n_cmndNotFound++;

			for (FALL_CMND g : f.fallCmnds) {
				if (g.CmndIndex > -1)
					n_cmndFound++;
				else
					n_cmndNotFound++;
			}
		}
	}

	log.logInfo(std::format("Found {} indexes for (FALL_)CMNDs", n_cmndFound));
	log.logInfo(std::format("Could not find {} indexes", n_cmndNotFound));
	total = n_cmndFound + n_cmndNotFound;
	loss = ((float)n_cmndNotFound / (float)total) * 100.0f;
	log.logInfo(std::format("Total: {}, loss: {}%", total, std::round(loss)));

#if PRINT_ALL
	for (SourceData e : data) {
		std::cout << "\n" << funcNames[e.getFuncIndex()];
		e.printData();
		log.blankLine();
	}
#endif
}

std::vector<SourceData>* SourceDataManager::getData(void) {
	return &data;
}

fs::path SourceDataManager::findFile(std::string file, std::vector<std::string>& exts) {
	for (int i = 0; i < MAX_FIND_ITERATIONS; i++) {
		try {
			for (const auto& entry : fs::recursive_directory_iterator(rootPath)) {
				if (entry.is_regular_file()) {
					for (const std::string e : exts) {
						if (entry.path().filename() == file + e) {
							return entry.path(); // Stop searching
						}
					}
				}
			}
		}
		catch (std::exception& e) {
			log.logError(std::format("Problem while searching for file {}", file));
		}

		if (i == 0) { // First try failed (normal)
			if (std::isalpha(file[0])) {
				file[0] = std::toupper(file[0]);
			}
		}
		else if (i == 1) { // Second try failed (first capital)
			std::transform(file.begin(), file.end(), file.begin(), ::toupper);
		}
		// Third try: full capital
	}

	return "";
}

void SourceDataManager::parseCFile(const fs::path& p, SourceData& element) {
	tls::File file(p.string());
	std::string line;
	bool inElement = false;
	CMND tmp;
	std::vector<FALL_CMND> fallCmnds;

	while (file.readLine2(line)) {
		if (containsStr(line, "FALL_CMND")) {
			FALL_CMND fallCmnd;
			fallCmnd.name = getContentInBrackets(line);
			fallCmnds.push_back(fallCmnd);
		}

		if (containsStr(line, "CMND")) {
			inElement = true;
			tmp.name = getContentInBrackets(line);
			tmp.fallCmnds = fallCmnds;
			fallCmnds.clear();
		}

		if (inElement) {
			if (containsStr(line, "START_STEP")) {
				STEP step;
				step.name = "START_STEP";
				tmp.steps.push_back(step);
			}
			else if (containsStr(line, "STEP")) {
				STEP step;
				step.name = getContentInBrackets(line);
				tmp.steps.push_back(step);
			}
		}

		if (containsStr(line, "ENDCMND")) {
			inElement = false;
			CMND cmnd;
			cmnd = tmp;
			element.addCmnd(cmnd);
			
			// Clear tmp
			tmp.name = "";
			tmp.steps.clear();
		}
	}

	if (!fallCmnds.empty())
		log.logError("FALL_CMNDs not connected to CMND");
}

void SourceDataManager::parseHFile(const fs::path& p, SourceData& element) {
	tls::File file(p.string());
	std::string line;
	std::vector<CMND> cmnds = element.getCMNDs();

	while (file.readLine2(line)) {
		// Check whether #define is in /* */ block...

		for (size_t i = 0; i < cmnds.size(); i++) {
			bool fallCmndFound = false;

			if (containsStr(line, "#define " + cmnds[i].name) && !isComment(line)) {
				element.setCmndIndex(i, findDefineIndex(line));
				break; // Corresponding cmnd is found, so skip to next line
			}

			for (size_t j = 0; j < cmnds[i].fallCmnds.size(); j++) {
				if (containsStr(line, "#define " + cmnds[i].fallCmnds[j].name) && !isComment(line)) {
					element.setFallCmndIndex(i, j, findDefineIndex(line));
					fallCmndFound = true;
					break;
				}
			}
			
			if (fallCmndFound)
				break; // Corresponding fall cmnd is found, so skip to next line
		}
	}
}

bool SourceDataManager::containsStr(const std::string& line, const std::string& str) {
	std::string tmp = "*" + line + "*";
	size_t pos = tmp.find(str);
	if (pos != std::string::npos) {
		if (!std::isalpha(tmp[pos - 1]) && 
			!std::isalpha(tmp[pos + str.length()]) &&
			tmp[pos - 1] != '_' &&
			tmp[pos + str.length()] != '_'
			)
			return true;
	}
	return false;
}

std::string SourceDataManager::getContentInBrackets(const std::string& line) {
	std::string result = "";
	size_t pos_bracketOpen = 0;
	size_t pos_bracketClose = 1;
	int n_toIgnore = 0;
	bool closingFound = false;

	for (int i = 0; i < line.size(); i++) {
		char c = line[i];
		if (c == '(') {
			n_toIgnore++;
			pos_bracketOpen = i;
		}
		else if (c == ')') {
			n_toIgnore--;
			closingFound = true;
			pos_bracketClose = i;
		}

		if (n_toIgnore == 0 && closingFound) {
			result = line.substr((pos_bracketOpen + 1), (pos_bracketClose - pos_bracketOpen - 1));
			break;
		}
	}

	if (result.empty() || n_toIgnore != 0 || !closingFound) {
		log.logError("Error finding content in brackets");
		return "";
	}

	std::string::iterator end_pos = std::remove(result.begin(), result.end(), ' ');
	result.erase(end_pos, result.end());

	return result;
}

int SourceDataManager::findDefineIndex(const std::string& str) {
	std::string define = "#define";
	size_t pos_define = str.find(define);

	if (pos_define != std::string::npos) {
		std::string substr = str.substr((pos_define + define.length() + 1));

		DefineDataObject ddo;
		ddo.setName(getName(substr)); // Not used but necesary to strip string for getting the number
		ddo.setValue(getNumber(substr));

		return ddo.getValue();
	}
	else {
		log.logError("'#define' not found");
		return -1;
	}
}

bool SourceDataManager::isComment(const std::string& line) {
	// This check is not complete: it can't know whether text is in a /* */ block!
	bool result = false;

	for (int i = 0; i < line.size(); i++) {
		char c = line[i];
		if (c >= 33 && c <= 126) { // ASCII readable characters
			if (c == '/') {
				if (line[i + 1] == '/' || line[i + 1] == '*') {
					result = true;
				}
			}
			else if (c == '*') {
				result = true;
			}
			break; // Stop checking when a readable char was found
		}
	}

	return result;
}