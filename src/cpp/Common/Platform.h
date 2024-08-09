#ifndef PlatformH
#define PlatformH

//
// Compiler determination
//
#if defined(__IBMC__) || defined(__IBMCPP__)
  #error("PPTactical does not have support for IBM compilers")
   #define PPT_COMPILER_IBM 1 // IBM shit (should we bother to check?)
   #define PPT_COMPILER "IBM"
#endif // IBM

#ifdef _MSC_VER
   #define PPT_COMPILER_MICROSOFT 1    // Microsoft Visual C++
   #define PPT_COMPILER "MICROSOFT"
#endif // MICROSOFT

#ifdef __BORLANDC__
   #define PPT_COMPILER_BORLAND 1      // Borland C++
   #define PPT_COMPILER "Borland"
#endif // BORLAND

#ifdef __GNUC__
   #define PPT_COMPILER_GNU 1          // GNU gcc/g++
   #define PPT_COMPILER "GNU"
#endif // GNU

#ifdef __SUNPRO_CC
  #define PPT_COMPILER_SUNFORTE 1
  #define PPT_COMPILER "Sun Forte"
#endif

#ifdef __MWERKS__
  #error("PPTactical does not have support for Metrowerks")
  #define PPT_COMPILER_MWERKS 1
  #define PPT_COMPILER "Metrowerks Codewarrior"
#endif

//
// Platform determination
//
#if PPT_COMPILER_IBM
   #ifdef __OS2__
      #define PPT_PLATFORM_OS2 1
      #define PPT_PATHSEPARATOR '\\'
   #endif
   #ifdef __WINDOWS__
      #define PPT_PLATFORM_WIN 1
      #define PPT_PATHSEPARATOR '\\'
   #endif
#endif

#if PPT_COMPILER_MICROSOFT
   #define PPT_PLATFORM_WIN 1
   #define PPT_PATHSEPARATOR '\\'
#endif

// also check for Kylix3 here...
#if PPT_COMPILER_BORLAND
   #if defined (__WIN32__) || defined(_Windows)
     #define PPT_PLATFORM_WIN 1
     #define PPT_PATHSEPARATOR '\\'
   #endif

   #if defined(__linux__)
     #defined PPT_PLATFORM_LINUX 1
     #define PPT_PATHSEPARATOR '/'
   #endif
#endif

#if PPT_COMPILER_SUNFORTE
  #define PPT_PLATFORM_SOLARIS 1
  #define PPT_PATHSEPARATOR '/'
#endif

// The Gnu compiler may live on different platforms...
#if PPT_COMPILER_GNU
   #if defined(__WIN32__) || defined(WIN32) || defined(_WIN32) || defined(__CYGWIN32__)
      #define PPT_PLATFORM_WIN 1
      #define PPT_PATHSEPARATOR '\\'
   #else
      #if defined(__linux__)
         #define PPT_PLATFORM_LINUX 1
      #elif defined(__NetBSD__)
         #define PPT_PLATFORM_NETBSD 1
         #define PPT_PLATFORM_BSD 1
      #elif defined(__FreeBSD__)
         #define PPT_PLATFORM_FREEBSD 1
         #define PPT_PLATFORM_BSD 1
      #elif defined(__APPLE__)
         #define PPT_PLATFORM_MACOSX 1
         #define PPT_PLATFORM_BSD 1
         #define PPT_BIGENDIAN_IO 1
      #elif defined(sparc) && defined(sun) && defined(unix)
         #define PPT_PLATFORM_SOLARIS 1
      #else
         #define PPT_PLATFORM_UNIX 1
      #endif
      #define PPT_FAMILY_UNIX 1
      #define PPT_PATHSEPARATOR '/'
      #define _MAX_PATH 2048
   #endif
#endif

// set the platform endianess (so that the data files which are in
// little endian format) may be read correctly
// #define PPT_BIGENDIAN_IO 1

#endif // Platform
