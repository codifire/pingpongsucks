#include "Button.h"

#define BUTTON_DEBUG 0

void Button::Enter()
{
	m_text.setFont(m_buttonFont);
	m_text.setCharacterSize(40);
	m_text.setFillColor(sf::Color::Red);
	m_text.setString(m_textString);

	auto txtbounds = m_text.getLocalBounds();
	m_text.setOrigin(txtbounds.width / 2, txtbounds.height / 2);
	m_text.setPosition(m_position.x - txtbounds.left, m_position.y - txtbounds.top);

	auto assetDir = m_gmPtr->AssetDir();
	bool success = m_backgroundTexture.loadFromFile(assetDir.append("main_menu").append("light_blue").append("small.png").string());
	assert(success && "background.jpg not loaded");

	m_background.setSize(m_size);
	m_background.setTexture(&m_backgroundTexture);
	m_background.setPosition(m_position);

	auto bounds = m_background.getLocalBounds();
	m_background.setOrigin(bounds.width / 2, bounds.height / 2);

	// Debug
#if defined(BUTTON_DEBUG)
	m_background.setOutlineColor(sf::Color::Red);
	m_background.setOutlineThickness(1.0f);
#endif
}

void Button::Leave()
{
}

void Button::Update(float dt)
{
	if (!m_wndPtr) return; // at max will skip 1 frame, so its ok
	
	sf::Vector2i wndPosition = sf::Mouse::getPosition(*m_wndPtr);

	const bool leftButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	
	auto locallBounds = m_background.getLocalBounds();
	auto globalBounds = m_background.getGlobalBounds();
	if (globalBounds.contains(static_cast<float>(wndPosition.x), static_cast<float>(wndPosition.y)))
	{
		if (!m_leftButtonPressedLastFrame && leftButtonPressed)
		{
			m_background.setFillColor(sf::Color(200, 200, 0, 100));
			m_text.setFillColor(sf::Color(240, 240, 250, 255));
		}
		else if (m_leftButtonPressedLastFrame && !leftButtonPressed)
		{
			// button released
			m_background.setFillColor(sf::Color::White);
			m_text.setFillColor(sf::Color::Red);

			if (m_buttonClickCB)
				m_buttonClickCB(*this);
		}

		ChangeState(State::Expanded);
	}
	else if (m_state == State::Expanded)
	{
		ChangeState(State::Normal);
	}

	m_leftButtonPressedLastFrame = leftButtonPressed;
}

bool Button::ChangeState(State newState)
{
	bool result = false;
	if (m_state != newState)
	{
		auto prevState = m_state;
		m_state = newState;

		if (prevState == State::Normal && newState == State::Expanded)
		{
			m_background.setScale(1.2f, 1.2f);
			m_text.setScale(1.2f, 1.2f);
		}
		else if (prevState == State::Expanded && newState == State::Normal)
		{
			m_background.setScale(1.0f, 1.0f);
			m_text.setScale(1.0f, 1.0f);
		}

		result = false;
	}
	return result;
}

namespace {
	//template<typename T>
	void GetDebugRectangle(sf::Text const& obj, sf::RectangleShape& rect, sf::Color outlineColor = sf::Color::Green, float thickness = 2.0f)
	{
		auto bounds = obj.getLocalBounds();
		auto sz = bounds.getSize();
		rect = sf::RectangleShape(sz);
		auto pos = obj.getPosition();

		auto gbounds = obj.getGlobalBounds();

		rect.setPosition(gbounds.left, gbounds.top);
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(outlineColor);
		rect.setOutlineThickness(thickness);
	}


	void DrawDebugRectangle(sf::RenderWindow& window, sf::Text const& obj)
	{
		sf::RectangleShape debugRect;
		GetDebugRectangle(obj, debugRect);
		window.draw(debugRect);
	}
}

void Button::Render(sf::RenderWindow & window)
{
	m_wndPtr = &window;
	window.draw(m_background);
	window.draw(m_text);

#if defined(BUTTON_DEBUG)
	DrawDebugRectangle(window, m_text);
#endif
}

