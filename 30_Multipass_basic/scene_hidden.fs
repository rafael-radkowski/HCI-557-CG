#version 410 core                                             
                                                                   
in vec3 pass_Normal;     
in vec3 pass_Position;   
                                          
out vec4 color;        
                           
void main(void)                                                  
{          

	// diffuse light
	vec3 light_source_pos = vec3(0.0, 30.0, -10.0);
	vec3 L = normalize( light_source_pos- pass_Position);   
	vec3 Idiff = vec3(1.0,1.0,1.0) * max(dot(pass_Normal,L), 0.0) ; 
	Idiff = clamp(Idiff, 0.0, 1.0); 

          
	                                             
	color = vec4(Idiff, 1.0);                            
}                                                      