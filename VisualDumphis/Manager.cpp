#include "Manager.h"

Manager::Manager(void) : log("Manager") {}

void Manager::start(void) {
	if (gui.askClosedQuestion("Have you built the project?", true))
		askForInput();
	else
		exit(0);

	log.blankLine();
	readFiles();
}

void Manager::askForInput(void) {
	std::string projectPathStr, snapshotPathStr;
	
	projectPathStr = gui.askOpenQuestion("Please enter the path to the project:");
	std::cout << "You entered: " << projectPathStr;

	snapshotPathStr = gui.askOpenQuestion("Please enter the path to the snapshot (MUST correspond with project):");
	std::cout << "You entered: " << snapshotPathStr;

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

	/*if (!fs::exists(projectPath)) {
		std::cerr << "\nError: Snapshot's path " << projectPath.string() << " does not exist";
		return false;
	}

	if (!fs::is_directory(projectPath)) {
		std::cerr << "\nError: " << projectPath.string() << " is not a directory";
		return false;
	}*/
	// Further checks for project come later (TODO)

	if (!fs::exists(snapshotPath)) {
		log.logError(std::format("Snapshot's path {} does not exist", snapshotPath.string()));
		return false;
	}

	if (!fs::is_directory(snapshotPath)) {
		log.logError(std::format("{} is not a directory. (Did you unzip?)", snapshotPath.string()));
		return false;
	}

	path_dumpcdb = snapshotPath / file_dumpcdb;
	if (!fs::exists(path_dumpcdb)) {
		log.logError(std::format("{} does not exist at {}", file_dumpcdb, path_dumpcdb.string()));
		return false;
	}

	return true;
}

void Manager::readFiles(void) {
	DumpCDBManager dumpcdbRead;
	std::cout << "\nFile path: " << path_dumpcdb.string();
	dumpcdbRead.createData(path_dumpcdb.string());
}
