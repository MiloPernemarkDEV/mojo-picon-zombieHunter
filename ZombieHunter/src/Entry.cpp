#include "Application.h"
#include <stdexcept>
#include <iostream>

int main(int argv, char* argc[]) {

	Application app;

	try {
		app.init();
		app.gameLoop();
		app.end();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	

	return EXIT_SUCCESS;
}