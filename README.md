# GenBrush

GenBrush is a C library providing structures and functions for creating 2D/3D, bitmap/vector, still/animated graphics on various supports.

It can be use as a monolithic library, or as an interface with another graphical library (Cairo/GTK). The choice can be made through a single compilation option.

GenBrush is based on the following model: the scene to be represented graphically is composed of Objects viewed by an Eye, interpreted by a Hand and recreated with a Tool applying an Ink on a Surface made of Layers.

The currently available implementations for these entities are:
* Object: Point (VecFloat), SCurve, Shapoid
* Eye: Orthographic, Isometric
* Hand: Default
* Tool: PixelPlotter
* Ink: Solid
* Surface: Image (TGA), GBApp, GBWidget
* Layer: divided into background, inside and foreground layers; blending modes: Default (overwritting), Normal, Over
For details refer to the following sections.

The library can easily be extended to match the user needs. Foreseen extensions of the entities are for example:
* Object: BBody, ...
* Eye: Perspective, ...
* Hand: Human (approximation of the viewed object), ...
* Tool: Pen, Brush, ...
* Ink: Generic (BBody), UVMapping
* Surface: Full support of TGA, other image formats, innterface with Cairo only, ...
* Layer: other blending modes, ...

A graphic can then be created with the GenBrush library by describing the scene as a set of Pods, which are combinations of an Object, an Eye, a Hand, a Tool, an Ink and a Layer. The rendering of each Pod processes sequentially the original Object through the other entities to generates pixels into the Layer. Once all the Pods have been processed, the Layers are blended to generate RGBA final pixels in the Surface. The scene description can be modified after creation. Pods use references to entities, meaning that if a parameter of, for example, an Ink is modified all the Pods attached to this Ink will be recalculated at the next rendering.

Several rendering of the same scene are optimized to recalculate only the final pixels affected by the entities which has been modified since the previous rendering. It is the reponsibility of the user to notify GenBrush when an entity has been modified.

The user can apply post processing to the surface at the end of each rendering. Currently implemented post processing are:
* Normalization by hue
* Ordered dithering
* Floyd-Steinberg dithering

Other functionalities include:
* scaling
* cropping

Example code:
* ImageViewer: display a tga image in a window.

The GenBrush library uses the ```PBErr```, ```PBMath```, ```GSet```, ```Shapoid```, ```BCurve``` libraries, and GTK if compiled as an interface for this library.

## How to install this repository
1) Create a directory which will contains this repository and all the repositories it is depending on. Lets call it "Repos"
2) Download the master branch of this repository into "Repos". Unzip it if necessary.
3) The folder's name for the repository should be followed by "-master", rename the folder by removing "-master".
4) cd to the repository's folder
5) If wget is not installed: ```sudo apt-get update && sudo apt-get install wget``` on Ubuntu 16.04, or ```brew install wget``` on Mac OSx
6) If gawk is not installed: ```sudo apt-get update && sudo apt-get install gawk```  on Ubuntu 16.04, ```brew install gawk``` on Mac OSx
7) If this repository is the first one you are installing in "Repos", run the command ```make -k pbmake_wget```
8) Run the command ```make``` to compile the repository. 
9) Eventually, run the command ```main``` to run the unit tests and check everything is ok.
10) Refer to the documentation to learn how to use this repository.

The dependancies to other repositories should be resolved automatically and needed repositories should be installed in the "Repos" folder. However this process is not completely functional and some repositories may need to be installed manually. In this case, you will see a message from the compiler saying it cannot find some headers. Then install the missing repository with the following command, e.g. if "pbmath.h" is missing: ```make pbmath_wget```. The repositories should compile fine on Ubuntu 16.04. On Mac OSx, there is currently a problem with the linker.
If you need assistance feel free to contact me with my gmail address: at bayashipascal.
