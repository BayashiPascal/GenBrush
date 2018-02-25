// ============ GENBRUSH-GTK.C ================

// ================ Functions declaration ====================

// Callback for the 'activate' event on a GBSurfaceApp
void GBSurfaceAppCallbackActivate(GtkApplication* app, 
  gpointer data);

// Callback for the 'draw' event on a GBSurfaceApp
gboolean GBSurfaceAppCallbackDraw(GtkWidget *widget, cairo_t *cr,
  gpointer data);

// Callback for the 'config' event on a GBSurfaceApp
gboolean GBSurfaceAppCallbackConfigEvt (GtkWidget *widget,
  GdkEventConfigure *event, gpointer data);
  
// Callback for the 'draw' event on a GBSurfaceWidget
gboolean GBSurfaceWidgetCallbackDraw(GtkWidget *widget, cairo_t *cr,
  gpointer data);

// Callback for the 'config' event on a GBSurfaceWidget
gboolean GBSurfaceWidgetCallbackConfigEvt(GtkWidget *widget,
  GdkEventConfigure *event, gpointer data);
  
// ================ Functions implementation ====================

// Callback for the 'draw' event on a GBSurfaceApp
gboolean GBSurfaceAppCallbackDraw(GtkWidget *widget, cairo_t *cr,
  gpointer data) {
  (void)widget;
  // Declare a variable to convert the data into the GBSurfaceApp
  GBSurfaceApp* GBApp = (GBSurfaceApp*)data;
  // Paint the final pixels of the GBSurface on the Cairo surface
  cairo_set_source_surface(cr, GBApp->_cairoSurf, 0, 0);
  cairo_paint(cr);
  return FALSE;
}

// Callback for the 'config' event on a GBSurfaceApp
gboolean GBSurfaceAppCallbackConfigEvt(GtkWidget *widget,
  GdkEventConfigure *event, gpointer data) {
  (void)event;
  // Declare a variable to convert the data into the GBSurfaceApp
  GBSurfaceApp* GBApp = (GBSurfaceApp*)data;
  // Attach the GBPixel to the cairo surface
  GBApp->_cairoSurf = cairo_image_surface_create_for_data(
    (unsigned char*)GBSurfaceFinalPixels(&(GBApp->_surf)),
    CAIRO_FORMAT_ARGB32,
    gtk_widget_get_allocated_width(widget),
    gtk_widget_get_allocated_height(widget),
    cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, 
      gtk_widget_get_allocated_width(widget)));
  // Draw the surface
  cairo_t *cr = cairo_create(GBApp->_cairoSurf);
  GBSurfaceAppCallbackDraw(widget, cr, GBApp);
  cairo_destroy (cr);
  return TRUE;
}

// Callback for the activate event on a GBSurfaceApp
void GBSurfaceAppCallbackActivate(GtkApplication* app, 
  gpointer data) {
  // Declare a variable to convert the data into the GBSurfaceApp
  GBSurfaceApp* GBApp = (GBSurfaceApp*)data;
  // Create the window
  GBApp->_window = gtk_application_window_new(app);
  // Set the title of the window
  gtk_window_set_title(GTK_WINDOW(GBApp->_window), GBApp->_title);
  // Set the size of the window
  gtk_window_set_default_size(GTK_WINDOW(GBApp->_window), 
    VecGet(GBSurfaceDim(&(GBApp->_surf)), 0), 
    VecGet(GBSurfaceDim(&(GBApp->_surf)), 1));
  // Set the window as non resizable
  gtk_window_set_resizable(GTK_WINDOW(GBApp->_window), false);
  // Create the drawing area
  GBApp->_drawingArea = gtk_drawing_area_new();
  // Set the size of the drawing area
  gtk_widget_set_size_request(GBApp->_drawingArea, 
    VecGet(GBSurfaceDim(&(GBApp->_surf)), 0), 
    VecGet(GBSurfaceDim(&(GBApp->_surf)), 1));
  // Insert the drawing area into the window
  gtk_container_add(GTK_CONTAINER(GBApp->_window), GBApp->_drawingArea);
  // Connect the events
  g_signal_connect(GBApp->_drawingArea, "draw",
    G_CALLBACK (GBSurfaceAppCallbackDraw), GBApp);
  g_signal_connect(GBApp->_drawingArea,"configure-event",
    G_CALLBACK (GBSurfaceAppCallbackConfigEvt), GBApp);
  if (GBApp->_idleFun != NULL)
    g_timeout_add(GBApp->_idleMs, GBApp->_idleFun, GBApp);
  // Display the window  
  gtk_widget_show_all(GBApp->_window);
}

// Create a new GBSurfaceApp with title 'title' and 
// dimensions 'dim'
// The surface is initialized with one layer of white pixels
// (rgba = {255})
GBSurfaceApp* GBSurfaceAppCreate(VecShort2D* dim, char* title) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'dim' is null");
    PBErrCatch(GenBrushErr);
  }
  if (title == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'title' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Allocate memory
  GBSurfaceApp* that = PBErrMalloc(GenBrushErr, sizeof(GBSurfaceApp));
  // Set the title of the application
  that->_title = PBErrMalloc(GenBrushErr, 
    sizeof(char) * (strlen(title) + 1));
  strcpy(that->_title, title);
  // Create the surface
  that->_surf = GBSurfaceCreateStatic(GBSurfaceTypeApp, dim);
  // Create the application
  that->_app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
  // Set the idle properties
  that->_idleFun = NULL;
  that->_idleMs = 0;
  // Set the return status
  that->_returnStatus = 0;
  // Connect the 'activate' event
  // Other properties will be set up by the activate event
  g_signal_connect (that->_app, "activate", 
    G_CALLBACK (GBSurfaceAppCallbackActivate), that);
  // Return the GBSurfaceApp
  return that;
}

// Set the idle function of the GBSurfaceApp 'that' to 'idleFun' 
// with a timeout of 'idleMs'
// The interface of the 'idleFun' is 
// gint tick(gpointer data)
// the argument of 'idleFun' is a pointer to GBSurfaceApp
void GBSurfaceSetIdle(GBSurfaceApp* that, 
  gint (*idleFun)(gpointer), int idleMs) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (idleFun == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'idleFun' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Memorize the idle properties
  that->_idleFun = idleFun;
  that->_idleMs = idleMs;
}

// Render the GBSurfaceApp 'that'
// This function block the execution until the app is killed
// Return true if the status of the app when closed was 0, false else
bool GBSurfaceAppRender(GBSurfaceApp* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Run the application
  int argc = 0;
  char* *argv = NULL;
  that->_returnStatus = 
    g_application_run(G_APPLICATION(that->_app), argc, argv);
  // After the application has been killed
  // Return the success code
  return (that->_returnStatus == 0 ? true : false);
}

// Free the GBSurfaceApp 'that'
void GBSurfaceAppFree(GBSurfaceApp** that) {
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  g_object_unref((*that)->_app);
  free((*that)->_title);
}

// Free the GBSurfaceWidget 'that'
void GBSurfaceWidgetFree(GBSurfaceWidget** that) {
  (void)that;
}

// Create a GenBrush with a blank GBSurfaceApp
GenBrush* GBCreateApp(VecShort2D* dim, char* title) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'dim' is null");
    PBErrCatch(GenBrushErr);
  }
  if (title == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'title' is null");
    PBErrCatch(GenBrushErr);
  }
#endif  
  // Allocate memory
  GenBrush* that = PBErrMalloc(GenBrushErr, sizeof(GenBrush));
  // Set properties
  that->_surf = (GBSurface*)GBSurfaceAppCreate(dim, title);
  that->_pods = GSetCreateStatic();
  // Return the new GenBrush
  return that;
}

// Set the idle function of the GBSurfaceApp of the GenBrush 'that' 
// to 'idleFun' with a timeout of 'idleMs'
// The interface of the 'idleFun' is 
// gint tick(gpointer data)
// the argument of 'idleFun' is a pointer to GBSurfaceApp
// If the surface of the app is not a GBSurfaceTypeApp, do nothing
void GBSetIdle(GenBrush* that, gint (*idleFun)(gpointer), int idleMs) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (idleFun == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'idleFun' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBSurfaceGetType(that->_surf) == GBSurfaceTypeApp)
    GBSurfaceSetIdle((GBSurfaceApp*)(that->_surf), idleFun, idleMs);
}

// Create a GenBrush with a blank GBSurfaceWidget and an eye
// of type 'eyeType'
// If 'eyeType' is GBEyeTypeOrtho the view is front
GenBrush* GBCreateWidget(VecShort2D* dim) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'dim' is null");
    PBErrCatch(GenBrushErr);
  }
#endif  
  // Allocate memory
  GenBrush* that = PBErrMalloc(GenBrushErr, sizeof(GenBrush));
  // Set properties
  that->_surf = (GBSurface*)GBSurfaceWidgetCreate(dim);
  that->_pods = GSetCreateStatic();
  // Return the new GenBrush
  return that;
}

// Create a new GBSurfaceWidget with dimensions 'dim'
GBSurfaceWidget* GBSurfaceWidgetCreate(VecShort2D* dim) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'dim' is null");
    PBErrCatch(GenBrushErr);
  }
#endif  
  // Allocate memory
  GBSurfaceWidget* that = PBErrMalloc(GenBrushErr, 
    sizeof(GBSurfaceWidget));
  // Create the surface
  that->_surf = GBSurfaceCreateStatic(GBSurfaceTypeWidget, dim);
  // Create the drawing area
  that->_drawingArea = gtk_drawing_area_new();
  // Set the size of the drawing area
  gtk_widget_set_size_request(that->_drawingArea, 
    VecGet(dim, 0), VecGet(dim, 1));
  // Connect to the draw event
  g_signal_connect(that->_drawingArea, "draw",
    G_CALLBACK (GBSurfaceWidgetCallbackDraw), that);
  // Connect to the configure event
  g_signal_connect(that->_drawingArea,"configure-event",
    G_CALLBACK (GBSurfaceWidgetCallbackConfigEvt), that);
  // Return the GBSurfaceWidget
  return that;
}

// Callback for the 'draw' event on a GBSurfaceWidget
gboolean GBSurfaceWidgetCallbackDraw(GtkWidget *widget, cairo_t *cr,
  gpointer data) {
  (void)widget;
  // Declare a variable to convert the data into the GBSurfaceApp
  GBSurfaceWidget* GBWidget = (GBSurfaceWidget*)data;
  // Paint the final pixels of the GBSurface on the Cairo surface
  cairo_set_source_surface(cr, GBWidget->_cairoSurf, 0, 0);
  cairo_paint (cr);
  return FALSE;
}

// Render the GBSurfaceWidget 'that'
void GBSurfaceWidgetRender(GBSurfaceWidget* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Request the update of the widget area
  gtk_widget_queue_draw_area(that->_drawingArea, 0.0, 0.0, 
    gtk_widget_get_allocated_width(that->_drawingArea), 
    gtk_widget_get_allocated_height(that->_drawingArea));
}

// Callback for the 'config' event on a GBSurfaceWidget
gboolean GBSurfaceWidgetCallbackConfigEvt(GtkWidget *widget,
  GdkEventConfigure *event, gpointer data) {
  (void)event;
  // Declare a variable to convert the data into the GBSurfaceApp
  GBSurfaceWidget* GBWidget = (GBSurfaceWidget*)data;
  // Attach the GBPixel to the cairo surface
  GBWidget->_cairoSurf = cairo_image_surface_create_for_data(
    (unsigned char* )GBSurfaceFinalPixels(&(GBWidget->_surf)),
    CAIRO_FORMAT_ARGB32,
    gtk_widget_get_allocated_width(widget),
    gtk_widget_get_allocated_height(widget),
    cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, 
      gtk_widget_get_allocated_width(widget)));
  // Draw the surface
  cairo_t *cr = cairo_create(GBWidget->_cairoSurf);
  GBSurfaceWidgetCallbackDraw(widget, cr, GBWidget);
  cairo_destroy (cr);
  return TRUE;
}
