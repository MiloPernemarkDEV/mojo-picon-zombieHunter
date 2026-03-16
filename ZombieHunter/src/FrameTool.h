#pragma once

#include "SauceMaker.h"

struct FrameTool : public SauceMaker  {
public:
	void update(float deltaTime) override;
	void draw_sprite() override;
	void start() override;
private: 
};