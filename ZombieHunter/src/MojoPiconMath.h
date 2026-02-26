#pragma once

#include <raylib.h>

struct MojoPiconMath {
public:
	inline constexpr float squaredDistance(const Vector2& v, const Vector2& other) {
		float distanceX = v.x - other.x;
		float distanceY = v.y - other.y;
		return distanceX * distanceX + distanceY * distanceY;
	}
private:

};