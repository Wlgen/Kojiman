#pragma once
class State
{
public:
	enum class state { menu, play, credits, instr };

	State();
	~State();

	void init();
	void changeState();
	void toCredits();
	void toMenu();
	void toPlay();
	void toInst();
	state getState();

private:
	state actual_state;
};

