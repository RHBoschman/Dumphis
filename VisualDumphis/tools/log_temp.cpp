#pragma once

#include "log.h"

namespace tls {
    template<typename T, typename... Args>
    void LogManager::formatString(std::ostream& os, const std::string& format, T value, Args... args) {
        try {
            size_t found = format.find_first_of('%');
            if (found != std::string::npos) {
                if (format[found + 1] == '%') {
                    os << format.substr(0, found + 1);
                    formatString(os, format.substr(found + 2), value, args...);
                }
                else if (format[found + 1] != 's') {
                    std::string fs = std::string(1, format[found + 1]);
                    throw std::invalid_argument("\nFATAL: unrecognized format specifier: \'" + fs + "\'");
                }
                else {
                    os << format.substr(0, found);
                    os << value;
                    formatString(os, format.substr(found + 2), args...);
                }
            }
            else {
                os << format;
            }
        }
        catch (const std::exception& e) {
            std::cerr << e.what();
            throw;
        }
    }

    template<typename... Args>
    void LogManager::fLog(const std::string& format, Args... args) {
        if (checkFormatLog(format, (int)sizeof...(Args))) {
            std::ostringstream oss;
            formatString(oss, format, args...);
            log(oss.str());
        }
        else
            return;
    }

    template<typename... Args>
    void LogManager::fLogPrefix(const unsigned int prefix, const std::string& format, Args... args) {
        if (checkFormatLog(format, (int)sizeof...(Args))) {
            std::ostringstream oss;
            formatString(oss, format, args...);
            log(oss.str(), prefix);
        }
        else
            return;
    }

    template<typename... Args>
    void LogManager::fLogNoNl(const std::string& format, Args... args) {
        if (checkFormatLog(format, (int)sizeof...(Args))) {
            std::ostringstream oss;
            formatString(oss, format, args...);
            logNoNl(oss.str());
        }
        else
            return;
    }
}