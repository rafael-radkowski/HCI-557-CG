#include "ModelBSMorphing.h"

#include "OBJLoader.h"


namespace cs557_ModelBSMorphing
{

	static const string vs_string_410 =
		"#version 410 core                                                 \n"
		"                                                                   \n"
		"uniform mat4 projectionMatrix;                                    \n"
		"uniform mat4 viewMatrix;                                           \n"
		"uniform mat4 modelMatrix;                                          \n"
		"in vec3 in_Position;                                               \n"
		"in vec2 in_Texture;                                                \n"
		"in vec3 in_Normal;                                                  \n"
		"out vec3 pass_Color;                                               \n"
		"out vec2 pass_Texture;												\n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
		"    pass_Color = in_Normal;                                         \n"
		"    pass_Texture = in_Texture;                                         \n"
		"}                                                                 \n";

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	static const string fs_string_410 =
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"in vec3 pass_Color;                                                 \n"
		"in vec2 pass_Texture;												\n"
		"out vec4 color;                                                    \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(pass_Color, 1.0);                               \n"
		"}                                                                 \n";



};



using namespace cs557_ModelBSMorphing;



	
/*
Load an OBJ model from file
@param path_and_filename1 - path and filename of the first model to load
@param path_and_filename2 - path and filename of the second model to load. 
@param shader_program - overwrite the default shader program by passing a hander to the constructor
*/
void cs557::ModelBSMorphing::create(string path_and_filename_1, string path_and_filename_2, unsigned int shader_program )
{
	program = -1;
	_N = 0;
	_I = 0;

	// This overwrite the default shader program
	if(shader_program != -1){
		program = shader_program;
	}


	float center_x = 0.0;
	float center_y = 0.0;
	float center_z = 0.0;



	//std::vector<std::pair<glm::vec3, glm::vec2> > points; // points and texture coordinates
	std::vector<MorphData > points;
	//std::vector<glm::vec3> normals;
	std::vector<int> indices;

	

	// create a shader program only if the progrm was not overwritten. 
	if(program == -1)
		program = cs557::CreateShaderProgram(vs_string_410, fs_string_410);


	// Find the id's of the related variable name in your shader code. 
	projMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
	modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader

	// Locations in the shader for model number 1
	int pos_location1 = glGetAttribLocation(program, "in_Position");
	int norm_location1 = glGetAttribLocation(program, "in_Normal");
	int tex_location1 = glGetAttribLocation(program, "in_Texture");

	// locations in the shader for model number 2
	int pos_location2 = glGetAttribLocation(program, "in_Position2");
	int norm_location2 = glGetAttribLocation(program, "in_Normal2");
	int tex_location2 = glGetAttribLocation(program, "in_Texture2");

	// the morphing blending index 
	blending_index_location = glGetUniformLocation(program, "blending_index");


	// Load the geometry from file. 
	objl::Loader loader1, loader2;
	loader1.LoadFile(path_and_filename_1);
	loader2.LoadFile(path_and_filename_2);

	int size1 = loader1.LoadedMeshes.size();
	int size2 = loader2.LoadedMeshes.size();

	// Check if both models have the same number of vertices
	if(size1 != size2)
	{
		std::cout << "Both models are of different size.\nCheck the models, morphing requires the same number of vertices." << std::endl;
	}

	if(size1 > 0)
	{


		// Fill the points data strcutre with data for morphing
		objl::Mesh curMesh1 = loader1.LoadedMeshes[0];
		objl::Mesh curMesh2 = loader2.LoadedMeshes[0];

		for (int j = 0; j < curMesh1.Vertices.size(); j++)
		{
			MorphData m;
			m.p1 = glm::vec3(curMesh1.Vertices[j].Position.X, curMesh1.Vertices[j].Position.Y, curMesh1.Vertices[j].Position.Z);
			m.t1 = glm::vec2(curMesh1.Vertices[j].TextureCoordinate.X, curMesh1.Vertices[j].TextureCoordinate.Y );
			m.n1 = glm::vec3(curMesh1.Vertices[j].Normal.X, curMesh1.Vertices[j].Normal.Y, curMesh1.Vertices[j].Normal.Z);

			m.p2 = glm::vec3(curMesh2.Vertices[j].Position.X, curMesh2.Vertices[j].Position.Y, curMesh2.Vertices[j].Position.Z);
			m.t2 = glm::vec2(curMesh2.Vertices[j].TextureCoordinate.X, curMesh2.Vertices[j].TextureCoordinate.Y );
			m.n2 = glm::vec3(curMesh2.Vertices[j].Normal.X, curMesh2.Vertices[j].Normal.Y, curMesh2.Vertices[j].Normal.Z);

			points.push_back(m);

		}
		

		for (int j = 0; j < curMesh1.Indices.size(); j++)
		{
			indices.push_back(curMesh1.Indices[j]);
		}

		_I = curMesh1.Indices.size();
		_N = points.size();

		//-----------------------------------------------------------------------------------------------------------------------------
		// create a vertex buffer object
		// Note that this creates one huge vertex buffer with all the data embedded
		// [ point 1, uv1, normal 1, point 2, uv2, normal 2]

		glGenVertexArrays(1, (GLuint*)vaoID); // Create our Vertex Array Object
		glBindVertexArray(*vaoID); // Bind our Vertex Array Object so we can use it


		if (vaoID[1] == -1){
			std::cout << "[ERROR] - Vertex array object was not generated." << std::endl;
		}

		glGenBuffers(2, (GLuint*)vboID); // Generate our Vertex Buffer Object for points, normals, and texture coordinates
		glGenBuffers(1, (GLuint*)iboID);

		if (vboID[0] == -1 || vboID[1] == -1 ||  iboID[0] == -1){
			std::cout << "[ERROR] - One or both vertex buffer objects were not generated." << std::endl;
		}


		// All data into one buffer
		glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
		glBufferData(GL_ARRAY_BUFFER, _N *  16 * sizeof(GLfloat), &points[0].p1.x, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW

		// vertices
		glVertexAttribPointer((GLuint)pos_location1, 3, GL_FLOAT, GL_FALSE, 16*sizeof(GLfloat), 0); // Set up our vertex attributes pointer
		glEnableVertexAttribArray(pos_location1); // Enable the Vertex Array Object

		// Texture Coordinates
		glVertexAttribPointer((GLuint)tex_location1, 2, GL_FLOAT, GL_FALSE, 16 *sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat))); // Set up our texture attributes pointer
		glEnableVertexAttribArray(tex_location1); // Enable the Vertex Array Object

		// Nornals Coordinates
		glVertexAttribPointer((GLuint)norm_location1, 3, GL_FLOAT, GL_FALSE, 16 *sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat))); // Set up our texture attributes pointer
		glEnableVertexAttribArray(norm_location1); // Enable the Vertex Array Object

		// vertices
		glVertexAttribPointer((GLuint)pos_location2, 3, GL_FLOAT, GL_FALSE, 16*sizeof(GLfloat), (const GLvoid*)(8 * sizeof(GLfloat))); // Set up our vertex attributes pointer
		glEnableVertexAttribArray(pos_location2); // Enable the Vertex Array Object

		// Texture Coordinates
		glVertexAttribPointer((GLuint)tex_location2, 2, GL_FLOAT, GL_FALSE, 16 *sizeof(GLfloat), (const GLvoid*)(11 * sizeof(GLfloat))); // Set up our texture attributes pointer
		glEnableVertexAttribArray(tex_location2); // Enable the Vertex Array Object

		// Nornals Coordinates
		glVertexAttribPointer((GLuint)norm_location2, 3, GL_FLOAT, GL_FALSE, 16 *sizeof(GLfloat), (const GLvoid*)(13 * sizeof(GLfloat))); // Set up our texture attributes pointer
		glEnableVertexAttribArray(norm_location2); // Enable the Vertex Array Object


		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _I * sizeof(unsigned int),
					 &indices[0], GL_STATIC_DRAW);


		glBindVertexArray(0); // Disable our Vertex Buffer Object

	}
}


/*
Draw the coordinate system
@param viewMatrix - a view matrix object
@param modelMatrix - a model matrix object.
*/
void cs557::ModelBSMorphing::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix, float time_index)
{

	// Enable the shader program
	glUseProgram(program);



	// this changes the camera location
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // send the view matrix to our shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // send the model matrix to our shader
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // send the projection matrix to our shader

	// add the blending index to the code
	blending_index = time_index;
	blending_index = std::min( std::max( float(time_index), 0.0f ), 1.0f );

	glUniform1f(blending_index_location, blending_index );


	 // Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[0]);

	// Draw the triangles
 	glDrawElements(GL_TRIANGLES, _N, GL_UNSIGNED_INT, 0);
	

	// Unbind our Vertex Array Object
	glBindVertexArray(0);

	// Unbind the shader program
	glUseProgram(0);


}



