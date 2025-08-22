#ifndef QDETAILS_VIEW_API_H
#define QDETAILS_VIEW_API_H

#include <QtCore/qglobal.h>

#if defined(QDETAILS_VIEW_LIBRARY)
#  define QDETAILS_VIEW_API Q_DECL_EXPORT
#else
#  define QDETAILS_VIEW_API Q_DECL_IMPORT
#endif

#endif // QDETAILS_VIEW_API_H