#include "ray.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct HittableList obj_list = {0, NULL};

const double infinity = INFINITY;
const double pi = 3.1415926535897932385;


// Create a ray
struct Ray Ray(struct Vec origin, struct Vec direction)
{
    struct Ray ret;
    ret.dir = direction;
    ret.orig = origin;
    return ret;
}

// Return a point at a ray
struct Vec RayAt(const struct Ray ray, const double at)
{
    return VecAdd(ray.orig, VecIncr(ray.dir, at));
}

// Get background color for ray
struct Vec RayColor(const struct Ray ray)
{
    struct HitRecord *rec = RayListHit(ray, 0, infinity);
    if(rec)
    {
        struct Vec normal = VecUnit(VecSub(RayAt(ray, rec->distance), obj_list.obj[rec->list_index].center));
        // 0.5 * color(N.x + 1, N.y + 1, N.z + 1) * 255
        free(rec);
        return VecIncr(VecAdd(normal, Vec(1.0, 1.0, 1.0)), 127.5);
    }
    struct Vec dir = VecUnit(ray.dir);
    double blend = (dir.e[1] + 1.0) * 0.5; // Blend only by y coordinate value (-1 to 1) -> (0 to 1)
    return VecAdd(VecIncr(Vec(255.0, 255.0, 255.0), 1.0 - blend),
                          VecIncr(Vec(127.0, 178.0, 255.0), blend));
}

// 
struct HitRecord *RayHitSphere(const struct Vec center, double radius, const struct Ray ray,
                 double min, double max)
{
    struct Vec oc = VecSub(ray.orig, center);
    double a = VecLenSq(ray.dir);
    double half_b = VecDot(oc, ray.dir);
    double c = VecLenSq(oc) - radius*radius;
    double discriminant = half_b*half_b - a*c;
    if (discriminant < 0)
        return NULL;

    // Find the nearest root that lies in the acceptable range.
    double sqrtd = sqrt(discriminant);
    double root = (-half_b - sqrtd) / a;
    if (root < min || max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < min || max < root)
            return NULL;
    }

    struct HitRecord *rec = malloc(sizeof(struct HitRecord));
    rec->distance = root;
    rec->point = RayAt(ray, root);
    struct Vec outward_normal = VecDecr(VecSub(rec->point, center), radius);
    RecSetFaceNormal(rec, ray, outward_normal);

    return rec;
}

void RecSetFaceNormal(struct HitRecord *rec, const struct Ray ray, const struct Vec outward_normal)
{
    rec->front_face = VecDot(ray.dir, outward_normal) < 0;
    rec->normal = rec->front_face ? outward_normal :VecInv(outward_normal);
}

struct HitRecord *RayListHit(const struct Ray ray, double min, double max)
{
    struct HitRecord *rec = NULL;
    double closest_hit = max;
    for(int i = 0; i < obj_list.size; i++)
    {
        struct HitRecord *tmp = RayHitSphere(obj_list.obj[i].center, obj_list.obj[i].radius, ray, min, closest_hit);
        if(tmp)
        {
            closest_hit = tmp->distance;
            if(rec) free(rec);
            rec = tmp;
            rec->list_index = i;
        }
    }
    return rec;
}

int HittableAdd(struct Vec center, double radius)
{
    void *tmp = realloc(obj_list.obj, sizeof(struct Hittable) * (obj_list.size + 1));
    if(tmp)
    {
        obj_list.obj = tmp;
        obj_list.obj[obj_list.size].center = center;
        obj_list.obj[obj_list.size].radius = radius;
        obj_list.size++;
        return 1;
    }
    return 0;
}

double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}
