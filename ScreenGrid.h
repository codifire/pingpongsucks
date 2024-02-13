#pragma once
#include <SFML/Graphics.hpp>
#include "Line.h"
#include <vector>

class ScreenGrid
{
	float W = 0.0, H = 0.0;
	float divisionX = 0.0, divisionY = 0.0;

	std::vector<Line> gridLines;

public:
	ScreenGrid(float W, float H, int divX, int divY);
	~ScreenGrid() = default;

	void Draw(sf::RenderWindow& wnd);

	float OffsetX() const { return divisionX; }
	float OffsetY() const { return divisionY; }
};

