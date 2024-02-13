#pragma once
#include "GameScreenState.h"
#include "GameManager.h"
#include "Button.h"

class MainMenuScreen : public GameScreen
{
public:
	MainMenuScreen(GameManager* manager, std::string name);
	~MainMenuScreen();

	void Enter() override;
	void Leave() override;

	void Update(float dt) override;
	void Render(sf::RenderWindow& window) override;
	void ProcessInput() override;

private:
	sf::RectangleShape m_background;
	sf::Texture m_backgroundTexture;

	Button* m_startGameplay = nullptr;
	Button* m_exit = nullptr;

	sf::Font m_fontRoboto;
};
