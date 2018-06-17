// ============ GENBRUSH-INLINE-GTK.C ================

// ================ Functions implementation ====================

// Close the GBSurfaceApp 'that'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceAppClose(const GBSurfaceApp* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Request the killing of the drawing area
  gtk_window_close(GTK_WINDOW(that->_window));
}

// Refresh the content of the GBSurfaceApp 'that'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceAppRefresh(const GBSurfaceApp* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  gtk_widget_queue_draw_area(that->_drawingArea, 0.0, 0.0, 
    gtk_widget_get_allocated_width(that->_drawingArea), 
    gtk_widget_get_allocated_height(that->_drawingArea));
}

// Return the GtkWidget of the GBSurfaceWidget 'that'
#if BUILDMODE != 0
inline
#endif 
GtkWidget* GBSurfaceWidgetGtkWidget(const GBSurfaceWidget* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_drawingArea;
}

// Return the GtkWidget of the GBSurfaceApp 'that'
#if BUILDMODE != 0
inline
#endif 
GtkWidget* GBSurfaceAppGtkWidget(const GBSurfaceApp* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_drawingArea;
}

// Return the GtkWidget of the GenBrush 'that', or NULL if the surface
// of the GenBursh is not a GBSurfaceWidget 
#if BUILDMODE != 0
inline
#endif 
GtkWidget* GBGetGtkWidget(const GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBSurfaceGetType(that->_surf) == GBSurfaceTypeWidget)
    return GBSurfaceWidgetGtkWidget((GBSurfaceWidget*)(that->_surf));
  else
    return NULL;
}
