#include "State.h"

State::State() { actual_state = state::menu; }
State::~State() {}

void State::init() { previous_state = actual_state = state::menu; }

void State::changeState() {
    previous_state = actual_state;
    switch (actual_state) {
        case state::menu:
            actual_state = state::play;
            break;
        case state::play:
            actual_state = state::credits;
            break;
        case state::credits:
            actual_state = state::menu;
            break;
        default:
            break;
    }
}

void State::toCredits() { 
    previous_state = actual_state;
    actual_state = state::credits;
}

void State::toMenu() { 
    previous_state = actual_state;
    actual_state = state::menu; 
}

void State::toPlay() { 
    previous_state = actual_state;
    actual_state = state::play;
}

void State::toInst() { 
    previous_state = actual_state;
    actual_state = state::instr;
}
void State::toDead() {
    previous_state = actual_state;
    actual_state = state::dead;
}

State::state State::getPreviousState() {
    return previous_state;
}

State::state State::getState() { return actual_state; }