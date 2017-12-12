#version 410 core                                                 
                                                                 
uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;                                          
in vec3 in_Position;                                                                                                           
in vec3 in_Normals;         
                                       
out vec3 pass_Normal; 
out vec3 pass_Position;                                             
                                                                
void main(void)                                                  
{            
	// for difuse lighting
	pass_Position = (viewMatrix * modelMatrix  *  vec4(in_Position, 1.0)).xyz;    
	
    pass_Normal = normalize(transpose(inverse(modelMatrix)) *  vec4(in_Normals, 1.0)).xyz;                         
			
	// pass the position				                       
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);       
	                             
}                                                            