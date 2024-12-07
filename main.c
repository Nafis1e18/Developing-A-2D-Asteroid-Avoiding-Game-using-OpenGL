#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include<math.h>

#define Space_width 400
#define Rocket_Width 50
#define Rocket_Height 30
#define Astroid_Width 60
#define Astroid_Height 40
#define HIGH_SCORE_FILE "highscore.txt"


int windowWidth = 800, windowHeight = 600;
int AstroidPosX, AstroidPosY;
int astroidX[2], astroidY[2], astroidFlag[2], spawnDelay[2];
int score = 0;
int level = 1;
int speed = 3;
int highScore = 0;
int paused = 0;


int loadHighScore() {
    FILE *file = fopen(HIGH_SCORE_FILE, "r");
    int highScore = 0;
    if (file) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
    return highScore;
}


void saveHighScore(int highScore) {
    FILE *file = fopen(HIGH_SCORE_FILE, "w");
    if (file) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}
void resetAstroid(int ind) {

    int newX, newY;
    int overlap;

    do {
        newY = windowHeight;
        newX = rand() % (Space_width - Astroid_Width) + (windowWidth - Space_width) / 2;


        overlap = 0;
        for (int i = 0; i < 2; i++) {
            if (i != ind && astroidFlag[i]) {

                if (!(newX + Astroid_Width < astroidX[i] || newX > astroidX[i] + Astroid_Width || newY + Astroid_Height < astroidY[i] || newY > astroidY[i] + Astroid_Height)) {
                    overlap = 1;
                    break;
                }
            }
        }
    } while (overlap);

    astroidX[ind] = newX;
    astroidY[ind] = newY;
    spawnDelay[ind] = rand() % 50 + 50;
}


void initGame() {
    srand(time(0));
    highScore = loadHighScore();
    AstroidPosX = windowWidth / 2 - Rocket_Width / 2;
    AstroidPosY = 100;

    for (int i = 0; i < 2; i++) {
        astroidFlag[i] = 0;
        resetAstroid(i);
    }
    astroidFlag[0] = 1;
}


int collision() {
    for (int i = 0; i < 2; i++) {
        if (astroidFlag[i] &&
            astroidY[i] + Astroid_Height >= AstroidPosY &&
            astroidY[i] <= AstroidPosY + Rocket_Height &&
            astroidX[i] + Astroid_Width >= AstroidPosX &&
            astroidX[i] <= AstroidPosX + Rocket_Width) {
            return 1;
        }
    }
    return 0;
}


void drawRect(int x, int y, int width, int height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}


void drawRocket(int x, int y) {

    glColor3f(0.8f, 0.8f, 0.8f);
    drawRect(x + 10, y, Astroid_Width - 20, Astroid_Height, 0.8f, 0.8f, 0.8f);


    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 10, y + Astroid_Height);
    glVertex2f(x + Astroid_Width - 10, y + Astroid_Height);
    glVertex2f(x + Astroid_Width / 2, y + Astroid_Height + 30);
    glEnd();


    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);

    glVertex2f(x, y);
    glVertex2f(x + 10, y + 20);
    glVertex2f(x + 10, y);
    glEnd();

    glBegin(GL_TRIANGLES);

    glVertex2f(x + Astroid_Width, y);
    glVertex2f(x + Astroid_Width - 10, y + 20);
    glVertex2f(x + Astroid_Width - 10, y);
    glEnd();


    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + Astroid_Width / 2 - 10, y);
    glVertex2f(x + Astroid_Width / 2 + 10, y);
    glVertex2f(x + Astroid_Width / 2, y - 30);
    glEnd();
}
void drawCircle(int x, int y, int radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++) {
        float theta = i * (3.14159 / 180);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}
void drawAstroid(int x, int y) {

    drawCircle(x + Astroid_Width / 2, y + Astroid_Height / 2, Astroid_Width / 2, 0.5f, 0.5f, 0.5f);
    drawCircle(x + 20, y + 30, 10, 0.4f, 0.4f, 0.4f);
    drawCircle(x + Astroid_Width - 20, y + 50, 15, 0.6f, 0.6f, 0.6f);


    drawCircle(x + 25, y + 40, 5, 0.3f, 0.3f, 0.3f);
    drawCircle(x + 50, y + 20, 8, 0.2f, 0.2f, 0.2f);
    drawCircle(x + Astroid_Width - 30, y + 60, 7, 0.2f, 0.2f, 0.2f);
}



int isGameOver = 0;

void resetGame() {
    isGameOver = 0;
    score = 0;
    level = 1;
    speed = 5;
    AstroidPosX = windowWidth / 2 - Rocket_Width / 2;
    AstroidPosY = 100;

    for (int i = 0; i < 2; i++) {
        astroidFlag[i] = 0;
        resetAstroid(i);
    }
    astroidFlag[0] = 1;
}


void updateHighScore() {
    if (score > highScore) {
        highScore = score;
        saveHighScore(highScore);
    }
}

void renderText(float x, float y, const char* text, void* font, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!isGameOver) {

        drawRect((windowWidth - Space_width) / 2, 0, Space_width, windowHeight, 0.1f, 0.1f, 0.1f);

        drawRocket(AstroidPosX, AstroidPosY);

        for (int i = 0; i < 2; i++) {
            if (astroidFlag[i]) {
                drawAstroid(astroidX[i], astroidY[i]);
            }
        }


        char buffer[50];
        sprintf(buffer, "Score: %d  Level: %d  High Score: %d", score, level, highScore);
        renderText(10, windowHeight - 30, buffer, GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);
        int marginX = (windowWidth + Space_width) / 2 + 20;
        renderText(marginX, windowHeight - 30, "1.Press < to move Left", GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);
        renderText(marginX, windowHeight - 60, "2.Press > to move Right", GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);
        renderText(marginX, windowHeight - 90, "3.Press P to pause and Resume", GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);

    } else {

        char gameOverText[50];
        sprintf(gameOverText, "GAME OVER! Final Score: %d", score);
        renderText(windowWidth / 2 - 120, windowHeight / 2 + 20, gameOverText, GLUT_BITMAP_TIMES_ROMAN_24, 1.0f, 0.0f, 0.0f);

        char highScoreText[50];
        sprintf(highScoreText, "High Score: %d", highScore);
        renderText(windowWidth / 2 - 80, windowHeight / 2 - 10, highScoreText, GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);

        renderText(windowWidth / 2 - 80, windowHeight / 2 - 40, "Press R to Restart", GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);
        renderText(windowWidth / 2 - 80, windowHeight / 2 - 70, "Press ESC to Exit", GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);
    }

    glutSwapBuffers();
}


void update(int value) {
    if (!isGameOver && !paused) {
        for (int i = 0; i < 2; i++) {
            if (astroidFlag[i]) {
                astroidY[i] -= speed;


                if (astroidY[i] < 0) {
                    resetAstroid(i);
                    score++;


                    if (score % 10 == 0) {
                        level++;
                        speed += 2;
                    }
                }
            } else if (--spawnDelay[i] <= 0) {
                astroidFlag[i] = 1;
            }
        }

        if (collision()) {
            isGameOver = 1;
             updateHighScore();
        }

        glutPostRedisplay();
    }
    glutTimerFunc(30, update, 0);
}


void handleKeys(int key, int x, int y) {
    if (!isGameOver && !paused ) {
        if (key == GLUT_KEY_LEFT && AstroidPosX > (windowWidth - Space_width) / 2) {
            AstroidPosX -= 20;
        }
        if (key == GLUT_KEY_RIGHT && AstroidPosX < (windowWidth + Space_width) / 2 - Rocket_Width) {
            AstroidPosX += 20;
        }
    }
}


void handleNormalKeys(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }
      if (key == 'p' || key == 'P') {
        paused = !paused;
    }
    if (key == 'r' || key == 'R') {
        resetGame();
    }
}


int main(int argc, char** argv) {
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Project using OpenGL");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    initGame();

    glutDisplayFunc(display);
    glutSpecialFunc(handleKeys);
    glutKeyboardFunc(handleNormalKeys);
    glutTimerFunc(30, update, 0);

    glutMainLoop();

    return 0;
}
