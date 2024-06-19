#include "Manager.h"
#include "tools/tls.h"

/* TODO:
* About retrieving data:
* - Linkage between CMND name and index (see SourceDataManager.cpp)
* 
* Later:
* - Unit parents (I already know its childeren, but parents is more difficult)
* - Possible outputs for Step (need to parse with function file parsing (SourceDataManager.cpp)
* - View code (will be part of javascript's code)
* 
* The rest:
* - Place data into JSON or XML format for further handling with JS
* - Creating HTML doc
* - Creating JS code
*/

int main(void) {
	tls::LogAdmin logAdmin;
	tls::LogManager log;
	logAdmin.setAllParams(false, false, true);

	Manager manager;
	try {
		manager.start();
	}
	catch (std::exception& e) {
		log.logError("Problem occured during execution of manager.start(): ");
		log.log(e.what());
	}
	

	log.log("\n"); // For readability
	return 0;
}
