
#include "../inc/Game.hpp"

Game::Game() {}
Game::~Game() {}

void Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) { // Initialises SDL

    SDL_Init( SDL_INIT_EVERYTHING );
    
    TTF_Init();
    gFont = TTF_OpenFont("./gameData/fonts/UbuntuMono-R.ttf", 24);

    int flags = (fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);

    window = SDL_CreateWindow("LibreJump", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, flags); // Creates Window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED); // Creates Renderer


    gameState = MAIN_MENU;
    RenderMainMenu();

    difficulty = 1;

    isRunning = true;
}

void Game::HandleEvents() {  // Responds to user inputs

    SDL_Event event;
    SDL_WaitEvent( &event );

    switch( event.type ) {

        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            
            if( gameState == MAIN_MENU )    QueryMainMenu( event.button.x, event.button.y );
            else if( gameState == SETTINGS )    QuerySettings( event.button.x, event.button.y );
            else if( gameState == SCORE )    QueryScores( event.button.x, event.button.y );

            break;

        default:
            break;

    }
}

void Game::RenderMainMenu() { // Display the game menu

    SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
    SDL_RenderClear( renderer );

    SDL_Rect b1, b2, b3, e;

    b1 = {100, 100, 200, 100};
    b2 = {100, 300, 200, 100};
    b3 = {100, 500, 200, 100};
    e = {700, 125, 50, 50};

    UI::RenderTextBox(renderer, b1, "PLAY", gFont, {0,0,255}, {0,0,0});
    UI::RenderTextBox(renderer, b2, "SETTINGS", gFont, {0,0,255}, {0,0,0});
    UI::RenderTextBox(renderer, b3, "SCORE", gFont, {0,0,255}, {0,0,0});

    UI::RenderTextBox(renderer, e, "X", gFont, {0,0,255}, {0,0,0});

    SDL_RenderPresent( renderer );
}
void Game::QueryMainMenu( int posX, int posY ) { //Takes action based on selection

    if( (100 < posX) and (posX < 300) and (100 < posY) and (posY < 200) ) {

        gameState = PLAYING;
        Play();

    } 
    
    else if((100 < posX) and (posX < 300) and (300 < posY) and (posY < 400)) {

        gameState = SETTINGS;
        RenderSettings();
    }

    else if((100 < posX) and (posX < 300) and (500 < posY) and (posY < 600)) {

        gameState = SCORE;
        RenderScores();
    }

    else if((700 < posX) and (posX < 750) and (125 < posY) and (posY < 175)) {

        isRunning = false;
    }
}

void Game::RenderSettings() {  // Settings page
    
    SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
    SDL_RenderClear( renderer );

    SDL_Rect b1, b2, b3, e;

    b1 = {100, 100, 200, 100};
    b2 = {100, 300, 200, 100};
    b3 = {100, 500, 200, 100};
    e = {700, 125, 50, 50};

    UI::RenderTextBox(renderer, b1, "EASY", gFont, {0,0,255}, {0,0,0});
    UI::RenderTextBox(renderer, b2, "MEDIUM", gFont, {0,0,255}, {0,0,0});
    UI::RenderTextBox(renderer, b3, "HARD", gFont, {0,0,255}, {0,0,0});

    UI::RenderTextBox(renderer, e, "X", gFont, {0,0,255}, {0,0,0});


    SDL_RenderPresent( renderer );

}
void Game::QuerySettings( int posX, int posY ) { // Set Difficulty

    if( (100 < posX) and (posX < 300) and (100 < posY) and (posY < 200) ) {

        difficulty = 0;

    } 
    
    else if((100 < posX) and (posX < 300) and (300 < posY) and (posY < 400)) {

        difficulty = 1;
    }

    else if((100 < posX) and (posX < 300) and (500 < posY) and (posY < 600)) {

        difficulty = 2;
    }

    else if((700 < posX) and (posX < 750) and (125 < posY) and (posY < 175)) {

    }

    else {

        return;
    }

    gameState = MAIN_MENU;
    RenderMainMenu();
}

void Game::RenderScores() { //Display Scores

    SDL_SetRenderDrawColor( renderer, 255, 0, 255, 255 );
    SDL_RenderClear( renderer );

    SDL_Rect r, r1, r2, r3, e;

    r = {500, 300, 200, 100};
    r1 = {100, 100, 200, 100};
    r2 = {100, 300, 200, 100};
    r3 = {100, 500, 200, 100};

    e = {700, 125, 50, 50};

    int scores[3];
    GetHighScores(scores);

    char numberstring1[50];
    sprintf(numberstring1, "%d", scores[0]);
    
    char numberstring2[50];
    sprintf(numberstring2, "%d", scores[1]);
    
    char numberstring3[50];
    sprintf(numberstring3, "%d", scores[2]);

    UI::RenderTextBox(renderer, r, "HIGH SCORES", gFont, {0,0,255}, {0,0,0});
    UI::RenderTextBox(renderer, r1, numberstring1, gFont, {0,50,100}, {0,0,0});
    UI::RenderTextBox(renderer, r2, numberstring2, gFont, {0,50,100}, {0,0,0});
    UI::RenderTextBox(renderer, r3, numberstring3, gFont, {0,50,100}, {0,0,0});
    UI::RenderTextBox(renderer, e, "X", gFont, {0,0,255}, {0,0,0});


    SDL_RenderPresent( renderer );
}
void Game::QueryScores( int posX, int posY ) {

    if((700 < posX) and (posX < 750) and (125 < posY) and (posY < 175)) {
        gameState = MAIN_MENU;
        RenderMainMenu();
    }
}

int* Game::GetHighScores( int *arr ) {  // Returns 3 highest scores

    std::ifstream fin;
    fin.open("./gameData/highScores", std::ios_base::in);

    fin >> arr[0] >> arr[1] >> arr[2];

    fin.close();

    return arr;
} 

void Game::ComputeHighScore( int score ) { // Calculates High scores


    int scores[3];
    GetHighScores(scores);

    std::ofstream fout;

    fout.open("./gameData/highScores", std::ofstream::out | std::ofstream::trunc);

    for(int i = 0; i < 3; i++) {
        if(score > scores[i]) {
            for(int j = 2; j > i; j--) {
                scores[j] = scores[j-1];
            }

            scores[i] = score;

            break;
        }
    }

    fout << scores[0] << '\n' << scores[1] << '\n' << scores[2];
}

void Game::Play() { // Main Game loop

    srand(time(NULL));

    GamePlay *gamePlay = new GamePlay( renderer, gFont, difficulty );

    while( gamePlay->IsPlaying() ) {
	    
        gamePlay->SetTotalFrames( gamePlay->GetTotalFrames() + 1 );

        Uint64 start = SDL_GetPerformanceCounter();

        gamePlay->HandleEvents();
        gamePlay->Update();
        gamePlay->Render();

        SDL_RenderPresent( renderer );

        Uint64 end = SDL_GetPerformanceCounter();

	    gamePlay->SetElapsed( (end - start) / (float)SDL_GetPerformanceFrequency() );

        gamePlay->SetFPS( 1.0f / gamePlay->GetElapsed() );

    }

    ComputeHighScore(gamePlay->GetTotalFrames());

    int arr[3];
    GetHighScores( arr );

    gamePlay->Clean();
    
    delete gamePlay;

    gameState = MAIN_MENU;
    RenderMainMenu();
}

void Game::Clean() { // Close SDL

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    TTF_CloseFont( gFont );
    
    TTF_Quit();
    SDL_Quit();
    
}

bool Game::IsRunning() {

    return this->isRunning;
}

TTF_Font* Game::GetFont() {

    return gFont;
}

SDL_Renderer* Game::GetRenderer() {

    return renderer;
}
