#pragma once
#include "GameManager.h"

class Button
{
	enum class State { Normal = 0, Expanded };

public:
	using ButtonClickCallbackType = std::function<void(const Button&)>;

	Button(GameManager* gm, const sf::Font& font, std::string text, sf::Vector2f position, sf::Vector2f size)
		: m_wndPtr(nullptr)
		, m_gmPtr(gm)
		, m_buttonFont(font)
		, m_textString(text)
		, m_position(position)
		, m_size(size)
		, m_state(State::Normal)
		, m_leftButtonPressedLastFrame(false)
		, m_buttonClickCB(nullptr)
	{}

	void Enter();
	void Leave();
	void Update(float dt);
	void Render(sf::RenderWindow& window);
	void ProcessInput() {}

	void SetOnButtonClick(ButtonClickCallbackType callback) { m_buttonClickCB = callback; }

private:
	// weak pointers
	sf::RenderWindow* m_wndPtr;
	GameManager* m_gmPtr;

	const sf::Font& m_buttonFont;
	std::string m_textString;
	sf::Text m_text;
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	State m_state;
	bool m_leftButtonPressedLastFrame;
	ButtonClickCallbackType m_buttonClickCB;

	bool ChangeState(State newState);

	sf::RectangleShape m_background;
	sf::Texture m_backgroundTexture;
};
