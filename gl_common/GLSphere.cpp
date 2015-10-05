//
//  GLSphere.cpp
//  OpenGL_Transformations
//
//  Created by Rafael Radkowski on 9/12/15.
//
//

#include "GLSphere.h"





static const string vs_string_GLSphere_410 =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrixBox;                                    \n"
"uniform mat4 viewMatrixBox;                                           \n"
"uniform mat4 inverseViewMatrix;                                        \n"
"uniform mat4 modelMatrixBox;                                          \n"
"uniform vec3 diffuse_color;                                          \n"
"uniform vec3 ambient_color;                                          \n"
"uniform vec3 specular_color;                                          \n"
"uniform vec4 light_position;                                          \n"
"uniform float diffuse_intensity;                                          \n"
"uniform float ambient_intensity;                                          \n"
"uniform float specular_intensity;                                          \n"
"uniform float shininess;                                          \n"
"in vec3 in_Position;                                               \n"
"in vec3 in_Normal;                                                  \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"                                                                 \n"
"                                                                 \n"
"                                                                 \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    vec3 normal = normalize(in_Normal);                                                                        \n"
"    vec4 transformedNormal =  normalize(transpose(inverse(modelMatrixBox)) * vec4( normal, 1.0 ));             \n"
"    vec4 surfacePostion = modelMatrixBox * vec4(in_Position, 1.0);                             \n"
"                                                                                                               \n"
"    vec4 surface_to_light =   normalize( light_position -  surfacePostion );                         \n"
"                                                                                                               \n"
"    // Diffuse color                                                                                           \n"
"    float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);                           \n"
"    vec3 out_diffuse_color = diffuse_color  * diffuse_coefficient * diffuse_intensity;                         \n"
"                                                                                                               \n"
"    // Ambient color                                                                                           \n"
"    vec3 out_ambient_color = vec3(ambient_color) * ambient_intensity;                                          \n"
"                                                                                                               \n"
"    // Specular color                                                                                          \n"
"    vec3 incidenceVector = -surface_to_light.xyz;                                                              \n"
"    vec3 reflectionVector = reflect(incidenceVector, normal);                                   \n"
"    vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);             \n"
"    vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz);                                     \n"
"    float cosAngle = max( dot(surfaceToCamera, reflectionVector), 0.0);                                        \n"
"    float specular_coefficient = pow(cosAngle, shininess);                                                     \n"
"    vec3 out_specular_color = specular_color * specular_coefficient * specular_intensity;                      \n"
"                                                                                                               \n"
"    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);               \n"
"                                                                                                               \n"
"    pass_Color = vec3(out_diffuse_color + out_ambient_color + out_specular_color);                             \n"
"}                                                                                                              \n";



static const string vs_string_GLSphere_300 =
"#version 310 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrixBox;                                    \n"
"uniform mat4 viewMatrixBox;                                           \n"
"uniform mat4 modelMatrixBox;                                          \n"
"uniform mat4 inverseViewMatrix;                                        \n"
"uniform vec3 diffuse_color;                                          \n"
"uniform vec3 ambient_color;                                          \n"
"uniform vec3 specular_color;                                          \n"
"uniform vec4 light_position;                                          \n"
"uniform float diffuse_intensity;                                          \n"
"uniform float ambient_intensity;                                          \n"
"uniform float specular_intensity;                                          \n"
"uniform float shininess;                                          \n"
"in vec3 in_Position;                                               \n"
"in vec3 in_Normal;                                                  \n"
"in vec3 in_Color;                                                  \n"
"                                                                  \n"
"                                                                 \n"
"                                                                 \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    vec3 normal = normalize(in_Normal);                                                                        \n"
"    vec4 transformedNormal =  normalize(transpose(inverse(modelMatrixBox)) * vec4( normal, 1.0 ));             \n"
"    vec4 surfacePostion = viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);                             \n"
"                                                                                                               \n"
"    vec4 surface_to_light =   normalize( light_position -  surfacePostion );                         \n"
"                                                                                                               \n"
"    // Diffuse color                                                                                           \n"
"    float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);                           \n"
"    vec3 out_diffuse_color = diffuse_color  * diffuse_coefficient * diffuse_intensity;                         \n"
"                                                                                                               \n"
"    // Ambient color                                                                                           \n"
"    vec3 out_ambient_color = vec3(ambient_color) * ambient_intensity;                                          \n"
"                                                                                                               \n"
"    // Specular color                                                                                          \n"
"    vec3 incidenceVector = -surface_to_light.xyz;                                                              \n"
"    vec3 reflectionVector = reflect(incidenceVector, normal);                                   \n"
"    vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);             \n"
"    vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz);                                     \n"
"    float cosAngle = max( dot(surfaceToCamera, reflectionVector), 0.0);                                        \n"
"    float specular_coefficient = pow(cosAngle, shininess);                                                     \n"
"    vec3 out_specular_color = specular_color * specular_coefficient * specular_intensity;                      \n"
"                                                                                                               \n"
"    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);               \n"
"                                                                                                               \n"
"    gl_FrontColor = vec3(out_diffuse_color + out_ambient_color + out_specular_color);                          \n"
"}                                                                                                              \n";




// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string_GLSphere_410  =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";


static const string fs_string_GLSphere_300  =
"#version 310 core                                                 \n"
"                                                                  \n"
"                                                                   \n"
"                                                                   \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_FragColor = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";










GLSphere::GLSphere(float center_x, float center_y, float center_z, float radius, int rows, int segments )
{
    
    _center.x() = center_x;
    _center.y() = center_y;
    _center.z() = center_z;
    
    _radius = radius;
    _rows = rows;
    _segments = segments;
    
    _render_normal_vectors = false;
    _program_normals = -1;
    _program = -1;
    
    initShader();
    initVBO();

    initShaderNormal();
    initVBONormals();
}




GLSphere::~GLSphere()
{
    // Program clean up when the window gets closed.
    glDeleteVertexArrays(1, _vaoID);
    glDeleteVertexArrays(1, _vaoIDNormals);
    glDeleteProgram(_program);
    glDeleteProgram(_program_normals);
}


/*!
 Draw the objects
 */
void GLSphere::draw(void)
{
 
    //////////////////////////////////////////////////
    // Renders the sphere
    
    // Enable the shader program
    glUseProgram(_program);

    // this changes the camera location
    glm::mat4 rotated_view =  rotatedViewMatrix();
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
    glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); //
    
    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(_vaoID[0]);
    
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // allows to see the primitives
    // Draw the triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _num_vertices);
    
    //////////////////////////////////////////////////
    // Renders the normal vectors
    
    if(_render_normal_vectors)
    {
    
        // Enable the shader program
        glUseProgram(_program_normals);
    
        glUniformMatrix4fv(_viewMatrixLocationN, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
        glUniformMatrix4fv(_modelMatrixLocationN, 1, GL_FALSE, &_modelMatrix[0][0]); //
    
    
        // Bind the buffer and switch it to an active buffer
        glBindVertexArray(_vaoIDNormals[0]);
        glDrawArrays(GL_LINES, 0, _num_vertices_normals);
    }
    
    // Unbind our Vertex Array Object
    glBindVertexArray(0);
    
    // Unbind the shader program
    glUseProgram(0);
    
    
}

/*!
 Set the model matrix for this object
 @param modelmatrix: 4x4 model matrix
 */
void GLSphere::setModelMatrix(glm::mat4& modelmatrix)
{
    _modelMatrix=modelmatrix;
    
}


/*!
 Enables or disables the normal vector renderer
 @param value = true -> enables the renderer, false -> disables the renderer
 */
void GLSphere::enableNormalVectorRenderer(bool value )
{
    _render_normal_vectors = value;
}


/*!
 Create the vertex buffer object for this element
 */
void GLSphere::initVBO(void)
{

    
    _spherePoints.clear();
    _normalVectors.clear();
    
    make_Sphere(_center, _radius, _spherePoints, _normalVectors);
    
    _num_vertices = _spherePoints.size();
    
    
    // create memory for the vertices, etc.
    float* vertices = new float[_spherePoints.size() * 3];
    float* colors = new float[_spherePoints.size() * 3];
    float* normals = new float[_spherePoints.size() * 3];
    
    
    // copy the data to the vectors
    for(int i=0; i<_spherePoints.size() ; i++)
    {
        Vertex v = _spherePoints[i];
        vertices[(i*3)] = v.x(); vertices[(i*3)+1] = v.y(); vertices[(i*3)+2] = v.z();
        
        Vertex n = _normalVectors[i];
        normals[(i*3)] = n.x(); normals[(i*3)+1] = n.y(); normals[(i*3)+2] = n.z();
        
        colors[(i*3)] = 1.0; colors[(i*3)+1] = 1.0; colors[(i*3)+2] = 0.0;
    }
    
    glUseProgram(_program);
    
    
    glGenVertexArrays(1, _vaoID); // Create our Vertex Array Object
    glBindVertexArray(_vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(3, _vboID); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    int locPos = glGetAttribLocation(_program, "in_Position");
    glVertexAttribPointer((GLuint)locPos, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locPos); //
    
    
    //Normals
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * 3 *  sizeof(GLfloat), normals, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    int locNorm = glGetAttribLocation(_program, "in_Normal");
    glVertexAttribPointer((GLuint)locNorm, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(locNorm); //
    
    
    //Color
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[2]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _num_vertices * 3 *  sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    int logColor = glGetAttribLocation(_program, "in_Color");
    glVertexAttribPointer((GLuint)logColor, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(logColor); //
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    // delete the memory
    delete vertices;
    delete colors;
    delete normals;
}


/*!
 Init a frame buffer object to draw normal vectors
 */
void GLSphere::initVBONormals(void)
{
    
    // create the normal vector lines
    vector<Vertex> normalVectorLines;
    for(int i=0; i<_spherePoints.size() ; i++)
    {
        Vertex v = _spherePoints[i];
        Vertex n = _normalVectors[i];
        
        normalVectorLines.push_back(v);
        normalVectorLines.push_back(v+n);
        
    }
    
    float *normal_lines = new float[normalVectorLines.size() * 3];
    float *colors = new float[normalVectorLines.size() * 3];

    for(int i=0; i<normalVectorLines.size() ; i++)
    {
        normal_lines[(i*3)] = normalVectorLines[i].x();
        normal_lines[(i*3)+1] = normalVectorLines[i].y();
        normal_lines[(i*3)+2] = normalVectorLines[i].z();
        
        colors[(i*3)] = 0.0; colors[(i*3)+1] = 0.0; colors[(i*3)+2] = 1.0;
    }
    
    _num_vertices_normals = normalVectorLines.size();
    
    glUseProgram(_program_normals);
    
    glGenVertexArrays(1, _vaoIDNormals); // Create our Vertex Array Object
    glBindVertexArray(_vaoIDNormals[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(2, _vboIDNormals); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, _vboIDNormals[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, normalVectorLines.size() * 3 * sizeof(GLfloat), normal_lines, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(0); //
    
    
    //Color
    glBindBuffer(GL_ARRAY_BUFFER, _vboIDNormals[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, normalVectorLines.size() * 3 *  sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)glGetAttribLocation(_program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(glGetAttribLocation(_program, "in_Color")); //
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    delete normal_lines;
    delete colors;
}


/*
 Inits the shader program for this object
 */
void GLSphere::initShader(void)
{
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    // static const string vertex_code = vs_string_CoordSystem;
    static const char * vs_source = vs_string_GLSphere_410.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    // static const string fragment_code = fs_string_CoordSystem;
    static const char * fs_source = fs_string_GLSphere_410.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    _program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    CheckShader(fs, GL_FRAGMENT_SHADER);
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    CheckShader(vs, GL_VERTEX_SHADER);
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(_program, vs);
    glAttachShader(_program, fs);
    
    glLinkProgram(_program);
    
    glUseProgram(_program);
    
    
    _modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    
    
    int projectionMatrixLocation = glGetUniformLocation(_program, "projectionMatrixBox"); // Get the location of our projection matrix in the shader
    _viewMatrixLocation = glGetUniformLocation(_program, "viewMatrixBox"); // Get the location of our view matrix in the shader
    _modelMatrixLocation = glGetUniformLocation(_program, "modelMatrixBox"); // Get the location of our model matrix in the shader
    _inverseViewMatrixLocation = glGetUniformLocation(_program, "inverseViewMatrix");
    
    _light_source0._lightPosIdx = glGetUniformLocation(_program, "light_position");
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix()[0][0] ); // Send our projection matrix to the shader
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, &viewMatrix()[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, &_modelMatrix[0][0]); // Send our model matrix to the shader
     glUniformMatrix4fv(_inverseViewMatrixLocation, 1, GL_FALSE, &invRotatedViewMatrix()[0][0]);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Material
    _material._diffuse_material = glm::vec3(1.0, 0.5, 0.0);
    _material._ambient_material = glm::vec3(1.0, 0.5, 0.0);
    _material._specular_material = glm::vec3(1.0, 1.0, 1.0);
    _material._shininess = 12.0;
    
    
    _material._ambientColorPos = glGetUniformLocation(_program, "ambient_color");
    _material._diffuseColorPos = glGetUniformLocation(_program, "diffuse_color");
    _material._specularColorPos = glGetUniformLocation(_program, "specular_color");
    _material._shininessIdx = glGetUniformLocation(_program, "shininess");
    
    
    // Send the material to your shader program
    glUniform3fv(_material._ambientColorPos, 1, &_material._ambient_material[0] );
    glUniform3fv(_material._diffuseColorPos, 1, &_material._diffuse_material[0]);
    glUniform3fv(_material._specularColorPos, 1, &_material._specular_material[0]);
    glUniform1f(_material._shininessIdx, _material._shininess);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Light
    
    // define the position of the light and send the light position to your shader program
    _light_source0._lightPos = glm::vec4(50.0,50.0,0.0,1.0);
    _light_source0._ambient_intensity = 0.5;
    _light_source0._specular_intensity = 1.0;
    _light_source0._diffuse_intensity = 1.0;
 
    

    _light_source0._ambientIdx = glGetUniformLocation(_program, "ambient_intensity");
    _light_source0._diffuseIdx = glGetUniformLocation(_program, "diffuse_intensity");
    _light_source0._specularIdx = glGetUniformLocation(_program, "specular_intensity");
   
    

    // Send the light information to your shader program
    glUniform1f(_light_source0._ambientIdx, _light_source0._ambient_intensity );
    glUniform1f(_light_source0._diffuseIdx, _light_source0._diffuse_intensity);
    glUniform1f(_light_source0._specularIdx, _light_source0._specular_intensity);
    
    glUniform4fv(_light_source0._lightPosIdx, 1, &_light_source0._lightPos[0]);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Vertex information / names
    // bind the to the shader program
    glBindAttribLocation(_program, 0, "in_Position");
    glBindAttribLocation(_program, 1, "in_Normal");
    glBindAttribLocation(_program, 2, "in_Color");
    
    
    
     glUseProgram(0);
    
}



/*!
 Shader for the normal vector renderer
 */
void GLSphere::initShaderNormal(void)
{
    _program_normals = CreateShaderProgram(vs_string_simple_shader_410, fs_string_simple_shader_410);
    
    glUseProgram(_program_normals);
    
    unsigned int projectionMatrixLocation = glGetUniformLocation(_program_normals, "projectionMatrix"); // Get the location of our projection matrix in the shader
    _viewMatrixLocationN = glGetUniformLocation(_program_normals, "viewMatrix"); // Get the location of our view matrix in the shader
    _modelMatrixLocationN = glGetUniformLocation(_program_normals, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix()[0][0] ); // Send our projection matrix to the shader
    glUniformMatrix4fv(_viewMatrixLocationN, 1, GL_FALSE, &viewMatrix()[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(_modelMatrixLocationN, 1, GL_FALSE, &_modelMatrix[0][0]); // Send our model matrix to the shader
    
    
    glBindAttribLocation(_program_normals, 0, "in_Position");
    glBindAttribLocation(_program_normals, 1, "in_Color");

    glUseProgram(0);
}




void GLSphere::make_Sphere(Vertex center, double r, std::vector<Vertex> &spherePoints, std::vector<Vertex> &normals)
{
    const double PI = 3.141592653589793238462643383279502884197;
    spherePoints.clear();
    
    int current_size = 0;
    for (double theta = 0.; theta < PI; theta += PI/float(_rows)) // Elevation [0, PI]
    {
        //double theta = 1.57;
        double theta2 = theta + PI/float(_rows);
        int count = 0;
        int count_row = 0;
        current_size = spherePoints.size();
        
        // Iterate through phi, theta then convert r,theta,phi to  XYZ
        for (double phi = 0.; phi < 2*PI + PI/float(_segments) ; phi += PI/float(_segments)) // Azimuth [0, 2PI]
        {
            
            Vertex point;
            point.x() = r * cos(phi) * sin(theta) + center.x();
            point.y() = r * sin(phi) * sin(theta) + center.y();
            point.z() = r            * cos(theta) + center.z();
            spherePoints.push_back(point); count++;
            
            
            Vertex point2;
            point2.x() = r * cos(phi) * sin(theta2) + center.x();
            point2.y() = r * sin(phi) * sin(theta2) + center.y();
            point2.z() = r            * cos(theta2) + center.z();
            spherePoints.push_back(point2); count++;
            
            Vertex normal;
            normal.x() =  cos(phi) * sin(theta);
            normal.y() =  sin(phi) * sin(theta);
            normal.z() =  cos(theta);
            normals.push_back(normal);
            
            Vertex normal2;
            normal2.x() =  cos(phi) * sin(theta2);
            normal2.y() =  sin(phi) * sin(theta2);
            normal2.z() =  cos(theta2);
            normals.push_back(normal2);
            
            
        }
        if(count_row == 0) count_row = count;
    }
    return;
}




