
#ifndef _PLAY_HPP_
#define _PLAY_HPP_

#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <sstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


enum CurrWall { LEFT, RIGHT };


class UI {

public:
    static void RenderTextBox( SDL_Renderer *renderer, SDL_Rect rect, const char* content, TTF_Font* gFont, SDL_Color bgColor, SDL_Color fontColor );

};


class Entity {

protected:
    CurrWall currWall;
    SDL_Rect hitBox;

    float posX, posY;
    float width, height;

    float coolTime;

public:
    Entity();
    ~Entity();

    virtual void Render( SDL_Renderer *renderer );
    void reduceCoolTime( float elapsed );


    CurrWall GetCurrWall();
    float GetPosX();
    float GetPosY();
    float GetWidth();
    float GetHeight();
};

class Player : public Entity {

public:
    Player();
    ~Player();

    void Jump();
};

class Obstacle : public Entity {

private:
    float fallRate;

public:
    Obstacle( CurrWall currWall, float fallRate );
    ~Obstacle();

    void Fall( float elapsed );
};

class Walls {

private:
    SDL_Rect r1, r2;

public:
    Walls();
    ~Walls();

    void Render( SDL_Renderer *renderer );
};

class Scenery {

private:

public:
    Scenery();
    ~Scenery();

    void Render( SDL_Renderer *renderer );
};

class Score {

private:
    SDL_Rect scoreBox;

public:
    Score();
    ~Score();

    void Render( SDL_Renderer *renderer, const char* content, TTF_Font* gFont );
};


class GamePlay {

private:

    float obstacleCoolTimes[3];
    float fallRates[3];
    int difficulty;

    float obstacleCoolTime;

    SDL_Renderer *renderer;
    TTF_Font *gFont;

    Player *player;
    Walls *walls;
    Scenery *scenery;
    Score *score;
    std::vector< Obstacle* > obstacles;

    bool isPlaying;

    float fps, elapsed;
    Uint32 totalFrames;

public:

    GamePlay( SDL_Renderer* renderer, TTF_Font* gFont, int difficulty );
    ~GamePlay();

    void GenerateObstacle();
    bool CheckCollision();

    void ReduceObstacleCoolTime( float elapsed );

    void HandleEvents();
    void Update();
    void Render();

    void Clean();


    void SetFPS( float );
    void SetElapsed( float );
    void SetTotalFrames( Uint32 );

    float GetFPS();
    float GetElapsed();
    Uint32 GetTotalFrames();

    bool IsPlaying();
};

#endif
