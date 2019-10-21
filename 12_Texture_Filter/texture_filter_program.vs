#version 410 core                                                 
                                                                   
uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;    
                                      
in vec3 in_Position;                                              
in vec2 in_Texture;                                                
in vec3 in_Normal;     
                                             
											 
out vec2 pass_Texture;
out vec3 pass_Normal; 
out vec3 pass_Position;
out vec4 pass_Color; 

                                                                 
void main(void)                                                   
{  
	// pass the texture coordinates through the pipeline.                                       
    pass_Texture = in_Texture;      

	// for lighting
	pass_Position = vec3(  viewMatrix *  modelMatrix  *  vec4(in_Position, 1.0));    
	
	// make it red
	pass_Color = vec4(1.0,0.0,0.0,1.0);   

	// pass the rotated normal vector to the fragment shader
    pass_Normal =   vec3( transpose(inverse(viewMatrix * modelMatrix)) *  vec4(in_Normal, 0.0));    	
           
	// the viewing pipeline
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  

}                                                                 