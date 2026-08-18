#include "2005046_classes.hpp"
#include <algorithm>
#include <cmath>
using namespace std;

bool isObscured(Vector3D start, Vector3D end)
{
    Vector3D dir = (end - start).normalize();
    double distance = (end - start).length();
    Ray shadowRay(start, dir);

    for (size_t i = 0; i < objects.size(); i++)
    {
        double color[3];
        double t = objects[i]->intersect(&shadowRay, color, 0);
        if (t > 1e-6 && t < distance - 1e-6)
        {
            return true;
        }
    }
    return false;
}
void computeLightingAndReflection(Ray *r, Vector3D intersectionPoint, Vector3D normal, Vector3D intersectionPointColor, 
                                 double *coEfficients, double shine, int level, double *color, int recursion_level)
{
    color[0] = intersectionPointColor.x * coEfficients[0];
    color[1] = intersectionPointColor.y * coEfficients[0];
    color[2] = intersectionPointColor.z * coEfficients[0];

    for (size_t i = 0; i < pointLights.size(); i++)
    {
        Vector3D lightDir = (pointLights[i].light_pos - intersectionPoint).normalize();

        if (!isObscured(intersectionPoint + normal * 1e-6, pointLights[i].light_pos))
        {
            double lambertValue = max(0.0, normal.dot(lightDir));

            color[0] += pointLights[i].color[0] * coEfficients[1] * lambertValue * intersectionPointColor.x;
            color[1] += pointLights[i].color[1] * coEfficients[1] * lambertValue * intersectionPointColor.y;
            color[2] += pointLights[i].color[2] * coEfficients[1] * lambertValue * intersectionPointColor.z;

            Vector3D reflected = lightDir - normal * (2 * lightDir.dot(normal));
            Vector3D viewDir = (r->start - intersectionPoint).normalize();
            double phongValue = max(0.0, reflected.dot(viewDir));
            phongValue = pow(phongValue, shine);

            color[0] += pointLights[i].color[0] * coEfficients[2] * phongValue;
            color[1] += pointLights[i].color[1] * coEfficients[2] * phongValue;
            color[2] += pointLights[i].color[2] * coEfficients[2] * phongValue;
        }
    }

    for (size_t i = 0; i < spotLights.size(); i++)
    {
        Vector3D lightDir = (spotLights[i].point_light.light_pos - intersectionPoint).normalize();
        Vector3D spotDir = spotLights[i].light_direction.normalize();

        double angle = acos((-lightDir).dot(spotDir)) * 180.0 / M_PI;
        if (angle <= spotLights[i].cutoff_angle)
        {
            if (!isObscured(intersectionPoint + normal * 1e-6, spotLights[i].point_light.light_pos))
            {
                double lambertValue = max(0.0, normal.dot(lightDir));

                color[0] += spotLights[i].point_light.color[0] * coEfficients[1] * lambertValue * intersectionPointColor.x;
                color[1] += spotLights[i].point_light.color[1] * coEfficients[1] * lambertValue * intersectionPointColor.y;
                color[2] += spotLights[i].point_light.color[2] * coEfficients[1] * lambertValue * intersectionPointColor.z;

                Vector3D reflected = lightDir - normal * (2 * lightDir.dot(normal));
                Vector3D viewDir = (r->start - intersectionPoint).normalize();
                double phongValue = max(0.0, reflected.dot(viewDir));
                phongValue = pow(phongValue, shine);

                color[0] += spotLights[i].point_light.color[0] * coEfficients[2] * phongValue;
                color[1] += spotLights[i].point_light.color[1] * coEfficients[2] * phongValue;
                color[2] += spotLights[i].point_light.color[2] * coEfficients[2] * phongValue;
            }
        }
    }

    if (level < recursion_level)
    {
        Vector3D reflected = r->dir - normal * (2 * r->dir.dot(normal));
        Ray reflectedRay(intersectionPoint + normal * 1e-6, reflected);

        double t_min = 1e9;
        int nearest = -1;

        for (size_t i = 0; i < objects.size(); i++)
        {
            double temp_color[3];
            double temp_t = objects[i]->intersect(&reflectedRay, temp_color, 0);
            if (temp_t > 0 && temp_t < t_min)
            {
                t_min = temp_t;
                nearest = (int)i;
            }
        }

        if (nearest != -1)
        {
            double reflected_color[3];
            objects[nearest]->intersect(&reflectedRay, reflected_color, level + 1);

            color[0] += reflected_color[0] * coEfficients[3];
            color[1] += reflected_color[1] * coEfficients[3];
            color[2] += reflected_color[2] * coEfficients[3];
        }
    }

    color[0] = clamp(color[0], 0.0, 1.0);
    color[1] = clamp(color[1], 0.0, 1.0);
    color[2] = clamp(color[2], 0.0, 1.0);
}



double Sphere::intersect(Ray *r, double *color, int level)
{
    Vector3D oc = r->start - reference_point;
    double a = r->dir.dot(r->dir);
    double b = 2.0 * oc.dot(r->dir);
    double c = oc.dot(oc) - length * length;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return -1;
    }

    double t1 = (-b - sqrt(discriminant)) / (2 * a);
    double t2 = (-b + sqrt(discriminant)) / (2 * a);

    double t = -1;
    if (t1 > 1e-6)
    {
        t = t1;
    }
    else if (t2 > 1e-6)
    {
        t = t2;
    }

    if (t < 0 || level == 0)
    {
        return t;
    }

    Vector3D intersectionPoint = r->start + r->dir * t;
    Vector3D normal = getNormal(intersectionPoint);
    Vector3D intersectionPointColor = getColorAt(intersectionPoint);

    computeLightingAndReflection(r, intersectionPoint, normal, intersectionPointColor, coEfficients, shine, level, color, recursion_level);

    return t;
}

double Triangle::intersect(Ray *r, double *color, int level)
{
    Vector3D edge1 = b - a;
    Vector3D edge2 = c - a;
    Vector3D h = r->dir.cross(edge2);
    double det = edge1.dot(h);

    if (det > -1e-6 && det < 1e-6)
    {
        return -1;
    }

    double inv_det = 1.0 / det;
    Vector3D s = r->start - a;
    double u = inv_det * s.dot(h);

    if (u < 0.0 || u > 1.0)
    {
        return -1;
    }

    Vector3D q = s.cross(edge1);
    double v = inv_det * r->dir.dot(q);

    if (v < 0.0 || u + v > 1.0)
    {
        return -1;
    }

    double t = inv_det * edge2.dot(q);

    if (t <= 1e-6 || level == 0)
    {
        return t;
    }

    Vector3D intersectionPoint = r->start + r->dir * t;
    Vector3D normal = getNormal(intersectionPoint);
    Vector3D intersectionPointColor = getColorAt(intersectionPoint);

    computeLightingAndReflection(r, intersectionPoint, normal, intersectionPointColor, coEfficients, shine, level, color, recursion_level);

    return t;
}

double GeneralQuadric::intersect(Ray *r, double *color, int level)
{
    double x0 = r->start.x, y0 = r->start.y, z0 = r->start.z;
    double xd = r->dir.x, yd = r->dir.y, zd = r->dir.z;

    double a_coeff = A * xd * xd + B * yd * yd + C * zd * zd + D * xd * yd + E * xd * zd + F * yd * zd;
    double b_coeff = 2 * A * x0 * xd + 2 * B * y0 * yd + 2 * C * z0 * zd + D * (x0 * yd + y0 * xd) +
                     E * (x0 * zd + z0 * xd) + F * (y0 * zd + z0 * yd) + G * xd + H * yd + I * zd;
    double c_coeff = A * x0 * x0 + B * y0 * y0 + C * z0 * z0 + D * x0 * y0 + E * x0 * z0 + F * y0 * z0 +
                     G * x0 + H * y0 + I * z0 + J;

    double discriminant = b_coeff * b_coeff - 4 * a_coeff * c_coeff;

    if (discriminant < 0)
    {
        return -1;
    }

    double t1 = (-b_coeff - sqrt(discriminant)) / (2 * a_coeff);
    double t2 = (-b_coeff + sqrt(discriminant)) / (2 * a_coeff);

    double t = -1;
    Vector3D intersectionPoint;

    if (t1 > 1e-6)
    {
        intersectionPoint = r->start + r->dir * t1;
        bool inBounds = true;

        if (cube_length > 0)
        {
            if (intersectionPoint.x < cube_ref_point.x ||
                intersectionPoint.x > cube_ref_point.x + cube_length)
            {
                inBounds = false;
            }
        }
        if (cube_width > 0)
        {
            if (intersectionPoint.y < cube_ref_point.y ||
                intersectionPoint.y > cube_ref_point.y + cube_width)
            {
                inBounds = false;
            }
        }
        if (cube_height > 0)
        {
            if (intersectionPoint.z < cube_ref_point.z ||
                intersectionPoint.z > cube_ref_point.z + cube_height)
            {
                inBounds = false;
            }
        }

        if (inBounds)
        {
            t = t1;
        }
    }

    if (t < 0 && t2 > 1e-6)
    {
        intersectionPoint = r->start + r->dir * t2;
        bool inBounds = true;

        if (cube_length > 0)
        {
            if (intersectionPoint.x < cube_ref_point.x ||
                intersectionPoint.x > cube_ref_point.x + cube_length)
            {
                inBounds = false;
            }
        }
        if (cube_width > 0)
        {
            if (intersectionPoint.y < cube_ref_point.y ||
                intersectionPoint.y > cube_ref_point.y + cube_width)
            {
                inBounds = false;
            }
        }
        if (cube_height > 0)
        {
            if (intersectionPoint.z < cube_ref_point.z ||
                intersectionPoint.z > cube_ref_point.z + cube_height)
            {
                inBounds = false;
            }
        }

        if (inBounds)
        {
            t = t2;
        }
    }

    if (t < 0 || level == 0)
    {
        return t;
    }

    intersectionPoint = r->start + r->dir * t;
    Vector3D normal = getNormal(intersectionPoint);
    Vector3D intersectionPointColor = getColorAt(intersectionPoint);

    computeLightingAndReflection(r, intersectionPoint, normal, intersectionPointColor, coEfficients, shine, level, color, recursion_level);

    return t;
}

double Floor::intersect(Ray *r, double *color, int level)
{
    if (abs(r->dir.z) < 1e-6)
    {
        return -1;
    }

    double t = -r->start.z / r->dir.z;

    if (t <= 1e-6)
    {
        return -1;
    }

    Vector3D intersectionPoint = r->start + r->dir * t;

    if (intersectionPoint.x < reference_point.x ||
        intersectionPoint.x > reference_point.x + floorWidth ||
        intersectionPoint.y < reference_point.y ||
        intersectionPoint.y > reference_point.y + floorWidth)
    {
        return -1;
    }

    if (level == 0)
    {
        return t;
    }

    Vector3D normal = getNormal(intersectionPoint);
    Vector3D intersectionPointColor = getColorAt(intersectionPoint);

    computeLightingAndReflection(r, intersectionPoint, normal, intersectionPointColor, coEfficients, shine, level, color, recursion_level);

    return t;
}