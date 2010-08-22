//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>
#include <gdk/gdkkeysyms.h>

#include "core.h"
#include "glmapview.h"

static float const FPS = 30.0f;

GlMapView::GlMapView(GtkBuilder *builder)
  : hadj(GTK_ADJUSTMENT(gtk_builder_get_object(builder, "gl_hadj"))),
    vadj(GTK_ADJUSTMENT(gtk_builder_get_object(builder, "gl_vadj"))),
    ticking(FALSE), panning(FALSE),
    mapviewer(0),
    xpan(0.0), ypan(0.0)
{
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

    glarea = gtk_gl_area_new(attrlist);
    gtk_widget_set_usize(glarea, 400, 300);
    gtk_widget_set_events(glarea, GDK_EXPOSURE_MASK |
                                  GDK_POINTER_MOTION_MASK |
                                  GDK_BUTTON_PRESS_MASK |
                                  GDK_BUTTON_RELEASE_MASK);
    gtk_widget_set_can_focus(glarea, TRUE);

    GtkContainer *cont = GTK_CONTAINER(gtk_builder_get_object(builder,
                                                              "gl_container"));
    gtk_container_add(cont, glarea);

    /* We tick from the idle function instead of a timeout to avoid
     * stealing time from the GTK loop when the callback time exceeds
     * the timeout value. */
    g_idle_add((GSourceFunc)IdleTickSignal, this);

    gtk_signal_connect(GTK_OBJECT(glarea), "realize",
                       GTK_SIGNAL_FUNC(SetupSignal), this);
    gtk_signal_connect(GTK_OBJECT(glarea), "destroy",
                       GTK_SIGNAL_FUNC(DestroySignal), this);
    gtk_signal_connect(GTK_OBJECT(glarea), "expose_event",
                       GTK_SIGNAL_FUNC(DrawSignal), this);
    gtk_signal_connect(GTK_OBJECT(glarea), "configure_event",
                       GTK_SIGNAL_FUNC(ReshapeSignal), this);

    gtk_signal_connect(GTK_OBJECT(glarea), "button_press_event",
                       GTK_SIGNAL_FUNC(MouseButtonSignal), this);
    gtk_signal_connect(GTK_OBJECT(glarea), "button_release_event",
                       GTK_SIGNAL_FUNC(MouseButtonSignal), this);
    gtk_signal_connect(GTK_OBJECT(glarea), "motion_notify_event",
                       GTK_SIGNAL_FUNC(MouseMotionSignal), this);

    gtk_signal_connect(GTK_OBJECT(glarea), "key_press_event",
                       GTK_SIGNAL_FUNC(KeyPressSignal), this);
}

void GlMapView::LoadMap(char const *path)
{
    // FIXME: detect when the map viewer is killed
    mapviewer = new MapViewer(path);

    UpdateAdjustments();
}

void GlMapView::SetFocus()
{
    gtk_widget_grab_focus(glarea);
}

gboolean GlMapView::IdleTick()
{
    // FIXME: do not do anything if the previous tick was too recent?
    ticking = TRUE;

    if (mapviewer)
        mapviewer->SetPOV(gtk_adjustment_get_value(hadj),
                          gtk_adjustment_get_value(vadj));

    /* Tick the game */
    Ticker::TickGame();

    gtk_widget_draw(GTK_WIDGET(glarea), NULL);

    return TRUE;
}

gboolean GlMapView::Setup()
{
    /* Set up display */
    if (gtk_gl_area_make_current(GTK_GL_AREA(glarea)))
        Video::Setup(glarea->allocation.width, glarea->allocation.height);

    UpdateAdjustments();

    return TRUE;
}

gboolean GlMapView::Destroy()
{
    g_idle_remove_by_data(this);
    return TRUE;
}

gboolean GlMapView::Draw(GdkEventExpose *event)
{
    if (event->count > 0)
        return TRUE;

    /* OpenGL functions can be called only if make_current returns true */
    if (ticking && gtk_gl_area_make_current(GTK_GL_AREA(glarea)))
    {
        ticking = FALSE;

        /* Clear the screen, tick the renderer, show the frame and
         * clamp to desired framerate */
        Video::Clear();
        Ticker::TickDraw();
        gtk_gl_area_swapbuffers(GTK_GL_AREA(glarea));
        while (g_main_context_iteration(NULL, FALSE))
            ;
        Ticker::ClampFps(1000.0f / FPS);
    }

    return TRUE;
}

gboolean GlMapView::UpdateAdjustments()
{
    float w = mapviewer ? mapviewer->GetWidth() : glarea->allocation.width;
    float h = mapviewer ? mapviewer->GetHeight() : glarea->allocation.height;

    /* Manage adjustments */
    struct { GtkAdjustment *adj; float map_size, sw_size; } s[2] =
    {
        { hadj, w, glarea->allocation.width },
        { vadj, h, glarea->allocation.height },
    };

    for (int i = 0; i < 2; i++)
    {
        gtk_adjustment_set_lower(s[i].adj, 0);
        gtk_adjustment_set_upper(s[i].adj, s[i].map_size);
        gtk_adjustment_set_step_increment(s[i].adj, 1);
        gtk_adjustment_set_page_increment(s[i].adj, s[i].sw_size);
        gtk_adjustment_set_page_size(s[i].adj, s[i].sw_size);

        float val = gtk_adjustment_get_value(s[i].adj);
        if (val + s[i].sw_size > s[i].map_size)
        {
            gtk_adjustment_set_value(s[i].adj,
                                     s[i].map_size - s[i].sw_size);
            gtk_adjustment_value_changed(s[i].adj);
        }
    }

    return TRUE;
}

gboolean GlMapView::MoveAdjustments(double dx, double dy)
{
    if (dx)
        gtk_adjustment_set_value(hadj, gtk_adjustment_get_value(hadj) + dx);

    if (dy)
        gtk_adjustment_set_value(vadj, gtk_adjustment_get_value(vadj) + dy);

    UpdateAdjustments();

    return TRUE;
}

gboolean GlMapView::MouseButton(GdkEventButton *event)
{
    if (event->type == GDK_BUTTON_PRESS && event->button == 2)
    {
        panning = TRUE;
        xpan = event->x;
        ypan = event->y;
        GdkCursor *cursor = gdk_cursor_new(GDK_HAND1);
        gdk_window_set_cursor(glarea->window, cursor);
        gdk_cursor_unref(cursor);
        return FALSE;
    }
    else if (event->type == GDK_BUTTON_RELEASE && event->button == 2)
    {
        panning = FALSE;
        gdk_window_set_cursor(glarea->window, NULL);
        return FALSE;
    }

    return TRUE;
}

gboolean GlMapView::MouseMotion(GdkEventMotion *event)
{
    if (panning)
    {
        if (event->x != xpan)
        {
            double val = gtk_adjustment_get_value(hadj);
            int map_width = mapviewer->GetWidth();
            val += xpan - event->x;
            xpan = event->x;
            if (val + glarea->allocation.width > map_width)
                val = map_width - glarea->allocation.width;
            gtk_adjustment_set_value(hadj, val);
            gtk_adjustment_value_changed(hadj);
        }

        if (event->y != ypan)
        {
            double val = gtk_adjustment_get_value(vadj);
            int map_height = mapviewer->GetHeight();
            val += ypan - event->y;
            ypan = event->y;
            if (val + glarea->allocation.height > map_height)
                val = map_height - glarea->allocation.height;
            gtk_adjustment_set_value(vadj, val);
            gtk_adjustment_value_changed(vadj);
        }
    }

    return TRUE;
}

gboolean GlMapView::KeyPress(GdkEventKey *event)
{
    switch (event->keyval)
    {
    case GDK_Up:    MoveAdjustments(  0.0, -10.0); break;
    case GDK_Down:  MoveAdjustments(  0.0,  10.0); break;
    case GDK_Left:  MoveAdjustments(-10.0,  0.0); break;
    case GDK_Right: MoveAdjustments( 10.0,  0.0); break;
    default: return FALSE;
    }

    return TRUE;
}

/* Private signal slots */
gboolean GlMapView::IdleTickSignal(GlMapView *that)
{
    return that->IdleTick();
}

gboolean GlMapView::SetupSignal(GtkWidget *w, GlMapView *that)
{
    (void)w;
    return that->Setup();
}

gboolean GlMapView::DestroySignal(GtkWidget *w, GlMapView *that)
{
    (void)w;
    return that->Destroy();
}

gboolean GlMapView::DrawSignal(GtkWidget *w, GdkEventExpose *event,
                               GlMapView *that)
{
    (void)w;
    return that->Draw(event);
}

gboolean GlMapView::ReshapeSignal(GtkWidget *w, GdkEventConfigure *event,
                                  GlMapView *that)
{
    (void)w;
    (void)event;
    return that->Setup();
}

gboolean GlMapView::MouseButtonSignal(GtkWidget *w, GdkEventButton *event,
                                      GlMapView *that)
{
    (void)w;
    return that->MouseButton(event);
}

gboolean GlMapView::MouseMotionSignal(GtkWidget *w, GdkEventMotion *event,
                                      GlMapView *that)
{
    (void)w;
    return that->MouseMotion(event);
}

gboolean GlMapView::KeyPressSignal(GtkWidget *w, GdkEventKey *event,
                                   GlMapView *that)
{
    (void)w;
    return that->KeyPress(event);
}

