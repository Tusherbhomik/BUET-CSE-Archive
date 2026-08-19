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

float angle = 0.0f;  

void drawTriangle() {
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); 
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f); 
        glVertex3f(1.0f, -1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f); 
        glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up the camera
    gluLookAt(0.0, 0.0, 5.0,   // Eye position
              0.0, 0.0, 0.0,   // Look at center
              0.0, 1.0, 0.0);  // Up vector

    // Apply rotation
    glRotatef(angle, 0.0f, 1.0f, 0.0f);  // Rotate around Y-axis

    drawTriangle();
    glutSwapBuffers();

}

void timer(int value) {
    angle += 1.0f;  // Increment rotation angle
    glutPostRedisplay();  // Request redraw
    glutTimerFunc(16, timer, 0);  // ~60 FPS
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;  // Prevent division by zero
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);  // 3D perspective projection
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glDisable(GL_LIGHTING);  // Disable lighting to use vertex colors
    glEnable(GL_DEPTH_TEST);  // Enable depth testing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Rotated Triangle");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);  // Register reshape function
    glutTimerFunc(0, timer, 0);  // Start animation
    init();
    glutPostRedisplay();  // Force initial redraw
    glutMainLoop();

    return 0;
}