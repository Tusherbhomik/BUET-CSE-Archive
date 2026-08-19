#include "classes.hpp"
#include "bitmap_image.hpp"
#include <algorithm>
#include <sstream>

// Include intersection methods
#include "intersection.cpp"

// Global variables
std::vector<Object*> objects;
std::vector<PointLight> pointLights;
std::vector<SpotLight> spotLights;

// Camera variables
Vector3D eye(100, 100, 50);
Vector3D look(-1, -1, 0);
Vector3D up(0, 0, 1);
Vector3D r, l, u;

// Ray tracing parameters
int recursion_level = 4;
int imageWidth = 768;
int imageHeight = 768;
int imageCount = 1;

// Window parameters
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
    floor->setCoEfficients(0.4, 0.2, 0.1, 0.1);
    floor->setShine(5);
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

// Camera setup functions - CORRECTED
void calculateCamera() {
    l = look.normalize();
    r = l.cross(up).normalize();
    u = r.cross(l).normalize();
}

void moveForward() {
    eye = eye + l * 3.0; // Added scaling factor for better movement
}

void moveBackward() {
    eye = eye - l * 3.0;
}

void moveLeft() {
    eye = eye - r * 3.0;
}

void moveRight() {
    eye = eye + r * 3.0;
}

void moveUp() {
    eye = eye + u * 3.0;
}

void moveDown() {
    eye = eye - u * 3.0;
}

// CORRECTED rotation functions
void rotateLeft() {
    double angle = 0.05;
    Vector3D temp_l = l;
    l = temp_l * cos(angle) + r * sin(angle);
    r = temp_l * (-sin(angle)) + r * cos(angle);
    calculateCamera();
}

void rotateRight() {
    double angle = -0.05;
    Vector3D temp_l = l;
    l = temp_l * cos(angle) + r * sin(angle);
    r = temp_l * (-sin(angle)) + r * cos(angle);
    calculateCamera();
}

void lookUp() {
    double angle = 0.05;
    Vector3D temp_l = l;
    l = temp_l * cos(angle) + u * sin(angle);
    u = temp_l * (-sin(angle)) + u * cos(angle);
    calculateCamera();
}

void lookDown() {
    double angle = -0.05;
    Vector3D temp_l = l;
    l = temp_l * cos(angle) + u * sin(angle);
    u = temp_l * (-sin(angle)) + u * cos(angle);
    calculateCamera();
}

void tiltClockwise() {
    double angle = -0.05;
    Vector3D temp_u = u;
    u = temp_u * cos(angle) + r * sin(angle);
    r = temp_u * (-sin(angle)) + r * cos(angle);
    calculateCamera();
}

void tiltCounterClockwise() {
    double angle = 0.05;
    Vector3D temp_u = u;
    u = temp_u * cos(angle) + r * sin(angle);
    r = temp_u * (-sin(angle)) + r * cos(angle);
    calculateCamera();
}

// OpenGL callback functions
void keyboardListener(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            rotateLeft();
            break;
        case '2':
            rotateRight();
            break;
        case '3':
            lookUp();
            break;
        case '4':
            lookDown();
            break;
        case '5':
            tiltClockwise();
            break;
        case '6':
            tiltCounterClockwise();
            break;
        case '0':
            capture();
            break;
        case 27: // ESC key
            exit(0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            moveForward();
            break;
        case GLUT_KEY_DOWN:
            moveBackward();
            break;
        case GLUT_KEY_LEFT:
            moveLeft();
            break;
        case GLUT_KEY_RIGHT:
            moveRight();
            break;
        case GLUT_KEY_PAGE_UP:
            moveUp();
            break;
        case GLUT_KEY_PAGE_DOWN:
            moveDown();
            break;
        default:
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
    
    // Draw objects - FIXED: Use size_t for comparison
    for (size_t i = 0; i < objects.size(); i++) {
        objects[i]->draw();
    }
    
    // Draw point lights - FIXED: Use size_t for comparison
    for (size_t i = 0; i < pointLights.size(); i++) {
        pointLights[i].draw();
    }
    
    // Draw spot lights - FIXED: Use size_t for comparison
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
    gluPerspective(viewAngle, 1, 1, 1000.0);
    
    calculateCamera();
    loadData();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    
    glutCreateWindow("Ray Tracing Assignment");
    
    init();
    
    glEnable(GL_DEPTH_TEST);
    
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    
    // Print controls
    std::cout << "=== RAY TRACING CONTROLS ===" << std::endl;
    std::cout << "Arrow Keys: Move camera" << std::endl;
    std::cout << "Page Up/Down: Move up/down" << std::endl;
    std::cout << "1/2: Rotate left/right" << std::endl;
    std::cout << "3/4: Look up/down" << std::endl;
    std::cout << "5/6: Tilt clockwise/counterclockwise" << std::endl;
    std::cout << "0: Capture image" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "============================" << std::endl;
    
    glutMainLoop();
    
    // Cleanup - FIXED: Use size_t for comparison
    for (size_t i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
    
    return 0;
}

// Ray tracing capture function
void capture() {
    std::cout << "Capturing image..." << std::endl;
    
    // Initialize bitmap image
    bitmap_image image(imageWidth, imageHeight);
    image.set_all_channels(0, 0, 0); // Black background
    
    // Calculate view plane parameters
    double planeDistance = (windowHeight / 2.0) / tan((viewAngle * M_PI / 180.0) / 2.0);
    Vector3D topleft = eye + l * planeDistance - r * (windowWidth / 2) + u * (windowHeight / 2);
    
    double du = windowWidth / imageWidth;
    double dv = windowHeight / imageHeight;
    
    // Choose middle of the grid cell
    topleft = topleft + r * (0.5 * du) - u * (0.5 * dv);
    
    // Ray tracing loop
    for (int i = 0; i < imageWidth; i++) {
        for (int j = 0; j < imageHeight; j++) {
            // Calculate current pixel position
            Vector3D curPixel = topleft + r * (i * du) - u * (j * dv);
            
            // Cast ray from eye to current pixel
            Vector3D rayDir = (curPixel - eye).normalize();
            Ray ray(eye, rayDir);
            
            // Find nearest intersection
            double t_min = 1e9;
            int nearest = -1;
            
            // FIXED: Use size_t for comparison
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
            std::cout << "Progress: " << (i * 100 / imageWidth) << "%" << std::endl;
        }
    }
    
    // Save image with proper numbering
    std::stringstream filename;
    filename << "Output_" << imageCount << ".bmp";
    image.save_image(filename.str());
    imageCount++;
    
    std::cout << "Image saved as " << filename.str() << std::endl;
}