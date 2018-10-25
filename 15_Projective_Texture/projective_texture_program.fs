#version 410 core        

in vec4 pass_Position;
in vec3 pass_Normal;                                  
in vec2 pass_Texture;									
in vec3 pass_PosWorld;

uniform sampler2D tex; //this is the texture

out vec4 color; 

void main(void)            
{   
	vec3 locationLight = vec3(-5.0, 5.0, 0.0);

	 // Compute the surface to light position
    vec3 surface_to_light =   normalize( locationLight - pass_PosWorld );       
  
    // Diffuse color                                                                                          
    float diffuse_coefficient = max( dot(pass_Normal, surface_to_light), 0.0);                         
    vec3 out_diffuse_color = vec3(0.0, 187, 255)  * surface_to_light * 1.0; 



	// Column major order             
	const mat4 corr = mat4( 0.5, 0.0, 0.0, 0.0, 
							0.0, 0.5, 0.0, 0.0,
				 			0.0, 0.0, 0.5, 0.0,
				 			0.5, 0.5, 0.5, 0.0);  

	vec4 normalize = pass_Position / pass_Position.w;
	normalize = corr * normalize;

	vec4 tex_color =  texture(tex, normalize.st);

	// The image has an alpha channel. 
	// Use it to determine where the object has a color and wher enot
	if(tex_color.a == 0.0  )
	{
		tex_color = vec4(surface_to_light, 1.0);
	}

	color =  tex_color;
}                                           