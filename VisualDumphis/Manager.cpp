#include "Manager.h"

Manager::Manager(void) : log("Manager") {}

void Manager::start(void) {
	if (gui.askClosedQuestion("Have you built the project?", true))
		askForInput();
	else
		exit(0);

	readFiles();
}

void Manager::askForInput(void) {
	std::string projectPathStr, snapshotPathStr;
	
	projectPathStr = gui.askOpenQuestion("Please enter the path to the project:");
	std::cout << "You entered: " << projectPathStr;

	snapshotPathStr = gui.askOpenQuestion("Please enter the path to the snapshot (MUST correspond with project):");
	std::cout << "You entered: " << snapshotPathStr;

	//TMP FOR QUICKER TESTING
	projectPathStr = "C:\\Users\\robinheslinga\\BTProjects\\RotorCore";
	snapshotPathStr = "C:\\Users\\robinheslinga\\OneDrive - Boschman Technologies B.V\\Documenten\\Projecten\\dumphis visualiseren\\S0000011";

	if (!verifyInput(projectPathStr, snapshotPathStr)) {
		log.logFatal("Problem with given paths. Application will terminate");
		exit(0);
	} 
	else {
		std::cout << "\nEntered paths are correct";
	}
}

bool Manager::verifyInput(const std::string& project, const std::string& snapshot) {
	projectPath = project;
	snapshotPath = snapshot;

	if (!fs::exists(projectPath)) {
		std::cerr << "\nError: Project's path " << projectPath.string() << " does not exist";
		return false;
	}

	if (!fs::is_directory(projectPath)) {
		std::cerr << "\nError: " << projectPath.string() << " is not a directory";
		return false;
	}

	if (!fs::exists(snapshotPath)) {
		log.logError(std::format("Snapshot's path {} does not exist", snapshotPath.string()));
		return false;
	}

	if (!fs::is_directory(snapshotPath)) {
		log.logError(std::format("{} is not a directory. (Did you unzip?)", snapshotPath.string()));
		return false;
	}

	return true;
}

void Manager::readFiles(void) {
	// Verify all files
	if (!doesFileExist(snapshotPath, file_dumpcdb) || 
		!doesFileExist(snapshotPath, file_dumphis) ||
		!doesFileExist(projectPath, file_cdb_data) ||
		!doesFileExist(projectPath, file_iom_name) 
		) {
		log.logFatal("Problem with finding a file. Application will terminate");
		exit(0);
	}

	// Handle dumpcdb.csv
	log.blankLine();
	fs::path path_dumpcdb = snapshotPath / file_dumpcdb;
	log.logInfo(std::format("File path : {}", path_dumpcdb.string()));
	dumpcdbRead.createData(path_dumpcdb.string());

	// Handle dumphis.csv
	log.blankLine();
	fs::path path_dumphis = snapshotPath / file_dumphis;
	log.logInfo(std::format("File path : {}", path_dumphis.string()));
	dumphisRead.createData(path_dumphis.string());

	// Handle cdb_data.h
	log.blankLine();
	fs::path path_cdb_data = projectPath / file_cdb_data;
	log.logInfo(std::format("File path : {}", path_cdb_data.string()));
	cdbdataRead.createData(path_cdb_data.string());

	// Handle IOM_DATA.PP
	log.blankLine();
	fs::path path_iom_name = projectPath / file_iom_name;
	log.logInfo(std::format("File path : {}", path_iom_name.string()));
	iomnameRead.createData(path_iom_name.string());

	// Get further data from source files
	log.blankLine();
	fs::path projectCtl = projectPath / ctlRoot;
	sourceDataRead.createData(projectCtl, dumpcdbRead.getFuncNames());
}

bool Manager::doesFileExist(const fs::path& root, const std::string& relativePath) {
	fs::path path = root / relativePath;
	if (!fs::exists(path)) {
		log.logError(std::format("{} does not exist at {}", relativePath, root.string()));
		return false;
	}
	return true;
}
