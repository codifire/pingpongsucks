#include "ScreenGrid.h"


ScreenGrid::ScreenGrid(float Width, float Height, int divX, int divY)
	: divisionX(0.0f)
	, divisionY(0.0f)
{
	int size = divY + divX + 1;
	gridLines.reserve(size);

	const float IncrementSizeX = Width / divX;
	const float IncrementSizeY = Height / divY;
	const float LineWidth = 1;

	divisionX = IncrementSizeX;
	divisionY = IncrementSizeY;

	// vertical
	for (int i = 0; i < divX; ++i)
	{	
		gridLines.emplace_back(Line(i * IncrementSizeX, 0, i * IncrementSizeX, Height, LineWidth));
	}

	// horizontal
	for (int i = 0; i < divY; ++i)
	{
		gridLines.emplace_back(Line(0, i * IncrementSizeY, Width, i * IncrementSizeY, LineWidth));
	}

	gridLines.emplace_back(Line(0, Height / 2, Width, Height / 2, 1, sf::Color::Yellow));
}

void ScreenGrid::Draw(sf::RenderWindow& window)
{
	for (auto& line : gridLines)
		line.Draw(window);
}