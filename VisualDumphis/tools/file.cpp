#include "file.h"

namespace tls {
	File::File() {}

	File::File(const std::string& filePath) {
		openFile(filePath);
	}

	File::~File() {
		closeFile();
	}

	void File::openFile(const std::string& filePath) {
		if (verifyInput(filePath))
			file.open(filePath, std::ios::in || std::ios::out);
	}

	void File::closeFile(void) {
		if (file.is_open())
			file.close();
	}

	std::string File::readAllContent(void) {
		file.clear();
		file.seekg(0, std::ios::beg);

		std::string content, line;
		bool f_firstTime = true;
		while (std::getline(file, line)) {
			if (f_firstTime) {
				content += line;
				f_firstTime = false;
			}
			else
				content += "\n" + line;
		}

		return content;
	}

	// Read line and return it
	std::string File::readLine(void) {
		std::string line;
		if (std::getline(file, line)) {
			return line;
		}
		else {
			return "";
		}
	}

	// Read line, write to referenced argument and 
	// return whether reading was successful or not
	bool File::readLine2(std::string& line) {
		if (std::getline(file, line))
			return true;
		else
			return false;
	}

	bool File::verifyInput(const std::string& p) {
		path = p;

		if (!fs::exists(path)) {
			std::cerr << "\nError: Snapshot's path " << path.string() << " does not exist";
			return false;
		}

		return true;
	}
}