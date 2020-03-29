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
int GBConvertToIndexLayerForTGA(int n, GBTGAHeader* header);

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
GBLayer* GBLayerCreateFromFileTGA(const char* const fileName);

// Save the GBSurfaceImage 'that' as a TGA file
// Return false if the image couldn't be saved, true else
bool GBSurfaceImageSaveAsTGA(const GBSurfaceImage* const that);

// Create a new GBSurfaceImage with one layer containing the content 
// of the TGA image located at 'fileName' and dimensions equal to the 
// dimensions of the image
// If the image couldn't be loaded return NULL
GBSurfaceImage* GBSurfaceImageCreateFromTGA(const char* const fileName);

// Draw the Point 'point' in the GBObjPod 'pod' with the 
// GBToolPlotter 'that'
void GBToolPlotterDrawPoint(const GBToolPlotter* const that, 
  const VecFloat* const point, const GBObjPod* const pod);
  
// Draw the Shapoid 'shap' in the GBObjPod 'pod' with the 
// GBToolPlotter 'that'
void GBToolPlotterDrawShapoid(const GBToolPlotter* const that, 
  const Shapoid* const shap, const GBObjPod* const pod);
  
// Draw the SCurve 'curve' in the GBObjPod 'pod' with the 
// GBToolPlotter 'that'
void GBToolPlotterDrawSCurve(const GBToolPlotter* const that, 
  const SCurve* const curve, const GBObjPod* const pod);

// Update the content of the layer 'layer' of the GenBrush 'that'
// Flush the content of the layer and redraw all the pod attached to it
void GBUpdateLayer(GenBrush* const that, GBLayer* const layer);

// Get the value for the projection matrix of a GBEyeOrtho
void GBEyeOrthoGetProjVal(const GBEyeOrtho* const that, 
  float* const val);
  
// Get the value for the projection matrix of a GBEyeIsometric
void GBEyeIsometricGetProjVal(const GBEyeIsometric* const that, 
  float* const val);

// Normalize in hue the final pixels of the GBSurface 'that'
void GBSurfaceNormalizeHue(GBSurface* const that);

// Apply ordered dithering to the final pixels of the GBSurface 'that'
void GBSurfaceOrderedDithering(GBSurface* const that);

// Apply Floyd-Steinberg dithering to the final pixels of the GBSurface 'that'
void GBSurfaceFloydSteinbergDithering(GBSurface* const that);

// Return a clone of the GenBrush 'that' with its final surface scaled
// to the dimensions 'dim' according to the scaling method AvgNeighbour
GenBrush* GBScaleAvgNeighbour(const GenBrush* const that, 
  const VecShort2D* const dim);
    
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
int GBConvertToIndexLayerForTGA(int n, GBTGAHeader* header) {
  if ((header->_imageDescriptor & 32) == 0)
    // lower left hand corner
    return n;
  else
    // upper left hand corner
    return (header->_height - 1 - 
      ((int)floor((float)n / (float)(header->_width)))) * 
      header->_width + n % header->_width;
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
  // Origin of the screen
  // TODO: Override the initial _yOrigin
  /*if ((header->_imageDescriptor & 32) == 0)
    // lower left hand corner
    header->_yOrigin = header->_height;
  else
    // upper left hand corner
    header->_yOrigin = 0;*/
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
    header->_bitsPerPixel != 32) {
    sprintf(GenBrushErr->_msg, "Invalid bitesPerPixel (%d)", 
      header->_bitsPerPixel);
    return false;
  }
  // Skip the unused information
  skipover += header->_idLength;
  skipover += header->_colorMapType * header->_colorMapLength;
  fseek(stream,skipover,SEEK_CUR);
  // Calculate the number of byte per pixel
  bytes2read = header->_bitsPerPixel / 8;
  // Flag to manage premature end of file
  bool flagPrematureEnd = false;
  // For each pixel
  int n = 0;
  while (n < header->_height * header->_width && !flagPrematureEnd) {
    int nLayer = GBConvertToIndexLayerForTGA(n, header);
    // Read the pixel according to the data type, merge and 
    // move to the next pixel
    if (header->_dataTypeCode == 2) {
      if (fread(p, 1, bytes2read, stream) != bytes2read) {
        flagPrematureEnd = true;
      } else {
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
        nLayer = GBConvertToIndexLayerForTGA(n, header);
      }
    } else if (header->_dataTypeCode == 10) {
      if (fread(p, 1, bytes2read + 1, stream) != bytes2read + 1) {
        flagPrematureEnd = true;
      } else {
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
        nLayer = GBConvertToIndexLayerForTGA(n, header);
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
            nLayer = GBConvertToIndexLayerForTGA(n, header);
          }
        } else {
          for (i = 0; i < j; ++i) {
            if (fread(p, 1, bytes2read, stream) != bytes2read) {
              sprintf(GenBrushErr->_msg, "fread failed (%s)", 
                strerror(errno));
              return false;
            }
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
            nLayer = GBConvertToIndexLayerForTGA(n, header);
          }
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
void GBPixelBlendNormal(GBPixel* const that , const GBPixel* const pix) {
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
void GBPixelBlendOver(GBPixel* const that, const GBPixel* const pix) {
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
GBPixel GBPixelStackBlend(const GSet* const stack, 
  const GBPixel* const bgColor) {
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

// Convert the GBPixel 'that' from RGB to HSV. Alpha channel is unchanged
GBPixel GBPixelRGB2HSV(const GBPixel* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare the result pixel
  GBPixel res = *that;
  // Calculate the hsv values
  float rgb[3] = {0.0, 0.0, 0.0};
  rgb[GBPixelRed] = (float)(that->_rgba[GBPixelRed]) / 255.0;
  rgb[GBPixelGreen] = (float)(that->_rgba[GBPixelGreen]) / 255.0;
  rgb[GBPixelBlue] = (float)(that->_rgba[GBPixelBlue]) / 255.0;
  float max = MAX(rgb[0], MAX(rgb[1], rgb[2]));
  float min = MIN(rgb[0], MIN(rgb[1], rgb[2]));
  float delta = max - min;
  float hsv[3] = {0.0, 0.0, 0.0};
  if (ISEQUALF(delta, 0.0))
    hsv[GBPixelHue] = 0.0;
  else if (ISEQUALF(max, rgb[GBPixelRed]))
    hsv[GBPixelHue] = 60.0 * (float)(((int)round(
      (rgb[GBPixelGreen] - rgb[GBPixelBlue]) / delta)) % 6);
  else if (ISEQUALF(max, rgb[GBPixelGreen]))
    hsv[GBPixelHue] = 
      60.0 * ((rgb[GBPixelBlue] - rgb[GBPixelRed]) / delta + 2.0);
  else if (ISEQUALF(max, rgb[GBPixelBlue]))
    hsv[GBPixelHue] = 
      60.0 * ((rgb[GBPixelRed] - rgb[GBPixelGreen]) / delta + 4.0);
  res._hsva[GBPixelHue] = 
    (unsigned char)round(hsv[GBPixelHue] / 360.0 * 255.0);
  if (ISEQUALF(max, 0.0))
    hsv[GBPixelSaturation] = 0.0;
  else 
    hsv[GBPixelSaturation] = delta / max;
  res._hsva[GBPixelSaturation] = 
    (unsigned char)round(hsv[GBPixelSaturation] * 255.0);
  hsv[GBPixelValue] = max;
  res._hsva[GBPixelValue] = 
    (unsigned char)round(hsv[GBPixelValue] * 255.0);
  // Return the result
  return res;
}

// Convert the GBPixel 'that' from HSV to RGB. Alpha channel is unchanged
GBPixel GBPixelHSV2RGB(const GBPixel* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare the result pixel
  GBPixel res = *that;
  // Calculate the rgb values
  float hsv[3] = {0.0, 0.0, 0.0};
  hsv[GBPixelHue] = (float)(that->_hsva[GBPixelHue]) / 255.0 * 360.0;
  hsv[GBPixelSaturation] = 
    (float)(that->_hsva[GBPixelSaturation]) / 255.0;
  hsv[GBPixelValue] = (float)(that->_hsva[GBPixelValue]) / 255.0;
  float c = hsv[GBPixelValue] * hsv[GBPixelSaturation];
  float x = c * (1.0 - fabs((int)round(hsv[GBPixelHue] / 60.0) % 2 - 1));
  float m = hsv[GBPixelValue] - c;
  float rgb[3] = {0.0, 0.0, 0.0};
  if (hsv[GBPixelHue] < 60.0) {
    rgb[GBPixelRed] = c;
    rgb[GBPixelGreen] = x;
    rgb[GBPixelBlue] = 0.0;
  } else if (hsv[GBPixelHue] < 120.0) {
    rgb[GBPixelRed] = x;
    rgb[GBPixelGreen] = c;
    rgb[GBPixelBlue] = 0.0;
  } else if (hsv[GBPixelHue] < 180.0) {
    rgb[GBPixelRed] = 0.0;
    rgb[GBPixelGreen] = c;
    rgb[GBPixelBlue] = x;
  } else if (hsv[GBPixelHue] < 240.0) {
    rgb[GBPixelRed] = 0.0;
    rgb[GBPixelGreen] = x;
    rgb[GBPixelBlue] = c;
  } else if (hsv[GBPixelHue] < 300.0) {
    rgb[GBPixelRed] = x;
    rgb[GBPixelGreen] = 0.0;
    rgb[GBPixelBlue] = c;
  } else {
    rgb[GBPixelRed] = c;
    rgb[GBPixelGreen] = 0.0;
    rgb[GBPixelBlue] = x;
  }
  res._rgba[GBPixelRed] = 
    (unsigned char)round(255.0 * (rgb[GBPixelRed] + m));
  res._rgba[GBPixelGreen] = 
    (unsigned char)round(255.0 * (rgb[GBPixelGreen] + m));
  res._rgba[GBPixelBlue] = 
    (unsigned char)round(255.0 * (rgb[GBPixelBlue] + m));
  // Return the result
  return res;
}

// ---------------- GBLayer --------------------------

// Create a new GBLayer with dimensions 'dim'
// The layer is initialized with empty stacks of pixel
// _pos = (0,0)
// blendMode = GBLayerBlendModeDefault
// stackPos = GBLayerStackPosBg
GBLayer* GBLayerCreate(const VecShort2D* const dim) {
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
  that->_scale = VecFloatCreateStatic2D();
  that->_prevScale = VecFloatCreateStatic2D();
  VecSet(&(that->_scale), 0, 1.0);
  VecSet(&(that->_scale), 1, 1.0);
  that->_prevScale = that->_scale;
  that->_pix = PBErrMalloc(GenBrushErr,
    sizeof(GSet) * GBLayerArea(that));
  for (int iPix = GBLayerArea(that); iPix--;)
    that->_pix[iPix] = GSetCreateStatic();
  that->_blendMode = GBLayerBlendModeDefault;
  that->_modified = true;
  that->_stackPos = GBLayerStackPosBg;
  that-> _isFlushed = true;
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
// By default layers created from images are not flushed
// Return NULL if we couldn't create the layer
GBLayer* GBLayerCreateFromFile(const char* const fileName) {
  // If the fileName is NULL
  if (fileName == NULL)
    // Nothing to do
    return NULL;
  // Declare the result layer
  GBLayer* layer = NULL;
  // Call the appropriate function depending on file extension
  if (strcmp(fileName + strlen(fileName) - 4, ".tga") == 0 || 
    strcmp(fileName + strlen(fileName) - 4, ".TGA") == 0)
    layer = GBLayerCreateFromFileTGA(fileName);
  // Set the flag isFlushed to false to avoid stacked pixels
  // to be flushed away between rendering
  GBLayerSetFlushed(layer, false);
  // Return the result layer
  return layer;
}

// Create a new GBLayer with dimensions and content given by the 
// TGA image on disk at location 'fileName'
// Return NULL if we couldn't create the layer
GBLayer* GBLayerCreateFromFileTGA(const char* const fileName) {
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
// If the flag 'prevStatus' is true, gives the boundary at the previous
// position
// Return NULL if the layer is completely out of the surface
Facoid* GBLayerGetBoundaryInSurface(const GBLayer* const that, 
  const GBSurface* const surf, const bool prevStatus) {
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
  // Declare a pointer toward the relevant position according to 
  // 'prevPos'
  VecShort2D* pos = 
    (prevStatus == false ? GBLayerPos(that) : GBLayerPrevPos(that));
  VecFloat2D* scale = 
    (prevStatus == false ? GBLayerScale(that) : GBLayerPrevScale(that));
  // Get the values of the clip area made by the intersection of the 
  // layer and the surface
  VecFloat2D min = VecFloatCreateStatic2D();
  VecFloat2D max = VecFloatCreateStatic2D();
  for (int i = 2; i--;) {
    VecSet(&min, i, MAX(0, floor(((float)VecGet(pos, i)) * VecGet(scale, i))));
    VecSet(&max, i, MIN(VecGet(GBSurfaceDim(surf), i), 
      floor((float)(VecGet(GBLayerDim(that), i) + VecGet(pos, i)) * VecGet(scale, i)) + 1.0));
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

// ---------------- GBPostProcessing --------------------------

// Create a static GBPostProcessing with type 'type'
GBPostProcessing* GBPostProcessingCreate(const GBPPType type) {
  // Declare the new GBPostProcessing
  GBPostProcessing* ret = PBErrMalloc(GenBrushErr, 
    sizeof(GBPostProcessing));
  // Set the properties
  ret->_type = type;
  // Return the new post processing
  return ret;
}

// Create a new static GBPostProcessing with type 'type'
GBPostProcessing GBPostProcessingCreateStatic(const GBPPType type) {
  // Declare the new GBPostProcessing
  GBPostProcessing ret;
  // Set the properties
  ret._type = type;
  // Return the new post processing
  return ret;
}

// Free the memory used by the GBPostProcessing 'that'
void GBPostProcessingFree(GBPostProcessing** that) {
  // Check argument
  if (that == NULL || *that == NULL)
    // Nothing to do
    return;
  // Free memory
  free(*that);
  *that = NULL;
}

// ---------------- GBSurface --------------------------

// Create a new static GBSurface with dimension 'dim' and type 'type'
// _finalPix is set to 0
// _bgColor is set to white
GBSurface GBSurfaceCreateStatic(const GBSurfaceType type, 
  const VecShort2D* const dim) {
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
GBSurface* GBSurfaceCreate(const GBSurfaceType type, 
  const VecShort2D* const dim) {
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
void GBSurfaceFreeStatic(GBSurface* const that) {
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
GBSurface GBSurfaceClone(const GBSurface* const that) {
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
GSetShapoid* GBSurfaceGetModifiedArea(const GBSurface* const that) {
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
        // If the layer has been moved or scaled we also need to  
        // repaint the area at its previous position/scale
        if (!VecIsEqual(GBLayerPos(layer), GBLayerPrevPos(layer)) ||
          !VecIsEqual(GBLayerScale(layer), GBLayerPrevScale(layer))) {
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
          // Immediately update the previous scale to avoid it
          // to be applied to next prevPos
          if (!VecIsEqual(GBLayerScale(layer), 
            GBLayerPrevScale(layer))) {
            layer->_prevScale = layer->_scale;
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
void GBSurfaceUpdate(GBSurface* const that) {
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
        VecSet(&from, i, (short)round(ShapoidPosGet(area, i)));
        VecSet(&to, i, (short)round(ShapoidPosGet(area, i) + 
          ShapoidAxisGet(area, i, i)));
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
          VecSet(&pLayer, 0, (int)floor(
            ((float)VecGet(&pLayer, 0)) / 
            VecGet(GBLayerScale(layer), 0)));
          VecSet(&pLayer, 1, (int)floor(
            ((float)VecGet(&pLayer, 1)) / 
            VecGet(GBLayerScale(layer), 1)));
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
void GBSurfaceFlush(GBSurface* const that) {
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

// Apply the post processing 'post' to the final pixels in the 
// GBSurface 'that'
void GBSurfacePostProcess(GBSurface* const that, 
  const GBPostProcessing* const post) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (post == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'post' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Call the appropriate post processing
  switch(GBPostProcessingGetType(post)) {
    case GBPPTypeNormalizeHue:
      GBSurfaceNormalizeHue(that);
      break;
    case GBPPTypeOrderedDithering:
      GBSurfaceOrderedDithering(that);
      break;
    case GBPPTypeFloydSteinbergDithering:
      GBSurfaceFloydSteinbergDithering(that);
      break;
    default:
      return;
  }
}

// Normalize in hue the final pixels of the GBSurface 'that'
void GBSurfaceNormalizeHue(GBSurface* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare a variable to memorize the maximum possible intensity
  float maxIntensity = 255.0;
  // Declare two variables to memorize the minimum and maximum hue 
  // intensity
  float min = maxIntensity;
  float max = 0.0;
  // Search the minimum and maximum intensity
  VecShort2D p = VecShortCreateStatic2D();
  do {
    GBPixel* pix = GBSurfaceFinalPixel(that, &p);
    float intensity = (float)MAX(pix->_rgba[GBPixelRed], 
      MAX(pix->_rgba[GBPixelGreen], pix->_rgba[GBPixelBlue]));
    if (intensity > max)
      max = intensity;
    intensity = (float)MIN(pix->_rgba[GBPixelRed], 
      MIN(pix->_rgba[GBPixelGreen], pix->_rgba[GBPixelBlue]));
    if (intensity < min)
      min = intensity;
  } while (VecStep(&p, GBSurfaceDim(that)));
  // If the image is not already normalized and is normalizable
  if (!ISEQUALF(min, max) && (min >= 1.0 || max <= maxIntensity)) {
    // Declare a variable for optimization
    float c = maxIntensity / (max - min);
    // Normalize each final pixel color
    VecSetNull(&p);
    do {
      GBPixel* pix = GBSurfaceFinalPixel(that, &p);
      float intensity = ((float)(pix->_rgba[GBPixelRed]) - min) * c;
      pix->_rgba[GBPixelRed] = 
        (unsigned char)round(MIN(255.0, MAX(0.0, intensity)));
      intensity = ((float)(pix->_rgba[GBPixelGreen]) - min) * c;
      pix->_rgba[GBPixelGreen] = 
        (unsigned char)round(MIN(255.0, MAX(0.0, intensity)));
      intensity = ((float)(pix->_rgba[GBPixelBlue]) - min) * c;
      pix->_rgba[GBPixelBlue] = 
        (unsigned char)round(MIN(255.0, MAX(0.0, intensity)));
    } while (VecStep(&p, GBSurfaceDim(that)));
  }
}

// Apply ordered dithering to the final pixels of the GBSurface 'that'
void GBSurfaceOrderedDithering(GBSurface* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare the filter matrix
  unsigned char filter[2][2] = {{64, 128}, {192, 0}};
  // Loop on the final pixels of the surface
  VecShort2D p = VecShortCreateStatic2D();
  do {
    // Get the pixel
    GBPixel* pix = GBSurfaceFinalPixel(that, &p);
    // Apply the filter to each channel of the pixel
    for (int iRgb = 4; iRgb--;) {
      if (pix->_rgba[iRgb] > filter[VecGet(&p, 1) % 2][VecGet(&p, 0) % 2]) {
        pix->_rgba[iRgb] = 255;
      } else {
        pix->_rgba[iRgb] = 0;
      }
    }
  } while (VecStep(&p, GBSurfaceDim(that)));
}

// Apply Floyd-Steinberg dithering to the final pixels of the GBSurface 'that'
void GBSurfaceFloydSteinbergDithering(GBSurface* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare the error propagation matrix
  float* error = PBErrMalloc(GenBrushErr, 
    GBSurfaceArea(that) * 4L * sizeof(float));
  for (long i = GBSurfaceArea(that) * 4; i--;) {
    error[i] = 0.0;
  }
  // Loop on the final pixels of the surface
  VecShort2D p = VecShortCreateStatic2D();
  do {
    // Get the pixel
    GBPixel* pix = GBSurfaceFinalPixel(that, &p);
    // Get the index of the current pixel
    long ip = GBPosIndex(&p, GBSurfaceDim(that)) * 4;
    // For each channel of the pixel
    for (int iRgb = 4; iRgb--;) {
      // Get the corrected value of the pixel for this channel
      float corrVal = (float)(pix->_rgba[iRgb]) - error[ip + iRgb];
      // Get the new value of the pixel for this channel
      unsigned char newVal = 0;
      if (corrVal > 127) {
        newVal = 255;
      }
      // Get the error on this pixel
      float err = ((float)newVal - (float)(pix->_rgba[iRgb])) / 16.0;
      // Propagate error to neighbours
      VecShort2D q = p;
      VecSet(&q, 1, VecGet(&p, 1) + 1);
      if (GBSurfaceIsPosInside(that, &q)) {
        long iq = GBPosIndex(&q, GBSurfaceDim(that)) * 4 + iRgb;
        error[iq] += err * 7.0;
      }
      VecSet(&q, 0, VecGet(&q, 0) + 1);
      if (GBSurfaceIsPosInside(that, &q)) {
        long iq = GBPosIndex(&q, GBSurfaceDim(that)) * 4 + iRgb;
        error[iq] += err * 1.0;
      }
      VecSet(&q, 1, VecGet(&q, 1) - 1);
      if (GBSurfaceIsPosInside(that, &q)) {
        long iq = GBPosIndex(&q, GBSurfaceDim(that)) * 4 + iRgb;
        error[iq] += err * 5.0;
      }
      VecSet(&q, 1, VecGet(&q, 1) - 1);
      if (GBSurfaceIsPosInside(that, &q)) {
        long iq = GBPosIndex(&q, GBSurfaceDim(that)) * 4 + iRgb;
        error[iq] += err * 3.0;
      }
      // Update the value of the pixel for this channel
      pix->_rgba[iRgb] = newVal;
    }
  } while (VecStep(&p, GBSurfaceDim(that)));
  // Free memory
  free(error);
}

// ---------------- GBSurfaceImage --------------------------

// Create a new GBSurfaceImage with dimension 'dim'
GBSurfaceImage* GBSurfaceImageCreate(const VecShort2D* const dim) {
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
GBSurfaceImage* GBSurfaceImageClone(const GBSurfaceImage* const that) {
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
bool GBSurfaceImageSave(const GBSurfaceImage* const that) {
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
bool GBSurfaceImageSaveAsTGA(const GBSurfaceImage* const that) {
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
  h._yOrigin = 0;
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
GBSurfaceImage* GBSurfaceImageCreateFromFile(
  const char* const fileName) {
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
GBSurfaceImage* GBSurfaceImageCreateFromTGA(const char* const fileName) {
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
GBEye GBEyeCreateStatic(const GBEyeType type) {
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
void GBEyeFreeStatic(GBEye* const that) {
  if (that == NULL)
    return;
  // Free memory
  MatFree(&(that->_proj));
}

// Update the projection matrix of the GBEye according to scale, rot
// and origin
void GBEyeUpdateProj(GBEye* const that) {
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
VecFloat* GBEyeGetProjectedPoint(const GBEye* const that, 
  const VecFloat* const point) {
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
  for (long i = MIN(VecGetDim(point), 3); i--;)
    VecSet(&v, i, VecGet(point, i));
  // Perform the product
  VecFloat* w = MatGetProdVec(that->_proj, &v);
  // Translate to origin
  for (int i = 2; i--;)
    VecSet(w, i, VecGet(w, i) + VecGet(&(that->_orig), i));
  // Copy the values into the result
  for (long i = MIN(VecGetDim(point), 3); i--;)
    VecSet(ret, i, VecGet(w, i));
  // Free the memory used for the product
  VecFree(&w);
  // Return the result
  return ret;
}
  
// Return the projection through the GBEye 'that' of the SCurve 'curve' 
SCurve* GBEyeGetProjectedCurve(const GBEye* const that, 
  const SCurve* const curve) {
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
Shapoid* GBEyeGetProjectedShapoid(const GBEye* const that, 
  const Shapoid* const shap) {
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
  GSetVecFloat* set = GSetVecFloatCreate();
  for (int iAxis = ShapoidGetDim(ret); iAxis--;) {
    proj = GBEyeGetProjectedPoint(that, ShapoidAxis(ret, iAxis));
    GSetPush(set, proj);
  }
  ShapoidSetAllAxis(ret, set);
  while (GSetNbElem(set) > 0) {
    proj = GSetPop(set);
    VecFree(&proj);
  }
  GSetFree(&set);
  // Return the new shapoid
  return ret;
}
  
// ---------------- GBEyeOrtho --------------------------

// Return a new GBEyeOrtho with orientation 'view'
// scale is initialized to (1,1), trans to (0,0) and rot to 0
GBEyeOrtho* GBEyeOrthoCreate(const GBEyeOrthoView view) {
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
void GBEyeOrthoGetProjVal(const GBEyeOrtho* const that, 
  float* const val) {
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
void GBEyeOrthoProcess(const GBEyeOrtho* const that, 
  GBObjPod* const pod) {
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
void GBEyeIsometricProcess(const GBEyeIsometric* const that, 
  GBObjPod* const pod) {
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
void GBEyeIsometricGetProjVal(const GBEyeIsometric* const that, 
  float* val) {
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
GBHand GBHandCreateStatic(const GBHandType type) {
  // Declare the new hand
  GBHand hand;
  // Set properties
  hand._type = type;
  // Return the new hand
  return hand;
}

// Free the memory used by the GBHand 'that'
void GBHandFreeStatic(GBHand* const that) {
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
void GBHandDefaultProcess(const GBHandDefault* const that, 
  GBObjPod* const pod) {
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
  if (GBObjPodEyeObj(pod) == NULL) {
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
  while (GSetNbElem(setSCurves) > 0) {
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
GBTool GBToolCreateStatic(const GBToolType type) {
  // Declare the GBTool
  GBTool that;
  // Set properties
  that._type = type;
  // Return the tool
  return that;
}

// Free the memory used by the GBTool 'that'
void GBToolFreeStatic(GBTool* const that) {
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
void GBToolPlotterDraw(const GBToolPlotter* const that, 
  const GBObjPod* const pod) {
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
void GBToolPlotterDrawPoint(const GBToolPlotter* const that, 
  const VecFloat*  const point, const GBObjPod*  const pod) {
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
  GBPixel pixColor = GBInkGet(GBObjPodInk(pod), that, pod, &null, 
    point, &posLayer);
  // Get the depth
  float depth = 0.0;
  if (VecGetDim(point) > 2)
    depth = VecGet(point, 2);
  // Add the pixel at the point position in the layer
  GBLayerAddPixelSafe(GBObjPodLayer(pod), &posLayer, 
    &pixColor, depth); 
}
  
// Draw the Shapoid 'shap' in the GBObjPod 'pod' with the 
// GBToolPlotter 'that'
void GBToolPlotterDrawShapoid(const GBToolPlotter* const that, 
  const Shapoid* const shap, const GBObjPod* const pod) {
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
        (short)round(MAX(ShapoidPosGet(bound, iAxis), 0.0)));
      short v = (short)round(ShapoidPosGet(bound, iAxis) + 
        ShapoidAxisGet(bound, iAxis, iAxis)) + 1;
      VecSet(to, iAxis, 
        MIN(v, VecGet(GBLayerDim(GBObjPodLayer(pod)),iAxis)));
    } else {
      VecSet(from, iAxis, 
        (short)round(ShapoidPosGet(bound, iAxis)));
      short v = (short)round(ShapoidPosGet(bound, iAxis) + 
        ShapoidAxisGet(bound, iAxis, iAxis)) + 1;
      VecSet(to, iAxis, v);
    }
  }
  // Loop on pixels in layers inside the bounding box
  VecShort* posLayer = VecClone(from);
  VecFloat* posLayerFloat = VecFloatCreate(VecGetDim(posLayer));
  do {
    for (long iAxis = VecGetDim(posLayer); iAxis--;) {
      VecSet(posLayerFloat, iAxis, 
        (float)VecGet(posLayer, iAxis) + 0.5);
    }
    // If this pixel is inside the Shapoid
    if (ShapoidIsPosInside(shap, posLayerFloat)) {
      // Get the current internal coordinates
      VecFloat* posIn = ShapoidImportCoord(shap, posLayerFloat);
      // Get the current external coordinates
      VecFloat* posExt = 
        ShapoidExportCoord(GBObjPodGetObjAsShapoid(pod), posIn);
      // Get the ink at this position
      GBPixel pixColor = GBInkGet(GBObjPodInk(pod), that, pod, 
        posIn, posExt, posLayer);
      // Get the depth
      float depth = 0.0;
      if (VecGetDim(posLayerFloat) > 2)
        depth = VecGet(posLayerFloat, 2);
      // Add the pixel to the layer
      GBLayerAddPixelSafe(GBObjPodLayer(pod), (VecShort2D*)posLayer, 
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
void GBToolPlotterDrawSCurve(const GBToolPlotter* const that, 
  const SCurve* const curve, const GBObjPod* const pod) {
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
    for (long iAxis = VecGetDim(posLayer); iAxis--;)
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
      GBPixel pixColor = GBInkGet(GBObjPodInk(pod), that, pod, 
        posIn, posExt, posLayer);
      // Get the depth
      float depth = 0.0;
      if (VecGetDim(posLayerFloat) > 2)
        depth = VecGet(posLayerFloat, 2);
      // Add the pixel to the layer
      GBLayerAddPixelSafe(GBObjPodLayer(pod), (VecShort2D*)posLayer, 
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
GBPixel _GBInkGet(const GBInk* const that, const GBTool* const tool, 
  const GBObjPod* const pod, const VecFloat* const posInternal, 
  const VecFloat* const posExternal, const VecShort* const posLayer) {
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
void GBInkFree(GBInk* const that) {
  // Nothing to do
  (void)that;
}

// ---------------- GBInkSolid --------------------------

// Create a new GBInkSolid with color 'col'
GBInkSolid* GBInkSolidCreate(const GBPixel* const col) {
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
GBObjPod* _GBObjPodCreatePoint(VecFloat* const pos, GBEye* const eye, 
  GBHand* const hand, GBTool* const tool, GBInk* const ink, 
  GBLayer* const layer) {
#if BUILDMODE == 0
  if (pos == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'pos' is null");
    PBErrCatch(GenBrushErr);
  }
  if (VecGetDim(pos) < 2) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, 
    "'pos' 's dimension is invalid (%ld>=2)",
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
  GBObjPodProcess(pod);
  // Return the new pod
  return pod;
}

// Create a new GBObjPod for the Shapoid 'shap'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'shap' 's dimension must be 2 or more
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreateShapoid(Shapoid* const shap, GBEye* const eye, 
  GBHand* const hand, GBTool* const tool, GBInk* const ink, 
  GBLayer* const layer) {
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
  GBObjPodProcess(pod);
 // Return the new pod
  return pod;
}

// Create a new GBObjPod for the SCurve 'curve'
// drawn with 'eye', 'hand', 'tool' and 'ink' in layer 'layer'
// 'curve' 's dimension must be 2 or more
// If 'eye', 'hand, 'tool', 'ink' or 'layer' is null return null
GBObjPod* _GBObjPodCreateSCurve(SCurve* const curve, GBEye* const eye, 
  GBHand* const hand, GBTool* const tool, GBInk* const ink, 
  GBLayer* const layer) {
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
  GBObjPodProcess(pod);
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
GenBrush* GBCreateImage(const VecShort2D* const dim) {
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
  ret->_postProcs = GSetCreateStatic();
  // Return the new GenBrush
  return ret;
}

// Create a new GenBrush with one layer containing the content 
// of the image located at 'fileName' and dimensions equal to the 
// dimensions of the image
// If the image couldn't be loaded return NULL
GenBrush* GBCreateFromFile(const char* const fileName) {
  // Declare the new GenBrush
  GenBrush* ret = NULL;
  // Load the image in a surface
  GBSurfaceImage *img = GBSurfaceImageCreateFromFile(fileName);
  if (img != NULL) {
    // Declare the new GenBrush
    ret = PBErrMalloc(GenBrushErr, sizeof(GenBrush));
    // Set properties
    ret->_surf = (GBSurface*)img;
    ret->_pods = GSetCreateStatic();
    ret->_postProcs = GSetCreateStatic();
    GBSetFileName(ret, fileName);
  }
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
  GBRemoveAllPod(*that);
  GBRemoveAllPostProcess(*that);
  free(*that);
  *that = NULL;
}

// Get the dimensions of the GenBrush 'that'
VecShort2D* GBDim(const GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  return GBSurfaceDim(that->_surf);
}

// Update the GBSurface of the GenBrush 'that'
void GBUpdate(GenBrush* const that) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Refresh the layers that have been modified
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
  // Apply the post processing
  if (GSetNbElem(GBPostProcs(that)) > 0) {
    GSetIterForward iter = 
      GSetIterForwardCreateStatic(GBPostProcs(that));
    do {
      GBPostProcessing* post = GSetIterGet(&iter);
      GBSurfacePostProcess(GBSurf(that), post);
    } while (GSetIterStep(&iter));
  }
}

// Update the content of the layer 'layer' of the GenBrush 'that'
// Flush the content of the layer and redraw all the pod attached to it
void GBUpdateLayer(GenBrush* const that, GBLayer* const layer) {
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
      GBToolDraw(GBObjPodTool(pod), pod);
  } while (GSetIterStep(&iter));
}

// Render the GBSurface (save on disk, display on screen, ...) of
// the GenBrush 'that'
bool GBRender(GenBrush* const that) {
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
void _GBRemovePod(GenBrush* const that, const void* const obj, 
  const GBEye* const eye, const GBHand* const hand, 
  const GBTool* const tool, const GBInk* const ink, 
  const GBLayer* const layer) {
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
void _GBSetPodEye(GenBrush* const that, GBEye* const toEye, 
  const void* const obj, const GBEye* const eye, 
  const GBHand* const hand, const GBTool* const tool, 
  const GBInk* const ink, const GBLayer* const layer) {
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
void _GBSetPodHand(GenBrush* const that, GBHand* const toHand, 
  const void* const obj, const GBEye* const eye, 
  const GBHand* const hand, const GBTool* const tool, 
  const GBInk* const ink, const GBLayer* const layer) {
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
void _GBSetPodTool(GenBrush* const that, GBTool* const toTool, 
  const void* const obj, const GBEye* const eye, 
  const GBHand* const hand, const GBTool* const tool, 
  const GBInk* const ink, const GBLayer* const layer) {
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
void _GBSetPodInk(GenBrush* const that, GBInk* const toInk, 
  const void* const obj, const GBEye* const eye, 
  const GBHand* const hand, const GBTool* const tool, 
  const GBInk* const ink, const GBLayer* const layer) {
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
void _GBSetPodLayer(GenBrush* const that, GBLayer* const toLayer, 
  const void* const obj, const GBEye* const eye, 
  const GBHand* const hand, const GBTool* const tool, 
  const GBInk* const ink, const GBLayer* const layer) {
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
void _GBNotifyChangeFromObj(GenBrush* const that, 
  const void* const obj) {
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
    if (GBObjPodObj(pod) == obj) {
      // Reprocess the object
      GBObjPodProcess(pod);
      // Set the layer flag
      GBLayerSetModified(pod->_layer, true);
    }
  } while (GSetIterStep(&iter));
}

// Set to true the modified flag of the layers of pods attached to the
// GBEye 'eye' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromEye(GenBrush* const that, 
  const GBEye* const eye) {
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
    if (GBObjPodEye(pod) == eye) {
      // Reprocess the object
      GBObjPodProcess(pod);
      // Set the layer flag
      GBLayerSetModified(pod->_layer, true);
    }
  } while (GSetIterStep(&iter));
}

// Set to true the modified flag of the layers of pods attached to the
// GBInk 'ink' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromInk(GenBrush* const that, 
  const GBInk* const ink) {
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
void _GBNotifyChangeFromHand(GenBrush* const that, 
  const GBHand* const hand) {
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
    if (GBObjPodHand(pod) == hand) {
      GBHandProcess(GBObjPodHand(pod), pod);
      // Set the layer flag
      GBLayerSetModified(pod->_layer, true);
    }
  } while (GSetIterStep(&iter));
}

// Set to true the modified flag of the layers of pods attached to the
// GBTool 'tool' in the list of pods of the GenBrush 'that'
void _GBNotifyChangeFromTool(GenBrush* const that, 
  const GBTool* const tool) {
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

// Return a clone of the GenBrush 'that' with its final surface scaled
// to the dimensions 'dim' according to the scaling method 'scaleMethod'
GenBrush* GBScale(const GenBrush* const that, 
  const VecShort2D* const dim, const GBScaleMethod scaleMethod) {
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
  // Declare the scaled version of the GenBrush
  GenBrush* scaledGB = NULL;
  // Call the appropriate scaling method
  switch (scaleMethod) {
    case GBScaleMethod_AvgNeighbour:
      scaledGB = GBScaleAvgNeighbour(that, dim);
      break;
    default:
      break;
  }
  // Return the scaled version of the GenBrush
  return scaledGB;
}

// Return a clone of the GenBrush 'that' with its final surface flipped
// relatively to the 'iAxis'-th axis
GenBrush* GBFlip(const GenBrush* const that, 
  const long iAxis) {
#if BUILDMODE == 0
  if (that == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'that' is null");
    PBErrCatch(GenBrushErr);
  }
  if (iAxis < 0 || iAxis > 1) {
    GenBrushErr->_type = PBErrTypeInvalidArg;
    sprintf(GenBrushErr->_msg, "'iAxis' is invalid (0<=%ld<=1)",
      iAxis);
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare the flipped version of the GenBrush
  GenBrush* flippedGB = GBCreateImage(GBDim(that));
  // Loop on the pixels of the final surface
  VecShort2D pos = VecShortCreateStatic2D();
  do {
    // Get the pixel in the original image
    GBPixel* pix = 
      GBSurfaceFinalPixel(GBSurf(that), &pos);
    // Get the position in the flipped GenBrush
    VecShort2D posFlipped = pos;
    VecSet(&posFlipped, iAxis,
      VecGet(GBDim(that), iAxis) - VecGet(&pos, iAxis) - 1);
    // Get the pixel in the flipped image
    GBPixel* pixFlipped = 
      GBSurfaceFinalPixel(GBSurf(flippedGB), &posFlipped);
    // Set the pixel in the flipped version of the GenBrush
    *pixFlipped = *pix;
  } while (VecStep(&pos, GBDim(that)));

  // Return the flipped version of the GenBrush
  return flippedGB;
}


// Return a clone of the GenBrush 'that' with its final surface scaled
// to the dimensions 'dim' according to the scaling method AvgNeighbour
GenBrush* GBScaleAvgNeighbour(const GenBrush* const that, 
  const VecShort2D* const dim) {
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
  // Declare the scaled version of the GenBrush
  GenBrush* scaledGB = GBCreateImage(dim);
  // Calculate the scale factors along each axis
  VecFloat2D scaleFactor = VecFloatCreateStatic2D();
  VecSet(&scaleFactor, 0, 
    (float)VecGet(dim, 0) / (float)VecGet(GBDim(that), 0));
  VecSet(&scaleFactor, 1, 
    (float)VecGet(dim, 1) / (float)VecGet(GBDim(that), 1));
  // Declare variables to calculate the average of pixels in the 
  // original image
  int nbPix = 0;
  VecFloat* avgPix = VecFloatCreate(4);
  // Declare a variable to loop on the scaled image
  VecShort2D posScaled = VecShortCreateStatic2D();
  // Loop on pixels of the scaled image
  do {
    // Get the floating point position in the original image of
    // the lower left and upper right of the 3x3 cell around for this
    // pixel
    VecFloat2D posLLFloat = VecFloatCreateStatic2D();
    VecSet(&posLLFloat, 0, 
      (float)(VecGet(&posScaled, 0) - 1) / VecGet(&scaleFactor, 0));
    VecSet(&posLLFloat, 1, 
      (float)(VecGet(&posScaled, 1) - 1) / VecGet(&scaleFactor, 1));
    VecFloat2D posURFloat = VecFloatCreateStatic2D();
    VecSet(&posURFloat, 0, 
      (float)(VecGet(&posScaled, 0) + 1) / VecGet(&scaleFactor, 0));
    VecSet(&posURFloat, 1, 
      (float)(VecGet(&posScaled, 1) + 1) / VecGet(&scaleFactor, 1));
    // Get the integer position in the original image of
    // the lower left and upper right of the 3x3 cell around for this
    // pixel
    // Ensure the positions are inside the original image
    VecShort2D posLL = VecShortCreateStatic2D();
    VecSet(&posLL, 0, MAX(0, (short)floor(VecGet(&posLLFloat, 0))));
    VecSet(&posLL, 1, MAX(0, (short)floor(VecGet(&posLLFloat, 1))));
    VecShort2D posUR = VecShortCreateStatic2D();
    VecSet(&posUR, 0, 
      MIN(VecGet(GBDim(that), 0), (short)floor(VecGet(&posURFloat, 0))));
    VecSet(&posUR, 1, 
      MIN(VecGet(GBDim(that), 1), (short)floor(VecGet(&posURFloat, 1))));
    // Init the variables to calculate the average of pixels in the 
    // original image
    nbPix = 0;
    VecSetNull(avgPix);
    // Declare a variable to loop on the original image
    VecShort2D posOrig = posLL;
    // Loop on the pixels of the original image
    do {
      // Get the pixel in the original image
      const GBPixel* pix = GBSurfaceFinalPixel(GBSurf(that), &posOrig);
      // Calculate the average pixel
      ++nbPix;
      for (int i = 4; i--;)
        VecSet(avgPix, i, VecGet(avgPix, i) + (float)(pix->_rgba[i]));
    } while (VecShiftStep(&posOrig, &posLL, &posUR));
    // Calculate the average pixel
    VecScale(avgPix, 1.0 / (float)nbPix);
    // Get the pixel in the original image
    GBPixel* pixScaled = 
      GBSurfaceFinalPixel(GBSurf(scaledGB), &posScaled);
    // Update the pixel in the scaled image with the average pixel
    for (int i = 4; i--;)
      pixScaled->_rgba[i] = (unsigned char)VecGet(avgPix, i);
  } while (VecStep(&posScaled, dim));
  // Free memory
  VecFree(&avgPix);
  // Return the scaled version of the GenBrush
  return scaledGB;
}

// Return a clone of the GenBrush 'that' with its final surface cropped
// to the dimensions 'dim' from the lower right position 'posLR'
// If the cropping area is partially or totally outside of the 
// original image, pixels outside of the image are filled with 
// 'fillPix' or left to their default value (cf GBSurfaceCreate) if
// 'fillPix is NULL
GenBrush* GBCrop(const GenBrush* const that, 
  const VecShort2D* const posLR, const VecShort2D* const dim,
  const GBPixel* const fillPix) {
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
  // Declare the cropped version of the GenBrush
  GenBrush* croppedGB = GBCreateImage(dim);
  // Declare a variable to loop on the cropped image
  VecShort2D posCropped = VecShortCreateStatic2D();
  // Loop on pixels of the scaled image
  do {
    // Get the position in the original image
    VecShort2D posOrig = VecGetOp(posLR, 1, &posCropped, 1);
    // Get the pixel in the original image
    const GBPixel* pixOrig = 
      GBSurfaceFinalPixelSafe(GBSurf(that), &posOrig);
    // If the pixel is inside the image
    if (pixOrig != NULL) {
      // Copy the pixel in the cropped image
      GBSurfaceSetFinalPixel(GBSurf(croppedGB), &posCropped, pixOrig);
    } else if (fillPix != NULL) {
      // Copy the filling pixel in the cropped image
      GBSurfaceSetFinalPixel(GBSurf(croppedGB), &posCropped, fillPix);
    }
  } while (VecStep(&posCropped, dim));
  // Return the cropped version of the GenBrush
  return croppedGB;
}

// Duplicate the final pixels of the GenBrush 'src' to the 
// GenBrush 'dest' for the area starting at 'posSrc' in 'src' and
// 'posDest' in 'dest' and having dimension 'dim'
// The fragment must be fully included in both 'src' and 'dest'
void GBCopyFragment(const GenBrush* const src, GenBrush* const dest,
  const VecShort2D* const posSrc, const VecShort2D* const posDest, 
  const VecShort2D* const dim) {
#if BUILDMODE == 0
  if (src == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'src' is null");
    PBErrCatch(GenBrushErr);
  }
  if (dest == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'dest' is null");
    PBErrCatch(GenBrushErr);
  }
  if (posSrc == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'posSrc' is null");
    PBErrCatch(GenBrushErr);
  }
  if (posDest == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'posDest' is null");
    PBErrCatch(GenBrushErr);
  }
  if (dim == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'dim' is null");
    PBErrCatch(GenBrushErr);
  }
#endif
  // Declare a variable to loop on pixels
  VecShort2D pos = VecShortCreateStatic2D();
  
  // Loop on pixels
  do {

    // Get the position in the source
    VecShort2D pSrc = VecGetOp(&pos, 1, posSrc, 1);

    // Get the position in the destination
    VecShort2D pDest = VecGetOp(&pos, 1, posDest, 1);

    // Get the pixel from the source
    GBPixel pix = GBGetFinalPixel(src, &pSrc);
    
    // Set the pixel in the destination
    GBSetFinalPixel(dest, &pDest, &pix);

  } while (VecStep(&pos, dim));
}

// Return a value between 0.0 and 1.0, such as 1.0 means 'gbA' and 'gbB'
// are identical, 0.0 means they are completely different
// The value is calculated as the average of the difference between
// rgba of final pixels of 'gbA' and final pixels of 'gbB'
// 'gbA' and 'gbB' must be of same dimensions
float GBGetSimilarity(const GenBrush* const gbA, GenBrush* const gbB) {
#if BUILDMODE == 0
  if (gbA == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'gbA' is null");
    PBErrCatch(GenBrushErr);
  }
  if (gbB == NULL) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, "'gbB' is null");
    PBErrCatch(GenBrushErr);
  }
  if (VecIsEqual(GBDim(gbA), GBDim(gbB)) == false) {
    GenBrushErr->_type = PBErrTypeNullPointer;
    sprintf(GenBrushErr->_msg, 
      "'gbA' and 'gbB' have different dimensions");
    PBErrCatch(GenBrushErr);
  }
#endif

  // Declare a variable to memorize the similarity
  float similarity = 0.0;

  // Declare a variable to loop on pixels
  VecShort2D pos = VecShortCreateStatic2D();
  
  // Loop on pixels
  do {

    // Get the pixels
    GBPixel pixA = GBGetFinalPixel(gbA, &pos);
    GBPixel pixB = GBGetFinalPixel(gbB, &pos);
    
    // Update the similarity
    float diff = 0.0;
    for (int iRgba = 4; iRgba--;)
      diff += 
        fabs(
          ((float)(pixA._rgba[iRgba]) - 
          (float)(pixB._rgba[iRgba])) / 255.0);
    similarity += diff / 4.0;

  } while (VecStep(&pos, GBDim(gbA)));

  // Calculate the average of similarity
  similarity = 1.0 - similarity / GBArea(gbA);

  // Return the similarity
  return similarity;

}
    
// ================ GTK Functions ====================
#if BUILDWITHGRAPHICLIB == 1
  #include "genbrush-GTK.c"
#endif
