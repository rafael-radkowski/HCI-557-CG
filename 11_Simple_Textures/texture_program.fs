#version 410 core        

in vec3 pass_Normal;                                  
in vec2 pass_Texture;									

uniform sampler2D tex; //this is the texture

out vec4 color; 

void main(void)            
{                  

	vec4 tex_color =  texture(tex, pass_Texture);

	color = tex_color;
}                                           