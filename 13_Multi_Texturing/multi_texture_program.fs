#version 410 core        

in vec3 pass_Normal;                                  
in vec2 pass_Texture;									

uniform sampler2D tex; //this is the texture
uniform sampler2D tex_light_map; // this is the light map

out vec4 color; 

void main(void)            
{                  

	vec4 tex_color =  texture(tex, pass_Texture);
	vec4 light_map_color =  texture(tex_light_map, pass_Texture);

	color = tex_color + light_map_color;
}                                           