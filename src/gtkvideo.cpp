
#include <cstdlib>

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

#include "gtkvideo.h"

/*
 * Gtk Video implementation class
 */

class GtkVideoData
{
    friend class GtkVideo;

private:
    void SetupView()
    {
        glViewport(0, 0, widget->allocation.width,
                         widget->allocation.height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, widget->allocation.width,
                   widget->allocation.height, 0, -1, 10);
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

    static gint init(GtkWidget *widget)
    {
        GtkVideoData *data = (GtkVideoData *)
            gtk_object_get_data(GTK_OBJECT(widget), "data");

        /* OpenGL functions can be called only if make_current returns true */
        if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
        {
            data->SetupView();
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
            data->SetupView();
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

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

void GtkVideo::FullScreen()
{
    // FIXME
}

GtkVideo::~GtkVideo()
{
    // FIXME
}

