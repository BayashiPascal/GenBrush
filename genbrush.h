// ============ GENBRUSH.H ================

#ifndef GENBRUSH_H
#define GENBRUSH_H

// About the coordinates systems:
// In input, the coordinates system must be a left handed
// coordinates system, with x toward the right, y toward the top and 
// z toward the rear.
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
// right, the first row is the bottom row in the surface.

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
    
// ================= Polymorphism ==================

#define GBPosIndex(Pos, Dim) \
  (VecGet(Dim, 0) * VecGet(Pos, 1) + VecGet(Pos, 0))

#define GBInkGetType(Ink) _Generic(Ink, \
  GBInk*: _GBInkGetType, \
  GBInkSolid*: _GBInkGetType, \
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
  default: PBErrInvalidPolymorphism) ((VecFloat*)(Pos), \
  (GBEye*)(Eye), (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)
  
#define GBObjPodCreateShapoid(Shap, Eye, Hand, Tool, Ink, Layer) \
  _Generic(Shap, \
  Shapoid*: _GBObjPodCreateShapoid, \
  Facoid*: _GBObjPodCreateShapoid, \
  Spheroid*: _GBObjPodCreateShapoid, \
  Pyramidoid*: _GBObjPodCreateShapoid, \
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
  default: PBErrInvalidPolymorphism) (GB, (VecFloat*)(Pos), \
  (GBEye*)(Eye), (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)
  
#define GBAddShapoid(GB, Shap, Eye, Hand, Tool, Ink, Layer) \
  _Generic(Shap, \
  Shapoid*: _GBAddShapoid, \
  Facoid*: _GBAddShapoid, \
  Spheroid*: _GBAddShapoid, \
  Pyramidoid*: _GBAddShapoid, \
  default: PBErrInvalidPolymorphism) (GB, (Shapoid*)(Shap), \
  (GBEye*)(Eye), (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBAddSCurve(GB, Curve, Eye, Hand, Tool, Ink, Layer) \
  _GBAddSCurve(GB, Curve, \
  (GBEye*)(Eye), (GBHand*)(Hand), (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBRemovePod(GB, Obj, Eye, Hand, Tool, Ink, Layer) \
  _GBRemovePod(GB, (void*)(Obj), (GBEye*)(Eye), (GBHand*)(Hand), \
  (GBTool*)(Tool), (GBInk*)(Ink), Layer)

#define GBObjPodGetObjAsPoint(Pod) (VecFloat*)GBObjPodGetObj(Pod)
#define GBObjPodGetObjAsShapoid(Pod) (Shapoid*)GBObjPodGetObj(Pod)
#define GBObjPodGetObjAsSCurve(Pod) (SCurve*)GBObjPodGetObj(Pod)

#define GBObjPodGetEyeObjAsPoint(Pod) (VecFloat*)GBObjPodGetEyeObj(Pod)
#define GBObjPodGetEyeObjAsShapoid(Pod) (Shapoid*)GBObjPodGetEyeObj(Pod)
#define GBObjPodGetEyeObjAsSCurve(Pod) (SCurve*)GBObjPodGetEyeObj(Pod)

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
  default: PBErrInvalidPolymorphism) ((GBHand*)(Hand))

#define GBHandProcess(Hand, Pod) _Generic(Hand, \
  GBHand*: _GBHandProcess, \
  GBHandDefault*: GBHandDefaultProcess, \
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
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeScale(Eye) _Generic(Eye, \
  GBEye*: _GBEyeScale, \
  GBEyeOrtho*: _GBEyeScale, \
  GBEyeIsometric*: _GBEyeScale, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeGetScale(Eye) _Generic(Eye, \
  GBEye*: _GBEyeGetScale, \
  GBEyeOrtho*: _GBEyeGetScale, \
  GBEyeIsometric*: _GBEyeGetScale, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeSetScale(Eye, Scale) _Generic(Scale, \
  float: GBEyeSetScaleFloat, \
  VecFloat3D*: GBEyeSetScaleVec, \
  default: PBErrInvalidPolymorphism)( \
    _Generic(Eye, \
      GBEye*: Eye, \
      GBEyeOrtho*: (GBEye*)Eye, \
      GBEyeIsometric*: (GBEye*)Eye, \
      default: Eye), \
    _Generic(Scale, \
      float: Scale, \
      VecFloat3D*: Scale, \
      default: Scale))

#define GBEyeOrig(Eye) _Generic(Eye, \
  GBEye*: _GBEyeOrig, \
  GBEyeOrtho*: _GBEyeOrig, \
  GBEyeIsometric*: _GBEyeOrig, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeGetOrig(Eye) _Generic(Eye, \
  GBEye*: _GBEyeGetOrig, \
  GBEyeOrtho*: _GBEyeGetOrig, \
  GBEyeIsometric*: _GBEyeGetOrig, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeSetOrig(Eye, Orig) _Generic(Eye, \
  GBEye*: _GBEyeSetOrig, \
  GBEyeOrtho*: _GBEyeSetOrig, \
  GBEyeIsometric*: _GBEyeSetOrig, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye), Orig)

#define GBEyeGetRot(Eye) _Generic(Eye, \
  GBEye*: _GBEyeGetRot, \
  GBEyeOrtho*: _GBEyeGetRot, \
  GBEyeIsometric*: _GBEyeGetRot, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeSetRot(Eye, Theta) _Generic(Eye, \
  GBEye*: _GBEyeSetRot, \
  GBEyeOrtho*: _GBEyeSetRot, \
  GBEyeIsometric*: _GBEyeSetRot, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye), Theta)

#define GBEyeProj(Eye) _Generic(Eye, \
  GBEye*: _GBEyeProj, \
  GBEyeOrtho*: _GBEyeProj, \
  GBEyeIsometric*: _GBEyeProj, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye))

#define GBEyeProcess(Eye, Pod) _Generic(Eye, \
  GBEye*: _GBEyeProcess, \
  GBEyeOrtho*: _GBEyeProcess, \
  GBEyeIsometric*: _GBEyeProcess, \
  default: PBErrInvalidPolymorphism)((GBEye*)(Eye), Pod)

#define GBEyeFree(EyeRef) _Generic(EyeRef, \
  GBEyeOrtho**: GBEyeOrthoFree, \
  GBEyeIsometric**: GBEyeIsometricFree, \
  default: PBErrInvalidPolymorphism)(EyeRef)

#define GBToolDraw(Tool, Pod) _Generic(Tool, \
  GBTool*: _GBToolDraw, \
  GBToolPlotter*: GBToolPlotterDraw, \
  default: PBErrInvalidPolymorphism) (Tool, Pod)
  
#if BUILDWITHGRAPHICLIB == 0


#elif BUILDWITHGRAPHICLIB == 1


#endif

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

typedef struct GenBrush {
  // Surface of the GenBrush
  GBSurface* _surf;
  // Set of GBObjPod to be drawn
  GSet _pods;
} GenBrush;

// ================ Functions declaration ====================

// ---------------- GBPixel --------------------------

// Blend the pixel 'pix' into the pixel 'that'
// BlendNormal mixes colors according to their relative alpha value
// and add the alpha values
void GBPixelBlendNormal(GBPixel* that, GBPixel* pix);

// Blend the pixel 'pix' into the pixel 'that'
// BlendOver mixes colors according to the alpha value of 'pix'
// and add the alpha values
void GBPixelBlendOver(GBPixel* that, GBPixel* pix);

// Return the blend result of the stack of Pixel 'stack'
// If there is transparency down to the bottom of the stack, use the 
// background color 'bgColor'
// If the stack is empty, return a transparent pixel
GBPixel GBPixelStackBlend(GSet* stack, GBPixel* bgColor);

// ---------------- GBLayer --------------------------

// Create a new GBLayer with dimensions 'dim'
// The layer is initialized with empty stacks of pixel
// _pos = (0,0)
// blendMode = GBLayerBlendModeDefault
// stackPos = GBLayerStackPosBg
GBLayer* GBLayerCreate(VecShort2D* dim);

// Free the GBLayer 'that'
void GBLayerFree(GBLayer** that);

// Get the area of the layer (width * height)
#if BUILDMODE != 0
inline
#endif 
int GBLayerArea(GBLayer* that);

// Get the position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBLayerPos(GBLayer* that);

// Get a copy of the position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBLayerGetPos(GBLayer* that);

// Set the position of the GBLayer 'that' to 'pos'
// If the flag _modified==false _prevPos is first set to _pos
// and _modified is set to true
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetPos(GBLayer* that, VecShort2D* pos);

// Get the previous position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBLayerPrevPos(GBLayer* that);

// Get a copy of the previous position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBLayerGetPrevPos(GBLayer* that);

// Get the dimensions of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBLayerDim(GBLayer* that);

// Get a copy of the dimensions of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBLayerGetDim(GBLayer* that);

// Get a copy of the blend mode of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
GBLayerBlendMode GBLayerGetBlendMode(GBLayer* that);

// Set the blend mode of the GBLayer 'that' to 'blend'
// Set the flag _modified to true
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetBlendMode(GBLayer* that, GBLayerBlendMode blend);

// Get a copy of the modified flag of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
bool GBLayerIsModified(GBLayer* that);

// Set the modified flag of the GBLayer 'that' to 'flag'
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetModified(GBLayer* that, bool flag);

// Get a copy of the stack position of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
GBLayerStackPosition GBLayerGetStackPos(GBLayer* that);

// Set the stack position of the GBLayer 'that' to 'pos'
// Set the flag _modified to true
#if BUILDMODE != 0
inline
#endif 
void GBLayerSetStackPos(GBLayer* that, GBLayerStackPosition pos);

// Get the stacked pixels of the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayerPixels(GBLayer* that);

// Get the stacked pixels of the GBLayer 'that' at position 'pos'
// 'pos' must be inside the layer
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayerPixel(GBLayer* that, VecShort2D* pos);

// Get the stacked pixels of the GBLayer 'that' at position 'pos'
// If 'pos' is out of the layer return NULL
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayerPixelSafe(GBLayer* that, VecShort2D* pos);

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
  float depth);

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
  float depth);

// Return true if the position 'pos' is inside the layer 'that' 
// boundary, false else
#if BUILDMODE != 0
inline
#endif 
bool GBLayerIsPosInside(GBLayer* that, VecShort2D *pos);

// Delete all the stacked pixels in the GBLayer 'that'
#if BUILDMODE != 0
inline
#endif 
void GBLayerFlush(GBLayer* that);

// Create a new GBLayer with dimensions and content given by the 
// image on disk at location 'fileName'
// Return NULL if we couldn't create the layer
GBLayer* GBLayerCreateFromFile(char* fileName);

// Get the boundary of the GBLayer 'that' inside the GBSurface 'surf'
// The boundaries are given as a Facoid
// If the flag 'prevPos' is true, gives the bounday at the previous
// position
// Return NULL if the layer is completely out of the surface
Facoid* GBLayerGetBoundaryInSurface(GBLayer* that, GBSurface* surf,
  bool prevPos);

// ---------------- GBSurface --------------------------

// Create a new static GBSurface with dimension 'dim' and type 'type'
// _finalPix is set to 0
// _bgColor is set to white
GBSurface GBSurfaceCreateStatic(GBSurfaceType type, VecShort2D* dim);

// Create a static GBSurface with dimension 'dim' and type 'type'
// _finalPix is set to 0
// _bgColor is set to white
GBSurface* GBSurfaceCreate(GBSurfaceType type, VecShort2D* dim);

// Free the memory used by the GBSurface 'that'
void GBSurfaceFree(GBSurface** that);

// Free the memory used by the properties of the GBSurface 'that'
void GBSurfaceFreeStatic(GBSurface* that);

// Clone the GBSurface 'that'
GBSurface GBSurfaceClone(GBSurface* that);

// Get the type of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBSurfaceType GBSurfaceGetType(GBSurface* that);

// Get a copy of the dimensions of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBSurfaceGetDim(GBSurface* that);

// Get the dimensions of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBSurfaceDim(GBSurface* that);

// Get the final pixels of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceFinalPixels(GBSurface* that);

// Get the final pixel at position 'pos' of the GBSurface 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceFinalPixel(GBSurface* that, VecShort2D* pos);

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel GBSurfaceGetFinalPixel(GBSurface* that, VecShort2D* pos);

// Set the final pixel at position 'pos' of the GBSurface 'that' to
// the pixel 'pix'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetFinalPixel(GBSurface* that, VecShort2D* pos, 
  GBPixel* pix);

// Get the final pixel at position 'pos' of the GBSurface 'that'
// If 'pos' is out of the surface return NULL
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceFinalPixelSafe(GBSurface* that, VecShort2D* pos);

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// 'that'
// If 'pos' is out of the surface return a transparent pixel
#if BUILDMODE != 0
inline
#endif 
GBPixel GBSurfaceGetFinalPixelSafe(GBSurface* that, VecShort2D* pos);

// Set the final pixel at position 'pos' of the GBSurface 'that' to
// the pixel 'pix'
// If 'pos' is out of the surface do nothing
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetFinalPixelSafe(GBSurface* that, VecShort2D* pos, 
  GBPixel* pix);

// Get the area of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
int GBSurfaceArea(GBSurface* that);

// Get the background color of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBSurfaceBgColor(GBSurface* that);

// Get a copy of the background color of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel GBSurfaceGetBgColor(GBSurface* that);

// Set the background color of the GBSurface 'that' to 'col'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetBgColor(GBSurface* that, GBPixel* col);

// Return true if the position 'pos' is inside the GBSurface 'that' 
// boundary, false else
#if BUILDMODE != 0
inline
#endif 
bool GBSurfaceIsPosInside(GBSurface* that, VecShort2D *pos);

// Get the set of layers of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBSurfaceLayers(GBSurface* that);

// Add a new GBLayer with dimensions 'dim' to the top of the stack 
// of layers of the GBSurface 'that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBSurfaceAddLayer(GBSurface* that, VecShort2D* dim);

// Add a new GBLayer with the content of the image located at 
// 'fileName' to the top of the stack of layers of the GBSurface 'that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBSurfaceAddLayerFromFile(GBSurface* that, char* fileName);

// Get the 'iLayer'-th layer of the GBSurface 'that'
// 'iLayer' must be valid
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBSurfaceGetLayer(GBSurface* that, int iLayer);

// Get the number of layer of the GBSurface 'that'
#if BUILDMODE != 0
inline
#endif 
int GBSurfaceNbLayer(GBSurface* that);

// Set the _modified flag of all layers of the GBSurface 'that' 
// to 'flag'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetLayersModified(GBSurface* that, bool flag);

// Set the position of the GBLayer 'layer' into the set of layers of 
// the GBSurface 'that' to 'pos'
// If 'layer' can't be found in the surface do nothing
// 'pos' must be valid (0<='pos'<nbLayers)
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceSetLayerPos(GBSurface* that, GBLayer* layer, int pos);

// Remove the GBLayer 'layer' from the set of layers of the 
// GBSurface 'that'
// The memory used by 'layer' is freed
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceRemoveLayer(GBSurface* that, GBLayer* layer);

// Get a GSet of Facoid representing the sub areas of the GBSurface 
// 'that' affected by layers with _modified flag equal to true
// If there is no modified sub area return an empty GSet
GSetShapoid* GBSurfaceGetModifiedArea(GBSurface* that);

// Update the final pixels according to layers of the GBSurface 'that'
// Update only pixels affected by layers with the _modified flag 
// equals to true
void GBSurfaceUpdate(GBSurface* that);

// Reset all the final pix of the GBSurface 'that' to its 
// background color, and reset all the modified flag of layers to true
void GBSurfaceFlush(GBSurface* that);

// ---------------- GBSurfaceImage --------------------------

// Create a new GBSurfaceImage with dimension 'dim'
GBSurfaceImage* GBSurfaceImageCreate(VecShort2D* dim);

// Free the memory used by the GBSurfaceImage 'that'
void GBSurfaceImageFree(GBSurfaceImage** that);

// Clone the GBSurfaceImage 'that'
GBSurfaceImage* GBSurfaceImageClone(GBSurfaceImage* that);

// Get the filename of the GBSurfaceImage 'that'
#if BUILDMODE != 0
inline
#endif 
char* GBSurfaceImageFileName(GBSurfaceImage* that);

// Set the filename of the GBSurfaceImage 'that' to 'fileName'
#if BUILDMODE != 0
inline
#endif 
void GBSurfaceImageSetFileName(GBSurfaceImage* that, char* fileName);

// Save a GBSurfaceImage 'that'
// If the filename is not set do nothing and return false
// Return true if it could save the surface, false else
bool GBSurfaceImageSave(GBSurfaceImage* that);

// Create a new GBSurfaceImage with one layer containing the content 
// of the image located at 'fileName' and dimensions equal to the 
// dimensions of the image
// If the image couldn't be loaded return NULL
GBSurfaceImage* GBSurfaceImageCreateFromFile(char* fileName);

// ---------------- GBEye --------------------------

// Create a new GBEye with type 'type'
GBEye GBEyeCreateStatic(GBEyeType type);

// Free the memory used by the GBEye 'that'
void GBEyeFreeStatic(GBEye* that);

// Return the type of the GBEye 'that'
#if BUILDMODE != 0
inline
#endif 
GBEyeType _GBEyeGetType(GBEye* that);

// Get the scale of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat3D* _GBEyeScale(GBEye* that);
 
// Get a copy of the scale of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat3D _GBEyeGetScale(GBEye* that);
 
// Get the translation of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat2D* _GBEyeOrig(GBEye* that);
 
// Get a copy of the translation of the GBEye
#if BUILDMODE != 0
inline
#endif 
VecFloat2D _GBEyeGetOrig(GBEye* that);
 
// Get the rotation of the GBEye (in radians)
#if BUILDMODE != 0
inline
#endif 
float _GBEyeGetRot(GBEye* that);

// Set the scale of the GBEye
#if BUILDMODE != 0
inline
#endif 
void GBEyeSetScaleVec(GBEye* that, VecFloat3D* scale);
#if BUILDMODE != 0
inline
#endif 
void GBEyeSetScaleFloat(GBEye* that, float scale);
 
// Set the translation of the GBEye
#if BUILDMODE != 0
inline
#endif 
void _GBEyeSetOrig(GBEye* that, VecFloat2D* trans);
 
// Set the rotation of the GBEye (in radians)
#if BUILDMODE != 0
inline
#endif 
void _GBEyeSetRot(GBEye* that, float theta);
  
// Update the projection matrix of the GBEye according to scale, rot
// and origin
void GBEyeUpdateProj(GBEye* that);
  
// Get the matrix projection of the eye
#if BUILDMODE != 0
inline
#endif 
MatFloat* _GBEyeProj(GBEye* that);

// Call the appropriate GBEye<>Process according to the type of the
// GBEye 'that'
#if BUILDMODE != 0
inline
#endif 
void _GBEyeProcess(GBEye* that, GBObjPod* pod); 

// Return the projection through the GBEye 'that' of the Point 'point' 
VecFloat* GBEyeGetProjectedPoint(GBEye* that, VecFloat* point);

// Return the projection through the GBEye 'that' of the SCurve 'curve' 
SCurve* GBEyeGetProjectedCurve(GBEye* that, SCurve* curve);

// Return the projection through the GBEye 'that' of the Shapoid 'shap' 
Shapoid* GBEyeGetProjectedShapoid(GBEye* that, Shapoid* shap);

// ---------------- GBEyeOrtho --------------------------

// Return a new GBEyeOrtho with orientation 'view'
// scale is initialized to (1,1), trans to (0,0) and rot to 0
GBEyeOrtho* GBEyeOrthoCreate(GBEyeOrthoView view);

// Free the memory used by the GBEyeOrho 'that'
void GBEyeOrthoFree(GBEyeOrtho** that);

// Set the orientation the GBEyeOrtho 'that'
#if BUILDMODE != 0
inline
#endif 
void GBEyeOrthoSetView(GBEyeOrtho* that, GBEyeOrthoView view);

// Process the object of the GBObjPod 'pod' to update the viewed object 
// through the GBEyeOrtho 'that'
void GBEyeOrthoProcess(GBEyeOrtho* that, GBObjPod* pod); 

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
void GBEyeIsometricSetRotY(GBEyeIsometric* that, float theta);

// Set the angle around Right of the GBEyeOrtho to 'theta' 
// (in radians, in [-pi/2, pi/2])
// If 'theta' is out of range it is automatically bounded 
// (ex: pi -> pi/2)
#if BUILDMODE != 0
inline
#endif 
void GBEyeIsometricSetRotRight(GBEyeIsometric* that, float theta);

// Get the angle around Y of the GBEyeOrtho 'that'
#if BUILDMODE != 0
inline
#endif 
float GBEyeIsometricGetRotY(GBEyeIsometric* that);

// Get the angle around Right of the GBEyeOrtho 'that'
#if BUILDMODE != 0
inline
#endif 
float GBEyeIsometricGetRotRight(GBEyeIsometric* that);

// Process the object of the GBObjPod 'pod' to update the viewed object 
// through the GBEyeIsometric 'that'
void GBEyeIsometricProcess(GBEyeIsometric* that, GBObjPod* pod); 

// ---------------- GBHand --------------------------

// Create a new GBHand with type 'type'
GBHand GBHandCreateStatic(GBHandType type);

// Free the memory used by the GBHand 'that'
void GBHandFreeStatic(GBHand* that);

// Return the type of the GBHand 'that'
#if BUILDMODE != 0
inline
#endif 
GBHandType _GBHandGetType(GBHand* that);

// Call the appropriate GBHand<>Process according to the type of the
// GBHand 'that'
#if BUILDMODE != 0
inline
#endif 
void _GBHandProcess(GBHand* that, GBObjPod* pod); 

// ---------------- GBHandDefault --------------------------

// Create a new GBHandDefault
GBHandDefault* GBHandDefaultCreate();

// Free the memory used by the GBHandDefault 'that'
void GBHandDefaultFree(GBHandDefault** that);

// Process the viewed projection of the object in GBObjPod 'pod' into
// its handed projection through the GBHandDefault 'that' 
void GBHandDefaultProcess(GBHandDefault* that, GBObjPod* pod); 

// ---------------- GBTool --------------------------

// Create a static GBTool with GBToolType 'type'
GBTool GBToolCreateStatic(GBToolType type);

// Free the memory used by the GBTool 'that'
void GBToolFreeStatic(GBTool* that);

// Return a copy of the type of the GBTool 'that'
#if BUILDMODE != 0
inline
#endif 
GBToolType GBToolGetType(GBTool* that);

// Function to call the appropriate GBTool<>Draw function according to 
// type of GBTool 'that'
#if BUILDMODE != 0
inline
#endif 
void _GBToolDraw(GBTool* that, GBObjPod* pod);

// ---------------- GBToolPlotter --------------------------

// Create a new GBToolPlotter
GBToolPlotter* GBToolPlotterCreate();

// Free the memory used by the GBToolPlotter 'that'
void GBToolPlotterFree(GBToolPlotter** that);

// Draw the object in the GBObjPod 'pod' with the GBToolPlotter 'that'
void GBToolPlotterDraw(GBToolPlotter* that, GBObjPod* pod);

// ---------------- GBInk --------------------------

// Return the type of the GBInk 'that'
#if BUILDMODE != 0
inline
#endif 
GBInkType _GBInkGetType(GBInk* that);

// Free the memory used by the GBInk 'that'
void GBInkFree(GBInk* that);

// Entry point for the GBTool<>Draw function to get the color of the 
// appropriate GBInk according to the type of 'that'
// posInternal represents the position in the object internal space
// posExternal represents the position in the global coordinates system
// posLayer represents the position in the destination layer
GBPixel _GBInkGet(GBInk* that, GBTool* tool, GBObjPod* pod, 
  VecFloat* posInternal, VecFloat* posExternal, VecShort* posLayer);

// ---------------- GBInkSolid --------------------------

// Create a new GBInkSolid with color 'col'
GBInkSolid* GBInkSolidCreate(GBPixel* col);

// Free the memory used by the GBInkSolid 'that'
void GBInkSolidFree(GBInkSolid** that);

// Get the color of the GBInkSolid 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel GBInkSolidGet(GBInkSolid* that);

// Set the color of the GBInkSolid 'that' to 'col'
#if BUILDMODE != 0
inline
#endif 
void GBInkSolidSet(GBInkSolid* that, GBPixel* col);

// ---------------- GBObjPod --------------------------

// Create a new GBObjPod for a Point at position 'pos'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'pos' must be a vector of 2 or more dimensions
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreatePoint(VecFloat* pos, GBEye* eye, GBHand* hand, 
  GBTool* tool, GBInk* ink, GBLayer* layer);

// Create a new GBObjPod for the Shapoid 'shap'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'shap' 's dimension must be 2 or more
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreateShapoid(Shapoid* shap, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer);

// Create a new GBObjPod for the SCurve 'curve'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'curve' 's dimension must be 2 or more
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreateSCurve(SCurve* curve, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer);
  
// Free the memory used by the GBObjPod 'that'
void GBObjPodFree(GBObjPod** that);

// Return the type of the object in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBObjType GBObjPodGetType(GBObjPod* that);

// Return the object in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
void* GBObjPodGetObj(GBObjPod* that);

// Return the object viewed by its attached eye in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
void* GBObjPodGetEyeObj(GBObjPod* that);

// Return the object processed as Points by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GSetVecFloat* GBObjPodGetHandObjAsPoints(GBObjPod* that);

// Return the object processed as Shapoids by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GSetShapoid* GBObjPodGetHandObjAsShapoids(GBObjPod* that);

// Return the object processed as SCurves by its attached hand in the 
// GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GSetSCurve* GBObjPodGetHandObjAsSCurves(GBObjPod* that);

// Return the eye in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBEye* GBObjPodGetEye(GBObjPod* that);

// Return the hand in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBHand* GBObjPodGetHand(GBObjPod* that);

// Return the tool in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBTool* GBObjPodGetTool(GBObjPod* that);

// Return the ink in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBInk* GBObjPodGetInk(GBObjPod* that);

// Return the layer in the GBObjPod 'that'
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBObjPodGetLayer(GBObjPod* that);

// Set the Point viewed by its attached eye in the GBObjPod 'that' 
// to 'point'
// If 'point' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEyePoint(GBObjPod* that, VecFloat* point);

// Set the Shapoid viewed by its attached eye in the GBObjPod 'that' 
// to 'shap'
// If 'shap' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEyeShapoid(GBObjPod* that, Shapoid* shap);

// Set the SCurve viewed by its attached eye in the GBObjPod 'that' 
// to 'curve'
// If 'curve' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEyeSCurve(GBObjPod* that, SCurve* curve);

// Set the eye in the GBObjPod 'that' to 'eye'
// If 'eye' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetEye(GBObjPod* that, GBEye* eye);

// Set the hand in the GBObjPod 'that' to 'hand'
// If 'hand' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetHand(GBObjPod* that, GBHand* hand);

// Set the tool in the GBObjPod 'that' to 'tool'
// If 'tool' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetTool(GBObjPod* that, GBTool* tool);

// Set the ink in the GBObjPod 'that' to 'ink'
// If 'ink' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void _GBObjPodSetInk(GBObjPod* that, GBInk* ink);

// Set the layer in the GBObjPod 'that' to 'layer'
// If 'layer' is null do nothing
#if BUILDMODE != 0
inline
#endif 
void GBObjPodSetLayer(GBObjPod* that, GBLayer* layer);

// ---------------- GenBrush --------------------------

// Create a new GenBrush with a GBSurface of type GBSurfaceTypeImage
// and dimensions 'dim'
GenBrush* GBCreateImage(VecShort2D* dim);

// Free memory used by the GenBrush 'that'
void GBFree(GenBrush** that);

// Get the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBSurface* GBSurf(GenBrush* that);

// Get the dimensions of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D* GBDim(GenBrush* that);

// Get a copy of the dimensions of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
VecShort2D GBGetDim(GenBrush* that);

// Get the final pixels of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBFinalPixels(GenBrush* that);

// Get the final pixel at position 'pos' of the GBSurface of the GB 
// 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBFinalPixel(GenBrush* that, VecShort2D* pos);

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// of the GB 'that'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
GBPixel GBGetFinalPixel(GenBrush* that, VecShort2D* pos);

// Set the final pixel at position 'pos' of the GBSurface of the GB 
// 'that' to the pixel 'pix'
// 'pos' must be in the surface
#if BUILDMODE != 0
inline
#endif 
void GBSetFinalPixel(GenBrush* that, VecShort2D* pos, GBPixel* pix);

// Get the final pixel at position 'pos' of the GBSurface of the GB 
// 'that'
// If 'pos' is out of the surface return NULL
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBFinalPixelSafe(GenBrush* that, VecShort2D* pos);

// Get a copy of the final pixel at position 'pos' of the GBSurface 
// of the GB 'that'
// If 'pos' is out of the surface return a transparent pixel
#if BUILDMODE != 0
inline
#endif 
GBPixel GBGetFinalPixelSafe(GenBrush* that, VecShort2D* pos);

// Set the final pixel at position 'pos' of the GBSurface of the GB 
// 'that' to the pixel 'pix'
// If 'pos' is out of the surface do nothing
#if BUILDMODE != 0
inline
#endif 
void GBSetFinalPixelSafe(GenBrush* that, VecShort2D* pos, GBPixel* pix);

// Get the type of the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBSurfaceType GBGetType(GenBrush* that);

// Get the background color of the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel* GBBgColor(GenBrush* that);

// Get a copy of the background color of the GBSurface of the 
// GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GBPixel GBGetBgColor(GenBrush* that);

// Set the background color of the GBSurface of the GenBrush
//  'that' to 'col'
#if BUILDMODE != 0
inline
#endif 
void GBSetBgColor(GenBrush* that, GBPixel* col);

// Get the filename of the GBSurfaceImage of the GenBrush 'that'
// Return NULL if the surface is not a GBSurfaceImage
#if BUILDMODE != 0
inline
#endif 
char* GBFileName(GenBrush* that);

// Set the filename of the GBSurfaceImage of the GenBrush 'that' 
// to 'fileName'
// Do nothing if the surface is not a GBSurfaceImage
#if BUILDMODE != 0
inline
#endif 
void GBSetFileName(GenBrush* that, char* fileName);

// Update the GBSurface of the GenBrush 'that'
void GBUpdate(GenBrush* that);

// Render the GBSurface (save on disk, display on screen, ...) of
// the GenBrush 'that'
bool GBRender(GenBrush* that);

// Get the area of the GBSurface of the Genbrush 'that'
#if BUILDMODE != 0
inline
#endif 
int GBArea(GenBrush* that);

// Return true if the position 'pos' is inside the GBSurface of the 
// GenBrush 'that' 
// boundary, false else
#if BUILDMODE != 0
inline
#endif 
bool GBIsPosInside(GenBrush* that, VecShort2D *pos);

// Return the layers of the surface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
GSet* GBLayers(GenBrush* that);

// Add a new GBLayer with dimensions 'dim' to the top of the stack 
// of layers of the GBSurface of the GenBrush 'that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBAddLayer(GenBrush* that, VecShort2D* dim);

// Add a new GBLayer with content equals to the image located at 
// 'fileName' to the top of the stack 
// of layers of the GBSurface of the GenBrush' that'
// Return the new GBLayer
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBAddLayerFromFile(GenBrush* that, char* fileName);

// Get the 'iLayer'-th layer of the GBSurface of the GenBrush 'that'
// 'iLayer' must be valid
#if BUILDMODE != 0
inline
#endif 
GBLayer* GBGetLayer(GenBrush* that, int iLayer);

// Get the number of layer of the GBSurface of the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
int GBGetNbLayer(GenBrush* that);

// Set the position of the GBLayer 'layer' into the set of layers of 
// the GBSurface of the GenBrush'that' to 'pos'
// If 'layer' can't be found in the surface do nothing
// 'pos' must be valid (0<='pos'<nbLayers)
#if BUILDMODE != 0
inline
#endif 
void GBSetLayerPos(GenBrush* that, GBLayer* layer, int pos);

// Remove the GBLayer 'layer' from the set of layers of the 
// GBSurface of the GenBrush 'that'
// The memory used by 'layer' is freed
#if BUILDMODE != 0
inline
#endif 
void GBRemoveLayer(GenBrush* that, GBLayer* layer);

// Get the number of objects in the GenBrush 'that'
#if BUILDMODE != 0
inline
#endif 
int GBGetNbPod(GenBrush* that);

// Add a GBObjPod for the Point at position 'pos' to the GenBrush 'that'
// drawn with 'eye', 'hand' and 'tool' in layer 'layer'
// 'pos' must be a vector of 2 or more dimensions
#if BUILDMODE != 0
inline
#endif 
void _GBAddPoint(GenBrush* that, VecFloat* pos, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer);

// Add a GBObjPod for the Shapoid 'shap' to the GenBrush 'that'
// drawn with 'eye', 'hand' and 'tool' in layer 'layer'
// 'shap' 's dimension must be 2 or more
#if BUILDMODE != 0
inline
#endif 
void _GBAddShapoid(GenBrush* that, Shapoid* shap, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer);

// Add a GBObjPod for the SCurve 'curve' to the GenBrush 'that'
// drawn with 'eye', 'hand' and 'tool' in layer 'layer'
// 'curve' 's dimension must be 2 or more
#if BUILDMODE != 0
inline
#endif 
void _GBAddSCurve(GenBrush* that, SCurve* curve, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer);

// Remove from the list of pods of the GenBrush 'that' those who 
// match the 'obj', 'eye', 'hand', 'tool', 'ink' and 'layer'
// Null arguments are ignored. For example:
// GBRemovePod(that, elemA, NULL, NULL, NULL, NULL, NULL) removes all 
// the pods related to the object 'elemA'
// GBRemovePod(that, elemA, NULL, handA, NULL, NULL, NULL) removes 
// all the pods related to both the object 'elemA' AND 'handA'
// If all the filters are null it removes all the pods
void _GBRemovePod(GenBrush* that, void* obj, GBEye* eye, GBHand* hand, 
  GBTool* tool, GBInk* ink, GBLayer* layer);
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
void _GBSetPodEye(GenBrush* that, GBEye* toEye, void* obj, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer);
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
void _GBSetPodHand(GenBrush* that, GBHand* toHand, void* obj, 
  GBEye* eye, GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer);
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
void _GBSetPodTool(GenBrush* that, GBTool* toTool, void* obj, 
  GBEye* eye, GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer);
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
void _GBSetPodInk(GenBrush* that, GBInk* toInk, void* obj, 
  GBEye* eye, GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer);
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
void _GBSetPodLayer(GenBrush* that, GBLayer* toLayer, void* obj, 
  GBEye* eye, GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer);
// TODO: the filtering arguments should be in a struct

// Reset all the final pix of the surface of the GenBrush 'that' to its 
// background color, and reset all the modified flag of layers to true
#if BUILDMODE != 0
inline
#endif 
void GBFlush(GenBrush* that);

// Set to true the modified flag of the layers of pods attached to the
// object 'obj' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromObj(GenBrush* that, void* obj);

// Set to true the modified flag of the layers of pods attached to the
// GBEye 'eye' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromEye(GenBrush* that, GBEye* eye);

// Set to true the modified flag of the layers of pods attached to the
// GBInk 'ink' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromInk(GenBrush* that, GBInk* ink);

// Set to true the modified flag of the layers of pods attached to the
// GBHand 'hand' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromHand(GenBrush* that, GBHand* hand);

// Set to true the modified flag of the layers of pods attached to the
// GBTool 'tool' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromTool(GenBrush* that, GBTool* tool);

#if BUILDWITHGRAPHICLIB == 1
#include "genbrush-GTK.h"
#endif

// ================ Inliner ====================

#if BUILDMODE != 0
#include "genbrush-inline.c"
#endif


#endif
