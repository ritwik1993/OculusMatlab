
#ifndef Resources_EXPORT_H
#define Resources_EXPORT_H

#ifdef Resources_STATIC
#  define Resources_EXPORT
#  define RESOURCES_NO_EXPORT
#else
#  ifndef Resources_EXPORT
#    ifdef ExampleResources_EXPORTS
        /* We are building this library */
#      define Resources_EXPORT 
#    else
        /* We are using this library */
#      define Resources_EXPORT 
#    endif
#  endif

#  ifndef RESOURCES_NO_EXPORT
#    define RESOURCES_NO_EXPORT 
#  endif
#endif

#ifndef RESOURCES_DEPRECATED
#  define RESOURCES_DEPRECATED __attribute__ ((__deprecated__))
#  define RESOURCES_DEPRECATED_EXPORT Resources_EXPORT __attribute__ ((__deprecated__))
#  define RESOURCES_DEPRECATED_NO_EXPORT RESOURCES_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define RESOURCES_NO_DEPRECATED
#endif

#endif
