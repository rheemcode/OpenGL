#pragma once

#if defined(ERROR)
#undef ERROR
#endif

#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4251 )
#pragma warning ( disable : 4275 )
#pragma warning ( disable : 26495 )

#ifdef GLIB_BUILD_DLL
#define GLIB_API _declspec(dllexport)
#define GLIBSTORAGE
#else 
#define GLIBSTORAGE extern 
#define GLIB_API _declspec(dllimport)
#endif

#include "Templates.h"