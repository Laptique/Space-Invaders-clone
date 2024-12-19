#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_WIDTH = 50;
const int PLAYER_HEIGHT = 20;
const int BULLET_WIDTH = 5;
const int BULLET_HEIGHT = 10;
const int ENEMY_WIDTH = 40;
const int ENEMY_HEIGHT = 20;
const int ENEMY_SPEED = 1;
const int SHOOT_COOLDOWN = 100; // Milliseconds between shots
bool gameOverDisplayed = false;

struct Bullet {
    int x, y;
    bool active;
};

struct Enemy {
    int x, y;
    bool active;
};

// Initialize SDL and create a window
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }
    return true;
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Draw a rectangle
void drawRect(int x, int y, int width, int height, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &rect);
}

// Draw text (stub for simplicity, use SDL_ttf for real fonts)
void drawText(const std::string& text, int x, int y, SDL_Color color) {
    // Placeholder: SDL_ttf integration required for real text rendering
    SDL_Log("%s at (%d, %d)", text.c_str(), x, y); // Log the text in the console
}

int SDL_main(int argc, char* args[]) {
    if (!init()) return -1;

    srand(static_cast<unsigned>(time(0)));

    // Player variables
    int playerX = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    int playerY = SCREEN_HEIGHT - 50;
    int playerSpeed = 5;

    // Bullet variables
    std::vector<Bullet> bullets;
    bool canShoot = true;
    Uint32 lastShootTime = 0;

    // Enemy variables
    std::vector<Enemy> enemies;
    int enemySpeed = ENEMY_SPEED;

    // Create initial enemies
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 10; ++j) {
            enemies.push_back({ j * (ENEMY_WIDTH + 10) + 50, i * (ENEMY_HEIGHT + 10) + 50, true });
        }
    }

    // Game variables
    bool quit = false;
    SDL_Event e;
    bool gameOver = false;
    int score = 0;


// Game loop
while (!quit) {
    // Handle events
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) quit = true;
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (!gameOver) {
        // Player movement
        if (state[SDL_SCANCODE_W] && playerY > 0) playerY -= playerSpeed;
        if (state[SDL_SCANCODE_S] && playerY < SCREEN_HEIGHT - PLAYER_HEIGHT) playerY += playerSpeed;
        if (state[SDL_SCANCODE_A] && playerX > 0) playerX -= playerSpeed;
        if (state[SDL_SCANCODE_D] && playerX < SCREEN_WIDTH - PLAYER_WIDTH) playerX += playerSpeed;

        // Shooting (with cooldown)
        Uint32 currentTime = SDL_GetTicks();
        if (state[SDL_SCANCODE_SPACE] && canShoot && currentTime - lastShootTime > SHOOT_COOLDOWN) {
            bullets.push_back({ playerX + PLAYER_WIDTH / 2 - BULLET_WIDTH / 2, playerY, true });
            lastShootTime = currentTime;
            canShoot = false;
        }
        if (!state[SDL_SCANCODE_SPACE]) canShoot = true;

        // Update bullets
        for (auto& bullet : bullets) {
            if (bullet.active) {
                bullet.y -= 10;
                if (bullet.y < 0) bullet.active = false;
            }
        }

        // Update enemies
        bool reachedBottom = false;
        for (auto& enemy : enemies) {
            if (enemy.active) {
                enemy.y += enemySpeed;
                if (enemy.y > SCREEN_HEIGHT - ENEMY_HEIGHT) {
                    reachedBottom = true;
                    enemy.active = false;
                }
            }
        }

        if (reachedBottom) {
            gameOver = true;
        }

        // Check collisions
        for (auto& bullet : bullets) {
            if (!bullet.active) continue;
            for (auto& enemy : enemies) {
                if (!enemy.active) continue;
                if (bullet.x < enemy.x + ENEMY_WIDTH && bullet.x + BULLET_WIDTH > enemy.x &&
                    bullet.y < enemy.y + ENEMY_HEIGHT && bullet.y + BULLET_HEIGHT > enemy.y) {
                    bullet.active = false;
                    enemy.active = false;
                    score += 10; // Increase score for each destroyed enemy
                }
            }
        }

        // Check if all enemies are destroyed
        gameOver = std::all_of(enemies.begin(), enemies.end(), [](Enemy& e) { return !e.active; });
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (!gameOver) {
        // Draw player
        drawRect(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, { 0, 255, 0, 255 });

        // Draw bullets
        for (auto& bullet : bullets) {
            if (bullet.active) {
                drawRect(bullet.x, bullet.y, BULLET_WIDTH, BULLET_HEIGHT, { 255, 255, 0, 255 });
            }
        }

        // Draw enemies
        for (auto& enemy : enemies) {
            if (enemy.active) {
                drawRect(enemy.x, enemy.y, ENEMY_WIDTH, ENEMY_HEIGHT, { 255, 0, 0, 255 });
            }
        }

        // Draw score
        drawText("Score: " + std::to_string(score), 10, 10, { 255, 255, 255, 255 });
    } else if (!gameOverDisplayed) {
        // Draw Game Over screen
        drawText("Game Over", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 20, { 255, 0, 0, 255 });
        drawText("Final Score: " + std::to_string(score), SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT / 2 + 20, { 255, 255, 255, 255 });
        gameOverDisplayed = true; // Mark game over screen as displayed
    }

    // Update screen
    SDL_RenderPresent(renderer);

    // Delay to control frame rate
    SDL_Delay(30);
}


    close();
    return 0;
}
