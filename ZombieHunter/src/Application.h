#pragma once

#include "Defines.h"
#include "SceneManager.h"
#include "HUD.h"
#include "SaveService.h"


constexpr i32 WIN_WIDTH = 1440;
constexpr i32 WIN_HEIGHT = 720;

constexpr i32 TOP_LEFT_CORNER_X = 0;
constexpr i32 TOP_LEFT_CORNER_Y = 0;

#define BACKGROUND_COLOR DARKGREEN

struct Application
{
public:
    Application();
    ~Application();

    void init();
    void gameLoop();
    void end();

private:
    bool m_hasEnded;
    SceneManager m_sceneManager;
    HUD m_hud; 
    SaveService m_saveService;

};