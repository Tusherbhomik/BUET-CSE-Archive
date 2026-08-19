#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cmath>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "bitmap_image.hpp"

using namespace std;
struct Point3D
{
    double x, y, z;
    Point3D()
    {
        x = y = z = 0;
    }

    Point3D(double a, double b, double c)
    {
        x = a;
        y = b;
        z = c;
    }
};

struct ColorRGB {
    int r, g, b;
    ColorRGB() {
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;
    }
};

struct Triangle
{
    Point3D vertices[3];
    ColorRGB color;

    Triangle() {
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
    }
};

struct Matrix4x4
{
    double m[4][4];

    Matrix4x4()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m[i][j] = 0.0;
            }
        }
    }

    void makeIdentity()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (i == j)
                {
                    m[i][j] = 1;
                }
                else
                {
                    m[i][j] = 0;
                }
            }
        }
    }
};

Point3D eye, look, up;
double fovY, aspectRatio, nearPlane, farPlane;

double degtoRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}

Point3D normalize(Point3D v)
{
    double len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len == 0)
        return Point3D(0, 0, 0);
    return Point3D(v.x / len, v.y / len, v.z / len);
}

Point3D subtract(Point3D a, Point3D b)
{
    return Point3D(a.x - b.x, a.y - b.y, a.z - b.z);
}

Point3D cross(Point3D a, Point3D b)
{
    return Point3D(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

double dot(Point3D a, Point3D b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Point3D rotateVector(Point3D v, Point3D axis, double angle)
{
    axis = normalize(axis);
    double cosA = cos(angle);
    double sinA = sin(angle);
    double dotProd = dot(v, axis);

    Point3D crossProd = cross(axis, v);

    return Point3D(
        v.x * cosA + crossProd.x * sinA + axis.x * dotProd * (1 - cosA),
        v.y * cosA + crossProd.y * sinA + axis.y * dotProd * (1 - cosA),
        v.z * cosA + crossProd.z * sinA + axis.z * dotProd * (1 - cosA));
}

Matrix4x4 multiply(Matrix4x4 a, Matrix4x4 b)
{
    Matrix4x4 result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}

Point3D transformPoint(Matrix4x4 mat, Point3D p)
{
    double homogeneous[4] = {p.x, p.y, p.z, 1.0};
    double result[4] = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result[i] += mat.m[i][j] * homogeneous[j];
        }
    }

    if (result[3] != 0 && result[3] != 1.0)
    {
        return Point3D(result[0] / result[3], result[1] / result[3], result[2] / result[3]);
    }
    return Point3D(result[0], result[1], result[2]);
}

Matrix4x4 generateTranslationMatrix(double tx, double ty, double tz)
{
    Matrix4x4 mat;
    mat.makeIdentity();
    mat.m[0][3] = tx;
    mat.m[1][3] = ty;
    mat.m[2][3] = tz;
    return mat;
}

Matrix4x4 generateScaleMatrix(double sx, double sy, double sz)
{
    Matrix4x4 mat;
    mat.makeIdentity();
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    mat.m[2][2] = sz;
    mat.m[3][3] = 1.0;
    return mat;
}

Matrix4x4 generateRotationMatrix(double angle, Point3D axis)
{
    Matrix4x4 mat;
    axis = normalize(axis);

    Point3D i(1, 0, 0), j(0, 1, 0), k(0, 0, 1);
    Point3D c1 = rotateVector(i, axis, degtoRadians(angle));
    Point3D c2 = rotateVector(j, axis, degtoRadians(angle));
    Point3D c3 = rotateVector(k, axis, degtoRadians(angle));

    mat.m[0][0] = c1.x; mat.m[0][1] = c2.x; mat.m[0][2] = c3.x; mat.m[0][3] = 0;
    mat.m[1][0] = c1.y; mat.m[1][1] = c2.y; mat.m[1][2] = c3.y; mat.m[1][3] = 0;
    mat.m[2][0] = c1.z; mat.m[2][1] = c2.z; mat.m[2][2] = c3.z; mat.m[2][3] = 0;
    mat.m[3][0] = 0;    mat.m[3][1] = 0;    mat.m[3][2] = 0;    mat.m[3][3] = 1;

    return mat;
}

void stage1()
{
    ifstream inputfile("scene.txt");
    ofstream outputfile("stage1.txt");

    inputfile >> eye.x >> eye.y >> eye.z;
    inputfile >> look.x >> look.y >> look.z;
    inputfile >> up.x >> up.y >> up.z;
    inputfile >> fovY >> aspectRatio >> nearPlane >> farPlane;

    stack<Matrix4x4> matrixStack;
    Matrix4x4 identityMatrix;
    identityMatrix.makeIdentity();
    matrixStack.push(identityMatrix);

    string command;
    while (inputfile >> command)
    {
        if (command == "triangle")
        {
            Point3D points[3];
            for (int i = 0; i < 3; i++)
            {
                inputfile >> points[i].x >> points[i].y >> points[i].z;
            }

            outputfile << fixed << setprecision(7);
            for (int i = 0; i < 3; i++)
            {
                Point3D transformed = transformPoint(matrixStack.top(), points[i]);
                outputfile << transformed.x << " " << transformed.y << " " << transformed.z << endl;
            }
            outputfile << endl;
        }
        else if (command == "translate")
        {
            double tx, ty, tz;
            inputfile >> tx >> ty >> tz;
            Matrix4x4 translateMat = generateTranslationMatrix(tx, ty, tz);
            Matrix4x4 currentTop = matrixStack.top();
            matrixStack.pop();
            matrixStack.push(multiply(currentTop, translateMat));
        }
        else if (command == "scale")
        {
            double sx, sy, sz;
            inputfile >> sx >> sy >> sz;
            Matrix4x4 scaleMat = generateScaleMatrix(sx, sy, sz);
            Matrix4x4 currentTop = matrixStack.top();
            matrixStack.pop();
            matrixStack.push(multiply(currentTop, scaleMat));
        }
        else if (command == "rotate")
        {
            double angle;
            Point3D axis;
            inputfile >> angle >> axis.x >> axis.y >> axis.z;
            Matrix4x4 rotateMat = generateRotationMatrix(angle, axis);
            Matrix4x4 currentTop = matrixStack.top();
            matrixStack.pop();
            matrixStack.push(multiply(currentTop, rotateMat));
        }
        else if (command == "push")
        {
            matrixStack.push(matrixStack.top());
        }
        else if (command == "pop")
        {
            if (matrixStack.size() > 1)
            {
                matrixStack.pop();
            }
        }
        else if (command == "end")
        {
            break;
        }
    }

    inputfile.close();
    outputfile.close();
}

void stage2()
{
    ifstream inputfile("stage1.txt");
    ofstream outputfile("stage2.txt");

    Point3D l = normalize(subtract(look, eye));
    Point3D r = normalize(cross(l, up));
    Point3D u = cross(r, l);

    Matrix4x4 T = generateTranslationMatrix(-eye.x, -eye.y, -eye.z);

    Matrix4x4 R;
    R.makeIdentity();
    R.m[0][0] = r.x; R.m[0][1] = r.y; R.m[0][2] = r.z;
    R.m[1][0] = u.x; R.m[1][1] = u.y; R.m[1][2] = u.z;
    R.m[2][0] = -l.x;R.m[2][1] = -l.y;R.m[2][2] = -l.z;
    
    Matrix4x4 V = multiply(R, T);

    outputfile << fixed << setprecision(7);
    Point3D vertex;
    string line;
    while (getline(inputfile, line)) {
        if(line.empty()){
            outputfile << endl;
            continue;
        }
        stringstream ss(line);
        ss >> vertex.x >> vertex.y >> vertex.z;
        Point3D transformed = transformPoint(V, vertex);
        outputfile << transformed.x << " " << transformed.y << " " << transformed.z << endl;
    }

    inputfile.close();
    outputfile.close();
}

void stage3()
{
    ifstream inputfile("stage2.txt");
    ofstream outputfile("stage3.txt");

    double fovX = fovY * aspectRatio;
    double t_val = nearPlane * tan(degtoRadians(fovY / 2.0));
    double r_val = nearPlane * tan(degtoRadians(fovX / 2.0));

    Matrix4x4 P;
    P.makeIdentity();
    P.m[0][0] = nearPlane / r_val;
    P.m[1][1] = nearPlane / t_val;
    P.m[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    P.m[2][3] = -(2 * farPlane * nearPlane) / (farPlane - nearPlane);
    P.m[3][2] = -1;
    P.m[3][3] = 0;

    outputfile << fixed << setprecision(7);
    Point3D vertex;
    string line;
    while (getline(inputfile, line)) {
         if(line.empty()){
            outputfile << endl;
            continue;
        }
        stringstream ss(line);
        ss >> vertex.x >> vertex.y >> vertex.z;
        Point3D transformed = transformPoint(P, vertex);
        outputfile << transformed.x << " " << transformed.y << " " << transformed.z << endl;
    }

    inputfile.close();
    outputfile.close();
}

void stage4() {
    int screen_width, screen_height;
    double left_x, right_x;
    double bottom_y, top_y;
    double z_front, z_rear;
    double dx, dy;
    double Top_Y, Left_X;
    
    vector<Triangle> triangles;
    vector<vector<double>> zbuffer;
    
    ifstream configfile("config.txt");
    configfile >> screen_width >> screen_height;
    
    double left_limit, bottom_limit;
    configfile >> left_limit;
    configfile >> bottom_limit;
    configfile >> z_front >> z_rear;
    
    left_x = left_limit;
    right_x = -left_limit;
    bottom_y = bottom_limit;
    top_y = -bottom_limit;
    
    dx = (right_x - left_x) / screen_width;
    dy = (top_y - bottom_y) / screen_height;
    Top_Y = top_y - dy / 2.0;
    Left_X = left_x + dx / 2.0;
    
    configfile.close();
    
    ifstream stage3file("stage3.txt");
    string line;
    while (getline(stage3file, line)) {
        if (line.empty()) continue;
        
        Triangle triangle;
        sscanf(line.c_str(), "%lf %lf %lf", &triangle.vertices[0].x, &triangle.vertices[0].y, &triangle.vertices[0].z);
        getline(stage3file, line);
        sscanf(line.c_str(), "%lf %lf %lf", &triangle.vertices[1].x, &triangle.vertices[1].y, &triangle.vertices[1].z);
        getline(stage3file, line);
        sscanf(line.c_str(), "%lf %lf %lf", &triangle.vertices[2].x, &triangle.vertices[2].y, &triangle.vertices[2].z);
        
        triangles.push_back(triangle);
    }
    stage3file.close();
    
    zbuffer = vector<vector<double>>(screen_height, vector<double>(screen_width, z_rear));
    bitmap_image image(screen_width, screen_height);
    image.clear(); 
    
    for (const Triangle& triangle : triangles) {
        Point3D vertices[3] = {triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]};
        
        double min_y = min({vertices[0].y, vertices[1].y, vertices[2].y});
        double max_y = max({vertices[0].y, vertices[1].y, vertices[2].y});
        
        min_y = max(min_y, bottom_y);
        max_y = min(max_y, top_y);
        
        if (min_y >= max_y) continue; 
        
        int top_scanline = max(0, (int)round((Top_Y - max_y) / dy));
        int bottom_scanline = min(screen_height - 1, (int)round((Top_Y - min_y) / dy));
        
        for (int row = top_scanline; row <= bottom_scanline; row++) {
            double y = Top_Y - row * dy;
            
            vector<pair<double, double>> intersections;
            
            for (int i = 0; i < 3; i++) {
                Point3D p1 = vertices[i];
                Point3D p2 = vertices[(i + 1) % 3];
                
                if ((p1.y <= y && y <= p2.y) || (p2.y <= y && y <= p1.y)) {
                    if (abs(p2.y - p1.y) < 1e-9) {
                        if (abs(y - p1.y) < 1e-9) {
                            intersections.push_back(make_pair(p1.x, p1.z));
                            intersections.push_back(make_pair(p2.x, p2.z));
                        }
                    } else {
                        double t = (y - p1.y) / (p2.y - p1.y);
                        if (t >= 0 && t <= 1) {
                            double x = p1.x + t * (p2.x - p1.x);
                            double z = p1.z + t * (p2.z - p1.z);
                            intersections.push_back(make_pair(x, z));
                        }
                    }
                }
            }
            
            sort(intersections.begin(), intersections.end());
            
            for (int i = 0; i < (int)intersections.size() - 1; i += 2) {
                if (i + 1 >= intersections.size()) break;
                
                double x1 = intersections[i].first;
                double z1 = intersections[i].second;
                double x2 = intersections[i + 1].first;
                double z2 = intersections[i + 1].second;
                
                x1 = max(x1, left_x);
                x2 = min(x2, right_x);
                
                if (x1 >= x2) continue;
                
                int left_col = max(0, (int)round((x1 - Left_X) / dx));
                int right_col = min(screen_width - 1, (int)round((x2 - Left_X) / dx));
                
                for (int col = left_col; col <= right_col; col++) {
                    double pixel_x = Left_X + col * dx;
                    
                    double z;
                    if (abs(x2 - x1) < 1e-9) {
                        z = z1;
                    } else {
                        double t = (pixel_x - x1) / (x2 - x1);
                        z = z1 + t * (z2 - z1);
                    }
                    
                    if (z >= z_front && z <= z_rear && z < zbuffer[row][col]) {
                        zbuffer[row][col] = z;
                        image.set_pixel(col, row, triangle.color.r, triangle.color.g, triangle.color.b);
                    }
                }
            }
        }
    }
    
    image.save_image("out.bmp");
    
    ofstream zbufferFile("z-buffer.txt");
    for (int row = 0; row < screen_height; row++) {
        for (int col = 0; col < screen_width; col++) {
            if (zbuffer[row][col] < z_rear) {
                zbufferFile<<fixed<<setprecision(6) << zbuffer[row][col] << "\t";
            }
        }
        zbufferFile << endl;
    }
    zbufferFile.close();
}
int main()
{
    srand(time(0));

    cout << "Stage 1 starts" << endl;
    stage1();
    cout << "Stage 2 starts" << endl;
    stage2();
    cout << "Stage 3 starts" << endl;
    stage3();
    cout << "Stage 4 starts" << endl;
    stage4();
    cout << "shes" << endl;
    return 0;
}

//g++ 2005046.cpp -o my_graphics_pipeline
//./my_graphics_pipeline