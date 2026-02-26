#include "Application.h"
#include <raylib.h>

Application::Application() 
	: m_hasEnded{false}
{
	
}

Application::~Application() 
{
	if (!m_hasEnded) 
	{
		end();
	}
}

void Application::init() {
	SetTargetFPS(60);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(WIN_WIDTH, WIN_HEIGHT, "Mojo Picon: ZombieLand");
}

void Application::end() {
	
	m_hasEnded = true;
}


void Application::gameLoop() {

	m_sceneManager.start();

	while (!WindowShouldClose()) 
	{
		PollInputEvents();
		BeginDrawing();
		ClearBackground(BACKGROUND_COLOR);

		m_sceneManager.update();
	

		EndDrawing();
	}

	CloseWindow();
}

