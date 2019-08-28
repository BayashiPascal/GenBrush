#include <gtk/gtk.h>
#include "pbmath.h"
#include "genbrush.h"

// Main structure of the application
typedef struct {
  char* gladeFilePath;
  GtkApplication* gtkApp;
  GApplication* gApp;
  GtkWidget* mainWindow;
  GenBrush* gbWidget;
  int green;
} DemoApp;

// Declare a global instance of the application
DemoApp app;

// Callback function for the 'activate' event on the GTK application
// 'data' is the DemoApp application
void GtkAppActivate(
  GtkApplication* gtkApp, 
  gpointer user_data);

// Callback function for the 'delete-event' event on the GTK application
// window
// 'user_data' is the DemoApp application
gboolean ApplicationWindowDeleteEvent(
  GtkWidget* widget,
  GdkEventConfigure* event, 
  gpointer user_data);

// Callback function for the 'delete-event' event on the GTK application
// window
// 'user_data' is the DemoApp application
gboolean ButtonClicked(
  gpointer user_data);

// Paint the GenBrush surface
void PaintSurface(
  GBSurface* const surf, 
       VecShort2D* dim, 
               int green);

// Create an instance of the application
DemoApp DemoAppCreate(
     int argc,
  char** argv) {
  
  // Unused arguments
  (void)argc;
  (void)argv;

  // Init the variable to paint the GenBrush widget
  app.green = 0;

  // Set the UI definition file path
  app.gladeFilePath = strdup("./test.glade");

  // Create a GTK application
  app.gtkApp = gtk_application_new(
    NULL, 
    G_APPLICATION_FLAGS_NONE);
  app.gApp = G_APPLICATION(app.gtkApp);

  // Connect the callback function on the 'activate' event of the GTK
  // application
  g_signal_connect(
    app.gtkApp, 
    "activate", 
    G_CALLBACK(GtkAppActivate), 
    NULL);

  // Return the instance of the application
  return app;
}

// Main function of the application
int DemoAppMain(
  DemoApp* that,
       int argc,
    char** argv) {
  
  // Run the application at the G level
  int status = g_application_run(
    that->gApp, 
    argc, 
    argv);

  // If the application failed
  if (status != 0) {
    GenBrushErr->_type = PBErrTypeUnitTestFailed;
    sprintf(
      GenBrushErr->_msg, 
      "g_application_run failed (%d)", 
      status);
    PBErrCatch(GenBrushErr);
  }

  // Unreference the GTK application
  g_object_unref(that->gtkApp);

  // Return the status code
  return status;
}

// Callback function for the 'activate' event on the GTK application
// 'user_data' is the DemoApp application
void GtkAppActivate(
  GtkApplication* gtkApp, 
  gpointer user_data) {

  // Unused arguments
  (void)gtkApp;
  (void)user_data;

  // Create a GTK builder with the UI definition file
  GtkBuilder* gtkBuilder = gtk_builder_new_from_file(app.gladeFilePath);

  // Set the GTK application in the GTK builder
  gtk_builder_set_application(gtkBuilder, app.gtkApp);

  // Get the widget container for the GenBrush widget
  GtkWidget* container = GTK_WIDGET(
    gtk_builder_get_object(
      gtkBuilder, 
      "box1"));

  // Declare a variable to memorize the size of the GenBrush widget
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 300); 
  VecSet(&dim, 1, 200); 

  // Create the GenBrush widget
  app.gbWidget = GBCreateWidget(&dim);

  // Get the GTK widget from the GenBrush widget
  GtkWidget* widget = GBGetGtkWidget(app.gbWidget);

  // Insert the GenBrush widget into its container
  gtk_box_pack_start(
    GTK_BOX(container), 
    widget, 
    TRUE, 
    TRUE, 
    0);

  // Get the UI's button
  GtkWidget* btn = GTK_WIDGET(
    gtk_builder_get_object(
      gtkBuilder, 
      "button1"));

  // Set the callback on the 'clicked' event of the UI's button
  g_signal_connect(
    btn, 
    "clicked", 
    G_CALLBACK(ButtonClicked), 
    NULL);

  // Get the main window
  app.mainWindow = GTK_WIDGET(
    gtk_builder_get_object(
      gtkBuilder, 
      "applicationwindow1"));

  // Set the callback on the delete-event of the main window
  g_signal_connect(
    app.mainWindow, 
    "delete-event",
    G_CALLBACK(ApplicationWindowDeleteEvent), 
    NULL);

  // Connect the other signals defined in the UI definition file
  gtk_builder_connect_signals(
    gtkBuilder, 
    NULL);

  // Free memory used by the GTK builder
  g_object_unref(G_OBJECT(gtkBuilder));

  // Display the main window and all its components
  gtk_widget_show_all(app.mainWindow);

  // Run the application at the GTK level
  gtk_main();
}

// Callback function for the 'delete-event' event on the GTK application
// window
// 'user_data' is the DemoApp application
gboolean ApplicationWindowDeleteEvent(
  GtkWidget* widget,
  GdkEventConfigure* event, 
  gpointer user_data) {

  // Unused arguments
  (void)widget;
  (void)event;
  (void)user_data;

  // Free memory used by the GenBrush instance
  GBFree(&(app.gbWidget));

  // Quit the application at GTK level
  gtk_main_quit();

  // Quit the application at G level
  g_application_quit(app.gApp);

  // Return false to continue the callback chain
  return FALSE;
}

// Callback function for the 'delete-event' event on the GTK application
// window
// 'user_data' is the DemoApp application
gboolean ButtonClicked(
  gpointer user_data) {

  // Unused arguments
  (void)user_data;

  // Paint the GenBrush surface
  PaintSurface(
    app.gbWidget->_surf, 
    GBSurfaceDim(app.gbWidget->_surf), 
    app.green);
  
  // Refresh the displayed surface
  GBRender(app.gbWidget);

  // Update the value used to pain the surface
  app.green += 10;
  
  // If the value reached its maximum
  if (app.green >= 255) {

    // Stop the GTK window
    gtk_window_close(GTK_WINDOW(app.mainWindow));
  }

  // Return true to stop the callback chain
  return TRUE;
}

// Paint the GenBrush surface
void PaintSurface(
  GBSurface* const surf, 
       VecShort2D* dim, 
               int green) {

  // Loop on the pixels of the surface
  VecShort2D pos = VecShortCreateStatic2D();
  do {

    // Set the pixel value according to the argument
    GBPixel pixel;
    pixel._rgba[GBPixelAlpha] = 255;
    pixel._rgba[GBPixelRed] = MIN(255, VecGet(&pos, 0));
    pixel._rgba[GBPixelGreen] = MIN(255, green);
    pixel._rgba[GBPixelBlue] = MIN(255, VecGet(&pos, 1));
    GBSurfaceSetFinalPixel(surf, &pos, &pixel);
  } while (VecStep(&pos, dim));
}

// Main function
int main(
     int argc,
  char** argv)
{
  // Initialise the GTK library
  gtk_init(&argc, &argv);

  // Create the application
  app = DemoAppCreate(argc, argv);
  
  // Run the application
  int status = DemoAppMain(&app, argc, argv);
  
  // Return the status at the end of the application
  return status;

}
