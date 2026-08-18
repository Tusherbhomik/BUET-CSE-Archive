#include <iostream>
#include <cmath>

// Simple Vector3D for testing
class Vector3D {
public:
    double x, y, z;
    
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
    
    Vector3D operator+(const Vector3D& v) const {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }
    
    Vector3D operator-(const Vector3D& v) const {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }
    
    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }
    
    Vector3D cross(const Vector3D& v) const {
        return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    
    double length() const {
        return sqrt(x * x + y * y + z * z);
    }
    
    Vector3D normalize() const {
        double len = length();
        if (len == 0) return Vector3D(0, 0, 0);
        return Vector3D(x / len, y / len, z / len);
    }
    
    void print(const std::string& name) const {
        std::cout << name << ": (" << x << ", " << y << ", " << z << ")" << std::endl;
    }
};

// Test vectors
Vector3D l(-1, -1, 0);  // Initial look direction
Vector3D up(0, 0, 1);   // Global up
Vector3D r, u;          // Right and camera up

void calculateCamera() {
    l = l.normalize();
    r = l.cross(up).normalize();
    u = r.cross(l).normalize();
}

void testRotateLeft() {
    std::cout << "\n=== Testing Rotate Left ===" << std::endl;
    
    // Print initial state
    std::cout << "BEFORE rotation:" << std::endl;
    l.print("Look");
    r.print("Right");
    u.print("Up");
    
    // Rotate left (around up vector)
    double angle = 0.1;
    Vector3D new_l = l * cos(angle) + r * sin(angle);
    Vector3D new_r = l * (-sin(angle)) + r * cos(angle);
    
    l = new_l.normalize();
    r = new_r.normalize();
    u = r.cross(l).normalize();
    
    std::cout << "\nAFTER rotation:" << std::endl;
    l.print("Look");
    r.print("Right");
    u.print("Up");
    
    // Verify vectors are still orthonormal
    std::cout << "\nOrthonormality check:" << std::endl;
    std::cout << "Look length: " << l.length() << std::endl;
    std::cout << "Right length: " << r.length() << std::endl;
    std::cout << "Up length: " << u.length() << std::endl;
}

int main() {
    std::cout << "Testing Rotation Mathematics" << std::endl;
    std::cout << "============================" << std::endl;
    
    // Initialize camera
    calculateCamera();
    
    std::cout << "\nInitial camera setup:" << std::endl;
    l.print("Look");
    r.print("Right");
    u.print("Up");
    
    // Test rotation
    testRotateLeft();
    
    std::cout << "\nIf the Look direction changed, rotation math is working!" << std::endl;
    std::cout << "If vectors remain length 1.0, normalization is working!" << std::endl;
    
    return 0;
}