#include "gui.h"

#include <sstream>

namespace tls {
	bool Gui::askClosedQuestion(const std::string& msg, const bool noNl) {
		std::string input;
		bool askAgain = false;

		if (!noNl)
			std::cout << "\n";

		do {
			std::cout << msg << " [Y/N]: ";
			std::getline(std::cin, input);
	
			if (input == "Y" || input == "y") {
				askAgain = false;
				return true;
			}
			else if (input == "N" || input == "n") {
				return false;
			}
			else {
				askAgain = true;
			}
		} while (askAgain);
	}

	std::string Gui::askOpenQuestion(const std::string& msg, const bool noNl) {
		std::string input;
		if (!noNl)
			std::cout << "\n";
		std::cout << msg << " ";
		std::getline(std::cin, input);
		return input;
	}
}