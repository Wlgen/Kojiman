#pragma once
#include "Menu.h"
class State
{
public:
	enum class state { menu, play, credits };

	State();
	~State();

	void init();
	void changeState();
	void toCredits();
	state getState();

private:
	state actual_state;
};

