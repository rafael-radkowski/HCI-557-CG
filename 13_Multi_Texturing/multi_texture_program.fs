#version 410 core        

in vec3 pass_Normal;   // normal vectors from the vertex program                               
in vec2 pass_Texture; // texture coordinates from the vertex program								

uniform sampler2D tex; //this is the background texture
uniform sampler2D tex_light_map; // this is the light map

// the output color. 
out vec4 color; 

void main(void)            
{                  
	// Fetch the color from the background texture for the current fragment
	vec4 tex_color =  texture(tex, pass_Texture);

	// Fetch the texture color from the light map. 
	vec4 light_map_color =  texture(tex_light_map, pass_Texture);

	// combine both colors. 
	color = tex_color + light_map_color;
}                                           