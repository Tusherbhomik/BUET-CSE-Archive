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

void drawTriangle()
{
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex2f(0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex2f(0.0f, 0.5f);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawTriangle();
    glutSwapBuffers();
}

void init()
{   
    //explain the  4th parameter of glClearColor
    // The fourth parameter of glClearColor specifies the alpha value for the clear color.
    // It determines the transparency of the cleared area. A value of 1.0 means fully opaque,
    // while a value of 0.0 means fully transparent. In this case, we set it to 1.0 to ensure
    // the background is fully opaque.
    // what does  opaque mean
    // Opaque means that an object does not allow light to pass through it,
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);// Black background with full transparency
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D effects
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(300, 200);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Triangle");

    glutDisplayFunc(display);
    init();
    glutMainLoop();

    return 0;
}
