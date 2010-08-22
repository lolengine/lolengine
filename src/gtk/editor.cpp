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
    gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(builder, "window")));
    g_object_unref(G_OBJECT(builder));

    new DebugFps();
    glmapview->LoadMap("maps/testmap.tmx");
    glmapview->SetFocus();

    gtk_main();

    delete glmapview;

    return EXIT_SUCCESS;
}

