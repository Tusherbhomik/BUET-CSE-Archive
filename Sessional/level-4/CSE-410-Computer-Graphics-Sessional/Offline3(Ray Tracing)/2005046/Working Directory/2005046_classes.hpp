#ifndef CLASSES_HPP
#define CLASSES_HPP
#include "stb_image.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Forward declarations
class Ray;

// Vector3D class for 3D operations
class Vector3D
{
public:
    double x, y, z;

    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

    Vector3D operator+(const Vector3D &v) const
    {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }

    Vector3D operator-(const Vector3D &v) const
    {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    // FIXED: Added unary minus operator
    Vector3D operator-() const
    {
        return Vector3D(-x, -y, -z);
    }

    Vector3D operator*(double scalar) const
    {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    double dot(const Vector3D &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3D cross(const Vector3D &v) const
    {
        return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    double length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3D normalize() const
    {
        double len = length();
        if (len == 0)
            return Vector3D(0, 0, 0);
        return Vector3D(x / len, y / len, z / len);
    }

    // Assignment operator
    Vector3D &operator=(const Vector3D &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
};

// Ray class
class Ray
{
public:
    Vector3D start;
    Vector3D dir;

    Ray() {}
    Ray(Vector3D start, Vector3D dir) : start(start), dir(dir.normalize()) {}
};

// Base Object class
class Object
{
public:
    Vector3D reference_point;
    double height, width, length;
    double color[3];
    double coEfficients[4]; // ambient, diffuse, specular, reflection coefficients
    int shine;              // exponent term of specular component

    Object()
    {
        color[0] = color[1] = color[2] = 0;
        coEfficients[0] = coEfficients[1] = coEfficients[2] = coEfficients[3] = 0;
        shine = 0;
        height = width = length = 0;
    }

    virtual ~Object() {}
    virtual void draw() {}
    virtual double intersect(Ray *r, double *color, int level) { return -1.0; }
    virtual Vector3D getNormal(Vector3D point) { return Vector3D(0, 0, 1); }
    virtual Vector3D getColorAt(Vector3D point)
    {
        return Vector3D(color[0], color[1], color[2]);
    }

    void setColor(double r, double g, double b)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void setShine(int s)
    {
        shine = s;
    }

    void setCoEfficients(double amb, double diff, double spec, double refl)
    {
        coEfficients[0] = amb;
        coEfficients[1] = diff;
        coEfficients[2] = spec;
        coEfficients[3] = refl;
    }
};

// Sphere class
class Sphere : public Object
{
public:
    Sphere(Vector3D center, double radius)
    {
        reference_point = center;
        length = radius; // using length to store radius
    }

    void draw() override
    {
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        glColor3f(color[0], color[1], color[2]);
        glutSolidSphere(length, 24, 24);
        glPopMatrix();
    }

    double intersect(Ray *r, double *color, int level) override;
    Vector3D getNormal(Vector3D point) override
    {
        return (point - reference_point).normalize();
    }
};

// Triangle class
class Triangle : public Object
{
public:
    Vector3D a, b, c;

    Triangle(Vector3D p1, Vector3D p2, Vector3D p3) : a(p1), b(p2), c(p3) {}

    void draw() override
    {
        glBegin(GL_TRIANGLES);
        glColor3f(color[0], color[1], color[2]);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glEnd();
    }

    double intersect(Ray *r, double *color, int level) override;
    Vector3D getNormal(Vector3D point) override
    {
        Vector3D edge1 = b - a;
        Vector3D edge2 = c - a;
        return edge1.cross(edge2).normalize();
    }
};

// General Quadric Surface class
class GeneralQuadric : public Object
{
public:
    double A, B, C, D, E, F, G, H, I, J;
    Vector3D cube_ref_point;
    double cube_length, cube_width, cube_height;

    GeneralQuadric(double a, double b, double c, double d, double e, double f,
                   double g, double h, double i, double j,
                   Vector3D ref, double len, double wid, double hei)
        : A(a), B(b), C(c), D(d), E(e), F(f), G(g), H(h), I(i), J(j),
          cube_ref_point(ref), cube_length(len), cube_width(wid), cube_height(hei) {}

    void draw() override
    {
        // General quadric surfaces are only rendered in ray tracing
    }

    double intersect(Ray *r, double *color, int level) override;
    Vector3D getNormal(Vector3D point) override
    {
        double dx = 2 * A * point.x + D * point.y + E * point.z + G;
        double dy = 2 * B * point.y + D * point.x + F * point.z + H;
        double dz = 2 * C * point.z + E * point.x + F * point.y + I;
        return Vector3D(dx, dy, dz).normalize();
    }
};

// TEXTURE SUPPORT ADDED: Floor class with texture capability
class Floor : public Object
{
private:
    // NEW: Texture data members
    unsigned char *textureData;
    int textureWidth;
    int textureHeight;
    int textureChannels;
    bool useTexture;

    // NEW: Texture sampling method
    Vector3D sampleTexture(double u, double v)
    {
        if (!textureData || textureWidth <= 0 || textureHeight <= 0)
        {
            return Vector3D(0.5, 0.5, 0.5); // Gray fallback
        }

        // Clamp u and v to [0,1]
        u = std::max(0.0, std::min(1.0, u));
        v = std::max(0.0, std::min(1.0, v));

        // Normalized -> pixel coords
        int pixel_x = (int)(u * (textureWidth - 1));
        int pixel_y = (int)((1.0 - v) * (textureHeight - 1)); // Flip Y

        // Safety clamp
        pixel_x = std::max(0, std::min(textureWidth - 1, pixel_x));
        pixel_y = std::max(0, std::min(textureHeight - 1, pixel_y));

        // Compute array index
        int index = (pixel_y * textureWidth + pixel_x) * textureChannels;
        int max_index = textureWidth * textureHeight * textureChannels;

        if (index < 0 || index + 2 >= max_index)
        {
            return Vector3D(1.0, 0.0, 1.0); // Magenta = error
        }

        Vector3D color;
        color.x = textureData[index] / 255.0;

        if (textureChannels >= 2)
        {
            color.y = textureData[index + 1] / 255.0;
        }
        else
        {
            color.y = color.x; // Grayscale
        }

        if (textureChannels >= 3)
        {
            color.z = textureData[index + 2] / 255.0;
        }
        else
        {
            color.z = color.x; // Grayscale
        }

        return color;
    }

public:
    double floorWidth, tileWidth;

    Floor(double fw, double tw) : floorWidth(fw), tileWidth(tw)
    {
        reference_point = Vector3D(-fw / 2, -fw / 2, 0);
        length = tw;

        // NEW: Initialize texture data
        textureData = nullptr;
        textureWidth = 0;
        textureHeight = 0;
        textureChannels = 0;
        useTexture = false;
    }

    // NEW: Destructor to clean up texture data
    ~Floor()
    {
        if (textureData)
        {
            delete[] textureData;
        }
    }

    // NEW: Method to load texture

    bool loadTexture(const char *filename)
    {
        if (textureData)
        {
            delete[] textureData;
            textureData = nullptr;
        }

        // Load the image using stb_image
        unsigned char *data = stbi_load(filename, &textureWidth, &textureHeight, &textureChannels, 0);

        if (!data)
        {
            std::cout << "Failed to load texture: " << filename << std::endl;
            std::cout << "STB Error: " << stbi_failure_reason() << std::endl;
            return false;
        }

        // Copy the data to our own buffer
        int dataSize = textureWidth * textureHeight * textureChannels;
        textureData = new unsigned char[dataSize];
        memcpy(textureData, data, dataSize);

        // Free the stb_image data
        stbi_image_free(data);

        std::cout << "Successfully loaded texture: " << filename << std::endl;
        std::cout << "Dimensions: " << textureWidth << "x" << textureHeight << std::endl;
        std::cout << "Channels: " << textureChannels << std::endl;

        return true;
    }
    // NEW: Method to toggle between texture and checkerboard
    void setUseTexture(bool use)
    {
        useTexture = use;
    }

    void draw() override
    {
        int tiles = (int)(floorWidth / tileWidth);
        glBegin(GL_QUADS);
        for (int i = 0; i < tiles; i++)
        {
            for (int j = 0; j < tiles; j++)
            {
                if ((i + j) % 2 == 0)
                {
                    glColor3f(1, 1, 1); // White
                }
                else
                {
                    glColor3f(0, 0, 0); // Black
                }

                double x1 = reference_point.x + i * tileWidth;
                double y1 = reference_point.y + j * tileWidth;
                double x2 = x1 + tileWidth;
                double y2 = y1 + tileWidth;

                glVertex3f(x1, y1, 0);
                glVertex3f(x2, y1, 0);
                glVertex3f(x2, y2, 0);
                glVertex3f(x1, y2, 0);
            }
        }
        glEnd();
    }

    double intersect(Ray *r, double *color, int level) override;
    Vector3D getNormal(Vector3D point) override
    {
        return Vector3D(0, 0, 1);
    }

    // MODIFIED: getColorAt method to place texture in each tile
    Vector3D getColorAt(Vector3D point) override
    {
        if (useTexture && textureData)
        {
            // Calculate tile coordinates
            double localX = point.x - reference_point.x;
            double localY = point.y - reference_point.y;

            // Find which tile we're in
            int tileI = (int)(localX / tileWidth);
            int tileJ = (int)(localY / tileWidth);

            // Calculate position within the current tile
            double tileLocalX = localX - (tileI * tileWidth);
            double tileLocalY = localY - (tileJ * tileWidth);

            // Convert to texture coordinates [0,1] within this tile
            double u = tileLocalX / tileWidth;
            double v = tileLocalY / tileWidth;

            // Ensure coordinates are in [0,1] range
            u = std::max(0.0, std::min(1.0, u));
            v = std::max(0.0, std::min(1.0, v));

            return sampleTexture(u, v);
        }
        else
        {
            // Original checkerboard pattern
            int i = (int)((point.x - reference_point.x) / tileWidth);
            int j = (int)((point.y - reference_point.y) / tileWidth);
            if ((i + j) % 2 == 0)
            {
                return Vector3D(1, 1, 1); // White
            }
            else
            {
                return Vector3D(0, 0, 0); // Black
            }
        }
    }
};

// Point Light class
class PointLight
{
public:
    Vector3D light_pos;
    double color[3];

    PointLight()
    {
        color[0] = color[1] = color[2] = 1;
    }

    PointLight(Vector3D pos, double r, double g, double b) : light_pos(pos)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void draw()
    {
        glPushMatrix();
        glTranslatef(light_pos.x, light_pos.y, light_pos.z);
        glColor3f(color[0], color[1], color[2]);
        glutSolidSphere(2, 8, 8);
        glPopMatrix();
    }
};

// Spot Light class
class SpotLight
{
public:
    PointLight point_light;
    Vector3D light_direction;
    double cutoff_angle;

    SpotLight() : cutoff_angle(30) {}

    SpotLight(Vector3D pos, double r, double g, double b, Vector3D dir, double cutoff)
        : point_light(pos, r, g, b), light_direction(dir.normalize()), cutoff_angle(cutoff) {}

    void draw()
    {
        point_light.draw();
    }
};

// Global variables (extern declarations)
extern std::vector<Object *> objects;
extern std::vector<PointLight> pointLights;
extern std::vector<SpotLight> spotLights;
extern int recursion_level;
extern int imageWidth, imageHeight;

// Function declarations
void loadData();
void capture();
double clamp(double value, double min_val, double max_val);

#endif