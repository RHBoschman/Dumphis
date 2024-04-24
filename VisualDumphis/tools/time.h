#pragma once

#include <string>
#include <ctime>
#include <chrono>
#include <iostream>

#define BUFF_SIZE 100

namespace tls {
	class TimeObject {
	public:
		TimeObject(std::tm timeInf);
		~TimeObject(void);

		std::string getTimeStdStr(void);
		std::string getDateStdStr(void);
		std::string getTimeFormat(const std::string& format);

	private:
		char* buffer;
		std::tm tm;

		bool checkFormat(const std::string& format);
	};

	class Time {
	public:
		TimeObject getTime(void);

		// TO BE IMPLEMENTED:
		void getRawTime(void);
		void getMilliSecs(void);
		// Schedule tasks
		void planAction(TimeObject); // Add also function pointer 
		// Measure time (stopwatch)
		void startStopwatch(void);
		TimeObject readStopwatch(void); // Is TimeObject suitable to get a time interval??

	private:
	};
}