//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>
#include <cmath>

#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#include "gtkvideo.h"
#include "ticker.h"
#include "game.h"

volatile int quit = 0;

static gint main_quit(GtkWidget *widget, GdkEventExpose *event)
{
    quit = 1;
    return FALSE;
}

int main(int argc, char **argv)
{
    GtkWidget *window, *glarea;

    /* initialize gtk */
    gtk_init(&argc, &argv);

    /* Create new top level window. */
    window = gtk_window_new( GTK_WINDOW_TOPLEVEL);
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
    GtkVideo *video = new GtkVideo("LOL", 640, 480);
    glarea = GTK_WIDGET(video->GetWidget());

    /* put glarea into window and show it all */
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(glarea));
    gtk_widget_show(GTK_WIDGET(glarea));
    gtk_widget_show(GTK_WIDGET(window));

    Game *game = new Game("maps/testmap.tmx");

    for (;;)
    {
        // Do GTK stuff until the user wants to quit
        while (g_main_context_iteration(NULL, FALSE));

        if (quit)
            break;

        game->SetMouse(0, 0);
        Ticker::TickGame(33.33333f);

        video->PreRender();
        Ticker::TickRender(33.33333f);
        video->PostRender(33.33333f);
    }

    delete video;

    return 0;
}
