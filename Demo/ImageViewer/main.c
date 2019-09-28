#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include "genbrush.h"

int main(int argc, char** argv) {
  // Check if one argument has been given
  if (argc != 2) {
    printf("No file name given in argument.\n");
    return 1;
  }
  // Load the image as a GBSurfaceImage to get its dimensions
  GBSurfaceImage* image = GBSurfaceImageCreateFromFile(argv[1]);
  // If we couldn't load the image
  if (image == NULL) {
    printf("Couldn't load the image %s.\n", argv[1]);
    return 1;
  }
  // Declare variable to memorize the title of the app
  char* title = "GenBrush: Image viewer";
  // Create the app with same dimensions as the image
  GenBrush* app = GBCreateApp(GBSurfaceDim((GBSurface*)image), title);
  // Steal the image content from the GBSurfaceImage to avoid 
  // reloading it
  GBPixel* tmp = ((GBSurface*)image)->_finalPix;
  ((GBSurface*)image)->_finalPix = GBSurf(app)->_finalPix;
  GBSurf(app)->_finalPix = tmp;
  // Render the app
  bool status = GBRender(app);
  // If the render of the app failed
  if (status == false) {
    printf("Couldn't create the application.\n");
    return 1;
  }
  // Free the memory used by the app
  GBFree(&app);
  // Free the memory used by the image
  GBSurfaceImageFree(&image);
  // Return success code
  return 0;
}

