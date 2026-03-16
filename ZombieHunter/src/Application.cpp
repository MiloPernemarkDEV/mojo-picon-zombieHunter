#include "Application.h"
#include <raylib.h>

Application::Application()
    : m_hasEnded(false), m_saveService(&m_hud, m_sceneManager.get_sheep())
{
    m_saveService = SaveService(&m_hud, m_sceneManager.get_sheep());
    m_hud.setSaveService(&m_saveService);
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

    // m_saveService.clear_data(); to start fresh
    m_saveService.load_data();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(WIN_WIDTH, WIN_HEIGHT, "Mojo Picon: ZombieLand");

    SetTargetFPS(500);
}

void Application::end()
{
    m_saveService.save_data();
    m_hasEnded = true;
}

void Application::game_loop()
{
    m_sceneManager.start();
    float deltaTime;
    while (!WindowShouldClose())
    {
        deltaTime = GetFrameTime();

        PollInputEvents();

        m_sceneManager.update(deltaTime);

        for (i32 killed : m_sceneManager.get_recently_killed_zombies())
        {
            m_hud.add_zombie_kill();
        }

        m_hud.update(deltaTime); 

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

       
        m_hud.draw();

        EndDrawing();
    }

    end();
}