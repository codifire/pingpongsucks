#include "MainMenuScreen.h"

MainMenuScreen::MainMenuScreen(GameManager* manager, std::string name)
	: GameScreen(manager, name)
{
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::Enter()
{
	auto& GM = *m_gameManager;
	
	bool success = false;
	
	auto fontDir = GM.FontAssetDir();
	success = m_fontRoboto.loadFromFile(fontDir.append("Roboto-Regular.ttf").string());
	assert(success && "Roboto-Light.ttf not loaded");

	float yOffset = GM.GetScreenGrid().OffsetY() / 2.0f;
	m_startGameplay =  new Button(m_gameManager, m_fontRoboto, "New Game", sf::Vector2f(GM.ScreenCenterX, GM.ScreenCenterY), sf::Vector2f(300.0, 80.0));
	m_exit = new Button(m_gameManager, m_fontRoboto, "Exit", sf::Vector2f(GM.ScreenCenterX, GM.ScreenCenterY + yOffset), sf::Vector2f(300.0, 80.0));

	m_startGameplay->Enter();
	m_exit->Enter();

	m_startGameplay->SetOnButtonClick([this](const Button&) {
		std::cout << "startGameplay pressed" << std::endl;
		m_gameManager->ChangeState(GameManager::EGameScreenState::Gameplay);
	});
	
	m_exit->SetOnButtonClick([this](const Button&) {
		std::cout << "m_exit pressed" << std::endl;
		m_gameManager->RequestExit();
	});

	auto assetDir = GM.AssetDir();
	success = m_backgroundTexture.loadFromFile(assetDir.append("main_menu").append("background.jpg").string());
	assert(success && "background.jpg not loaded");

	m_background.setSize(sf::Vector2f(GM.ScreenWidth, GM.ScreenHeight));

	m_background.setTexture(&m_backgroundTexture);
	m_background.setTextureRect(sf::IntRect{0, 50, static_cast<int>(GM.ScreenWidth), static_cast<int>(GM.ScreenHeight)});
	m_background.setPosition(0, 0);
}

void MainMenuScreen::Leave()
{
	m_startGameplay->Leave();
	m_exit->Leave();

	delete m_startGameplay;
	delete m_exit;
	
	m_startGameplay = nullptr;
	m_exit = nullptr;
}

void MainMenuScreen::Update(float dt)
{
	m_startGameplay->Update(dt);
	m_exit->Update(dt);
}

void MainMenuScreen::Render(sf::RenderWindow& window)
{
	window.draw(m_background);
	m_startGameplay->Render(window);
	m_exit->Render(window);
}

void MainMenuScreen::ProcessInput()
{
}
