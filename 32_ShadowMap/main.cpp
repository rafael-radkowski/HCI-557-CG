
// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions

// glfw includes
#include <GLFW/glfw3.h>

// local
#define GLM_ENABLE_EXPERIMENTAL
#include "Window.h" // the windows
#include "OpenGLDefaults.h" // some open gl and glew defaults
#include "VertexBuffers.h"  // create vertex buffer object
#include "ShaderProgram.h"  // create a shader program
#include "Texture2D.h"      // for regular 2D texturing
#include "RenderToTexture.h" // for the prerender texture

// Geometry
#include "ModelCoordinateSystem.h"
#include "ModelPlane.h"
#include "ModelObj.h"
#include "ModelSphere.h"


using namespace std;
using namespace cs557;
//------------------------------------------------------------
//
//	Some global variables to keep track of

// The handle to the window object
GLFWwindow *window = NULL;

// Transformation pipeline variables
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix;       // Store the view matrix





//---------------------------------------------------------------------
// Parameters for the hidden scene
GLuint program_hidden = -1;
unsigned int fboHidden = -1;
GLuint color_texture_idx = -1;
GLuint depth_texture_idx = -1;

unsigned int texture_width = 1024;
unsigned int texture_height = 1024;


// Transformation pipeline variables for the prerenderer scene
glm::mat4 projectionMatrixPrerenderer; // Store the projection matrix
glm::mat4 viewMatrixPrerenderer;       // Store the view matrix



/*
This struct is a helper which contains additonial model data.
*/
typedef struct _ModelData
{
	int shader_program_id;
	unsigned int texture_id;
	glm::mat4 model_matrix;

	_ModelData(){
		shader_program_id = -1;
		texture_id = -1;
		model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 
	}

} ModelData;

/*
Light and material data. 
*/
typedef struct _LMData
{
	int shader_program_id;
    glm::vec3 diffuse_mat;
    float     diffuse_int;
    glm::vec3 ambient_mat;
    float     ambient_int;
    glm::vec3 specular_mat;
    float     specular_int;
    float     specular_s;
    // spotlight
    float     cutoff_angle;

    glm::vec3 pos;
    glm::vec3 center;

	_LMData(){
		shader_program_id = -1;
        diffuse_mat = glm::vec3(0.8,0.8,0.8);
        diffuse_int = 1.0;
        ambient_mat = glm::vec3(0.8,0.8,0.8);
        ambient_int = 1.0;
        specular_mat = glm::vec3(0.8,0.8,0.8);
        specular_int = 1.0;
        specular_s = 12.0;
        pos =  glm::vec3(0.0,0.0,0.0);
        center =  glm::vec3(0.0,0.0,0.0);
	}

}LMData;


//------------------------------------------------------------
//
//	Some model  to keep track of

// a corodinate system
cs557::CoordinateSystem coordinateSystem;
glm::mat4 modelMatrixCoord; 


// a plane
cs557::Plane plane0;
ModelData	 plane0d;

// a teapot
cs557::OBJModel teapot0;
ModelData	 	teapot0d;

// The light source
LMData          teapot_light;

// a light source.
// I render a sphere to indicate the location of the light source.
cs557::Sphere   sphere;
ModelData       sphered;






// a helper plane to render the pre-render content
cs557::Plane display;
ModelData	 displayd;
glm::mat4    display_p; // Store the projection matrix
glm::mat4    display_vm;       // Store the view matrix

//------------------------------------------------------------
//
// Set up our green background color
GLfloat clear_color[] = {0.6f, 0.7f, 1.0f, 1.0f};
GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};



// keyboard helpers
float kx = 0.0;
float ky = -3.0;
float kz = 3.0;
float delta = 0.5;

// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void my_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //cout << key << endl << flush;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == 87 && action == GLFW_PRESS) // w
    {
        ky += delta;
        if(ky  > 5.0) ky = 5.0;
        else if(ky < -5.0) ky = -5.0;
    }
    else if(key == 83 && action == GLFW_PRESS)//s
    {
        ky -= delta;
        if(ky  > 5.0) ky = 5.0;
        else if(ky < -5.0) ky = -5.0;
    }
    else if(key == 82 && action == GLFW_PRESS)  // r
    {
        kz += delta;
        if(kz  > 5.0) kz = 5.0;
        else if(kz < 0.0) kz = 0.0;
    }
    else if(key == 70 && action == GLFW_PRESS) // f
    {
        kz -= delta;
        if(kz  > 5.0) kz = 5.0;
        else if(kz < 0.0) kz = 0.0;
    }
    else if(key == 65 && action == GLFW_PRESS) // a
    {
        kx += delta;
        if(kx  > 5.0) kx = 5.0;
        else if(kx < -5.0) kx = -5.0;

    }
    else if(key == 68 && action == GLFW_PRESS) // d
    {
        kx -= delta;
        if(kx  > 5.0) kx = 5.0;
        else if(kx < -5.0) kx = -5.0;

    }
     else if(key == 84 && action == GLFW_PRESS) // t
    {
    

    }

     teapot_light.pos =  glm::vec3(kx, ky, kz);
}




/*
This function creates some helper content that allows one to better debug the
scene.
*/
void CreateHelperContent(void)
{

     // Load the shader program
    displayd.shader_program_id = cs557::LoadAndCreateShaderProgram("../display.vs", "../display.fs");

    // create a plane
    display.create(2.0, 2.0, displayd.shader_program_id);
	displayd.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 

    // Note that those two are dummy parameters. They are necessary to have parameters to pass.
    // The shader program does not user them.
    display_p = glm::perspective(1.57f, (float)400 / (float)400, 0.1f, 100.f);
	display_vm = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Load the texture and create it.
    // Note that this function bind the texture to texture unit GL_TEXTURE0 
    glUseProgram(displayd.shader_program_id);
    
    // associate the pre-render texture with the display texture
	displayd.texture_id = depth_texture_idx;

    // Activate the texture unit and bind the texture. 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, displayd.texture_id);

    // Fetch the texture location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    int texture_location = glGetUniformLocation(displayd.shader_program_id, "tex");
    glUniform1i(texture_location, 0);

    glUniform1f(glGetUniformLocation(displayd.shader_program_id, "display_scale"), 2.0f);

	glUseProgram(0);


     //------------------------------------------------
    // Create a sphere to indicate the light

    // Load the shader program
    sphered.shader_program_id = cs557::LoadAndCreateShaderProgram("../light.vs", "../light.fs");
    sphere.create(0.1, 30, 40, sphered.shader_program_id);

    // This aligns the light position with the sphere position.
    // Note that the light has not graphical representation.
    // Means, the model must be moved to the light locatoin manually.
    sphered.model_matrix = glm::translate(glm::mat4(1.0f), teapot_light.pos); 

    glUseProgram(0);

}


/*
This function creates the scene with all invisble objects. 
The are rendered into a texture which can be used as 2d map in the visible scene. 
*/
void CreateShadowCaster(void)
{
    //---------------------------------------------------------
    // define the position of the light
    teapot_light.pos =  glm::vec3(0.0f, -3.0f, 3.0f);
    teapot_light.center =  glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 light_dir = teapot_light.center - teapot_light.pos;

    teapot_light.diffuse_mat = glm::vec3(1.0, 0.0, 0.0);
    teapot_light.diffuse_int = 1.0;
    teapot_light.ambient_mat = glm::vec3(1.0, 0.0, 0.0);
    teapot_light.ambient_int = 0.2;
    teapot_light.specular_mat = glm::vec3(1.0, 1.0, 1.0);
    teapot_light.specular_int = 0.6;
    teapot_light.specular_s = 6.0;

    // place the light into the position of the camera. 
    viewMatrixPrerenderer = glm::lookAt(teapot_light.pos, teapot_light.center, glm::vec3(0.0f, 1.0f, 0.0f));

	// Projection transformations
    // Keep in mind that the z-buffer is not linear and that the resolution depends on the near and far clipping planes. 
	projectionMatrixPrerenderer = glm::perspective(1.57f, (float)400 / (float)400, 1.0f, 100.f);
	

    // Load the shader program
    teapot0d.shader_program_id = cs557::LoadAndCreateShaderProgram("../lit_scene.vs", "../lit_scene.fs");

    glUseProgram(teapot0d.shader_program_id );
    glUniform3fv(glGetUniformLocation(teapot0d.shader_program_id , "light_position"), 1, &teapot_light.pos[0]);
    glUniform3fv(glGetUniformLocation(teapot0d.shader_program_id , "light_direction"), 1, &light_dir[0]);
    glUniform3fv(glGetUniformLocation(teapot0d.shader_program_id , "diffColor"), 1, &teapot_light.diffuse_mat[0]);
    glUniform1f(glGetUniformLocation(teapot0d.shader_program_id , "diffInt"), teapot_light.diffuse_int);
    glUniform3fv(glGetUniformLocation(teapot0d.shader_program_id , "ambColor"), 1, &teapot_light.ambient_mat[0]);
    glUniform1f(glGetUniformLocation(teapot0d.shader_program_id , "ambInt"), teapot_light.ambient_int);
    glUniform3fv(glGetUniformLocation(teapot0d.shader_program_id , "specColor"), 1, &teapot_light.specular_mat[0]);
    glUniform1f(glGetUniformLocation(teapot0d.shader_program_id , "specInt"), teapot_light.specular_int);
    glUniform1f(glGetUniformLocation(teapot0d.shader_program_id , "shininess"), teapot_light.specular_s);

	 // load a teapot from file 
    teapot0.create("../teapot_t.obj", teapot0d.shader_program_id );
	teapot0d.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 


	// This function is part of RenderToTexture.h
	// It creates a texture as target for a pre-render step. 
	// The texture is linked to a framebuffer object (fbo), memory to store the image into.
	// This fbo is linked to a texture, so that we can use it in the  scene.
	CreateRenderToTexture(texture_width, texture_height, fboHidden, color_texture_idx, depth_texture_idx);

	// Reset to the regular buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(0);

}

/*
This function creates the scene with all visble objects, those, which are part of this scene.
*/
void CreateShadowReceiver(void)
{

	// Projection transformations
	projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
	viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -4.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    //------------------------------------------------
    // create a coordinate system
    coordinateSystem.create(2.5);
	modelMatrixCoord = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 


    //------------------------------------------------
    // Create a plane

    // Load the shader program
    plane0d.shader_program_id = cs557::LoadAndCreateShaderProgram("../shadow_scene.vs", "../shadow_scene.fs");

    // create the geometry
    plane0.create(8.0, 8.0, plane0d.shader_program_id);
	plane0d.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, 0.0f, 0.0f)); 

    // Load the texture and create it.
    // Note that this function bind the texture to texture unit GL_TEXTURE0 
    glUseProgram(plane0d.shader_program_id);

    //  assign the pre-render depth matrix as to the shadow receiver
	plane0d.texture_id = depth_texture_idx;

    // Activate the texture unit and bind the texture. 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, plane0d.texture_id);

    // Fetch the texture location and set the parameter to 0.
    // Note that 0 is the number of the texture unit GL_TEXTURE0.
    int texture_location = glGetUniformLocation(plane0d.shader_program_id, "tex");
    glUniform1i(texture_location, 0);

    // set the mvp matrix 
    glUniformMatrix4fv( glGetUniformLocation(plane0d.shader_program_id, "shadow_map_p"), 1, GL_FALSE, &projectionMatrixPrerenderer[0][0]);
    glUniformMatrix4fv( glGetUniformLocation(plane0d.shader_program_id, "shadow_map_v"), 1, GL_FALSE, &viewMatrixPrerenderer[0][0]);
    
	glUseProgram(0);

}





//------------------------------------------------------------
//
//	INIT THE SCENE

void Init(void)
{

    // Init the GLFW Window
    window = cs557::initWindow();

    // set a keyboard callback
    glfwSetKeyCallback(window, my_key_callback);

    // Initialize the GLEW apis
    cs557::initGlew();


	// Create the prerenderer scene  with the object that casts the shadow
	// The content is rendered into a texture
	CreateShadowCaster();

	// Create the scene with all visible objects which receive the shadow. 
    CreateShadowReceiver();

    // Create some content that helps to debug the scene
    CreateHelperContent();

}

//------------------------------------------------------------
//
//	RENDER THE SCENE

void Draw(void)
{

    

    // Enable depth test
    glEnable(GL_DEPTH_TEST); // ignore this line

    while (!glfwWindowShouldClose(window))
    {

        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR, 0, clear_color);
        glClearBufferfv(GL_DEPTH, 0, clear_depth);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects

        // update the camera values.
        // Note that this line changes the view matrix.
        glm::mat4 rotated_view =   viewMatrix * cs557::GetTrackball().getRotationMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) ;



		//-----------------------------------------------------------------------------
		// 
		// Draw the shadow caster scene

		// Bind the frame buffer object
		glBindFramebuffer(GL_FRAMEBUFFER, fboHidden);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set the viewport. It must match the texture size.
		glViewport(0, 0, texture_width, texture_height);

		// Clear the entire buffer with our green color.
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);

		// activate the shader program
		glUseProgram(teapot0d.shader_program_id);

        // update the light position
        viewMatrixPrerenderer = glm::lookAt(teapot_light.pos, teapot_light.center, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniform3fv(glGetUniformLocation(teapot0d.shader_program_id , "light_position"), 1, &teapot_light.pos[0]);

		double t = glfwGetTime();
		teapot0d.model_matrix = glm::rotate(  (float)2.0 * (float) sin((double)t), glm::vec3(0.0f, 0.0f, 1.0f)); 


		// draw the teapot
		teapot0.draw(projectionMatrixPrerenderer, viewMatrixPrerenderer, teapot0d.model_matrix);

		// Reset to the regular buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//-----------------------------------------------------------------------------
		//
		// Draw the visible scene

		// reset the viewport to the previouse size
		// Note that the opperating system zoom factor may interfer with this setting. 
		// Check your system scale and set it accordingly 
		int OSZoom = 2.0;
		glViewport(0, 0, OSZoom * 1280, OSZoom *1024); // the defaut window is set to this size


        // Object 0
        // This draws a coordinate system
        coordinateSystem.draw(projectionMatrix, rotated_view, modelMatrixCoord);


		// activate and bind the texture
		glUseProgram(plane0d.shader_program_id);

         // set the mvp matrix 
        glUniformMatrix4fv( glGetUniformLocation(plane0d.shader_program_id, "shadow_map_p"), 1, GL_FALSE, &projectionMatrixPrerenderer[0][0]);
        glUniformMatrix4fv( glGetUniformLocation(plane0d.shader_program_id, "shadow_map_v"), 1, GL_FALSE, &viewMatrixPrerenderer[0][0]);

		glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, plane0d.texture_id);

        // draw the plane
        plane0.draw(projectionMatrix, rotated_view, plane0d.model_matrix);

    
         // draw the teapot
        sphered.model_matrix = glm::translate(glm::mat4(1.0f), teapot_light.pos);  // need to be updated in case the light moves
		teapot0.draw(projectionMatrix, rotated_view, teapot0d.model_matrix);

     
        // draw the sphere
        sphere.draw(projectionMatrix, rotated_view, sphered.model_matrix);


        glUseProgram(0);

        //-----------------------------------------------------------------------------
		//
		// Draw helper content
        glUseProgram(displayd.shader_program_id);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, displayd.texture_id);

        display.draw(display_p, display_vm, displayd.model_matrix);


       

        
        glUseProgram(0);


        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}








//------------------------------------------------------------
//
//	RENDER THE SCENE
int main(int argc, const char *argv[])
{
    cout << "This code example demonstrates a multipass renderer." << endl;
    cout << "Rafael Radkowski\nrafael@iastate.edu\nIowa State University" << endl;
    cout << "This example is part of ME/CS/CPRE557" << endl;


	// Init the scene 
    Init();

	// Draw the scene
    Draw();

    return 1;
}
