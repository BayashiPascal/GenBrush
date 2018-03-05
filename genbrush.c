// ============ GENBRUSH.C ================

// ================= Include =================

#include "genbrush.h"
#if BUILDMODE == 0
#include "genbrush-inline.c"
#endif

// ================ Structure declaration ====================

// Header of a TGA file
typedef struct GBTGAHeader {
  // Origin of the color map
  short int _colorMapOrigin;
  // Length of the color map
  short int _colorMapLength;
  // X coordinate of the origin
  short int _xOrigin;
  // Y coordinate of the origin
  short int _yOrigin;
  // Width of the TGA
  short _width;
  // Height of the TGA
  short _height;
  // Length of a string located located after the header
  char _idLength;
  // Type of the color map
  char _colorMapType;
  // Type of the image
  char _dataTypeCode;
  // Depth of the color map
  char _colorMapDepth;
  // Number of bit per pixel
  char _bitsPerPixel;
  // Image descriptor
  char _imageDescriptor;
} GBTGAHeader;

// ================ Functions declaration ====================

// Function to decode rgba values when loading a TGA file
// Do nothing if arguments are invalid
void GBTGAMergeBytes(GBPixel* pixel, unsigned char* p, int bytes);

// Convert the indice of the TGA file data to the indice in layer's
// pixel according to origin position
int GBConvertToIndexLayerForTGA(int n, int xOrig, int yOrig, 
  int width, int height);

// Read the header of a TGA file from the stream 'stream' and store 
// it into 'header'
// Return true if it could read the header, else false
bool GBReadTGAHeader(FILE* stream, GBTGAHeader* header);

// Read the body of a TGA file from the stream 'stream' and store 
// it into 'pix', given the header 'header' of the file and the 
// blend mode 'blendMode'
// Return true if it could read the body, else false
bool GBReadTGABody(FILE *stream, GSet* pix, GBTGAHeader* header, 
  GBLayerBlendMode blendMode);

// Create a new GBLayer with dimensions and content given by the 
// TGA image on disk at location 'fileName'
// Return NULL if we couldn't create the layer
GBLayer* GBLayerCreateFromFileTGA(char* fileName);

// Save the GBSurfaceImage 'that' as a TGA file
// Return false if the image couldn't be saved, true else
bool GBSurfaceImageSaveAsTGA(GBSurfaceImage* that);

// Create a new GBSurfaceImage with one layer containing the content 
// of the TGA image located at 'fileName' and dimensions equal to the 
// dimensions of the image
// If the image couldn't be loaded return NULL
GBSurfaceImage* GBSurfaceImageCreateFromTGA(char* fileName);

// Draw the Point 'point' in the GBObjPod 'pod' with the 
// GBToolPlotter 'that'
void GBToolPlotterDrawPoint(GBToolPlotter* that, VecFloat* point, 
  GBObjPod* pod);
  
// Draw the Shapoid 'shap' in the GBObjPod 'pod' with the 
// GBToolPlotter 'that'
void GBToolPlotterDrawShapoid(GBToolPlotter* that, Shapoid* shap, 
  GBObjPod* pod);
  
// Draw the SCurve 'curve' in the GBObjPod 'pod' with the 
// GBToolPlotter 'that'
void GBToolPlotterDrawSCurve(GBToolPlotter* that, SCurve* curve, 
  GBObjPod* pod);

// Update the content of the layer 'layer' of the GenBrush 'that'
// Flush the content of the layer and redraw all the pod attached to it
void GBUpdateLayer(GenBrush* that, GBLayer* layer);

// Get the value for the projection matrix of a GBEyeOrtho
void GBEyeOrthoGetProjVal(GBEyeOrtho* that, float* val);
  
// Get the value for the projection matrix of a GBEyeIsometric
void GBEyeIsometricGetProjVal(GBEyeIsometric* that, float* val);
  
// ================ Functions implementation ====================

// Function to decode rgba values when loading a TGA file
// Do nothing if arguments are invalid
void GBTGAMergeBytes(GBPixel* pixel, unsigned char* p, int bytes) {
  // Check arguments
  if (pixel == NULL || p == NULL)
    return;
  // Merge bytes
  if (bytes == 4) {
    pixel->_rgba[GBPixelRed] = p[2];
    pixel->_rgba[GBPixelGreen] = p[1];
    pixel->_rgba[GBPixelBlue] = p[0];
    pixel->_rgba[GBPixelAlpha] = p[3];
  } else if (bytes == 3) {
    pixel->_rgba[GBPixelRed] = p[2];
    pixel->_rgba[GBPixelGreen] = p[1];
    pixel->_rgba[GBPixelBlue] = p[0];
    pixel->_rgba[GBPixelAlpha] = 255;
  } else if (bytes == 2) {
    pixel->_rgba[GBPixelRed] = (p[1] & 0x7c) << 1;
    pixel->_rgba[GBPixelGreen] = 
      ((p[1] & 0x03) << 6) | ((p[0] & 0xe0) >> 2);
    pixel->_rgba[GBPixelBlue] = (p[0] & 0x1f) << 3;
    pixel->_rgba[GBPixelAlpha] = (p[1] & 0x80);
  }
}

// Convert the indice of the TGA file data to the indice in layer's
// pixel according to origin position
int GBConvertToIndexLayerForTGA(int n, int xOrig, int yOrig, 
  int width, int height) {
  if (xOrig == 0 && yOrig == height)
    return n;
  else if (xOrig == 0 && yOrig == 0)
    return 
      (height - 1 - ((int)floor((float)n / (float)width))) * width + 
      n % width;
  else
    return n;
}

// Read the header of a TGA file from the stream 'stream' and store 
// it into 'header'
// Return true if it could read the header, else false
bool GBReadTGAHeader(FILE* stream, GBTGAHeader* header) {
  // Declare a variable to memorize the return value of fread
  size_t ret = 0;
  // Read the header's values
  header->_idLength = fgetc(stream);
  header->_colorMapType = fgetc(stream);
  // If the color map type is not supported
  if (header->_colorMapType != 0 && 
    header->_colorMapType != 1) {
    // Stop here
    return false;
  }
  header->_dataTypeCode = fgetc(stream);
  // If the data type is not supported
  if (header->_dataTypeCode != 2 && header->_dataTypeCode != 10) {
    // Stop here
    return false;
  }
  ret = fread(&(header->_colorMapOrigin), 2, 1, stream);
  if (ret == 0) {
    // Stop here
    return false;
  }
  ret = fread(&(header->_colorMapLength), 2, 1, stream);
  if (ret == 0) {
    // Stop here
    return false;
  }
  header->_colorMapDepth = fgetc(stream);
  ret = fread(&(header->_xOrigin), 2, 1, stream);
  if (ret == 0) {
    // Stop here
    return false;
  }
  ret = fread(&(header->_yOrigin), 2, 1, stream);
  if (ret == 0) {
    // Stop here
    return false;
  }
  ret = fread(&(header->_width), 2, 1, stream);
  if (ret == 0 || header->_width <= 0) {
    // Stop here
    return false;
  }
  ret = fread(&(header->_height), 2, 1, stream);
  if (ret == 0 || header->_height <= 0) {
    // Stop here
    return false;
  }
  header->_bitsPerPixel = fgetc(stream);
  header->_imageDescriptor = fgetc(stream);
  // Return the success code
  return true;
}

// Read the body of a TGA file from the stream 'stream' and store 
// it into 'pix', given the header 'header' of the file and the 
// blend mode 'blendMode'
// Return true if it could read the body, else false
bool GBReadTGABody(FILE *stream, GSet* pix, GBTGAHeader* header, 
  GBLayerBlendMode blendMode) {
  // Declare variables used during decoding
  int i = 0, j = 0;
  unsigned int bytes2read = 0, skipover = 0;
  unsigned char p[5] = {0};
  // If the number of byte per pixel is not supported
  if (header->_bitsPerPixel != 16 && 
    header->_bitsPerPixel != 24 && 
    header->_bitsPerPixel != 32)
    return false;
  // Skip the unused information
  skipover += header->_idLength;
  skipover += header->_colorMapType * header->_colorMapLength;
  fseek(stream,skipover,SEEK_CUR);
  // Calculate the number of byte per pixel
  bytes2read = header->_bitsPerPixel / 8;
  // For each pixel
  int n = 0;
  while (n < header->_height * header->_width) {
    int nLayer = GBConvertToIndexLayerForTGA(
      n, header->_xOrigin, header->_yOrigin, header->_width, header->_height);
    // Read the pixel according to the data type, merge and 
    // move to the next pixel
    if (header->_dataTypeCode == 2) {
      if (fread(p, 1, bytes2read, stream) != bytes2read)
        return false;
      GBPixel mergedPix;
      GBTGAMergeBytes(&mergedPix, p, bytes2read);
      // Create a stacked pixel
      GBStackedPixel* stacked = PBErrMalloc(GenBrushErr, 
        sizeof(GBStackedPixel));
      memcpy(&(stacked->_val), &mergedPix, sizeof(GBPixel));
      stacked->_depth = 0.0;
      stacked->_blendMode = blendMode;
      // Add the stacked pixel to the stack 
      GSetAddSort(pix + nLayer, stacked, stacked->_depth);
      // Increment the index
      ++n;
      nLayer = GBConvertToIndexLayerForTGA(
        n, header->_xOrigin, header->_yOrigin, header->_width, header->_height);
    } else if (header->_dataTypeCode == 10) {
      if (fread(p, 1, bytes2read + 1, stream) != bytes2read + 1)
        return false;
      j = p[0] & 0x7f;
      GBPixel mergedPix;
      GBTGAMergeBytes(&mergedPix, &(p[1]), bytes2read);
      // Create a stacked pixel
      GBStackedPixel* stacked = PBErrMalloc(GenBrushErr, 
        sizeof(GBStackedPixel));
      memcpy(&(stacked->_val), &mergedPix, sizeof(GBPixel));
      stacked->_depth = 0.0;
      stacked->_blendMode = blendMode;
      // Add the stacked pixel to the stack 
      GSetAddSort(pix + nLayer, stacked, stacked->_depth);
      // Increment the index
      ++n;
      nLayer = GBConvertToIndexLayerForTGA(
        n, header->_xOrigin, header->_yOrigin, header->_width, header->_height);
      if (p[0] & 0x80) {
        for (i = 0; i < j; ++i) {
          GBPixel mergedPix;
          GBTGAMergeBytes(&mergedPix, &(p[1]), bytes2read);
          // Create a stacked pixel
          GBStackedPixel* stacked = PBErrMalloc(GenBrushErr, 
            sizeof(GBStackedPixel));
          memcpy(&(stacked->_val), &mergedPix, sizeof(GBPixel));
          stacked->_depth = 0.0;
          stacked->_blendMode = blendMode;
          // Add the stacked pixel to the stack 
          GSetAddSort(pix + nLayer, stacked, stacked->_depth);
          // Increment the index
          ++n;
          nLayer = GBConvertToIndexLayerForTGA(
            n, header->_xOrigin, header->_yOrigin, header->_width, 
            header->_height);
        }
      } else {
        for (i = 0; i < j; ++i) {
          if (fread(p, 1, bytes2read, stream) != bytes2read)
            return false;
          GBPixel mergedPix;
          GBTGAMergeBytes(&mergedPix, p, bytes2read);
          // Create a stacked pixel
          GBStackedPixel* stacked = PBErrMalloc(GenBrushErr, 
            sizeof(GBStackedPixel));
          memcpy(&(stacked->_val), &mergedPix, sizeof(GBPixel));
          stacked->_depth = 0.0;
          stacked->_blendMode = blendMode;
          // Add the stacked pixel to the stack 
          GSetAddSort(pix + nLayer, stacked, stacked->_depth);
          // Increment the index
          ++n;
          nLayer = GBConvertToIndexLayerForTGA(
            n, header->_xOrigin, header->_yOrigin, header->_width, header->_height);
        }
      }
    }
  }
  // Return the success code
  return true;
}

// ---------------- GBPixel --------------------------

// Blend the pixel 'pix' into the pixel 'that'
// BlendNormal mixes colors according to their relative alpha value
// and add the alpha values
void GBPixelBlendNormal(GBPixel* that, GBPixel* pix) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pix == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pix' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare a variable for computation
  float u = 0.5 * 
    (1.0 + ((float)(that->_rgba[GBPixelAlpha]) - 
    (float)(pix->_rgba[GBPixelAlpha])) / 255.0);
  for (int iRgb = 4; iRgb--;)
    // Add 0.5 to the conversion to float ot avoid numeric instability
    if (iRgb == GBPixelAlpha)
      that->_rgba[iRgb] = (unsigned char)floor(
        MIN(255.0, (float)(that->_rgba[iRgb]) + 
        (float)(pix->_rgba[iRgb])) + 0.5);
    else
      that->_rgba[iRgb] = (unsigned char)floor(
        (1.0 - u) * (float)(that->_rgba[iRgb]) + 
        u * (float)(pix->_rgba[iRgb]) + 0.5);
}

// Blend the pixel 'pix' into the pixel 'that'
// BlendOver mixes colors according to the alpha value of 'pix'
// and add the alpha values
void GBPixelBlendOver(GBPixel* that, GBPixel* pix) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (pix == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pix' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare a variable for computation
  float u = (float)(that->_rgba[GBPixelAlpha]) / 255.0;
  for (int iRgb = 4; iRgb--;)
    // Add 0.5 to the conversion to float ot avoid numeric instability
    if (iRgb == GBPixelAlpha)
      that->_rgba[iRgb] = (unsigned char)floor(
        MIN(255.0, (float)(that->_rgba[iRgb]) + 
        (float)(pix->_rgba[iRgb])) + 0.5);
    else
      that->_rgba[iRgb] = (unsigned char)floor(
        (1.0 - u) * (float)(that->_rgba[iRgb]) + 
        u * (float)(pix->_rgba[iRgb]) + 0.5);
}

// Return the blend result of the stack of Pixel 'stack'
// If there is transparency down to the bottom of the stack, use the 
// background color 'bgColor'
// If the stack is empty, return a transparent pixel
GBPixel GBPixelStackBlend(GSet* stack, GBPixel* bgColor) {
#if BUILDMODE == 0
  if (stack == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'stack' is null");
    PBErrCatch(GenBrushErr);
  }
  if (bgColor == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'bgColor' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare the result pixel
  GBPixel res = GBColorTransparent;
  // Declare a variable to memorize the blend mode
  GBLayerBlendMode blendMode = GBLayerBlendModeNormal;
  // If the stack is not empty
  if (GSetNbElem(stack) > 0) {
    // Initalise the result pixel with the last pixel
    GSetIterBackward iter = GSetIterBackwardCreateStatic(stack);
    GBStackedPixel* pix = GSetIterGet(&iter);
    blendMode = pix->_blendMode;
    res = pix->_val;
    // For each following pixel until we reach the bottom of the stack
    // or the opacity reaches 255
    while (res._rgba[GBPixelAlpha] < 255 && GSetIterStep(&iter)) {
      pix = GSetIterGet(&iter);
      switch (blendMode) {
        case GBLayerBlendModeDefault:
          break;
        case GBLayerBlendModeNormal:
          GBPixelBlendNormal(&res, &(pix->_val));
          break;
        case GBLayerBlendModeOver:
          GBPixelBlendOver(&res, &(pix->_val));
          break;
        default:
          break;
      }
      blendMode = pix->_blendMode;
    }
    // If we've reached the bottom of the stack and there is still 
    // transparency
    if (res._rgba[GBPixelAlpha] < 255) {
      // Add the background color
      switch (blendMode) {
        case GBLayerBlendModeDefault:
          break;
        case GBLayerBlendModeNormal:
          GBPixelBlendNormal(&res, bgColor);
          break;
        case GBLayerBlendModeOver:
          GBPixelBlendOver(&res, bgColor);
          break;
        default:
          break;
      }
    }
  // Else, the stack is empty
  } else {
    // Simply copy the background pixel;
    res = *bgColor;
  }
  // Return the result pixel
  return res;
}

// ---------------- GBLayer --------------------------

// Create a new GBLayer with dimensions 'dim'
// The layer is initialized with empty stacks of pixel
// _pos = (0,0)
// blendMode = GBLayerBlendModeDefault
// stackPos = GBLayerStackPosBg
GBLayer* GBLayerCreate(VecShort2D* dim) {
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
  // Declare a variable to memorize the new layer
  GBLayer* that = PBErrMalloc(GenBrushErr, sizeof(GBLayer));
  // Set properties
  that->_dim = *dim;
  that->_pos = VecShortCreateStatic2D();
  that->_prevPos = VecShortCreateStatic2D();
  that->_pix = PBErrMalloc(GenBrushErr,
    sizeof(GSet) * GBLayerArea(that));
  for (int iPix = GBLayerArea(that); iPix--;)
    that->_pix[iPix] = GSetCreateStatic();
  that->_blendMode = GBLayerBlendModeDefault;
  that->_modified = true;
  that->_stackPos = GBLayerStackPosBg;
  // Return the new layer
  return that;
}

// Free the GBLayer 'that'
void GBLayerFree(GBLayer** that) {
  if (that == NULL || *that == NULL) {
    return;
  }
  // Free the memory
  if ((*that)->_pix != NULL) {
    GBLayerFlush(*that);
    free((*that)->_pix);
  }
  free(*that);
  *that = NULL;
}

// Create a new GBLayer with dimensions and content given by the 
// image on disk at location 'fileName'
// Return NULL if we couldn't create the layer
GBLayer* GBLayerCreateFromFile(char* fileName) {
  // If the fileName is NULL
  if (fileName == NULL)
    // Nothing to do
    return NULL;
  // Call the appropriate function depending on file extension
  if (strcmp(fileName + strlen(fileName) - 4, ".tga") == 0 || 
    strcmp(fileName + strlen(fileName) - 4, ".TGA") == 0)
    return GBLayerCreateFromFileTGA(fileName);
  return NULL;
}

// Create a new GBLayer with dimensions and content given by the 
// TGA image on disk at location 'fileName'
// Return NULL if we couldn't create the layer
GBLayer* GBLayerCreateFromFileTGA(char* fileName) {
  // Open the file
  FILE* fptr = fopen(fileName,"r");
  // If we couldn't open the file
  if (fptr == NULL)
    // Stop here
    return NULL;
  // Declare a variable to memorize the header
  GBTGAHeader h;
  // Read the header
  if (GBReadTGAHeader(fptr, &h) == false) {
    // Stop here
    fclose(fptr);
    return NULL;
  }
  // Create the layer
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, h._width);
  VecSet(&dim, 1, h._height);
  GBLayer* layer = GBLayerCreate(&dim);
  // Set a pointer to the pixels of the new layer
  GSet* pix = GBLayerPixels(layer);
  // Read the body of the TGA file
  if (GBReadTGABody(fptr, pix, &h, layer->_blendMode) == false) {
    // Stop here
    GBLayerFree(&layer);
    fclose(fptr);
    return NULL;
  }
  // Close the file
  fclose(fptr);
  // Return the new layer
  return layer; 
}

// Get the boundary of the GBLayer 'that' inside the GBSurface 'surf'
// The boundaries are given as a Facoid
// If the flag 'prevPos' is true, gives the bounday at the previous
// position
// Return NULL if the layer is completely out of the surface
Facoid* GBLayerGetBoundaryInSurface(GBLayer* that, GBSurface* surf,
  bool prevPos) {
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
  // Declare a pointer toward the relevant position accoridng to 
  // 'prevPos'
  VecShort2D* pos = 
    (prevPos == false ? GBLayerPos(that) : GBLayerPrevPos(that));
  // Get the values of the clip area made by the intersection of the 
  // layer and the surface
  VecFloat2D min = VecFloatCreateStatic2D();
  VecFloat2D max = VecFloatCreateStatic2D();
  for (int i = 2; i--;) {
    VecSet(&min, i, MAX(0, VecGet(pos, i)));
    VecSet(&max, i, MIN(VecGet(GBSurfaceDim(surf), i), 
      VecGet(GBLayerDim(that), i) + VecGet(pos, i)));
  }
  // Declare a Facoid to memorize the result
  Facoid* bound = NULL;
  // If the layer is in intersection with the surface
  VecOp(&max, 1, &min, -1);
  if (VecGet(&max, 0) > 0.0 &&
    VecGet(&max, 1) > 0.0) {
    // Set the values of the Facoid
    bound = FacoidCreate(2);
    ShapoidSetPos(bound, (VecFloat*)&min);
    ShapoidScale(bound, (VecFloat*)&max);
  }
  // Return the result
  return bound;
}

// ---------------- GBSurface --------------------------

// Create a new static GBSurface with dimension 'dim' and type 'type'
// _finalPix is set to 0
// _bgColor is set to white
GBSurface GBSurfaceCreateStatic(GBSurfaceType type, VecShort2D* dim) {
#if BUILDMODE == 0
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
  // Declare the new GBSurface
  GBSurface ret;
  // Set properties
  ret._type = type;
  ret._dim = *dim;
  ret._bgColor = GBColorWhite;
  ret._layers = GSetCreateStatic();
  // Allocate memory for the final pixels
  ret._finalPix = PBErrMalloc(GenBrushErr, 
    sizeof(GBPixel) * VecGet(dim, 0) * VecGet(dim, 1));
  memset(ret._finalPix, 0, 
    sizeof(GBPixel) * VecGet(dim, 0) * VecGet(dim, 1));
  // Flush the surface to initialize the _finalPix with the _bgColor
  GBSurfaceFlush(&ret);
  // Return the new GBSurface
  return ret;
}

// Create a new GBSurface with dimension 'dim' and type 'type'
// _finalPix is set to 0
// _bgColor is set to white
GBSurface* GBSurfaceCreate(GBSurfaceType type, VecShort2D* dim) {
#if BUILDMODE == 0
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
  // Declare the new GBSurface
  GBSurface *ret = PBErrMalloc(GenBrushErr, sizeof(GBSurface));
  // Set properties
  *ret = GBSurfaceCreateStatic(type, dim);
  // Return the new surface
  return ret;
}

// Free the memory used by the properties of the GBSurface 'that'
void GBSurfaceFreeStatic(GBSurface* that) {
  // If the pointer is null
  if (that == NULL)
    // Nothing to do
    return;
  // Free the memory
  while (GSetNbElem(&(that->_layers)) > 0) {
    GBLayer* layer = (GBLayer*)GSetPop(&(that->_layers));
    GBLayerFree(&layer);
  }
  free(that->_finalPix);
  that->_finalPix = NULL;
}

// Free the memory used by the GBSurface 'that'
void GBSurfaceFree(GBSurface** that) {
  // If the pointer is null
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Free the memory
  GBSurfaceFreeStatic(*that);
  free(*that);
  *that = NULL;
}

// Clone the GBSurface 'that'
GBSurface GBSurfaceClone(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare the clone
  GBSurface clone = *that;
  // Clone properties
  clone._finalPix = PBErrMalloc(GenBrushErr, 
    sizeof(GBPixel) * GBSurfaceArea(that));
  memcpy(clone._finalPix, GBSurfaceFinalPixels(that), 
    sizeof(GBPixel) * GBSurfaceArea(that));
  // Return the clone
  return clone;
}

// Get a GSet of Facoid representing the sub areas of the GBSurface 
// 'that' affected by layers with _modified flag equal to true
// If there is no modified sub area return an empty GSet
GSetShapoid* GBSurfaceGetModifiedArea(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare the GSet to memorize the areas
  GSetShapoid* areas = GSetShapoidCreate();
  // If the surface has layers
  if (GBSurfaceNbLayer(that) > 0) {
    // For each layer of the surface
    GSetIterForward iter = 
      GSetIterForwardCreateStatic(GBSurfaceLayers(that));
    do {
      // Get the current layer
      GBLayer* layer = GSetIterGet(&iter);
      // If the layer is modified
      if (GBLayerIsModified(layer)) {
        // Get the boundary in surface of this layer
        Facoid* bound = 
          GBLayerGetBoundaryInSurface(layer, that, false);
        // If the boundary is not empty
        if (bound != NULL) {
          // Add the boundary to the areas taking care there is no
          // overlapping areas
          FacoidAlignedAddClippedToSet(bound, areas);
          // Free memory used by the boundary
          ShapoidFree(&bound);
        }
        // If the layer has been moved we also need to repaint the 
        // area at its previous position
        if (!VecIsEqual(GBLayerPos(layer), GBLayerPrevPos(layer))) {
          // Get the boundary in surface of this layer at previous 
          // position
          bound = GBLayerGetBoundaryInSurface(layer, that, true);
          // If the boundary is not empty
          if (bound != NULL) {
            // Add the boundary to the areas taking care there is no
            // overlapping areas
            FacoidAlignedAddClippedToSet(bound, areas);
            // Free memory used by the boundary
            ShapoidFree(&bound);
          }
        }
      }
    } while (GSetIterStep(&iter));
  }
  // Return the areas
  return areas;
}

// Update the final pixels according to layers of the GBSurface 'that'
// Update only pixels affected by layers with the _modified flag 
// equals to true
void GBSurfaceUpdate(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If the surface has layers
  if (GBSurfaceNbLayer(that) > 0) {
    // Get the areas needing to be updated
    GSetShapoid* areas = GBSurfaceGetModifiedArea(that);
    // Declare some vectors to memorize pixel coordinates
    VecShort2D from = VecShortCreateStatic2D();
    VecShort2D to = VecShortCreateStatic2D();
    VecShort2D pCoord = VecShortCreateStatic2D();
    VecShort2D pLayer = VecShortCreateStatic2D();
    // Declare 3 sets for the blending of pixels
    GSet setBg = GSetCreateStatic();
    GSet setIn = GSetCreateStatic();
    GSet setFg = GSetCreateStatic();
    // For each area
    while (GSetNbElem(areas) > 0) {
      // Get the current area
      Facoid* area = (Facoid*)GSetPop(areas);
      // Facoid values are float, we need to convert to short for
      // pixel coordinates
      for (int i = 2; i--;) {
        VecSet(&from, i, (short)round(VecGet(ShapoidPos(area), i)));
        VecSet(&to, i, (short)round(VecGet(ShapoidPos(area), i) + 
          VecGet(ShapoidAxis(area, i), i)));
      }
      // For each pixel in the current area
      VecCopy(&pCoord, &from);
      do {
        // For each layer
        GSetIterForward iter = 
          GSetIterForwardCreateStatic(GBSurfaceLayers(that));
        do {
          // Get the current layer
          GBLayer* layer = GSetIterGet(&iter);
          // Get the position of the current pixel in layer coordinates
          VecCopy(&pLayer, &pCoord);
          VecOp(&pLayer, 1.0, GBLayerPos(layer), -1.0);
          // If the current pixel is inside the layer
          if (GBLayerIsPosInside(layer, &pLayer)) {
            // Add the stacked pixels of the layer to the appropriate
            // set
            switch (GBLayerGetStackPos(layer)) {
              case GBLayerStackPosBg:
                GSetAppendSet(&setBg, GBLayerPixel(layer, &pLayer));
                break;
              case GBLayerStackPosInside:
                GSetAppendSortedSet(&setIn, 
                  GBLayerPixel(layer, &pLayer));
                break;
              case GBLayerStackPosFg:
                GSetAppendSet(&setFg, GBLayerPixel(layer, &pLayer));
                break;
              default:
                break;
            }
          }
        } while (GSetIterStep(&iter));
        // Merge the 3 stacks
        GSetAppendSet(&setBg, &setIn); 
        GSetAppendSet(&setBg, &setFg);
        // Update the final pixel with the blend of the stack
        GBPixel* finalPix = GBSurfaceFinalPixel(that, &pCoord);
        *finalPix = GBPixelStackBlend(&setBg, GBSurfaceBgColor(that));
        // Empty the sets of stacked pixels
        GSetFlush(&setBg);
        GSetFlush(&setIn);
        GSetFlush(&setFg);
      } while (VecShiftStep(&pCoord, &from ,&to));
      // Free the memory used by the area
      ShapoidFree(&area);
    }
    // Free memory
    GSetFree(&areas);
    // Reset the modified flag of layers
    GBSurfaceSetLayersModified(that, false);
  }
}

// Reset all the final pix of the GBSurface 'that' to its 
// background color, and reset all the modified flag of layers to true
void GBSurfaceFlush(GBSurface* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Set all the pixel to the background color
  for (int iPix = GBSurfaceArea(that); iPix--;)
    memcpy(that->_finalPix + iPix, &(that->_bgColor), sizeof(GBPixel));
  // Reset the modified flag of layers
  GBSurfaceSetLayersModified(that, true);
}

// ---------------- GBSurfaceImage --------------------------

// Create a new GBSurfaceImage with dimension 'dim'
GBSurfaceImage* GBSurfaceImageCreate(VecShort2D* dim) {
#if BUILDMODE == 0
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
  // Declare the new GBSurfaceImage
  GBSurfaceImage* ret = PBErrMalloc(GenBrushErr, 
    sizeof(GBSurfaceImage));
  // Set properties
  ret->_surf = GBSurfaceCreateStatic(GBSurfaceTypeImage, dim);
  ret->_fileName = NULL;
  // Return the new surface
  return ret;
}

// Free the memory used by the GBSurfaceImage 'that'
void GBSurfaceImageFree(GBSurfaceImage** that) {
  // If the pointer is null
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Free the memory
  GBSurfaceFreeStatic((GBSurface*)(*that));
  if ((*that)->_fileName != NULL) {
    free((*that)->_fileName);
    (*that)->_fileName = NULL;
  }
  free(*that);
  *that = NULL;
}

// Clone the GBSurfaceImage 'that'
GBSurfaceImage* GBSurfaceImageClone(GBSurfaceImage* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare the clone
  GBSurfaceImage* clone = 
    GBSurfaceImageCreate(GBSurfaceDim((GBSurface*)that));
  // Declare a variable ot memorize he newly allocated stack of pixels
  GBPixel* nFinal = clone->_surf._finalPix;
  // Clone properties
  memcpy(clone, that, sizeof(GBSurface));
  clone->_surf._finalPix = nFinal;
  memcpy(clone->_surf._finalPix, 
    GBSurfaceFinalPixels((GBSurface*)that), 
    sizeof(GBPixel) * GBSurfaceArea((GBSurface*)that));
  clone->_fileName = NULL;
  GBSurfaceImageSetFileName(clone, GBSurfaceImageFileName(that));
  // Return the clone
  return clone;
}

// Save a GBSurfaceImage 'that'
// If the filename is not set do nothing and return false
// Return true if it could save the surface, false else
bool GBSurfaceImageSave(GBSurfaceImage* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (that->_fileName == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that->_fileName' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If the file name is null, do nothing
  if (that->_fileName == NULL)
    return false;
  // Call the appropriate function depending on file extension
  if (strcmp(that->_fileName + strlen(that->_fileName) - 4, 
    ".tga") == 0 || 
    strcmp(that->_fileName + strlen(that->_fileName) - 4, 
    ".TGA") == 0)
    return GBSurfaceImageSaveAsTGA(that);
  return false;
}

// Save the GBSurfaceImage 'that' as a TGA file
// Return false if the image couldn't be saved, true else
bool GBSurfaceImageSaveAsTGA(GBSurfaceImage* that) {
  int ret;
  // Open the file
  FILE* fptr = fopen(that->_fileName, "w");
  // If we couln't open the file
  if (fptr == NULL)
    // Stop here
    return false;
  // Declare a header
  GBTGAHeader h;
  // Create the header
  h._idLength = 0;
  h._colorMapType = 0;
  h._dataTypeCode = 2;
  h._colorMapOrigin = 0;
  h._colorMapLength = 0;
  h._colorMapDepth = 0;
  h._width = VecGet(GBSurfaceDim((GBSurface*)that), 0);
  h._height = VecGet(GBSurfaceDim((GBSurface*)that), 1);
  h._xOrigin = 0;
  h._yOrigin = h._height;
  h._bitsPerPixel = 32;
  h._imageDescriptor = 0;
  // Write the header
  ret = putc(h._idLength, fptr);
  if (ret == EOF) {
    fclose(fptr);
    return false;
  }
  ret = putc(h._colorMapType, fptr);
  if (ret == EOF) {
    fclose(fptr);
    return false;
  }
  ret = putc(h._dataTypeCode, fptr);
  if (ret == EOF) {
    fclose(fptr);
    return false;
  }
  ret = fwrite(&(h._colorMapOrigin), 2, 1, fptr);
  if (ret == 0) {
    fclose(fptr);
    return false;
  }
  ret = fwrite(&(h._colorMapLength), 2, 1, fptr);
  if (ret == 0) {
    fclose(fptr);
    return false;
  }
  ret = putc(h._colorMapDepth, fptr);
  if (ret == EOF) {
    fclose(fptr);
    return false;
  }
  ret = fwrite(&(h._xOrigin), 2, 1, fptr);
  if (ret == 0) {
    fclose(fptr);
    return false;
  }
  ret = fwrite(&(h._yOrigin), 2, 1, fptr);
  if (ret == 0) {
    fclose(fptr);
    return false;
  }
  ret = fwrite(&(h._width), 2, 1, fptr);
  if (ret == 0) {
    fclose(fptr);
    return false;
  }
  ret = fwrite(&(h._height), 2, 1, fptr);
  if (ret == 0) {
    fclose(fptr);
    return false;
  }
  ret = putc(h._bitsPerPixel, fptr);
  if (ret == EOF) {
    fclose(fptr);
    return false;
  }
  ret = putc(h._imageDescriptor, fptr);
  if (ret == EOF) {
    fclose(fptr);
    return false;
  }
  // Declare a variable to memorize the final layer's pixel
  GBPixel* pix = GBSurfaceFinalPixels((GBSurface*)that);
  // For each pixel
  for (int y = 0; y < h._height; ++y) {
    for (int x = 0; x < h._width; ++x) {
      // Get the index of the pixel
      int i = y * h._width + x;
      // Write the pixel values
      ret = putc(pix[i]._rgba[GBPixelBlue], fptr);
      if (ret == EOF) {
        fclose(fptr);
        return false;
      }
      ret = putc(pix[i]._rgba[GBPixelGreen], fptr);
      if (ret == EOF) {
        fclose(fptr);
        return false;
      }
      ret = putc(pix[i]._rgba[GBPixelRed], fptr);
      if (ret == EOF) {
        fclose(fptr);
        return false;
      }
      ret = putc(pix[i]._rgba[GBPixelAlpha], fptr);
      if (ret == EOF) {
        fclose(fptr);
        return false;
      }
    }
  }
  // Close the file
  fclose(fptr);
  // Return the success code
  return true;
}

// Create a new GBSurfaceImage with one layer containing the content 
// of the image located at 'fileName' and dimensions equal to the 
// dimensions of the image
// If the image couldn't be loaded return NULL
GBSurfaceImage* GBSurfaceImageCreateFromFile(char* fileName) {
#if BUILDMODE == 0
  if (fileName == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'fileName' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If the file name is null, return NULL
  if (fileName == NULL)
    return NULL;
  // Call the appropriate function depending on file extension
  if (strcmp(fileName + strlen(fileName) - 4, ".tga") == 0 || 
    strcmp(fileName + strlen(fileName) - 4, ".TGA") == 0)
    return GBSurfaceImageCreateFromTGA(fileName);
  return NULL;
}

// Create a new GBSurfaceImage with one layer containing the content 
// of the TGA image located at 'fileName' and dimensions equal to the 
// dimensions of the image
// If the image couldn't be loaded return NULL
GBSurfaceImage* GBSurfaceImageCreateFromTGA(char* fileName) {
#if BUILDMODE == 0
  if (fileName == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'fileName' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Load the image into a GBLayer
  GBLayer* layer = GBLayerCreateFromFileTGA(fileName);
  // If we couldn't load the image
  if (layer == NULL)
    // Return null
    return NULL;
  // Create a new GBSurfaceImage with same dimension as the layer
  GBSurfaceImage* surf = GBSurfaceImageCreate(GBLayerDim(layer));
  // Set the background color to transparent
  GBPixel transparent = GBColorTransparent;
  GBSurfaceSetBgColor((GBSurface*)surf, &transparent);
  // Add the layer to the surf
  GSetAppend(GBSurfaceLayers((GBSurface*)surf), layer);
  // Update the surface
  GBSurfaceUpdate((GBSurface*)surf);
  // Return the new surface
  return surf;
}

// ---------------- GBEye --------------------------

// Return a new GBEye with type 'type'
// scale is initialized to (1,1), trans to (0,0) and rot to 0
GBEye GBEyeCreateStatic(GBEyeType type) {
  // Declare a GBEye
  GBEye that;
  // Set properties
  that._type = type;
  that._scale = VecFloatCreateStatic3D();
  VecSet(&(that._scale), 0, 1.0); VecSet(&(that._scale), 1, 1.0); 
  VecSet(&(that._scale), 2, 1.0);
  that._orig = VecFloatCreateStatic2D();
  VecSet(&(that._orig), 0, 0.0); VecSet(&(that._orig), 1, 0.0);
  that._theta = 0.0;
  // Allocate memory for the projection matrix
  VecShort2D dim = VecShortCreateStatic2D();
  VecSet(&dim, 0, 3); VecSet(&dim, 1, 3); 
  that._proj = MatFloatCreate(&dim);
  // Return the new GBEye
  return that;
}

// Free the memory used by the GBEye
void GBEyeFreeStatic(GBEye* that) {
  if (that == NULL)
    return;
  // Free memory
  MatFree(&(that->_proj));
}

// Update the projection matrix of the GBEye according to scale, rot
// and origin
void GBEyeUpdateProj(GBEye* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare a variable to memorize the values of the projection matrix
  // order is: (0,0),(0,1),(0,2),(1,0),...
  float val[12] = {0.0};
  // Set the values according to the eye type
  switch (that->_type) {
    case GBEyeTypeOrtho:
      GBEyeOrthoGetProjVal((GBEyeOrtho*)that, val);
      break;
    case GBEyeTypeIsometric:
      GBEyeIsometricGetProjVal((GBEyeIsometric*)that, val);
      break;
    default:
      break;
  }
  // Set the values to the matrix
  VecShort2D index = VecShortCreateStatic2D();
  VecShort2D dim = MatGetDim(that->_proj);
  int iVal = 0;
  do {
    MatSet(that->_proj, &index, val[iVal]);
    ++iVal;
  } while (VecStep(&index, &dim));
}

// Return the projection through the GBEye 'that' of the Point 'point' 
VecFloat* GBEyeGetProjectedPoint(GBEye* that, VecFloat* point) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (point == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'point' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Allocate memory for the output vector
  VecFloat* ret = VecClone(point);
  // Create a VecFloat3D to perform the product with the projection
  // matrix
  VecFloat3D v = VecFloatCreateStatic3D();
  for (int i = MIN(VecGetDim(point), 3); i--;)
    VecSet(&v, i, VecGet(point, i));
  // Perform the product
  VecFloat* w = MatGetProdVec(that->_proj, &v);
  // Translate to origin
  for (int i = 2; i--;)
    VecSet(w, i, VecGet(w, i) + VecGet(&(that->_orig), i));
  // Copy the values into the result
  for (int i = MIN(VecGetDim(point), 3); i--;)
    VecSet(ret, i, VecGet(w, i));
  // Free the memory used for the product
  VecFree(&w);
  // Return the result
  return ret;
}
  
// Return the projection through the GBEye 'that' of the SCurve 'curve' 
SCurve* GBEyeGetProjectedCurve(GBEye* that, SCurve* curve) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (curve == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'curve' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Get a copy of the curve
  SCurve* ret = SCurveClone(curve);
  // Replace each curve's control point by its projection
  GSetIterForward iter = 
    GSetIterForwardCreateStatic(SCurveCtrls(ret));
  do {
    VecFloat* ctrl = (VecFloat*)GSetIterGet(&iter);
    VecFloat* v = GBEyeGetProjectedPoint(that, ctrl);
    VecCopy(ctrl ,v);
    VecFree(&v);
  } while (GSetIterStep(&iter));
  // Return the projected curve
  return ret;
}

// Return the projection through the GBEye 'that' of the Shapoid 'shap' 
Shapoid* GBEyeGetProjectedShapoid(GBEye* that, Shapoid* shap) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (shap == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'shap' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Clone the shapoid
  Shapoid* ret = ShapoidClone(shap);
  // Project the origin of the shapoid
  VecFloat* proj = GBEyeGetProjectedPoint(that, ShapoidPos(ret));
  ShapoidSetPos(ret, proj);
  VecFree(&proj);
  // Project each axis
  for (int iAxis = ShapoidGetDim(ret); iAxis--;) {
    proj = GBEyeGetProjectedPoint(that, ShapoidAxis(ret, iAxis));
    VecCopy(ShapoidAxis(ret, iAxis), proj);
    VecFree(&proj);
  }
  // Return the new shapoid
  return ret;
}
  
// ---------------- GBEyeOrtho --------------------------

// Return a new GBEyeOrtho with orientation 'view'
// scale is initialized to (1,1), trans to (0,0) and rot to 0
GBEyeOrtho* GBEyeOrthoCreate(GBEyeOrthoView view) {
  // Allocate memory
  GBEyeOrtho* that = PBErrMalloc(GenBrushErr, sizeof(GBEyeOrtho));
  // Set properties
  that->_view = view;
  that->_eye = GBEyeCreateStatic(GBEyeTypeOrtho);
  GBEyeUpdateProj((GBEye*)that);
  // Return the new GBEyeOrtho
  return that;
}

// Free the memory used by the GBEyeOrho
void GBEyeOrthoFree(GBEyeOrtho** that) {
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  GBEyeFreeStatic((GBEye*)*that);
  free(*that);
  *that = NULL;
}

// Get the value for the projection matrix of a GBEyeOrtho
void GBEyeOrthoGetProjVal(GBEyeOrtho* that, float* val) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (val == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'val' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare variable for optimization
  float cosTheta = cos(GBEyeGetRot(that));
  float sinTheta = sin(GBEyeGetRot(that));
  float scaleX = VecGet(GBEyeScale(that), 0);
  float scaleY = VecGet(GBEyeScale(that), 1);
  float scaleZ = VecGet(GBEyeScale(that), 2);
  // Set the values according to the orientation
  switch (that->_view) {
    case GBEyeOrthoViewFront:
      val[0] = scaleX * cosTheta;
      val[1] = scaleX * sinTheta;
      val[2] = 0.0;
      val[3] = scaleY * -sinTheta;
      val[4] = scaleY * cosTheta;
      val[5] = 0.0;
      val[6] = 0.0;
      val[7] = 0.0;
      val[8] = scaleZ;
      break;
    case GBEyeOrthoViewRear:
      val[0] = scaleX * -cosTheta;
      val[1] = scaleX * -sinTheta;
      val[2] = 0.0;
      val[3] = scaleY * -sinTheta;
      val[4] = scaleY * cosTheta;
      val[5] = 0.0;
      val[6] = 0.0;
      val[7] = 0.0;
      val[8] = -scaleZ;
      break;
    case GBEyeOrthoViewTop:
      val[0] = scaleX * cosTheta;
      val[1] = scaleX * sinTheta;
      val[2] = 0.0;
      val[3] = 0.0;
      val[4] = 0.0;
      val[5] = -scaleZ;
      val[6] = scaleY * -sinTheta;
      val[7] = scaleY * cosTheta;
      val[8] = 0.0;
      break;
    case GBEyeOrthoViewBottom:
      val[0] = scaleX * cosTheta;
      val[1] = scaleX * sinTheta;
      val[2] = 0.0;
      val[3] = 0.0;
      val[4] = 0.0;
      val[5] = scaleZ;
      val[6] = scaleY * sinTheta;
      val[7] = scaleY * -cosTheta;
      val[8] = 0.0;
      break;
    case GBEyeOrthoViewLeft:
      val[0] = 0.0;
      val[1] = 0.0;
      val[2] = scaleZ;
      val[3] = scaleY * -sinTheta;
      val[4] = scaleY * cosTheta;
      val[5] = 0.0;
      val[6] = scaleX * -cosTheta;
      val[7] = scaleX * -sinTheta;
      val[8] = 0.0;
      break;
    case GBEyeOrthoViewRight:
      val[0] = 0.0;
      val[1] = 0.0;
      val[2] = -scaleZ;
      val[3] = scaleY * -sinTheta;
      val[4] = scaleY * cosTheta;
      val[5] = 0.0;
      val[6] = scaleX * cosTheta;
      val[7] = scaleX * sinTheta;
      val[8] = 0.0;
      break;
    default: 
      break;
  }
}

// Process the object of the GBObjPod 'pod' to update the viewed object 
// through the GBEyeOrtho 'that'
void GBEyeOrthoProcess(GBEyeOrtho* that, GBObjPod* pod) {
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
  VecFloat* point = NULL;
  SCurve* curve = NULL;
  Shapoid* shap = NULL;
  switch (GBObjPodGetType(pod)) {
    case GBObjTypePoint:
      point = GBEyeGetProjectedPoint((GBEye*)that, 
        GBObjPodGetObjAsPoint(pod));
      GBObjPodSetEyePoint(pod, point);
      break;
    case GBObjTypeSCurve:
      curve = GBEyeGetProjectedCurve((GBEye*)that, 
        GBObjPodGetObjAsSCurve(pod));
      GBObjPodSetEyeSCurve(pod, curve);
      break;
    case GBObjTypeShapoid:
      shap = GBEyeGetProjectedShapoid((GBEye*)that, 
        GBObjPodGetObjAsShapoid(pod));
      GBObjPodSetEyeShapoid(pod,shap);
      break;
    default:
      break;
  }
}

// ---------------- GBEyeIsometric --------------------------

// Return a new GBEyeIsometric with orientation 'view'
// scale is initialized to (1,1), trans to (0,0) and rot to 0
// thetaY is initialized to pi/4 and thetaRight to pi/4
GBEyeIsometric* GBEyeIsometricCreate() {
  // Allocate memory
  GBEyeIsometric* that = PBErrMalloc(GenBrushErr, 
    sizeof(GBEyeIsometric));
  // Set properties
  that->_thetaY = PBMATH_QUARTERPI;
  that->_thetaRight = PBMATH_QUARTERPI;
  that->_eye = GBEyeCreateStatic(GBEyeTypeIsometric);
  GBEyeUpdateProj((GBEye*)that);
  // Return the new GBEyeIsometric
  return that;
}

// Free the memory used by the GBEyeIsometric 'that'
void GBEyeIsometricFree(GBEyeIsometric** that) {
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  GBEyeFreeStatic((GBEye*)*that);
  free(*that);
  *that = NULL;
}

// Process the object of the GBObjPod 'pod' to update the viewed object 
// through the GBEyeIsometric 'that'
void GBEyeIsometricProcess(GBEyeIsometric* that, GBObjPod* pod) {
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
  VecFloat* point = NULL;
  SCurve* curve = NULL;
  Shapoid* shap = NULL;
  switch (GBObjPodGetType(pod)) {
    case GBObjTypePoint:
      point = GBEyeGetProjectedPoint((GBEye*)that, 
        GBObjPodGetObjAsPoint(pod));
      GBObjPodSetEyePoint(pod, point);
      break;
    case GBObjTypeSCurve:
      curve = GBEyeGetProjectedCurve((GBEye*)that, 
        GBObjPodGetObjAsSCurve(pod));
      GBObjPodSetEyeSCurve(pod, curve);
      break;
    case GBObjTypeShapoid:
      shap = GBEyeGetProjectedShapoid((GBEye*)that, 
        GBObjPodGetObjAsShapoid(pod));
      GBObjPodSetEyeShapoid(pod,shap);
      break;
    default:
      break;
  }
}

// Get the value for the projection matrix of a GBEyeIsometric
void GBEyeIsometricGetProjVal(GBEyeIsometric* that, float* val) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (val == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'val' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare variables for optimization
  float scaleX = VecGet(GBEyeScale(that), 0);
  float scaleY = VecGet(GBEyeScale(that), 1);
  float scaleZ = VecGet(GBEyeScale(that), 2);
  float cosTheta = cos(GBEyeGetRot(that));
  float sinTheta = sin(GBEyeGetRot(that));
  float cosThetaY = cos(GBEyeIsometricGetRotY(that));
  float sinThetaY = sin(GBEyeIsometricGetRotY(that));
  float cosThetaR = cos(GBEyeIsometricGetRotRight(that));
  float sinThetaR = sin(GBEyeIsometricGetRotRight(that));
  // Set the values
  val[0] = scaleX * cosTheta * cosThetaY - 
    scaleY * sinTheta * sinThetaY * sinThetaR;
  val[1] = scaleX * sinTheta * cosThetaY + 
    scaleY * cosTheta * sinThetaY * sinThetaR;
  val[2] = scaleZ * sinThetaY * cosThetaR;

  val[3] = -1.0 * scaleY * sinTheta * cosThetaR;
  val[4] = scaleY * cosTheta * cosThetaR;
  val[5] = -scaleZ * sinThetaR;
  
  val[6] = -1.0 * (scaleX * cosTheta * sinThetaY + 
    scaleY * sinTheta * cosThetaY * sinThetaR);
  val[7] = -1.0 * scaleX * sinTheta * sinThetaY + 
    scaleY * cosTheta * cosThetaY * sinThetaR;
  val[8] = scaleZ * cosThetaY * cosThetaR;
}

// ---------------- GBHand --------------------------

// Create a new GBHand with type 'type'
GBHand GBHandCreateStatic(GBHandType type) {
  // Declare the new hand
  GBHand hand;
  // Set properties
  hand._type = type;
  // Return the new hand
  return hand;
}

// Free the memory used by the GBHand 'that'
void GBHandFreeStatic(GBHand* that) {
  // Nothing to do
  (void)that;
}

// ---------------- GBHandDefault --------------------------

// Create a new GBHandDefault
GBHandDefault* GBHandDefaultCreate() {
  // Declare the new hand
  GBHandDefault* hand = PBErrMalloc(GenBrushErr, sizeof(GBHandDefault));
  // Set properties
  hand->_hand = GBHandCreateStatic(GBHandTypeDefault);
  // Return the new hand
  return hand;
}

// Free the memory used by the GBHandDefault 'that'
void GBHandDefaultFree(GBHandDefault** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    return;
  // Free memory
  GBHandFreeStatic((GBHand*)(*that));
  free(*that);
  *that = NULL;
}

// Process the viewed projection of the object in GBObjPod 'pod' into
// its handed projection through the GBHandDefault 'that' 
void GBHandDefaultProcess(GBHandDefault* that, GBObjPod* pod) {
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
  if (GBObjPodGetEyeObj(pod) == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "Viewed object of 'pod' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  (void)that;
  // Empty all the sets
  GSetVecFloat* setPoints = GBObjPodGetHandObjAsPoints(pod);
  while (GSetNbElem(setPoints) > 0) {
    VecFloat* point = GSetPop(setPoints);
    VecFree(&point);
  }
  GSetShapoid* setShapoids = GBObjPodGetHandObjAsShapoids(pod);
  while (GSetNbElem(setShapoids) > 0) {
    Shapoid* shap = GSetPop(setShapoids);
    ShapoidFree(&shap);
  }
  GSetSCurve* setSCurves = GBObjPodGetHandObjAsSCurves(pod);
  while (GSetNbElem(setPoints) > 0) {
    SCurve* curve = GSetPop(setSCurves);
    SCurveFree(&curve);
  }
  // Simply copy the viewed object as it is according to its type
  switch (GBObjPodGetType(pod)) {
    case GBObjTypePoint:
      // Add a copy of the viewed point
      GSetAppend(setPoints, VecClone(GBObjPodGetEyeObjAsPoint(pod)));
      break;
    case GBObjTypeShapoid:
      // Add a copy of the viewed shapoid
      GSetAppend(setShapoids, 
        ShapoidClone(GBObjPodGetEyeObjAsShapoid(pod)));
      break;
    case GBObjTypeSCurve:
      // Add a copy of the viewed scurve
      GSetAppend(setSCurves, 
        SCurveClone(GBObjPodGetEyeObjAsSCurve(pod)));
      break;
    default:
      break;
  }
}

// ---------------- GBTool --------------------------

// Create a static GBTool with GBToolType 'type'
GBTool GBToolCreateStatic(GBToolType type) {
  // Declare the GBTool
  GBTool that;
  // Set properties
  that._type = type;
  // Return the tool
  return that;
}

// Free the memory used by the GBTool 'that'
void GBToolFreeStatic(GBTool* that) {
  // Nothing to do
  (void)that;
}

// ---------------- GBToolPlotter --------------------------

// Create a new GBToolPlotter
GBToolPlotter* GBToolPlotterCreate() {
  // Declare the new GBToolPlotter
  GBToolPlotter* that = PBErrMalloc(GenBrushErr, sizeof(GBToolPlotter));
  // Set properties
  that->_tool = GBToolCreateStatic(GBToolTypePlotter);
  // Return the new tool
  return that;
}

// Free the memory used by the GBToolPlotter 'that'
void GBToolPlotterFree(GBToolPlotter** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Free memory
  GBToolFreeStatic(&((*that)->_tool));
  free(*that);
  *that = NULL;
}

// Draw the object in the GBObjPod 'pod' with the GBToolPlotter 'that'
void GBToolPlotterDraw(GBToolPlotter* that, GBObjPod* pod) {
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
  // Loop on the Points drawn by hand
  if (GSetNbElem(&(pod->_handPoints)) > 0) {
    GSetIterForward iter = 
      GSetIterForwardCreateStatic(&(pod->_handPoints));
    do {
      // Get the point
      VecFloat* point = GSetIterGet(&iter);
      // Draw the point
      GBToolPlotterDrawPoint(that, point, pod);
    } while (GSetIterStep(&iter));
  }
  // Loop on the Shapoids drawn by hand
  if (GSetNbElem(&(pod->_handShapoids)) > 0) {
    GSetIterForward iter = 
      GSetIterForwardCreateStatic(&(pod->_handShapoids));
    do {
      // Get the shapoid
      Shapoid* shap = GSetIterGet(&iter);
      // Draw the shapoid
      GBToolPlotterDrawShapoid(that, shap, pod);
    } while (GSetIterStep(&iter));
  }
  // Loop on the SCurves drawn by hand
  if (GSetNbElem(&(pod->_handSCurves)) > 0) {
    GSetIterForward iter = 
      GSetIterForwardCreateStatic(&(pod->_handSCurves));
    do {
      // Get the curve
      SCurve* curve = GSetIterGet(&iter);
      // Draw the curve
      GBToolPlotterDrawSCurve(that, curve, pod);
    } while (GSetIterStep(&iter));
  }
}

// Draw the Point 'point' in the GBObjPod 'pod' with the 
// GBToolPlotter 'that'
void GBToolPlotterDrawPoint(GBToolPlotter* that, VecFloat* point, 
  GBObjPod* pod) {
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
  if (point == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'point' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Get the position in layer
  VecShort2D posLayer = VecShortCreateStatic2D();
  for (int i = 2; i--;)
    VecSet(&posLayer, i, (short)round(VecGet(point, i)));
  // Get the color from the ink
  // As the point as no internal parameters create a null 3d vector
  // instead
  VecFloat3D null = VecFloatCreateStatic3D();
  GBPixel pixColor = GBInkGet(GBObjPodGetInk(pod), that, pod, &null, 
    point, &posLayer);
  // Get the depth
  float depth = 0.0;
  if (VecGetDim(point) > 2)
    depth = VecGet(point, 2);
  // Add the pixel at the point position in the layer
  GBLayerAddPixelSafe(GBObjPodGetLayer(pod), &posLayer, 
    &pixColor, depth); 
}
  
// Draw the Shapoid 'shap' in the GBObjPod 'pod' with the 
// GBToolPlotter 'that'
void GBToolPlotterDrawShapoid(GBToolPlotter* that, Shapoid* shap, 
  GBObjPod* pod) {
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
  if (shap == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'shap' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Get the bounding box of the shapoid in layer
  Facoid* bound = ShapoidGetBoundingBox(shap);
  // Declare two vectors for the VecShiftStep
  VecShort* from = VecShortCreate(ShapoidGetDim(bound));
  VecShort* to = VecShortCreate(ShapoidGetDim(bound));
  // Initialise the values of the vectors for the VecShiftStep
  for (int iAxis = ShapoidGetDim(bound); iAxis--;) {
    if (iAxis < 2) {
      VecSet(from, iAxis, 
        (short)round(MAX(VecGet(ShapoidPos(bound), iAxis), 0.0)));
      short v = (short)round(VecGet(ShapoidPos(bound), iAxis) + 
        VecGet(ShapoidAxis(bound, iAxis), iAxis)) + 1;
      VecSet(to, iAxis, 
        MIN(v, VecGet(GBLayerDim(GBObjPodGetLayer(pod)),iAxis)));
    } else {
      VecSet(from, iAxis, 
        (short)round(VecGet(ShapoidPos(bound), iAxis)));
      short v = (short)round(VecGet(ShapoidPos(bound), iAxis) + 
        VecGet(ShapoidAxis(bound, iAxis), iAxis)) + 1;
      VecSet(to, iAxis, v);
    }
  }
  // Loop on pixels in layers inside the bounding box
  VecShort* posLayer = VecClone(from);
  VecFloat* posLayerFloat = VecFloatCreate(VecGetDim(posLayer));
  do {
    for (int iAxis = VecGetDim(posLayer); iAxis--;)
      VecSet(posLayerFloat, iAxis, 
        (float)VecGet(posLayer, iAxis) + 0.5);
    // If this pixel is inside the Shapoid
    if (ShapoidIsPosInside(shap, posLayerFloat)) {
      // Get the current internal coordinates
      VecFloat* posIn = ShapoidImportCoord(shap, posLayerFloat);
      // Get the current external coordinates
      VecFloat* posExt = 
        ShapoidExportCoord(GBObjPodGetObjAsShapoid(pod), posIn);
      // Get the ink at this position
      GBPixel pixColor = GBInkGet(GBObjPodGetInk(pod), that, pod, 
        posIn, posExt, posLayer);
      // Get the depth
      float depth = 0.0;
      if (VecGetDim(posLayerFloat) > 2)
        depth = VecGet(posLayerFloat, 2);
      // Add the pixel to the layer
      GBLayerAddPixelSafe(GBObjPodGetLayer(pod), (VecShort2D*)posLayer, 
        &pixColor, depth); 
      // Free memory
      VecFree(&posIn);
      VecFree(&posExt);
    }
  } while (VecShiftStep(posLayer, from, to));
  // Free memory
  VecFree(&from);
  VecFree(&to);
  VecFree(&posLayer);
  VecFree(&posLayerFloat);
  ShapoidFree(&bound);
}

// Draw the SCurve 'curve' in the GBObjPod 'pod' with the 
// GBToolPlotter 'that'
void GBToolPlotterDrawSCurve(GBToolPlotter* that, SCurve* curve, 
  GBObjPod* pod) {
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
  if (curve == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'curve' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Det the approximate length of the curve
  float length = SCurveGetApproxLen(curve);
  // Calculate the delta step based on teh apporximate length
  float delta = 0.5 / length;
  // Create an iterator on the curve
  SCurveIter iter = SCurveIterCreateStatic(curve, delta);
  // Declare a vector to store the internal position as a vector
  VecFloat* posIn = VecFloatCreate(1);
  // Declare a vector to memorize the position in layer as short
  VecShort* posLayer = VecShortCreate(SCurveGetDim(curve));
  // Declare a vector to memorize the 
  VecShort* prevPosLayer = VecShortCreate(SCurveGetDim(curve));
  // Loop on the curve internal position
  do {
    // Get the internal position
    float p = SCurveIterGetPos(&iter);
    // Get the position in layer
    VecFloat* posLayerFloat = SCurveIterGet(&iter);
    for (int iAxis = VecGetDim(posLayer); iAxis--;)
      VecSet(posLayer, iAxis, 
        (short)round(VecGet(posLayerFloat, iAxis)));
    // Ensure the position is different (at pixel level) with the 
    // previous one, except for the first step (p==0.0)
    if (ISEQUALF(p, 0.0) == true || 
      VecIsEqual(posLayer, prevPosLayer) == false) {
      // Get the internal position as a vector
      VecSet(posIn, 0, p);
      // Get the external position
      VecFloat* posExt = SCurveGet(GBObjPodGetObjAsSCurve(pod), p);
      // Get the ink at this position
      GBPixel pixColor = GBInkGet(GBObjPodGetInk(pod), that, pod, 
        posIn, posExt, posLayer);
      // Get the depth
      float depth = 0.0;
      if (VecGetDim(posLayerFloat) > 2)
        depth = VecGet(posLayerFloat, 2);
      // Add the pixel to the layer
      GBLayerAddPixelSafe(GBObjPodGetLayer(pod), (VecShort2D*)posLayer, 
        &pixColor, depth); 
      // Free memory
      VecFree(&posExt);
      // Set the previous position
      VecCopy(prevPosLayer, posLayer);
    }
    // Free memory
    VecFree(&posLayerFloat);
  } while (SCurveIterStep(&iter));
  // Free memory
  VecFree(&posIn);
  VecFree(&posLayer);
  VecFree(&prevPosLayer);
}

// ---------------- GBInk --------------------------

// Entry point for the GBTool<>Draw function to get the color of the 
// appropriate GBInk according to the type of 'that'
// posInternal represents the position in the object internal space
// posExternal represents the position in the global coordinates system
// posLayer represents the position in the destination layer
GBPixel _GBInkGet(GBInk* that, GBTool* tool, GBObjPod* pod, 
  VecFloat* posInternal, VecFloat* posExternal, VecShort* posLayer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Currently unused, voided and left for forward compatibility
  (void)tool; (void)pod;
  (void)posInternal; (void)posExternal; (void)posLayer;
  // Declare the result pixel
  GBPixel pix = GBColorTransparent;
  // Call the appropriate function according to the type of ink
  switch (GBInkGetType(that)) {
    case GBInkTypeSolid:
      pix = GBInkSolidGet((GBInkSolid*)that);
      break;
    default:
      break;
  }
  // Return the result pixel
  return pix;
}  

// ---------------- GBInk --------------------------

// Free the memory used by the GBInk 'that'
void GBInkFree(GBInk* that) {
  // Nothing to do
  (void)that;
}

// ---------------- GBInkSolid --------------------------

// Create a new GBInkSolid with color 'col'
GBInkSolid* GBInkSolidCreate(GBPixel* col) {
#if BUILDMODE == 0
  if (col == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'col' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare the new GBInkSolid
  GBInkSolid* ink = PBErrMalloc(GenBrushErr, sizeof(GBInkSolid));
  // Set properties
  ink->_ink._type = GBInkTypeSolid;
  ink->_color = *col;
  // Return the new ink
  return ink;
}

// Free the memory used by the GBInkSolid 'that'
void GBInkSolidFree(GBInkSolid** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Free memory
  GBInkFree(&((*that)->_ink));
  free(*that);
  *that = NULL;
}

// ---------------- GBObjPod --------------------------

// Create a new GBObjPod for a Point at position 'pos'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'pos' must be a vector of 2 or more dimensions
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreatePoint(VecFloat* pos, GBEye* eye, GBHand* hand, 
  GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (VecGetDim(pos) < 2) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, 
    "'pos' 's dimension is invalid (%d>=2)",
    VecGetDim(pos));
    PBErrCatch(GenBrushErr);
  }
#endif
  // Check that the parameters are not null
  if (eye == NULL || hand == NULL || tool == NULL || 
    ink == NULL || layer == NULL)
    return NULL;
  // Declare the new GBObjPod
  GBObjPod* pod = PBErrMalloc(GenBrushErr, sizeof(GBObjPod));
  // Set properties
  pod->_type = GBObjTypePoint;
  pod->_srcPoint = pos;
  pod->_eyePoint = NULL;
  pod->_handPoints = GSetVecFloatCreateStatic();
  pod->_handShapoids = GSetShapoidCreateStatic();
  pod->_handSCurves = GSetSCurveCreateStatic();
  pod->_eye = eye;
  pod->_hand = hand;
  pod->_tool = tool;
  pod->_ink = ink;
  pod->_layer = layer;
  // Process the Point through eye and hand
  GBEyeProcess(eye, pod);
  GBHandProcess(hand, pod);
  // Return the new pod
  return pod;
}

// Create a new GBObjPod for the Shapoid 'shap'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'shap' 's dimension must be 2 or more
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreateShapoid(Shapoid* shap, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (shap == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'shap' is null");
    PBErrCatch(GenBrushErr);
  }
  if (ShapoidGetDim(shap) < 2) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, 
    "'shap' 's dimension is invalid (%d>=2)",
    ShapoidGetDim(shap));
    PBErrCatch(GenBrushErr);
  }
#endif
  // Check that the parameters are not null
  if (eye == NULL || hand == NULL || tool == NULL || 
    ink == NULL || layer == NULL)
    return NULL;
  // Declare the new GBObjPod
  GBObjPod* pod = PBErrMalloc(GenBrushErr, sizeof(GBObjPod));
  // Set properties
  pod->_type = GBObjTypeShapoid;
  pod->_srcShapoid = shap;
  pod->_eyeShapoid = NULL;
  pod->_handPoints = GSetVecFloatCreateStatic();
  pod->_handShapoids = GSetShapoidCreateStatic();
  pod->_handSCurves = GSetSCurveCreateStatic();
  pod->_eye = eye;
  pod->_hand = hand;
  pod->_tool = tool;
  pod->_ink = ink;
  pod->_layer = layer;
  // Process the Shapoid through eye and hand
  GBEyeProcess(eye, pod);
  GBHandProcess(hand, pod);
 // Return the new pod
  return pod;
}

// Create a new GBObjPod for the SCurve 'curve'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'curve' 's dimension must be 2 or more
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreateSCurve(SCurve* curve, GBEye* eye, 
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (curve == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'curve' is null");
    PBErrCatch(GenBrushErr);
  }
  if (SCurveGetDim(curve) < 2) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, 
    "'curve' 's dimension is invalid (%d>=2)",
    SCurveGetDim(curve));
    PBErrCatch(GenBrushErr);
  }
#endif
  // Check that the parameters are not null
  if (eye == NULL || hand == NULL || tool == NULL || 
    ink == NULL || layer == NULL)
    return NULL;
  // Declare the new GBObjPod
  GBObjPod* pod = PBErrMalloc(GenBrushErr, sizeof(GBObjPod));
  // Set properties
  pod->_type = GBObjTypeSCurve;
  pod->_srcSCurve = curve;
  pod->_eyeSCurve = NULL;
  pod->_handPoints = GSetVecFloatCreateStatic();
  pod->_handShapoids = GSetShapoidCreateStatic();
  pod->_handSCurves = GSetSCurveCreateStatic();
  pod->_eye = eye;
  pod->_hand = hand;
  pod->_tool = tool;
  pod->_ink = ink;
  pod->_layer = layer;
  // Process the SCurve through eye and hand
  GBEyeProcess(eye, pod);
  GBHandProcess(hand, pod);
  // Return the new pod
  return pod;
}

// Free the memory used by the GBObjPod 'that'
void GBObjPodFree(GBObjPod** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Free memory
  switch (GBObjPodGetType(*that)) {
    case GBObjTypePoint:
      VecFree(&((*that)->_eyePoint));
      break;
    case GBObjTypeShapoid:
      ShapoidFree(&((*that)->_eyeShapoid));
      break;
    case GBObjTypeSCurve:
      SCurveFree(&((*that)->_eyeSCurve));
      break;
    default:
      break;
  }
  while (GSetNbElem(&((*that)->_handPoints)) > 0) {
    VecFloat* point = GSetPop(&((*that)->_handPoints));
    VecFree(&point);
  }
  while (GSetNbElem(&((*that)->_handShapoids)) > 0) {
    Shapoid* shap = GSetPop(&((*that)->_handShapoids));
    ShapoidFree(&shap);
  }
  while (GSetNbElem(&((*that)->_handSCurves)) > 0) {
    SCurve* curve = GSetPop(&((*that)->_handSCurves));
    SCurveFree(&curve);
  }
  free(*that);
  *that = NULL;
}

// ---------------- GenBrush --------------------------

// Create a new GenBrush with a GBSurface of type GBSurfaceTypeImage
// and dimensions 'dim'
GenBrush* GBCreateImage(VecShort2D* dim) {
#if BUILDMODE == 0
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
  // Declare the new GenBrush
  GenBrush* ret = PBErrMalloc(GenBrushErr, sizeof(GenBrush));
  // Set properties
  ret->_surf = (GBSurface*)GBSurfaceImageCreate(dim);
  ret->_pods = GSetCreateStatic();
  // Return the new GenBrush
  return ret;
}

// Free memory used by the GenBrush 'that'
void GBFree(GenBrush** that) {
  // If the pointer is null
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Free memory
  switch (GBGetType(*that)) {
    case GBSurfaceTypeDefault:
      GBSurfaceFree(&((*that)->_surf));
      break;
    case GBSurfaceTypeImage:
      GBSurfaceImageFree((GBSurfaceImage**)&((*that)->_surf));
      break;
    default:
      break;
  }
  while (GBGetNbPod(*that) > 0) {
    GBObjPod* pod = GSetPop(&((*that)->_pods));
    GBObjPodFree(&pod);
  }
  free(*that);
  *that = NULL;
}

// Update the GBSurface of the GenBrush 'that'
void GBUpdate(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Refresh the layers that I've been modified
  // If there is layers
  if (GBGetNbLayer(that) > 0) {
    // Declare an iterator on the layers
    GSetIterForward iter = 
      GSetIterForwardCreateStatic(GBSurfaceLayers(GBSurf(that)));
    // Loop on the layers
    do {
      // Get the current layer
      GBLayer* layer = GSetIterGet(&iter);
      // If this layer has been modified
      if (GBLayerIsModified(layer))
        // Refresh the layer content
        GBUpdateLayer(that, layer);
    } while (GSetIterStep(&iter));
  }
  // Request the update of the surface
  GBSurfaceUpdate(GBSurf(that));
}

// Update the content of the layer 'layer' of the GenBrush 'that'
// Flush the content of the layer and redraw all the pod attached to it
void GBUpdateLayer(GenBrush* that, GBLayer* layer) {
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
  // Flush the content of the layer
  GBLayerFlush(layer);
  // If there is no pods
  if (GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod is attahced ot the requested layer
    if (pod->_layer == layer)
      // Redraw the pod
      GBToolDraw(GBObjPodGetTool(pod), pod);
  } while (GSetIterStep(&iter));
}

// Render the GBSurface (save on disk, display on screen, ...) of
// the GenBrush 'that'
bool GBRender(GenBrush* that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare a variable to memorize the success of rendering
  bool status = false;
  switch (GBSurfaceGetType(that->_surf)) {
    case GBSurfaceTypeImage:
      status = GBSurfaceImageSave((GBSurfaceImage*)(that->_surf));
      break;
#if BUILDWITHGRAPHICLIB == 1
    case GBSurfaceTypeWidget:
      GBSurfaceWidgetRender((GBSurfaceWidget*)(that->_surf));
      status = true;
      break;
    case GBSurfaceTypeApp:
      status = GBSurfaceAppRender((GBSurfaceApp*)(that->_surf));
      break;
#endif
    default:
      break;
  }
  // Return the status of rendering
  return status;
}

// Remove from the list of pods of the GenBrush 'that' those who 
// match the 'obj', 'eye', 'hand', 'tool', 'ink' and 'layer'
// Null arguments are ignored. For example:
// GBRemovePod(that, elemA, NULL, NULL, NULL, NULL, NULL) removes all 
// the pods related to the object 'elemA'
// GBRemovePod(that, elemA, NULL, handA, NULL, NULL, NULL) removes 
// all the pods related to both the object 'elemA' AND 'handA'
// If all the filters are null it removes all the pods
void _GBRemovePod(GenBrush* that, void* obj, GBEye* eye, GBHand* hand, 
  GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If there is no pods
  if (GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Declare a variable for the end condition of the loop
  bool flag = true;
  // Loop on pods
  while (flag) {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filters
    if ((obj == NULL || (void*)(pod->_srcPoint) == obj) &&
      (eye == NULL || pod->_eye == eye) &&
      (hand == NULL || pod->_hand == hand) &&
      (tool == NULL || pod->_tool == tool) &&
      (ink == NULL || pod->_ink == ink) &&
      (layer == NULL || pod->_layer == layer)) {
      // Remove the pod and move to the next one
      GBObjPodFree(&pod);
      flag = GSetIterRemoveElem(&iter);
    // Else the pod doesn't match the filters
    } else {
      // Move to the next pod
      flag = GSetIterStep(&iter);
    }
  }
}

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
  GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If 'toEye' is null or there is no pods
  if (toEye == NULL || GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filters
    if ((obj == NULL || (void*)(pod->_srcPoint) == obj) &&
      (eye == NULL || pod->_eye == eye) &&
      (hand == NULL || pod->_hand == hand) &&
      (tool == NULL || pod->_tool == tool) &&
      (ink == NULL || pod->_ink == ink) &&
      (layer == NULL || pod->_layer == layer))
      // Set the eye of the pod
      pod->_eye = toEye;
  } while (GSetIterStep(&iter));
}

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
  GBEye* eye, GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If 'toHand' is null or there is no pods
  if (toHand == NULL || GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filters
    if ((obj == NULL || (void*)(pod->_srcPoint) == obj) &&
      (eye == NULL || pod->_eye == eye) &&
      (hand == NULL || pod->_hand == hand) &&
      (tool == NULL || pod->_tool == tool) &&
      (ink == NULL || pod->_ink == ink) &&
      (layer == NULL || pod->_layer == layer))
      // Set the hand of the pod
      pod->_hand = toHand;
  } while (GSetIterStep(&iter));
}

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
  GBEye* eye, GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If 'toTool' is null or there is no pods
  if (toTool == NULL || GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filters
    if ((obj == NULL || (void*)(pod->_srcPoint) == obj) &&
      (eye == NULL || pod->_eye == eye) &&
      (hand == NULL || pod->_hand == hand) &&
      (tool == NULL || pod->_tool == tool) &&
      (ink == NULL || pod->_ink == ink) &&
      (layer == NULL || pod->_layer == layer))
      // Set the tool of the pod
      pod->_tool = toTool;
  } while (GSetIterStep(&iter));
}

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
  GBEye* eye, GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If 'toInk' is null or there is no pods
  if (toInk == NULL || GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filters
    if ((obj == NULL || (void*)(pod->_srcPoint) == obj) &&
      (eye == NULL || pod->_eye == eye) &&
      (hand == NULL || pod->_hand == hand) &&
      (tool == NULL || pod->_tool == tool) &&
      (ink == NULL || pod->_ink == ink) &&
      (layer == NULL || pod->_layer == layer))
      // Set the tool of the pod
      pod->_ink = toInk;
  } while (GSetIterStep(&iter));
}

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
  GBEye* eye, GBHand* hand, GBTool* tool, GBInk* ink, GBLayer* layer) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If 'toLayer' is null or there is no pods
  if (toLayer == NULL || GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filters
    if ((obj == NULL || (void*)(pod->_srcPoint) == obj) &&
      (eye == NULL || pod->_eye == eye) &&
      (hand == NULL || pod->_hand == hand) &&
      (tool == NULL || pod->_tool == tool) &&
      (ink == NULL || pod->_ink == ink) &&
      (layer == NULL || pod->_layer == layer))
      // Set the layer of the pod
      pod->_layer = toLayer;
  } while (GSetIterStep(&iter));
}

// Set to true the modified flag of the layers of pods attached to the
// object 'obj' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromObj(GenBrush* that, void* obj) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (obj == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'obj' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If there is no pods
  if (GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filter
    if (GBObjPodGetObj(pod) == obj) {
      // Reprocess the object
      GBEyeProcess(GBObjPodGetEye(pod), pod);
      GBHandProcess(GBObjPodGetHand(pod), pod);
      // Set the layer flag
      GBLayerSetModified(pod->_layer, true);
    }
  } while (GSetIterStep(&iter));
}

// Set to true the modified flag of the layers of pods attached to the
// GBEye 'eye' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromEye(GenBrush* that, GBEye* eye) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (eye == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'eye' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If there is no pods
  if (GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filter
    if (GBObjPodGetEye(pod) == eye) {
      // Reprocess the object
      GBEyeProcess(GBObjPodGetEye(pod), pod);
      GBHandProcess(GBObjPodGetHand(pod), pod);
      // Set the layer flag
      GBLayerSetModified(pod->_layer, true);
    }
  } while (GSetIterStep(&iter));
}

// Set to true the modified flag of the layers of pods attached to the
// GBInk 'ink' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromInk(GenBrush* that, GBInk* ink) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (ink == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'ink' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If there is no pods
  if (GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filter
    if (pod->_ink == ink)
      // Set the layer flag
      GBLayerSetModified(pod->_layer, true);
  } while (GSetIterStep(&iter));
}

// Set to true the modified flag of the layers of pods attached to the
// GBHand 'hand' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromHand(GenBrush* that, GBHand* hand) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (hand == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'hand' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If there is no pods
  if (GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filter
    if (GBObjPodGetHand(pod) == hand) {
      GBHandProcess(GBObjPodGetHand(pod), pod);
      // Set the layer flag
      GBLayerSetModified(pod->_layer, true);
    }
  } while (GSetIterStep(&iter));
}

// Set to true the modified flag of the layers of pods attached to the
// GBTool 'tool' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromTool(GenBrush* that, GBTool* tool) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (tool == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'tool' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // If there is no pods
  if (GBGetNbPod(that) == 0)
    // Nothing to do
    return;
  // Declare an iterator on the set of pods
  GSetIterForward iter = GSetIterForwardCreateStatic(&(that->_pods));
  // Loop on pods
  do {
    // Get the current pod
    GBObjPod* pod = GSetIterGet(&iter);
    // If the pod matches the filter
    if (pod->_tool == tool)
      // Set the layer flag
      GBLayerSetModified(pod->_layer, true);
  } while (GSetIterStep(&iter));
}

// ================ GTK Functions ====================
#if BUILDWITHGRAPHICLIB == 1
  #include "genbrush-GTK.c"
#endif
