
#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "./GamePlay.hpp"

class Game {

private:

    enum GameState { MAIN_MENU, PLAYING, SCORE, SETTINGS };

    GameState gameState;
    bool isRunning;

    int difficulty;

    SDL_Window *window;
    SDL_Renderer *renderer;

    TTF_Font *gFont;

public:

    Game();
    ~Game();

    void Init(const char*, int, int, int, int, bool);

    void HandleEvents();

    void RenderMainMenu();
    void QueryMainMenu(int, int);

    void RenderSettings();
    void QuerySettings(int, int);

    void RenderScores();
    void QueryScores(int, int);

    void Play();

    int* GetHighScores( int *arr );
    void ComputeHighScore( int score );

    void Clean();

    bool IsRunning();
    TTF_Font* GetFont();
    SDL_Renderer* GetRenderer();
};

#endif
