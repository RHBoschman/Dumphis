#include "SourceDataManager.h"


//TODO: 
// - Trim result getContentInBrackets()
// - ContainsStr specifiek naar één woord laten zoeken (Dus CMND en FALLCMND apart)
// - FALLCMND moet eigenlijk worden afgehandeld zoals volgende CMND afgehandeld wordt (hoe?, moet een soort pending state krijgen)
// - Betere afwerking met haken (gaat nog niet goed met uitlezen)
// - Hij crasht uiteindelijk ook nog.

SourceDataManager::SourceDataManager() : log("SourceDataManager") {}

void SourceDataManager::createData(const fs::path& path, const std::vector<std::string>& functions) {
	rootPath = path;
	funcNames = functions;

	if (!fs::exists(path) || !fs::is_directory(path)) {
		log.logFatal(std::format("{} is not the path to ctl directory. Application will terminate", rootPath.string()));
		exit(0);
	}

	log.logInfo("Searching in source files...");
	log.logInfo(std::format("Number of functions: {}", functions.size()));
	log.logInfo("Searching on extensions: ");
	for (std::string e : extensions) {
		log.logNoNl(e);
		log.logNoNl(" ");
	}

	int n_found = 0, n_notFound = 0, total;
	for (size_t i = 0; i < funcNames.size(); i++) {

		SourceData element;
		element.setFuncIndex(i);

		// Find func.c / .C / .cpp / .CPP
		std::string CFileName = funcNames[i];
		fs::path CFilePath = findFile(CFileName);
		if (!CFilePath.empty()) {
			n_found++;
			element.setFoundFunction();
			element.setFilePath(CFilePath.string());
			parseFile(CFilePath, element);
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
	float loss = ((float)n_notFound / (float)total) * 100;
	log.logInfo(std::format("Total: {}, loss: {}%", total, std::round(loss)));

#if PRINT_ALL
	for (SourceData e : data) {
		std::cout << "\n" << funcNames[e.getFuncIndex()];
		e.printData();
		log.blankLine();
	}
#endif
}

fs::path SourceDataManager::findFile(std::string file) {
	for (int i = 0; i < MAX_FIND_ITERATIONS; i++) {
		try {
			for (const auto& entry : fs::recursive_directory_iterator(rootPath)) {
				if (entry.is_regular_file()) {
					for (const std::string e : extensions) {
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

void SourceDataManager::parseFile(const fs::path& p, SourceData& element) {
	tls::File file(p.string());
	std::string line;
	bool inElement = false;
	CMND tmp;

	while (file.readLine2(line)) {
		if (containsStr(line, "CMND") && !containsStr(line, "ENDCMND")) {
			inElement = true;
			tmp.name = getContentInBrackets(line);
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
		}
	}
}

bool SourceDataManager::containsStr(const std::string& line, const std::string& str) {
	if (line.find(str) != std::string::npos)
		return true;
	return false;
}

std::string SourceDataManager::getContentInBrackets(const std::string& line) {
	std::string result = "";
	size_t pos_bracketOpen = line.find('(');
	size_t pos_bracketClose = 0;
	if (pos_bracketOpen != std::string::npos) {
		pos_bracketClose = line.find(')', pos_bracketOpen + 1);

		result = line.substr((pos_bracketOpen + 1), (pos_bracketClose - pos_bracketOpen - 1));
	}
	else {
		log.logError("Did not find an opening bracket");
	}


	return result;
}