#pragma once

#include "Common.h"
#include "ScreenGrid.h"
#include "GameScreenState.h" 
#include "StateMachine.h"

class GameManager
{
public:
	static float ScreenWidth;
	static float ScreenHeight;
	static float ScreenCenterX;
	static float ScreenCenterY;

	GameManager(float screenWidth, float screenHeight);
	~GameManager();

	bool Init();
	void Term();

	void Update(float dt);
	void Render(sf::RenderWindow& window);

	std::filesystem::path FontAssetDir() const { return m_fontAssetDir; }
	std::filesystem::path SpriteAssetDir() const { return m_spriteAssetDir; }
	std::filesystem::path AssetDir() const { return m_assetDir; }

	const ScreenGrid& GetScreenGrid() const { return m_screenGrid; }

	bool IsExitRequested() { return m_exitRequested; }
	void RequestExit() { m_nextState = EGameScreenState::ExitRequested; }

	enum class EGameScreenState
	{
		Invalid = 0,
		Gameplay,
		MainMenu,
		ExitRequested
	};

	void ChangeState(EGameScreenState nextState)
	{
		m_nextState = nextState;
	}
private:
	GameScreen* m_currentScreen;
	StateMachine<GameScreen> m_stateMachine;

	std::filesystem::path m_spriteAssetDir;
	std::filesystem::path m_fontAssetDir;
	std::filesystem::path m_assetDir;

	std::filesystem::path m_projDir;

	// actual game data
	ScreenGrid m_screenGrid;
	sf::RectangleShape m_obj1;
	sf::Texture m_obj1Texture;
	sf::Text m_text;
	sf::Font m_fontRoberto;
	bool m_exitRequested;
	
	EGameScreenState m_nextState;
};

