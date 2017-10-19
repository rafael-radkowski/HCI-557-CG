//
//  main.cpp
//  OpenGL4Test
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Sphere3D.h"

using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;



int main(int argc, const char * argv[])
{


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Init glfw, create a window, and init glew

	// Init the GLFW Window
	window = initWindow();


	// Init the glew api
	initGlew();



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Create some models

	// coordinate system
	CoordSystem* cs = new CoordSystem(40.0);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Sphere number 1


	// create an apperance object.
	GLAppearance* apperance0 = new GLAppearance("../shaders/sphere_01.vs", "../shaders/sphere_01.fs");


	// The spotlight object
	GLSpotLightSource  light_source1;
	light_source1._lightPos = glm::vec4(20.0, 20.0, 0.0, 1.0);
	light_source1._ambient_intensity = 0.3;
	light_source1._specular_intensity = 8.0;
	light_source1._diffuse_intensity = 3.0;
	light_source1._attenuation_coeff = 0.02;
	light_source1._cone_angle = 12.0; // in degree
	light_source1._cone_direction = glm::vec3(-1.0, -1.0, 0.0); // this must be aligned with the object and light position.


	GLDirectLightSource  light_source2;
	light_source2._lightPos = glm::vec4(20.0, 0.0, 0.0, 0.0);
	light_source2._ambient_intensity = 0.1;
	light_source2._specular_intensity = 5.5;
	light_source2._diffuse_intensity = 1.0;
	light_source2._attenuation_coeff = 0.02;
	// light_source2._cone_angle = 12.0; // in degree
	// light_source2._cone_direction = glm::vec3(-1.0, 0.0, 0.0); // this must be aligned with the object and light position.


	// add the spot light to this apperance object
	apperance0->addLightSource(light_source1);
	apperance0->addLightSource(light_source2);

	// Create a material object
	GLMaterial material0;
	material0._diffuse_material = glm::vec3(0.0, 0.0, 1.0);
	material0._ambient_material = glm::vec3(0.0, 0.0, 1.0);
	material0._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material0._shininess = 12.0;

	// Add the material to the apperance object
	apperance0->setMaterial(material0);
	apperance0->finalize();

	// create the sphere geometry
	GLSphere3D* sphere0 = new GLSphere3D(0.0, 0.0, 0.0, 5.0, 10, 10);
	sphere0->setApperance(*apperance0);
	sphere0->init();

	material0._diffuse_material = glm::vec3(1.0, 0.0, 0.2);
	material0._ambient_material = glm::vec3(1.0, 0.0, 0.2);
	apperance0->updateMaterial();
	light_source1._diffuse_intensity = 1.0;
	apperance0->updateLightSources();



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Sphere number 2

	// create an apperance object.
	GLAppearance* apperance1 = new GLAppearance("../shaders/sphere_01.vs", "../shaders/sphere_01.fs");

	// -> FEEL FREE TO CREATE NEW LIGHT SOURCES


	// add the spot light to this apperance object
	apperance1->addLightSource(light_source1);
	apperance1->addLightSource(light_source2);

	// Create a material object
	GLMaterial material1;
	material1._diffuse_material = glm::vec3(0.0, 0.0, 1.0);
	material1._ambient_material = glm::vec3(0.0, 0.0, 1.0);
	material1._specular_material = glm::vec3(0.0, 0.0, 0.0);
	material1._shininess = 12.0;

	// Add the material to the apperance object
	apperance1->setMaterial(material1);
	apperance1->finalize();

	// create the sphere geometry
	GLSphere3D* sphere1 = new GLSphere3D(0.0, 0.0, 0.0, 5.0, 10, 10);
	sphere1->setApperance(*apperance1);
	sphere1->init();


	// NOTE - THIS IS AN EXAMPLE THAT SHOWS HOW TO UPDATE LIGHT SOURCES AMD MATERIAL PARAMETER

	material1._diffuse_material = glm::vec3(0.0, 0.2, 0.2);
	material1._ambient_material = glm::vec3(0.0, 0.2, 0.2);
	apperance1->updateMaterial();
	light_source1._diffuse_intensity = 1.0;
	apperance1->updateLightSources();



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Sphere number 3

	// create an apperance object.
	GLAppearance* apperance2 = new GLAppearance("../shaders/sphere_01.vs", "../shaders/sphere_01.fs");

	// -> FEEL FREE TO CREATE NEW LIGHT SOURCES


	// add the spot light to this apperance object
	apperance2->addLightSource(light_source1);
	apperance2->addLightSource(light_source2);

	// Create a material object
	GLMaterial material2;
	material2._diffuse_material = glm::vec3(0.0, 0.0, 1.0);
	material2._ambient_material = glm::vec3(0.0, 0.0, 1.0);
	material2._specular_material = glm::vec3(1.0, 1.0, 0.0);
	material2._shininess = 2.7;

	// Add the material to the apperance object
	apperance2->setMaterial(material2);
	apperance2->finalize();

	// create the sphere geometry
	GLSphere3D* sphere2 = new GLSphere3D(0.0, 0.0, 0.0, 5.0, 10, 10);
	sphere2->setApperance(*apperance2);
	sphere2->init();



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Sphere number 4

	// create an apperance object.
	GLAppearance* apperance3 = new GLAppearance("../shaders/sphere_01.vs", "../shaders/sphere_01.fs");

	// -> FEEL FREE TO CREATE NEW LIGHT SOURCES


	// add the spot light to this apperance object
	apperance3->addLightSource(light_source1);
	apperance3->addLightSource(light_source2);

	// Create a material object
	GLMaterial material3;
	material3._diffuse_material = glm::vec3(1.0, 0.0, 0.2);
	material3._ambient_material = glm::vec3(0.3, 0.0, 1.0);
	material3._specular_material = glm::vec3(1.0, 0.0, 0.0);
	material3._shininess = 0.0;

	// Add the material to the apperance object
	apperance3->setMaterial(material3);
	apperance3->finalize();

	// create the sphere geometry
	GLSphere3D* sphere3 = new GLSphere3D(0.0, 0.0, 0.0, 5.0, 10, 10);
	sphere3->setApperance(*apperance3);
	sphere3->init();




	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Main render loop

	// Set up our green background color
	static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// This sets the camera to a new location
	// the first parameter is the eye position, the second the center location, and the third the up vector. 
	SetViewAsLookAt(glm::vec3(30.0f, 30.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	// Enable depth test
	// ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
	glEnable(GL_DEPTH_TEST);

	//sphere->enableNormalVectorRenderer();

	// This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
	while (!glfwWindowShouldClose(window))
	{

		// Clear the entire buffer with our green color (sets the background to be green).
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// This renders the objects

		// Set the trackball locatiom
		SetTrackballLocation(trackball.getRotationMatrix());

		// draw the objects
		cs->draw();

		glm::mat4 matrix = glm::translate(glm::mat4(), glm::vec3(7.0f, 0.0f, 0.0f));
		sphere0->setModelMatrix(matrix);
		sphere0->draw();

		matrix = glm::translate(glm::mat4(), glm::vec3(19.0f, 0.0f, 0.0f));
		sphere1->setModelMatrix(matrix);
		sphere1->draw();


		matrix = glm::translate(glm::mat4(), glm::vec3(-7.0f, 0.0f, 0.0f));
		sphere2->setModelMatrix(matrix);
		sphere2->draw();


		matrix = glm::translate(glm::mat4(), glm::vec3(-19.0f, 0.0f, 0.0f));
		sphere3->setModelMatrix(matrix);
		sphere3->draw();

		//// This renders the objects
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		// Swap the buffers so that what we drew will appear on the screen.
		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	delete cs;


}

