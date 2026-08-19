#include "classes.hpp"
#include "bitmap_image.hpp"
#include <algorithm>
#include <sstream>

// Include intersection methods
#include "intersection.cpp"

// Debug macros
#ifdef DEBUG
#define DEBUG_PRINT(x) std::cout << "[DEBUG] " << x << std::endl
#define DEBUG_VECTOR(name, vec) std::cout << "[DEBUG] " << name << ": (" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl
#define DEBUG_CAMERA() \
    do { \
        std::cout << "[DEBUG] === CAMERA STATE ===" << std::endl; \
        DEBUG_VECTOR("Eye", eye); \
        DEBUG_VECTOR("Look", l); \
        DEBUG_VECTOR("Up", u); \
        DEBUG_VECTOR("Right", r); \
        std::cout << "[DEBUG] ===================" << std::endl; \
    } while(0)
#else
#define DEBUG_PRINT(x)
#define DEBUG_VECTOR(name, vec)
#define DEBUG_CAMERA()
#endif

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

// Debug: Function call counter
static int function_call_count = 0;

// Utility function to clamp values
double clamp(double value, double min_val, double max_val) {
    DEBUG_PRINT("Clamping value: " << value << " to range [" << min_val << ", " << max_val << "]");
    return std::max(min_val, std::min(max_val, value));
}

// Function to load scene data
void loadData() {
    DEBUG_PRINT("Starting loadData()");
    
    std::ifstream file("scene.txt");
    if (!file.is_open()) {
        DEBUG_PRINT("ERROR: Cannot open scene.txt");
        std::cout << "Error: Cannot open scene.txt" << std::endl;
        return;
    }
    
    DEBUG_PRINT("Scene file opened successfully");
    
    // Read recursion level and image dimensions
    file >> recursion_level >> imageWidth;
    imageHeight = imageWidth; // Assuming square images
    
    DEBUG_PRINT("Recursion level: " << recursion_level << ", Image size: " << imageWidth << "x" << imageHeight);
    
    // Read number of objects
    int num_objects;
    file >> num_objects;
    DEBUG_PRINT("Number of objects to load: " << num_objects);
    
    // Read objects
    for (int i = 0; i < num_objects; i++) {
        std::string object_type;
        file >> object_type;
        DEBUG_PRINT("Loading object " << (i+1) << " of type: " << object_type);
        
        if (object_type == "sphere") {
            double x, y, z, radius;
            double r, g, b;
            double amb, diff, spec, refl;
            int shine;
            
            file >> x >> y >> z >> radius;
            file >> r >> g >> b;
            file >> amb >> diff >> spec >> refl;
            file >> shine;
            
            DEBUG_PRINT("  Sphere at (" << x << ", " << y << ", " << z << ") radius=" << radius);
            
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
            
            DEBUG_PRINT("  Triangle with vertices (" << x1 << "," << y1 << "," << z1 << ") (" << x2 << "," << y2 << "," << z2 << ") (" << x3 << "," << y3 << "," << z3 << ")");
            
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
            
            DEBUG_PRINT("  General quadric surface");
            
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
    DEBUG_PRINT("Adding floor");
    Floor* floor = new Floor(1000, 20);
    floor->setColor(1, 1, 1);
    floor->setCoEfficients(0.4, 0.2, 0.1, 0.1);
    floor->setShine(5);
    objects.push_back(floor);
    
    // Read point lights
    int num_point_lights;
    file >> num_point_lights;
    DEBUG_PRINT("Number of point lights: " << num_point_lights);
    
    for (int i = 0; i < num_point_lights; i++) {
        double x, y, z, r, g, b;
        file >> x >> y >> z >> r >> g >> b;
        DEBUG_PRINT("  Point light " << (i+1) << " at (" << x << ", " << y << ", " << z << ")");
        pointLights.push_back(PointLight(Vector3D(x, y, z), r, g, b));
    }
    
    // Read spot lights
    int num_spot_lights;
    file >> num_spot_lights;
    DEBUG_PRINT("Number of spot lights: " << num_spot_lights);
    
    for (int i = 0; i < num_spot_lights; i++) {
        double pos_x, pos_y, pos_z;
        double r, g, b;
        double dir_x, dir_y, dir_z;
        double cutoff;
        
        file >> pos_x >> pos_y >> pos_z;
        file >> r >> g >> b;
        file >> dir_x >> dir_y >> dir_z;
        file >> cutoff;
        
        DEBUG_PRINT("  Spot light " << (i+1) << " at (" << pos_x << ", " << pos_y << ", " << pos_z << ") cutoff=" << cutoff);
        
        spotLights.push_back(SpotLight(Vector3D(pos_x, pos_y, pos_z), r, g, b,
                                      Vector3D(dir_x, dir_y, dir_z), cutoff));
    }
    
    file.close();
    DEBUG_PRINT("loadData() completed successfully");
}

// Camera setup functions
void calculateCamera() {
    DEBUG_PRINT("calculateCamera() called");
    l = look.normalize();
    r = l.cross(up).normalize();
    u = r.cross(l).normalize();
    DEBUG_CAMERA();
}

void moveForward() {
    function_call_count++;
    DEBUG_PRINT("moveForward() called (call #" << function_call_count << ")");
    DEBUG_VECTOR("Eye before", eye);
    
    eye = eye + l * 3.0;
    
    DEBUG_VECTOR("Eye after", eye);
    std::cout << "Moved forward: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

void moveBackward() {
    function_call_count++;
    DEBUG_PRINT("moveBackward() called (call #" << function_call_count << ")");
    DEBUG_VECTOR("Eye before", eye);
    
    eye = eye - l * 3.0;
    
    DEBUG_VECTOR("Eye after", eye);
    std::cout << "Moved backward: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

void moveLeft() {
    function_call_count++;
    DEBUG_PRINT("moveLeft() called (call #" << function_call_count << ")");
    DEBUG_VECTOR("Eye before", eye);
    
    eye = eye - r * 3.0;
    
    DEBUG_VECTOR("Eye after", eye);
    std::cout << "Moved left: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

void moveRight() {
    function_call_count++;
    DEBUG_PRINT("moveRight() called (call #" << function_call_count << ")");
    DEBUG_VECTOR("Eye before", eye);
    
    eye = eye + r * 3.0;
    
    DEBUG_VECTOR("Eye after", eye);
    std::cout << "Moved right: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

void moveUp() {
    function_call_count++;
    DEBUG_PRINT("moveUp() called (call #" << function_call_count << ")");
    DEBUG_VECTOR("Eye before", eye);
    
    eye = eye + u * 3.0;
    
    DEBUG_VECTOR("Eye after", eye);
    std::cout << "Moved up: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

void moveDown() {
    function_call_count++;
    DEBUG_PRINT("moveDown() called (call #" << function_call_count << ")");
    DEBUG_VECTOR("Eye before", eye);
    
    eye = eye - u * 3.0;
    
    DEBUG_VECTOR("Eye after", eye);
    std::cout << "Moved down: Eye at (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
}

// ROTATION FUNCTIONS WITH EXTENSIVE DEBUG INFO
void rotateLeft() {
    function_call_count++;
    DEBUG_PRINT("=== rotateLeft() START (call #" << function_call_count << ") ===");
    
    std::cout << "Rotating left..." << std::endl;
    
    // Print state before rotation
    DEBUG_PRINT("BEFORE rotation:");
    DEBUG_CAMERA();
    
    double angle = 0.1; // Rotation angle
    DEBUG_PRINT("Rotation angle: " << angle << " radians (" << (angle * 180.0 / M_PI) << " degrees)");
    
    // Calculate new vectors
    Vector3D new_l = l * cos(angle) + r * sin(angle);
    Vector3D new_r = l * (-sin(angle)) + r * cos(angle);
    
    DEBUG_PRINT("Calculated new vectors:");
    DEBUG_VECTOR("new_l", new_l);
    DEBUG_VECTOR("new_r", new_r);
    
    // Apply changes
    l = new_l.normalize();
    r = new_r.normalize();
    u = r.cross(l).normalize();
    
    // Print state after rotation
    DEBUG_PRINT("AFTER rotation and normalization:");
    DEBUG_CAMERA();
    
    std::cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << std::endl;
    DEBUG_PRINT("=== rotateLeft() END ===");
}

void rotateRight() {
    function_call_count++;
    DEBUG_PRINT("=== rotateRight() START (call #" << function_call_count << ") ===");
    
    std::cout << "Rotating right..." << std::endl;
    
    // Print state before rotation
    DEBUG_PRINT("BEFORE rotation:");
    DEBUG_CAMERA();
    
    double angle = -0.1; // Rotation angle (negative for right)
    DEBUG_PRINT("Rotation angle: " << angle << " radians (" << (angle * 180.0 / M_PI) << " degrees)");
    
    // Calculate new vectors
    Vector3D new_l = l * cos(angle) + r * sin(angle);
    Vector3D new_r = l * (-sin(angle)) + r * cos(angle);
    
    DEBUG_PRINT("Calculated new vectors:");
    DEBUG_VECTOR("new_l", new_l);
    DEBUG_VECTOR("new_r", new_r);
    
    // Apply changes
    l = new_l.normalize();
    r = new_r.normalize();
    u = r.cross(l).normalize();
    
    // Print state after rotation
    DEBUG_PRINT("AFTER rotation and normalization:");
    DEBUG_CAMERA();
    
    std::cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << std::endl;
    DEBUG_PRINT("=== rotateRight() END ===");
}

void lookUp() {
    function_call_count++;
    DEBUG_PRINT("=== lookUp() START (call #" << function_call_count << ") ===");
    
    std::cout << "Looking up..." << std::endl;
    
    DEBUG_PRINT("BEFORE rotation:");
    DEBUG_CAMERA();
    
    double angle = 0.1;
    DEBUG_PRINT("Pitch angle: " << angle << " radians");
    
    Vector3D new_l = l * cos(angle) + u * sin(angle);
    Vector3D new_u = l * (-sin(angle)) + u * cos(angle);
    
    l = new_l.normalize();
    u = new_u.normalize();
    r = l.cross(up).normalize(); // Recalculate r
    
    DEBUG_PRINT("AFTER rotation:");
    DEBUG_CAMERA();
    
    std::cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << std::endl;
    DEBUG_PRINT("=== lookUp() END ===");
}

void lookDown() {
    function_call_count++;
    DEBUG_PRINT("=== lookDown() START (call #" << function_call_count << ") ===");
    
    std::cout << "Looking down..." << std::endl;
    
    DEBUG_PRINT("BEFORE rotation:");
    DEBUG_CAMERA();
    
    double angle = -0.1;
    DEBUG_PRINT("Pitch angle: " << angle << " radians");
    
    Vector3D new_l = l * cos(angle) + u * sin(angle);
    Vector3D new_u = l * (-sin(angle)) + u * cos(angle);
    
    l = new_l.normalize();
    u = new_u.normalize();
    r = l.cross(up).normalize(); // Recalculate r
    
    DEBUG_PRINT("AFTER rotation:");
    DEBUG_CAMERA();
    
    std::cout << "Look direction: (" << l.x << ", " << l.y << ", " << l.z << ")" << std::endl;
    DEBUG_PRINT("=== lookDown() END ===");
}

void tiltClockwise() {
    function_call_count++;
    DEBUG_PRINT("=== tiltClockwise() START (call #" << function_call_count << ") ===");
    
    std::cout << "Tilting clockwise..." << std::endl;
    
    DEBUG_PRINT("BEFORE rotation:");
    DEBUG_CAMERA();
    
    double angle = -0.1;
    DEBUG_PRINT("Roll angle: " << angle << " radians");
    
    Vector3D new_u = u * cos(angle) + r * sin(angle);
    Vector3D new_r = u * (-sin(angle)) + r * cos(angle);
    
    u = new_u.normalize();
    r = new_r.normalize();
    l = u.cross(r).normalize(); // Recalculate l
    
    DEBUG_PRINT("AFTER rotation:");
    DEBUG_CAMERA();
    
    std::cout << "Up direction: (" << u.x << ", " << u.y << ", " << u.z << ")" << std::endl;
    DEBUG_PRINT("=== tiltClockwise() END ===");
}

void tiltCounterClockwise() {
    function_call_count++;
    DEBUG_PRINT("=== tiltCounterClockwise() START (call #" << function_call_count << ") ===");
    
    std::cout << "Tilting counter-clockwise..." << std::endl;
    
    DEBUG_PRINT("BEFORE rotation:");
    DEBUG_CAMERA();
    
    double angle = 0.1;
    DEBUG_PRINT("Roll angle: " << angle << " radians");
    
    Vector3D new_u = u * cos(angle) + r * sin(angle);
    Vector3D new_r = u * (-sin(angle)) + r * cos(angle);
    
    u = new_u.normalize();
    r = new_r.normalize();
    l = u.cross(r).normalize(); // Recalculate l
    
    DEBUG_PRINT("AFTER rotation:");
    DEBUG_CAMERA();
    
    std::cout << "Up direction: (" << u.x << ", " << u.y << ", " << u.z << ")" << std::endl;
    DEBUG_PRINT("=== tiltCounterClockwise() END ===");
}

// OpenGL callback functions
void keyboardListener(unsigned char key, int x, int y) {
    DEBUG_PRINT("=== KEY PRESS EVENT ===");
    DEBUG_PRINT("Key code: " << (int)key << " Character: '" << key << "'");
    DEBUG_PRINT("Mouse position: (" << x << ", " << y << ")");
    
    std::cout << "Key pressed: " << (int)key << " ('" << key << "')" << std::endl;
    
    switch (key) {
        case '1':
            DEBUG_PRINT("Processing rotate left command...");
            std::cout << "Processing rotate left command..." << std::endl;
            rotateLeft(); // BREAKPOINT HERE
            break;
        case '2':
            DEBUG_PRINT("Processing rotate right command...");
            std::cout << "Processing rotate right command..." << std::endl;
            rotateRight(); // BREAKPOINT HERE
            break;
        case '3':
            DEBUG_PRINT("Processing look up command...");
            std::cout << "Processing look up command..." << std::endl;
            lookUp(); // BREAKPOINT HERE
            break;
        case '4':
            DEBUG_PRINT("Processing look down command...");
            std::cout << "Processing look down command..." << std::endl;
            lookDown(); // BREAKPOINT HERE
            break;
        case '5':
            DEBUG_PRINT("Processing tilt clockwise command...");
            std::cout << "Processing tilt clockwise command..." << std::endl;
            tiltClockwise(); // BREAKPOINT HERE
            break;
        case '6':
            DEBUG_PRINT("Processing tilt counter-clockwise command...");
            std::cout << "Processing tilt counter-clockwise command..." << std::endl;
            tiltCounterClockwise(); // BREAKPOINT HERE
            break;
        case '0':
            DEBUG_PRINT("Processing capture command...");
            std::cout << "Processing capture command..." << std::endl;
            capture(); // BREAKPOINT HERE
            break;
        case 27: // ESC key
            DEBUG_PRINT("ESC key pressed - exiting...");
            std::cout << "Exiting..." << std::endl;
            exit(0);
            break;
        default:
            DEBUG_PRINT("Unhandled key: " << (int)key);
            std::cout << "Unhandled key: " << (int)key << std::endl;
            break;
    }
    
    DEBUG_PRINT("Calling glutPostRedisplay()");
    glutPostRedisplay();
    DEBUG_PRINT("=== KEY PRESS EVENT END ===");
}

void specialKeyListener(int key, int x, int y) {
    DEBUG_PRINT("=== SPECIAL KEY PRESS EVENT ===");
    DEBUG_PRINT("Special key code: " << key);
    DEBUG_PRINT("Mouse position: (" << x << ", " << y << ")");
    
    std::cout << "Special key pressed: " << key << std::endl;
    
    switch (key) {
        case GLUT_KEY_UP:
            DEBUG_PRINT("Processing move forward command...");
            std::cout << "Processing move forward command..." << std::endl;
            moveForward(); // BREAKPOINT HERE
            break;
        case GLUT_KEY_DOWN:
            DEBUG_PRINT("Processing move backward command...");
            std::cout << "Processing move backward command..." << std::endl;
            moveBackward(); // BREAKPOINT HERE
            break;
        case GLUT_KEY_LEFT:
            DEBUG_PRINT("Processing move left command...");
            std::cout << "Processing move left command..." << std::endl;
            moveLeft(); // BREAKPOINT HERE
            break;
        case GLUT_KEY_RIGHT:
            DEBUG_PRINT("Processing move right command...");
            std::cout << "Processing move right command..." << std::endl;
            moveRight(); // BREAKPOINT HERE
            break;
        case GLUT_KEY_PAGE_UP:
            DEBUG_PRINT("Processing move up command...");
            std::cout << "Processing move up command..." << std::endl;
            moveUp(); // BREAKPOINT HERE
            break;
        case GLUT_KEY_PAGE_DOWN:
            DEBUG_PRINT("Processing move down command...");
            std::cout << "Processing move down command..." << std::endl;
            moveDown(); // BREAKPOINT HERE
            break;
        default:
            DEBUG_PRINT("Unhandled special key: " << key);
            std::cout << "Unhandled special key: " << key << std::endl;
            break;
    }
    
    DEBUG_PRINT("Calling glutPostRedisplay()");
    glutPostRedisplay();
    DEBUG_PRINT("=== SPECIAL KEY PRESS EVENT END ===");
}

void display() {
    DEBUG_PRINT("display() called");
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    DEBUG_PRINT("Setting up camera with gluLookAt:");
    DEBUG_VECTOR("Eye", eye);
    DEBUG_VECTOR("Look target", Vector3D(eye.x + l.x, eye.y + l.y, eye.z + l.z));
    DEBUG_VECTOR("Up", u);
    
    gluLookAt(eye.x, eye.y, eye.z,
              eye.x + l.x, eye.y + l.y, eye.z + l.z,
              u.x, u.y, u.z);
    
    // Draw objects
    DEBUG_PRINT("Drawing " << objects.size() << " objects");
    for (size_t i = 0; i < objects.size(); i++) {
        DEBUG_PRINT("Drawing object " << i);
        objects[i]->draw(); // BREAKPOINT HERE for object rendering
    }
    
    // Draw point lights
    DEBUG_PRINT("Drawing " << pointLights.size() << " point lights");
    for (size_t i = 0; i < pointLights.size(); i++) {
        DEBUG_PRINT("Drawing point light " << i);
        pointLights[i].draw();
    }
    
    // Draw spot lights
    DEBUG_PRINT("Drawing " << spotLights.size() << " spot lights");
    for (size_t i = 0; i < spotLights.size(); i++) {
        DEBUG_PRINT("Drawing spot light " << i);
        spotLights[i].draw();
    }
    
    glutSwapBuffers();
    DEBUG_PRINT("display() completed");
}

void animate() {
    glutPostRedisplay();
}

void init() {
    DEBUG_PRINT("=== INITIALIZATION START ===");
    
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, 1, 1, 1000.0);
    
    DEBUG_PRINT("OpenGL initialization completed");
    
    calculateCamera(); // BREAKPOINT HERE for initial camera setup
    loadData(); // BREAKPOINT HERE for scene loading
    
    // Print initial camera state
    std::cout << "Initial camera state:" << std::endl;
    std::cout << "Eye: (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << std::endl;
    std::cout << "Look: (" << l.x << ", " << l.y << ", " << l.z << ")" << std::endl;
    std::cout << "Up: (" << u.x << ", " << u.y << ", " << u.z << ")" << std::endl;
    std::cout << "Right: (" << r.x << ", " << r.y << ", " << r.z << ")" << std::endl;
    
    DEBUG_PRINT("=== INITIALIZATION END ===");
}

int main(int argc, char** argv) {
    DEBUG_PRINT("=== MAIN START ===");
    DEBUG_PRINT("Command line arguments: " << argc);
    
    for (int i = 0; i < argc; i++) {
        DEBUG_PRINT("  argv[" << i << "] = " << argv[i]);
    }
    
    DEBUG_PRINT("Initializing GLUT");
    glutInit(&argc, argv); // BREAKPOINT HERE for GLUT initialization
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    
    DEBUG_PRINT("Creating window");
    glutCreateWindow("Ray Tracing Assignment - Debug Mode");
    
    DEBUG_PRINT("Calling init()");
    init(); // BREAKPOINT HERE for initialization
    
    glEnable(GL_DEPTH_TEST);
    DEBUG_PRINT("Depth testing enabled");
    
    DEBUG_PRINT("Setting up GLUT callbacks");
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboardListener); // BREAKPOINT HERE for keyboard setup
    glutSpecialFunc(specialKeyListener);
    
    // Print controls with debug info
    std::cout << "=== RAY TRACING CONTROLS ===" << std::endl;
    std::cout << "Arrow Keys: Move camera" << std::endl;
    std::cout << "Page Up/Down: Move up/down" << std::endl;
    std::cout << "1/2: Rotate left/right (YAW)" << std::endl;
    std::cout << "3/4: Look up/down (PITCH)" << std::endl;
    std::cout << "5/6: Tilt clockwise/counterclockwise (ROLL)" << std::endl;
    std::cout << "0: Capture image" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "DEBUG MODE: All function calls logged to console" << std::endl;
    std::cout << "Set breakpoints on rotation functions to debug step-by-step" << std::endl;
    
    DEBUG_PRINT("Starting GLUT main loop");
    glutMainLoop(); // BREAKPOINT HERE before main loop
    
    // Cleanup (this code may never be reached due to glutMainLoop)
    DEBUG_PRINT("Cleaning up objects");
    for (size_t i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
    
    DEBUG_PRINT("=== MAIN END ===");
    return 0;
}

// Ray tracing capture function
void capture() {
    DEBUG_PRINT("=== CAPTURE START ===");
    std::cout << "Capturing image..." << std::endl;
    
    // Initialize bitmap image
    DEBUG_PRINT("Creating bitmap image " << imageWidth << "x" << imageHeight);
    bitmap_image image(imageWidth, imageHeight);
    image.set_all_channels(0, 0, 0); // Black background
    
    // Calculate view plane parameters
    double planeDistance = (windowHeight / 2.0) / tan((viewAngle * M_PI / 180.0) / 2.0);
    DEBUG_PRINT("Plane distance: " << planeDistance);
    
    Vector3D topleft = eye + l * planeDistance - r * (windowWidth / 2) + u * (windowHeight / 2);
    DEBUG_VECTOR("Topleft corner", topleft);
    
    double du = windowWidth / imageWidth;
    double dv = windowHeight / imageHeight;
    DEBUG_PRINT("Pixel steps: du=" << du << ", dv=" << dv);
    
    // Choose middle of the grid cell
    topleft = topleft + r * (0.5 * du) - u * (0.5 * dv);
    DEBUG_VECTOR("Adjusted topleft", topleft);
    
    // Ray tracing loop
    DEBUG_PRINT("Starting ray tracing loop");
    for (int i = 0; i < imageWidth; i++) {
        for (int j = 0; j < imageHeight; j++) {
            // Calculate current pixel position
            Vector3D curPixel = topleft + r * (i * du) - u * (j * dv);
            
            // Cast ray from eye to current pixel
            Vector3D rayDir = (curPixel - eye).normalize();
            Ray ray(eye, rayDir); // BREAKPOINT HERE for ray creation
            
            // Find nearest intersection
            double t_min = 1e9;
            int nearest = -1;
            
            for (size_t k = 0; k < objects.size(); k++) {
                double dummy_color[3];
                double t = objects[k]->intersect(&ray, dummy_color, 0); // BREAKPOINT HERE for intersection
                if (t > 0 && t < t_min) {
                    t_min = t;
                    nearest = (int)k;
                }
            }
            
            // Calculate final color
            if (nearest != -1) {
                double pixel_color[3];
                objects[nearest]->intersect(&ray, pixel_color, 1); // BREAKPOINT HERE for color calculation
                
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
            DEBUG_PRINT("Ray tracing progress: " << (i * 100 / imageWidth) << "%");
            std::cout << "Progress: " << (i * 100 / imageWidth) << "%" << std::endl;
        }
    }
    
    // Save image with proper numbering
    std::stringstream filename;
    filename << "Output_" << imageCount << ".bmp";
    
    DEBUG_PRINT("Saving image as: " << filename.str());
    image.save_image(filename.str());
    imageCount++;
    
    std::cout << "Image saved as " << filename.str() << std::endl;
    DEBUG_PRINT("=== CAPTURE END ===");
}