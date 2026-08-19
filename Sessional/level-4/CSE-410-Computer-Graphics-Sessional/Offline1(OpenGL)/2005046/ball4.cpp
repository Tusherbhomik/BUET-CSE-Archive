#ifdef __linux__
#include <GL/glut.h>
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <GL/glut.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "_vector.h"

const float ROOM_SIZE = 2.5f;
Vector camera(-1.0f, 1.0f, 0.0f);
Vector look(1.0f, 1.0f, 0.0f);
Vector up(0.0f, 1.0f, 0.0f);

const float CENTER_X = 0.0f;
const float CENTER_Y = 2.5f;
const float CENTER_Z = 0.0f;
bool useFixedViewpoint = false;

const float MOVE_SPEED = 0.1f;
const float ROTATE_SPEED = 2.0f;
const float WALL_PADDING = 0.3f;

const float BALL_RADIUS = 0.15f;
float ballX = 0.0f, ballY = BALL_RADIUS, ballZ = 0.0f;
float ballVelX = 0.0f, ballVelY = 0.0f, ballVelZ = 0.0f;
float ballRotX = 0.0f, ballRotY = 0.0f, ballRotZ = 0.0f;
float ballAxisX = 1.0f, ballAxisY = 0.0f, ballAxisZ = 0.0f;
float ballAngle = 0.0f;

const float GRAVITY = 9.8f;
const float RESTITUTION = 0.8f;
const float MIN_VELOCITY = 0.01f;
float initialSpeed = 2.0f;
bool simulationRunning = false;
bool showVelocityArrow = true;
int lastTime = 0;
bool showLookAtDot = false;

void init();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void drawRoom();
void drawBall();
void drawVelocityArrow();
void update(int value);
void resetBall();

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    float lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float lightPosition[] = {2.0f, 4.0f, 2.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    float matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float matShininess = 100.0f;
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);

    srand(static_cast<unsigned int>(time(NULL)));

    resetBall();
}

void drawLookAtDot()
{
    glDisable(GL_LIGHTING);

    float dotSize = 0.08f;
    float dotDistance = 2.0f;

    Vector dot;
    if (useFixedViewpoint)
    {
        dot = Vector(CENTER_X, CENTER_Y, CENTER_Z) + look.normalize() * dotDistance;
    }
    else
    {
        dot = camera + look.normalize() * dotDistance;
    }

    glPushMatrix();
    glTranslatef(dot.x, dot.y, dot.z);
    glColor3f(1.0f, 0.0f, 0.0f);

    GLUquadricObj *quadric = gluNewQuadric();
    gluSphere(quadric, dotSize, 16, 16);

    glLineWidth(2.0f);

    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-dotSize * 1.5f, 0.0f, 0.0f);
    glVertex3f(dotSize * 1.5f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0.0f, -dotSize * 1.5f, 0.0f);
    glVertex3f(0.0f, dotSize * 1.5f, 0.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -dotSize * 1.5f);
    glVertex3f(0.0f, 0.0f, dotSize * 1.5f);
    glEnd();

    gluDeleteQuadric(quadric);
    glPopMatrix();

    glLineWidth(1.0f);
    glColor3f(0.8f, 0.8f, 0.0f);
    glBegin(GL_LINES);
    if (useFixedViewpoint)
    {
        glVertex3f(CENTER_X, CENTER_Y, CENTER_Z);
    }
    else
    {
        glVertex3f(camera.x, camera.y, camera.z);
    }
    glVertex3f(dot.x, dot.y, dot.z);
    glEnd();

    glEnable(GL_LIGHTING);
}

bool checkCollision(float x, float y, float z)
{
    return (abs(x) > ROOM_SIZE - WALL_PADDING || abs(z) > ROOM_SIZE - WALL_PADDING);
}

float randomFloat(float min, float max)
{
    return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Vector eye = useFixedViewpoint ? Vector(CENTER_X, CENTER_Y, CENTER_Z) : camera;
    Vector center = eye + look;

    gluLookAt(
        eye.x, eye.y, eye.z,
        center.x, center.y, center.z,
        up.x, up.y, up.z);

    drawLookAtDot();
    drawRoom();
    drawBall();

    if (showVelocityArrow)
    {
        drawVelocityArrow();
    }

    glFlush();
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    if (height == 0)
        height = 1;
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0f, aspectRatio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawRoom()
{
    glDisable(GL_LIGHTING);

    glBegin(GL_QUADS);

    int tiles = 15;
    float tileSize = (2.0f * ROOM_SIZE) / tiles;

    for (int i = 0; i < tiles; ++i)
    {
        for (int j = 0; j < tiles; ++j)
        {
            if ((i + j) % 2 == 0)
                glColor3f(0.9f, 0.9f, 0.9f);
            else
                glColor3f(0.2f, 0.2f, 0.2f);

            float xStart = -ROOM_SIZE + i * tileSize;
            float zStart = -ROOM_SIZE + j * tileSize;

            Vector p1(xStart, 0.0f, zStart);
            Vector p2 = p1 + Vector(tileSize, 0.0f, 0.0f);
            Vector p3 = p2 + Vector(0.0f, 0.0f, tileSize);
            Vector p4 = p1 + Vector(0.0f, 0.0f, tileSize);

            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p3.x, p3.y, p3.z);
            glVertex3f(p4.x, p4.y, p4.z);
        }
    }

    glColor3f(0.95f, 0.95f, 0.95f);
    Vector ceiling1(-ROOM_SIZE, ROOM_SIZE, -ROOM_SIZE);
    Vector ceiling2(ROOM_SIZE, ROOM_SIZE, -ROOM_SIZE);
    Vector ceiling3(ROOM_SIZE, ROOM_SIZE, ROOM_SIZE);
    Vector ceiling4(-ROOM_SIZE, ROOM_SIZE, ROOM_SIZE);

    glVertex3f(ceiling1.x, ceiling1.y, ceiling1.z);
    glVertex3f(ceiling2.x, ceiling2.y, ceiling2.z);
    glVertex3f(ceiling3.x, ceiling3.y, ceiling3.z);
    glVertex3f(ceiling4.x, ceiling4.y, ceiling4.z);

    glColor3f(0.6f, 0.375f, 0.375f);
    glVertex3f(-ROOM_SIZE, 0.0f, ROOM_SIZE);
    glVertex3f(ROOM_SIZE, 0.0f, ROOM_SIZE);
    glVertex3f(ROOM_SIZE, ROOM_SIZE, ROOM_SIZE);
    glVertex3f(-ROOM_SIZE, ROOM_SIZE, ROOM_SIZE);

    glColor3f(0.375f, 0.6f, 0.375f);
    glVertex3f(ROOM_SIZE, 0.0f, -ROOM_SIZE);
    glVertex3f(-ROOM_SIZE, 0.0f, -ROOM_SIZE);
    glVertex3f(-ROOM_SIZE, ROOM_SIZE, -ROOM_SIZE);
    glVertex3f(ROOM_SIZE, ROOM_SIZE, -ROOM_SIZE);

    glColor3f(0.375f, 0.375f, 0.6f);
    glVertex3f(-ROOM_SIZE, 0.0f, -ROOM_SIZE);
    glVertex3f(-ROOM_SIZE, 0.0f, ROOM_SIZE);
    glVertex3f(-ROOM_SIZE, ROOM_SIZE, ROOM_SIZE);
    glVertex3f(-ROOM_SIZE, ROOM_SIZE, -ROOM_SIZE);

    glColor3f(0.8f, 0.8f, 0.5f);
    glVertex3f(ROOM_SIZE, 0.0f, ROOM_SIZE);
    glVertex3f(ROOM_SIZE, 0.0f, -ROOM_SIZE);
    glVertex3f(ROOM_SIZE, ROOM_SIZE, -ROOM_SIZE);
    glVertex3f(ROOM_SIZE, ROOM_SIZE, ROOM_SIZE);

    glEnd();

    glEnable(GL_LIGHTING);
}

void drawBall()
{
    glPushMatrix();

    Vector ballPos(ballX, ballY, ballZ);
    glTranslatef(ballPos.x, ballPos.y, ballPos.z);

    Vector ballAxis(ballAxisX, ballAxisY, ballAxisZ);
    glRotatef(ballAngle, ballAxis.x, ballAxis.y, ballAxis.z);

    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GLU_TRUE);

    float matAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    float matDiffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float matShininess = 100.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

    int segments = 32;
    int stripes = 25;
    float stripWidth = 180.0f / stripes;

    for (int i = 0; i < stripes; i++)
    {
        float startAngle = i * stripWidth;
        float endAngle = (i + 1) * stripWidth;

        if (i % 2 == 0)
            glColor3f(1.0f, 0.0f, 0.0f);
        else
            glColor3f(0.0f, 0.8f, 0.0f);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= segments; j++)
        {
            float longitude = (360.0f * j) / segments;

            auto sphericalToCartesian = [](float latDeg, float longDeg)
            {
                float lat = latDeg * M_PI / 180.0f;
                float lon = longDeg * M_PI / 180.0f;
                return Vector(
                    sin(lat) * cos(lon),
                    cos(lat),
                    sin(lat) * sin(lon));
            };

            Vector p1 = sphericalToCartesian(startAngle, longitude);
            Vector p2 = sphericalToCartesian(endAngle, longitude);

            glNormal3f(p1.x, p1.y, p1.z);
            glVertex3f(BALL_RADIUS * p1.x, BALL_RADIUS * p1.y, BALL_RADIUS * p1.z);

            glNormal3f(p2.x, p2.y, p2.z);
            glVertex3f(BALL_RADIUS * p2.x, BALL_RADIUS * p2.y, BALL_RADIUS * p2.z);
        }
        glEnd();
    }

    gluDeleteQuadric(quadric);
    glPopMatrix();
}

void drawVelocityArrow()
{
    Vector velocity(ballVelX, ballVelY, ballVelZ);
    float velMagnitude = velocity.magnitude();
    if (velMagnitude < MIN_VELOCITY)
        return;

    glDisable(GL_LIGHTING);

    Vector direction = velocity.normalize();
    float arrowLength = std::min(velMagnitude * 0.5f, ROOM_SIZE / 2.0f);

    Vector start(ballX, ballY, ballZ);
    Vector end = start + direction * arrowLength;

    glLineWidth(3.0f);
    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();

    float headSize = 0.1f;

    Vector up;
    if (fabs(direction.x) < 0.9f && fabs(direction.y) < 0.9f)
    {
        up = Vector(0.0f, 0.0f, 1.0f);
    }
    else
    {
        up = Vector(0.0f, 1.0f, 0.0f);
    }

    Vector perp1 = direction.cross(up).normalize();
    Vector perp2 = direction.cross(perp1).normalize();

    Vector arrowBack = end - direction * (arrowLength * 0.2f);

    glBegin(GL_TRIANGLES);
    glVertex3f(end.x, end.y, end.z);
    glVertex3f((arrowBack + perp1 * headSize).x, (arrowBack + perp1 * headSize).y, (arrowBack + perp1 * headSize).z);
    glVertex3f((arrowBack - perp1 * headSize).x, (arrowBack - perp1 * headSize).y, (arrowBack - perp1 * headSize).z);

    glVertex3f(end.x, end.y, end.z);
    glVertex3f((arrowBack + perp2 * headSize).x, (arrowBack + perp2 * headSize).y, (arrowBack + perp2 * headSize).z);
    glVertex3f((arrowBack - perp2 * headSize).x, (arrowBack - perp2 * headSize).y, (arrowBack - perp2 * headSize).z);
    glEnd();

    glEnable(GL_LIGHTING);
}

void resetBall()
{
    Vector position(
        randomFloat(-ROOM_SIZE + BALL_RADIUS * 2, ROOM_SIZE - BALL_RADIUS * 2),
        BALL_RADIUS,
        randomFloat(-ROOM_SIZE + BALL_RADIUS * 2, ROOM_SIZE - BALL_RADIUS * 2));

    float randomAngle = randomFloat(0, 2 * M_PI);
    Vector velocity(
        initialSpeed * cos(randomAngle),
        initialSpeed * 1.5f,
        initialSpeed * sin(randomAngle));

    ballX = position.x;
    ballY = position.y;
    ballZ = position.z;

    ballVelX = velocity.x;
    ballVelY = velocity.y;
    ballVelZ = velocity.z;

    ballAngle = 0.0f;
    ballAxisX = 1.0f;
    ballAxisY = 0.0f;
    ballAxisZ = 0.0f;

    simulationRunning = false;

    printf("Ball reset. Position: (%.2f, %.2f, %.2f), Initial speed: %.2f\n", ballX, ballY, ballZ, initialSpeed);
    printf("Press SPACE to start simulation.\n");
}

void update(int value)
{
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    if (deltaTime > 0.05f)
        deltaTime = 0.05f;
    lastTime = currentTime;

    if (simulationRunning)
    {
        Vector oldPos(ballX, ballY, ballZ);
        
        ballVelY -= GRAVITY * deltaTime;
        
        ballX += ballVelX * deltaTime;
        ballY += ballVelY * deltaTime;
        ballZ += ballVelZ * deltaTime;
        
        bool collided = false;
        bool onGround = false;
        
        if (ballX + BALL_RADIUS > ROOM_SIZE)
        {
            ballX = ROOM_SIZE - BALL_RADIUS;
            ballVelX = -ballVelX * RESTITUTION;
            collided = true;
        }
        else if (ballX - BALL_RADIUS < -ROOM_SIZE)
        {
            ballX = -ROOM_SIZE + BALL_RADIUS;
            ballVelX = -ballVelX * RESTITUTION;
            collided = true;
        }
        
        if (ballZ + BALL_RADIUS > ROOM_SIZE)
        {
            ballZ = ROOM_SIZE - BALL_RADIUS;
            ballVelZ = -ballVelZ * RESTITUTION;
            collided = true;
        }
        else if (ballZ - BALL_RADIUS < -ROOM_SIZE)
        {
            ballZ = -ROOM_SIZE + BALL_RADIUS;
            ballVelZ = -ballVelZ * RESTITUTION;
            collided = true;
        }
        
        if (ballY + BALL_RADIUS > ROOM_SIZE)
        {
            ballY = ROOM_SIZE - BALL_RADIUS;
            ballVelY = -ballVelY * RESTITUTION;
            collided = true;
        }

        if (ballY - BALL_RADIUS < 0.0f)
        {
            ballY = BALL_RADIUS;
            onGround = true;

            if (fabs(ballVelY) < MIN_VELOCITY * 5)
            {
                ballVelY = 0;
            }
            else
            {
                ballVelY = -ballVelY * RESTITUTION;
                collided = true;
            }

            const float ROLLING_FRICTION = 0.95f; 
            const float STATIC_THRESHOLD = 0.05f; 
            
            ballVelX *= ROLLING_FRICTION;
            ballVelZ *= ROLLING_FRICTION;

            float horizontalVel = sqrt(ballVelX * ballVelX + ballVelZ * ballVelZ);
            if (horizontalVel < STATIC_THRESHOLD && fabs(ballVelY) < MIN_VELOCITY)
            {
                ballVelX = 0;
                ballVelZ = 0;
            }
        }

        if (onGround)
        {
            Vector newPos(ballX, ballY, ballZ);
            Vector displacement = newPos - oldPos;
            Vector horizDisp(displacement.x, 0.0f, displacement.z);
            float dist = horizDisp.magnitude();
            
            if (dist > 0.0001f) 
            {
                ballAxisX = -horizDisp.z / dist;
                ballAxisY = 0;
                ballAxisZ = horizDisp.x / dist;
                float rotationAmount = (dist / (2 * M_PI * BALL_RADIUS)) * 360.0f;
                
                ballAngle += rotationAmount;
                
                while (ballAngle > 360.0f)
                    ballAngle -= 360.0f;
                while (ballAngle < 0.0f)
                    ballAngle += 360.0f;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ' ':
        simulationRunning = !simulationRunning;
        printf("Simulation %s\n", simulationRunning ? "started" : "paused");
        break;

    case 'r':
    case 'R':
        resetBall();
        break;

    case '+':
    case '=':
        if (!simulationRunning)
        {
            initialSpeed = std::min(initialSpeed + 0.5f, 10.0f);
            printf("Initial speed increased to %.1f\n", initialSpeed);

            float currentDir = atan2(ballVelZ, ballVelX);
            ballVelX = cos(currentDir) * initialSpeed;
            ballVelY = 1.5f * initialSpeed;
            ballVelZ = sin(currentDir) * initialSpeed;
        }
        break;

    case '-':
    case '_':
        if (!simulationRunning)
        {
            initialSpeed = std::max(initialSpeed - 0.5f, 0.5f);
            printf("Initial speed decreased to %.1f\n", initialSpeed);
            float currentDir = atan2(ballVelZ, ballVelX);
            ballVelX = cos(currentDir) * initialSpeed;
            ballVelY = 1.5f * initialSpeed;
            ballVelZ = sin(currentDir) * initialSpeed;
        }
        break;

    case 'v':
    case 'V':
        showVelocityArrow = !showVelocityArrow;
        break;

    case 'l':
    case 'L':
        showLookAtDot = !showLookAtDot;
        break;

    case 'c':
    case 'C':
        useFixedViewpoint = !useFixedViewpoint;
        break;

        case '1':
        case '2':
        {
            float radians = (key == '1' ? -ROTATE_SPEED : ROTATE_SPEED) * M_PI / 180.0f;
            Vector right = look.cross(up).normalize();
            Vector newLook;
            newLook.x = look.x * cos(radians) + right.x * sin(radians);
            newLook.y = look.y * cos(radians) + right.y * sin(radians);
            newLook.z = look.z * cos(radians) + right.z * sin(radians);
            
            look = newLook.normalize();
            break;
        }
    case '3':
    case '4':
    {
        float radians = (key == '3' ? ROTATE_SPEED : -ROTATE_SPEED) * M_PI / 180.0f;
        Vector right = look.cross(up).normalize();
        Vector newLook;
        newLook.x = look.x * cos(radians) + look.y * sin(radians);
        newLook.y = -look.x * sin(radians) + look.y * cos(radians);
        newLook.z = look.z;
        if ((key == '3' && newLook.normalize().dot(up) < 0.95f) ||
            (key == '4' && newLook.normalize().dot(up) > -0.95f))
        {
            look = newLook.normalize();
        }
        break;
    }

    case '5':
    case '6':
    {
        float radians = (key == '5' ? -ROTATE_SPEED : ROTATE_SPEED) * M_PI / 180.0f;
        float cosR = cos(radians);
        float sinR = sin(radians);
        Vector right = look.cross(up).normalize();
        Vector newUp = up * cosR + right * sinR;
        up = newUp.normalize();
        break;
    }
    case 'w':
        if (!useFixedViewpoint)
        {
            float newY = camera.y + MOVE_SPEED;
            if (newY < ROOM_SIZE - WALL_PADDING)
                camera.y = newY;
        }
        break;

    case 's':
        if (!useFixedViewpoint)
        {
            float newY = camera.y - MOVE_SPEED;
            if (newY > WALL_PADDING)
                camera.y = newY;
        }
        break;

    case 27: 
        exit(0);
        break;
    }
    look = look.normalize();
    Vector right = look.cross(up).normalize();
    up = right.cross(look).normalize();

    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    if (useFixedViewpoint)
    {
        glutPostRedisplay();
        return;
    }

    Vector lookVec = look;
    Vector upVec = up;
    Vector camPos = camera;

    Vector forward(lookVec.x, 0.0f, lookVec.z);
    if (forward.magnitude() > 0.0001f)
        forward = forward.normalize() * MOVE_SPEED;
    else
        forward = Vector(0, 0, MOVE_SPEED);

    Vector right = lookVec.cross(upVec);
    right.y = 0.0f;
    if (right.magnitude() > 0.0001f)
        right = right.normalize() * MOVE_SPEED;
    else
        right = Vector(MOVE_SPEED, 0, 0);

    Vector newPos = camPos;

    switch (key)
    {
    case GLUT_KEY_UP:
        newPos = newPos + forward;
        break;
    case GLUT_KEY_DOWN:
        newPos = newPos - forward;
        break;
    case GLUT_KEY_LEFT:
        newPos = newPos - right;
        break;
    case GLUT_KEY_RIGHT:
        newPos = newPos + right;
        break;
    case GLUT_KEY_PAGE_UP:
        newPos.y += MOVE_SPEED;
        if (newPos.y >= ROOM_SIZE - WALL_PADDING)
            newPos.y = ROOM_SIZE - WALL_PADDING;
        break;
    case GLUT_KEY_PAGE_DOWN:
        newPos.y -= MOVE_SPEED;
        if (newPos.y <= WALL_PADDING)
            newPos.y = WALL_PADDING;
        break;
    }

    bool collision = false;
    if (newPos.x + WALL_PADDING > ROOM_SIZE)
    {
        newPos.x = ROOM_SIZE - WALL_PADDING;
        collision = true;
    }
    else if (newPos.x - WALL_PADDING < -ROOM_SIZE)
    {
        newPos.x = -ROOM_SIZE + WALL_PADDING;
        collision = true;
    }

    if (newPos.z + WALL_PADDING > ROOM_SIZE)
    {
        newPos.z = ROOM_SIZE - WALL_PADDING;
        collision = true;
    }
    else if (newPos.z - WALL_PADDING < -ROOM_SIZE)
    {
        newPos.z = -ROOM_SIZE + WALL_PADDING;
        collision = true;
    }

    if (!collision)
    {
        camera.x = newPos.x;
        camera.y = newPos.y;
        camera.z = newPos.z;
    }
    else
    {
        if (camera.x + WALL_PADDING <= ROOM_SIZE && camera.x - WALL_PADDING >= -ROOM_SIZE)
            camera.x = newPos.x;
        if (camera.z + WALL_PADDING <= ROOM_SIZE && camera.z - WALL_PADDING >= -ROOM_SIZE)
            camera.z = newPos.z;

        camera.y = newPos.y;
    }

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Bouncing Ball Simulation");
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    lastTime = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}