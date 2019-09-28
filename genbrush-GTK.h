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
  GtkApplication* _app;
  // Window
  GtkWidget* _window;
  // Widget
  GtkWidget* _drawingArea;
  // cairo surface
  cairo_surface_t* _cairoSurf;
  // Idle function
  gint (*_idleFun)(gpointer);
  // Idle function time out
  int _idleMs;
  // Returned status when the application is killed
  int _returnStatus;

  // cf GBSurfaceAppCallbackDraw
  // Flipped data
  unsigned char* _flippedData;
  
} GBSurfaceApp;

typedef struct GBSurfaceWidget {
  GBSurface _surf;
  // Widget
  GtkWidget* _drawingArea;
  // cairo surface
  cairo_surface_t* _cairoSurf;

  // cf GBSurfaceWidgetCallbackDraw
  // Flipped data
  unsigned char* _flippedData;
  
} GBSurfaceWidget;

// ================ Functions declaration ====================

// Create a new GBSurfaceApp with title 'title' and 
// dimensions 'dim'
GBSurfaceApp* GBSurfaceAppCreate(const VecShort2D* const dim, 
  const char* const title);

// Free the GBSurfaceApp 'that'
void GBSurfaceAppFree(GBSurfaceApp** that);

// Render the GBSurfaceApp 'that'
// This function block the execution until the app is killed
// Return true if the status of the app when closed was 0, false else
bool GBSurfaceAppRender(GBSurfaceApp* const that);

// Close the GBSurfaceApp 'that'
#if BUILDMODE != 0
static inline
#endif 
void GBSurfaceAppClose(const GBSurfaceApp* const that);

// Refresh the content of the GBSurfaceApp 'that'
#if BUILDMODE != 0
static inline
#endif 
void GBSurfaceAppRefresh(const GBSurfaceApp* const that);

// Set the idle function of the GBSurfaceApp 'that' to 'idleFun' 
// with a timeout of 'idleMs'
// The interface of the 'idleFun' is 
// gint tick(gpointer data)
// the argument of 'idleFun' is a pointer to GBSurfaceApp
void GBSurfaceSetIdle(GBSurfaceApp* const that, 
  gint (*idleFun)(gpointer), int idleMs);

// Create a new GBSurfaceWidget with dimensions 'dim'
GBSurfaceWidget* GBSurfaceWidgetCreate(const VecShort2D* const dim);

// Free the GBSurfaceWidget 'that'
void GBSurfaceWidgetFree(GBSurfaceWidget** that);

// Return the GtkWidget of the GBSurfaceWidget 'that'
#if BUILDMODE != 0
static inline
#endif 
GtkWidget* GBSurfaceWidgetGtkWidget(const GBSurfaceWidget* const that);

// Return the GtkWidget of the GBSurfaceApp 'that'
#if BUILDMODE != 0
static inline
#endif 
GtkWidget* GBSurfaceAppGtkWidget(const GBSurfaceApp* const that);

// Render the GBSurfaceWidget 'that'
void GBSurfaceWidgetRender(const GBSurfaceWidget* const that);

// Create a GenBrush with a blank GBSurfaceApp
GenBrush* GBCreateApp(const VecShort2D* const dim, 
  const char* const title);

// Set the idle function of the GBSurfaceApp of the GenBrush 'that' 
// to 'idleFun' with a timeout of 'idleMs'
// The interface of the 'idleFun' is 
// gint tick(gpointer data)
// the argument of 'idleFun' is a pointer to GBSurfaceApp
// If the surface of the app is not a GBSurfaceTypeApp, do nothing
void GBSetIdle(GenBrush* that, gint (*idleFun)(gpointer), 
  const int idleMs);

// Create a GenBrush with a blank GBSurfaceWidget
GenBrush* GBCreateWidget(const VecShort2D* const dim);

// Return the GtkWidget of the GenBrush 'that', or NULL if the surface
// of the GenBursh is not a GBSurfaceWidget 
#if BUILDMODE != 0
static inline
#endif 
GtkWidget* GBGetGtkWidget(const GenBrush* const that);

// Take a snapshot of the GBSurfaceApp 'that' and save it to 'filename'
// Return true if successfull, flase else
bool GBSurfaceAppScreenshot(
  const GBSurfaceApp* const that,
             const char* const filename);

// Take a snapshot of the GBSurfaceWidget 'that' and save it to 'filename'
// Return true if successfull, flase else
bool GBSurfaceWidgetScreenshot(
  const GBSurfaceWidget* const that,
             const char* const filename);
