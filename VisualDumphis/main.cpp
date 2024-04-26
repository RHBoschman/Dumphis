#include "Manager.h"
#include "tools/tls.h"

#include <filesystem>

namespace fs = std::filesystem;

#if 1
int main(void) {
	tls::LogAdmin logAdmin;
	tls::LogManager log;
	logAdmin.setAllParams(false, false, true);

	Manager manager;
	manager.start();

	log.log("\n"); // For readability
	return 0;
}

#else // For Test

int main(void) {
	tls::File file("afile.txt");
	std::cout << file.readLine();
	std::cout << "\n" << file.readAllContent();
	std::cout << file.readLine();
	file.closeFile();

	file.openFile("afile2.txt");
	std::cout << "\n" << file.readAllContent();

	return 0;
}

#endif