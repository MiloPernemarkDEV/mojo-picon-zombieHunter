#include "FrameTool.h"

#include <raylib.h>

#include <iostream>

void FrameTool::update(float deltaTime)
{
	draw_sprite();
}

void FrameTool::draw_sprite()
{
	DrawFPS(0, 0);
}

void FrameTool::start() {

}
