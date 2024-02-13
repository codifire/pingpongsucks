#pragma once

namespace sf {
	class RenderWindow;
}


template<typename T>
class State
{
public:
	virtual ~State() = default;
	virtual void Enter() = 0;
	virtual void Leave() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;
	virtual void ProcessInput() = 0;
};