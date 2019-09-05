/*
Glfw_WIndow.

This program demonstrate how we can create and open and GLFW Window
to render OpenGL content.  GLFW requrests window resources from your 
operating system and provides a 'context' for  graphics content. 

The code and the class demonstrate:
- the usage of include preprocessor instructions to link the required header files. 
- the usage of the GLFW APIs glfwInit(), glfwWindowHint(), glfwCreateWindow(), glfwMakeContextCurrent(), glfwSwapBuffers()
- the usage of the GLEW APIs glClearBufferfv(), glSwapBuffers()
- the graphics initialization process
- the empty render loop. 

Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1(515) 294 7044
August 8, 2015 
MIT License

This code is part of the Iowa State U. course CS/CPRE/ME 557 - Computer Graphics and Geometric Modeling. 

----------------------------------------------------------------
Last edited:
Sep 5, 2019, RR
- Revised the comments in this program
*/

// All #include instructions are preprocessor instructions. 
// The copy & past the code in the files <file> or "file" to this location 
// when the build tool starts its work.  

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// GLFW include
#include <GLFW/glfw3.h>

// Namessapce definitions to simplify variable writing. 
// We use the std namespace (the standard template library for C/C++) and glm. 
using namespace std;
using namespace glm;

// The variable which contains the main window. 
GLFWwindow* window;

// The main-function is the general entry point for an operating system.
// The compiler 'translates' main into a unique start point. 
int main( void )
{
    //-----------------------------------------------------------------------------------
    // Initialization

    // Step 1. 
    // This function initializes the GLFW library
    // and allocates the required resources. 
    if( !glfwInit() )
    {
        cout <<  "Failed to initialize GLFW\n" << endl;
        system("pause");
        return -1;
    }
    
    // Step 2. 
    // glfwWindowHints gives glfw information about the 
    // resources and glfw version we intend to request. 
    // Note that these hints are used when calling glfwCreateWindow.
    // Also, it is not guranteed that we get them since they depend on your
    // available computer resources. 
    // Set the version which we expect, here, 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    // Step 3. 
    // Create the window. 
    // The function returns a pointer to the resources. 
    // If the function fails, check your hints. You may want to remove them. 
    window = glfwCreateWindow( 1024, 768, "Simple Window", NULL, NULL);
    if( window == NULL ){
        cout <<  "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible." << endl;
        glfwTerminate();
        system("pause");
        return -1;
    }
    
    // Step 4. 
    // Make this window the current context. 
    // In other words, switch this window to become the active OpenGL window. 
    glfwMakeContextCurrent(window);
    
    
    // Step 5. 
    // Initialize GLEW
    // Thus function must be called before you can call any OpenGL instruction. 
    if (glewInit() != GLEW_OK) {
        cout <<  "Failed to initialize GLEW\n" << endl;
        system("pause");
        return -1;
    }
    
    // Enable an imput mode. Sticky keys allow you to 
    // press multiple key in quick succession and we still get the message. 
    // Here, it allows us to disable the application by pressing esc. 
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    

    // Step 6. 
    // Define the empty buffers - The clear color and the depth color. 
    // The depth buffer will be explained at a later time. 
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    
    //-----------------------------------------------------------------------------------
    // The OpenGL main draw loop. 
    
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 )
    {
        // Draw loop step 1:
        // We clear the render buffers by setting a defined 'empty' value. 
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // Draw loop step 2:
        // We will later draw the content at this location.
        // [this is the place where we need to draw some objects]
        
        // Draw loop step 3:
        // Swap buffers
        glfwSwapBuffers(window);

        // and poll all hardware events, e.g., keyboard hits. 
        glfwPollEvents();
        
    } // Check if the ESC key was pressed or the window was closed
    
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}

