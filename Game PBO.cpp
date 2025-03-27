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
protected:
    int x, y;
public:
    Entity(int xPos, int yPos) : x(xPos), y(yPos) {}
    virtual ~Entity() {}
    int getX() const { return x; }
    int getY() const { return y; }
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
private:
    bool active;
public:
    Bullet() : Entity(-1, -1), active(false) {}
    ~Bullet() { cout << "Bullet destroyed" << endl; }

    bool isActive() const { return active; }
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
private:
    static const int moveDelay;
    int moveCounter;
public:
    Enemy() : Entity(rand() % width, 0), moveCounter(0) {}
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
const int Enemy::moveDelay = 3;


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

                if (i == enemy.getY() && j == enemy.getX()) cout << "X";
                else {
                    bool printed = false;
                    for (Bullet &b : bullets) {
                        if (b.isActive() && i == b.getY() && j == b.getX()) {
                            cout << "|";
                            printed = true;
                            break;
                        }
                    }
                    if (!printed) {
                        if (i == player.getY() && j == player.getX()) cout << "^";
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
                        if (!b.isActive()) {
                            b.shoot(player.getX());
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
            if (b.isActive() && b.getX() == enemy.getX() && b.getY() == enemy.getY()) {
                score++;
                enemy.reset();
            }
        }
        if (enemy.getY() >= height - 1) gameOver = true;
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
