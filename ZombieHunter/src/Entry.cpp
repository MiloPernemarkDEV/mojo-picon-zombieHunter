#include "Application.h"
#include <stdexcept>
#include <iostream>

int main(int argv, char* argc[]) {

	Application app;

	try {
		app.init();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	app.gameLoop();

	app.end();

	return EXIT_SUCCESS;
}