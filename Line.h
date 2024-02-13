#pragma once
#include <SFML/Graphics.hpp>
#include "Utils.h"

class Line
{
	sf::RectangleShape m_rect;

public:
	Line(float startX, float startY, float endX, float endY, float lineWidth = 1, sf::Color color = sf::Color::Cyan)
	{
		SetColor(color);
		SetBeginEnd(startX, startY, endX, endY, lineWidth);
	}

	void SetBeginEnd(float startX, float startY, float endX, float endY, float lineWidth = 1)
	{
		float width = (endX - startX);
		float height = (endY - startY);
		float diagonal = sqrt(width * width + height * height);

		//float diagonal = std::hypotf(width, height);

		m_rect.setSize(sf::Vector2f(diagonal, lineWidth));
		m_rect.setOrigin(diagonal / 2, lineWidth / 2);
		m_rect.setPosition(startX + (width / 2), startY + (height / 2));

		float angle = atan2((endY - startY), endX - startX);
		float degrees = RadToDeg(angle);
		m_rect.setRotation(degrees);
	}

	void SetColor(sf::Color color)
	{
		m_rect.setFillColor(color);
	}

	void Draw(sf::RenderWindow& wnd)
	{
		wnd.draw(m_rect);
	}
};