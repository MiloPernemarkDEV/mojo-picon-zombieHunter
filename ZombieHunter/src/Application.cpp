#include "Application.h"
#include <raylib.h>

Application::Application()
    : m_hasEnded(false), m_saveService(m_hud, m_sceneManager.getSheep())
{
}

Application::~Application()
{
    if (!m_hasEnded)
    {
        end();
    }
}

void Application::init()
{
    m_saveService.load_data();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Mojo Picon: ZombieLand");

    SetTargetFPS(60);
}

void Application::end()
{
    m_saveService.save_data();
    m_hasEnded = true;
}

void Application::gameLoop()
{
    m_sceneManager.start();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        PollInputEvents();

        m_sceneManager.update(deltaTime);

        for (i32 killed : m_sceneManager.getRecentlyKilledZombies())
        {
            m_hud.addZombieKill();
        }

        m_hud.update(); 

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

       
        m_hud.draw();

        EndDrawing();
    }

    end();
}