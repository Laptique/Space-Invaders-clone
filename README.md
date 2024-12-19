# Space Invaders

This is simple attempt at making a Space Invaders-style clone, built using SDL2 in C++. Defend the planet by shooting down waves of enemies before they reach the bottom of the screen. This game features player movement, shooting mechanics, and a game-over condition when enemies break through your defenses.

---

## 🎮 Gameplay

- **Move the your ship**: Use `W`, `A`, `S`, `D` keys to move the player (green rectangle).
- **Shoot the enemy**: Press the `Space` key to shoot bullets.
- **Objective**: Destroy all enemies (red rectangles) to win. If any enemy reaches the bottom of the screen, the game is over.

---

## 🛠️ Installation and Setup

Follow these steps to set up and run the game in **Visual Studio Code**:

### Prerequisites
1. Install **SDL2 Development Libraries** (for this example, I've already included the necessary files in the repository):
   - For Windows: Use `vcpkg` or download from [SDL2](https://libsdl.org).
   - For macOS: Use `brew install sdl2`.
   - For Linux: Use `sudo apt install libsdl2-dev`.

2. Install a C++ compiler (e.g., `g++` or `clang`).

3. Install **Make** if not already installed.

---

### How to Run

1. Clone or download this repository to your local machine.
   
2. Open the project folder in **Visual Studio Code**.

3. Compile the game by typing the following command in the terminal:
   ```bash
   make
