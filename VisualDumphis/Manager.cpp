#include "Manager.h"

Manager::Manager(void) {
	log.setCaller("Manager");
	log.log("Constructed");
}

void Manager::start(void) {
	log.log("Started");

	std::string choice;
	bool askAgain = false;
	std::cout << "\nMake sure you have built the project";
	do {
		std::cout << "\nDid you build the project? [Y/N] ";
		std::cin >> choice;
		if (choice == "Y" || choice == "y") {
			askAgain = false;
			askForInput();
		}
		else if (choice == "N" || choice == "n") {
			exit(0);
		}
		else {
			askAgain = true;
		}
	} while (askAgain);

	readFiles();
}

void Manager::askForInput(void) {
	std::string projectPathStr, snapshotPathStr;
	
	std::cout << "\nPlease enter the path to the project:\n";
	std::cin.ignore();
	std::getline(std::cin, projectPathStr);
	std::cout << "You entered: " << projectPathStr;
	std::cout << "\nPlease enter the path to the snapshot (MUST correspond with project):\n";
	std::getline(std::cin, snapshotPathStr);
	std::cout << "You entered: " << snapshotPathStr;

	if (!verifyInput(projectPathStr, snapshotPathStr)) {
		std::cerr << "\nError: Problem with given paths. Application will terminate";
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
		std::cerr << "\nError: Snapshot's path " << snapshotPath.string() << " does not exist";
		return false;
	}

	if (!fs::is_directory(snapshotPath)) {
		std::cerr << "\nError: " << snapshotPath.string() << " is not a directory. (Did you unzip?)";
		return false;
	}

	path_dumpcdb = snapshotPath / file_dumpcdb;
	if (!fs::exists(path_dumpcdb)) {
		std::cerr << "\nError: " << file_dumpcdb << " does not exist at " << path_dumpcdb.string();
		return false;
	}

	return true;
}

void Manager::readFiles(void) {
	DumpCDBManager dumpcdbRead;
	dumpcdbRead.createData(path_dumpcdb.string());
}
