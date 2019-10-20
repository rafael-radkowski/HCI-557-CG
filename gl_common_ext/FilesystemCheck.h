/*
@file: FilesystemCheck.h

This header file includes the windows header <experimental/filesystem> depending on the 
used visual studio version and the operating system.
experimental/filesystem is deprecated and causes error messages. Thus, this code
prevent those.

This file is part of CS/CPRE/ME 557 Computer Graphics at Iowa State University

Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294-7044
MIT License

-------------------------------------------------------------------------------
Last edited:


*/
#pragma once
#include <iostream>
#include <string>


#ifdef _WIN32
	#if _MSC_VER > 1900	
		#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
		#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
		#include <experimental\filesystem>
	#else
		#include <experimental\filesystem>
	#endif
#endif