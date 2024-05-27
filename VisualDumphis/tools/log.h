#pragma once

#include "time.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>
#include <ctime>
#include <chrono>

namespace tls {
	class LogAdmin {
	public:
		void setOutputFile(const std::string& fP);
		void removeOutputFile(void);
		void clearFile(void);
		void setShowDate(const bool sD);
		void setShowTimeStamp(const bool sTS);
		void setShowCallers(const bool sC);
		void setAllParams(const bool showDate, const bool showTimeStamp, const bool showCallers);
		void setTitleLength(const int len);
		void disableStdCout(void);

	protected:
		static std::string filePath;
		static bool showDate;
		static bool showTimeStamp;
		static bool showCallers;
		static bool doCout;
		static int titleLength;
		std::ofstream logFile;

		bool isFilePresent(void) const;
	};

	class LogManager : LogAdmin {
	public:
		LogManager();
		LogManager(const std::string& cllr);
		~LogManager();

		void logNoNl(const std::string& msg);
		void log(const std::string& msg, const unsigned int prefix = 0);
		void logInfo(std::string msg);
		void logWarning(std::string msg);
		void logError(std::string msg);
		void logFatal(std::string msg);
		void blankLine(void);
		void setCaller(const std::string& cllr);
		void removeCaller(void);
		int setPrefix(const std::string& pf);
		std::vector<std::string> getPrefixList(void);
		void logTitle(const std::string& titel);

		template<typename... Args>
		void fLog(const std::string& format, Args... args);

		template<typename... Args>
		void fLogPrefix(const unsigned int prefix, const std::string& format, Args... args);

		template<typename... Args>
		void fLogNoNl(const std::string& format, Args... args);

	private:
		std::string caller = "";
		std::ofstream logFile;
		std::string curFilePath = "";
		Time time;
		std::vector<std::string> prefixes;

		void formatString(std::ostream& os, const std::string& format);
		void writeFile(const std::ostringstream& oss);
		bool logToFile(const std::ostringstream& oss);
		void syncFilePath(void);
		bool checkFormatLog(const std::string& str, const unsigned int n_args);
		void setLine(std::ostream& os, const int cnt) const;

		template<typename T, typename... Args>
		void formatString(std::ostream& os, const std::string& format, T value, Args... args);
	};

	class SimpleLog {
	public:
		static void log(const std::string& msg);
	};
}

#include "log_temp.cpp"