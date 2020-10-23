#include "State.h"

State::State() { actual_state = state::menu; }
State::~State() {}

void State::init() { actual_state = state::menu; }

void State::changeState() {
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

void State::toCredits() { actual_state = state::credits; }

void State::toMenu() { actual_state = state::menu; }

void State::toPlay() { actual_state = state::play; }

void State::toInst() { actual_state = state::instr; }

State::state State::getState() { return actual_state; }