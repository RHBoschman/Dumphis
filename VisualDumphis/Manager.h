#pragma once

#include "tools/tls.h"
#include "iostream"
#include "filesystem"

namespace std::filesystem = fs;

class Manager
{
public:
	Manager(void);
	void start(void);

private:
	tls::LogManager log;

	void askForInput(void);
	bool verifyInput(void);
	bool verifyFile(void);
};

