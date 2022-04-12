#include <gtk/gtk.h>
#include <math.h>

#include "vec.h"
#include "ray.h"
#include "camera.h"

// Draw callback for drawing area widget
gboolean redraw(GtkWidget *self, cairo_t *cr, struct Camera *cam)
{
    static GtkAllocation alloc;
    gtk_widget_get_allocation(self, &alloc);
    GdkPixbuf *buf2 = gdk_pixbuf_scale_simple(cam->buf, alloc.width, alloc.height, GDK_INTERP_NEAREST);
    gdk_cairo_set_source_pixbuf(cr, buf2, 0, 0);
    cairo_rectangle(cr, 0, 0, alloc.width, alloc.height);
    cairo_fill(cr);
    g_object_unref(buf2);
    return TRUE;
}

// Mouse click callback for event box widget
void press(GtkWidget *widget, GdkEventButton *event, struct Camera *cam)
{
    GtkAllocation alloc;
    gtk_widget_get_allocation(widget, &alloc);
    double x = event->x * cam->image_width / (double)alloc.width;
    double y = event->y * cam->image_height / (double)alloc.height;
    if(event->button == 1)
    {
        char *str = VecPrn(get_pixel(cam, x, y));
        printf("%s\n", str);
        free(str);
    }
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Create a camera
    struct Camera *cam = Camera(16.0/9.0, 480.0);

    // Create a window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // Register gtk_main_quit on window destroy
    g_signal_connect(window, "destroy", gtk_main_quit, NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), cam->image_width, cam->image_height);
    // Create an event box to process mouse clicks
    GtkWidget *event_box = gtk_event_box_new();
    // Create a drawing area to output an image to
    GtkWidget *draw_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(draw_area, cam->image_width, cam->image_height);
    // Connect draw signal
    g_signal_connect(draw_area, "draw", G_CALLBACK(redraw), cam);
    // Connect mouse click signal
    g_signal_connect(event_box, "button-press-event", G_CALLBACK(press), cam);
    // Add widgets to the window
    gtk_container_add(GTK_CONTAINER(event_box), draw_area);
    gtk_container_add(GTK_CONTAINER(window), event_box);
    gtk_widget_show_all(window);

    cam->draw_area = draw_area;

    // Add object to world
    HittableAdd(Vec(.0, .0, -1.0), 0.5);
    HittableAdd(Vec(.0, -100.5, -1.0), 100.0);
    // Render an image once
    render(cam);

    // Enter GTK main loop
    gtk_main();

    return 1;
}