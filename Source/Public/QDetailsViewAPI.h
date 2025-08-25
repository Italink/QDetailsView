#ifndef QDETAILS_VIEW_API_H
#define QDETAILS_VIEW_API_H

#include <QtCore/qglobal.h>

#ifdef _WIN32
#	ifdef QDETAILS_VIEW_STATIC_LIBRARY
#		define QDETAILS_VIEW_API 
#	else
#		ifdef QDETAILS_VIEW_SHARED_LIBRARY
#			define QDETAILS_VIEW_API __declspec(dllexport)
#		else
#			define QDETAILS_VIEW_API __declspec(dllimport)
#		endif
#	endif
#else
#	define QDETAILS_VIEW_API __attribute__((visibility("default")))
#endif


#endif // QDETAILS_VIEW_API_H