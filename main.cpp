#include <iostream>
#include <raylib.h>
#include <cmath>
using namespace std;

class PlayerAnimation {
    public:
        Image animation;
        Texture2D texture;
        int frameWidth;
        int frames;
        int frameHeight;
        int currentFrame;
        float frameTime;
        float timer;
        Rectangle frameRec;
        Vector2 position;

        PlayerAnimation(const char* image_path, int frames)
        {
            
            this->animation = LoadImage(image_path);
            this->texture = LoadTextureFromImage(this->animation);
            this->frameWidth = this->texture.width / frames;
            this->frames = frames;
            this->frameHeight = this->texture.height;
            this->currentFrame = 0;
            this->frameTime = 0.1f;
            this->timer = 0.0f;
            this->frameRec = { 0.0f, 0.0f, static_cast<float>(this->frameWidth), static_cast<float>(this->frameHeight) };
            this->position = { 0.0f, 240.0f };

            if(this->texture.id ==0){
                cout << "Failed to load texture.";
            }

            UnloadImage(this->animation);

        }

        void update(){
            this->timer += GetFrameTime();

            
            if (this->timer >= this->frameTime) {
                this->timer = 0.0f; 
                this->currentFrame++;  

                if (this->currentFrame >= this->frames) this->currentFrame = 0; 
            }

            this->frameRec.x = static_cast<float>(this->currentFrame) * static_cast<float>(this->frameWidth);
        }

        void draw() const{
            DrawTextureRec(this->texture, this->frameRec, this->position, WHITE);

        }

        int getFrameWidth() const{
            return this->frameWidth;
        }

        int getFrameHeight() const{
            return this->frameHeight;
        }

        Rectangle getBoundingBox() const {
            return Rectangle{
                position.x,
                position.y,
                static_cast<float>(frameWidth/2),
                static_cast<float>(frameHeight/2)
            };
        }

        void setPosition(Vector2 newPosition) {
            position = newPosition;
        }

        void unload() const{
            UnloadTexture(this->texture);
        }

};

class PlayerAnimation2 : public PlayerAnimation {
    public:
        PlayerAnimation2(const char* image_path, int frames) : PlayerAnimation(image_path, frames) {
            this->position = { 800.0f, 240.0f };
        }

        void update() {
            this->timer += GetFrameTime();

            if (this->timer >= this->frameTime) {
                this->timer = 0.0f; 
                this->currentFrame++;  

                if (this->currentFrame >= this->frames) this->currentFrame = 0; 
            }

            
            this->frameRec.x = static_cast<float>(this->currentFrame) * static_cast<float>(this->frameWidth);
        }

        void draw() const {
            DrawTextureRec(this->texture, this->frameRec, this->position, WHITE);
        }

        void unload() const {
            UnloadTexture(this->texture);
        }
};

bool checkCollision(const PlayerAnimation& player1, const PlayerAnimation& player2) {
    Rectangle rect1 = player1.getBoundingBox();
    Rectangle rect2 = player2.getBoundingBox();
    return CheckCollisionRecs(rect1, rect2);
}

int main() {
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Huh?");

    Texture2D background_image = LoadTexture("../textures/background1.png");

    Rectangle sourceRec = { 0.0f, 0.0f, static_cast<float>(background_image.width), static_cast<float>(background_image.height) };

   
    Rectangle destRec = { 0.0f, 0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight) };

    Vector2 origin = { 0.0f, 0.0f };

    PlayerAnimation player_walking("../textures/City_men_2/Walk.png", 10);

    PlayerAnimation player_idle("../textures/City_men_2/Idle.png", 6);
   
    PlayerAnimation player_running("../textures/City_men_2/Run.png", 10);
    
    PlayerAnimation player_attack("../textures/City_men_2/Attack.png", 4);

    PlayerAnimation player_hurt("../textures/City_men_2/Hurt.png", 3);

    PlayerAnimation player_dead("../textures/City_men_2/Dead.png", 4);

    PlayerAnimation player_walking2("../textures/City_men_2/reverse/Walk2.png", 10);

    PlayerAnimation player_idle2("../textures/City_men_2/reverse/Idle2.png", 6);

    PlayerAnimation player_running2("../textures/City_men_2/reverse/Run2.png", 10);

    PlayerAnimation player_attack2("../textures/City_men_2/reverse/Attack2.png", 4);

    PlayerAnimation player_hurt2("../textures/City_men_2/reverse/Hurt2.png", 3);

    PlayerAnimation player_dead2("../textures/City_men_2/reverse/Dead2.png", 4);

    PlayerAnimation2 player2_walking("../textures/City_men_1/Walk.png", 10);
    PlayerAnimation2 player2_idle("../textures/City_men_1/Idle.png", 6);
    PlayerAnimation2 player2_running("../textures/City_men_1/Run.png", 10);
    PlayerAnimation2 player2_attack("../textures/City_men_1/Attack.png", 5);
    PlayerAnimation2 player2_hurt("../textures/City_men_1/Hurt.png", 3);
    PlayerAnimation2 player2_dead("../textures/City_men_1/Dead.png", 4);


    bool isAttacking = false;
 
    SetTargetFPS(60);               
    PlayerAnimation *current_animation = &player_walking;
    PlayerAnimation2 *current_animation2 = &player2_idle;
    current_animation2->position = { screenWidth- screenWidth/2, 240.0f };
    
    while (!WindowShouldClose())    
    { 
        if (IsKeyDown(KEY_SPACE)) {
            
            player_attack.position = current_animation->position;  
            current_animation = &player_attack;
            isAttacking = true;
        } 
        else if (IsKeyDown(KEY_RIGHT)) {
            
            if (current_animation != &player_running) {
                player_running.position = current_animation->position;  
                current_animation = &player_running;
            }

            current_animation->position.x += 2.0f;
        } 

        else if (IsKeyDown(KEY_LEFT)) {
                
                if (current_animation != &player_running2) {
                    player_running2.position = current_animation->position;  
                    current_animation = &player_running2;
                }
    
                current_animation->position.x -= 2.0f;
            
        }
    
        else {
            player_idle.position = current_animation->position;  
            current_animation = &player_idle;
        }

        if (current_animation2->position.x < current_animation->position.x+current_animation->getFrameWidth()/4) {
            player2_running.position = current_animation2->position;  
            current_animation2 = &player2_running;
            current_animation2->position.x += 2.0f;
            if (checkCollision(*current_animation, *current_animation2)) {
            player_hurt.position = current_animation->position;
            current_animation = &player_hurt;

            
        }
        } else if (current_animation2->position.x > current_animation->position.x + current_animation->getFrameWidth()/4) {
            player2_running.position = current_animation2->position;  
            current_animation2 = &player2_running;
            current_animation2->position.x -= 2.0f;
            if (checkCollision(*current_animation, *current_animation2)) {
            player_hurt.position = current_animation->position;
            current_animation = &player_hurt;

            
        }
        } else {
            player2_attack.position = current_animation2->position;  
            current_animation2 = &player2_attack;
        }
        
    
        current_animation->update();
        current_animation2->update();
  
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexturePro(background_image, sourceRec, destRec, origin, 0.0f, WHITE);
            current_animation->draw();
            current_animation2->draw();

        EndDrawing();
    }

    player_walking.unload();
    player_idle.unload();
    player_running.unload();
    player_attack.unload();
    player_hurt.unload();
    player_dead.unload();
    player_walking2.unload();
    player_idle2.unload();
    player_running2.unload();
    player_attack2.unload();
    player_hurt2.unload();
    player_dead2.unload();
    player2_walking.unload();
    player2_idle.unload();
    player2_running.unload();
    player2_attack.unload();
    player2_hurt.unload();
    player2_dead.unload();
    
    UnloadTexture(background_image);

 CloseWindow();       
    return 0;
}

