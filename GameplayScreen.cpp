#include "GameplayScreen.h"

// constants for motion
const float GameplayScreen::MaxSpeedBall = 900.0f;

// paddle motion
const float GameplayScreen::AccelerationFactorPaddle = 1900.0f;	// increase for responsiveness
const float GameplayScreen::MaxSpeedPaddle = 1500.f;			// adjust for gameplay feel
const float GameplayScreen::FrictionFactorPaddle = 0.92f;		// use a multiplicative factor for simplicity
const float GameplayScreen::ZeroOutVelocityThreshold = 0.9f;	// adjust threshold for gameplay feel

namespace {
	template<typename T>
	void GetDebugRectangle(T const& obj, sf::RectangleShape& rect, sf::Color outlineColor = sf::Color::Green, float thickness = 2.0f)
	{
		auto bounds = obj.getGlobalBounds();
		auto sz = bounds.getSize();
		rect = sf::RectangleShape(sz);
		auto pos = obj.getPosition();

		rect.setPosition(bounds.left, bounds.top);
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(outlineColor);
		rect.setOutlineThickness(thickness);
	}

	template<typename T>
	void DrawDebugRectangle(sf::RenderWindow& window, T const& obj)
	{
		sf::RectangleShape debugRect;
		GetDebugRectangle(obj, debugRect);
		window.draw(debugRect);
	}
}

GameplayScreen::GameplayScreen(GameManager* manager, std::string name)
	: GameScreen(manager, name)
	, m_hitCount(0)
{
}

GameplayScreen::~GameplayScreen()
{
}

void GameplayScreen::Enter()
{
	auto& GM = *m_gameManager;
	bool success = false;

	LoadAsset(GM.AssetDir().append("level1"), "background.png", m_backgroundTexture);
	m_background.setTexture(&m_backgroundTexture);

	m_background.setSize(sf::Vector2f(m_gameManager->ScreenWidth, m_gameManager->ScreenHeight));
	m_background.setPosition(0, 0);
	m_background.setTextureRect(sf::IntRect{75, 130, static_cast<int>(m_gameManager->ScreenWidth), static_cast<int>(m_gameManager->ScreenHeight) });

	LoadAsset(GM.AssetDir().append("level1"), "paddle.png", m_paddleTex);
	LoadAsset(GM.AssetDir().append("level1"), "ball.png", m_ballTex);
	LoadAsset(GM.AssetDir().append("level1"), "tile.png", m_tileTex);
	LoadAsset(GM.AssetDir().append("level1"), "wall-3.png", m_wallTex);
	
	LoadAsset(GM.AssetDir().append("fonts"), "CourierPrime-Regular.ttf", m_font);
	m_text.setFont(m_font);
	m_text.setCharacterSize(35);
	m_text.setPosition(m_gameManager->ScreenCenterX, m_gameManager->ScreenCenterY);
	m_text.setFillColor(sf::Color::Blue);

	// scene layout
	const float TileWidth = 50.0f;
	const float TileHeight = 20.0f;
	const float TileOffsetX = 100.0f;
	const float TileOffsetY = 50.0f;

	// tiles
	for (unsigned i = 0; i < NumRows; ++i)
	{
		for (unsigned j = 0; j < NumCols; ++j)
		{
			int index = i * NumCols + j;
			auto& tile = m_tiles[index];
			tile.setTexture(&m_tileTex);
			tile.setSize(sf::Vector2f(TileWidth, TileHeight));
			tile.setOrigin(tile.getSize().x / 2.0f, tile.getSize().y / 2.0f);

			tile.setPosition((j+1) * TileOffsetX, (i+1) * TileOffsetY);
		}
	}

	// walls
	const float WallHeight = 56 / 4;
	const float WallOffset = WallHeight;

	m_walls[Up].setSize(sf::Vector2f(GM.ScreenWidth - 2.0f * WallOffset, WallHeight));
	m_walls[Up].setPosition(0 + WallOffset, 0 + WallOffset);
	m_walls[Up].setTexture(&m_wallTex);
	m_walls[Up].setOutlineColor(sf::Color(0, 160, 230, 255));
	m_walls[Up].setOutlineThickness(3);

	m_walls[Left].setSize(sf::Vector2f(GM.ScreenHeight - 2.0f * WallOffset, WallHeight));
	m_walls[Left].setOrigin(0, m_walls[Left].getSize().y);
	m_walls[Left].setRotation(90);
	m_walls[Left].setPosition(0 + WallOffset, WallOffset);
	m_walls[Left].setTexture(&m_wallTex);
	m_walls[Left].setOutlineColor(sf::Color(0, 160, 230, 255));
	m_walls[Left].setOutlineThickness(3);

	m_walls[Right].setSize(sf::Vector2f(GM.ScreenHeight - 2.0f * WallOffset, WallHeight));
	m_walls[Right].setRotation(-90);
	m_walls[Right].setOrigin(m_walls[Right].getSize().x, m_walls[Right].getSize().y);
	m_walls[Right].setPosition(GM.ScreenWidth - WallOffset, WallOffset);
	m_walls[Right].setTexture(&m_wallTex);
	m_walls[Right].setOutlineColor(sf::Color(0, 160, 230, 255));
	m_walls[Right].setOutlineThickness(3);

	m_walls[Down].setSize(sf::Vector2f(GM.ScreenWidth - 2.0f * WallOffset, WallHeight));
	m_walls[Down].setPosition(0 + WallOffset, GM.ScreenHeight - WallHeight - WallOffset);
	m_walls[Down].setTexture(&m_wallTex);
	m_walls[Down].setOutlineColor(sf::Color(0, 160, 230, 255));
	m_walls[Down].setOutlineThickness(3);

	// paddle
	float paddleWidth = 120;
	float paddleHeight = 25;
	float outlineThickness = 4;
	m_paddle.setSize(sf::Vector2f{ paddleWidth, paddleHeight });
	m_paddle.setPosition(sf::Vector2f{ GM.ScreenWidth / 2, GM.ScreenHeight - paddleHeight - m_walls->getSize().y - outlineThickness });
	m_paddle.setOrigin(m_paddle.getSize().x / 2, m_paddle.getSize().y / 2);
	m_paddle.setTexture(&m_paddleTex);

	// ball
	float ballRadius = 10;
	m_ball.setRadius(ballRadius);

	auto ballBounds = m_ball.getLocalBounds();
	m_ball.setOrigin(ballBounds.getSize().x / 2, ballBounds.getSize().y / 2);
	m_ball.setPosition(sf::Vector2f{ GM.ScreenWidth/2, m_paddle.getPosition().y - ballRadius - m_paddle.getSize().y/2 });
	m_ball.setTexture(&m_ballTex);
}

void GameplayScreen::Leave()
{
}

void GameplayScreen::Update(float dt)
{	
	m_text.setString(""); // clear debug

	UpdateBallMovement(dt);
	UpdateBallWallCollision(dt);
	UpdateBallPaddleCollision(dt);

	UpdatePaddleMovement(dt);
	UpdatePaddleWallCollision(dt);

	// implement ball slowdown
	// check collision with tiles
}

void GameplayScreen::Render(sf::RenderWindow& window)
{
	window.draw(m_background);

	for (unsigned i = 0; i < NumTiles; ++i)
		window.draw(m_tiles[i]);

	window.draw(m_paddle);
	window.draw(m_ball);

	for (auto wall : m_walls)
		window.draw(wall);
	
	float lineScaleFactor = magnitude(m_paddleVelocity) / MaxSpeedPaddle;
	float x2 = Sign(m_paddleVelocity.x) * m_gameManager->ScreenWidth / 2.0f;
	x2 *= lineScaleFactor;

	Line velocity(m_gameManager->ScreenCenterX, m_gameManager->ScreenCenterY, m_gameManager->ScreenCenterX + x2, m_gameManager->ScreenCenterY, 5.0f, sf::Color::Blue);
	velocity.Draw(window);
	DebugDraw(window);	
}

void GameplayScreen::ProcessInput()
{
}

template<typename T>
void GameplayScreen::LoadAsset(std::filesystem::path path, std::string name, T& asset)
{
	bool success = asset.loadFromFile(path.append(name).string());
	assert(success && "background.png not loaded");
}

void GameplayScreen::DebugDraw(sf::RenderWindow& window)
{
	DrawDebugRectangle(window, m_paddle);
	DrawDebugRectangle(window, m_walls[Right]);
	DrawDebugRectangle(window, m_walls[Left]);

	auto paddleGlobalBounds = m_paddle.getGlobalBounds();
	auto leftWallGlobalBounds = m_walls[Left].getGlobalBounds();
	auto rightWallGlobalBounds = m_walls[Right].getGlobalBounds();

	Line toLeftWall(leftWallGlobalBounds.left + leftWallGlobalBounds.width, leftWallGlobalBounds.top, paddleGlobalBounds.left, paddleGlobalBounds.top, 2.0f, sf::Color::Blue);
	Line toRightWall(rightWallGlobalBounds.left, rightWallGlobalBounds.top, paddleGlobalBounds.left + paddleGlobalBounds.width, paddleGlobalBounds.top, 2.0f, sf::Color::Blue);

	float paddleCenterX = paddleGlobalBounds.left + paddleGlobalBounds.width / 2.0f;

	Line paddleCenter(paddleCenterX, paddleGlobalBounds.top, paddleCenterX, paddleGlobalBounds.top - 50.0f, 2.0f, sf::Color::Blue);

	toLeftWall.Draw(window);
	toRightWall.Draw(window);
	paddleCenter.Draw(window);
	window.draw(m_text);
}

float GameplayScreen::GetInputDirection()
{
	float dirInput = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		dirInput = -1.0f;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		dirInput = 1.0f;

	return dirInput;
}

void GameplayScreen::UpdatePaddleMovement(float dt)
{
	float dirInput = GetInputDirection();

	// decrease velocity to improve the feel when changing directions suddenly at higher velocities
	if (dirInput != 0.0f && fabs(m_paddleVelocity.x) > MaxSpeedPaddle * 0.2f && Sign(dirInput) != Sign(m_paddleVelocity.x)) {
		m_paddleVelocity.x *= 0.85f;
	}

	// apply acceleration
	sf::Vector2f acceleration(dirInput * AccelerationFactorPaddle * dt, 0.0f);
	m_paddleVelocity += acceleration;
	m_paddleVelocity = truncate(m_paddleVelocity, MaxSpeedPaddle);

	// apply friction when there's no input to gradually slow down the paddle
	if (dirInput == 0.0f) {
		m_paddleVelocity.x *= FrictionFactorPaddle;
	}

	// zero out small velocities to prevent endless sliding
	if (NearZero(magnitude(m_paddleVelocity), ZeroOutVelocityThreshold)) {
		m_paddleVelocity = sf::Vector2f(0.0f, 0.0f);
	}

	m_paddle.move(m_paddleVelocity * dt);
}

void GameplayScreen::UpdatePaddleWallCollision(float dt)
{
	const float SpeedX = m_paddleVelocity.x;
	
	Wall collidingWall = None;
	auto paddleGlobalBounds = m_paddle.getGlobalBounds();

	if (SpeedX > 0.0f && m_walls[Right].getGlobalBounds().intersects(paddleGlobalBounds)) {
		collidingWall = Right;
	}
	else if (SpeedX < 0.0f && m_walls[Left].getGlobalBounds().intersects(paddleGlobalBounds)) {
		collidingWall = Left;
	}
	else {
		if (m_walls[Left].getGlobalBounds().intersects(paddleGlobalBounds)) {
			collidingWall = Left;
		}
		else if (m_walls[Right].getGlobalBounds().intersects(paddleGlobalBounds)) {
			collidingWall = Right;
		}
	}
	
	std::string str = "SpeedX: ";
	NumberToString<float>(SpeedX, str);
	if (collidingWall != None)
	{
		str += "\n";
		str += collidingWall == Left ? "||<--- Collision" : "Collision --->||";
	}

	float dir = GetInputDirection();

	if (collidingWall == Left) {
		auto wallBounds = m_walls[Left].getGlobalBounds();
		float wallRightEdge = wallBounds.left + wallBounds.width;
		float diff = wallRightEdge - paddleGlobalBounds.left;
		
		m_paddle.move(sf::Vector2f(diff, 0.0f));
		
		if (SpeedX < 0.0f && dir < 0.0) { // moving left, left input pressed
			m_paddleVelocity.x *= -0.5; //m_paddleVelocity = sf::Vector2f(0.0f, 0.0f);
		}
		else if (SpeedX < 0.0f && dir >= 0.0f) {
			m_paddleVelocity.x *= -1;
		}
	}
	else if (collidingWall == Right) {
		auto wallBounds = m_walls[Right].getGlobalBounds();
		float diff = wallBounds.left - (paddleGlobalBounds.left + paddleGlobalBounds.width);
		
		m_paddle.move(sf::Vector2f(diff, 0.0f));

		if (SpeedX > 0.0f && dir > 0.0) { // moving right, right input pressed
			m_paddleVelocity.x *= -0.5; //m_paddleVelocity = sf::Vector2f(0.0f, 0.0f);
		}
		else if (SpeedX > 0.0f && dir <= 0.0f)
		{
			sf::Vector2f vec = sf::Vector2f(-1 * SpeedX, 0.0f);
			m_paddleVelocity.x *= -1;
		}
	}

	auto textStr = m_text.getString();
	m_text.setString(textStr + "\n" + str);
}

void GameplayScreen::UpdateBallPaddleCollision(float dt)
{
	auto paddleGlobalBounds = m_paddle.getGlobalBounds();
	auto ballBounds = m_ball.getGlobalBounds();

	if (ballBounds.intersects(paddleGlobalBounds)) {
		float diff = paddleGlobalBounds.top - (ballBounds.top + paddleGlobalBounds.height);
		m_ball.move(sf::Vector2f(0.0f, diff));

		auto ballCenterX = ballBounds.left + ballBounds.width / 2.0f;
		auto paddleHalfWidth = paddleGlobalBounds.width / 2.0f;
		auto paddleCenterX = paddleGlobalBounds.left + paddleHalfWidth;
		
		// -1 represents the far left of the paddle
		//  0 represents the exact center of the paddle
		//  1 represents the far right of the paddle
		float hitPosition = (ballCenterX - paddleCenterX) / paddleHalfWidth;

		hitPosition = Clamp(hitPosition, -1.0f, 1.0f);

		const float ballSpeed = std::hypot(m_ballVelocity.x, m_ballVelocity.y);
		const bool ballDirRight = m_ballVelocity.x > 0.0;
		const bool ballDirLeft = m_ballVelocity.x < 0.0;
		
		// adjust horizontal velocity, can normalize and then scale the vector??
		const float MaxHorizontalVelocityChange = 15000.0f;
		m_ballVelocity.x += hitPosition * MaxHorizontalVelocityChange * dt;

		if(ballDirRight && ballCenterX < paddleCenterX || ballDirLeft && ballCenterX > paddleCenterX) {
			m_ballVelocity = normalize(m_ballVelocity);
			m_ballVelocity *= ballSpeed;
		}

		// Optionally increase speed for harder hits
		const float speedIncreaseFactor = 200;
		float incrementFactor = (1.0f - (ballSpeed / (MaxSpeedBall/2.0f)));
		incrementFactor *= ballSpeed;
		
		auto acceleration = normalize(m_ballVelocity) * incrementFactor * dt * speedIncreaseFactor;;
		//m_ballVelocity += acceleration;

		m_ballVelocity.y = -std::abs(m_ballVelocity.y); //m_ballVelocity.y *= -1;
	}

	// Optionally increase speed for harder hits
	//#1
	// Increase ball speed slightly to increase game difficulty
	// float speedIncrease = std::min(MaxBallSpeed - std::hypot(ballVelocity.x, ballVelocity.y), BallAcceleration);
	// ballVelocity += normalize(ballVelocity) * speedIncrease;

	//#2
	// float speedIncreaseFactor = 1.0 + std::abs(hitPosition) * additionalSpeedPercentage;
	// m_ballVelocity *= speedIncreaseFactor;

	//#3
	// ballSpeed = MaxBallSpeed * (1.0f - exp(-hitCount / someFactor));
	// if (missedBall) {
	// ballSpeed = InitialBallSpeed; // Reset to a predefined initial speed
	// hitCount = 0; // Reset hit counter or any mechanism tracking speed increases

	//#4
	// Apply a diminishing effect
	// increment *= (1.0f - (ballSpeed / MaxBallSpeed));
	// ballSpeed += increment;
}

void GameplayScreen::UpdateBallMovement(float dt)
{
	if (m_ballVelocity.x == 0.0f && m_ballVelocity.y == 0.0f)
	{
		m_ballVelocity = sf::Vector2f(0.0f, -1.0f * 5.0f * MaxSpeedBall * dt);
	}

	m_ballVelocity = truncate(m_ballVelocity, MaxSpeedBall);

	m_ball.move(m_ballVelocity * dt);

	// debug
	std::string ballVelocityStr;
	float ballSpeed = magnitude(m_ballVelocity);
	NumberToString<float>(ballSpeed, ballVelocityStr);

	auto textStr = m_text.getString();
	m_text.setString(ballVelocityStr + "\n" + textStr);
}

void GameplayScreen::UpdateBallWallCollision(float dt)
{
	auto leftWallBounds = m_walls[Left].getGlobalBounds();
	auto rightWallBounds = m_walls[Right].getGlobalBounds();
	auto topWallBounds = m_walls[Up].getGlobalBounds();
	auto bottomWallBounds = m_walls[Down].getGlobalBounds();
	auto ballBounds = m_ball.getGlobalBounds();

	float diff = 0.0f;

	if(leftWallBounds.intersects(ballBounds)) {
		m_ballVelocity.x *= -1;
		diff = fabs((leftWallBounds.left + leftWallBounds.width) - ballBounds.left);
	}
	else if (rightWallBounds.intersects(ballBounds)) {
		m_ballVelocity.x *= -1;
		diff = fabs(rightWallBounds.left - (ballBounds.left + ballBounds.width));
	}
	else if (topWallBounds.intersects(ballBounds)) {
		m_ballVelocity.y *= -1;
		diff = fabs((topWallBounds.top + topWallBounds.height) - ballBounds.top);
	}
	else if (bottomWallBounds.intersects(ballBounds)) {
		m_ballVelocity.y *= -1;
		diff = fabs(bottomWallBounds.top - (ballBounds.top+ ballBounds.height));

		m_hitCount++;
		m_hitCount = Min(m_hitCount, m_hitTolerenceThreshold);

		float factor = static_cast<float>(m_hitCount) / static_cast<float>(m_hitTolerenceThreshold);
		sf::Color color;
		color.r = 255.0f;
		color.g = (255.0f - factor * 255.0f);
		color.b = (255.0f - factor * 255.0f);
		color.a = 255;// (255.0f - factor * 255.0f);
		m_walls[Down].setFillColor(color);
		//m_walls[Down].set
	}

	m_paddle.move(sf::Vector2f(diff, 0.0f));
}
