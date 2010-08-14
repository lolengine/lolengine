//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>

#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#include "gtkvideo.h"
#include "video.h"

/*
 * Gtk Video implementation class
 */

class GtkVideoData
{
    friend class GtkVideo;

private:
    static gint init(GtkWidget *widget)
    {
        GtkVideoData *data = (GtkVideoData *)
            gtk_object_get_data(GTK_OBJECT(widget), "data");

        /* OpenGL functions can be called only if make_current returns true */
        if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
        {
            Video::Setup(widget->allocation.width,
                         widget->allocation.height);
        }
        return TRUE;
    }

    static gint draw(GtkWidget *widget, GdkEventExpose *event)
    {
        GtkVideoData *data = (GtkVideoData *)
            gtk_object_get_data(GTK_OBJECT(widget), "data");

        if (event->count == 0 && data->drawing == 2
             && gtk_gl_area_make_current(GTK_GL_AREA(widget)))
        {
            data->drawing = 0;

            /* Swap backbuffer to front */
            gtk_gl_area_swapbuffers(GTK_GL_AREA(widget));
        }

        return TRUE;
    }

    static gint reshape(GtkWidget *widget, GdkEventConfigure *event)
    {
        GtkVideoData *data = (GtkVideoData *)
            gtk_object_get_data(GTK_OBJECT(widget), "data");

        if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
        {
            Video::Setup(widget->allocation.width,
                         widget->allocation.height);
        }
        return TRUE;
    }

    GtkWidget *widget;
    int drawing;
};

/*
 * Public GtkVideo class
 */

GtkVideo::GtkVideo(char const *title, int width, int height)
{
    data = new GtkVideoData();

    int attrlist[] =
    {
        GDK_GL_RGBA,
        GDK_GL_RED_SIZE, 1,
        GDK_GL_GREEN_SIZE, 1,
        GDK_GL_BLUE_SIZE, 1,
        GDK_GL_DOUBLEBUFFER,
        GDK_GL_NONE
    };

    if (gdk_gl_query() == FALSE)
    {
        // FIXME: implement a panic() mode
        g_print("OpenGL not supported\n");
        exit(1);
    }

    data->widget = gtk_gl_area_new(attrlist);
    data->drawing = 0;

    gtk_object_set_data(GTK_OBJECT(data->widget), "data", data);
    gtk_widget_set_events(GTK_WIDGET(data->widget),
                          GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);

    gtk_signal_connect(GTK_OBJECT(data->widget), "expose_event",
                       GTK_SIGNAL_FUNC(GtkVideoData::draw), NULL);
    gtk_signal_connect(GTK_OBJECT(data->widget), "configure_event",
                       GTK_SIGNAL_FUNC(GtkVideoData::reshape), NULL);
    gtk_signal_connect(GTK_OBJECT(data->widget), "realize",
                       GTK_SIGNAL_FUNC(GtkVideoData::init), NULL);

    // FIXME: is this needed?
    gtk_widget_set_usize(GTK_WIDGET(data->widget), 400, 300);
}

void *GtkVideo::GetWidget()
{
    return data->widget;
}

int GtkVideo::GetWidth() const
{
    return data->widget->allocation.width;
}

int GtkVideo::GetHeight() const
{
    return data->widget->allocation.height;
}

void GtkVideo::PreRender()
{
    /// XXX: is this right?
    gtk_gl_area_make_current(GTK_GL_AREA(data->widget));

    Video::Clear();

    data->drawing = 1;
}

void GtkVideo::PostRender(float milliseconds)
{
    data->drawing = 2;

    gtk_widget_draw(GTK_WIDGET(data->widget), NULL);
#if 0
    if (milliseconds > 0.0f)
        while (SDL_GetTicks() < data->ticks + (milliseconds - 0.5f))
            SDL_Delay(1);
    data->ticks = SDL_GetTicks();
    data->frames++;

    SDL_GL_SwapBuffers();
#endif
}

