#pragma once
#include "GameScreenState.h"

template <typename T>
class StateMachine
{
public:
	State<T>* m_currentState = nullptr;

	void ChangeState(State<T>* newState)
	{
		if (newState == m_currentState)
			return;

		if (m_currentState)
			m_currentState->Leave();

		m_currentState = newState;
		
		if(newState)
			newState->Enter();
	}

	void Update(float dt)
	{
		if (m_currentState)
		{
			m_currentState->Update(dt);
		}
	}

	void Render(sf::RenderWindow& window)
	{
		if (m_currentState)
		{
			m_currentState->Render(window);
		}
	}

	void Terminate()
	{
		if (m_currentState)
		{
			m_currentState->Leave();
			m_currentState = nullptr;
		}
	}
};

