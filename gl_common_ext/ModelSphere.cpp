#include "ModelSphere.h"



namespace cs557_sphere
{

	static const string vs_string_box_410 =
		"#version 410 core                                                 \n"
		"                                                                   \n"
		"uniform mat4 projectionMatrix;                                    \n"
		"uniform mat4 viewMatrix;                                           \n"
		"uniform mat4 modelMatrix;                                          \n"
		"in vec3 in_Position;                                               \n"
		"                                                                   \n"
		"in vec3 in_Normal;                                                  \n"
		"out vec3 pass_Color;                                               \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
		"    pass_Color = in_Normal;                                         \n"
		"}                                                                 \n";

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	static const string fs_string_box_410 =
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"in vec3 pass_Color;                                                 \n"
		"out vec4 color;                                                    \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(pass_Color, 1.0);                               \n"
		"}                                                                 \n";



};



using namespace cs557_sphere;



	
/*
Create a simple coordinate system in the centroid
@param radius - the radios of the sphere
*/
void cs557::Sphere::create(float radius, int segments, int rows,  int shader_program)
{
	program = -1;

	// This overwrite the default shader program
	if(shader_program != -1){
		program = shader_program;
		cout << "Use default shader program " << endl;
	}

	_radius = radius;

	float center_x = 0.0;
	float center_y = 0.0;
	float center_z = 0.0;

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;

	// This creates the geometry
	make_geometry(glm::vec3(center_x, center_y, center_z), _radius, segments, rows, 
							points, normals);

	_N = points.size();

	// create a vertex buffer object
	CreateVertexObjects33(vaoID, vboID, &points[0].x, &normals[0].x, _N);


	// create a shader program only if the progrm was not overwritten. 
	if(program == -1)
		program = cs557::CreateShaderProgram(vs_string_box_410, fs_string_box_410);


	// Find the id's of the related variable name in your shader code. 
	projMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
	modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
	int pos_location = glGetAttribLocation(program, "in_Position");
	int col_location = glGetAttribLocation(program, "in_Normal");


	glBindAttribLocation(program, pos_location, "in_Position");
	glBindAttribLocation(program, col_location, "in_Normal");

}


/*
Draw the coordinate system
@param viewMatrix - a view matrix object
@param modelMatrix - a model matrix object.
*/
void cs557::Sphere::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 modelMatrix )
{

	// Enable the shader program
	glUseProgram(program);



	// this changes the camera location
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // send the view matrix to our shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // send the model matrix to our shader
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // send the projection matrix to our shader


	 // Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[0]);

	// Draw the triangles
	glDrawArrays(GL_TRIANGLE_STRIP, 0, _N);
	

	// Unbind our Vertex Array Object
	glBindVertexArray(0);

	// Unbind the shader program
	glUseProgram(0);


}




void cs557::Sphere::make_geometry(glm::vec3 center, double radius, double segments, double rows, 
									std::vector<glm::vec3> &spherePoints, std::vector<glm::vec3> &normals)
{
    const double PI = 3.141592653589793238462643383279502884197;
    spherePoints.clear();
    
    int current_size = 0;
    for (double theta = 0.; theta < PI; theta += PI/float(rows)) // Elevation [0, PI]
    {
        //double theta = 1.57;
        double theta2 = theta + PI/float(rows);
        int count = 0;
        int count_row = 0;
        current_size = spherePoints.size();
        
        // Iterate through phi, theta then convert r,theta,phi to  XYZ
        for (double phi = 0.; phi < 2*PI + PI/float(segments) ; phi += PI/float(segments)) // Azimuth [0, 2PI]
        {
            
            glm::vec3 point;
            point.x = radius * cos(phi) * sin(theta) + center.x;
            point.y = radius * sin(phi) * sin(theta) + center.y;
            point.z = radius            * cos(theta) + center.z;
            spherePoints.push_back(point); count++;
            
            
            glm::vec3  point2;
            point2.x = radius * cos(phi) * sin(theta2) + center.x;
            point2.y = radius * sin(phi) * sin(theta2) + center.y;
            point2.z = radius            * cos(theta2) + center.z;
            spherePoints.push_back(point2); count++;
            
            glm::vec3  normal;
            normal.x =  cos(phi) * sin(theta);
            normal.y =  sin(phi) * sin(theta);
            normal.z =  cos(theta);
            normals.push_back(normal);
            
            glm::vec3  normal2;
            normal2.x =  cos(phi) * sin(theta2);
            normal2.y =  sin(phi) * sin(theta2);
            normal2.z =  cos(theta2);
            normals.push_back(normal2);
            
            
        }
        if(count_row == 0) count_row = count;
    }
    return;
}