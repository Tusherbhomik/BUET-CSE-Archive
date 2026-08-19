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
#include <ctime>
#include <chrono>


const float PI = 3.14159265358979323846;
void  init();
void display();
float degToRad(float deg);
void  drawCircle(float radius);
void drawHand(float length, float angleDeg, float width);
void timerFunction(int value);

float degToRad(float deg) {
    return deg * PI / 180.0f;
}
void drawCircle(float radius) {
    glColor3f(1.0f, 1.0f, 1.0f); 
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float angle = degToRad(i);
        glVertex2f(cos(angle) * radius, sin(angle) * radius);
    }
    glEnd();

    for (int i = 0; i < 60; i++) {
        float angle = degToRad(i * 6); 

        float outerX = cos(angle) * (radius-0.03f);
        float outerY = sin(angle) * (radius-0.03f);

        float innerRadius = (i % 5 == 0) ? (radius-0.03f) * 0.80f : (radius-0.03f) * 0.90f;
        float innerX = cos(angle) * innerRadius;
        float innerY = sin(angle) * innerRadius;

        glBegin(GL_LINES);
        glVertex2f(innerX, innerY);
        glVertex2f(outerX, outerY);
        glEnd();
    }
}
void drawHand(float length, float angleDeg, float width,float radius) {
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(0, 0); 
    glVertex2f(length * cos(degToRad(angleDeg)),length * sin(degToRad(angleDeg)));
    glEnd();
    float dxdy = 0.02f;
    glBegin(GL_QUADS);
    {
        glVertex2f( radius*cos(degToRad(angleDeg))+dxdy, radius*sin(degToRad(angleDeg))+dxdy);   
        glVertex2f( radius*cos(degToRad(angleDeg))+dxdy, radius*sin(degToRad(angleDeg))-dxdy);  
        glVertex2f( radius*cos(degToRad(angleDeg))-dxdy, radius*sin(degToRad(angleDeg))-dxdy); 
        glVertex2f( radius*cos(degToRad(angleDeg))-dxdy, radius*sin(degToRad(angleDeg))+dxdy); 
    }
    glEnd();
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    float radius = 0.5f;
    drawCircle(radius); 

    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    time_t t = millis / 1000;
    struct tm* local = localtime(&t);

    float ms_part = millis % 1000;
    float seconds = local->tm_sec + ms_part / 1000.0f;
    float minutes = local->tm_min + seconds / 60.0f;
    float hours = (local->tm_hour % 12) + minutes / 60.0f;

    float secAngle = 90.0f - seconds*6.0f;
    float minAngle = 90.0f - minutes * 6.0f;
    float hourAngle = 90.0f - hours * 30.0f;

    glColor3f(1.0f, 1.0f, 1.0f);  drawHand(0.25f, hourAngle, 3.5f,radius);
    glColor3f(1.0f, 1.0f, 1.0f);   drawHand(0.3f, minAngle, 2.5f,radius);
    glColor3f(1.0f, 0.0f, 0.0f);   drawHand(0.4f, secAngle, 2.0f,radius);

    glutSwapBuffers();
}

void timerFunction(int value) {
    glutPostRedisplay();    
    glutTimerFunc(16, timerFunction, 0); 
}
void init(){
    printf("Initialization complete:\n");
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("----------------------------------------\n");
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}



int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("2D Real-time clock ");

    glutDisplayFunc(display);
    
    glutTimerFunc(0,timerFunction,0);
    init();
    printf("Starting GLUT main loop...\n");
    glutMainLoop();

    return 0;
}