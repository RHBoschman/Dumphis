#include "Manager.h"

Manager::Manager(void) {
	log.setCaller("Manager");
	log.log("Constructed");
}

void Manager::start(void) {
	log.log("Started");
	askForInput();
}

void Manager::askForInput(void) {
	std::string projectPath;
	std::string dumphisPath;

	std::cout << "\nPlease enter the path to the project:\n";
	std::cin >> projectPath;
	std::cout << "\nPlease enter the path to the dumphis.csv file:\n";
	std::cin >> dumphisPath;
}

bool Manager::verifyInput(void) {
	return true;
}

bool Manager::verifyFile(void) {
	return true;
}

