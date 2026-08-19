#include "2005046_classes.hpp"
#include "bitmap_image.hpp"
#include <algorithm>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "2005046_intersection.cpp"
using namespace std;
vector<Object*> objects;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;

Vector3D eye(100, 100, 100);
Vector3D look(-100, -100, -100);
Vector3D up(0, 0, 1);
Vector3D r, l, u;
Floor* floorObject = nullptr;

int recursion_level = 4;
int imageWidth = 768;
int imageHeight = 768;
int imageCount = 1;

double windowWidth = 500;
double windowHeight = 500;
double viewAngle = 80;


double clamp(double value, double min_val, double max_val) {
    return max(min_val, min(max_val, value));
}


void loadData() {
    ifstream file("scene.txt");
    if (!file.is_open()) {
        cout << "Error: Cannot open scene.txt" << endl;
        return;
    }
    
    file >> recursion_level >> imageWidth;
    imageHeight = imageWidth; 
    
    int num_objects;
    file >> num_objects;
    

    for (int i = 0; i < num_objects; i++) {
        string object_type;
        file >> object_type;
        
        if (object_type == "sphere") {
            double x, y, z, radius;
            double r, g, b;
            double amb, diff, spec, refl;
            int shine;
            
            file >> x >> y >> z >> radius;
            file >> r >> g >> b;
            file >> amb >> diff >> spec >> refl;
            file >> shine;
            
            Sphere* sphere = new Sphere(Vector3D(x, y, z), radius);
            sphere->setColor(r, g, b);
            sphere->setCoEfficients(amb, diff, spec, refl);
            sphere->setShine(shine);
            objects.push_back(sphere);
        }
        else if (object_type == "triangle") {
            double x1, y1, z1, x2, y2, z2, x3, y3, z3;
            double r, g, b;
            double amb, diff, spec, refl;
            int shine;
            
            file >> x1 >> y1 >> z1;
            file >> x2 >> y2 >> z2;
            file >> x3 >> y3 >> z3;
            file >> r >> g >> b;
            file >> amb >> diff >> spec >> refl;
            file >> shine;
            
            Triangle* triangle = new Triangle(Vector3D(x1, y1, z1), 
                                            Vector3D(x2, y2, z2), 
                                            Vector3D(x3, y3, z3));
            triangle->setColor(r, g, b);
            triangle->setCoEfficients(amb, diff, spec, refl);
            triangle->setShine(shine);
            objects.push_back(triangle);
        }
        else if (object_type == "general") {
            double A, B, C, D, E, F, G, H, I, J;
            double ref_x, ref_y, ref_z, len, wid, hei;
            double r, g, b;
            double amb, diff, spec, refl;
            int shine;
            
            file >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            file >> ref_x >> ref_y >> ref_z >> len >> wid >> hei;
            file >> r >> g >> b;
            file >> amb >> diff >> spec >> refl;
            file >> shine;
            
            GeneralQuadric* quad = new GeneralQuadric(A, B, C, D, E, F, G, H, I, J,
                                                     Vector3D(ref_x, ref_y, ref_z),
                                                     len, wid, hei);
            quad->setColor(r, g, b);
            quad->setCoEfficients(amb, diff, spec, refl);
            quad->setShine(shine);
            objects.push_back(quad);
        }
    }
    

    Floor* floor = new Floor(1000, 20);
    floor->setColor(1, 1, 1);
    // floor->setCoEfficients(0.4, 0.2, 0.1, 0.1);
    floor->setCoEfficients(0.25, 0.6, 0.1, 0.05);// testing for somik
    floor->setShine(5);

    if (floor->loadTexture("texture2.jpg")) {
        floor->setUseTexture(true);
        cout << "Texture loaded and enabled for floor." << endl;
    } else {
        cout << "Failed to load texture, using checkerboard pattern." << endl;
        floor->setUseTexture(false);
    }

    floorObject = floor;
    objects.push_back(floor);
    

    int num_point_lights;
    file >> num_point_lights;
    
    for (int i = 0; i < num_point_lights; i++) {
        double x, y, z, r, g, b;
        file >> x >> y >> z >> r >> g >> b;
        pointLights.push_back(PointLight(Vector3D(x, y, z), r, g, b));
    }
    

    int num_spot_lights;
    file >> num_spot_lights;
    
    for (int i = 0; i < num_spot_lights; i++) {
        double pos_x, pos_y, pos_z;
        double r, g, b;
        double dir_x, dir_y, dir_z;
        double cutoff;
        
        file >> pos_x >> pos_y >> pos_z;
        file >> r >> g >> b;
        file >> dir_x >> dir_y >> dir_z;
        file >> cutoff;
        
        spotLights.push_back(SpotLight(Vector3D(pos_x, pos_y, pos_z), r, g, b,
                                      Vector3D(dir_x, dir_y, dir_z), cutoff));
    }
    
    file.close();
}

void calculateCamera() {
    l = look.normalize();
    r = l.cross(up).normalize();
    u = r.cross(l).normalize();
}

void moveForward() {
    eye = eye + l * 3.0;
    cout << "Moved forward: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << endl;
}

void moveBackward() {
    eye = eye - l * 3.0;
    cout << "Moved backward: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << endl;
}

void moveLeft() {
    eye = eye - r * 3.0;
    cout << "Moved left: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << endl;
}

void moveRight() {
    eye = eye + r * 3.0;
    cout << "Moved right: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << endl;
}

void moveUp() {
    eye = eye + u * 3.0;
    cout << "Moved up: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << endl;
}

void moveDown() {
    eye = eye - u * 3.0;
    cout << "Moved down: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << endl;
}


void rotateLeft() {
    double angle = 0.1; 
    cout << "Rotating left..." << endl;
    
    Vector3D new_l = l * cos(angle) + r * sin(angle);
    Vector3D new_r = l * (-sin(angle)) + r * cos(angle);
    
    l = new_l.normalize();
    r = new_r.normalize();
    u = r.cross(l).normalize();
    
    cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << endl;
}

void rotateRight() {
    double angle = -0.1;
    cout << "Rotating right..." << endl;
    
    Vector3D new_l = l * cos(angle) + r * sin(angle);
    Vector3D new_r = l * (-sin(angle)) + r * cos(angle);
    
    l = new_l.normalize();
    r = new_r.normalize();
    u = r.cross(l).normalize();
    
    cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << endl;
}

void lookUp() {
    double angle = 0.1; 
    cout << "Looking up..." << endl;
    
    Vector3D new_l = l * cos(angle) + u * sin(angle);
    Vector3D new_u = l * (-sin(angle)) + u * cos(angle);
    
    l = new_l.normalize();
    u = new_u.normalize();
    r = l.cross(up).normalize(); 
    
    cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << endl;
}

void lookDown() {
    double angle = -0.1; 
    cout << "Looking down..." << endl;
    

    Vector3D new_l = l * cos(angle) + u * sin(angle);
    Vector3D new_u = l * (-sin(angle)) + u * cos(angle);
    
    l = new_l.normalize();
    u = new_u.normalize();
    r = l.cross(up).normalize(); 
    
    cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << endl;
}

void tiltClockwise() {
    double angle = -0.1; 
    cout << "Tilting clockwise..." << endl;
    

    Vector3D new_u = u * cos(angle) + r * sin(angle);
    Vector3D new_r = u * (-sin(angle)) + r * cos(angle);
    
    u = new_u.normalize();
    r = new_r.normalize();
    l = u.cross(r).normalize(); 
    
    cout << "Up direction: (" << u.x << ", " << u.y << ", " << u.z << ")" << endl;
}

void tiltCounterClockwise() {
    double angle = 0.1; 
    cout << "Tilting counter-clockwise..." << endl;
    

    Vector3D new_u = u * cos(angle) + r * sin(angle);
    Vector3D new_r = u * (-sin(angle)) + r * cos(angle);
    
    u = new_u.normalize();
    r = new_r.normalize();
    l = u.cross(r).normalize(); 
    
    cout << "Up direction: (" << u.x << ", " << u.y << ", " << u.z << ")" << endl;
}


void reshapeListener(GLsizei width, GLsizei height) {
    if (height == 0)
        height = 1;

    windowWidth = width;
    windowHeight = height;


    double desiredAspect = (double)imageWidth / imageHeight;
    double windowAspect = (double)width / height;

    int viewportWidth, viewportHeight;
    int viewportX = 0, viewportY = 0;

    if (windowAspect >= desiredAspect) {

        viewportWidth = (int)(height * desiredAspect);
        viewportHeight = height;
        viewportX = (width - viewportWidth) / 2;
    } else {

        viewportHeight = (int)(width / desiredAspect);
        viewportWidth = width;
        viewportY = (height - viewportHeight) / 2;
    }

    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, desiredAspect, 1, 2000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cout << "Window resized to: " << width << "x" << height 
              << " (window aspect: " << windowAspect 
              << ", viewport: " << viewportWidth << "x" << viewportHeight 
              << ", desired aspect: " << desiredAspect << ")" << endl;
}


void keyboardListener(unsigned char key, int x, int y) {
    cout << "Key pressed: " << (int)key << " ('" << key << "')" << endl;
    
    switch (key) {
        case '1':
            cout << "Processing rotate left command..." << endl;
            rotateLeft();
            break;
        case '2':
            cout << "Processing rotate right command..." << endl;
            rotateRight();
            break;
        case '3':
            cout << "Processing look up command..." << endl;
            lookUp();
            break;
        case '4':
            cout << "Processing look down command..." << endl;
            lookDown();
            break;
        case '5':
            cout << "Processing tilt clockwise command..." << endl;
            tiltClockwise();
            break;
        case '6':
            cout << "Processing tilt counter-clockwise command..." << endl;
            tiltCounterClockwise();
            break;
        case '0':
            cout << "Processing capture command..." << endl;
            capture();
            break;
        case 't':
        case 'T':
            if (floorObject) {
                static bool useTexture = true;
                useTexture = !useTexture;
                floorObject->setUseTexture(useTexture);
                if (useTexture) {
                    cout << "Switched to texture floor" << endl;
                } else {
                    cout << "Switched to checkerboard floor" << endl;
                }
            }
            break;
        case 27:
            cout << "Exiting..." << endl;
            exit(0);
            break;
        default:
            cout << "Unhandled key: " << (int)key << endl;
            break;
    }
    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y) {
    cout << "Special key pressed: " << key << endl;
    
    switch (key) {
        case GLUT_KEY_UP:
            cout << "Processing move forward command..." << endl;
            moveForward();
            break;
        case GLUT_KEY_DOWN:
            cout << "Processing move backward command..." << endl;
            moveBackward();
            break;
        case GLUT_KEY_LEFT:
            cout << "Processing move left command..." << endl;
            moveLeft();
            break;
        case GLUT_KEY_RIGHT:
            cout << "Processing move right command..." << endl;
            moveRight();
            break;
        case GLUT_KEY_PAGE_UP:
            cout << "Processing move up command..." << endl;
            moveUp();
            break;
        case GLUT_KEY_PAGE_DOWN:
            cout << "Processing move down command..." << endl;
            moveDown();
            break;
        default:
            cout << "Unhandled special key: " << key << endl;
            break;
    }
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(eye.x, eye.y, eye.z,
              eye.x + l.x, eye.y + l.y, eye.z + l.z,
              u.x, u.y, u.z);
    
    for (size_t i = 0; i < objects.size(); i++) {
        objects[i]->draw();
    }
    
    for (size_t i = 0; i < pointLights.size(); i++) {
        pointLights[i].draw();
    }
    
    for (size_t i = 0; i < spotLights.size(); i++) {
        spotLights[i].draw();
    }
    
    glutSwapBuffers();
}

void animate() {
    glutPostRedisplay();
}

void init() {
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, 1, 1, 2000.0);
    
    calculateCamera();
    loadData();
    
    cout << "Initial camera state:" << endl;
    cout << "Eye: (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << endl;
    cout << "Look: (" << l.x << ", " << l.y << ", " << l.z << ")" << endl;
    cout << "Up: (" << u.x << ", " << u.y << ", " << u.z << ")" << endl;
    cout << "Right: (" << r.x << ", " << r.y << ", " << r.z << ")" << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutReshapeFunc(reshapeListener);
    glutCreateWindow("Ray Tracing Assignment - Fixed Aspect Ratio");
    
    init();
    
    glEnable(GL_DEPTH_TEST);
    
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    
    cout << "=== RAY TRACING CONTROLS ===" << endl;
    cout << "Arrow Keys: Move camera" << endl;
    cout << "Page Up/Down: Move up/down" << endl;
    cout << "1/2: Rotate left/right (YAW)" << endl;
    cout << "3/4: Look up/down (PITCH)" << endl;
    cout << "5/6: Tilt clockwise/counterclockwise (ROLL)" << endl;
    cout << "0: Capture image" << endl;
    cout << "T: Toggle between texture and checkerboard floor" << endl;
    cout << "ESC: Exit" << endl;
    
    glutMainLoop();

    for (size_t i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
    
    return 0;
}


void capture() {
    cout << "Capturing image..." << endl;
    cout << "Current window size: " << windowWidth << "x" << windowHeight << endl;
    
    double windowAspect = windowWidth / windowHeight;
    

    int captureWidth, captureHeight;
    
    if (windowAspect >= 1.0) {

        captureWidth = imageWidth;
        captureHeight = (int)(imageWidth / windowAspect);
    } else {
        captureHeight = imageHeight;
        captureWidth = (int)(imageHeight * windowAspect);
    }
    
    cout << "Capture dimensions: " << captureWidth << "x" << captureHeight 
              << " (aspect ratio: " << (double)captureWidth/captureHeight << ")" << endl;
    

    bitmap_image image(captureWidth, captureHeight);
    image.set_all_channels(0, 0, 0); 
    
    double planeDistance = (windowHeight / 2.0) / tan((viewAngle * M_PI / 180.0) / 2.0);
    Vector3D topleft = eye + l * planeDistance - r * (windowWidth / 2) + u * (windowHeight / 2);
    
    double du = windowWidth / captureWidth;
    double dv = windowHeight / captureHeight;
    

    topleft = topleft + r * (0.5 * du) - u * (0.5 * dv);
    

    for (int i = 0; i < captureWidth; i++) {
        for (int j = 0; j < captureHeight; j++) {

            Vector3D curPixel = topleft + r * (i * du) - u * (j * dv);
            

            Vector3D rayDir = (curPixel - eye).normalize();
            Ray ray(eye, rayDir);
            
            double t_min = 1e9;
            int nearest = -1;
            
            for (size_t k = 0; k < objects.size(); k++) {
                double dummy_color[3];
                double t = objects[k]->intersect(&ray, dummy_color, 0);
                if (t > 0 && t < t_min) {
                    t_min = t;
                    nearest = (int)k;
                }
            }
            

            if (nearest != -1) {
                double pixel_color[3];
                objects[nearest]->intersect(&ray, pixel_color, 1);
                
                int red = (int)(pixel_color[0] * 255);
                int green = (int)(pixel_color[1] * 255);
                int blue = (int)(pixel_color[2] * 255);
                
                red = max(0, min(255, red));
                green = max(0, min(255, green));
                blue = max(0, min(255, blue));
                
                image.set_pixel(i, j, red, green, blue);
            }
        }

        if (i % 50 == 0) {
            cout << "Progress: " << (i * 100 / captureWidth) << "%" << endl;
        }
    }

    stringstream filename;
    filename << "Output_" << imageCount << ".bmp";
    image.save_image(filename.str());
    imageCount++;
    
    cout << "Image saved as " << filename.str() << endl;
}