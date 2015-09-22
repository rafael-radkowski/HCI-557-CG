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


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600



using namespace std;


/* A trackball to move and rotate the camera view */
static Trackball trackball( WINDOW_WIDTH, WINDOW_HEIGHT, 0.4f, true, true );




static const string vs_string =
    "#version 410 core                                                 \n"
    "                                                                   \n"
    "uniform mat4 projectionMatrix;                                    \n"
    "uniform mat4 viewMatrix;                                           \n"
    "uniform mat4 modelMatrix;                                          \n"
    "in vec3 in_Position;                                               \n"
    "                                                                   \n"
    "in vec3 in_Color;                                                  \n"
    "out vec3 pass_Color;                                               \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
    "    pass_Color = in_Color;                                         \n"
    "}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string  =
    "#version 410 core                                                 \n"
    "                                                                  \n"
    "in vec3 pass_Color;                                                 \n"
    "out vec4 color;                                                    \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "    color = vec4(pass_Color, 1.0);                               \n"
    "}                                                                 \n";


glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix


// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;

unsigned int vaoID[1]; // Our Vertex Array Object

unsigned int vboID[2]; // Our Vertex Buffer Object


// This is the callback we'll be registering with GLFW for errors.
// It'll just print out the error to the STDERR stream.
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}


// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}



/* In GLFW mouse callback */
void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods ){
    
    
    trackball.mouseButtonCallback( window, button, action, mods );
}

/* In GLFW curser callback */
void cursorCallback( GLFWwindow *window, double x, double y ) {
    

    trackball.cursorCallback( window, x, y );
}



void createBox(void)
{
    float* vertices = new float[48];  // Vertices for our square
    float *colors = new float[48]; // Colors for our vertices
    
    // FRONT
    vertices[0] = -0.5; vertices[1] = 0.5; vertices[2] = 0.5; // Top left corner
    colors[0] = 1.0; colors[1] = 0.0; colors[2] = 0.0; // Top left corner
    
    vertices[3] = -0.5; vertices[4] = -0.5; vertices[5] = 0.5; // Bottom left corner
    colors[3] = 1.0; colors[4] = 1.0; colors[5] = 1.0; // Bottom left corner
    
    vertices[6] = 0.5; vertices[7] = 0.5; vertices[8] = 0.5; // Top Right corner
    colors[6] = 0.0; colors[7] = 1.0; colors[8] = 0.0; // Top Right corner
    
    vertices[9] = 0.5; vertices[10] = -0.5; vertices[11] = 0.5; // Bottom right corner
    colors[9] = 0.0; colors[10] = 0.0; colors[11] = 1.0; // Bottom right corner
    
    // RIGHT SIDE
    vertices[12] = 0.5; vertices[13] = 0.5; vertices[14] = -0.5; // Top right corner
    colors[12] = 1.0; colors[13] = 1.0; colors[14] = 0.0; // Top right corner
    
    vertices[15] = 0.5; vertices[16] = -0.5; vertices[17] = -0.5; // Bottom right corner
    colors[15] = 1.0; colors[16] = 0.0; colors[17] = 1.0; // Bottom right corner
    
    // BACK
    vertices[18] = -0.5; vertices[19] = 0.5; vertices[20] = -0.5; // Top left  corner
    colors[18] = 0.0; colors[19] = 0.5; colors[20] = 0.5; // Top left corner
    
    vertices[21] = -0.5; vertices[22] = -0.5; vertices[23] = -0.5; // Bottom left  corner
    colors[21] = 0.5; colors[22] = 0.5; colors[23] = 0.0; // Bottom left corner
    
    // LEFT
    vertices[24] = -0.5; vertices[25] = 0.5; vertices[26] = 0.5; // Top left  corner
    colors[24] = 1.0; colors[25] = 0.0; colors[26] = 0.0; // Top left corner
    
    vertices[27] = -0.5; vertices[28] = -0.5; vertices[29] = 0.5; // Bottom left  corner
    colors[27] = 1.0; colors[28] = 0.0; colors[29] = 0.0; // Bottom left corner
    
     // BOTTOM
    vertices[30] = 0.5; vertices[31] = -0.5; vertices[32] = -0.5; // Top left  corner
    colors[30] = 1.0; colors[31] = 0.0; colors[32] = 0.0; // Top left corner
    
    vertices[33] = 0.5; vertices[34] = -0.5; vertices[35] = 0.5; // Bottom left  corner
    colors[33] = 1.0; colors[34] = 1.0; colors[35] = 0.0; // Bottom left corner
    
    //TOP
    vertices[36] = -0.5; vertices[37] = 0.5; vertices[38] = 0.5; // Top left  corner
    colors[36] = 0.0; colors[37] = 1.0; colors[38] = 0.0; // Top left corner
    
    vertices[39] = -0.5; vertices[40] = 0.5; vertices[41] = -0.5; // Bottom left  corner
    colors[39] = 0.0; colors[40] = 0.3; colors[41] = 0.0; // Bottom left corner
    
    vertices[42] = 0.5; vertices[43] = 0.5; vertices[44] = 0.5; // Top left  corner
    colors[42] = 0.0; colors[43] = 0.8; colors[44] = 0.0; // Top left corner
    
    vertices[45] = 0.5; vertices[46] = 0.5; vertices[47] = -0.5; // Bottom left  corner
    colors[45] = 0.0; colors[46] = 0.5; colors[47] = 0.0; // Bottom left corner
    
    
    glGenVertexArrays(1, &vaoID[0]); // Create our Vertex Array Object
    glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(2, vboID); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
  
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(0); // Disable our Vertex Array Object
    
    
    //Color
    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(1); // Enable the second vertex attribute array
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    
    delete [] vertices; // Delete our vertices from memory
}


void setupScene(void) {
    createBox();
}





/*!
 This function initializes the GLFW window
 */
int initWindow(void)
{
    // Initialize GLFW, and if it fails to initialize for any reason, print it out to STDERR.
    if (!glfwInit()) {
        fprintf(stderr, "Failed initialize GLFW.");
        exit(EXIT_FAILURE);
    }
    
    // Set the error callback, as mentioned above.
    glfwSetErrorCallback(error_callback);
    
    // Set up OpenGL options.
    // Use GLFW verion 3.3,
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    // Create a window to put our stuff in.
    window = glfwCreateWindow(800, 600, "Hello OpenGL", NULL, NULL);
    
    // If the window fails to be created, print out the error, clean up GLFW and exit the program.
    if( window == NULL ){
        cout <<  "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible." << endl;
        glfwTerminate();
        system("pause");
        exit(-1);
    }
    
    // Use the window as the current context (everything that's drawn will be place in this window).
    glfwMakeContextCurrent(window);
    
    // set the cursor callback
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    return 1;
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
        cout <<  "Failed to initialize GLEW\n" << endl;
        system("pause");
        return -1;
    }
    
    cout <<  "OpenGL version supported by this platform " << glGetString(GL_VERSION) <<  endl;
}






int main(int argc, const char * argv[])
{
    // Init the GLFW Window
    initWindow();
    
    // Set the keyboard callback so that when we press ESC, it knows what to do.
    glfwSetKeyCallback(window, key_callback);
    
    // Init the glew api
    initGlew();
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    static const string vertex_code = vs_string;
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = fs_string;
    static const char * fs_source = fragment_code.c_str();

    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);
    
     //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
   
    /// IGNORE THE NEXT PART OF THIS CODE
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    
    projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
    
    
     
    setupScene();
    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");
    
    

    
    // Enable depth test
    glEnable(GL_DEPTH_TEST); // ignore this line
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
       
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // update the virtual camera
        //updateCamera();


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This generate the object
        
        // Enable the shader program
        glUseProgram(program);
        
        /* Creating rotation matrices */
        glm::mat4 rotated_view = viewMatrix * trackball.getRotationMatrix();
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
        
        // Bind the buffer and switch it to an active buffer
        glBindVertexArray(vaoID[0]);
        
        // Draw the triangles
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 16);
        // Unbind our Vertex Array Object
        glBindVertexArray(0);
        
        // Unbind the shader program
        glUseProgram(0);
        
        //// This generate the object
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    // Program clean up when the window gets closed.
    glDeleteVertexArrays(1, &vaoID[0]);
    glDeleteProgram(program);
}

