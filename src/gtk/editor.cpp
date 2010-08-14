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

#include "ticker.h"
#include "debugfps.h"
#include "video.h"
#include "game.h"

volatile int quit = 0;

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
    float const delta_time = 33.33333f;

    // FIXME: do not do anything if the previous tick was too recent?

    // FIXME: only quit if all assets have been cleaned
    if (quit)
        return FALSE;

    /* Tick the game */
    Ticker::TickGame(delta_time);

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
    if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
    {
        // FIXME: do not do anything if the game tick wasn't called?
        float const delta_time = 33.33333f;

        /* Clear the screen, tick the renderer, and show the frame */
        Video::Clear();
        Ticker::TickRender(delta_time);
        gtk_gl_area_swapbuffers(GTK_GL_AREA(widget));
    }

    return TRUE;
}

int main(int argc, char **argv)
{
    /* Initialize GTK */
    gtk_init(&argc, &argv);

    if (gdk_gl_query() == FALSE)
    {
        g_print("OpenGL not supported\n");
        return EXIT_FAILURE;
    }

    /* Create new top level window. */
    GtkWidget *window = gtk_window_new( GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple");
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);

    /* Quit form main if got delete event */
    gtk_signal_connect(GTK_OBJECT(window), "delete_event",
                       GTK_SIGNAL_FUNC(main_quit), NULL);

    /* You should always delete gtk_gl_area widgets before exit or else
       GLX contexts are left undeleted, this may cause problems (=core dump)
       in some systems.
       Destroy method of objects is not automatically called on exit.
       You need to manually enable this feature. Do gtk_quit_add_destroy()
       for all your top level windows unless you are certain that they get
       destroy signal by other means.
    */
    gtk_quit_add_destroy(1, GTK_OBJECT(window));

    /* Create new OpenGL widget. */
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

    gtk_widget_set_events(GTK_WIDGET(glarea),
                          GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);

    gtk_signal_connect(GTK_OBJECT(glarea), "expose_event",
                       GTK_SIGNAL_FUNC(draw), NULL);
    gtk_signal_connect(GTK_OBJECT(glarea), "configure_event",
                       GTK_SIGNAL_FUNC(reshape), NULL);
    gtk_signal_connect(GTK_OBJECT(glarea), "realize",
                       GTK_SIGNAL_FUNC(init), NULL);

    // FIXME: is this needed?
    gtk_widget_set_usize(GTK_WIDGET(glarea), 400, 300);

    /* Put glarea into window and show it all */
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(glarea));
    gtk_widget_show(GTK_WIDGET(glarea));
    gtk_widget_show(GTK_WIDGET(window));

    // FIXME: detect when the game is killed
    new Game("maps/testmap.tmx");
    new DebugFps();

    //gtk_idle_add(tick, glarea);
    gtk_timeout_add(33, tick, glarea);
    gtk_main();

    return EXIT_SUCCESS;
}
