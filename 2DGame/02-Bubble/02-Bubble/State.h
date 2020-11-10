#ifndef _STATE_INCLUDE_
#define _STATE_INCLUDE_

class State {
   public:
    enum class state { menu, play, dead, credits, instr };

    State();
    ~State();

    void init();
    void changeState();
    void toCredits();
    void toMenu();
    void toPlay();
    void toInst();
    void toDead();
    state getState();
    state getPreviousState();

   private:
    state actual_state;
    state previous_state;
};

#endif  // _STATE_INCLUDE_