#include "FrameTool.h"

#include <raylib.h>

#include <iostream>

void FrameTool::update(float deltaTime)
{
	drawSprite();
}

void FrameTool::drawSprite()
{
	DrawFPS(0, 0);
}

void FrameTool::start() {

}
