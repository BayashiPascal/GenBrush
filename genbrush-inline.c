// ============ GENBRUSH-static inline.C ================

// ================ Functions implementation ====================

// ---------------- GBPixel --------------------------

// Return true if the GBPixel 'that' and 'tho' are the same, else false.
#if BUILDMODE != 0
static inline
#endif 
bool GBPixelIsSame(const GBPixel* const that, const GBPixel* const tho) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (tho == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'tho' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (memcmp(that, tho, sizeof(GBPixel)) == 0);
}

// ---------------- GBLayer --------------------------

// Get the area of the layer (width * height)
#if BUILDMODE != 0
static inline
#endif 
int GBLayerArea(const GBLayer* const that) {
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
static inline
#endif 
VecShort2D* GBLayerPos(const GBLayer* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (VecShort2D*)&(that->_pos);
}

// Get a copy of the position of the GBLayer 'that'
#if BUILDMODE != 0
static inline
#endif 
VecShort2D GBLayerGetPos(const GBLayer* const that) {
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
static inline
#endif 
void GBLayerSetPos(GBLayer* const that, const VecShort2D* const pos) {
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
static inline
#endif 
VecShort2D* GBLayerPrevPos(const GBLayer* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (VecShort2D*)&(that->_prevPos);
}

// Get a copy of the previous position of the GBLayer 'that'
#if BUILDMODE != 0
static inline
#endif 
VecShort2D GBLayerGetPrevPos(const GBLayer* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_prevPos;
}

// Get the scale of the GBLayer 'that'
#if BUILDMODE != 0
static inline
#endif 
VecFloat2D* GBLayerScale(const GBLayer* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (VecFloat2D*)&(that->_scale);
}

// Get a copy of the scale of the GBLayer 'that'
#if BUILDMODE != 0
static inline
#endif 
VecFloat2D GBLayerGetScale(const GBLayer* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_scale;
}

// Set the scale of the GBLayer 'that' to 'scale'
// If the flag _modified==false _prevScale is first set to _scale
// and _modified is set to true
#if BUILDMODE != 0
static inline
#endif 
void GBLayerSetScale(GBLayer* const that, const VecFloat2D* const scale) {
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
#endif
  if (!(that->_modified)) {
    that->_modified = true;
    that->_prevScale = that->_scale;
  }
  that->_scale = *scale;
}

// Get the previous scale of the GBLayer 'that'
#if BUILDMODE != 0
static inline
#endif 
VecFloat2D* GBLayerPrevScale(const GBLayer* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (VecFloat2D*)&(that->_prevScale);
}

// Get a copy of the previous scale of the GBLayer 'that'
#if BUILDMODE != 0
static inline
#endif 
VecFloat2D GBLayerGetPrevScale(const GBLayer* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_prevScale;
}

// Get the dimensions of the GBLayer 'that'
#if BUILDMODE != 0
static inline
#endif 
VecShort2D* GBLayerDim(const GBLayer* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (VecShort2D*)&(that->_dim);  
}

// Get a copy of the dimensions of the GBLayer 'that'
#if BUILDMODE != 0
static inline
#endif 
VecShort2D GBLayerGetDim(const GBLayer* const that) {
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
static inline
#endif 
GBLayerBlendMode GBLayerGetBlendMode(const GBLayer* const that) {
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
static inline
#endif 
void GBLayerSetBlendMode(GBLayer* const that, 
  const GBLayerBlendMode blend) {
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
static inline
#endif 
bool GBLayerIsModified(const GBLayer* const that) {
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
static inline
#endif 
void GBLayerSetModified(GBLayer* const that, const bool flag) {
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
static inline
#endif 
GBLayerStackPosition GBLayerGetStackPos(const GBLayer* const that) {
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
static inline
#endif 
void GBLayerSetStackPos(GBLayer* const that, 
  const GBLayerStackPosition pos) {
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
static inline
#endif 
GSet* GBLayerPixels(const GBLayer* const that) {
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
static inline
#endif 
GSet* GBLayerPixel(const GBLayer* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
GSet* GBLayerPixelSafe(const GBLayer* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
void GBLayerAddPixel(GBLayer* const that, const VecShort2D* const pos, 
  const GBPixel* const pix, const float depth) {
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
static inline
#endif 
void GBLayerAddPixelSafe(GBLayer* const that, 
  const VecShort2D* const pos, const GBPixel* const pix,
  const float depth) {
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
static inline
#endif 
bool GBLayerIsPosInside(const GBLayer* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
void GBLayerFlush(GBLayer* const that) {
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

// ---------------- GBPostProcessing --------------------------

// Return the type of the GBPostProcessing 'that'
#if BUILDMODE != 0
static inline
#endif 
GBPPType GBPostProcessingGetType(const GBPostProcessing* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_type;
}

// ---------------- GBSurface --------------------------

// Return true if the GBSurface 'that' has same dimension and same
// values for _finalPix as GBSurface 'surf'
// Else, return false
#if BUILDMODE != 0
static inline
#endif 
bool GBSurfaceIsSameAs(const GBSurface* const that, 
  const GBSurface* const surf) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (surf == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'surf' is null");
    PBErrCatch(GenBrushErr);
  }
#endif

  // Display different pixels, for debugging purpose
  /*VecShort2D p = VecShortCreateStatic2D();
  do {
    GBPixel* pA = GBSurfaceFinalPixel(that, &p);
    GBPixel* pB = GBSurfaceFinalPixel(surf, &p);
    if (pA->_rgba[0] != pB->_rgba[0] || 
      pA->_rgba[1] != pB->_rgba[1] || 
      pA->_rgba[2] != pB->_rgba[2] || 
      pA->_rgba[3] != pB->_rgba[3]) {
      VecPrint(&p, stdout);
      printf(" %d,%d,%d,%d %d,%d,%d,%d\n", 
        pA->_rgba[0], pA->_rgba[1], pA->_rgba[2], pA->_rgba[3], 
        pB->_rgba[0], pB->_rgba[1], pB->_rgba[2], pB->_rgba[3]); 
    }
  } while (VecStep(&p, GBSurfaceDim(that)));*/

  if (VecIsEqual(GBSurfaceDim(that), GBSurfaceDim(surf)) &&
    memcmp(GBSurfaceFinalPixels(that), GBSurfaceFinalPixels(surf),
    sizeof(GBPixel) * VecGet(GBSurfaceDim(that), 0) *
    VecGet(GBSurfaceDim(that), 1)) == 0)
    return true;
  else
    return false;
}

// Get the type of the GBSurface 'that'
#if BUILDMODE != 0
static inline
#endif 
GBSurfaceType GBSurfaceGetType(const GBSurface* const that) {
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
static inline
#endif 
VecShort2D GBSurfaceGetDim(const GBSurface* const that) {
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
static inline
#endif 
VecShort2D* GBSurfaceDim(const GBSurface* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (VecShort2D*)&(that->_dim);
}

// Get the final pixels of the GBSurface 'that'
#if BUILDMODE != 0
static inline
#endif 
GBPixel* GBSurfaceFinalPixels(const GBSurface* const that) {
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
static inline
#endif 
GBPixel* GBSurfaceFinalPixel(const GBSurface* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
GBPixel GBSurfaceGetFinalPixel(const GBSurface* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
void GBSurfaceSetFinalPixel(GBSurface* const that, 
  const VecShort2D* const pos, const GBPixel* const pix) {
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
static inline
#endif 
GBPixel* GBSurfaceFinalPixelSafe(const GBSurface* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
GBPixel GBSurfaceGetFinalPixelSafe(const GBSurface* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
void GBSurfaceSetFinalPixelSafe(GBSurface* const that, 
  const VecShort2D* const pos, const GBPixel* const pix) {
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
static inline
#endif 
long GBSurfaceArea(const GBSurface* const that) {
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
static inline
#endif 
GBPixel* GBSurfaceBgColor(const GBSurface* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (GBPixel*)&(that->_bgColor);
}

// Get a copy of the background color of the GBSurface 'that'
#if BUILDMODE != 0
static inline
#endif 
GBPixel GBSurfaceGetBgColor(const GBSurface* const that) {
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
static inline
#endif 
void GBSurfaceSetBgColor(GBSurface* const that, 
  const GBPixel* const col) {
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
static inline
#endif 
bool GBSurfaceIsPosInside(const GBSurface* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
GSet* GBSurfaceLayers(const GBSurface* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (GSet*)&(that->_layers);
}

// Add a new GBLayer with dimensions 'dim' to the top of the stack 
// of layers of the GBSurface 'that'
// Return the new GBLayer
#if BUILDMODE != 0
static inline
#endif 
GBLayer* GBSurfaceAddLayer(GBSurface* const that, 
  const VecShort2D* const dim) {
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
static inline
#endif 
GBLayer* GBSurfaceAddLayerFromFile(GBSurface* const that, 
  const char* const fileName) {
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
static inline
#endif 
GBLayer* GBSurfaceLayer(const GBSurface* const that, const int iLayer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (iLayer < 0 || iLayer >= GSetNbElem(&(that->_layers))) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'iLayer' is invalid (0<=%d<%ld)",
      iLayer, GSetNbElem(&(that->_layers)));
    PBErrCatch(GenBrushErr);
  }
#endif
  return (GBLayer*)GSetGet(&(that->_layers), iLayer);
}

// Get the number of layer of the GBSurface 'that'
#if BUILDMODE != 0
static inline
#endif 
int GBSurfaceNbLayer(const GBSurface* const that) {
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
static inline
#endif 
void GBSurfaceSetLayersModified(GBSurface* const that, const bool flag) {
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

// Set the stack position of the GBLayer 'layer' into the set of 
// layers of the GBSurface 'that' to 'pos'
// If 'layer' can't be found in the surface do nothing
// 'pos' must be valid (0<='pos'<nbLayers)
#if BUILDMODE != 0
static inline
#endif 
void GBSurfaceSetLayerStackPos(GBSurface* const that, 
  GBLayer* const layer, const int pos) {
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
    sprintf(GenBrushErr->_msg, "'pos' is invalid (0<=%d<%ld)",
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
static inline
#endif 
void GBSurfaceRemoveLayer(GBSurface* const that, GBLayer* layer) {
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
static inline
#endif 
char* GBSurfaceImageFileName(const GBSurfaceImage* const that) {
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
static inline
#endif 
void GBSurfaceImageSetFileName(GBSurfaceImage* const that, 
  const char* const fileName) {
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
static inline
#endif 
GBEyeType _GBEyeGetType(const GBEye* const that) {
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
static inline
#endif 
VecFloat3D* _GBEyeScale(const GBEye* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (VecFloat3D*)&(that->_scale);
}
 
// Get a copy of the scale of the GBEye
#if BUILDMODE != 0
static inline
#endif 
VecFloat3D _GBEyeGetScale(const GBEye* const that) {
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
static inline
#endif 
VecFloat2D* _GBEyeOrig(const GBEye* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (VecFloat2D*)&(that->_orig);
}

 
// Get a copy of the translation of the GBEye
#if BUILDMODE != 0
static inline
#endif 
VecFloat2D _GBEyeGetOrig(const GBEye* const that) {
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
static inline
#endif 
float _GBEyeGetRot(const GBEye* const that) {
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
static inline
#endif 
void GBEyeSetScaleVec(GBEye* const that, const VecFloat3D* const scale) {
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
static inline
#endif 
void GBEyeSetScaleFloat(GBEye* const that, const float scale) {
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
 
// Set the origin of the GBEye
#if BUILDMODE != 0
static inline
#endif 
void _GBEyeSetOrig(GBEye* const that, const VecFloat2D* const orig) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (orig == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'orig' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  that->_orig = *orig;
  // Update the projection matrix
  GBEyeUpdateProj(that);
}

 
// Set the rotation of the GBEye (in radians)
#if BUILDMODE != 0
static inline
#endif 
void _GBEyeSetRot(GBEye* const that, const float theta) {
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
static inline
#endif 
MatFloat* _GBEyeProj(const GBEye* const that) {
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
static inline
#endif 
void _GBEyeProcess(const GBEye* const that, GBObjPod* const pod) {
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
static inline
#endif 
void GBEyeOrthoSetView(GBEyeOrtho* const that, 
  const GBEyeOrthoView view) {
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
static inline
#endif 
void GBEyeIsometricSetRotY(GBEyeIsometric* const that, 
  const float theta) {
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
static inline
#endif 
void GBEyeIsometricSetRotRight(GBEyeIsometric* const that, 
  const float theta) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Ensure 'theta' is in bounds
  float safeTheta = theta;
  if (safeTheta > PBMATH_HALFPI)
    safeTheta = PBMATH_HALFPI;
  else if (safeTheta < -PBMATH_HALFPI)
    safeTheta = -PBMATH_HALFPI;
  that->_thetaRight = safeTheta;
  GBEyeUpdateProj((GBEye*)that);
}

// Get the angle around Y of the GBEyeOrtho 'that'
#if BUILDMODE != 0
static inline
#endif 
float GBEyeIsometricGetRotY(const GBEyeIsometric* const that) {
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
static inline
#endif 
float GBEyeIsometricGetRotRight(const GBEyeIsometric* const that) {
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
static inline
#endif 
GBHandType _GBHandGetType(const GBHand* const that) {
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
static inline
#endif 
void _GBHandProcess(const GBHand* const that, GBObjPod* const pod) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If 'pod' has no viewed object do nothing
  if (GBObjPodEyeObj(pod) == NULL)
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
static inline
#endif 
GBToolType GBToolGetType(const GBTool* const that) {
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
static inline
#endif 
GBInkType _GBInkGetType(const GBInk* const that) {
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
static inline
#endif 
GBPixel GBInkSolidGet(const GBInkSolid* const that) {
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
static inline
#endif 
void GBInkSolidSet(GBInkSolid* const that, const GBPixel* const col) {
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
static inline
#endif 
void _GBToolDraw(const GBTool* const that, GBObjPod* const pod) {
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
static inline
#endif 
GBObjType GBObjPodGetType(const GBObjPod* const that) {
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
static inline
#endif 
void* GBObjPodObj(const GBObjPod* const that) {
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
static inline
#endif 
void* GBObjPodEyeObj(const GBObjPod* const that) {
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
static inline
#endif 
GSetVecFloat* GBObjPodGetHandObjAsPoints(const GBObjPod* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (GSetVecFloat*)&(that->_handPoints);  
}

// Return the object processed as Shapoids by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
static inline
#endif 
GSetShapoid* GBObjPodGetHandObjAsShapoids(const GBObjPod* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (GSetShapoid*)&(that->_handShapoids);  
}

// Return the object processed as SCurves by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
static inline
#endif 
GSetSCurve* GBObjPodGetHandObjAsSCurves(const GBObjPod* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (GSetSCurve*)&(that->_handSCurves);  
}

// Return the eye in the GBObjPod 'that'
#if BUILDMODE != 0
static inline
#endif 
GBEye* GBObjPodEye(const GBObjPod* const that) {
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
static inline
#endif 
GBHand* GBObjPodHand(const GBObjPod* const that) {
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
static inline
#endif 
GBTool* GBObjPodTool(const GBObjPod* const that) {
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
static inline
#endif 
GBInk* GBObjPodInk(const GBObjPod* const that) {
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
static inline
#endif 
GBLayer* GBObjPodLayer(const GBObjPod* const that) {
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
static inline
#endif 
void _GBObjPodSetEyePoint(GBObjPod* const that, VecFloat* const point) {
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
static inline
#endif 
void _GBObjPodSetEyeShapoid(GBObjPod* const that, Shapoid* const shap) {
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
static inline
#endif 
void _GBObjPodSetEyeSCurve(GBObjPod* const that, SCurve* const curve) {
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
static inline
#endif 
void _GBObjPodSetEye(GBObjPod* const that, GBEye* const eye) {
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
static inline
#endif 
void _GBObjPodSetHand(GBObjPod* const that, GBHand* const hand) {
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
static inline
#endif 
void _GBObjPodSetTool(GBObjPod* const that, GBTool* const tool) {
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
static inline
#endif 
void _GBObjPodSetInk(GBObjPod* const that, GBInk* const ink) {
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
static inline
#endif 
void GBObjPodSetLayer(GBObjPod* const that, GBLayer* const layer) {
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
static inline
#endif 
GBSurface* GBSurf(const GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return that->_surf;
}

// Get the set of pods of the GenBrush 'that'
#if BUILDMODE != 0
static inline
#endif 
GSet* GBPods(const GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (GSet*)&(that->_pods);
}

// Get a copy of the dimensions of the GenBrush 'that'
#if BUILDMODE != 0
static inline
#endif 
VecShort2D GBGetDim(const GenBrush* const that) {
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
static inline
#endif 
GBPixel* GBFinalPixels(const GenBrush* const that) {
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
static inline
#endif 
GBPixel* GBFinalPixel(const GenBrush* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
GBPixel GBGetFinalPixel(const GenBrush* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
void GBSetFinalPixel(GenBrush* const that, const VecShort2D* const pos, 
  const GBPixel* const pix) {
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
static inline
#endif 
GBPixel* GBFinalPixelSafe(const GenBrush* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
GBPixel GBGetFinalPixelSafe(const GenBrush* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
void GBSetFinalPixelSafe(GenBrush* const that, 
  const VecShort2D* const pos, const GBPixel* const pix) {
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
static inline
#endif 
GBSurfaceType GBGetType(const GenBrush* const that) {
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
static inline
#endif 
GBPixel* GBBgColor(const GenBrush* const that) {
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
static inline
#endif 
GBPixel GBGetBgColor(const GenBrush* const that) {
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
static inline
#endif 
void GBSetBgColor(GenBrush* const that, const GBPixel* const col) {
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
static inline
#endif 
char* GBFileName(const GenBrush* const that) {
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
static inline
#endif 
void GBSetFileName(GenBrush* const that, const char* const fileName) {
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
static inline
#endif 
int GBArea(const GenBrush* const that) {
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
static inline
#endif 
bool GBIsPosInside(const GenBrush* const that, 
  const VecShort2D* const pos) {
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
static inline
#endif 
GSet* GBLayers(const GenBrush* const that) {
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
static inline
#endif 
GBLayer* GBAddLayer(GenBrush* const that, const VecShort2D* const dim) {
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
static inline
#endif 
GBLayer* GBAddLayerFromFile(GenBrush* const that, 
  const char* const fileName) {
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
static inline
#endif 
GBLayer* GBLay(const GenBrush* const that, const int iLayer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (iLayer < 0 || 
    iLayer >= GSetNbElem(GBSurfaceLayers(GBSurf(that)))) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'iLayer' is invalid (0<=%d<%ld)",
      iLayer, GSetNbElem(GBSurfaceLayers(GBSurf(that))));
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceLayer(GBSurf(that), iLayer);
}

// Get the number of layer of the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
static inline
#endif 
int GBGetNbLayer(const GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceNbLayer(GBSurf(that));
}

// Set the stack position of the GBLayer 'layer' into the set of 
// layers of the GBSurface of the GenBrush'that' to 'pos'
// If 'layer' can't be found in the surface do nothing
// 'pos' must be valid (0<='pos'<nbLayers)
#if BUILDMODE != 0
static inline
#endif 
void GBSetLayerStackPos(GenBrush* const that, GBLayer* const layer, 
  const int pos) {
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
  GBSurfaceSetLayerStackPos(GBSurf(that), layer, pos);
}

// Remove the GBLayer 'layer' from the set of layers of the 
// GBSurface of the GenBrush 'that'
// The memory used by 'layer' is freed
#if BUILDMODE != 0
static inline
#endif 
void GBRemoveLayer(GenBrush* const that, GBLayer* layer) {
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
static inline
#endif 
int GBGetNbPod(const GenBrush* const that) {
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
static inline
#endif 
void _GBAddPoint(GenBrush* const that, VecFloat* const pos, 
  GBEye* const eye, GBHand* const hand, GBTool* const tool, 
  GBInk* const ink, GBLayer* const layer) {
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
  GSetAppend(GBPods(that), 
    GBObjPodCreatePoint(pos, eye, hand, tool, ink, layer));
}

// Add a GBObjPod for the Shapoid 'shap' to the GenBrush 'that'
// drawn with 'eye', 'hand' and 'tool' in layer 'layer'
// 'shap' 's dimension must be 2 or more
#if BUILDMODE != 0
static inline
#endif 
void _GBAddShapoid(GenBrush* const that, Shapoid* const shap, 
  GBEye* const eye, GBHand* const hand, GBTool* const tool, 
  GBInk* const ink, GBLayer* const layer) {
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
static inline
#endif 
void _GBAddSCurve(GenBrush* const that, SCurve* const curve, 
  GBEye* const eye, GBHand* const hand, GBTool* const tool, 
  GBInk* const ink, GBLayer* const layer) {
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
static inline
#endif 
void GBFlush(GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  GBSurfaceFlush(GBSurf(that));
}

// Return true if the surface of the GenBrush 'that' is same as the
// surface of the GenBrush 'gb'
// Else, return false
#if BUILDMODE != 0
static inline
#endif 
bool GBIsSameAs(const GenBrush* const that, const GenBrush* const gb) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (gb == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'gb' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceIsSameAs(GBSurf(that), GBSurf(gb));
}

// Add a GBPostProcessing of type 'type' to the GenBrush 'that'
// Return the GBPostProcessing
#if BUILDMODE != 0
static inline
#endif 
GBPostProcessing* GBAddPostProcess(GenBrush* const that, 
  const GBPPType type) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  GBPostProcessing* post = GBPostProcessingCreate(type);
  GSetAppend(&(that->_postProcs), post);
  return post;
}

// Remove the GBPostProcessing 'post' from the GenBrush 'that'
#if BUILDMODE != 0
static inline
#endif 
void GBRemovePostProcess(GenBrush* const that, 
  const GBPostProcessing* const post) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  if (GBGetNbPostProcs(that) > 0) {
    GSetIterForward iter = 
      GSetIterForwardCreateStatic(GBPostProcs(that));
    bool flag = true;
    do {
      GBPostProcessing* p = GSetIterGet(&iter);
      if (post == p) {
        GBPostProcessingFree(&p);
        GSetIterRemoveElem(&iter);
      }
    } while (flag == true && GSetIterStep(&iter));
  }
  GSetRemoveFirst(&(that->_postProcs), post);
}

// Remove all the GBPostProcessing from the GenBrush 'that'
#if BUILDMODE != 0
static inline
#endif 
void GBRemoveAllPostProcess(GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  while (GBGetNbPostProcs(that) > 0) {
    GBPostProcessing* post = GSetPop(&(that->_postProcs));
    GBPostProcessingFree(&post);
  }
}

// Get the 'iPost'-th post process of the GenBrush 'that'
#if BUILDMODE != 0
static inline
#endif 
GBPostProcessing* GBPostProcess(const GenBrush* const that, 
  const int iPost) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GSetGet(&(that->_postProcs), iPost);
}

// Get the GSet of GBPostProcessing of the GenBrush 'that'
#if BUILDMODE != 0
static inline
#endif 
GSet* GBPostProcs(const GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return (GSet*)&(that->_postProcs);
}

// Get the number of GBPostProcessing of the GenBrush 'that'
#if BUILDMODE != 0
static inline
#endif 
int GBGetNbPostProcs(const GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GSetNbElem(&(that->_postProcs));
}

// Remove all the GBObjPods from the GenBrush 'that'
#if BUILDMODE != 0
static inline
#endif 
void GBRemoveAllPod(GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  while (GBGetNbPod(that) > 0) {
    GBObjPod* pod = GSetPop(&(that->_pods));
    GBObjPodFree(&pod);
  }
}


// ================ GTK Functions ====================

#if BUILDWITHGRAPHICLIB == 1
  #include "genbrush-inline-GTK.c"
#endif
