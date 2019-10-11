/*
@file: per_fragment_light.vs
@brief: Per-fragment light shader for one light. 

This is example code for ME/CS/CPRE 557 - Computer Graphics. 

Rafael Radkowski
Iowa State University
November 2018
rafael@iastate.edu

MIT License
------------------------------------------------
Last changes:

Oct 10, 2019, RR
- Removed unnessary variables. 

*/


#version 410 core                                                 

// The uniform variable for the rendering pipeline.                                                      
uniform mat4 projectionMatrix;                                   
uniform mat4 viewMatrix;                                         
uniform mat4 modelMatrix;  

// These are the input variables/
// The gpu will pass the vertices and normal vectors stored in the vertex buffer object
// through these variables. 
in vec3 in_Position;                                               
in vec3 in_Normal;            

  
// The output data. 
smooth out vec3 pass_Normal;
smooth out vec3 pass_Point;
    

void main(void)                                                  
{  
    // Calculate the normal vector per vertex and pass it to the fragment shader.
    // Note that all coordiantes must be in camera coordinate space. 
    // Passing it through rasterization will interpolate it per fragment
    pass_Normal = normalize( vec3(  transpose(inverse( viewMatrix *  modelMatrix))  * vec4( in_Normal, 0.0 )));

    // Calculate the surface position and pass it to the fragment shader.
     // Note that all coordiantes must be in camera coordinate space. 
    // Passing it through rasterization will interpolate it per fragment.
    pass_Point = vec3( viewMatrix * modelMatrix * vec4(in_Position, 1.0) );  

    // THe projection. 
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0); 
	
}                                                                 