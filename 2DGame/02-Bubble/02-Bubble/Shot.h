#ifndef _SHOT_INCLUDE_
#define _SHOT_INCLUDE_

#include "Sprite.h"
#include "TileMap.h"
#include "ShaderProgram.h"

class Shot {
   public:

	   void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
       void initSprite();
	   void update(int deltatime);
       void setTileMap(TileMap *tileMap);

	   void addShot(const glm::ivec2 &posPlayer);
       void render();
	   void deleteAll();

       bool getShotAlarm();
	
   private:
        bool exit, shotAlarm;
        glm::vec2 sizeShot, tileMapDispl;
		vector<glm::ivec2> shots;
		TileMap *map;
        Sprite *sprite;
        vector<Sprite*>sprites;
        Texture spritesheet;
        ShaderProgram texProgram;
};

#endif  // _SHOT_INCLUDE_
