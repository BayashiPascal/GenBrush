// ============ GENBRUSH-INLINE.C ================

// ================ Functions implementation ====================

// ---------------- GBLayer --------------------------

// Get the area of the layer (width * height)
#if BUILDMODE != 0
inline
#endif 
int GBLayerArea(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (int)VecGet(&(that->_dim), 0) * 
    (int)VecGet(&(that->_dim), 1);
}

// Get the position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBLayerPos(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_pos);
}

// Get a copy of the position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBLayerGetPos(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_pos;
}

// Set the position of the GBLayer 'that' to 'pos'
// If the flag _modified==false _prevPos is first set to _pos
// and _modified is set to true
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetPos(GBLayer* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (!(that->_modified)) {
    that->_modified = true;
    that->_prevPos = that->_pos;
  }
  that->_pos = *pos;
}

// Get the previous position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBLayerPrevPos(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_prevPos);
}

// Get a copy of the previous position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBLayerGetPrevPos(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_prevPos;
}

// Get the dimensions of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBLayerDim(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_dim);  
}

// Get a copy of the dimensions of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBLayerGetDim(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_dim;
}

// Get a copy of the blend mode of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
GBLayerBlendMode GBLayerGetBlendMode(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_blendMode;
}

// Set the blend mode of the GBLayer 'that' to 'blend'
// Set the flag _modified to true
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetBlendMode(GBLayer* that, GBLayerBlendMode blend) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  that->_modified = true;
  that->_blendMode = blend;
}

// Get a copy of the modified flag of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
bool GBLayerIsModified(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_modified;
}

// Set the modified flag of the GBLayer 'that' to 'flag'
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetModified(GBLayer* that, bool flag) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  that->_modified = flag;
}

// Get a copy of the stack position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
GBLayerStackPosition GBLayerGetStackPos(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_stackPos;
}

// Set the stack position of the GBLayer 'that' to 'pos'
// Set the flag _modified to true
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetStackPos(GBLayer* that, GBLayerStackPosition pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  that->_modified = true;
  that->_stackPos = pos;
}

// Get the stacked pixels of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayerPixels(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_pix;
}

// Get the stacked pixels of the GBLayer 'that' at position 'pos'
// 'pos' must be inside the layer
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayerPixel(GBLayer* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (!GBLayerIsPosInside(that, pos)) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is out of the layer (%d,%d)",
      VecGet(pos, 0), VecGet(pos, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  int iPix = GBPosIndex(pos, GBLayerDim(that));
  return that->_pix + iPix;
}

// Get the stacked pixels of the GBLayer 'that' at position 'pos'
// If 'pos' is out of the layer return NULL
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayerPixelSafe(GBLayer* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (!GBLayerIsPosInside(that, pos))
    return NULL;
  else
    return GBLayerPixel(that, pos);
}

// Add the pixel 'pix' with depth 'depth' on top of the stack at 
// position 'pos' of GBLayer 'that'
// Set the flag _modified to true
// 'pos' must be inside the layer
// If the pixel is completely transparent (_rgba[GBPixelAlpha]==0) 
// do nothing
#if BUILDMODE != 0
inline
#endif 
void GBLayerAddPixel(GBLayer* that, VecShort2D* pos, GBPixel* pix,
  float depth) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pix == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pix' is null");
    PBErrCatch(GenBrushErr);
  }
  if (!GBLayerIsPosInside(that, pos)) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is out of the layer (%d,%d)",
      VecGet(pos, 0), VecGet(pos, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  // If the pixel is transparent 
  if (pix->_rgba[GBPixelAlpha] == 0)
    // Do nothing
    return;
  int iPix = GBPosIndex(pos, GBLayerDim(that));
  GBStackedPixel* clone = PBErrMalloc(GenBrushErr, 
    sizeof(GBStackedPixel));
  clone->_val = *pix;
  clone->_depth = depth;
  clone->_blendMode = GBLayerGetBlendMode(that);
  GSetAppend(that->_pix + iPix, clone);
}

// Add the pixel 'pix' with depth 'depth' on top of the stack at 
// position 'pos' of GBLayer 'that'
// Set the flag _modified to true
// If 'pos' is out of the layer do nothing
// If the pixel is completely transparent (_rgba[GBPixelAlpha]==0) 
// do nothing
#if BUILDMODE != 0
inline
#endif 
void GBLayerAddPixelSafe(GBLayer* that, VecShort2D* pos, GBPixel* pix,
  float depth) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pix == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pix' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBLayerIsPosInside(that, pos))
    GBLayerAddPixel(that, pos, pix, depth);
}

// Return true if the position 'pos' is inside the layer 'that' 
// boundary, false else
#if BUILDMODE != 0
inline
#endif 
bool GBLayerIsPosInside(GBLayer* that, VecShort2D *pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (VecGet(pos, 0) >= 0 &&
    VecGet(pos, 1) >= 0 &&
    VecGet(pos, 0) < VecGet(GBLayerDim(that), 0) &&
    VecGet(pos, 1) < VecGet(GBLayerDim(that), 1))
    return true;
  else
    return false;
}

// Delete all the stacked pixels in the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
void GBLayerFlush(GBLayer* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  for (int iPix = GBLayerArea(that); iPix--;) {
    GSet *stack = that->_pix + iPix;
    while (GSetNbElem(stack) > 0) {
      GBStackedPixel *pix = GSetPop(stack);
      free(pix);
    }
  }
}

// ---------------- GBSurface --------------------------

// Get the type of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBSurfaceType GBSurfaceGetType(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_type;
}

// Get a copy of the dimensions of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBSurfaceGetDim(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_dim;
}

// Get the dimensions of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBSurfaceDim(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_dim);
}

// Get the final pixels of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceFinalPixels(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_finalPix;
}

// Get the final pixel at position 'pos' of the GBSurface 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceFinalPixel(GBSurface* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (!GBSurfaceIsPosInside(that, pos)) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is out of the surface (%d,%d)",
      VecGet(pos, 0),VecGet(pos, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  int iPix = GBPosIndex(pos, GBSurfaceDim(that));
  return that->_finalPix + iPix;
}

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel GBSurfaceGetFinalPixel(GBSurface* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (!GBSurfaceIsPosInside(that, pos)) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is out of the surface (%d,%d)",
      VecGet(pos, 0),VecGet(pos, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  int iPix = GBPosIndex(pos, GBSurfaceDim(that));
  return that->_finalPix[iPix];
}

// Set the final pixel at position 'pos' of the GBSurface 'that' to
// the pixel 'pix'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetFinalPixel(GBSurface* that, VecShort2D* pos, 
  GBPixel* pix) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (!GBSurfaceIsPosInside(that, pos)) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is out of the surface (%d,%d)",
      VecGet(pos, 0),VecGet(pos, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  int iPix = GBPosIndex(pos, GBSurfaceDim(that));
  that->_finalPix[iPix] = *pix;
}

// Get the final pixel at position 'pos' of the GBSurface 'that'
// If 'pos' is out of the surface return NULL
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceFinalPixelSafe(GBSurface* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBSurfaceIsPosInside(that, pos))
    return GBSurfaceFinalPixel(that, pos);
  else
    return NULL;
}

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// 'that'
// If 'pos' is out of the surface return a transparent pixel
#if BUILDMODE != 0
inline
#endif 
GBPixel GBSurfaceGetFinalPixelSafe(GBSurface* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBSurfaceIsPosInside(that, pos))
    return GBSurfaceGetFinalPixel(that, pos);
  else
    return GBColorTransparent;
}

// Set the final pixel at position 'pos' of the GBSurface 'that' to
// the pixel 'pix'
// If 'pos' is out of the surface do nothing
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetFinalPixelSafe(GBSurface* that, VecShort2D* pos, 
  GBPixel* pix) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pix == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pix' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBSurfaceIsPosInside(that, pos))
    GBSurfaceSetFinalPixel(that, pos, pix);
}

// Get the area of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
int GBSurfaceArea(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return VecGet(&(that->_dim), 0) * VecGet(&(that->_dim), 1);
}

// Get the background color of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceBgColor(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_bgColor);
}

// Get a copy of the background color of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel GBSurfaceGetBgColor(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_bgColor;
}

// Set the background color of the GBSurface 'that' to 'col'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetBgColor(GBSurface* that, GBPixel* col) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (col == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'col' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  that->_bgColor = *col;
}

// Return true if the position 'pos' is inside the GBSurface 'that' 
// boundary, false else
#if BUILDMODE != 0
inline
#endif 
bool GBSurfaceIsPosInside(GBSurface* that, VecShort2D *pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (VecGet(pos, 0) >= 0 &&
    VecGet(pos, 1) >= 0 &&
    VecGet(pos, 0) < VecGet(GBSurfaceDim(that), 0) &&
    VecGet(pos, 1) < VecGet(GBSurfaceDim(that), 1))
    return true;
  else
    return false;
}

// Get the seet of layers of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBSurfaceLayers(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_layers);
}

// Add a new GBLayer with dimensions 'dim' to the top of the stack 
// of layers of the GBSurface 'that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBSurfaceAddLayer(GBSurface* that, VecShort2D* dim) {
#if BUILDMODE == 0
  if (dim == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'dim' is null");
    PBErrCatch(GenBrushErr);
  }
  if (VecGet(dim, 0) <= 0 || VecGet(dim, 1) <= 0) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'dim' is invalid (%d>0,%d>0)",
      VecGet(dim, 0), VecGet(dim, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  // Create the layer
  GBLayer* layer = GBLayerCreate(dim);
  // Add the new layer to the set of layers
  GSetAppend(&(that->_layers), layer);
  // Return the new layer
  return layer;
}

// 'fileName' to the top of the stack of layers of the GBSurface 
// 'that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBSurfaceAddLayerFromFile(GBSurface* that, char* fileName) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (fileName == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'fileName' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Create the layer from the file
  GBLayer* layer = GBLayerCreateFromFile(fileName);
  // If we could create the layer
  if (layer != NULL)
    // Add the layer to the surface
    GSetAppend(&(that->_layers), layer);
  // Return the new layer
  return layer;
}

// Get the 'iLayer'-th layer of the GBSurface 'that'
// 'iLayer' must be valid
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBSurfaceGetLayer(GBSurface* that, int iLayer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (iLayer < 0 || iLayer >= GSetNbElem(&(that->_layers))) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'iLayer' is invalid (0<=%d<%d)",
      iLayer, GSetNbElem(&(that->_layers)));
    PBErrCatch(GenBrushErr);
  }
#endif
  return (GBLayer*)GSetGet(&(that->_layers), iLayer);
}

// Get the number of layer of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
int GBSurfaceNbLayer(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GSetNbElem(&(that->_layers));
}

// Set the _modified flag of all layers of the GBSurface 'that' 
// to 'flag'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetLayersModified(GBSurface* that, bool flag) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If there are layers
  if (GBSurfaceNbLayer(that) > 0) {
    // Declare an iterator on the set of layers
    GSetIterForward iter = 
      GSetIterForwardCreateStatic(GBSurfaceLayers(that));
    do {
      GBLayer* layer = GSetIterGet(&iter);
      GBLayerSetModified(layer, flag);
    } while (GSetIterStep(&iter));
  }
}

// Set the position of the GBLayer 'layer' into the set of layers of 
// the GBSurface 'that' to 'pos'
// If 'layer' can't be found in the surface do nothing
// 'pos' must be valid (0<='pos'<nbLayers)
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetLayerPos(GBSurface* that, GBLayer* layer, int pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (layer == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'layer' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos < 0 || pos >= GSetNbElem(&(that->_layers))) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is invalid (0<=%d<%d)",
      pos, GSetNbElem(&(that->_layers)));
    PBErrCatch(GenBrushErr);
  }
#endif
  // Get the current index of the layer to move
  int curPos = GSetGetIndexFirst(&(that->_layers), layer);
  // If we could find the layer
  if (curPos != -1) {
    // Move the layer
    GSetMoveElem(&(that->_layers), curPos, pos);
    // Set the _modified flag
    GBLayerSetModified(layer, true);
  }
}

// Remove the GBLayer 'layer' from the set of layers of the 
// GBSurface 'that'
// The memory used by 'layer' is freed
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceRemoveLayer(GBSurface* that, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (layer == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'layer' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Remove the layer from the set
  GSetRemoveAll(&(that->_layers), layer);
  // Free the memory
  GBLayerFree(&layer);
}

// ---------------- GBSurfaceImage --------------------------

// Get the filename of the GBSurfaceImage 'that'
#if BUILDMODE != 0
inline
#endif 
char* GBSurfaceImageFileName(GBSurfaceImage* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_fileName;
}

// Set the filename of the GBSurfaceImage 'that' to 'fileName'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceImageSetFileName(GBSurfaceImage* that, char* fileName) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If the filename is already allocated, free memory
  if (that->_fileName != NULL) {
    free(that->_fileName);
  }
  // If the new filename is not null
  if (fileName != NULL) {
    // Allocate memory and copy the new filename
    that->_fileName = PBErrMalloc(GenBrushErr, 
      sizeof(char) * (strlen(fileName) + 1));
    strcpy(that->_fileName, fileName);
  }
}

// ---------------- GBEye --------------------------

// Return the type of the GBEye 'that'
#if BUILDMODE != 0
inline
#endif 
GBEyeType _GBEyeGetType(GBEye* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_type;
}

// Get the scale of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat3D* _GBEyeScale(GBEye* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_scale);
}
 
// Get a copy of the scale of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat3D _GBEyeGetScale(GBEye* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_scale;
}

 
// Get the translation of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat2D* _GBEyeOrig(GBEye* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_orig);
}

 
// Get a copy of the translation of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat2D _GBEyeGetOrig(GBEye* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_orig;
}

 
// Get the rotation of the GBEye (in radians)
#if BUILDMODE != 0
inline
#endif 
float _GBEyeGetRot(GBEye* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_theta;
}

// Set the scale of the GBEye
#if BUILDMODE != 0
inline
#endif 
void GBEyeSetScaleVec(GBEye* that, VecFloat3D* scale) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (scale == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'scale' is null");
    PBErrCatch(GenBrushErr);
  }
  if (ISEQUALF(VecGet(scale, 0), 0.0) == true ||
    ISEQUALF(VecGet(scale, 1), 0.0) == true) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'scale' is invalid (%f!=0 && %f!=0)",
      VecGet(scale, 0), VecGet(scale, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  VecCopy(&(that->_scale), scale);
  // Update the projection matrix
  GBEyeUpdateProj(that);
}
#if BUILDMODE != 0
inline
#endif 
void GBEyeSetScaleFloat(GBEye* that, float scale) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (ISEQUALF(scale, 0.0) == true) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'scale' is invalid (%f!=0)", scale);
    PBErrCatch(GenBrushErr);
  }
#endif
  for (int i = 3; i--;)
    VecSet(&(that->_scale), i, scale);
  // Update the projection matrix
  GBEyeUpdateProj(that);
}
 
// Set the translation of the GBEye
#if BUILDMODE != 0
inline
#endif 
void _GBEyeSetOrig(GBEye* that, VecFloat2D* trans) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (trans == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'trans' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  that->_orig = *trans;
  // Update the projection matrix
  GBEyeUpdateProj(that);
}

 
// Set the rotation of the GBEye (in radians)
#if BUILDMODE != 0
inline
#endif 
void _GBEyeSetRot(GBEye* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  that->_theta = theta;
  // Update the projection matrix
  GBEyeUpdateProj(that);
}

// Get the matrix projection of the eye
#if BUILDMODE != 0
inline
#endif 
MatFloat* _GBEyeProj(GBEye* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_proj;
}

// Call the appropriate GBEye<>Process according to the type of the
// GBEye 'that'
#if BUILDMODE != 0
inline
#endif 
void _GBEyeProcess(GBEye* that, GBObjPod* pod) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pod == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pod' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Call the appropriate function according to the type of eye
  switch (that->_type) {
    case GBEyeTypeOrtho:
      GBEyeOrthoProcess((GBEyeOrtho*)that, pod);
      break;
    case GBEyeTypeIsometric:
      GBEyeIsometricProcess((GBEyeIsometric*)that, pod);
      break;
    default:
      switch (GBObjPodGetType(pod)) {
        case GBObjTypePoint:
          GBObjPodSetEyePoint(pod, 
            VecClone(GBObjPodGetObjAsPoint(pod)));
          break;
        case GBObjTypeSCurve:
          GBObjPodSetEyeSCurve(pod, 
            SCurveClone(GBObjPodGetObjAsSCurve(pod)));
          break;
        case GBObjTypeShapoid:
          GBObjPodSetEyeShapoid(pod, 
            ShapoidClone(GBObjPodGetObjAsShapoid(pod)));
          break;
        default:
          break;
      }
      break;
  }  
}

// ---------------- GBEyeOrtho --------------------------

// Set the orientation the GBEyeOrtho
#if BUILDMODE != 0
inline
#endif 
void GBEyeOrthoSetView(GBEyeOrtho* that, GBEyeOrthoView view) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  that->_view = view;
  // Update the projection matrix
  GBEyeUpdateProj((GBEye*)that);
}
  
// ---------------- GBEyeIsometric --------------------------

// Set the angle around Y of the GBEyeOrtho to 'theta' (in radians)
#if BUILDMODE != 0
inline
#endif 
void GBEyeIsometricSetRotY(GBEyeIsometric* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  that->_thetaY = theta;
  GBEyeUpdateProj((GBEye*)that);
}

// Set the angle around Right of the GBEyeOrtho to 'theta' 
// (in radians, in [-pi/2, pi/2])
// If 'theta' is out of range it is automatically bounded 
// (ex: pi -> pi/2)
#if BUILDMODE != 0
inline
#endif 
void GBEyeIsometricSetRotRight(GBEyeIsometric* that, float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Ensure 'theta' is in bounds
  if (theta > PBMATH_HALFPI)
    theta = PBMATH_HALFPI;
  else if (theta < -PBMATH_HALFPI)
    theta = -PBMATH_HALFPI;
  that->_thetaRight = theta;
  GBEyeUpdateProj((GBEye*)that);
}

// Get the angle around Y of the GBEyeOrtho 'that'
#if BUILDMODE != 0
inline
#endif 
float GBEyeIsometricGetRotY(GBEyeIsometric* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_thetaY;
}

// Get the angle around Right of the GBEyeOrtho 'that'
#if BUILDMODE != 0
inline
#endif 
float GBEyeIsometricGetRotRight(GBEyeIsometric* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_thetaRight;
}

// ---------------- GBHand --------------------------

// Return the type of the GBHand 'that'
#if BUILDMODE != 0
inline
#endif 
GBHandType _GBHandGetType(GBHand* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_type;
}

// Call the appropriate GBHand<>Process according to the type of the
// GBHand 'that'
#if BUILDMODE != 0
inline
#endif 
void _GBHandProcess(GBHand* that, GBObjPod* pod) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If 'pod' has no viewed object do nothing
  if (GBObjPodGetEyeObj(pod) == NULL)
    return;
  // Call the appropriate function depending on the type of hand
  switch (that->_type) {
    case GBHandTypeDefault:
      GBHandDefaultProcess((GBHandDefault*)that, pod);
      break;
    default:
      break;
  }
}

// ---------------- GBTool --------------------------

// Return a copy of the type of the GBTool 'that'
#if BUILDMODE != 0
inline
#endif 
GBToolType GBToolGetType(GBTool* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_type;
}

// ---------------- GBInkSolid --------------------------

// Return the type of the GBInk 'that'
#if BUILDMODE != 0
inline
#endif 
GBInkType _GBInkGetType(GBInk* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_type;
}

// ---------------- GInkSolid --------------------------

// Get the color of the GBInkSolid 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel GBInkSolidGet(GBInkSolid* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_color;
}

// Set the color of the GBInkSolid 'that' to 'col'
#if BUILDMODE != 0
inline
#endif 
void GBInkSolidSet(GBInkSolid* that, GBPixel* col) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (col == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'col' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  that->_color = *col;
}


// ---------------- GBTool --------------------------

// Function to call the appropriate GBTool<>Draw function according to 
// type of GBTool 'that'
#if BUILDMODE != 0
inline
#endif 
void _GBToolDraw(GBTool* that, GBObjPod* pod) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pod == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pod' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  switch (GBToolGetType(that)) {
    case GBToolTypePlotter:
      GBToolPlotterDraw((GBToolPlotter*)that, pod);
      break;
    default:
      break;
  }
}

// ---------------- GBObjPod --------------------------

// Return the type of the object in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBObjType GBObjPodGetType(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_type;
}

// Return the object in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
void* GBObjPodGetObj(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_srcPoint;
}

// Return the object viewed by its attached eye in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
void* GBObjPodGetEyeObj(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_eyePoint;
}

// Return the object processed as Points by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBObjPodGetHandObjAsPoints(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_handPoints);  
}

// Return the object processed as Shapoids by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBObjPodGetHandObjAsShapoids(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_handShapoids);  
}


// Return the object processed as SCurves by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBObjPodGetHandObjAsSCurves(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return &(that->_handSCurves);  
}



// Return the eye in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBEye* GBObjPodGetEye(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_eye;
}

// Return the hand in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBHand* GBObjPodGetHand(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_hand;
}

// Return the tool in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBTool* GBObjPodGetTool(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_tool;
}

// Return the ink in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBInk* GBObjPodGetInk(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_ink;
}

// Return the layer in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBObjPodGetLayer(GBObjPod* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_layer;
}

// Set the Point viewed by its attached eye in the GBObjPod 'that' 
// to 'point'
// If 'point' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEyePoint(GBObjPod* that, VecFloat* point) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (point != NULL) {
    if (that->_eyePoint != NULL)
      VecFree(&(that->_eyePoint));
    that->_eyePoint = point;
  }
}

// Set the Shapoid viewed by its attached eye in the GBObjPod 'that' 
// to 'shap'
// If 'shap' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEyeShapoid(GBObjPod* that, Shapoid* shap) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (shap != NULL) {
    if (that->_eyeShapoid != NULL)
      ShapoidFree(&(that->_eyeShapoid));
    that->_eyeShapoid = shap;
  }
}

// Set the SCurve viewed by its attached eye in the GBObjPod 'that' 
// to 'curve'
// If 'curve' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEyeSCurve(GBObjPod* that, SCurve* curve) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (curve != NULL) {
    if (that->_eyeSCurve != NULL)
      SCurveFree(&(that->_eyeSCurve));
    that->_eyeSCurve = curve;
  }
}

// Set the eye in the GBObjPod 'that' to 'eye'
// If 'eye' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEye(GBObjPod* that, GBEye* eye) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (eye != NULL)
    that->_eye = eye;
}

// Set the hand in the GBObjPod 'that' to 'hand'
// If 'hand' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetHand(GBObjPod* that, GBHand* hand) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (hand != NULL)
    that->_hand = hand;
}

// Set the tool in the GBObjPod 'that' to 'tool'
// If 'tool' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetTool(GBObjPod* that, GBTool* tool) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (tool != NULL)
    that->_tool = tool;
}

// Set the ink in the GBObjPod 'that' to 'ink'
// If 'ink' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetInk(GBObjPod* that, GBInk* ink) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (ink != NULL)
    that->_ink = ink;
}

// Set the layer in the GBObjPod 'that' to 'layer'
// If 'layer' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void GBObjPodSetLayer(GBObjPod* that, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (layer != NULL)
    that->_layer = layer;
}

// ---------------- GenBrush --------------------------

// Get the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBSurface* GBSurf(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_surf;
}

// Get the dimensions of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBDim(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceDim(that->_surf);
}

// Get a copy of the dimensions of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBGetDim(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceGetDim(that->_surf);
}

// Get the final pixels of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBFinalPixels(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceFinalPixels(that->_surf);
}

// Get the final pixel at position 'pos' of the GBSurface of the GB 
// 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBFinalPixel(GenBrush* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (!GBIsPosInside(that, pos)) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is out of the surface (%d,%d)",
      VecGet(pos, 0), VecGet(pos, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceFinalPixel(GBSurf(that), pos);
}

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// of the GB 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel GBGetFinalPixel(GenBrush* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (!GBIsPosInside(that, pos)) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is out of the surface (%d,%d)",
      VecGet(pos, 0), VecGet(pos, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceGetFinalPixel(GBSurf(that), pos);
}

// Set the final pixel at position 'pos' of the GBSurface of the GB 
// 'that' to the pixel 'pix'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
void GBSetFinalPixel(GenBrush* that, VecShort2D* pos, GBPixel* pix) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pix == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pix' is null");
    PBErrCatch(GenBrushErr);
  }
  if (!GBIsPosInside(that, pos)) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is out of the surface (%d,%d)",
      VecGet(pos, 0), VecGet(pos, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  GBSurfaceSetFinalPixel(GBSurf(that), pos, pix);
}

// Get the final pixel at position 'pos' of the GBSurface of the GB 
// 'that'
// If 'pos' is out of the surface return NULL
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBFinalPixelSafe(GenBrush* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBIsPosInside(that, pos))
    return GBSurfaceFinalPixel(GBSurf(that), pos);
  else
    return NULL;
}

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// of the GB 'that'
// If 'pos' is out of the surface return a transparent pixel
#if BUILDMODE != 0
inline
#endif 
GBPixel GBGetFinalPixelSafe(GenBrush* that, VecShort2D* pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBIsPosInside(that, pos))
    return GBSurfaceGetFinalPixel(GBSurf(that), pos);
  else
    return GBColorTransparent;
}

// Set the final pixel at position 'pos' of the GBSurface of the GB 
// 'that' to the pixel 'pix'
// If 'pos' is out of the surface do nothing
#if BUILDMODE != 0
inline
#endif 
void GBSetFinalPixelSafe(GenBrush* that, VecShort2D* pos, GBPixel* pix) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pix == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pix' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBIsPosInside(that, pos))
    GBSurfaceSetFinalPixel(GBSurf(that), pos, pix);
}

// Get the type of the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBSurfaceType GBGetType(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceGetType(that->_surf);
}

// Get the background color of the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBBgColor(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceBgColor(GBSurf(that));
}

// Get a copy of the background color of the GBSurface of the 
// GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel GBGetBgColor(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceGetBgColor(GBSurf(that));
}

// Set the background color of the GBSurface of the GenBrush
//  'that' to 'col'
#if BUILDMODE != 0
inline
#endif 
void GBSetBgColor(GenBrush* that, GBPixel* col) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (col == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'col' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  GBSurfaceSetBgColor(GBSurf(that), col);
}

// Get the filename of the GBSurfaceImage of the GenBrush 'that'
// Return NULL if the surface is not a GBSurfaceImage
#if BUILDMODE != 0
inline
#endif 
char* GBFileName(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBGetType(that) == GBSurfaceTypeImage)
    return GBSurfaceImageFileName((GBSurfaceImage*)GBSurf(that));
  else
    return NULL;
}

// Set the filename of the GBSurfaceImage of the GenBrush 'that' 
// to 'fileName'
// Do nothing if the surface is not a GBSurfaceImage
#if BUILDMODE != 0
inline
#endif 
void GBSetFileName(GenBrush* that, char* fileName) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBGetType(that) == GBSurfaceTypeImage)
    GBSurfaceImageSetFileName((GBSurfaceImage*)GBSurf(that), fileName);
}

// Get the area of the GBSurface of the Genbrush 'that'
#if BUILDMODE != 0
inline
#endif 
int GBArea(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceArea(GBSurf(that));
}

// Return true if the position 'pos' is inside the GBSurface of the 
// GenBrush 'that' 
// boundary, false else
#if BUILDMODE != 0
inline
#endif 
bool GBIsPosInside(GenBrush* that, VecShort2D *pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceIsPosInside(GBSurf(that), pos);
}

// Return the layers of the surface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayers(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceLayers(GBSurf(that));
}

// Add a new GBLayer with dimensions 'dim' to the top of the stack 
// of layers of the GBSurface of the GenBrush' that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBAddLayer(GenBrush* that, VecShort2D* dim) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (dim == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'dim' is null");
    PBErrCatch(GenBrushErr);
  }
  if (VecGet(dim, 0) <= 0 || VecGet(dim, 1) <= 0) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'dim' is invalid (%d>0,%d>0)",
      VecGet(dim, 0), VecGet(dim, 1));
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceAddLayer(GBSurf(that), dim);
}

// Add a new GBLayer with content equals to the image located at 
// 'fileName' to the top of the stack 
// of layers of the GBSurface of the GenBrush' that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBAddLayerFromFile(GenBrush* that, char* fileName) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (fileName == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'fileName' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceAddLayerFromFile(GBSurf(that), fileName);
}

// Get the 'iLayer'-th layer of the GBSurface of the GenBrush 'that'
// 'iLayer' must be valid
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBGetLayer(GenBrush* that, int iLayer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (iLayer < 0 || 
    iLayer >= GSetNbElem(GBSurfaceLayers(GBSurf(that)))) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'iLayer' is invalid (0<=%d<%d)",
      iLayer, GSetNbElem(GBSurfaceLayers(GBSurf(that))));
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceGetLayer(GBSurf(that), iLayer);
}

// Get the number of layer of the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
int GBGetNbLayer(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceNbLayer(GBSurf(that));
}

// Set the position of the GBLayer 'layer' into the set of layers of 
// the GBSurface of the GenBrush'that' to 'pos'
// If 'layer' can't be found in the surface do nothing
// 'pos' must be valid (0<='pos'<nbLayers)
#if BUILDMODE != 0
inline
#endif 
void GBSetLayerPos(GenBrush* that, GBLayer* layer, int pos) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (layer == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'layer' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos < 0 || pos >= GBGetNbLayer(that)) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is invalid (0<=%d<%d)",
      pos, GBGetNbLayer(that));
    PBErrCatch(GenBrushErr);
  }
#endif
  GBSurfaceSetLayerPos(GBSurf(that), layer, pos);
}

// Remove the GBLayer 'layer' from the set of layers of the 
// GBSurface of the GenBrush 'that'
// The memory used by 'layer' is freed
#if BUILDMODE != 0
inline
#endif 
void GBRemoveLayer(GenBrush* that, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (layer == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'layer' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Remove the graphical elements bounded to this layer
  GBRemovePod(that, NULL, NULL, NULL, NULL, NULL, layer);
  // Remove the layer from the surface
  GBSurfaceRemoveLayer(GBSurf(that), layer);
}

// Get the number of pod in the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
int GBGetNbPod(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GSetNbElem(&(that->_pods));
}

// Add a GBObjPod for the Point at position 'pos' to the GenBrush 'that'
// drawn with 'eye', 'hand' and 'tool' in layer 'layer'
// 'pos' must be a vector of 2 or more dimensions
#if BUILDMODE != 0
inline
#endif 
void _GBAddPoint(GenBrush* that, VecFloat* pos, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  GSetAppend(&(that->_pods), 
    GBObjPodCreatePoint(pos, eye, hand, tool, ink, layer));
}

// Add a GBObjPod for the Shapoid 'shap' to the GenBrush 'that'
// drawn with 'eye', 'hand' and 'tool' in layer 'layer'
// 'shap' 's dimension must be 2 or more
#if BUILDMODE != 0
inline
#endif 
void _GBAddShapoid(GenBrush* that, Shapoid* shap, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (shap == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'shap' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  GSetAppend(&(that->_pods), 
    GBObjPodCreateShapoid(shap, eye, hand, tool, ink, layer));
}

// Add a GBObjPod for the SCurve 'curve' to the GenBrush 'that'
// drawn with 'eye', 'hand' and 'tool' in layer 'layer'
// 'curve' 's dimension must be 2 or more
#if BUILDMODE != 0
inline
#endif 
void _GBAddSCurve(GenBrush* that, SCurve* curve, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (curve == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'curve' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  GSetAppend(&(that->_pods), 
    GBObjPodCreateSCurve(curve, eye, hand, tool, ink, layer));
}

// Reset all the final pix of the surface of the GenBrush 'that' to its 
// background color, and reset all the modified flag of layers to true
#if BUILDMODE != 0
inline
#endif 
void GBFlush(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  GBSurfaceFlush(GBSurf(that));
}

// ================ GTK Functions ====================

#if BUILDWITHGRAPHICLIB == 1
  #include "genbrush-inline-GTK.c"
#endif
