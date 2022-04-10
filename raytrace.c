#include <gtk/gtk.h>
#include <math.h>

#include "vec.h"
#include "ray.h"

const double aspect_ratio = 16.0/9.0;
const double image_width = 640;
const double image_height = image_width/aspect_ratio;
const double color_max = 255;
int rowstride;

const double viewport_height = 2.0;
const double viewport_width = viewport_height * aspect_ratio;
const double focal_length = 1.0;

struct Vec origin;
struct Vec horizontal;
struct Vec vertical;
struct Vec lower_left_corner;

static void put_pixel (GdkPixbuf *pixbuf, int x, int y, struct Vec pixel)
{
    guchar *pixels = gdk_pixbuf_get_pixels (pixbuf);
    guchar *p = pixels + y * rowstride + x * 3;
    for(int i = 0; i < 3; i ++)
        p[i] = pixel.e[i];
}

struct Vec get_pixel (GdkPixbuf *pixbuf, int x, int y)
{
    guchar *pixels = gdk_pixbuf_get_pixels (pixbuf);
    guchar *p = pixels + y * rowstride + x * 3;
    return Vec(p[0], p[1], p[2]);
}

gboolean redraw(GtkWidget *self, cairo_t *cr, GdkPixbuf *buf)
{
    static GtkAllocation alloc;
    gtk_widget_get_allocation(self, &alloc);
    GdkPixbuf *buf2 = gdk_pixbuf_scale_simple(buf, alloc.width, alloc.height, GDK_INTERP_BILINEAR);
    gdk_cairo_set_source_pixbuf(cr, buf2, 0, 0);
    cairo_rectangle(cr, 0, 0, alloc.width, alloc.height);
    cairo_fill(cr);
    g_object_unref(buf2);
    return TRUE;
}

void press(GtkWidget *widget, GdkEventButton *event,  GdkPixbuf *buf)
{
    if(event->button == 1 && event->x < image_width && event->y < image_height)
    {
        char *str = VecPrn(get_pixel(buf, event->x, event->y));
        printf("%s\n", str);
        free(str);
    }
}

gboolean refresh (GtkWidget *widget)
{
    gtk_widget_queue_draw(widget);
    return G_SOURCE_CONTINUE;
}


gboolean render(GdkPixbuf *buf)
{
    for(int j = 0; j < image_height; j++)
        for(int i = 0; i < image_width; i++)
            {
                double x = (double)i / (image_width - 1);
                double y = (double)j / (image_height - 1);
                struct Ray r = Ray(origin,
                    // lower_left_corner + u*horizontal + v*vertical - origin
                    VecSub(VecAdd(VecAdd(lower_left_corner, VecIncr(horizontal, x)), VecIncr(vertical, y)), origin));
                put_pixel(buf, i, j, RayColor(r));
            }
    return G_SOURCE_REMOVE;
}

int main(int argc, char *argv[])
{
    origin = Vec(.0, .0, .0);
    horizontal = Vec(viewport_width, .0, .0);
    vertical = Vec(.0, -viewport_height, .0);
    // origin - horizontal/2 - vertical/2 - focal_length
    lower_left_corner = VecSub(VecSub(VecSub(origin, VecDecr(horizontal, 2.0)),
                        VecDecr(vertical, 2.0)), Vec(.0, .0, focal_length));

    printf("lower left corner %s\n", VecPrn(lower_left_corner));
    printf("horizontal %s\n", VecPrn(horizontal));
    printf("vertical %s\n", VecPrn(vertical));

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", gtk_main_quit, NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), image_width, image_height);

    GdkPixbuf *buf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, image_width, image_height);
    rowstride = gdk_pixbuf_get_rowstride (buf);
    gdk_pixbuf_fill(buf, 0x00000000);

    GtkWidget *event_box = gtk_event_box_new();

    GtkWidget *draw_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(draw_area, image_width, image_height);
    g_signal_connect(draw_area, "draw", G_CALLBACK(redraw), buf);
    g_signal_connect(event_box, "button-press-event", G_CALLBACK(press), buf);
    gtk_container_add(GTK_CONTAINER(event_box), draw_area);
    gtk_container_add(GTK_CONTAINER(window), event_box);

    gtk_widget_show_all(window);

    g_timeout_add(100, G_SOURCE_FUNC(refresh), draw_area);
    HittableAdd(Vec(.0, .0, -1.0), 0.5);
    HittableAdd(Vec(.0, -100.5, -1.0), 100.0);
    render(buf);

    gtk_main();

    return 1;
}