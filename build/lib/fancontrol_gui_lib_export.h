
#ifndef FANCONTROL_GUI_LIB_EXPORT_H
#define FANCONTROL_GUI_LIB_EXPORT_H

#ifdef FANCONTROL_GUI_LIB_STATIC_DEFINE
#  define FANCONTROL_GUI_LIB_EXPORT
#  define FANCONTROL_GUI_LIB_NO_EXPORT
#else
#  ifndef FANCONTROL_GUI_LIB_EXPORT
#    ifdef fancontrol_gui_lib_EXPORTS
        /* We are building this library */
#      define FANCONTROL_GUI_LIB_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define FANCONTROL_GUI_LIB_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef FANCONTROL_GUI_LIB_NO_EXPORT
#    define FANCONTROL_GUI_LIB_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef FANCONTROL_GUI_LIB_DEPRECATED
#  define FANCONTROL_GUI_LIB_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef FANCONTROL_GUI_LIB_DEPRECATED_EXPORT
#  define FANCONTROL_GUI_LIB_DEPRECATED_EXPORT FANCONTROL_GUI_LIB_EXPORT FANCONTROL_GUI_LIB_DEPRECATED
#endif

#ifndef FANCONTROL_GUI_LIB_DEPRECATED_NO_EXPORT
#  define FANCONTROL_GUI_LIB_DEPRECATED_NO_EXPORT FANCONTROL_GUI_LIB_NO_EXPORT FANCONTROL_GUI_LIB_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define FANCONTROL_GUI_LIB_NO_DEPRECATED
#endif

#endif
