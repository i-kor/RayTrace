#include <stdlib.h>
#include "camera.h"
#include "utility.h"

// Create a new camera
struct Camera *Camera(double aspect_ratio, double image_height)
{
    
    double viewport_height = 2.0;
    double viewport_width = viewport_height * aspect_ratio;
    double focal_length = 1.0; // Distance from the camera to the viewport

    struct Camera *camera = malloc(sizeof(struct Camera));
    camera->origin = Vec(.0, .0, .0);
    // Size of thew viewport
    camera->horizontal = Vec(viewport_width, .0, .0);
    camera->vertical = Vec(.0, -viewport_height, .0); // Y coord positive should be up
    // Coordinates of a lower left corner of the viewport
    // origin - horizontal/2 - vertical/2 - focal_length
    camera->lower_left_corner = VecSub(VecSub(VecSub(camera->origin, VecDecr(camera->horizontal, 2.0)),
                        VecDecr(camera->vertical, 2.0)), Vec(.0, .0, focal_length));
    
    // Create an image and save it's rowstride
    camera->image_width = image_height * aspect_ratio;
    camera->image_height = image_height;
    camera->buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, camera->image_width, camera->image_height);
    camera->rowstride = gdk_pixbuf_get_rowstride(camera->buf);
    gdk_pixbuf_fill(camera->buf, 0x00000000);

    camera->samples_per_pixel = 100;

    return camera;
}

// Create a ray from camera origin to a viewport point
// x, y values are from 0 to 1
struct Ray CameraGetRay(struct Camera *cam, double x, double y)
{
    return Ray(cam->origin,
               // lower_left_corner + u*horizontal + v*vertical - origin
               VecSub(VecAdd(VecAdd(cam->lower_left_corner, VecIncr(cam->horizontal, x)),
                             VecIncr(cam->vertical, y)),
                      cam->origin));

}

// Put a pixel to camera rendered image at x,y coords
static void put_pixel (struct Camera *cam, int x, int y, struct Vec pixel)
{
    if(x < cam->image_width && y < cam->image_height)
    {
        guchar *pixels = gdk_pixbuf_get_pixels (cam->buf);
        guchar *p = pixels + y * cam->rowstride + x * 3;
        for(int i = 0; i < 3; i ++)
            p[i] = clamp(pixel.e[i]/cam->samples_per_pixel, 0.0, 255.0);
    }
}

// Get a pixel from camera rendered image
struct Vec get_pixel (struct Camera *cam, int x, int y)
{
    if(x < cam->image_width && y < cam->image_height)
    {
        guchar *pixels = gdk_pixbuf_get_pixels (cam->buf);
        guchar *p = pixels + y * cam->rowstride + x * 3;
        return Vec(p[0], p[1], p[2]);
    }
    return Vec(.0, .0, .0);
}

// Renders a 3D scene image to GdkPixbuf
gboolean render(struct Camera *cam)
{
    for(int j = 0; j < cam->image_height; j++)
    {
        for(int i = 0; i < cam->image_width; i++)
            {
                double x = (double)i / (cam->image_width - 1);
                double y = (double)j / (cam->image_height - 1);
                // Translate to 0..1 viewport coords
                struct Vec color = Vec(0.0, 0.0, 0.0);
                for(int k = 0; k < cam->samples_per_pixel; k++)
                {
                    double x_diff = ((double)i + random_double_unit()) / (cam->image_width - 1);
                    double y_diff = ((double)j + random_double_unit()) / (cam->image_height - 1);
                    // Cast ray from origin through a viewport point
                    struct Ray ray = CameraGetRay(cam, x_diff, y_diff);
                    color = VecAdd(color, RayColor(ray));
                }
                put_pixel(cam, i, j, color);
            }
        printf("\rRendering line %d/%d", j+1, cam->image_height);
    }
    printf("\n");
    return G_SOURCE_REMOVE;
}