#ifndef CAMERA_H
#define CAMERA_H

#include <gtk/gtk.h>
#include "vec.h"
#include "ray.h"

struct Camera {
    struct Vec origin; // position of the camera
    struct Vec lower_left_corner; // lower left corner of the viewport
    struct Vec horizontal; // Viewport horizontal size
    struct Vec vertical; // Viewport vertical size
    GdkPixbuf *buf; // Rendered image
    int image_width; // Image size
    int image_height;
    int rowstride; // Size of one image row in bytes
    GtkWidget *draw_area; // Camera draing area widget
    int samples_per_pixel;
};

struct Camera *Camera(double aspect_ratio, double image_height);
struct Ray CameraGetRay(struct Camera *cam, double x, double y);
static void put_pixel (struct Camera *cam, int x, int y, struct Vec pixel);
struct Vec get_pixel (struct Camera *cam, int x, int y);
gboolean render(struct Camera *cam);


#endif // CAMERA_H