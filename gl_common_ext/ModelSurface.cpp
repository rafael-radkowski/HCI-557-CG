

#include "ModelSurface.h"

#include <algorithm>

namespace cs557_surface
{

	static const string vs_string_surface_410 =
		"#version 410 core                                                 \n"
		"                                                                   \n"
		"uniform mat4 projectionMatrix;                                    \n"
		"uniform mat4 viewMatrix;                                           \n"
		"uniform mat4 modelMatrix;                                          \n"
		"in vec3 in_Position;                                               \n"
		"in vec2 in_Texture;                                                 \n"
		"in vec3 in_Normal;                                                  \n"
		"out vec3 pass_Normal;                                              \n"
		"out vec2 pass_Texture;												\n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
		"    pass_Normal = in_Normal;                                         \n"
		"	 pass_Texture = in_Texture;										\n"
		"}                                                                 \n";

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	static const string fs_string_surface_410 =
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"in vec3 pass_Normal;                                                 \n"
		"in vec2 pass_Texture;												\n"
		"out vec4 color;                                                    \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(pass_Texture, 0.0, 1.0);                               \n"
		"}                                                                 \n";



};


using namespace cs557_surface;







/*!
 Init the geometry object
 */
void ModelSurface::create(vector<glm::vec3> &vertices, vector<glm::vec3> &normals,  int shader_program)
{
    
    _vertices = vertices;
    _normals = normals;
    _program = -1;

    // This overwrite the default shader program
	if(shader_program != -1){
		_program = shader_program;
	}


    	// create a shader program only if the progrm was not overwritten. 
	if(_program == -1)
		_program = cs557::CreateShaderProgram(vs_string_surface_410, fs_string_surface_410);

    

    initVBO();
    
}








/*!
 Create the vertex buffer object for this element
 */
void ModelSurface::initVBO(void)
{
    _num_vertices = _vertices.size();
    
    // create memory for the vertices, etc.
    float* vertices = new float[_num_vertices * 3];
    float* normals = new float[_normals.size() * 3];

    
    // Copy all vertices
    for(int i=0; i<_vertices.size() ; i++)
    {
        glm::vec3 t = _vertices[i];
        for (int j=0; j<3; j++) {
            vertices[(i*3)+j] = t[j];
        }
    }
    
    // copy all normals
    for(int i=0; i<_normals.size() ; i++)
    {
        glm::vec3 n = _normals[i];
        for (int j=0; j<3; j++) {
            normals[(i*3)+j] = n[j];
        }
    }

    
    glGenVertexArrays(1, _vaoID); // Create our Vertex Array Object
    glBindVertexArray(_vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(2, _vboID); // Generate our Vertex Buffer Object

    glUseProgram(_program);

	int pos_location = glGetAttribLocation(_program, "in_Position");
	int normal_location = glGetAttribLocation(_program, "in_Normal");
	int texture_location = glGetAttribLocation(_program, "in_Texture");

    
    // vertices
    int locPos = glGetAttribLocation(_program, "in_Position");
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locPos); //
    
    
     // normals
    int locNorm = glGetAttribLocation(_program, "in_Normal");
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _normals.size() * 3 * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)locNorm, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locNorm); //
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object


	// create a vertex buffer object
	//CreateVertexObjects53(vaoID, vboID, vertices, normals, 4, 
	//					  pos_location, texture_location, normal_location);


	// Find the id's of the related variable name in your shader code. 
	projMatrixLocation = glGetUniformLocation(_program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	viewMatrixLocation = glGetUniformLocation(_program, "viewMatrix"); // Get the location of our view matrix in the shader
	modelMatrixLocation = glGetUniformLocation(_program, "modelMatrix"); // Get the location of our model matrix in the shader

	

	//glBindAttribLocation(program, pos_location, "in_Position");
	//glBindAttribLocation(program, texture_location, "in_Texture");
	//glBindAttribLocation(program, normal_location, "in_Normal");

	glUseProgram(0);


}





/*!
 Draw the objects
 */
void ModelSurface::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix)
{


    	// Enable the shader program
	glUseProgram(_program);



	// this changes the camera location
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // send the view matrix to our shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // send the model matrix to our shader
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // send the projection matrix to our shader


	 // Bind the buffer and switch it to an active buffer
	glBindVertexArray(_vaoID[0]);

	//glLineWidth((GLfloat)3.0);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

 
	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, _num_vertices);
	
	// Unbind our Vertex Array Object
	glBindVertexArray(0);

	// Unbind the shader program
	glUseProgram(0);

    
}



