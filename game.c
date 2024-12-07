#include "game.h"

// Game state variables
int windowWidth = 800, windowHeight = 600;
int CARPosX, CARPosY;
int enemyX[3], enemyY[3], enemyFlag[3], spawnDelay[3];
int score = 0;
int level = 1;
int speed = 3;
int highScore = 0;
int paused = 0;

// Function to load the high score from the file
int loadHighScore() {
    FILE *file = fopen(HIGH_SCORE_FILE, "r");
    int highScore = 0;
    if (file) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
    return highScore;
}

// Function to save the high score to the file
void saveHighScore(int highScore) {
    FILE *file = fopen(HIGH_SCORE_FILE, "w");
    if (file) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}

// Function to reset the enemy's position
void resetEnemy(int ind) {
    enemyY[ind] = windowHeight; // Spawn at the top of the screen
    enemyX[ind] = rand() % (ROAD_WIDTH - ENEMY_WIDTH) + (windowWidth - ROAD_WIDTH) / 2;
    spawnDelay[ind] = rand() % 50+ 50;
}

// Initialize the game
void initGame() {
    srand(time(0));
    highScore = loadHighScore(); // Load the high score
    CARPosX = windowWidth / 2 - CAR_WIDTH / 2;
    CARPosY = 100;

    for (int i = 0; i < 3; i++) {
        enemyFlag[i] = 0;
        resetEnemy(i);
    }
    enemyFlag[0] = 1;
}

// Check for collision between motorcycle and enemy
int collision() {
    for (int i = 0; i < 3; i++) {
        if (enemyFlag[i] &&
            enemyY[i] + ENEMY_HEIGHT >= CARPosY &&
            enemyY[i] <= CARPosY + CAR_HEIGHT &&
            enemyX[i] + ENEMY_WIDTH >= CARPosX &&
            enemyX[i] <= CARPosX + CAR_WIDTH) {
            return 1;
        }
    }
    return 0;
}

// Function to draw a rectangle (used for body)
void drawRect(int x, int y, int width, int height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Function to draw a circle (used for wheels)
void drawCircle(int x, int y, int radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);  // Center of circle
    for (int i = 0; i <= 360; i++) {
        float theta = i * (3.14159 / 180);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

// Function to draw an enemy car like the uploaded image
void drawCar(int x, int y) {
    // Main body of the car (blue)
    drawRect(x + 10, y, ENEMY_WIDTH - 20, ENEMY_HEIGHT, 0.0f, 0.0f, 1.0f);  // Blue color (R=0, G=0, B=1)

    // Hood and roof highlight (lighter blue overlay)
    drawRect(x + 20, y + ENEMY_HEIGHT / 2, ENEMY_WIDTH - 40, ENEMY_HEIGHT / 4, 0.5f, 0.5f, 1.0f); // Lighter blue (R=0.5, G=0.5, B=1)

    // Windshield (black)
    drawRect(x + ENEMY_WIDTH / 4, y + 3 * ENEMY_HEIGHT / 4, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 4, 0.0f, 0.0f, 0.0f);

    // Side mirrors (red rectangles on the sides)
    drawRect(x - 5, y + ENEMY_HEIGHT / 2, 5, 10, 1.0f, 0.0f, 0.0f);           // Left mirror (red)
    drawRect(x + ENEMY_WIDTH, y + ENEMY_HEIGHT / 2, 5, 10, 1.0f, 0.0f, 0.0f); // Right mirror (red)

    // Wheels (black circles)
    drawCircle(x + 15, y - 10, 10, 0.0f, 0.0f, 0.0f);             // Front left wheel (black)
    drawCircle(x + ENEMY_WIDTH - 15, y - 10, 10, 0.0f, 0.0f, 0.0f); // Front right wheel (black)
}



// Function to draw an enemy car like the uploaded image
void drawEnemyCar(int x, int y) {
    // Main body of the car (red)
    drawRect(x + 10, y, ENEMY_WIDTH - 20, ENEMY_HEIGHT, 1.0f, 0.0f, 0.0f);

    // Hood and roof highlight (lighter red overlay)
    drawRect(x + 20, y + ENEMY_HEIGHT / 2, ENEMY_WIDTH - 40, ENEMY_HEIGHT / 4, 1.0f, 0.5f, 0.5f);

    // Windshield (black)
    drawRect(x + ENEMY_WIDTH / 4, y + 3 * ENEMY_HEIGHT / 4, ENEMY_WIDTH / 2, ENEMY_HEIGHT / 4, 0.0f, 0.0f, 0.0f);

    // Side mirrors (red rectangles on the sides)
    drawRect(x - 5, y + ENEMY_HEIGHT / 2, 5, 10, 1.0f, 0.0f, 0.0f);           // Left mirror
    drawRect(x + ENEMY_WIDTH, y + ENEMY_HEIGHT / 2, 5, 10, 1.0f, 0.0f, 0.0f); // Right mirror

    // Wheels (black circles)
    drawCircle(x + 15, y - 10, 10, 0.0f, 0.0f, 0.0f);             // Front left wheel
    drawCircle(x + ENEMY_WIDTH - 15, y - 10, 10, 0.0f, 0.0f, 0.0f); // Front right wheel
}


// Game state variables
int isGameOver = 0;  // Flag to indicate the game is over

// Function to reset the game state
void resetGame() {
    isGameOver = 0;
    score = 0;
    level = 1;
    speed = 5;
    CARPosX = windowWidth / 2 - CAR_WIDTH / 2;
    CARPosY = 100;

    for (int i = 0; i < 3; i++) {
        enemyFlag[i] = 0;
        resetEnemy(i);
    }
    enemyFlag[0] = 1;
}

// Function to update high score
void updateHighScore() {
    if (score > highScore) {
        highScore = score;
        saveHighScore(highScore);
    }
}
// Function to render text on the screen
void renderText(float x, float y, const char* text, void* font, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Display callback for rendering
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (!isGameOver) {
        // Draw the road (dark gray)
        drawRect((windowWidth - ROAD_WIDTH) / 2, 0, ROAD_WIDTH, windowHeight, 0.1f, 0.1f, 0.1f);

        // Draw the player's motorcycle (blue)
        drawCar(CARPosX, CARPosY);

        // Draw enemies
        for (int i = 0; i < 3; i++) {
            if (enemyFlag[i]) {
                drawEnemyCar(enemyX[i], enemyY[i]);
            }
        }

        // Display score and level
        char buffer[50];
        sprintf(buffer, "Score: %d  Level: %d  High Score: %d", score, level, highScore);
        renderText(10, windowHeight - 30, buffer, GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f);
    } else {
        // Game over screen
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

// Game update logic (e.g., move enemies, check collisions)
void update(int value) {
    if (!isGameOver && !paused) {
        for (int i = 0; i < 3; i++) {
            if (enemyFlag[i]) {
                enemyY[i] -= speed;  // Move the enemy downward

                // Reset enemy if it goes off the screen
                if (enemyY[i] < 0) {
                    resetEnemy(i);
                    score++;

                    // Increase level and speed every 10 points
                    if (score % 10 == 0) {
                        level++;
                        speed += 2;
                    }
                }
            } else if (--spawnDelay[i] <= 0) {
                enemyFlag[i] = 1;  // Activate the enemy after the spawn delay
            }
        }

        if (collision()) {
            isGameOver = 1;  // Set game-over flag
             updateHighScore();
        }

        glutPostRedisplay();
    }
    glutTimerFunc(30, update, 0);
}

// Handle special key press events (arrow keys)
void handleKeys(int key, int x, int y) {
    if (!isGameOver && !paused ) {
        if (key == GLUT_KEY_LEFT && CARPosX > (windowWidth - ROAD_WIDTH) / 2) {
            CARPosX -= 20;
        }
        if (key == GLUT_KEY_RIGHT && CARPosX < (windowWidth + ROAD_WIDTH) / 2 - CAR_WIDTH) {
            CARPosX += 20;
        }
    }
}

// Handle normal key press events (e.g., restart or exit)
void handleNormalKeys(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key Ascii
        exit(0);
    }
      if (key == 'p' || key == 'P') {
        paused = !paused;
    }
    if (key == 'r' || key == 'R') { // R key to restart
        resetGame();
    }
}

// Main function to set up OpenGL
void play(int argc, char** argv) {
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("CAR Game - OpenGL");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    initGame();

    glutDisplayFunc(display);
    glutSpecialFunc(handleKeys);
    glutKeyboardFunc(handleNormalKeys);
    glutTimerFunc(30, update, 0);

    glutMainLoop();
}
