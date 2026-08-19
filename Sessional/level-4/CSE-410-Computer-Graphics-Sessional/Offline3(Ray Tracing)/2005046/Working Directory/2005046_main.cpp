#include "2005046_classes.hpp"
#include "bitmap_image.hpp"
#include <algorithm>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// Include intersection methods
#include "2005046_intersection.cpp"

// Global variables
std::vector<Object*> objects;
std::vector<PointLight> pointLights;
std::vector<SpotLight> spotLights;

// Camera variables
Vector3D eye(100, 100, 100);
Vector3D look(-100, -100, -100);
Vector3D up(0, 0, 1);
Vector3D r, l, u;
Floor* floorObject = nullptr;

// Ray tracing parameters
int recursion_level = 4;
int imageWidth = 768;
int imageHeight = 768;
int imageCount = 1;

// Window parameters - THESE WILL BE UPDATED ON RESIZE
double windowWidth = 500;
double windowHeight = 500;
double viewAngle = 80;

// Utility function to clamp values
double clamp(double value, double min_val, double max_val) {
    return std::max(min_val, std::min(max_val, value));
}

// Function to load scene data
void loadData() {
    std::ifstream file("scene.txt");
    if (!file.is_open()) {
        std::cout << "Error: Cannot open scene.txt" << std::endl;
        return;
    }
    
    // Read recursion level and image dimensions
    file >> recursion_level >> imageWidth;
    imageHeight = imageWidth; // Assuming square images
    
    // Read number of objects
    int num_objects;
    file >> num_objects;
    
    // Read objects
    for (int i = 0; i < num_objects; i++) {
        std::string object_type;
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
    
    // Add floor
    Floor* floor = new Floor(1000, 20);
    floor->setColor(1, 1, 1);
    // floor->setCoEfficients(0.4, 0.2, 0.1, 0.1);
    floor->setCoEfficients(0.25, 0.6, 0.1, 0.05);// testing for somik
    floor->setShine(5);

    if (floor->loadTexture("texture.jpg")) {
        floor->setUseTexture(true); // Enable texture by default
        std::cout << "Texture loaded and enabled for floor." << std::endl;
    } else {
        std::cout << "Failed to load texture, using checkerboard pattern." << std::endl;
        floor->setUseTexture(false);
    }

    floorObject = floor;
    objects.push_back(floor);
    
    // Read point lights
    int num_point_lights;
    file >> num_point_lights;
    
    for (int i = 0; i < num_point_lights; i++) {
        double x, y, z, r, g, b;
        file >> x >> y >> z >> r >> g >> b;
        pointLights.push_back(PointLight(Vector3D(x, y, z), r, g, b));
    }
    
    // Read spot lights
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

// COMPLETELY REWRITTEN Camera setup functions
void calculateCamera() {
    l = look.normalize();
    r = l.cross(up).normalize();
    u = r.cross(l).normalize();
}

void moveForward() {
    eye = eye + l * 3.0;
    std::cout << "Moved forward: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

void moveBackward() {
    eye = eye - l * 3.0;
    std::cout << "Moved backward: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

void moveLeft() {
    eye = eye - r * 3.0;
    std::cout << "Moved left: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

void moveRight() {
    eye = eye + r * 3.0;
    std::cout << "Moved right: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

void moveUp() {
    eye = eye + u * 3.0;
    std::cout << "Moved up: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

void moveDown() {
    eye = eye - u * 3.0;
    std::cout << "Moved down: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

// FIXED rotation functions - using proper rotation matrices
void rotateLeft() {
    double angle = 0.1; // Increased angle for more noticeable rotation
    std::cout << "Rotating left..." << std::endl;
    
    // Rotate around the up vector (yaw)
    Vector3D new_l = l * cos(angle) + r * sin(angle);
    Vector3D new_r = l * (-sin(angle)) + r * cos(angle);
    
    l = new_l.normalize();
    r = new_r.normalize();
    u = r.cross(l).normalize();
    
    std::cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << std::endl;
}

void rotateRight() {
    double angle = -0.1; // Increased angle for more noticeable rotation
    std::cout << "Rotating right..." << std::endl;
    
    // Rotate around the up vector (yaw)
    Vector3D new_l = l * cos(angle) + r * sin(angle);
    Vector3D new_r = l * (-sin(angle)) + r * cos(angle);
    
    l = new_l.normalize();
    r = new_r.normalize();
    u = r.cross(l).normalize();
    
    std::cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << std::endl;
}

void lookUp() {
    double angle = 0.1; // Increased angle for more noticeable rotation
    std::cout << "Looking up..." << std::endl;
    
    // Rotate around the right vector (pitch)
    Vector3D new_l = l * cos(angle) + u * sin(angle);
    Vector3D new_u = l * (-sin(angle)) + u * cos(angle);
    
    l = new_l.normalize();
    u = new_u.normalize();
    r = l.cross(up).normalize(); // Recalculate r
    
    std::cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << std::endl;
}

void lookDown() {
    double angle = -0.1; // Increased angle for more noticeable rotation
    std::cout << "Looking down..." << std::endl;
    
    // Rotate around the right vector (pitch)
    Vector3D new_l = l * cos(angle) + u * sin(angle);
    Vector3D new_u = l * (-sin(angle)) + u * cos(angle);
    
    l = new_l.normalize();
    u = new_u.normalize();
    r = l.cross(up).normalize(); // Recalculate r
    
    std::cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << std::endl;
}

void tiltClockwise() {
    double angle = -0.1; // Increased angle for more noticeable rotation
    std::cout << "Tilting clockwise..." << std::endl;
    
    // Rotate around the look vector (roll)
    Vector3D new_u = u * cos(angle) + r * sin(angle);
    Vector3D new_r = u * (-sin(angle)) + r * cos(angle);
    
    u = new_u.normalize();
    r = new_r.normalize();
    l = u.cross(r).normalize(); // Recalculate l
    
    std::cout << "Up direction: (" << u.x << ", " << u.y << ", " << u.z << ")" << std::endl;
}

void tiltCounterClockwise() {
    double angle = 0.1; // Increased angle for more noticeable rotation
    std::cout << "Tilting counter-clockwise..." << std::endl;
    
    // Rotate around the look vector (roll)
    Vector3D new_u = u * cos(angle) + r * sin(angle);
    Vector3D new_r = u * (-sin(angle)) + r * cos(angle);
    
    u = new_u.normalize();
    r = new_r.normalize();
    l = u.cross(r).normalize(); // Recalculate l
    
    std::cout << "Up direction: (" << u.x << ", " << u.y << ", " << u.z << ")" << std::endl;
}

// UPDATED: Maintain aspect ratio in fullscreen
void reshapeListener(GLsizei width, GLsizei height) {
    if (height == 0)
        height = 1;

    // Update global window dimensions
    windowWidth = width;
    windowHeight = height;

    // Desired aspect ratio based on image dimensions
    double desiredAspect = (double)imageWidth / imageHeight;
    double windowAspect = (double)width / height;

    // Calculate viewport dimensions to maintain aspect ratio
    int viewportWidth, viewportHeight;
    int viewportX = 0, viewportY = 0;

    if (windowAspect >= desiredAspect) {
        // Window is wider than desired: add letterbox bars on left/right
        viewportWidth = (int)(height * desiredAspect);
        viewportHeight = height;
        viewportX = (width - viewportWidth) / 2;
    } else {
        // Window is taller than desired: add letterbox bars on top/bottom
        viewportHeight = (int)(width / desiredAspect);
        viewportWidth = width;
        viewportY = (height - viewportHeight) / 2;
    }

    // Set the viewport to maintain aspect ratio
    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);

    // Set up projection matrix with correct aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, desiredAspect, 1, 2000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    std::cout << "Window resized to: " << width << "x" << height 
              << " (window aspect: " << windowAspect 
              << ", viewport: " << viewportWidth << "x" << viewportHeight 
              << ", desired aspect: " << desiredAspect << ")" << std::endl;
}

// OpenGL callback functions
void keyboardListener(unsigned char key, int x, int y) {
    std::cout << "Key pressed: " << (int)key << " ('" << key << "')" << std::endl;
    
    switch (key) {
        case '1':
            std::cout << "Processing rotate left command..." << std::endl;
            rotateLeft();
            break;
        case '2':
            std::cout << "Processing rotate right command..." << std::endl;
            rotateRight();
            break;
        case '3':
            std::cout << "Processing look up command..." << std::endl;
            lookUp();
            break;
        case '4':
            std::cout << "Processing look down command..." << std::endl;
            lookDown();
            break;
        case '5':
            std::cout << "Processing tilt clockwise command..." << std::endl;
            tiltClockwise();
            break;
        case '6':
            std::cout << "Processing tilt counter-clockwise command..." << std::endl;
            tiltCounterClockwise();
            break;
        case '0':
            std::cout << "Processing capture command..." << std::endl;
            capture();
            break;
        case 't':
        case 'T':
            if (floorObject) {
                // Toggle between texture and checkerboard
                static bool useTexture = true;
                useTexture = !useTexture;
                floorObject->setUseTexture(useTexture);
                if (useTexture) {
                    std::cout << "Switched to texture floor" << std::endl;
                } else {
                    std::cout << "Switched to checkerboard floor" << std::endl;
                }
            }
            break;
        case 27: // ESC key
            std::cout << "Exiting..." << std::endl;
            exit(0);
            break;
        default:
            std::cout << "Unhandled key: " << (int)key << std::endl;
            break;
    }
    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y) {
    std::cout << "Special key pressed: " << key << std::endl;
    
    switch (key) {
        case GLUT_KEY_UP:
            std::cout << "Processing move forward command..." << std::endl;
            moveForward();
            break;
        case GLUT_KEY_DOWN:
            std::cout << "Processing move backward command..." << std::endl;
            moveBackward();
            break;
        case GLUT_KEY_LEFT:
            std::cout << "Processing move left command..." << std::endl;
            moveLeft();
            break;
        case GLUT_KEY_RIGHT:
            std::cout << "Processing move right command..." << std::endl;
            moveRight();
            break;
        case GLUT_KEY_PAGE_UP:
            std::cout << "Processing move up command..." << std::endl;
            moveUp();
            break;
        case GLUT_KEY_PAGE_DOWN:
            std::cout << "Processing move down command..." << std::endl;
            moveDown();
            break;
        default:
            std::cout << "Unhandled special key: " << key << std::endl;
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
    
    // Draw objects
    for (size_t i = 0; i < objects.size(); i++) {
        objects[i]->draw();
    }
    
    // Draw point lights
    for (size_t i = 0; i < pointLights.size(); i++) {
        pointLights[i].draw();
    }
    
    // Draw spot lights
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
    
    // Print initial camera state
    std::cout << "Initial camera state:" << std::endl;
    std::cout << "Eye: (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
    std::cout << "Look: (" << l.x << ", " << l.y << ", " << l.z << ")" << std::endl;
    std::cout << "Up: (" << u.x << ", " << u.y << ", " << u.z << ")" << std::endl;
    std::cout << "Right: (" << r.x << ", " << r.y << ", " << r.z << ")" << std::endl;
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
    
    // Print controls with debug info
    std::cout << "=== RAY TRACING CONTROLS ===" << std::endl;
    std::cout << "Arrow Keys: Move camera" << std::endl;
    std::cout << "Page Up/Down: Move up/down" << std::endl;
    std::cout << "1/2: Rotate left/right (YAW)" << std::endl;
    std::cout << "3/4: Look up/down (PITCH)" << std::endl;
    std::cout << "5/6: Tilt clockwise/counterclockwise (ROLL)" << std::endl;
    std::cout << "0: Capture image" << std::endl;
    std::cout << "T: Toggle between texture and checkerboard floor" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "Debug: Camera movements will be printed to console" << std::endl;
    std::cout << "Test keys 1 and 2 - you should see rotation messages" << std::endl;
    
    glutMainLoop();
    
    // Cleanup
    for (size_t i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
    
    return 0;
}

// FIXED: Ray tracing capture function with proper aspect ratio handling
void capture() {
    std::cout << "Capturing image..." << std::endl;
    std::cout << "Current window size: " << windowWidth << "x" << windowHeight << std::endl;
    
    // Calculate current window aspect ratio
    double windowAspect = windowWidth / windowHeight;
    
    // Determine capture dimensions maintaining aspect ratio
    int captureWidth, captureHeight;
    
    if (windowAspect >= 1.0) {
        // Window is wider than it is tall
        captureWidth = imageWidth;
        captureHeight = (int)(imageWidth / windowAspect);
    } else {
        // Window is taller than it is wide
        captureHeight = imageHeight;
        captureWidth = (int)(imageHeight * windowAspect);
    }
    
    std::cout << "Capture dimensions: " << captureWidth << "x" << captureHeight 
              << " (aspect ratio: " << (double)captureWidth/captureHeight << ")" << std::endl;
    
    // Initialize bitmap image with proper dimensions
    bitmap_image image(captureWidth, captureHeight);
    image.set_all_channels(0, 0, 0); // Black background
    
    // Calculate view plane parameters using current window dimensions
    double planeDistance = (windowHeight / 2.0) / tan((viewAngle * M_PI / 180.0) / 2.0);
    Vector3D topleft = eye + l * planeDistance - r * (windowWidth / 2) + u * (windowHeight / 2);
    
    double du = windowWidth / captureWidth;
    double dv = windowHeight / captureHeight;
    
    // Choose middle of the grid cell
    topleft = topleft + r * (0.5 * du) - u * (0.5 * dv);
    
    // Ray tracing loop
    for (int i = 0; i < captureWidth; i++) {
        for (int j = 0; j < captureHeight; j++) {
            // Calculate current pixel position
            Vector3D curPixel = topleft + r * (i * du) - u * (j * dv);
            
            // Cast ray from eye to current pixel
            Vector3D rayDir = (curPixel - eye).normalize();
            Ray ray(eye, rayDir);
            
            // Find nearest intersection
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
            
            // Calculate final color
            if (nearest != -1) {
                double pixel_color[3];
                objects[nearest]->intersect(&ray, pixel_color, 1);
                
                // Convert to 0-255 range and set pixel
                int red = (int)(pixel_color[0] * 255);
                int green = (int)(pixel_color[1] * 255);
                int blue = (int)(pixel_color[2] * 255);
                
                // Clamp values
                red = std::max(0, std::min(255, red));
                green = std::max(0, std::min(255, green));
                blue = std::max(0, std::min(255, blue));
                
                image.set_pixel(i, j, red, green, blue);
            }
        }
        
        // Show progress
        if (i % 50 == 0) {
            std::cout << "Progress: " << (i * 100 / captureWidth) << "%" << std::endl;
        }
    }
    
    // Save image with proper numbering
    std::stringstream filename;
    filename << "Output_" << imageCount << ".bmp";
    image.save_image(filename.str());
    imageCount++;
    
    std::cout << "Image saved as " << filename.str() << std::endl;
}