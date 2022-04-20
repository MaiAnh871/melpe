/*
 *  Copyright 2010 by Texas Instruments Incorporated.
 *  @(#) DSP/BIOS_Kernel 5,2,5,28 06-06-2010 (cuda-u29)
 */
/*
 *  ======== tistdtypes.h ========
 *
 */


/*
 *  These types are also defined by other TI components.  They are bracketed
 *  with _TI_STD_TYPES to avoid warnings for duplicate definition.  The
 *  The Uint16, etc. definitions were provided in early 2.x versions of CSL
 *  that did not have _TI_STD_TYPES protection.
 *
 *  You may get warnings about duplicate type definitions when using this
 *  header file with 2.x CSL.  You can use the '-pds303' compiler option to
 *  suppress these warnings.
 */
#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES

/*
 * Aliases for standard C types
 */
typedef int                     Int;
typedef unsigned                Uns;
typedef char                    Char;

/* pointer to null-terminated character sequence */
typedef char                    *String;

typedef void                    *Ptr;           /* pointer to arbitrary type */

typedef unsigned short          Bool;           /* boolean */

/*
 * Uint8, Uint16, Uint32, etc are defined to be "smallest unit of
 * available storage that is large enough to hold unsigned or integer
 * of specified size".
 */

/* Handle the 6x ISA */
#if defined(_TMS320C6X)
    /* Unsigned integer definitions (32bit, 16bit, 8bit) follow... */
    typedef unsigned int        Uint32;
    typedef unsigned short      Uint16;
    typedef unsigned char       Uint8;

    /* Signed integer definitions (32bit, 16bit, 8bit) follow... */
    typedef int                 Int32;
    typedef short               Int16;
    typedef char                Int8;

/* Handle the 54x, 55x and 28x ISAs */
#elif defined(_TMS320C5XX) || defined(__TMS320C55X__)
    /* Unsigned integer definitions (32bit, 16bit, 8bit) follow... */
    typedef unsigned long       Uint32;
    typedef unsigned short      Uint16;
    typedef unsigned char       Uint8;

    /* Signed integer definitions (32bit, 16bit, 8bit) follow... */
    typedef long                Int32;
    typedef short               Int16;
    typedef char                Int8;

#elif defined(_TMS320C28X)
    /* Unsigned integer definitions (32bit, 16bit, 8bit) follow... */
    typedef unsigned long       Uint32;
    typedef unsigned int        Uint16;
    typedef unsigned char       Uint8;

    /* Signed integer definitions (32bit, 16bit, 8bit) follow... */
    typedef long                Int32;
    typedef int                 Int16;
    typedef char                Int8;
#else
    /* Other ISAs not supported */
    #error <tistdtypes.h> is not supported for this target
#endif  /* defined(_6x_) */

#endif  /* _TI_STD_TYPES */
