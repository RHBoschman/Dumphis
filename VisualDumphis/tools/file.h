#pragma once

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

namespace tls {
	class File {
	public:
		File(); 
		File(const std::string& filePath);
		~File();
		
		void openFile(const std::string& filePath);
		void closeFile(void);
		std::string readAllContent(void);
		std::string readLine(void);
		bool readLine2(std::string& line);

	private:
		fs::path path;
		std::fstream file;

		bool verifyInput(const std::string& p);
	};
}