//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if !defined __DH_GLMAPVIEW_H__
#define __DH_GLMAPVIEW_H__

#include <gtk/gtk.h>

class GlMapView
{
public:
    GlMapView(GtkBuilder *builder);
    void LoadMap(char const *path);
    void SetFocus();

private:
    /* Private methods */
    gboolean IdleTick();
    gboolean Setup();
    gboolean Destroy();
    gboolean Draw(GdkEventExpose *e);
    gboolean UpdateAdjustments();
    gboolean MoveAdjustments(double dx, double dy);
    gboolean MouseButton(GdkEventButton *e);
    gboolean MouseMotion(GdkEventMotion *e);
    gboolean KeyPress(GdkEventKey *e);

    /* Private signal slots */
    static gboolean IdleTickSignal(GlMapView *that);
    static gboolean SetupSignal(GtkWidget *w, GlMapView *that);
    static gboolean DestroySignal(GtkWidget *w, GlMapView *that);
    static gboolean DrawSignal(GtkWidget *w, GdkEventExpose *e,
                               GlMapView *that);
    static gboolean ReshapeSignal(GtkWidget *w, GdkEventConfigure *e,
                                  GlMapView *that);
    static gboolean MouseButtonSignal(GtkWidget *w, GdkEventButton *e,
                                      GlMapView *that);
    static gboolean MouseMotionSignal(GtkWidget *w, GdkEventMotion *e,
                                      GlMapView *that);
    static gboolean KeyPressSignal(GtkWidget *w, GdkEventKey *e,
                                   GlMapView *that);

private:
    GtkAdjustment *hadj, *vadj;
    GtkWidget *glarea;
    gboolean ticking, panning;

    MapViewer *mapviewer;
    double xpan, ypan;
};

#endif // __DH_GLMAPVIEW_H__

