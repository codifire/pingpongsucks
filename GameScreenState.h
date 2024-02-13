#pragma once
#include <string>
#include "State.h"

class GameManager;

class GameScreen : public State<GameScreen>
{
protected:
	GameManager* m_gameManager = nullptr;
	std::string m_name;
public:
	GameScreen(GameManager* manager, std::string name) : m_gameManager(manager), m_name(name) {}
};