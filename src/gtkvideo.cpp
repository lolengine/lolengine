
#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif
#if defined __APPLE__ && defined __MACH__
#   include <OpenGL/gl.h>
#else
#   define GL_GLEXT_PROTOTYPES
#   include <GL/gl.h>
#endif

#include <stdlib.h>

#include "gtkvideo.h"

/*
 * Gtk Video implementation class
 */

class GtkVideoData
{
    friend class GtkVideo;

private:
    static gint init(GtkWidget *widget)
    {
        /* OpenGL functions can be called only if make_current returns true */
        if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
        {
            glViewport(0, 0, widget->allocation.width,
                             widget->allocation.height);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0,100, 100,0, -1,1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }
        return TRUE;
    }

    static gint draw(GtkWidget *widget, GdkEventExpose *event)
    {
        if (event->count == 0 && gtk_gl_area_make_current(GTK_GL_AREA(widget)))
        {
            /* Draw simple triangle */
            glClearColor(0,0,0,1);
            glClear(GL_COLOR_BUFFER_BIT);
            glColor3f(1,1,1);
            glBegin(GL_TRIANGLES);
            glVertex2f(10,10);
            glVertex2f(10,90);
            glVertex2f(90,90);
            glEnd();

            /* Swap backbuffer to front */
            gtk_gl_area_swapbuffers(GTK_GL_AREA(widget));
        }

        return TRUE;
    }

    static gint reshape(GtkWidget *widget, GdkEventConfigure *event)
    {
        if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
        {
            glViewport(0,0, widget->allocation.width,
                            widget->allocation.height);
        }
        return TRUE;
    }

    GtkWidget *widget;
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
    gtk_widget_set_events(GTK_WIDGET(data->widget),
                          GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);

    gtk_signal_connect(GTK_OBJECT(data->widget), "expose_event",
                       GTK_SIGNAL_FUNC(GtkVideoData::draw), NULL);
    gtk_signal_connect(GTK_OBJECT(data->widget), "configure_event",
                       GTK_SIGNAL_FUNC(GtkVideoData::reshape), NULL);
    gtk_signal_connect(GTK_OBJECT(data->widget), "realize",
                       GTK_SIGNAL_FUNC(GtkVideoData::init), NULL);

    // FIXME: is this needed?
    gtk_widget_set_usize(GTK_WIDGET(data->widget), 100, 100);

    /* Initialise OpenGL */
    glViewport(0, 0, data->widget->allocation.width,
                     data->widget->allocation.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, data->widget->allocation.width,
               data->widget->allocation.height, 0, -1, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void GtkVideo::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void GtkVideo::Refresh(float milliseconds)
{
#if 0
    if (milliseconds > 0.0f)
        while (SDL_GetTicks() < data->ticks + (milliseconds - 0.5f))
            SDL_Delay(1);
    data->ticks = SDL_GetTicks();
    data->frames++;

    SDL_GL_SwapBuffers();
#endif
}

void GtkVideo::FullScreen()
{
    // FIXME
}

GtkVideo::~GtkVideo()
{
    // FIXME
}

