#version 410 core                                             
                                                                   
in vec3 pass_Color;     
in vec2 pass_Texture;  
in vec3 pass_Normal;

uniform sampler2D tex;  
                                          
out vec4 color;        
                           
void main(void)                                                  
{                    
	vec4 tex_color =  texture(tex, pass_Texture.xy);
	                                            
	color =  vec4(pass_Color, 1.0f) ;                            
}                                                              