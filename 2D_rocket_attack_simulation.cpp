#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

// Rocket
float rocketX = 0.0;
float rocketY = -0.8;

// Bullet
float bulletX = 0.0;
float bulletY = -0.8;
bool bulletActive = false;

// Enemy
float enemyX = 0.0;
float enemyY = 0.8;
bool enemyAlive = true;
float enemyDir = 0.01;

// Explosion
bool explode = false;
float explodeSize = 0.0;
int explodeType = 0; // 0 = circle, 1 = square

// Stars
float starX[100], starY[100];

// Smoke
float smokeY = -0.8;

// Draw Circle
void drawCircle(float x, float y, float r)
{
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i < 360; i++)
    {
        float angle = i * 3.1416 / 180;
        glVertex2f(x + cos(angle)*r, y + sin(angle)*r);
    }
    glEnd();
}

// ⭐ Stars
void drawStars()
{
    glColor3f(1,1,1);
    glPointSize(2);
    glBegin(GL_POINTS);
    for(int i=0; i<100; i++)
    {
        glVertex2f(starX[i], starY[i]);
    }
    glEnd();
}

// 💨 Smoke
void drawSmoke()
{
    glColor3f(0.7,0.7,0.7);
    drawCircle(rocketX, smokeY, 0.03);
}

// 🚀 Rocket
void drawRocket()
{

    // Body
    glColor3f(0,1,0);
    glBegin(GL_QUADS);
    glVertex2f(rocketX - 0.03, rocketY - 0.05);
    glVertex2f(rocketX + 0.03, rocketY - 0.05);
    glVertex2f(rocketX + 0.03, rocketY + 0.05);
    glVertex2f(rocketX - 0.03, rocketY + 0.05);
    glEnd();

    // Head
    glBegin(GL_TRIANGLES);
    glVertex2f(rocketX, rocketY + 0.1);
    glVertex2f(rocketX - 0.03, rocketY + 0.05);
    glVertex2f(rocketX + 0.03, rocketY + 0.05);
    glEnd();

    // Flame
    glColor3f(1,0.5,0);
    glBegin(GL_TRIANGLES);
    glVertex2f(rocketX, rocketY - 0.05);
    glVertex2f(rocketX - 0.02, rocketY - 0.1);
    glVertex2f(rocketX + 0.02, rocketY - 0.1);
    glEnd();
}

// 🔫 Bullet
void drawBullet()
{
    if(bulletActive)
    {
        glColor3f(1,1,0);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2f(bulletX, bulletY);
        glVertex2f(bulletX, bulletY + 0.08);
        glEnd();
    }
}

// 🛸 Enemy
void drawEnemy()
{
    if(enemyAlive)
    {
        glColor3f(1,0,0);
        glBegin(GL_QUADS);
        glVertex2f(enemyX - 0.05, enemyY + 0.05);
        glVertex2f(enemyX + 0.05, enemyY + 0.05);
        glVertex2f(enemyX + 0.05, enemyY - 0.05);
        glVertex2f(enemyX - 0.05, enemyY - 0.05);
        glEnd();
    }
}

// 💥 Explosion
void drawExplosion()
{
    if(explode)
    {
        glColor3f(1,0.3,0);

        if(explodeType == 0)
        {
            drawCircle(enemyX, enemyY, explodeSize);
        }
        else
        {
            glBegin(GL_QUADS);
            glVertex2f(enemyX - explodeSize, enemyY + explodeSize);
            glVertex2f(enemyX + explodeSize, enemyY + explodeSize);
            glVertex2f(enemyX + explodeSize, enemyY - explodeSize);
            glVertex2f(enemyX - explodeSize, enemyY - explodeSize);
            glEnd();
        }
    }
}

// Display
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawStars();
    drawSmoke();
    drawRocket();
    drawBullet();
    drawEnemy();
    drawExplosion();

    glutSwapBuffers();
}

// Update
void update(int value)
{

    // Bullet
    if(bulletActive)
    {
        bulletY += 0.03;

        if(enemyAlive &&
                fabs(bulletX - enemyX) < 0.06 &&
                fabs(bulletY - enemyY) < 0.06)
        {

            enemyAlive = false;
            bulletActive = false;
            explode = true;
            explodeSize = 0.02;

            explodeType = rand() % 2; // random shape
        }

        if(bulletY > 1.0)
        {
            bulletActive = false;
        }
    }

    // Explosion grow
    if(explode)
    {
        explodeSize += 0.03;
        if(explodeSize > 0.2)
        {
            explode = false;

            // 🛸 Respawn enemy
            enemyAlive = true;
            enemyX = (rand()%160)/100.0 - 0.8;
        }
    }

    // Enemy move
    if(enemyAlive)
    {
        enemyX += enemyDir;
        if(enemyX > 0.8 || enemyX < -0.8)
            enemyDir = -enemyDir;
    }

    // Smoke move
    smokeY -= 0.01;
    if(smokeY < -1.0)
        smokeY = rocketY;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Keyboard
void keyboard(unsigned char key, int x, int y)
{
    if(key == ' ')
    {
        if(!bulletActive)
        {
            bulletActive = true;
            bulletX = rocketX;
            bulletY = rocketY;
        }
    }
}

// Arrow keys
void specialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT)
        rocketX -= 0.05;
    if(key == GLUT_KEY_RIGHT)
        rocketX += 0.05;
}

// Init
void init()
{
    glClearColor(0,0,0,1);

    // Random stars
    for(int i=0; i<100; i++)
    {
        starX[i] = (rand()%200)/100.0 - 1;
        starY[i] = (rand()%200)/100.0 - 1;
    }
}

// Main
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600,600);
    glutCreateWindow("Rocket Attack Simulation");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
