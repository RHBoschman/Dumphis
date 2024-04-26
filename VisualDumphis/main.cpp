#include "Manager.h"
#include "tools/tls.h"

#include <filesystem>

namespace fs = std::filesystem;

int main(void) {
	tls::LogAdmin logAdmin;
	tls::LogManager log;
	logAdmin.setAllParams(false, false, true);

	Manager manager;
	manager.start();

	log.log("\n"); // For readability
	return 0;
}