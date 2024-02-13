#include "GameManager.h"
#include "MainMenuScreen.h"
#include "GameplayScreen.h"

float GameManager::ScreenWidth = 0.0f;
float GameManager::ScreenHeight = 0.0f;
float GameManager::ScreenCenterX = 0.0f;
float GameManager::ScreenCenterY = 0.0f;

namespace fs = std::filesystem;


GameManager::GameManager(float screenWidth, float screenHeight)
	:m_screenGrid(screenWidth, screenHeight, 7, 4)
	, m_currentScreen(nullptr)
	, m_exitRequested(false)
	, m_nextState(EGameScreenState::Invalid)
{
	ScreenWidth = screenWidth;
	ScreenHeight = screenHeight;
	ScreenCenterX = ScreenWidth / 2.0f;
	ScreenCenterY = ScreenHeight / 2.0f;

	m_projDir = fs::current_path();

	m_assetDir = m_projDir;
	m_assetDir = m_assetDir.append("assets");
	
	m_spriteAssetDir = m_assetDir;
	m_spriteAssetDir = m_spriteAssetDir.append("sprites");

	m_fontAssetDir = m_assetDir;
	m_fontAssetDir = m_fontAssetDir.append("fonts");
}

GameManager::~GameManager()
{
}

bool GameManager::Init()
{
	//obj1.setFillColor(sf::Color::Green);
	m_obj1.setSize(sf::Vector2f(100.0, 100.0));
	m_obj1.setPosition(ScreenWidth/2, ScreenHeight/2);

	// asset loading
	bool success = false;
	auto fontDir = FontAssetDir().append("Roboto-Light.ttf").string();
	success = m_fontRoberto.loadFromFile(fontDir);
	assert(success && "Roboto-Light.ttf not loaded");

	m_text.setFont(m_fontRoberto);
	m_text.setCharacterSize(24);
	m_text.setFillColor(sf::Color::Red);
	m_text.setString("Hello world");
	m_text.setPosition(ScreenCenterX, ScreenCenterY);

	auto spriteDir = SpriteAssetDir().append("mario.png").string();
	success = m_obj1Texture.loadFromFile(spriteDir);
	assert(success && "mario.png not loaded");

	m_obj1.setTexture(&m_obj1Texture);

	m_nextState = EGameScreenState::Gameplay;
	return true;
}

void GameManager::Term()
{
}

void GameManager::Update(float dt)
{
	if (m_nextState != EGameScreenState::Invalid)
	{
		GameScreen* previousScreen = m_currentScreen;

		switch (m_nextState)
		{
		case EGameScreenState::ExitRequested:
			m_exitRequested = true;
			m_stateMachine.ChangeState(nullptr);
			m_currentScreen = nullptr;
			previousScreen = nullptr;
			break;
		case EGameScreenState::Gameplay:
			m_currentScreen = new GameplayScreen(this, "Gameplay Screen");
			m_stateMachine.ChangeState(m_currentScreen);
			if (previousScreen)
				delete previousScreen;
			break;
		case EGameScreenState::MainMenu:
			m_currentScreen = new MainMenuScreen(this, "Main Menu");
			m_stateMachine.ChangeState(m_currentScreen);
			if (previousScreen)
				delete previousScreen;
			break;
		case EGameScreenState::Invalid:
			break;
		}

		m_nextState = EGameScreenState::Invalid;
	}

	m_stateMachine.Update(dt);
}

void GameManager::Render(sf::RenderWindow& window)
{
	window.clear();

	m_stateMachine.Render(window);

	m_screenGrid.Draw(window);

	window.display();
}