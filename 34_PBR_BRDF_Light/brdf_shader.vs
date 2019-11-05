/*
@file: brdf_shader.vs
@brief: Example vertex shader for a brdf renderer

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
                                                                 
uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;  


in vec3 in_Position;                                                                                                           
in vec3 in_Normal;         
                                       
out vec3 pass_Normal; 
out vec3 pass_Position;
out vec4 pass_Color;                                             
                                                                
void main(void)                                                  
{            
	// for lighting
	pass_Position = vec3(  viewMatrix *  modelMatrix  *  vec4(in_Position, 1.0));    
	
	// make it red, a backup color
	pass_Color = vec4(1.0,0.0,0.0,1.0);   

    // pass the normal vector along
    pass_Normal =   vec3( transpose(inverse(viewMatrix * modelMatrix)) *  vec4(in_Normal, 0.0));                         
			
	// pass the position				                       
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);       
	       
		
}                                                            