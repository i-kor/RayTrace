#ifndef RAY_H
#define RAY_H

#include "vec.h"

struct Ray {
    struct Vec orig; // Origin of the ray
    struct Vec dir; // Vector direction of the ray
};

struct HittableList {
    int size; // Number of objects in the list
    struct Hittable {
        struct Vec center; // Center of the sphere
        double radius; // Radius of the sphere
    } *obj; // Array of objects
};

struct HitRecord {
    struct Vec point;
    struct Vec normal;
    double distance;
    int front_face; // Normal is outside
    int list_index;
};

struct Ray Ray(const struct Vec origin, const struct Vec direction);
struct Vec RayAt(const struct Ray left, const double at);
struct Vec RayColor(const struct Ray ray);
struct HitRecord *RayHitSphere(const struct Vec center, double radius, const struct Ray ray, double min, double max);
void RecSetFaceNormal(struct HitRecord *rec, const struct Ray ray, const struct Vec outward_normal);
struct HitRecord *RayListHit(const struct Ray ray, double min, double max);
int HittableAdd(struct Vec center, double radius);
double degrees_to_radians(double degrees);


#endif //RAY_H