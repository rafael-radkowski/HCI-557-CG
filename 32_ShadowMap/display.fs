#version 410 core                                             
                                                                   
in vec3 pass_Color;     
in vec2 pass_Texture;  
in vec3 pass_Normal;

uniform sampler2D tex;  
                                          
out vec4 color;        
                           
void main(void)                                                  
{                    
	vec4 tex_color =  texture(tex, pass_Texture.xy);
	                                            
	color =  vec4(tex_color.x, tex_color.x, tex_color.x, 1.0) ; // + vec4(pass_Color, 1.0f) * 0.2;      

	// Set the fragment depth to a small value to make sure that the object is always in front. 
	gl_FragDepth = 0.000001;                     
}                                                              