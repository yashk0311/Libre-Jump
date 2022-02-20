
#include "../inc/GamePlay.hpp"


void UI::RenderTextBox( SDL_Renderer *renderer, SDL_Rect rect, const char* content, TTF_Font* gFont, SDL_Color bgColor, SDL_Color fontColor ) {
    
    SDL_Surface* surfaceMessage =
    TTF_RenderText_Solid(gFont, content, fontColor); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_FreeSurface( surfaceMessage ); 

    SDL_SetRenderDrawColor( renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a );
    SDL_RenderFillRect( renderer, &rect );

    SDL_RenderCopy( renderer, Message, NULL, &rect );

}

Entity::Entity() {}
Entity::~Entity() {}
void Entity::Render( SDL_Renderer *renderer ) { // Used for rendering

    SDL_SetRenderDrawColor( renderer, 0, 200, 20, 255 );
    SDL_RenderFillRect( renderer, &hitBox );
}
void Entity::reduceCoolTime( float elapsed ) {

    coolTime -= 1.0 * elapsed;

    coolTime = (coolTime < 0) ? 0 : coolTime;

    // std::cout << coolTime << '\n';
}
CurrWall Entity::GetCurrWall() {

    return currWall;
}
float Entity::GetPosX() {

    return posX;
}
float Entity::GetPosY() {

    return posY;
}
float Entity::GetWidth() {

    return width;
}
float Entity::GetHeight() {

    return height;
}



Player::Player() { // Player of the game (Ninja)

    coolTime = 0;

    currWall = LEFT;

    posX = 200;
    posY = 700;

    width = 50;
    height = 80;

    hitBox.x = posX;
    hitBox.y =  posY;
    hitBox.w = width;
    hitBox.h = height;
}
Player::~Player() {}
void Player::Jump() {

    if( coolTime )
        return;

    if( currWall == LEFT ) {
        currWall = RIGHT;
        posX = 550;
        hitBox.x = posX;
    } else {
        currWall = LEFT;
        posX = 200;
        hitBox.x = posX;
    }

    coolTime = 0.1;
}

Obstacle::Obstacle( CurrWall currWall, float fallRate ) { // Obstacle of the game
    
    this->currWall = currWall;

    posX = (currWall == LEFT) ? 200 : 550;
    posY = 0;

    width = 50;
    height = 50;

    hitBox.x = posX;
    hitBox.y =  posY;
    hitBox.w = width;
    hitBox.h = height;

    this->fallRate = fallRate; 
};
Obstacle::~Obstacle() {};
void Obstacle::Fall( float elapsed ) {

    posY += elapsed * fallRate; // = 800.0
    hitBox.y = posY;
}

Walls::Walls() {

    r1.h = 800;
    r1.w = 200;
    r1.x = 0;
    r1.y = 0;

    r2.h = 800;
    r2.w = 200;
    r2.x = 600;
    r2.y = 0;
}
Walls::~Walls() {}
void Walls::Render( SDL_Renderer *renderer ) { // Creates Two walls

    SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
    SDL_RenderFillRect( renderer, &r1 );
    SDL_RenderFillRect( renderer, &r2 );

}

Scenery::Scenery() {}
Scenery::~Scenery() {}
void Scenery::Render( SDL_Renderer *renderer ) {

    SDL_SetRenderDrawColor( renderer, 0, 255, 255, 255 );
    SDL_RenderClear( renderer );

    // SDL_RenderPresent( renderer );
}

Score::Score() {

    scoreBox.h = 100;
    scoreBox.w = 200;

    scoreBox.x = 300;
    scoreBox.y = 100;
}
Score::~Score() {}
void Score::Render( SDL_Renderer *renderer, const char* content, TTF_Font *gFont ) { // Shows score on the screen

    UI::RenderTextBox( renderer, scoreBox, content, gFont, {0,0,255,255}, {0,0,0});
}


GamePlay::GamePlay( SDL_Renderer *renderer, TTF_Font *gFont, int difficulty ) { // Starts the game
    
    scenery = new Scenery();
    walls = new Walls();
    player = new Player();
    score = new Score();

    isPlaying = true;
    this->renderer = renderer;
    this->gFont = gFont;

    fps = 0;
    totalFrames = 0;
    elapsed = 0;

    obstacleCoolTime = 0;

    obstacleCoolTimes[0] = 1.8;
    obstacleCoolTimes[1] = 1;
    obstacleCoolTimes[2] = 0.4;

    fallRates[0] = 200.0;
    fallRates[1] = 400.0;
    fallRates[2] = 800.0;

    this->difficulty = difficulty;
}

GamePlay::~GamePlay() {

    delete scenery;
    delete walls;
    delete player;

    for(auto o : obstacles)
        delete o;
}

void GamePlay::GenerateObstacle() {

    Obstacle *o = new Obstacle( ((rand()%2) ? LEFT : RIGHT), fallRates[difficulty] );

    obstacles.push_back(o);
}
bool GamePlay::CheckCollision() { // Collision Detection and ending game
    
    for(auto o : obstacles) {
        if(player->GetCurrWall() == o->GetCurrWall()) {
            if(o->GetPosY() > 650) {

                isPlaying = false;

                SDL_SetRenderDrawColor( renderer, 100, 100, 100, 255 );
                SDL_RenderClear( renderer );

                SDL_Rect r;

                r.h = 300;
                r.w = 800;
                r.x = 0;
                r.y = 250;
               
                UI::RenderTextBox( renderer, r, "GAME OVER!", gFont, {0,255,0}, {0,0,0});

                char numberstring[50];
                sprintf(numberstring, "%d", totalFrames);

                score->Render( renderer, numberstring, gFont );

                SDL_RenderPresent( renderer );

                SDL_Delay(1000);
            }
        }
    }
}
void GamePlay::ReduceObstacleCoolTime( float elapsed ) {

    obstacleCoolTime -= 1.0 * elapsed;

    obstacleCoolTime = (obstacleCoolTime < 0) ? 0 : obstacleCoolTime;

    // std::cout << coolTime << '\n';
}

void GamePlay::HandleEvents() {  // Jumping of ninja and quit game
    
    SDL_Event event;
    SDL_PollEvent(&event);

    switch( event.type ) {

        case SDL_QUIT:
            isPlaying = false;
            break;

        case SDL_KEYDOWN:
            player->Jump();
            break;

        default:
            break;
    }
}
void GamePlay::Update() {

    CheckCollision();

    if( !obstacleCoolTime ) {

        GenerateObstacle();

        obstacleCoolTime = obstacleCoolTimes[difficulty];

        std::cout << difficulty << '\n';
    }
    
    player->reduceCoolTime( elapsed );
    ReduceObstacleCoolTime( elapsed );

    auto it = obstacles.begin();
    while(it != obstacles.end()) {

        (*it)->Fall( elapsed );

        if((*it)->GetPosY() > 800) {
            delete (*it);
            it = obstacles.erase(it);
            continue;
        }
        
        it++;
    }
}
void GamePlay::Render() {

    scenery->Render( renderer );
    walls->Render( renderer );
    
    char numberstring[50];
    sprintf(numberstring, "%d", totalFrames);
    score->Render( renderer, numberstring, gFont );

    player->Render(renderer);
    
    for(auto o : obstacles)
        o->Render(renderer);
}

void GamePlay::Clean() {
    
}

void GamePlay::SetFPS( float fps ) {

    this->fps = fps;
}
void GamePlay::SetElapsed( float elapsed ) {

    this->elapsed = elapsed;
}
void GamePlay::SetTotalFrames( Uint32 totalFrames ) {

    this->totalFrames = totalFrames;
}

float GamePlay::GetFPS() {

    return fps;
}
float GamePlay::GetElapsed() {

    return elapsed;
}
Uint32 GamePlay::GetTotalFrames() {

    return totalFrames;
}
bool GamePlay::IsPlaying() {
    
    return isPlaying;
}
