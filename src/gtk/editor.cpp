//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#include "core.h"
#include "debugfps.h"

static volatile int quit = 0;

static int ticking = 0;
static float const FPS = 30.0f;

static gint main_quit(GtkWidget *widget, GdkEventExpose *event)
{
    (void)widget;
    (void)event;

    quit = 1;
    gtk_main_quit();
    return FALSE;
}

static gboolean tick(void *widget)
{
    // FIXME: do not do anything if the previous tick was too recent?

    // FIXME: only quit if all entities have been cleaned
    if (quit)
        return FALSE;

    ticking = 1;

    /* Tick the game */
    Ticker::TickGame();

    gtk_widget_draw(GTK_WIDGET(widget), NULL);

    return TRUE;
}

static gint init(GtkWidget *widget)
{
    if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
        Video::Setup(widget->allocation.width, widget->allocation.height);

    return TRUE;
}

static gint reshape(GtkWidget *widget, GdkEventConfigure *event)
{
    (void)event;

    return init(widget);
}

static gint draw(GtkWidget *widget, GdkEventExpose *event)
{
    if (event->count > 0)
        return TRUE;

    /* OpenGL functions can be called only if make_current returns true */
    if (ticking && gtk_gl_area_make_current(GTK_GL_AREA(widget)))
    {
        ticking = 0;

        /* Clear the screen, tick the renderer, show the frame and
         * clamp to desired framerate */
        Video::Clear();
        Ticker::TickDraw();
        gtk_gl_area_swapbuffers(GTK_GL_AREA(widget));
        while (g_main_context_iteration(NULL, FALSE))
            ;
        Ticker::ClampFps(1000.0f / FPS);
    }

    return TRUE;
}

int main(int argc, char **argv)
{
    /* Initialize GTK */
    g_thread_init(NULL);
    gtk_init(&argc, &argv);

    if (gdk_gl_query() == FALSE)
    {
        g_print("OpenGL not supported\n");
        return EXIT_FAILURE;
    }

    /* Build the rest of the application */
    GtkBuilder *builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "src/gtk/editor.xml", NULL))
    {
        g_print("Cannot build from XML\n");
        return EXIT_FAILURE;
    }

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    GtkWidget *sw = GTK_WIDGET(gtk_builder_get_object(builder, "sw1"));
    g_object_unref(G_OBJECT(builder));

    /* Create new OpenGL widget */
    int attrlist[] =
    {
        GDK_GL_RGBA,
        GDK_GL_RED_SIZE, 1,
        GDK_GL_GREEN_SIZE, 1,
        GDK_GL_BLUE_SIZE, 1,
        GDK_GL_DOUBLEBUFFER,
        GDK_GL_NONE
    };

    GtkWidget *glarea = gtk_gl_area_new(attrlist);
    gtk_widget_set_events(glarea, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sw), glarea);

    /* Connect signals and show window */
    gtk_signal_connect(GTK_OBJECT(window), "delete_event",
                       GTK_SIGNAL_FUNC(main_quit), NULL);
    gtk_signal_connect(GTK_OBJECT(glarea), "expose_event",
                       GTK_SIGNAL_FUNC(draw), NULL);
    gtk_signal_connect(GTK_OBJECT(glarea), "configure_event",
                       GTK_SIGNAL_FUNC(reshape), NULL);
    gtk_signal_connect(GTK_OBJECT(glarea), "realize",
                       GTK_SIGNAL_FUNC(init), NULL);

    gtk_widget_show_all(window);

    // FIXME: detect when the map viewer is killed
    new MapViewer("maps/testmap.tmx");
    new DebugFps();

    /* We tick from the idle function instead of a timeout to avoid
     * stealing time from the GTK loop when the callback time exceeds
     * the timeout value. */
    gtk_idle_add(tick, glarea);
    gtk_main();

    return EXIT_SUCCESS;
}

