#include "Manager.h"
#include "tools/tls.h"

int main(void) {
	tls::LogAdmin logAdmin;
	tls::LogManager log;
	logAdmin.setAllParams(false, false, true);

	Manager manager;
	manager.start();

	log.log("\n"); // For readability
	return 0;
}
