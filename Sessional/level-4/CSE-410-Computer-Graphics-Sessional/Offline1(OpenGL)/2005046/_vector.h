#include <cmath>
class Vector {
    public:
        float x, y, z;
    
        Vector(float x_=0, float y_=0, float z_=0) : x(x_), y(y_), z(z_) {}

        Vector operator+(const Vector& v) const {
            return Vector(x + v.x, y + v.y, z + v.z);
        }

        Vector operator-(const Vector& v) const {
            return Vector(x - v.x, y - v.y, z - v.z);
        }

        Vector operator*(float scalar) const {
            return Vector(x * scalar, y * scalar, z * scalar);
        }
    
        float dot(const Vector& v) const {
            return x * v.x + y * v.y + z * v.z;
        }
    
        Vector cross(const Vector& v) const {
            return Vector(
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            );
        }
    
        Vector normalize() const {
            float len = length();
            if (len == 0.0f) return Vector();
            return Vector(x / len, y / len, z / len);
        }
    
        float length() const {
            return sqrt(x*x + y*y + z*z);
        }
    
        Vector rotateAround(const Vector& axis, float radians) const {
            Vector u = axis.normalize();
            float cosR = cos(radians);
            float sinR = sin(radians);
    
            return (*this * cosR) +
                   (u.cross(*this) * sinR) +
                   (u * (u.dot(*this)) * (1.0f - cosR));
        }
        float magnitude() const {
            return length();
        }
    };
    