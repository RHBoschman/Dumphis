#include "time.h"

namespace tls {
	TimeObject::TimeObject(std::tm timeInf) : tm(timeInf) {
		buffer = new char[BUFF_SIZE];
	}

	TimeObject::~TimeObject(void) {
		delete[] buffer;
	}

	std::string TimeObject::getTimeStdStr(void) {
		strftime(buffer, BUFF_SIZE, "%X", &tm);
		return std::string(buffer);
	}

	std::string TimeObject::getDateStdStr(void) {
		strftime(buffer, BUFF_SIZE, "%Y-%m-%d", &tm);
		return std::string(buffer);
	}

	std::string TimeObject::getTimeFormat(const std::string& format) {
		if (!checkFormat(format)) {
			std::cerr << "\nInvalid format: \"" << format << "\"";
			return "";
		}

		if (strftime(buffer, BUFF_SIZE, format.c_str(), &tm) == 0) {
			throw std::invalid_argument("Unable to format time");
		}

		return std::string(buffer);
	}

	bool TimeObject::checkFormat(const std::string& format) {
		size_t pos = 0;
		while ((pos = format.find("%", pos)) != std::string::npos) {
			if (
				format[pos + 1] == '-' ||
				format[pos + 1] == '_' ||
				format[pos + 1] == '0' ||
				format[pos + 1] == '^' ||
				format[pos + 1] == '#' 
				) {
				if (
					format[pos + 2] != 'a' &&
					format[pos + 2] != 'A' &&
					format[pos + 2] != 'b' &&
					format[pos + 2] != 'B' &&
					format[pos + 2] != 'c' &&
					format[pos + 2] != 'd' &&
					format[pos + 2] != 'H' &&
					format[pos + 2] != 'I' &&
					format[pos + 2] != 'j' &&
					format[pos + 2] != 'm' &&
					format[pos + 2] != 'M' &&
					format[pos + 2] != 'p' &&
					format[pos + 2] != 'S' &&
					format[pos + 2] != 'U' &&
					format[pos + 2] != 'w' &&
					format[pos + 2] != 'W' &&
					format[pos + 2] != 'x' &&
					format[pos + 2] != 'X' &&
					format[pos + 2] != 'y' &&
					format[pos + 2] != 'Y' &&
					format[pos + 2] != 'z' &&
					format[pos + 2] != 'Z'
					) {
					return false;
				} 
				else {
					pos += 3;
				}
			}
			else {
				if (
					format[pos + 1] != 'a' &&
					format[pos + 1] != 'A' &&
					format[pos + 1] != 'b' &&
					format[pos + 1] != 'B' &&
					format[pos + 1] != 'c' &&
					format[pos + 1] != 'd' &&
					format[pos + 1] != 'H' &&
					format[pos + 1] != 'I' &&
					format[pos + 1] != 'j' &&
					format[pos + 1] != 'm' &&
					format[pos + 1] != 'M' &&
					format[pos + 1] != 'p' &&
					format[pos + 1] != 'S' &&
					format[pos + 1] != 'U' &&
					format[pos + 1] != 'w' &&
					format[pos + 1] != 'W' &&
					format[pos + 1] != 'x' &&
					format[pos + 1] != 'X' &&
					format[pos + 1] != 'y' &&
					format[pos + 1] != 'Y' &&
					format[pos + 1] != 'z' &&
					format[pos + 1] != 'Z'
					) {
					return false;
				}
				else {
					pos += 2;
				}
			}
		}

		return true;
	}

	TimeObject Time::getTime(void) {
		auto now = std::chrono::system_clock::now();
		auto now_c = std::chrono::system_clock::to_time_t(now);

		std::tm timeInfo;
		localtime_s(&timeInfo, &now_c);
		return TimeObject(timeInfo);
	}
}