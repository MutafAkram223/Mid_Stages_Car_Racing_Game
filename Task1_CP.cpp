#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void gotoxy(int x, int y);
void printEnemy();
void eraseEnemy();
void erasePlayer();
void printPlayer();
void movePlayerLeft();
void movePlayerRight();
void moveEnemy();
void printMaze();
void scrollRoads();
void generateBullet();
void moveBullets();
void printBullet(int x, int y);
void eraseBullet(int x, int y);
void bulletCollisionWithEnemy();
void checkCollisionWithPlayer();
void addScore();
void printScore();
void gameOver();

const int enemyWidth = 7;
const int enemyHeight = 4;
const int playerWidth = 7;
const int playerHeight = 4;
const int BULLET_MAX = 100;
const int mazeWidth = 54;
const int NUM_ROADS = 6;

int roadPositions[NUM_ROADS]; // X positions of the roads
int linePositions[NUM_ROADS - 1]; // X positions of the lines between roads

int eX, eY = 2;
int eRoadIndex;
int pX = 20 , pY = 21;
int pRoadIndex = 2; // Start at the middle road
int bulletX[BULLET_MAX], bulletY[BULLET_MAX];
bool isBulletActive[BULLET_MAX];
int bulletCount = 0;      
int score = 0;
int roadOffset = 0; // Used to simulate road movement

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main() {
    srand(time(0));
    system("cls");
    printMaze();

    int playableWidth = mazeWidth - 2; 
    int roadWidth = playableWidth / NUM_ROADS;
    for (int i = 0; i < NUM_ROADS; i++) {
        roadPositions[i] = 1 + i * roadWidth + roadWidth / 2 - playerWidth / 2;
    }
    for (int i = 0; i < NUM_ROADS - 1; i++) {
        linePositions[i] = 1 + (i + 1) * roadWidth;
    }

    printPlayer();
    eRoadIndex = rand() % NUM_ROADS;
    printEnemy();

    while (true) {
        if (GetAsyncKeyState(VK_LEFT)) {
            movePlayerLeft();
        }
        if (GetAsyncKeyState(VK_RIGHT)) {
            movePlayerRight();
        }
        if (GetAsyncKeyState(VK_SPACE)) {
            generateBullet();
        }
        moveBullets();
        bulletCollisionWithEnemy();
        moveEnemy();
        checkCollisionWithPlayer();
        scrollRoads()       ;
        printScore();
        Sleep(100);
    }
    return 0;
}

void printPlayer() {
    pX = roadPositions[pRoadIndex];
    gotoxy(pX, pY);
    cout << "   || ";
    gotoxy(pX, pY + 1);
    cout << "  ||||";
    gotoxy(pX, pY + 2);
    cout << "   || ";
    gotoxy(pX, pY + 3);
    cout << "  |||| ";
}

void movePlayerLeft() {
    if (pRoadIndex > 0) {
        erasePlayer();
        pRoadIndex--;
        printPlayer();
    }
}

void movePlayerRight() {
    if (pRoadIndex < NUM_ROADS - 1) {
        erasePlayer();
        pRoadIndex++;
        printPlayer();
    }
}

void erasePlayer() {
    gotoxy(pX, pY);
    cout << "       ";
    gotoxy(pX, pY + 1);
    cout << "       ";
    gotoxy(pX, pY + 2);
    cout << "       ";
    gotoxy(pX, pY + 3);
    cout << "       ";
}

void printEnemy() {
    eX = roadPositions[eRoadIndex];
    gotoxy(eX, eY);
    cout << " ****  ";
    gotoxy(eX, eY + 1);
    cout << "  **   ";
    gotoxy(eX, eY + 2);
    cout << " ****  ";
    gotoxy(eX, eY + 3);
    cout << "  **   ";
}

void eraseEnemy() {
    gotoxy(eX, eY);
    cout << "       ";
    gotoxy(eX, eY + 1);
    cout << "       ";
    gotoxy(eX, eY + 2);
    cout << "       ";
    gotoxy(eX, eY + 3);
    cout << "       ";
}

void moveEnemy() {
    eraseEnemy();
    eY++;
    if (eY > 21) {
        eY = 2;
        eRoadIndex = rand() % NUM_ROADS;
    }
    printEnemy();
}

void printMaze() {
    cout << "###################################################" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "#                                                 #" << endl;
    cout << "###################################################" << endl;
}

void scrollRoads() {
    for (int y = 1; y <= 25; y++) {
        for (int i = 0; i < NUM_ROADS - 1; i++) {
            gotoxy(linePositions[i], y);
            cout << " ";
        }
    }

    roadOffset = (roadOffset + 1) % 2;

    for (int y = 2; y <= 24; y++) {
        if ((y + roadOffset) % 2 == 0) {
            for (int i = 0; i < NUM_ROADS - 1; i++) {
                gotoxy(linePositions[i], y);
                cout << "|";
            }
        }
    }
}

void generateBullet() {
    bulletX[bulletCount] = pX + 3;
    bulletY[bulletCount] = pY - 1;
    isBulletActive[bulletCount] = true;
    printBullet(bulletX[bulletCount], bulletY[bulletCount]);
    bulletCount++;
    if (bulletCount >= BULLET_MAX) {
        bulletCount = 0;
    }
}

void moveBullets() {
    for (int i = 0; i < BULLET_MAX; i++) {
        if (isBulletActive[i]) {
            eraseBullet(bulletX[i], bulletY[i]);
            bulletY[i]--;
            if (bulletY[i] <= 1) {
                isBulletActive[i] = false;
            } else {
                printBullet(bulletX[i], bulletY[i]);
            }
        }
    }
}

void printBullet(int x, int y) {
    gotoxy(x, y);
    cout << "*";
}

void eraseBullet(int x, int y) {
    gotoxy(x, y);
    cout << " ";
}

void bulletCollisionWithEnemy() {
    for (int i = 0; i < BULLET_MAX; i++) {
        if (isBulletActive[i]) {
            if ((bulletX[i] >= eX) && (bulletX[i] <= eX + enemyWidth) &&
                (bulletY[i] >= eY) && (bulletY[i] <= eY + enemyHeight)) {
                isBulletActive[i] = false;
                eraseBullet(bulletX[i], bulletY[i]);
                eraseEnemy();
                eY = 2;
                eRoadIndex = rand() % NUM_ROADS;
                addScore();
            }
        }
    }
}

void checkCollisionWithPlayer() {
    if (eRoadIndex == pRoadIndex &&
        eY + enemyHeight >= pY && eY <= pY + playerHeight) {
        gameOver();
    }
}

void addScore() {
    score = score + 10;
}

void printScore() {
    gotoxy(mazeWidth + 2, 1);
    cout << "Score: " << score << "   ";
}

void gameOver() {
    system("cls");  
      cout << "Mutaf, your Game is Over! Your score is: " << score << endl;
    exit(0);
} 
