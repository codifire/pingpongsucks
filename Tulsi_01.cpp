// Tulsi_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Common.h"

#include <SFML/Graphics.hpp>

#include "Utils.h"
#include "Line.h"
#include "GameManager.h"

#define SCREEN_WIDTH ((float)window.getSize().x)
#define SCREEN_HEIGHT ((float)window.getSize().y)

using namespace sf;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Default);
	window.setFramerateLimit(60);
	//window.setVerticalSyncEnabled(true);

	Line diagonal1(SCREEN_WIDTH * (1.0f / 2.0f), SCREEN_HEIGHT * (1.0f / 3.0f), SCREEN_WIDTH * (3.0f / 4.0f), SCREEN_HEIGHT * (2.0f / 3.0f), 1 , sf::Color::Magenta);
	Line diagonal2(SCREEN_WIDTH * (3.0f / 4.0f), SCREEN_HEIGHT * (1.0f / 3.0f), SCREEN_WIDTH * (1.0f / 2.0f), SCREEN_HEIGHT * (2.0f / 3.0f), 1 , sf::Color::Magenta);

	sf::RectangleShape rect1(sf::Vector2f(200, 200));
	rect1.setFillColor(sf::Color::Blue);
	//rect1.setOrigin(100, 100);
	rect1.setPosition(SCREEN_WIDTH * (3.0f / 4.0f), SCREEN_HEIGHT * (1.0f / 4.0f));

	sf::RectangleShape rect2(sf::Vector2f(200, 200));
	rect2.setFillColor(sf::Color::Green);
	//rect2.setOrigin(75, 100);
	rect2.setPosition(400, 400 );

	sf::Clock clock;
	
	float rotate = 0;
	float deltaTime = 0.0;
	int frameCount = 0;

	using namespace std::chrono;
	auto secondStartTime = high_resolution_clock::now();

	GameManager gameManager(SCREEN_WIDTH, SCREEN_HEIGHT);
	bool initSuccess = gameManager.Init();

	if (!initSuccess)
		window.close();

	using Clock = std::chrono::high_resolution_clock;
	using milliseconds = std::chrono::milliseconds;
	using seconds = std::chrono::seconds;

	auto lastTime = Clock::now();

	while (window.isOpen())
	{
		auto currTime = Clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - lastTime).count() / 1000.0f;
		lastTime = currTime;

		// while debugging we may hit a break point for too long in that case delta may be a few seconds or minutes or so on..
		if (deltaTime > 0.050) deltaTime = 0.050f;

		// Process Input
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (gameManager.IsExitRequested())
				window.close();

		//Update
		gameManager.Update(deltaTime);

		//Render
		gameManager.Render(window);
	}

	gameManager.Term();
	
	return 0;
}