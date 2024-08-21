# Piron Games C++ GameToolkit

This is a game toolkit that was used for development of several games and game tools by Piron Games. 

Its development started in 1998 as part of the [Pure Power](https://www.pirongames.com/games/ppower/ppower.htm) project, using C++ Builder 5 and 6. This is the reason why you will still find remnants of C++ Builder specific code in the core part :)

From 2001 onward, development continued as part of the [Pure Power Tactical Engine](http://sourceforge.net/projects/pptactical/) project.

Starting from 2003, more game tools were added, which helped with the development of CyberNinja/Lethal Metal (2003-2005), as well as Flash/HTML5/Haxe games (2008-2018)

This project is in a half-frozen state and further advancements are tied to the PPTactical Engine project. It is released for historical reasons, as currently there are much better options out there to choose from.

The toolkit has a core (or Common) part and a tools part, which are described below.

## Gametoolkit Core

This contains systems to develop games and tools. Note that some of the classes provided were originally developed for C++98, when the language lacked built-in libraries.

The notable parts are:

* System: a wrapper over various interfaces/libraries like SDL/DirectX (Allegro was, at some point, planned to be added). These are, in theory, exchangeable.
* GUI: a GUI library, providing a framework and graphical elements to develop any stand-alone graphical applications.
* MiniCSL: a scripting language, a reduced version of CSL (C Scripting Language by Peter Koch), which is seamlessly integrated into the toolkit.
* String Manager: support for localization.
* Virtual File System: it is possible to mount various methods, like directories or archieves (zip, wad, etc). This part uses the physfs library.
* Fx: support for full screen effects (software)
* Math: along with the usual math support, there's also fixed math and curves (including scripted through MiniCSL curves)
* Mot: a particle effect system
* GLoad: support to load various graphical files and to convert between internal formats. Some of these formats are long obsolete, with only PNG/JPG/BMP still applicable in the modern world.
* Terrain Generator/Eco System Generator: exactly what their names are. for 2D games only.

## Core Issues

File access is not uniform and it uses C and C++ standard API, like handles, FILE* and streams. Goal would be to have everything as a file stream.

String handling is not uniform: CPString (our custom string implementation done cca 1998), ZString (CSL) and std::string. Goal is to use std::string everywhere.

I favour UTF-8, so the core is mostly UTF-8 "aware". There are two exceptions, however:
* CSL: it uses ZString internally, and while similar to std::string, it may not parse UTF8 correctly. It is advised to keep all CSL scripts (if using external files) as ANSI. Internally, the tools that operate on scripts are converting everything to ANSI;
* file access: it uses various APIs which don't natively support UTF-8 for file names parameters

## Tools

The graphical tools were developed using C++ Builder and I've continued to maintain them through-out versions of it. Currently, they build with C++ Builder 11.

In hind-sight, it wasn't very clever to tie them up with a proprietary software/graphical library like C++ Builder. However, in 1998, there weren't a lot of options that allowed for fast development of graphical applications. We were only 3 developers working on Pure Power and we needed to expand the game tooling in a short period of time, so C++ Builder helped a lot. After all, it's not called RAD for nothing ;)

The following tools are included:

* Sentry: a full-featured sprite editor (see separate chapter below)
* StringTool: convert/transform a localization XML to a custom format by means of a CSL script (usable for games where parsing a xml would otherwise not be available/efficient - J2ME, Flash games)
* Curve Editor: a graphical wrapper over the curve library, with support to visualize and save a "library" of curves
* Data Growth: support to edit data packages (for J2ME games). Has both a graphical mode and a command-line mode 
* JadUpdate: small command line application to update versions in jad/manifest files (for J2ME games)
* SprEd: a sprite editor originally used by Pure Power and PPTactical Engine (obsolete, superseded by Sentry)
* XSprite: a sprite editor used by Lethal Metal/CyberNinja (obsolete, superseded by Sentry)

Along with the tools, a couple of VCL components shared between them are available.

Sample test data for each of these tools is available in the [data](https://github.com/stefandee/gametoolkit/tree/main/data) folder.

## Sentry
It's an advanced sprite editor, written mostly for space-efficient games (browser, J2ME), but not limited to. It combines the experiences with XSprite/SprEd and sprite editors I came across while working in the mainstream game dev industry.

It was used to create the sprites for all the Flash/HTML5/Haxe games, although the format it exported to was not very efficient for HTML5. Plans to refactor PPTactical Engine to natively load Sentry sprites were made, but never implemented.

It supports defining static data (cells or modules), animations and logical data like collisions and hotspots.

It allows using CSL scripts to export from a sprite definition to a format usable for a specific application. Samples are available in [src/csl](https://github.com/stefandee/gametoolkit/tree/main/src/csl/Sentry) folder.

There are a few more features and fixes to be done, namely:
* command line export (.xsprite + csl script + parameters -> .bin)
* selection errors with zoom
* script: module optimization: split in 2x2 and ignore the empty modules
* script: frames from modules (create a frame for each module)
* multiple flogic support (circle, poly shape)
* color map: export

## Library Dependencies
The toolkit uses the following libraries, for convenience included in the [lib](https://github.com/stefandee/gametoolkit/tree/main/lib) folder:

* Physfs: virtual file system
* Easylogging++: file logger
* Tinyxml: xml parser
* Pnglib/zlib: for png support
* Pngdib: render png data in Windows GDI (used by some tools)

## Setup&Install&Build
Most of the code will compile with a C++11 compiler.

Most of the tools require C++ Builder 11. A project to build them all is located in the [dev/bcb](https://github.com/stefandee/gametoolkit/tree/main/dev/bcb) folder.

Some command line tools may be compiled with any compiler.

## Plans
Due to the time when it was developed, the toolkit and tools don't have (full) Unicode support. The goal is to make it UTF-8 compatible, which depends on replacing CPString and ZString with std::string, but this is an on-going effort which might not be achieved soon.

In parts, the code is still C++98. Gradual upgrading to latest standard and std library would be nice to have.

## Contributors
Stefan "Karg" Dicu, Tudor "grab" Garba, Mikael "tamlin" Nordell

## License

Code license:
https://opensource.org/licenses/MIT

Artwork license:
https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en