#version 410 core                                             
                                                                   
in vec3 pass_Color;     
in vec2 pass_TexCoord;  

uniform sampler2D in_Texture;  
                                          
out vec4 color;        
                           
void main(void)                                                  
{                    
	vec4 tex_color =  texture(in_Texture, pass_TexCoord.xy);
	                                            
	color =  vec4(pass_Color, 1.0f) * 0.2 +  tex_color;                            
}                                                              