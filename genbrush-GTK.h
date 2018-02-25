// ============ GENBRUSH-GTK.H ================

#include <gtk/gtk.h>
#include <cairo.h>

// ================= Define ==================


// ================= Data structure ===================

typedef struct GBSurfaceApp {
  GBSurface _surf;
  // Title of the window
  char* _title;
  // Application
  GtkApplication *_app;
  // Window
  GtkWidget *_window;
  // Widget
  GtkWidget *_drawingArea;
  // cairo surface
  cairo_surface_t *_cairoSurf;
  // Idle function
  gint (*_idleFun)(gpointer);
  // Idle function time out
  int _idleMs;
  // Returned status when the application is killed
  int _returnStatus;
} GBSurfaceApp;

typedef struct GBSurfaceWidget {
  GBSurface _surf;
  // Widget
  GtkWidget *_drawingArea;
  // cairo surface
  cairo_surface_t *_cairoSurf;
} GBSurfaceWidget;

// ================ Functions declaration ====================

// Create a new GBSurfaceApp with title 'title' and 
// dimensions 'dim'
GBSurfaceApp* GBSurfaceAppCreate(VecShort2D* dim, char* title);

// Free the GBSurfaceApp 'that'
void GBSurfaceAppFree(GBSurfaceApp** that);

// Render the GBSurfaceApp 'that'
// This function block the execution until the app is killed
// Return true if the status of the app when closed was 0, false else
bool GBSurfaceAppRender(GBSurfaceApp* that);

// Close the GBSurfaceApp 'that'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceAppClose(GBSurfaceApp* that);

// Refresh the content of the GBSurfaceApp 'that'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceAppRefresh(GBSurfaceApp* that);

// Set the idle function of the GBSurfaceApp 'that' to 'idleFun' 
// with a timeout of 'idleMs'
// The interface of the 'idleFun' is 
// gint tick(gpointer data)
// the argument of 'idleFun' is a pointer to GBSurfaceApp
void GBSurfaceSetIdle(GBSurfaceApp* that, 
  gint (*idleFun)(gpointer), int idleMs);

// Create a new GBSurfaceWidget with dimensions 'dim'
GBSurfaceWidget* GBSurfaceWidgetCreate(VecShort2D* dim);

// Free the GBSurfaceWidget 'that'
void GBSurfaceWidgetFree(GBSurfaceWidget** that);

// Return the GtkWidget of the GBSurfaceWidget 'that'
#if BUILDMODE != 0
inline
#endif 
GtkWidget* GBSurfaceWidgetGetGtkWidget(GBSurfaceWidget* that);

// Return the GtkWidget of the GBSurfaceApp 'that'
#if BUILDMODE != 0
inline
#endif 
GtkWidget* GBSurfaceAppGetGtkWidget(GBSurfaceApp* that);

// Render the GBSurfaceWidget 'that'
void GBSurfaceWidgetRender(GBSurfaceWidget* that);

// Create a GenBrush with a blank GBSurfaceApp
GenBrush* GBCreateApp(VecShort2D* dim, char* title);

// Set the idle function of the GBSurfaceApp of the GenBrush 'that' 
// to 'idleFun' with a timeout of 'idleMs'
// The interface of the 'idleFun' is 
// gint tick(gpointer data)
// the argument of 'idleFun' is a pointer to GBSurfaceApp
// If the surface of the app is not a GBSurfaceTypeApp, do nothing
void GBSetIdle(GenBrush* that, gint (*idleFun)(gpointer), int idleMs);

// Create a GenBrush with a blank GBSurfaceWidget
GenBrush* GBCreateWidget(VecShort2D* dim);

// Return the GtkWidget of the GenBrush 'that', or NULL if the surface
// of the GenBursh is not a GBSurfaceWidget 
#if BUILDMODE != 0
inline
#endif 
GtkWidget* GBGetGtkWidget(GenBrush* that);
