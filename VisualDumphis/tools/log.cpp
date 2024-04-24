#include "log.h"

namespace fs = std::filesystem;

namespace tls {
    std::string LogAdmin::filePath = "";
    bool LogAdmin::showDate = false;
    bool LogAdmin::showTimeStamp = false;
    bool LogAdmin::showCallers = false;
    int LogAdmin::titleLength = 40;

    /**
    * @brief Set the output file to log to
    *
    * @param fp The file path
    */
    void LogAdmin::setOutputFile(const std::string& fP) {
        filePath = fP;
        if (!fs::exists(filePath)) {
            std::cerr << "\nError: File path does not exits: " << filePath;
        }
    }

    void LogAdmin::removeOutputFile(void) {
        filePath = "";
    }

    void LogAdmin::clearFile(void) {
        if (isFilePresent()) {
            logFile.open(filePath, std::ofstream::out | std::ofstream::trunc);

            if (!logFile.is_open()) {
                std::cerr << "\nError: Could not open the file to clear: " << filePath;
                return;
            }
            else {
                logFile << "";
            }
            logFile.close();
        }
        else {
            std::cerr << "\nError: No file specified";
        }
    }

    void LogAdmin::setShowDate(const bool sD) {
        showDate = sD;
    }

    void LogAdmin::setShowTimeStamp(const bool sTS) {
        showTimeStamp = sTS;
    }

    void LogAdmin::setShowCallers(const bool sC) {
        showCallers = sC;
    }

    void LogAdmin::setAllParams(const bool showDate, const bool showTimeStamp, const bool showCallers) {
        this->showDate = showDate;
        this->showTimeStamp = showTimeStamp;
        this->showCallers = showCallers;
    }

    void LogAdmin::setTitleLength(const int len) {
        titleLength = len;
    }

    bool LogAdmin::isFilePresent(void) const {
        if (filePath == "")
            return false;
        else
            return true;
    }

    LogManager::LogManager() {
        prefixes.push_back("");
        time = Time();
    }

    LogManager::~LogManager() {
        if (logFile.is_open())
            logFile.close();
    }

    void LogManager::logNoNl(const std::string& msg) {
        std::ostringstream oss;
        oss << msg;
        std::cout << oss.str();
        writeFile(oss);
    }

    void LogManager::log(const std::string& msg, const unsigned int prefix) {
        if (prefix > (int)(prefixes.size() - 1) || prefix < 0) {
            bool negative = false;
            std::string negNumber = "";
            if (prefix > 1000000) { // Probably a negative number was given
                negative = true;
                negNumber = " (" + std::to_string((int)prefix) + "?)";
            }
            std::cerr << "\nError: prefix id " << prefix << (negative ? negNumber : "") << " is not available";
            return;
        }

        std::ostringstream oss;
        TimeObject to = time.getTime();
        oss << "\n" + (showDate ? (to.getDateStdStr() + " ") : "");
        oss << (showTimeStamp ? (to.getTimeStdStr() + " ") : "");
        oss << (showCallers ? caller : "");
        oss << (((showDate || showTimeStamp || showCallers) && caller != "") ? ": " : "");
        oss << prefixes[prefix] << msg;
        std::cout << oss.str();
        writeFile(oss);
    }

    void LogManager::blankLine(void) {
        std::ostringstream oss;
        oss << "\n";
        std::cout << oss.str();
        writeFile(oss);
    }

    void LogManager::setCaller(const std::string& cllr) {
        caller = "[" + cllr + "] ";
    }

    void LogManager::removeCaller(void) {
        caller = "";
    }

    int LogManager::setPrefix(const std::string& pf) {
        prefixes.push_back(pf + ": ");
        return (int)(prefixes.size() - 1);
    }

    std::vector<std::string> LogManager::getPrefixList(void) {
        return prefixes;
    }

    void LogManager::logTitle(const std::string& title) {
        std::string t = " " + title + " ";
        try {
            if (t.size() > (titleLength - 2))
                throw std::invalid_argument("\nFATAL: title is too long! Max. " + std::to_string(titleLength - 4) + " characters, was: "
                + std::to_string(title.size()));
        }
        catch (std::exception& e) {
            std::cerr << e.what();
            return;
        }

        std::ostringstream oss;
        int lineLength = titleLength - (int)t.size();
        int singleLineLength = 0;
        bool notEven = false;
        if (lineLength % 2 != 0.0f) {
            notEven = true;
            singleLineLength = (lineLength - 1) / 2;
        }
        else {
            singleLineLength = lineLength / 2;
        }

        setLine(oss, singleLineLength);
        oss << t;
        if (notEven)
            setLine(oss, singleLineLength + 1);
        else
            setLine(oss, singleLineLength);

        std::cout << "\n" << oss.str();
        writeFile(oss);
    }

    void LogManager::formatString(std::ostream& os, const std::string& format) {
        size_t pos = 0;
        std::string changedStr = format;
        while ((pos = changedStr.find("%%", pos)) != std::string::npos) {
            changedStr.replace(pos, 2, "%");
            pos++;
        }

        os << changedStr;
    }

    void LogManager::writeFile(const std::ostringstream& oss) {
        if (isFilePresent()) {
            if (curFilePath != filePath) {
                syncFilePath();
                if (logFile.is_open())
                    logFile.close();
                logFile.open(filePath, std::ofstream::out | std::ofstream::app);
            }

            if (!logToFile(oss))
                std::cerr << "\nError while writing to file";
        }
    }

    bool LogManager::logToFile(const std::ostringstream& oss) {
        if (filePath == "" || !logFile.is_open()) {
            std::cerr << "\nCould not write to file: " << ((filePath == "") ? "no file path specified" : "file not open");
            return false;
        }

        logFile << oss.str();
        return true;
    }

    void LogManager::syncFilePath(void) {
        curFilePath = filePath;
    }

    bool LogManager::checkFormatLog(const std::string& str, const unsigned int n_args) {
        int count = 0;
        size_t pos = 0;
        while ((pos = str.find('%', pos)) != std::string::npos) {
            if (str[pos + 1] != '%') {
                count++;
                pos++;
            }
            else {
                pos += 2;
            }
        }

        if (count != n_args) {
            std::cerr << "\nError: number of format specifiers does not match the number of arguments";
            return false;
        }
        else
            return true;
    }

    void LogManager::setLine(std::ostream& os, const int cnt) const {
        for (int i = 0; i < cnt; i++) {
            os << '-';
        }
    }
}

