#pragma once

#include "SauceMaker.h"

struct FrameTool : public SauceMaker  {
public:
	void update(float deltaTime) override;
	void drawSprite() override;
	void start() override;
private: 
};