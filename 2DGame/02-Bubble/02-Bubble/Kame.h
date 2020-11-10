#ifndef _KAME_INCLUDE_
#define _KAME_INCLUDE_

#include "ShaderProgram.h"
#include "Sprite.h"
#include "TileMap.h"

class Kame {
    struct kameP {
        glm::ivec2 pos;
        int cont;
    };

   public:
    void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
    void initSprite();
    void update(int deltatime);
    void setTileMap(TileMap *tileMap);

    void addKame(const glm::ivec2 &posPlayer);
    void render();
    void deleteAll();

   private:

    void initKame(glm::vec2 pos);

    bool exit;
    glm::vec2 sizeKame, tileMapDispl;
    vector<kameP> kames;
    TileMap *map;
    Sprite *sprite;
    vector<Sprite *> sprites;
    vector<int> posDelete;
    Texture spritesheet;
    ShaderProgram* texProgram;
};

#endif  // _KAME_INCLUDE_
