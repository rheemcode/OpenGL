#pragma once



#ifdef GLIB_BUILD_DLL
	#define GLIB_API _declspec(dllexport)
#else 
	#define GLIB_API _declspec(dllimport)
#endif