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

#include "core.h"
#include "glmapview.h"
#include "debugfps.h"

static gboolean delayed_quit(GtkWidget *w, GdkEvent *e, void *data)
{
    (void)w;
    (void)e;
    (void)data;
    gtk_main_quit();
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

    /* Build the application interface */
    GtkBuilder *builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "src/gtk/editor.xml", NULL))
    {
        g_print("Cannot build from XML\n");
        return EXIT_FAILURE;
    }
    gtk_builder_connect_signals(builder, NULL);

    /* Add our custom GL map viewer */
    GlMapView *glmapview = new GlMapView(builder);

    /* Show window. We're good to go! */
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_widget_show_all(window);
    gtk_signal_connect(GTK_OBJECT(window), "delete_event",
                       GTK_SIGNAL_FUNC(delayed_quit), NULL);
    g_object_unref(G_OBJECT(builder));

    glmapview->LoadMap("maps/testmap.tmx");
    new DebugFps();

    gtk_main();

    delete glmapview;

    return EXIT_SUCCESS;
}

