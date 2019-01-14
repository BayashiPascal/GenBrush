// ============ GENBRUSH.H ================

#ifndef GENBRUSH_H
#define GENBRUSH_H

// About the coordinates systems:
// In input, the coordinates system must be a left handed
// coordinates system, with x toward the right, y toward the top and 
// z away from the viewer.
//   y
//   ^ z
//   |/
//   -->x
// So the front view of a GBEyeOrtho has x to the right, y to the top
// and z as the depth component (positive is away from the viewer).
//   y
//   ^
//   |
//  z-->x
// For a GBEyeIsometric, x is toward the top-right, y is toward the top
// and z is toward the top-left.
//   y
// z ^ x 
//  \|/
// In output, the coordinates system of the surface is x toward the 
// right and y toward the top, with the origin at the bottom-left of 
// the surface.
//   y
//   ^
//   |
//   -->x
// In GBSurface, the final pixels are stored in rows from left to 
// right, the first row is the top row in the surface.

// ================= Include =================

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "pberr.h"
#include "pbmath.h"
#include "gset.h"
#include "shapoid.h"
#include "bcurve.h"

// ================= Define ==================

// Color index in _rgba
// for CAIRO_FORMAT_ARGB32 :
#define GBPixelRed 2
#define GBPixelGreen 1
#define GBPixelBlue 0
#define GBPixelAlpha 3

#define GBColorWhite (GBPixel){ \
    ._rgba[GBPixelAlpha]=255,._rgba[GBPixelRed]=255, \
    ._rgba[GBPixelGreen]=255,._rgba[GBPixelBlue]=255}
#define GBColorBlack (GBPixel){ \
    ._rgba[GBPixelAlpha]=255,._rgba[GBPixelRed]=0, \
    ._rgba[GBPixelGreen]=0,._rgba[GBPixelBlue]=0}
#define GBColorTransparent (GBPixel){ \
    ._rgba[GBPixelAlpha]=0,._rgba[GBPixelRed]=0, \
    ._rgba[GBPixelGreen]=0,._rgba[GBPixelBlue]=0}
#define GBColorRed (GBPixel){ \
    ._rgba[GBPixelAlpha]=255,._rgba[GBPixelRed]=255, \
    ._rgba[GBPixelGreen]=0,._rgba[GBPixelBlue]=0}
#define GBColorGreen (GBPixel){ \
    ._rgba[GBPixelAlpha]=255,._rgba[GBPixelRed]=0, \
    ._rgba[GBPixelGreen]=255,._rgba[GBPixelBlue]=0}
#define GBColorBlue (GBPixel){ \
    ._rgba[GBPixelAlpha]=255,._rgba[GBPixelRed]=0, \
    ._rgba[GBPixelGreen]=0,._rgba[GBPixelBlue]=255}
    
// ================= Data structure ===================

typedef struct GBPixel {
  // Pixel values
  unsigned char _rgba[4];
} GBPixel;

typedef enum GBLayerBlendMode {
  GBLayerBlendModeDefault, // Simple overwritting
  GBLayerBlendModeNormal, // Blending according to relative alpha
  GBLayerBlendModeOver // Blending according to alpha of top pix
} GBLayerBlendMode;

typedef enum GBLayerStackPosition {
  GBLayerStackPosBg, // Layers' pixels are stacked according to their 
                     // index 
  GBLayerStackPosInside, // Layers' pixels are stacked according to 
                         // their depth
  GBLayerStackPosFg // Layers' pixels are stacked according to their 
                    // index 
} GBLayerStackPosition;

typedef struct GBStackedPixel {
  // Pixel values
  GBPixel _val;
  // Depth
  float _depth;
  // BlendMode
  GBLayerBlendMode _blendMode;
}  GBStackedPixel;

typedef struct GBLayer {
  // Position in the GBSurface of the bottom left corner of the GBLayer
  // (coordinates of the bottom left corner in the GBSurface are 0,0)
  // (x toward right, y toward top)
  VecShort2D _pos;
  // Previous position
  VecShort2D _prevPos;
  // Dimension
  VecShort2D _dim;
  // _dim[0] x _dim[1] GSet of StackedPixel (stored by rows)
  GSet* _pix;
  // Composition
  GBLayerBlendMode _blendMode;
  // Modified flag
  bool _modified;
  // Position in stack
  GBLayerStackPosition _stackPos;
} GBLayer;

typedef enum GBPPType {
  GBPPTypeNormalizeHue
} GBPPType;

typedef struct GBPostProcessing {
  enum GBPPType _type;
} GBPostProcessing;

typedef enum GBSurfaceType {
  GBSurfaceTypeDefault,
  GBSurfaceTypeImage
#if BUILDWITHGRAPHICLIB == 1
    , GBSurfaceTypeApp, GBSurfaceTypeWidget
#endif
} GBSurfaceType;

typedef struct GBSurface {
  // Type of the surface
  GBSurfaceType _type;
  // Dimension (x toward right, y toward top)
  VecShort2D _dim;
  // Background color
  GBPixel _bgColor;
  // Set of GBLayer
  GSet _layers;
  // Final pixels
  GBPixel* _finalPix;
} GBSurface;

typedef struct GBSurfaceImage {
  // Parent GBSurface
  GBSurface _surf;
  // File name
  char* _fileName;
} GBSurfaceImage;

typedef enum GBEyeType {
  GBEyeTypeOrtho,
  GBEyeTypeIsometric
} GBEyeType;

typedef struct GBEye {
  // Type
  GBEyeType _type;
  // Scale
  VecFloat3D _scale;
  // (0,0) in graphical element is translated at _orig in the surface
  VecFloat2D _orig;
  // Rotation of the eye (clockwise, radians)
  float _theta;
  // Projection matrix from real space to surface space
  // Transformation are applied in the following order in the 
  // surface space: 
  // scale, rotation, translation
  MatFloat* _proj;
} GBEye;

typedef enum GBEyeOrthoView {
  // View from ...
  GBEyeOrthoViewFront,
  GBEyeOrthoViewRear,
  GBEyeOrthoViewTop,
  GBEyeOrthoViewBottom,
  GBEyeOrthoViewLeft,
  GBEyeOrthoViewRight
} GBEyeOrthoView;

typedef struct GBEyeOrtho {
  // GBEye data
  GBEye _eye;
  // Orientation of the orthographic projection
  GBEyeOrthoView _view;
} GBEyeOrtho;

typedef struct GBEyeIsometric {
  // GBEye data
  GBEye _eye;
  // Rotation right handed around y (in radians, initially 0.0)
  float _thetaY;
  // Rotation right handed around the right direction in the surface
  // (in radians, in [-pi/2, pi/2], initially pi/4)
  float _thetaRight;
} GBEyeIsometric;

typedef enum GBHandType {
  GBHandTypeDefault
} GBHandType;

typedef struct GBHand {
  // Type
  GBHandType _type;
} GBHand;

typedef struct GBHandDefault {
  // Parent
  GBHand _hand;
} GBHandDefault;

typedef enum GBToolType {
  GBToolTypePlotter
} GBToolType;

typedef struct GBTool {
  // Type
  GBToolType _type;
} GBTool;

typedef struct GBToolPlotter {
  // Parent
  GBTool _tool;
} GBToolPlotter;

typedef enum GBInkType {
  GBInkTypeSolid
} GBInkType;

typedef struct GBInk {
  // Type
  GBInkType _type;
} GBInk;

typedef struct GBInkSolid {
  // Parent
  GBInk _ink;
  // Color
  GBPixel _color;
} GBInkSolid;

typedef enum GBObjType {
  GBObjTypePoint,
  GBObjTypeShapoid,
  GBObjTypeSCurve
} GBObjType;

typedef struct GBObjPod {
  // Type
  GBObjType _type;
  // Source object
  union {
    VecFloat* _srcPoint;
    Shapoid* _srcShapoid;
    SCurve* _srcSCurve;
  };
  // Object projected through eye
  union {
    VecFloat* _eyePoint;
    Shapoid* _eyeShapoid;
    SCurve* _eyeSCurve;
  };
  // GSet of projected Points (VecFloat) through eye + hand
  GSetVecFloat _handPoints;
  // GSet of projected Shapoids through eye + hand
  GSetShapoid _handShapoids;
  // GSet of projected Shapoids through eye + hand
  GSetSCurve _handSCurves;
  // Eye
  GBEye* _eye;
  // Hand
  GBHand* _hand;
  // Tool
  GBTool* _tool;
  // Ink
  GBInk* _ink;
  // Layer
  GBLayer* _layer;
} GBObjPod;

typedef enum GBScaleMethod {
  GBScaleMethod_AvgNeighbour
} GBScaleMethod;
#define GBScaleMethod_Default GBScaleMethod_AvgNeighbour

typedef struct GenBrush {
  // Surface of the GenBrush
  GBSurface* _surf;
  // Set of GBObjPod to be drawn
  GSet _pods;
  // Set of GBPostProcessing to be apply at the end of GBUpdate
  GSet _postProcs;
} GenBrush;

// ================ Functions declaration ====================

// ---------------- GBPixel --------------------------

// Blend the pixel 'pix' into the pixel 'that'
// BlendNormal mixes colors according to their relative alpha value
// and add the alpha values
void GBPixelBlendNormal(GBPixel* const that , const GBPixel* const pix);

// Blend the pixel 'pix' into the pixel 'that'
// BlendOver mixes colors according to the alpha value of 'pix'
// and add the alpha values
void GBPixelBlendOver(GBPixel* const that, const GBPixel* const pix);

// Return the blend result of the stack of Pixel 'stack'
// If there is transparency down to the bottom of the stack, use the 
// background color 'bgColor'
// If the stack is empty, return a transparent pixel
GBPixel GBPixelStackBlend(const GSet* const stack, 
  const GBPixel* const bgColor);

// ---------------- GBLayer --------------------------

// Create a new GBLayer with dimensions 'dim'
// The layer is initialized with empty stacks of pixel
// _pos = (0,0)
// blendMode = GBLayerBlendModeDefault
// stackPos = GBLayerStackPosBg
GBLayer* GBLayerCreate(const VecShort2D* const dim);

// Free the GBLayer 'that'
void GBLayerFree(GBLayer** that);

// Get the area of the layer (width * height)
#if BUILDMODE != 0
inline
#endif 
int GBLayerArea(const GBLayer* const that);

// Get the position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBLayerPos(const GBLayer* const that);

// Get a copy of the position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBLayerGetPos(const GBLayer* const that);

// Set the position of the GBLayer 'that' to 'pos'
// If the flag _modified==false _prevPos is first set to _pos
// and _modified is set to true
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetPos(GBLayer* const that, const VecShort2D* const pos);

// Get the previous position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBLayerPrevPos(const GBLayer* const that);

// Get a copy of the previous position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBLayerGetPrevPos(const GBLayer* const that);

// Get the dimensions of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBLayerDim(const GBLayer* const that);

// Get a copy of the dimensions of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBLayerGetDim(const GBLayer* const that);

// Get a copy of the blend mode of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
GBLayerBlendMode GBLayerGetBlendMode(const GBLayer* const that);

// Set the blend mode of the GBLayer 'that' to 'blend'
// Set the flag _modified to true
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetBlendMode(GBLayer* const that, 
  const GBLayerBlendMode blend);

// Get a copy of the modified flag of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
bool GBLayerIsModified(const GBLayer* const that);

// Set the modified flag of the GBLayer 'that' to 'flag'
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetModified(GBLayer* const that, const bool flag);

// Get a copy of the stack position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
GBLayerStackPosition GBLayerGetStackPos(const GBLayer* const that);

// Set the stack position of the GBLayer 'that' to 'pos'
// Set the flag _modified to true
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetStackPos(GBLayer* const that, 
  const GBLayerStackPosition pos);

// Get the stacked pixels of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayerPixels(const GBLayer* const that);

// Get the stacked pixels of the GBLayer 'that' at position 'pos'
// 'pos' must be inside the layer
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayerPixel(const GBLayer* const that, 
  const VecShort2D* const pos);

// Get the stacked pixels of the GBLayer 'that' at position 'pos'
// If 'pos' is out of the layer return NULL
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayerPixelSafe(const GBLayer* const that, 
  const VecShort2D* const pos);

// Add the pixel 'pix' with depth 'depth' on top of the stack at 
// position 'pos' of GBLayer 'that'
// Set the flag _modified to true
// 'pos' must be inside the layer
// If the pixel is completely transparent (_rgba[GBPixelAlpha]==0) 
// do nothing
#if BUILDMODE != 0
inline
#endif 
void GBLayerAddPixel(GBLayer* const that, const VecShort2D* const pos, 
  const GBPixel* const pix, const float depth);

// Add the pixel 'pix' with depth 'depth' on top of the stack at 
// position 'pos' of GBLayer 'that'
// Set the flag _modified to true
// If 'pos' is out of the layer do nothing
// If the pixel is completely transparent (_rgba[GBPixelAlpha]==0) 
// do nothing
#if BUILDMODE != 0
inline
#endif 
void GBLayerAddPixelSafe(GBLayer* const that, 
  const VecShort2D* const pos, const GBPixel* const pix,
  const float depth);

// Return true if the position 'pos' is inside the layer 'that' 
// boundary, false else
#if BUILDMODE != 0
inline
#endif 
bool GBLayerIsPosInside(const GBLayer* const that, 
  const VecShort2D* const pos);

// Delete all the stacked pixels in the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
void GBLayerFlush(GBLayer* const that);

// Create a new GBLayer with dimensions and content given by the 
// image on disk at location 'fileName'
// Return NULL if we couldn't create the layer
GBLayer* GBLayerCreateFromFile(const char* const fileName);

// Get the boundary of the GBLayer 'that' inside the GBSurface 'surf'
// The boundaries are given as a Facoid
// If the flag 'prevPos' is true, gives the bounday at the previous
// position
// Return NULL if the layer is completely out of the surface
Facoid* GBLayerGetBoundaryInSurface(const GBLayer* const that, 
  const GBSurface* const surf, const bool prevPos);

// ---------------- GBPostProcessing --------------------------

// Create a static GBPostProcessing with type 'type'
GBPostProcessing* GBPostProcessingCreate(const GBPPType type);

// Create a new static GBPostProcessing with type 'type'
GBPostProcessing GBPostProcessingCreateStatic(const GBPPType type);

// Free the memory used by the GBPostProcessing 'that'
void GBPostProcessingFree(GBPostProcessing** that);

// Return the type of the GBPostProcessing 'that'
#if BUILDMODE != 0
inline
#endif 
GBPPType GBPostProcessingGetType(const GBPostProcessing* const that);

// ---------------- GBSurface --------------------------

// Create a new static GBSurface with dimension 'dim' and type 'type'
// _finalPix is set to 0
// _bgColor is set to white
GBSurface GBSurfaceCreateStatic(const GBSurfaceType type, 
  const VecShort2D* const dim);

// Create a static GBSurface with dimension 'dim' and type 'type'
// _finalPix is set to 0
// _bgColor is set to white
GBSurface* GBSurfaceCreate(const GBSurfaceType type, 
  const VecShort2D* const dim);

// Free the memory used by the GBSurface 'that'
void GBSurfaceFree(GBSurface** that);

// Free the memory used by the properties of the GBSurface 'that'
void GBSurfaceFreeStatic(GBSurface* const that);

// Clone the GBSurface 'that'
GBSurface GBSurfaceClone(const GBSurface* const that);

// Return true if the GBSurface 'that' has same dimension and same
// values for _finalPix as GBSurface 'surf'
// Else, return false
#if BUILDMODE != 0
inline
#endif 
bool GBSurfaceIsSameAs(const GBSurface* const that, 
  const GBSurface* const surf);

// Get the type of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBSurfaceType GBSurfaceGetType(const GBSurface* const that);

// Get a copy of the dimensions of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBSurfaceGetDim(const GBSurface* const that);

// Get the dimensions of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBSurfaceDim(const GBSurface* const that);

// Get the final pixels of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceFinalPixels(const GBSurface* const that);

// Get the final pixel at position 'pos' of the GBSurface 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceFinalPixel(const GBSurface* const that, 
  const VecShort2D* const pos);

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel GBSurfaceGetFinalPixel(const GBSurface* const that, 
  const VecShort2D* const pos);

// Set the final pixel at position 'pos' of the GBSurface 'that' to
// the pixel 'pix'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetFinalPixel(GBSurface* const that, 
  const VecShort2D* const pos, const GBPixel* const pix);

// Get the final pixel at position 'pos' of the GBSurface 'that'
// If 'pos' is out of the surface return NULL
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceFinalPixelSafe(const GBSurface* const that, 
  const VecShort2D* const pos);

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// 'that'
// If 'pos' is out of the surface return a transparent pixel
#if BUILDMODE != 0
inline
#endif 
GBPixel GBSurfaceGetFinalPixelSafe(const GBSurface* const that, 
  const VecShort2D* const pos);

// Set the final pixel at position 'pos' of the GBSurface 'that' to
// the pixel 'pix'
// If 'pos' is out of the surface do nothing
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetFinalPixelSafe(GBSurface* const that, 
  const VecShort2D* const pos, const GBPixel* const pix);

// Get the area of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
long GBSurfaceArea(const GBSurface* const that);

// Get the background color of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceBgColor(const GBSurface* const that);

// Get a copy of the background color of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel GBSurfaceGetBgColor(const GBSurface* const that);

// Set the background color of the GBSurface 'that' to 'col'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetBgColor(GBSurface* const that, 
  const GBPixel* const col);

// Return true if the position 'pos' is inside the GBSurface 'that' 
// boundary, false else
#if BUILDMODE != 0
inline
#endif 
bool GBSurfaceIsPosInside(const GBSurface* const that, 
  const VecShort2D* const pos);

// Get the set of layers of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBSurfaceLayers(const GBSurface* const that);

// Add a new GBLayer with dimensions 'dim' to the top of the stack 
// of layers of the GBSurface 'that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBSurfaceAddLayer(GBSurface* const that, 
  const VecShort2D* const dim);

// Add a new GBLayer with the content of the image located at 
// 'fileName' to the top of the stack of layers of the GBSurface 'that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBSurfaceAddLayerFromFile(GBSurface* const that, 
  const char* const fileName);

// Get the 'iLayer'-th layer of the GBSurface 'that'
// 'iLayer' must be valid
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBSurfaceLayer(const GBSurface* const that, const int iLayer);

// Get the number of layer of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
int GBSurfaceNbLayer(const GBSurface* const that);

// Set the _modified flag of all layers of the GBSurface 'that' 
// to 'flag'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetLayersModified(GBSurface* const that, const bool flag);

// Set the stack position of the GBLayer 'layer' into the set of 
// layers of the GBSurface 'that' to 'pos'
// If 'layer' can't be found in the surface do nothing
// 'pos' must be valid (0<='pos'<nbLayers)
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetLayerStackPos(GBSurface* const that, 
  GBLayer* const layer, const int pos);

// Remove the GBLayer 'layer' from the set of layers of the 
// GBSurface 'that'
// The memory used by 'layer' is freed
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceRemoveLayer(GBSurface* const that, GBLayer* layer);

// Get a GSet of Facoid representing the sub areas of the GBSurface 
// 'that' affected by layers with _modified flag equal to true
// If there is no modified sub area return an empty GSet
GSetShapoid* GBSurfaceGetModifiedArea(const GBSurface* const that);

// Update the final pixels according to layers of the GBSurface 'that'
// Update only pixels affected by layers with the _modified flag 
// equals to true
void GBSurfaceUpdate(GBSurface* const that);

// Reset all the final pix of the GBSurface 'that' to its 
// background color, and reset all the modified flag of layers to true
void GBSurfaceFlush(GBSurface* const that);

// Apply the post processing 'post' to the final pixels in the 
// GBSurface 'that'
void GBSurfacePostProcess(GBSurface* const that, 
  const GBPostProcessing* const post);

// ---------------- GBSurfaceImage --------------------------

// Create a new GBSurfaceImage with dimension 'dim'
GBSurfaceImage* GBSurfaceImageCreate(const VecShort2D* const dim);

// Free the memory used by the GBSurfaceImage 'that'
void GBSurfaceImageFree(GBSurfaceImage** that);

// Clone the GBSurfaceImage 'that'
GBSurfaceImage* GBSurfaceImageClone(const GBSurfaceImage* const that);

// Get the filename of the GBSurfaceImage 'that'
#if BUILDMODE != 0
inline
#endif 
char* GBSurfaceImageFileName(const GBSurfaceImage* const that);

// Set the filename of the GBSurfaceImage 'that' to 'fileName'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceImageSetFileName(GBSurfaceImage* const that, 
  const char* const fileName);

// Save a GBSurfaceImage 'that'
// If the filename is not set do nothing and return false
// Return true if it could save the surface, false else
bool GBSurfaceImageSave(const GBSurfaceImage* const that);

// Create a new GBSurfaceImage with one layer containing the content 
// of the image located at 'fileName' and dimensions equal to the 
// dimensions of the image
// If the image couldn't be loaded return NULL
GBSurfaceImage* GBSurfaceImageCreateFromFile(const char* const fileName);

// ---------------- GBEye --------------------------

// Create a new GBEye with type 'type'
GBEye GBEyeCreateStatic(const GBEyeType type);

// Free the memory used by the GBEye 'that'
void GBEyeFreeStatic(GBEye* const that);

// Return the type of the GBEye 'that'
#if BUILDMODE != 0
inline
#endif 
GBEyeType _GBEyeGetType(const GBEye* const that);

// Get the scale of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat3D* _GBEyeScale(const GBEye* const that);
 
// Get a copy of the scale of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat3D _GBEyeGetScale(const GBEye* const that);
 
// Get the translation of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat2D* _GBEyeOrig(const GBEye* const that);
 
// Get a copy of the translation of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat2D _GBEyeGetOrig(const GBEye* const that);
 
// Get the rotation of the GBEye (in radians)
#if BUILDMODE != 0
inline
#endif 
float _GBEyeGetRot(const GBEye* const that);

// Set the scale of the GBEye
#if BUILDMODE != 0
inline
#endif 
void GBEyeSetScaleVec(GBEye* const that, const VecFloat3D* const scale);
#if BUILDMODE != 0
inline
#endif 
void GBEyeSetScaleFloat(GBEye* const that, const float scale);
 
// Set the origin of the GBEye
#if BUILDMODE != 0
inline
#endif 
void _GBEyeSetOrig(GBEye* const that, const VecFloat2D* const orig);
 
// Set the rotation of the GBEye (in radians)
#if BUILDMODE != 0
inline
#endif 
void _GBEyeSetRot(GBEye* const that, const float theta);
  
// Update the projection matrix of the GBEye according to scale, rot
// and origin
void GBEyeUpdateProj(GBEye* const that);
  
// Get the matrix projection of the eye
#if BUILDMODE != 0
inline
#endif 
MatFloat* _GBEyeProj(const GBEye* const that);

// Call the appropriate GBEye<>Process according to the type of the
// GBEye 'that'
#if BUILDMODE != 0
inline
#endif 
void _GBEyeProcess(const GBEye* const that, GBObjPod* const pod); 

// Return the projection through the GBEye 'that' of the Point 'point' 
VecFloat* GBEyeGetProjectedPoint(const GBEye* const that, 
  const VecFloat* const point);

// Return the projection through the GBEye 'that' of the SCurve 'curve' 
SCurve* GBEyeGetProjectedCurve(const GBEye* const that, 
  const SCurve* const curve);

// Return the projection through the GBEye 'that' of the Shapoid 'shap' 
Shapoid* GBEyeGetProjectedShapoid(const GBEye* const that, 
  const Shapoid* const shap);

// ---------------- GBEyeOrtho --------------------------

// Return a new GBEyeOrtho with orientation 'view'
// scale is initialized to (1,1), trans to (0,0) and rot to 0
GBEyeOrtho* GBEyeOrthoCreate(const GBEyeOrthoView view);

// Free the memory used by the GBEyeOrho 'that'
void GBEyeOrthoFree(GBEyeOrtho** that);

// Set the orientation the GBEyeOrtho 'that'
#if BUILDMODE != 0
inline
#endif 
void GBEyeOrthoSetView(GBEyeOrtho* const that, 
  const GBEyeOrthoView view);

// Process the object of the GBObjPod 'pod' to update the viewed object 
// through the GBEyeOrtho 'that'
void GBEyeOrthoProcess(const GBEyeOrtho* const that, 
  GBObjPod* const pod); 

// ---------------- GBEyeIsometric --------------------------

// Return a new GBEyeIsometric with orientation 'view'
// scale is initialized to (1,1), trans to (0,0) and rot to 0
// thetaY is initialized to pi/4 and thetaRight to pi/4
GBEyeIsometric* GBEyeIsometricCreate();

// Free the memory used by the GBEyeIsometric 'that'
void GBEyeIsometricFree(GBEyeIsometric** that);

// Set the angle around Y of the GBEyeOrtho to 'theta' (in radians)
#if BUILDMODE != 0
inline
#endif 
void GBEyeIsometricSetRotY(GBEyeIsometric* const that, 
  const float theta);

// Set the angle around Right of the GBEyeOrtho to 'theta' 
// (in radians, in [-pi/2, pi/2])
// If 'theta' is out of range it is automatically bounded 
// (ex: pi -> pi/2)
#if BUILDMODE != 0
inline
#endif 
void GBEyeIsometricSetRotRight(GBEyeIsometric* const that, 
  const float theta);

// Get the angle around Y of the GBEyeOrtho 'that'
#if BUILDMODE != 0
inline
#endif 
float GBEyeIsometricGetRotY(const GBEyeIsometric* const that);

// Get the angle around Right of the GBEyeOrtho 'that'
#if BUILDMODE != 0
inline
#endif 
float GBEyeIsometricGetRotRight(const GBEyeIsometric* const that);

// Process the object of the GBObjPod 'pod' to update the viewed object 
// through the GBEyeIsometric 'that'
void GBEyeIsometricProcess(const GBEyeIsometric* const that, 
  GBObjPod* const pod); 

// ---------------- GBHand --------------------------

// Create a new GBHand with type 'type'
GBHand GBHandCreateStatic(const GBHandType type);

// Free the memory used by the GBHand 'that'
void GBHandFreeStatic(GBHand* const that);

// Return the type of the GBHand 'that'
#if BUILDMODE != 0
inline
#endif 
GBHandType _GBHandGetType(const GBHand* const that);

// Call the appropriate GBHand<>Process according to the type of the
// GBHand 'that'
#if BUILDMODE != 0
inline
#endif 
void _GBHandProcess(const GBHand* const that, GBObjPod* const pod); 

// ---------------- GBHandDefault --------------------------

// Create a new GBHandDefault
GBHandDefault* GBHandDefaultCreate();

// Free the memory used by the GBHandDefault 'that'
void GBHandDefaultFree(GBHandDefault** that);

// Process the viewed projection of the object in GBObjPod 'pod' into
// its handed projection through the GBHandDefault 'that' 
void GBHandDefaultProcess(const GBHandDefault* const that, 
  GBObjPod* const pod); 

// ---------------- GBTool --------------------------

// Create a static GBTool with GBToolType 'type'
GBTool GBToolCreateStatic(const GBToolType type);

// Free the memory used by the GBTool 'that'
void GBToolFreeStatic(GBTool* const that);

// Return a copy of the type of the GBTool 'that'
#if BUILDMODE != 0
inline
#endif 
GBToolType GBToolGetType(const GBTool* const that);

// Function to call the appropriate GBTool<>Draw function according to 
// type of GBTool 'that'
#if BUILDMODE != 0
inline
#endif 
void _GBToolDraw(const GBTool* const that, GBObjPod* const pod);

// ---------------- GBToolPlotter --------------------------

// Create a new GBToolPlotter
GBToolPlotter* GBToolPlotterCreate();

// Free the memory used by the GBToolPlotter 'that'
void GBToolPlotterFree(GBToolPlotter** that);

// Draw the object in the GBObjPod 'pod' with the GBToolPlotter 'that'
void GBToolPlotterDraw(const GBToolPlotter* const that, 
  const GBObjPod* const pod);

// ---------------- GBInk --------------------------

// Return the type of the GBInk 'that'
#if BUILDMODE != 0
inline
#endif 
GBInkType _GBInkGetType(const GBInk* const that);

// Free the memory used by the GBInk 'that'
void GBInkFree(GBInk* const that);

// Entry point for the GBTool<>Draw function to get the color of the 
// appropriate GBInk according to the type of 'that'
// posInternal represents the position in the object internal space
// posExternal represents the position in the global coordinates system
// posLayer represents the position in the destination layer
GBPixel _GBInkGet(const GBInk* const that, const GBTool* const tool, 
  const GBObjPod* const pod, const VecFloat* const posInternal, 
  const VecFloat* const posExternal, const VecShort* const posLayer);

// ---------------- GBInkSolid --------------------------

// Create a new GBInkSolid with color 'col'
GBInkSolid* GBInkSolidCreate(const GBPixel* const col);

// Free the memory used by the GBInkSolid 'that'
void GBInkSolidFree(GBInkSolid** that);

// Get the color of the GBInkSolid 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel GBInkSolidGet(const GBInkSolid* const that);

// Set the color of the GBInkSolid 'that' to 'col'
#if BUILDMODE != 0
inline
#endif 
void GBInkSolidSet(GBInkSolid* const that, const GBPixel* const col);

// ---------------- GBObjPod --------------------------

// Create a new GBObjPod for a Point at position 'pos'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'pos' must be a vector of 2 or more dimensions
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreatePoint(VecFloat* const pos, GBEye* const eye, 
  GBHand* const hand, GBTool* const tool, GBInk* const ink, 
  GBLayer* const layer);

// Create a new GBObjPod for the Shapoid 'shap'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'shap' 's dimension must be 2 or more
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreateShapoid(Shapoid* const shap, GBEye* const eye, 
  GBHand* const hand, GBTool* const tool, GBInk* const ink,
  GBLayer* const layer);

// Create a new GBObjPod for the SCurve 'curve'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'curve' 's dimension must be 2 or more
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreateSCurve(SCurve* const curve, GBEye* const eye, 
  GBHand* const hand, GBTool* const tool, GBInk* const ink, 
  GBLayer* const layer);
  
// Free the memory used by the GBObjPod 'that'
void GBObjPodFree(GBObjPod** that);

// Return the type of the object in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBObjType GBObjPodGetType(const GBObjPod* const that);

// Return the object in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
void* GBObjPodObj(const GBObjPod* const that);

// Return the object viewed by its attached eye in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
void* GBObjPodEyeObj(const GBObjPod* const that);

// Return the object processed as Points by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GSetVecFloat* GBObjPodGetHandObjAsPoints(const GBObjPod* const that);

// Return the object processed as Shapoids by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GSetShapoid* GBObjPodGetHandObjAsShapoids(const GBObjPod* const that);

// Return the object processed as SCurves by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GSetSCurve* GBObjPodGetHandObjAsSCurves(const GBObjPod* const that);

// Return the eye in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBEye* GBObjPodEye(const GBObjPod* const that);

// Return the hand in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBHand* GBObjPodHand(const GBObjPod* const that);

// Return the tool in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBTool* GBObjPodTool(const GBObjPod* const that);

// Return the ink in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBInk* GBObjPodInk(const GBObjPod* const that);

// Return the layer in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBObjPodLayer(const GBObjPod* const that);

// Set the Point viewed by its attached eye in the GBObjPod 'that' 
// to 'point'
// If 'point' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEyePoint(GBObjPod* const that, VecFloat* const point);

// Set the Shapoid viewed by its attached eye in the GBObjPod 'that' 
// to 'shap'
// If 'shap' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEyeShapoid(GBObjPod* const that, Shapoid* const shap);

// Set the SCurve viewed by its attached eye in the GBObjPod 'that' 
// to 'curve'
// If 'curve' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEyeSCurve(GBObjPod* const that, SCurve* const curve);

// Set the eye in the GBObjPod 'that' to 'eye'
// If 'eye' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEye(GBObjPod* const that, GBEye* const eye);

// Set the hand in the GBObjPod 'that' to 'hand'
// If 'hand' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetHand(GBObjPod* const that, GBHand* const hand);

// Set the tool in the GBObjPod 'that' to 'tool'
// If 'tool' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetTool(GBObjPod* const that, GBTool* const tool);

// Set the ink in the GBObjPod 'that' to 'ink'
// If 'ink' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetInk(GBObjPod* const that, GBInk* const ink);

// Set the layer in the GBObjPod 'that' to 'layer'
// If 'layer' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void GBObjPodSetLayer(GBObjPod* const that, GBLayer* const layer);

// ---------------- GenBrush --------------------------

// Create a new GenBrush with a GBSurface of type GBSurfaceTypeImage
// and dimensions 'dim'
GenBrush* GBCreateImage(const VecShort2D* const dim);

// Free memory used by the GenBrush 'that'
void GBFree(GenBrush** that);

// Create a new GenBrush with one layer containing the content 
// of the image located at 'fileName' and dimensions equal to the 
// dimensions of the image
// If the image couldn't be loaded return NULL
GenBrush* GBCreateFromFile(const char* const fileName);

// Get the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBSurface* GBSurf(const GenBrush* const that);

// Get the set of pods of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBPods(const GenBrush* const that);

// Get the dimensions of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBDim(const GenBrush* const that);

// Get a copy of the dimensions of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBGetDim(const GenBrush* const that);

// Get the final pixels of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBFinalPixels(const GenBrush* const that);

// Get the final pixel at position 'pos' of the GBSurface of the GB 
// 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBFinalPixel(const GenBrush* const that, 
  const VecShort2D* const pos);

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// of the GB 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel GBGetFinalPixel(const GenBrush* const that, 
  const VecShort2D* const pos);

// Set the final pixel at position 'pos' of the GBSurface of the GB 
// 'that' to the pixel 'pix'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
void GBSetFinalPixel(GenBrush* const that, const VecShort2D* const pos, 
  const GBPixel* const pix);

// Get the final pixel at position 'pos' of the GBSurface of the GB 
// 'that'
// If 'pos' is out of the surface return NULL
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBFinalPixelSafe(const GenBrush* const that, 
  const VecShort2D* const pos);

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// of the GB 'that'
// If 'pos' is out of the surface return a transparent pixel
#if BUILDMODE != 0
inline
#endif 
GBPixel GBGetFinalPixelSafe(const GenBrush* const that, 
  const VecShort2D* const pos);

// Set the final pixel at position 'pos' of the GBSurface of the GB 
// 'that' to the pixel 'pix'
// If 'pos' is out of the surface do nothing
#if BUILDMODE != 0
inline
#endif 
void GBSetFinalPixelSafe(GenBrush* const that, 
  const VecShort2D* const pos, const GBPixel* const pix);

// Get the type of the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBSurfaceType GBGetType(const GenBrush* const that);

// Get the background color of the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBBgColor(const GenBrush* const that);

// Get a copy of the background color of the GBSurface of the 
// GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel GBGetBgColor(const GenBrush* const that);

// Set the background color of the GBSurface of the GenBrush
//  'that' to 'col'
#if BUILDMODE != 0
inline
#endif 
void GBSetBgColor(GenBrush* const that, const GBPixel* const col);

// Get the filename of the GBSurfaceImage of the GenBrush 'that'
// Return NULL if the surface is not a GBSurfaceImage
#if BUILDMODE != 0
inline
#endif 
char* GBFileName(const GenBrush* const that);

// Set the filename of the GBSurfaceImage of the GenBrush 'that' 
// to 'fileName'
// Do nothing if the surface is not a GBSurfaceImage
#if BUILDMODE != 0
inline
#endif 
void GBSetFileName(GenBrush* const that, const char* const fileName);

// Update the GBSurface of the GenBrush 'that'
void GBUpdate(GenBrush* const that);

// Render the GBSurface (save on disk, display on screen, ...) of
// the GenBrush 'that'
bool GBRender(GenBrush* const that);

// Get the area of the GBSurface of the Genbrush 'that'
#if BUILDMODE != 0
inline
#endif 
int GBArea(const GenBrush* const that);

// Return true if the position 'pos' is inside the GBSurface of the 
// GenBrush 'that' 
// boundary, false else
#if BUILDMODE != 0
inline
#endif 
bool GBIsPosInside(const GenBrush* const that, 
  const VecShort2D* const pos);

// Return the layers of the surface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayers(const GenBrush* const that);

// Add a new GBLayer with dimensions 'dim' to the top of the stack 
// of layers of the GBSurface of the GenBrush 'that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBAddLayer(GenBrush* const that, const VecShort2D* const dim);

// Add a new GBLayer with content equals to the image located at 
// 'fileName' to the top of the stack 
// of layers of the GBSurface of the GenBrush' that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBAddLayerFromFile(GenBrush* const that, 
  const char* const fileName);

// Get the 'iLayer'-th layer of the GBSurface of the GenBrush 'that'
// 'iLayer' must be valid
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBLay(const GenBrush* const that, const int iLayer);

// Get the number of layer of the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
int GBGetNbLayer(const GenBrush* const that);

// Set the stack position of the GBLayer 'layer' into the set of 
// layers of the GBSurface of the GenBrush'that' to 'pos'
// If 'layer' can't be found in the surface do nothing
// 'pos' must be valid (0<='pos'<nbLayers)
#if BUILDMODE != 0
inline
#endif 
void GBSetLayerStackPos(GenBrush* const that, 
  GBLayer* const layer, const int pos);

// Remove the GBLayer 'layer' from the set of layers of the 
// GBSurface of the GenBrush 'that'
// The memory used by 'layer' is freed
#if BUILDMODE != 0
inline
#endif 
void GBRemoveLayer(GenBrush* const that, GBLayer* layer);

// Get the number of objects in the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
int GBGetNbPod(const GenBrush* const that);

// Add a GBObjPod for the Point at position 'pos' to the GenBrush 'that'
// drawn with 'eye', 'hand' and 'tool' in layer 'layer'
// 'pos' must be a vector of 2 or more dimensions
#if BUILDMODE != 0
inline
#endif 
void _GBAddPoint(GenBrush* const that, VecFloat* const pos, 
  GBEye* const eye, GBHand* const hand, GBTool* const tool, 
  GBInk* const ink, GBLayer* const layer);

// Add a GBObjPod for the Shapoid 'shap' to the GenBrush 'that'
// drawn with 'eye', 'hand' and 'tool' in layer 'layer'
// 'shap' 's dimension must be 2 or more
#if BUILDMODE != 0
inline
#endif 
void _GBAddShapoid(GenBrush* const that, Shapoid* const shap, 
  GBEye* const eye, GBHand* const hand, GBTool* const tool, 
  GBInk* const ink, GBLayer* const layer);

// Add a GBObjPod for the SCurve 'curve' to the GenBrush 'that'
// drawn with 'eye', 'hand' and 'tool' in layer 'layer'
// 'curve' 's dimension must be 2 or more
#if BUILDMODE != 0
inline
#endif 
void _GBAddSCurve(GenBrush* const that, SCurve* const curve, 
  GBEye* const eye, GBHand* const hand, GBTool* const tool, 
  GBInk* const ink, GBLayer* const layer);

// Remove from the list of pods of the GenBrush 'that' those who 
// match the 'obj', 'eye', 'hand', 'tool', 'ink' and 'layer'
// Null arguments are ignored. For example:
// GBRemovePod(that, elemA, NULL, NULL, NULL, NULL, NULL) removes all 
// the pods related to the object 'elemA'
// GBRemovePod(that, elemA, NULL, handA, NULL, NULL, NULL) removes 
// all the pods related to both the object 'elemA' AND 'handA'
// If all the filters are null it removes all the pods
void _GBRemovePod(GenBrush* const that, const void* const obj, 
  const GBEye* const eye, const GBHand* const hand, 
  const GBTool* const tool, const GBInk* const ink, 
  const GBLayer* const layer);
// TODO: the filtering arguments should be in a struct

// Set the eye to 'toEye' in the pods of the GenBrush 'that' who 
// match the 'obj', 'eye', 'hand', 'tool', 'ink' and 'layer'
// Null arguments are ignored. For example:
// GBSetPodEye(that, toEye, elemA, NULL, NULL, NULL, NULL, NULL) 
// affects all the pods related to the object 'elemA'
// GBSetPodEye(that, toEye, elemA, NULL, handA, NULL, NULL, NULL) 
// affects all the pods related to both the object 'elemA' AND 'handA'
// If all the filters are null it affects all the pods
// If 'toEye' is null, do nothing
void _GBSetPodEye(GenBrush* const that, GBEye* const toEye, 
  const void* const obj, const GBEye* const eye, 
  const GBHand* const hand, const GBTool* const tool, 
  const GBInk* const ink, const GBLayer* const layer);
// TODO: the filtering arguments should be in a struct

// Set the hand to 'toHand' in the pods of the GenBrush 'that' who 
// match the 'obj', 'eye', 'hand', 'tool', 'ink' and 'layer'
// Null arguments are ignored. For example:
// GBSetPodEye(that, toHand, elemA, NULL, NULL, NULL, NULL, NULL) 
// affects all the pods related to the object 'elemA'
// GBSetPodEye(that, toHand, elemA, NULL, handA, NULL, NULL, NULL) 
// affects all the pods related to both the object 'elemA' AND 'handA'
// If all the filters are null it affects all the pods
// If 'toHand' is null, do nothing
void _GBSetPodHand(GenBrush* const that, GBHand* const toHand, 
  const void* const obj, const GBEye* const eye, 
  const GBHand* const hand, const GBTool* const tool, 
  const GBInk* const ink, const GBLayer* const layer);
// TODO: the filtering arguments should be in a struct

// Set the tool to 'toTool' in the pods of the GenBrush 'that' who 
// match the 'obj', 'eye', 'hand', 'tool', 'ink' and 'layer'
// Null arguments are ignored. For example:
// GBSetPodTool(that, toTool, elemA, NULL, NULL, NULL, NULL) affects 
// all the pods related to the object 'elemA'
// GBSetPodTool(that, toTool, elemA, NULL, handA, NULL, NULL) affects 
// all the pods related to both the object 'elemA' AND 'handA'
// If all the filters are null it affects all the pods
// If 'toTool' is null, do nothing
void _GBSetPodTool(GenBrush* const that, GBTool* const toTool, 
  const void* const obj, const GBEye* const eye, 
  const GBHand* const hand, const GBTool* const tool, 
  const GBInk* const ink, const GBLayer* const layer);
// TODO: the filtering arguments should be in a struct

// Set the ink to 'toInk' in the pods of the GenBrush 'that' who 
// match the 'obj', 'eye', 'hand', 'tool', 'ink' and 'layer'
// Null arguments are ignored. For example:
// GBSetPodTool(that, toTool, elemA, NULL, NULL, NULL, NULL) affects 
// all the pods related to the object 'elemA'
// GBSetPodTool(that, toTool, elemA, NULL, handA, NULL, NULL) affects 
// all the pods related to both the object 'elemA' AND 'handA'
// If all the filters are null it affects all the pods
// If 'toInk' is null, do nothing
void _GBSetPodInk(GenBrush* const that, GBInk* const toInk, 
  const void* const obj, const GBEye* const eye, 
  const GBHand* const hand, const GBTool* const tool, 
  const GBInk* const ink, const GBLayer* const layer);
// TODO: the filtering arguments should be in a struct

// Set the layer to 'toLayer' in the pods of the GenBrush 'that' who 
// match the 'obj', 'eye', 'hand', 'tool', 'ink' and 'layer'
// Null arguments are ignored. For example:
// GBSetPodLayer(that, toLayer, elemA, NULL, NULL, NULL, NULL, NULL) 
// affects all the pods related to the object 'elemA'
// GBSetPodLayer(that, toLayer, elemA, NULL, handA, NULL, NULL, NULL) 
// affects all the pods related to both the object 'elemA' AND 'handA'
// If all the filters are null it affects all the pods
// If 'toLayer' is null, do nothing
void _GBSetPodLayer(GenBrush* const that, GBLayer* const toLayer, 
  const void* const obj, const GBEye* const eye, 
  const GBHand* const hand, const GBTool* const tool, 
  const GBInk* const ink, const GBLayer* const layer);
// TODO: the filtering arguments should be in a struct

// Reset all the final pix of the surface of the GenBrush 'that' to its 
// background color, and reset all the modified flag of layers to true
#if BUILDMODE != 0
inline
#endif 
void GBFlush(GenBrush* const that);

// Set to true the modified flag of the layers of pods attached to the
// object 'obj' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromObj(GenBrush* const that, const void* const obj);

// Set to true the modified flag of the layers of pods attached to the
// GBEye 'eye' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromEye(GenBrush* const that, 
  const GBEye* const eye);

// Set to true the modified flag of the layers of pods attached to the
// GBInk 'ink' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromInk(GenBrush* const that, 
  const GBInk* const ink);

// Set to true the modified flag of the layers of pods attached to the
// GBHand 'hand' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromHand(GenBrush* const that, 
  const GBHand* const hand);

// Set to true the modified flag of the layers of pods attached to the
// GBTool 'tool' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromTool(GenBrush* const that, 
  const GBTool* const tool);

// Return true if the surface of the GenBrush 'that' is same as the
// surface of the GenBrush 'gb'
// Else, return false
#if BUILDMODE != 0
inline
#endif 
bool GBIsSameAs(const GenBrush* const that, const GenBrush* const gb);

// Add a GBPostProcessing of type 'type' to the GenBrush 'that'
// Return the GBPostProcessing
#if BUILDMODE != 0
inline
#endif 
GBPostProcessing* GBAddPostProcess(GenBrush* const that, 
  const GBPPType type);

// Remove the GBPostProcessing 'post' from the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
void GBRemovePostProcess(GenBrush* const that, 
  const GBPostProcessing* const post);

// Remove all the GBPostProcessing from the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
void GBRemoveAllPostProcess(GenBrush* const that);

// Get the 'iPost'-th post process of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBPostProcessing* GBPostProcess(const GenBrush* const that, 
  const int iPost);

// Get the GSet of GBPostProcessing of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBPostProcs(const GenBrush* const that);

// Get the number of GBPostProcessing of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
int GBGetNbPostProcs(const GenBrush* const that);

// Remove all the GBObjPods from the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
void GBRemoveAllPod(GenBrush* const that);

// Return a clone of the GenBrush 'that' with its final surface scaled
// to the dimensions 'dim' according to the scaling method 'scaleMethod'
GenBrush* GBScale(const GenBrush* const that, 
  const VecShort2D* const dim, const GBScaleMethod scaleMethod);

// Return a clone of the GenBrush 'that' with its final surface cropped
// to the dimensions 'dim' from the lower right position 'posLR'
// If the cropping area is partially or totally outside of the 
// original image, pixels outside of the image are filled with 
// 'fillPix' or left to their default value (cf GBSurfaceCreate) if
// 'fillPix is NULL
GenBrush* GBCrop(const GenBrush* const that, 
  const VecShort2D* const posLR, const VecShort2D* const dim,
  const GBPixel* const fillPix);

#if BUILDWITHGRAPHICLIB == 1
#include "genbrush-GTK.h"
#endif

// ================= Polymorphism ==================

#define GBPosIndex(Pos, Dim) \
  (VecGet(Dim, 0) * VecGet(Pos, 1) + VecGet(Pos, 0))

#define GBInkGetType(Ink) _Generic(Ink, \
  GBInk*: _GBInkGetType, \
  GBInkSolid*: _GBInkGetType, \
  const GBInk*: _GBInkGetType, \
  const GBInkSolid*: _GBInkGetType, \
  default: PBErrInvalidPolymorphism) ((GBInk*)(Ink))

#define GBInkGet(Ink, Tool, Pod, PosInternal, PosExternal, PosLayer) \
  _GBInkGet((GBInk*)(Ink), (GBTool*)(Tool), Pod, \
  (VecFloat*)(PosInternal), (VecFloat*)(PosExternal), \
  (VecShort*)(PosLayer))
  
#define GBObjPodCreatePoint(Pos, Eye, Hand, Tool, Ink, Layer) \
  _Generic(Pos, \
  VecFloat*: _GBObjPodCreatePoint, \
  VecFloat2D*: _GBObjPodCreatePoint, \
  VecFloat3D*: _GBObjPodCreatePoint, \
  const VecFloat*: _GBObjPodCreatePoint, \
  const VecFloat2D*: _GBObjPodCreatePoint, \
  const VecFloat3D*: _GBObjPodCreatePoint, \
  default: PBErrInvalidPolymorphism) ((VecFloat*)(Pos), \
  (GBEye*)(Eye), (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)
  
#define GBObjPodCreateShapoid(Shap, Eye, Hand, Tool, Ink, Layer) \
  _Generic(Shap, \
  Shapoid*: _GBObjPodCreateShapoid, \
  Facoid*: _GBObjPodCreateShapoid, \
  Spheroid*: _GBObjPodCreateShapoid, \
  Pyramidoid*: _GBObjPodCreateShapoid, \
  const Shapoid*: _GBObjPodCreateShapoid, \
  const Facoid*: _GBObjPodCreateShapoid, \
  const Spheroid*: _GBObjPodCreateShapoid, \
  const Pyramidoid*: _GBObjPodCreateShapoid, \
  default: PBErrInvalidPolymorphism) ((Shapoid*)(Shap), \
  (GBEye*)(Eye), (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBObjPodCreateSCurve(Curve, Eye, Hand, Tool, Ink, Layer) \
  _GBObjPodCreateSCurve(Curve, \
  (GBEye*)(Eye), (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBAddPoint(GB, Pos, Eye, Hand, Tool, Ink, Layer) \
  _Generic(Pos, \
  VecFloat*: _GBAddPoint, \
  VecFloat2D*: _GBAddPoint, \
  VecFloat3D*: _GBAddPoint, \
  const VecFloat*: _GBAddPoint, \
  const VecFloat2D*: _GBAddPoint, \
  const VecFloat3D*: _GBAddPoint, \
  default: PBErrInvalidPolymorphism) (GB, (VecFloat*)(Pos), \
  (GBEye*)(Eye), (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)
  
#define GBAddShapoid(GB, Shap, Eye, Hand, Tool, Ink, Layer) \
  _Generic(Shap, \
  Shapoid*: _GBAddShapoid, \
  Facoid*: _GBAddShapoid, \
  Spheroid*: _GBAddShapoid, \
  Pyramidoid*: _GBAddShapoid, \
  const Shapoid*: _GBAddShapoid, \
  const Facoid*: _GBAddShapoid, \
  const Spheroid*: _GBAddShapoid, \
  const Pyramidoid*: _GBAddShapoid, \
  default: PBErrInvalidPolymorphism) (GB, (Shapoid*)(Shap), \
  (GBEye*)(Eye), (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBAddSCurve(GB, Curve, Eye, Hand, Tool, Ink, Layer) \
  _GBAddSCurve(GB, Curve, \
  (GBEye*)(Eye), (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBRemovePod(GB, Obj, Eye, Hand, Tool, Ink, Layer) \
  _GBRemovePod(GB, (void*)(Obj), (GBEye*)(Eye), (GBHand*)(Hand), \
  (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBObjPodGetObjAsPoint(Pod) (VecFloat*)GBObjPodObj(Pod)
#define GBObjPodGetObjAsShapoid(Pod) (Shapoid*)GBObjPodObj(Pod)
#define GBObjPodGetObjAsSCurve(Pod) (SCurve*)GBObjPodObj(Pod)

#define GBObjPodGetEyeObjAsPoint(Pod) (VecFloat*)GBObjPodEyeObj(Pod)
#define GBObjPodGetEyeObjAsShapoid(Pod) (Shapoid*)GBObjPodEyeObj(Pod)
#define GBObjPodGetEyeObjAsSCurve(Pod) (SCurve*)GBObjPodEyeObj(Pod)

#define GBObjPodSetEyePoint(Pod, Point) \
  _GBObjPodSetEyePoint(Pod, (VecFloat*)Point)
#define GBObjPodSetEyeShapoid(Pod, Shap) \
  _GBObjPodSetEyeShapoid(Pod, (Shapoid*)Shap)
#define GBObjPodSetEyeSCurve(Pod, Curve) \
  _GBObjPodSetEyeSCurve(Pod, (SCurve*)Curve)

#define GBSetPodEye(GB, ToEye, Obj, Eye, Hand, Tool, Ink, Layer) \
  _GBSetPodEye(GB, (GBEye*)(ToEye), (void*)(Obj), (GBEye*)(Eye), \
  (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBSetPodHand(GB, ToHand, Obj, Eye, Hand, Tool, Ink, Layer) \
  _GBSetPodHand(GB, (GBHand*)(ToHand), (void*)(Obj), (GBEye*)(Eye), \
  (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBSetPodTool(GB, ToTool, Obj, Eye, Hand, Tool, Ink, Layer) \
  _GBSetPodTool(GB, (GBTool*)(ToTool), (void*)(Obj), (GBEye*)(Eye), \
  (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBSetPodInk(GB, ToInk, Obj, Eye, Hand, Tool, Ink, Layer) \
  _GBSetPodInk(GB, (GBInk*)(ToInk), (void*)(Obj), (GBEye*)(Eye), \
  (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBSetPodLayer(GB, ToLayer, Obj, Eye, Hand, Tool, Ink, Layer) \
  _GBSetPodLayer(GB, ToLayer, (void*)(Obj), (GBEye*)(Eye), \
  (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBNotifyChangeFromObj(GB, Obj) \
  _GBNotifyChangeFromObj(GB, (void*)(Obj))

#define GBNotifyChangeFromEye(GB, Eye) \
  _GBNotifyChangeFromEye(GB, (GBEye*)(Eye))

#define GBNotifyChangeFromHand(GB, Hand) \
  _GBNotifyChangeFromHand(GB, (GBHand*)(Hand))

#define GBNotifyChangeFromTool(GB, Tool) \
  _GBNotifyChangeFromTool(GB, (GBTool*)(Tool))

#define GBNotifyChangeFromInk(GB, Ink) \
  _GBNotifyChangeFromInk(GB, (GBInk*)(Ink))

#define GBHandGetType(Hand) _Generic(Hand, \
  GBHand*: _GBHandGetType, \
  GBHandDefault*: _GBHandGetType, \
  const GBHand*: _GBHandGetType, \
  const GBHandDefault*: _GBHandGetType, \
  default: PBErrInvalidPolymorphism) ((GBHand*)(Hand))

#define GBHandProcess(Hand, Pod) _Generic(Hand, \
  GBHand*: _GBHandProcess, \
  GBHandDefault*: GBHandDefaultProcess, \
  const GBHand*: _GBHandProcess, \
  const GBHandDefault*: GBHandDefaultProcess, \
  default: PBErrInvalidPolymorphism) (Hand, Pod)

#define GBObjPodSetHand(Pod, Hand) \
  _GBObjPodSetHand(Pod, (GBHand*)(Hand))
  
#define GBObjPodSetEye(Pod, Eye) \
  _GBObjPodSetEye(Pod, (GBEye*)(Eye))
  
#define GBObjPodSetTool(Pod, Tool) \
  _GBObjPodSetTool(Pod, (GBTool*)(Tool))
  
#define GBObjPodSetInk(Pod, Ink) \
  _GBObjPodSetInk(Pod, (GBInk*)(Ink))

#define GBEyeGetType(Eye) _Generic(Eye, \
  GBEye*: _GBEyeGetType, \
  GBEyeOrtho*: _GBEyeGetType, \
  GBEyeIsometric*: _GBEyeGetType, \
  const GBEye*: _GBEyeGetType, \
  const GBEyeOrtho*: _GBEyeGetType, \
  const GBEyeIsometric*: _GBEyeGetType, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeScale(Eye) _Generic(Eye, \
  GBEye*: _GBEyeScale, \
  GBEyeOrtho*: _GBEyeScale, \
  GBEyeIsometric*: _GBEyeScale, \
  const GBEye*: _GBEyeScale, \
  const GBEyeOrtho*: _GBEyeScale, \
  const GBEyeIsometric*: _GBEyeScale, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeGetScale(Eye) _Generic(Eye, \
  GBEye*: _GBEyeGetScale, \
  GBEyeOrtho*: _GBEyeGetScale, \
  GBEyeIsometric*: _GBEyeGetScale, \
  const GBEye*: _GBEyeGetScale, \
  const GBEyeOrtho*: _GBEyeGetScale, \
  const GBEyeIsometric*: _GBEyeGetScale, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeSetScale(Eye, Scale) _Generic(Scale, \
  float: GBEyeSetScaleFloat, \
  VecFloat3D*: GBEyeSetScaleVec, \
  const float: GBEyeSetScaleFloat, \
  const VecFloat3D*: GBEyeSetScaleVec, \
  default: PBErrInvalidPolymorphism)( \
    _Generic(Eye, \
      GBEye*: Eye, \
      GBEyeOrtho*: (GBEye*)Eye, \
      GBEyeIsometric*: (GBEye*)Eye, \
      const GBEye*: Eye, \
      const GBEyeOrtho*: (GBEye*)Eye, \
      const GBEyeIsometric*: (GBEye*)Eye, \
      default: Eye), \
    _Generic(Scale, \
      float: Scale, \
      VecFloat3D*: Scale, \
      const float: Scale, \
      const VecFloat3D*: Scale, \
      default: Scale))

#define GBEyeOrig(Eye) _Generic(Eye, \
  GBEye*: _GBEyeOrig, \
  GBEyeOrtho*: _GBEyeOrig, \
  GBEyeIsometric*: _GBEyeOrig, \
  const GBEye*: _GBEyeOrig, \
  const GBEyeOrtho*: _GBEyeOrig, \
  const GBEyeIsometric*: _GBEyeOrig, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeGetOrig(Eye) _Generic(Eye, \
  GBEye*: _GBEyeGetOrig, \
  GBEyeOrtho*: _GBEyeGetOrig, \
  GBEyeIsometric*: _GBEyeGetOrig, \
  const GBEye*: _GBEyeGetOrig, \
  const GBEyeOrtho*: _GBEyeGetOrig, \
  const GBEyeIsometric*: _GBEyeGetOrig, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeSetOrig(Eye, Orig) _Generic(Eye, \
  GBEye*: _GBEyeSetOrig, \
  GBEyeOrtho*: _GBEyeSetOrig, \
  GBEyeIsometric*: _GBEyeSetOrig, \
  const GBEye*: _GBEyeSetOrig, \
  const GBEyeOrtho*: _GBEyeSetOrig, \
  const GBEyeIsometric*: _GBEyeSetOrig, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye), Orig)

#define GBEyeGetRot(Eye) _Generic(Eye, \
  GBEye*: _GBEyeGetRot, \
  GBEyeOrtho*: _GBEyeGetRot, \
  GBEyeIsometric*: _GBEyeGetRot, \
  const GBEye*: _GBEyeGetRot, \
  const GBEyeOrtho*: _GBEyeGetRot, \
  const GBEyeIsometric*: _GBEyeGetRot, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeSetRot(Eye, Theta) _Generic(Eye, \
  GBEye*: _GBEyeSetRot, \
  GBEyeOrtho*: _GBEyeSetRot, \
  GBEyeIsometric*: _GBEyeSetRot, \
  const GBEye*: _GBEyeSetRot, \
  const GBEyeOrtho*: _GBEyeSetRot, \
  const GBEyeIsometric*: _GBEyeSetRot, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye), Theta)

#define GBEyeProj(Eye) _Generic(Eye, \
  GBEye*: _GBEyeProj, \
  GBEyeOrtho*: _GBEyeProj, \
  GBEyeIsometric*: _GBEyeProj, \
  const GBEye*: _GBEyeProj, \
  const GBEyeOrtho*: _GBEyeProj, \
  const GBEyeIsometric*: _GBEyeProj, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeProcess(Eye, Pod) _Generic(Eye, \
  GBEye*: _GBEyeProcess, \
  GBEyeOrtho*: _GBEyeProcess, \
  GBEyeIsometric*: _GBEyeProcess, \
  const GBEye*: _GBEyeProcess, \
  const GBEyeOrtho*: _GBEyeProcess, \
  const GBEyeIsometric*: _GBEyeProcess, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye), Pod)

#define GBEyeFree(EyeRef) _Generic(EyeRef, \
  GBEyeOrtho**: GBEyeOrthoFree, \
  GBEyeIsometric**: GBEyeIsometricFree, \
  default: PBErrInvalidPolymorphism)(EyeRef)

#define GBToolDraw(Tool, Pod) _Generic(Tool, \
  GBTool*: _GBToolDraw, \
  GBToolPlotter*: GBToolPlotterDraw, \
  const GBTool*: _GBToolDraw, \
  const GBToolPlotter*: GBToolPlotterDraw, \
  default: PBErrInvalidPolymorphism) (Tool, Pod)
  
#if BUILDWITHGRAPHICLIB == 0


#elif BUILDWITHGRAPHICLIB == 1


#endif

// ================ Inliner ====================

#if BUILDMODE != 0
#include "genbrush-inline.c"
#endif


#endif
