#ifndef EXPORT_H
#define EXPORT_H

#if defined(FANCONTROL_GUI_LIBRARY)
#  define FANCONTROL_GUI_EXPORT Q_DECL_EXPORT
#else
#  define FANCONTROL_GUI_EXPORT Q_DECL_IMPORT
#endif

//#define TRANSLATION_DOMAIN "fancontrol-gui"

#endif // EXPORT_H

