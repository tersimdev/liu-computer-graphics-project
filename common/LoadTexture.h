#ifndef I_LOAD_TEX
#define I_LOAD_TEX

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#else
	#if defined(_WIN32)
		#include "glew.h"
		#include <GL/gl.h>
	#else
		#include <GL/gl.h>
	#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

GLuint LoadTexture(const char *filename, char dorepeat);

#ifdef __cplusplus
}
#endif

#endif
