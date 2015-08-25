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


using namespace std;

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




// Define some of the global variables we're using for this sample
GLuint program;
GLuint vao;

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


void createSquare(void)
{
    float* vertices = new float[18];  // Vertices for our square
    float *colors = new float[18]; // Colors for our vertices
    
    vertices[0] = -0.5; vertices[1] = -0.5; vertices[2] = 0.5; // Bottom left corner
    colors[0] = 1.0; colors[1] = 1.0; colors[2] = 1.0; // Bottom left corner
    
    vertices[3] = -0.5; vertices[4] = 0.5; vertices[5] = 0.5; // Top left corner
    colors[3] = 1.0; colors[4] = 0.0; colors[5] = 0.0; // Top left corner
    
    vertices[6] = 0.5; vertices[7] = 0.5; vertices[8] = 0.5; // Top Right corner
    colors[6] = 0.0; colors[7] = 1.0; colors[8] = 0.0; // Top Right corner
    
    vertices[9] = 0.5; vertices[10] = -0.5; vertices[11] = 0.5; // Bottom right corner
    colors[9] = 0.0; colors[10] = 0.0; colors[11] = 1.0; // Bottom right corner
    
    vertices[12] = -0.5; vertices[13] = -0.5; vertices[14] = 0.5; // Bottom left corner
    colors[12] = 1.0; colors[13] = 1.0; colors[14] = 1.0; // Bottom left corner
    
    vertices[15] = 0.5; vertices[16] = 0.5; vertices[17] = 0.5; // Top Right corner
    colors[15] = 0.0; colors[16] = 1.0; colors[17] = 0.0; // Top Right corner
    
    
    glGenVertexArrays(2, &vaoID[0]); // Create our Vertex Array Object
    glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(2, vboID); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
  
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(0); // Disable our Vertex Array Object
    
    
    //Color
    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(1); // Enable the second vertex attribute array
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    
    delete [] vertices; // Delete our vertices from memory
}


void setupScene(void) {
    createSquare();
}




int main(int argc, const char * argv[])
{
    // Initialize GLFW, and if it fails to initialize for any reason, print it out to STDERR.
    if (!glfwInit()) {
        fprintf(stderr, "Failed initialize GLFW.");
        exit(EXIT_FAILURE);
    }
    
    // Set the error callback, as mentioned above.
    glfwSetErrorCallback(error_callback);
    
    // Set up OpenGL options.
    // Use OpenGL verion 4.1,
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // GLFW_OPENGL_FORWARD_COMPAT specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in the requested version of OpenGL is removed.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Indicate we only want the newest core profile, rather than using backwards compatible and deprecated features.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Make the window resize-able.
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Create a window to put our stuff in.
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello OpenGL", NULL, NULL);
    
    // If the window fails to be created, print out the error, clean up GLFW and exit the program.
    if(!window) {
        fprintf(stderr, "Failed to create GLFW window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // Use the window as the current context (everything that's drawn will be place in this window).
    glfwMakeContextCurrent(window);
    
    // Set the keyboard callback so that when we press ESC, it knows what to do.
    glfwSetKeyCallback(window, key_callback);
    
    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
    
    // Makes sure all extensions will be exposed in GLEW and initialize GLEW.
    glewExperimental = GL_TRUE;
    glewInit();
    
    // Shaders is the next part of our program. Notice that we use version 410 core. This has to match our version of OpenGL we are using, which is the core profile in version 4.1, thus 410 core.
    
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
    
    // Generate vertex arrays for our program. More explanation on this will come in the future.
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);
    
    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);
    
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    
    
    
    projectionMatrix = glm::perspective(1.57f, (float)800 / (float)600, 0.1f, 100.f);
   // viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f)); // Create our view matrix which will translate us back 5 units
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
    
    
     
    setupScene();
    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");
    
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
       
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR, 0, clear_color);
        
        // Draw our triangles
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glUseProgram(program);
        
        glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object
        
        glDrawArrays(GL_TRIANGLES, 0, 6); // Draw our square
        
        glBindVertexArray(0); // Unbind our Vertex Array Object
        
        glUseProgram(0);
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Program clean up when the window gets closed.
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
}

