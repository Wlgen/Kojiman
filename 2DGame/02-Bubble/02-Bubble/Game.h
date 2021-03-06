#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Credits.h"
#include "Menu.h"
#include "Scene.h"
#include "State.h"


#include <irrKlang.h>
#include <ik_ISoundEngine.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Game is a singleton (a class with a single instance) that represents our
// whole application

class Game {
   public:
    Game() {}

    static Game &instance() {
        static Game G;

        return G;
    }

    void init();
    bool update(int deltaTime);
    void render();

    // Input callback methods
    void keyPressed(int key);
    void keyReleased(int key);
    void specialKeyPressed(int key);
    void specialKeyReleased(int key);
    void mouseMove(int x, int y);
    void mousePress(int button);
    void mouseRelease(int button);

    bool getKey(int key) const;
    bool getSpecialKey(int key) const;

    void restart(bool death);
    void pause(bool player);
    void toggleRend();
    void getSceneInTransitionUp();
    void getSceneInTransitionDown();
    void changeLevel(int level);

    void loopMusic(char* fileName);
    void stopMusic();
    void playSound(char* fileName);
    bool isGodMode();

    void iWin();

   private:
    bool bPlay, first, win;   // Continue to play game?
    Scene scene;  // Scene to render
    Menu menu;
    bool keys[256], specialKeys[256], godMode;  // Store key states so that
                                       // we can have access at any time
    State gState;

    float deadTime;

    irrklang::ISoundEngine* engine;  // para la musica del juego
    irrklang::ISound* music;
};

#endif  // _GAME_INCLUDE
