#include "OpenGLDefaults.h"


namespace cs557
{


	// variable too keep the glsl version.
	int glsl_major = 0, glsl_minor = 0;



	/*!
	Return the glsl major / minor version.
	*/
	int GLSLMajor(void)
	{
		return glsl_major;
	}

	int GLSLMinor(void)
	{
		return glsl_minor;
	}



	/*!
	This function initializes glew
	*/
	bool initGlew(void)
	{

		// Makes sure all extensions will be exposed in GLEW and initialize GLEW.
		glewExperimental = GL_TRUE;

		// Initialize GLEW
		if (glewInit() != GLEW_OK) {
			cout << "Failed to initialize GLEW\n" << endl;
			system("pause");
			return false;
		}

		cout << "OpenGL version supported by this platform " << glGetString(GL_VERSION) << endl;


		// Check the GLSL version
		const char *verstr = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
		cout << "GLSL version supported by this platform " << string(verstr) << endl;
		#ifdef _WIN32
			sscanf_s(verstr, "%d.%d", &glsl_major, &glsl_minor);
		#else
			sscanf(verstr, "%d.%d", &glsl_major, &glsl_minor);
		#endif

		

		return true;
	}



}