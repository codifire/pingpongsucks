#pragma once
#include "GameManager.h"
#include "GameScreenState.h"

class GameplayScreen : public  GameScreen
{
public:
	GameplayScreen(GameManager* manager, std::string name);
	~GameplayScreen();
	void Enter() override;
	void Leave() override;

	void Update(float dt) override;
	void Render(sf::RenderWindow& window) override;
	void ProcessInput() override;

private:
	sf::RectangleShape m_background;
	sf::Texture m_backgroundTexture;

	// Scene Objects
	// paddle
	sf::Texture m_paddleTex;
	sf::RectangleShape m_paddle;

	// ball
	sf::Texture m_ballTex;
	sf::CircleShape m_ball;
	
	// tiles
	static const unsigned NumCols = 11;
	static const unsigned NumRows = 4;
	static const unsigned NumTiles = NumRows * NumCols;
	
	sf::Texture m_tileTex;
	sf::RectangleShape m_tiles[NumTiles];

	// walls
	enum Wall {
		Down = 0, Up, Left, Right, Num, None
	};

	static const unsigned int NumWalls = 4;
	sf::Texture m_wallTex;
	sf::RectangleShape m_walls[NumWalls];

	// movement
	sf::Vector2f m_ballVelocity;
	sf::Vector2f m_paddleVelocity;

	// ball
	static const float MaxSpeedBall;

	// paddle
	static const float AccelerationFactorPaddle;
	static const float MaxSpeedPaddle;
	static const float FrictionFactorPaddle;
	static const float ZeroOutVelocityThreshold;

	// game rules
	int m_hitCount;
	static const int m_hitTolerenceThreshold = 5;

private:
	template<typename T>
	void LoadAsset(std::filesystem::path path, std::string name, T& asset);

	// GetInputDirection()
	//  0: no input, don't move
	//  1: move right
	// -1: move left
	float GetInputDirection();

	// movement Controllers
	void UpdatePaddleMovement(float dt);
	void UpdatePaddleWallCollision(float dt);
	void UpdateBallPaddleCollision(float dt);

	void UpdateBallMovement(float dt);
	void UpdateBallWallCollision(float dt);

	// debug
	void DebugDraw(sf::RenderWindow& window);
	sf::Text m_text;
	sf::Font m_font;
};

