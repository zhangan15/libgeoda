/**
* Name:        wx/platform.h
* Purpose:     define the OS and compiler identification macros
* Author:      Vadim Zeitlin
* Modified by:
* Created:     29.10.01 (extracted from wx/defs.h)
* Copyright:   (c) 1997-2001 Vadim Zeitlin
* Licence:     wxWindows licence
*/

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

#ifndef _WX_PLATFORM_H_
#define _WX_PLATFORM_H_

#ifdef __WXMAC_XCODE__
#    include <unistd.h>
#    include <TargetConditionals.h>
#    include <AvailabilityMacros.h>
#    ifndef MAC_OS_X_VERSION_10_4
#       define MAC_OS_X_VERSION_10_4 1040
#    endif
#    ifndef MAC_OS_X_VERSION_10_5
#       define MAC_OS_X_VERSION_10_5 1050
#    endif
#    ifndef MAC_OS_X_VERSION_10_6
#       define MAC_OS_X_VERSION_10_6 1060
#    endif
#    ifndef MAC_OS_X_VERSION_10_7
#       define MAC_OS_X_VERSION_10_7 1070
#    endif
#    ifndef MAC_OS_X_VERSION_10_8
#       define MAC_OS_X_VERSION_10_8 1080
#    endif
#    include "wx/osx/config_xcode.h"
#    ifndef __WXOSX__
#        define __WXOSX__ 1
#    endif
#    ifndef __WXMAC__
#        define __WXMAC__ 1
#    endif
#endif

/*
    We use __WINDOWS__ as our main identification symbol for Microsoft Windows
    but it's actually not predefined directly by any commonly used compilers
    (only Watcom defines it itself and it's not supported any longer), so we
    define it ourselves if any of the following macros is defined:

    - MSVC _WIN32 (notice that this is also defined under Win64)
    - Borland __WIN32__
    - Our __WXMSW__ which selects Windows as platform automatically
 */
#if defined(_WIN32) || defined(__WIN32__) || defined(__WXMSW__)
#    ifndef __WINDOWS__
#        define __WINDOWS__
#    endif /* !__WINDOWS__ */
#endif /* Any standard symbol indicating Windows */

#if defined(__WINDOWS__)
    /* Select wxMSW under Windows if no other port is specified. */
#   if !defined(__WXMSW__) && !defined(__WXMOTIF__) && !defined(__WXGTK__) && !defined(__WXX11__)
#       define __WXMSW__
#   endif

#   if !defined(__WINDOWS__)
#       define __WINDOWS__
#   endif

#   ifndef _WIN32
#        define _WIN32
#   endif

#   ifndef WIN32
#        define WIN32
#   endif

#   ifndef __WIN32__
#        define __WIN32__
#   endif

    /* MSVC predefines _WIN64 for 64 bit builds, for gcc we use generic
       architecture definitions. */
#   if defined(_WIN64) || defined(__x86_64__)
#       ifndef __WIN64__
#           define __WIN64__
#       endif /* !__WIN64__ */
#   endif /* _WIN64 */

#endif /* __WINDOWS__ */

/*
    Don't use widget toolkit specific code in non-GUI code in the library
    itself to ensure that the same base library is used for both MSW and GTK
    ports. But keep __WXMSW__ defined for (console) applications using
    wxWidgets for compatibility.
 */
#if defined(WXBUILDING) && defined(wxUSE_GUI) && !wxUSE_GUI
#   ifdef __WXMSW__
#       undef __WXMSW__
#   endif
#   ifdef __WXGTK__
#       undef __WXGTK__
#   endif
#endif

#if defined(__WXGTK__) && defined(__WINDOWS__)

#   ifdef __WXMSW__
#       undef __WXMSW__
#   endif

#endif /* __WXGTK__ && __WINDOWS__ */

/* detect MS SmartPhone */
#if defined( WIN32_PLATFORM_WFSP )
#   ifndef __SMARTPHONE__
#       define __SMARTPHONE__
#   endif
#   ifndef __WXWINCE__
#       define __WXWINCE__
#   endif
#endif

/* detect PocketPC */
#if defined( WIN32_PLATFORM_PSPC )
#   ifndef __POCKETPC__
#       define __POCKETPC__
#   endif
#   ifndef __WXWINCE__
#       define __WXWINCE__
#   endif
#endif

/* detect Standard WinCE SDK */
#if defined( WCE_PLATFORM_STANDARDSDK )
#   ifndef __WINCE_STANDARDSDK__
#       define __WINCE_STANDARDSDK__
#   endif
#   ifndef __WXWINCE__
#       define __WXWINCE__
#   endif
#endif

#if defined(_WIN32_WCE) && !defined(WIN32_PLATFORM_WFSP) && !defined(WIN32_PLATFORM_PSPC)
#   if (_WIN32_WCE >= 400)
#       ifndef __WINCE_NET__
#           define __WINCE_NET__
#       endif
#   elif (_WIN32_WCE >= 200)
#       ifndef __HANDHELDPC__
#           define __HANDHELDPC__
#       endif
#   endif
#   ifndef __WXWINCE__
#       define __WXWINCE__
#   endif
#endif

#if defined(__WXWINCE__) && defined(_MSC_VER) && (_MSC_VER == 1201)
    #define __EVC4__
#endif

#if defined(__POCKETPC__) || defined(__SMARTPHONE__) || defined(__WXGPE__)
#   define __WXHANDHELD__
#endif

#ifdef __ANDROID__
#   define __WXANDROID__
#   include "wx/android/config_android.h"
#endif

#include "wx/compiler.h"

/*
   Include wx/setup.h for the Unix platform defines generated by configure and
   the library compilation options

   Note that it must be included before defining hardware symbols below as they
   could be already defined by configure but it must be included after defining
   the compiler macros above as msvc/wx/setup.h relies on them under Windows.
 */
#include "wx/setup.h"

/*
   Convenience for any optional classes that use the wxAnyButton base class.
 */
#if wxUSE_TOGGLEBTN || wxUSE_BUTTON
    #define wxHAS_ANY_BUTTON
#endif


/*
   Hardware platform detection.

   VC++ defines _M_xxx symbols.
 */
#if defined(_M_IX86) || defined(i386) || defined(__i386) || defined(__i386__)
    #ifndef __INTEL__
        #define __INTEL__
    #endif
#endif /* x86 */

#if defined(_M_IA64)
    #ifndef __IA64__
        #define __IA64__
    #endif
#endif /* ia64 */

#if defined(_M_MPPC) || defined(__PPC__) || defined(__ppc__)
    #ifndef __POWERPC__
        #define __POWERPC__
    #endif
#endif /* alpha */

#if defined(_M_ALPHA) || defined(__AXP__)
    #ifndef __ALPHA__
        #define __ALPHA__
    #endif
#endif /* alpha */


/*
   adjust the Unicode setting: wxUSE_UNICODE should be defined as 0 or 1
   and is used by wxWidgets, _UNICODE and/or UNICODE may be defined or used by
   the system headers so bring these settings in sync
 */

/* set wxUSE_UNICODE to 1 if UNICODE or _UNICODE is defined */
#if defined(_UNICODE) || defined(UNICODE)
#   undef wxUSE_UNICODE
#   define wxUSE_UNICODE 1
#else /* !UNICODE */
#   ifndef wxUSE_UNICODE
#       define wxUSE_UNICODE 0
#   endif
#endif /* UNICODE/!UNICODE */

/* and vice versa: define UNICODE and _UNICODE if wxUSE_UNICODE is 1 */
#if wxUSE_UNICODE
#   ifndef _UNICODE
#       define _UNICODE
#   endif
#   ifndef UNICODE
#       define UNICODE
#   endif
#endif /* wxUSE_UNICODE */


/*
   test for old versions of Borland C, normally need at least 5.82, Turbo
   explorer, available for free at http://www.turboexplorer.com/downloads
*/


/*
    Older versions of Borland C have some compiler bugs that need
    workarounds. Mostly pertains to the free command line compiler 5.5.1.
*/
#if defined(__BORLANDC__) && (__BORLANDC__ <= 0x551)
    /*
        The Borland free compiler is unable to handle overloaded enum
        comparisons under certain conditions e.g. when any class has a
        conversion ctor for an integral type and there's an overload to
        compare between an integral type and that class type.
    */
#   define wxCOMPILER_NO_OVERLOAD_ON_ENUM

    /*
        This is needed to overcome bugs in 5.5.1 STL, linking errors will
        result if it is not defined.
     */
#   define _RWSTD_COMPILE_INSTANTIATE

    /*
        Preprocessor in older Borland compilers have major problems
        concatenating with ##. Specifically, if the string operands being
        concatenated have special meaning (e.g. L"str", 123i64 etc)
        then ## will not concatenate the operands correctly.

        As a workaround, define wxPREPEND* and wxAPPEND* without using
        wxCONCAT_HELPER.
    */
#   define wxCOMPILER_BROKEN_CONCAT_OPER
#endif /* __BORLANDC__ */

/*
   OS: first of all, test for MS-DOS platform. We must do this before testing
       for Unix, because DJGPP compiler defines __unix__ under MS-DOS
 */
#if defined(__GO32__) || defined(__DJGPP__) || defined(__DOS__)
#    ifndef __DOS__
#        define __DOS__
#    endif
    /* size_t is the same as unsigned int for Watcom 11 compiler, */
    /* so define it if it hadn't been done by configure yet */
#    if !defined(wxSIZE_T_IS_UINT) && !defined(wxSIZE_T_IS_ULONG)
#        ifdef __WATCOMC__
#            define wxSIZE_T_IS_UINT
#        endif
#        ifdef __DJGPP__
#            define wxSIZE_T_IS_ULONG
#        endif
#    endif

/*
   OS: then test for generic Unix defines, then for particular flavours and
       finally for Unix-like systems
       Mac OS X matches this case (__MACH__), prior Mac OS do not.
 */
#elif defined(__UNIX__) || defined(__unix) || defined(__unix__) || \
      defined(____SVR4____) || defined(__LINUX__) || defined(__sgi) || \
      defined(__hpux) || defined(__sun) || defined(__SUN__) || defined(_AIX) || \
      defined(__EMX__) || defined(__VMS) || defined(__BEOS__) || defined(__MACH__)

#    define __UNIX_LIKE__

#    ifdef __SGI__
#        ifdef __GNUG__
#        else /* !gcc */
            /*
               Note I use the term __SGI_CC__ for both cc and CC, its not a good
               idea to mix gcc and cc/CC, the name mangling is different
             */
#            define __SGI_CC__
#        endif /* gcc/!gcc */

        /* system headers use this symbol and not __cplusplus in some places */
#       ifndef _LANGUAGE_C_PLUS_PLUS
#           define _LANGUAGE_C_PLUS_PLUS
#       endif
#    endif  /* SGI */

#    ifdef __EMX__
#        define OS2EMX_PLAIN_CHAR
#    endif
#    if defined(__INNOTEK_LIBC__)
        /* Ensure visibility of strnlen declaration */
#        define _GNU_SOURCE
#    endif

    /* define __HPUX__ for HP-UX where standard macro is __hpux */
#    if defined(__hpux) && !defined(__HPUX__)
#        define __HPUX__
#    endif /* HP-UX */

    /*  All of these should already be defined by including configure-
        generated setup.h but we wish to support Xcode compilation without
        requiring the user to define these himself.
     */
#    if defined(__APPLE__) && defined(__MACH__)
#        ifndef __UNIX__
#            define __UNIX__ 1
#        endif
#        ifndef __BSD__
#            define __BSD__ 1
#        endif
        /*  __DARWIN__ is our own define to mean OS X or pure Darwin */
#        ifndef __DARWIN__
#            define __DARWIN__ 1
#        endif
        /*  NOTE: TARGET_CARBON is actually a 0/1 and must be 1 for OS X */
#        ifndef TARGET_CARBON
#            define TARGET_CARBON 1
#        endif
        /* OS X uses unsigned long size_t for both ILP32 and LP64 modes. */
#        if !defined(wxSIZE_T_IS_UINT) && !defined(wxSIZE_T_IS_ULONG)
#            define wxSIZE_T_IS_ULONG
#        endif
#    endif

/*
   OS: OS/2
 */
#elif defined(__OS2__)

    /* wxOS2 vs. non wxOS2 ports on OS2 platform */
#    if !defined(__WXMOTIF__) && !defined(__WXGTK__) && !defined(__WXX11__)
#        ifndef __WXPM__
#            define __WXPM__
#        endif
#    endif

#    if defined(__IBMCPP__)
#        define __VISAGEAVER__ __IBMCPP__
#    endif

    /* Place other OS/2 compiler environment defines here */
#    if defined(__VISAGECPP__)
        /* VisualAge is the only thing that understands _Optlink */
#        define LINKAGEMODE _Optlink
#    endif
#    define wxSIZE_T_IS_UINT

/*
   OS: Windows
 */
#elif defined(__WINDOWS__)

    /* to be changed for Win64! */
#    ifndef __WIN32__
#        error "__WIN32__ should be defined for Win32 and Win64, Win16 is not supported"
#    endif

    /* size_t is the same as unsigned int for all Windows compilers we know, */
    /* so define it if it hadn't been done by configure yet */
#    if !defined(wxSIZE_T_IS_UINT) && !defined(wxSIZE_T_IS_ULONG) && !defined(__WIN64__)
#        define wxSIZE_T_IS_UINT
#    endif
#else
#   error "Unknown platform."
#endif  /* OS */

/*
   if we're on a Unix system but didn't use configure (so that setup.h didn't
   define __UNIX__), do define __UNIX__ now
 */
#if !defined(__UNIX__) && defined(__UNIX_LIKE__)
#    define __UNIX__
#endif /* Unix */

#if defined(__WXMOTIF__) || defined(__WXX11__)
#    define __X__
#endif

/*
   We get "Large Files (ILP32) not supported in strict ANSI mode." #error
   from HP-UX standard headers when compiling with g++ without this:
 */
#if defined(__HPUX__) && !defined(__STDC_EXT__)
#   define __STDC_EXT__ 1
#endif

/* Force linking against required libraries under Windows: */
#ifdef __WXWINCE__
#   include "wx/msw/wince/libraries.h"
#elif defined __WINDOWS__
#   include "wx/msw/libraries.h"
#endif

#if defined(__BORLANDC__) || (defined(__GNUC__) && __GNUC__ < 3)
#define wxNEEDS_CHARPP
#endif

/*
    Note that wx/msw/gccpriv.h must be included after defining UNICODE and
    _UNICODE macros as it includes _mingw.h which relies on them being set.
 */
#if ( defined( __GNUWIN32__ ) || defined( __MINGW32__ ) || \
    ( defined( __CYGWIN__ ) && defined( __WINDOWS__ ) ) || \
      wxCHECK_WATCOM_VERSION(1,0) ) && \
    !defined(__DOS__) && \
    !defined(__WXPM__) && \
    !defined(__WXMOTIF__) && \
    !defined(__WXX11__)
#    include "wx/msw/gccpriv.h"
#else
#    undef wxCHECK_W32API_VERSION
#    define wxCHECK_W32API_VERSION(maj, min) (0)
#    undef wxCHECK_MINGW32_VERSION
#    define wxCHECK_MINGW32_VERSION( major, minor ) (0)
#    define wxDECL_FOR_STRICT_MINGW32(rettype, func, params)
#endif


/*
    Handle Darwin gcc universal compilation.  Don't do this in an Apple-
    specific case since no sane compiler should be defining either
    __BIG_ENDIAN__ or __LITTLE_ENDIAN__ unless it really is generating
    code that will be hosted on a machine with the appropriate endianness.
    If a compiler defines neither, assume the user or configure set
    WORDS_BIGENDIAN appropriately.
 */
#if defined(__BIG_ENDIAN__)
#    undef WORDS_BIGENDIAN
#    define WORDS_BIGENDIAN 1
#elif defined(__LITTLE_ENDIAN__)
#    undef WORDS_BIGENDIAN
#elif defined(__WXMAC__) && !defined(WORDS_BIGENDIAN)
/*  According to Stefan even ancient Mac compilers defined __BIG_ENDIAN__ */
#    warning "Compiling wxMac with probably wrong endianness"
#endif
/* also the 32/64 bit universal builds must be handled accordingly */
#ifdef __DARWIN__
#    ifdef __LP64__
#        undef SIZEOF_VOID_P
#        undef SIZEOF_LONG
#        undef SIZEOF_SIZE_T
#        define SIZEOF_VOID_P 8
#        define SIZEOF_LONG 8
#        define SIZEOF_SIZE_T 8
#    else
#        undef SIZEOF_VOID_P
#        undef SIZEOF_LONG
#        undef SIZEOF_SIZE_T
#        define SIZEOF_VOID_P 4
#        define SIZEOF_LONG 4
#        define SIZEOF_SIZE_T 4
#    endif
#endif

/*
    Define various OS X symbols before including wx/chkconf.h which uses them.

    __WXOSX_MAC__ means Mac OS X, non embedded
    __WXOSX_IPHONE__ means OS X iPhone
 */

/*
    Normally all of __WXOSX_XXX__, __WXOSX__ and __WXMAC__ are defined by
    configure but ensure that we also define them if configure was not used for
    whatever reason.

    The primary symbol remains __WXOSX_XXX__ one, __WXOSX__ exists to allow
    checking for any OS X port (Carbon and Cocoa) and __WXMAC__ is an old name
    for it.
 */
#if defined(__WXOSX_CARBON__) || defined(__WXOSX_COCOA__) || defined(__WXOSX_IPHONE__)
#   ifndef __WXOSX__
#       define __WXOSX__ 1
#   endif
#   ifndef __WXMAC__
#       define __WXMAC__ 1
#   endif
#endif

#ifdef __WXOSX__
/* setup precise defines according to sdk used */
#   include <TargetConditionals.h>
#   if defined(__WXOSX_IPHONE__)
#       if !( defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE )
#           error "incorrect SDK for an iPhone build"
#       endif
#   else
#       if wxUSE_GUI && !(defined(__WXOSX_CARBON__) || defined(__WXOSX_COCOA__))
#           error "one of __WXOSX_IPHONE__, __WXOSX_CARBON__ or __WXOSX_COCOA__ must be defined for the GUI build"
#       endif
#       if !( defined(TARGET_OS_MAC) && TARGET_OS_MAC )
#           error "incorrect SDK for a Mac OS X build"
#       endif
#       define __WXOSX_MAC__ 1
#   endif
#endif

#ifdef __WXOSX_MAC__
#    if defined(__MACH__)
#        include <AvailabilityMacros.h>
#        ifndef MAC_OS_X_VERSION_10_4
#           define MAC_OS_X_VERSION_10_4 1040
#        endif
#        ifndef MAC_OS_X_VERSION_10_5
#           define MAC_OS_X_VERSION_10_5 1050
#        endif
#        ifndef MAC_OS_X_VERSION_10_6
#           define MAC_OS_X_VERSION_10_6 1060
#        endif
#        ifndef MAC_OS_X_VERSION_10_7
#           define MAC_OS_X_VERSION_10_7 1070
#        endif
#        ifndef MAC_OS_X_VERSION_10_8
#           define MAC_OS_X_VERSION_10_8 1080
#        endif
#    else
#        error "only mach-o configurations are supported"
#    endif
#endif

/*
    __WXOSX_OR_COCOA__ is a common define to wxOSX (Carbon or Cocoa) and wxCocoa ports under OS X.

    DO NOT use this define in base library code.  Although wxMac has its own
    private base library (and thus __WXOSX_OR_COCOA__,__WXMAC__ and related defines are
    valid there), wxCocoa shares its library with other ports like wxGTK and wxX11.

    To keep wx authors from screwing this up, only enable __WXOSX_OR_COCOA__ for wxCocoa when
    not compiling the base library.  We determine this by first checking if
    wxUSE_BASE is not defined.  If it is not defined, then we're not buildling
    the base library, and possibly not building wx at all (but actually building
    user code that's using wx). If it is defined then we must check to make sure
    it is not true.  If it is true, we're building base.

    If you want it in the common darwin base library then use __DARWIN__.  You
    can use any Darwin-available libraries like CoreFoundation but please avoid
    using OS X libraries like Carbon or CoreServices.

 */
#if defined(__WXOSX__) || (defined(__WXCOCOA__) && (!defined(wxUSE_BASE) || !wxUSE_BASE))
#   define __WXOSX_OR_COCOA__ 1
#endif

/*
   check the consistency of the settings in setup.h: note that this must be
   done after setting wxUSE_UNICODE correctly as it is used in wx/chkconf.h
   and after defining the compiler macros which are used in it too
 */
#include "wx/chkconf.h"


/*
   some compilers don't support iostream.h any longer, while some of theme
   are not updated with <iostream> yet, so override the users setting here
   in such case.
 */
#if defined(_MSC_VER) && (_MSC_VER >= 1310)
#    undef wxUSE_IOSTREAMH
#    define wxUSE_IOSTREAMH 0
#elif defined(__DMC__) || defined(__WATCOMC__)
#    undef wxUSE_IOSTREAMH
#    define wxUSE_IOSTREAMH 1
#elif defined(__MINGW32__)
#    undef wxUSE_IOSTREAMH
#    define wxUSE_IOSTREAMH 0
#endif /* compilers with/without iostream.h */

/*
   old C++ headers (like <iostream.h>) declare classes in the global namespace
   while the new, standard ones (like <iostream>) do it in std:: namespace,
   unless it's an old gcc version.

   using this macro allows constuctions like "wxSTD iostream" to work in
   either case
 */
#if !wxUSE_IOSTREAMH && (!defined(__GNUC__) || ( __GNUC__ > 2 ) || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95))
#    define wxSTD std::
#else
#    define wxSTD
#endif

/* On OpenVMS with the most recent HP C++ compiler some function (i.e. wscanf)
 * are only available in the std-namespace. (BUG???)
 */
#if defined(  __VMS ) && (__DECCXX_VER >= 70100000) && !defined(__STD_CFRONT) && !defined( __NONAMESPACE_STD )
# define wxVMS_USE_STD std::
#else
# define wxVMS_USE_STD
#endif

#ifdef __VMS
#define XtDisplay XTDISPLAY
#ifdef __WXMOTIF__
#define XtParent XTPARENT
#define XtScreen XTSCREEN
#define XtWindow XTWINDOW
#endif
#endif

/* Choose which method we will use for updating menus
 * - in OnIdle, or when we receive a wxEVT_MENU_OPEN event.
 * Presently, only Windows, OS X and GTK+ support wxEVT_MENU_OPEN.
 */
#ifndef wxUSE_IDLEMENUUPDATES
#    if (defined(__WXMSW__) || defined(__WXGTK__) || defined(__WXOSX__)) && !defined(__WXUNIVERSAL__)
#        define wxUSE_IDLEMENUUPDATES 0
#    else
#        define wxUSE_IDLEMENUUPDATES 1
#    endif
#endif

/*
 * Define symbols that are not yet in
 * configure or possibly some setup.h files.
 * They will need to be added.
 */

#ifndef wxUSE_FILECONFIG
#    if wxUSE_CONFIG && wxUSE_TEXTFILE
#        define wxUSE_FILECONFIG 1
#    else
#        define wxUSE_FILECONFIG 0
#    endif
#endif

#ifndef wxUSE_HOTKEY
#    define wxUSE_HOTKEY 0
#endif

#if !defined(wxUSE_WXDIB) && defined(__WXMSW__)
#    define wxUSE_WXDIB 1
#endif

/*
    Optionally supported C++ features.
 */

/*
    RTTI: if it is disabled in build/msw/makefile.* then this symbol will
    already be defined but it's also possible to do it from configure (with
    g++) or by editing project files with MSVC so test for it here too.
 */
#ifndef wxNO_RTTI
    /*
        Only 4.3 defines __GXX_RTTI by default so its absence is not an
        indication of disabled RTTI with the previous versions.
     */
#   if wxCHECK_GCC_VERSION(4, 3)
#       ifndef __GXX_RTTI
#           define wxNO_RTTI
#       endif
#   elif defined(_MSC_VER)
#       ifndef _CPPRTTI
#           define wxNO_RTTI
#       endif
#   endif
#endif /* wxNO_RTTI */

#endif /* _WX_PLATFORM_H_ */
