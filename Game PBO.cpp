#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

const int width = 20;
const int height = 20;
const int maxBullets = 10;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPos;

void clearScreen() {
    cursorPos.X = 0;
    cursorPos.Y = 0;
    SetConsoleCursorPosition(hConsole, cursorPos);
}


class Entity {
public:
    int x, y;
    Entity(int xPos, int yPos) : x(xPos), y(yPos) {}
    virtual ~Entity() {}
};


class Player : public Entity {
public:
    Player() : Entity(width / 2, height - 1) {}
    ~Player() { cout << "Player destroyed" << endl; }

    void move(char direction) {
        if (direction == 'a' && x > 0) x--;
        if (direction == 'd' && x < width - 1) x++;
    }
};


class Bullet : public Entity {
public:
    bool active;
    Bullet() : Entity(-1, -1), active(false) {}
    ~Bullet() { cout << "Bullet destroyed" << endl; }

    void shoot(int px) {
        if (!active) {
            x = px;
            y = height - 2;
            active = true;
        }
    }

    void update() {
        if (active) {
            y--;
            if (y < 0) active = false;
        }
    }
};


class Enemy : public Entity {
public:
    int moveDelay;
    int moveCounter;
    Enemy() : Entity(rand() % width, 0), moveDelay(3), moveCounter(0) {}
    ~Enemy() { cout << "Enemy destroyed" << endl; }

    void update() {
        moveCounter++;
        if (moveCounter >= moveDelay) {
            y++;
            moveCounter = 0;
        }
    }

    void reset() {
        x = rand() % width;
        y = 0;
        moveCounter = 0;
    }
};


class Game {
public:
    Player player;
    vector<Bullet> bullets;
    Enemy enemy;
    bool gameOver;
    int score;

    Game() : gameOver(false), score(0) {
        bullets.resize(maxBullets);
    }
    ~Game() { cout << "Game Over! Skor akhir: " << score << endl; }

    void draw() {
        clearScreen();

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0) cout << "#";

                if (i == enemy.y && j == enemy.x) cout << "X";
                else {
                    bool printed = false;
                    for (Bullet &b : bullets) {
                        if (b.active && i == b.y && j == b.x) {
                            cout << "|";
                            printed = true;
                            break;
                        }
                    }
                    if (!printed) {
                        if (i == player.y && j == player.x) cout << "^";
                        else cout << " ";
                    }
                }

                if (j == width - 1) cout << "#";
            }
            cout << endl;
        }

        for (int i = 0; i < width + 2; i++) cout << "#";
        cout << endl;

        cout << "Gunakan A/D untuk bergerak, Spasi untuk menembak, X untuk keluar.\n";
        cout << "Skor: " << score << endl;
    }

    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a': player.move('a'); break;
                case 'd': player.move('d'); break;
                case ' ':
                    for (Bullet &b : bullets) {
                        if (!b.active) {
                            b.shoot(player.x);
                            break;
                        }
                    }
                    break;
                case 'x': gameOver = true; break;
            }
        }
    }

    void update() {
        enemy.update();
        for (Bullet &b : bullets) {
            b.update();
            if (b.active && b.x == enemy.x && b.y == enemy.y) {
                score++;
                b.active = false;
                enemy.reset();
            }
        }

        if (enemy.y >= height - 1) gameOver = true;
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            update();
            Sleep(50);
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
