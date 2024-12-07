#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <GL/glut.h> //for opengl library
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#define ROAD_WIDTH 400
#define CAR_WIDTH 50
#define CAR_HEIGHT 30
#define ENEMY_WIDTH 60
#define ENEMY_HEIGHT 40
#define HIGH_SCORE_FILE "highscore.txt"


// Function declarations
int loadHighScore();
void saveHighScore(int highScore);
void resetEnemy(int ind);
void initGame();
int collision();
void drawRect(int x, int y, int width, int height, float r, float g, float b);
void drawCircle(int x, int y, int radius, float r, float g, float b);
void drawCar(int x, int y);
void drawEnemyCar(int x, int y);
void resetGame();
void updateHighScore();
void renderText(float x, float y, const char* text, void* font, float r, float g, float b);
void display();
void update(int value);
void handleKeys(int key, int x, int y);
void handleNormalKeys(unsigned char key, int x, int y);
void play();

#endif // GAME_H
