#version 410 core        

in vec4 pass_Position;
in vec3 pass_Normal;                                  
in vec2 pass_Texture;									
in vec3 pass_PosWorld;

uniform sampler2D tex; //this is the texture

out vec4 color; 


vec3 useDiffuseLight(vec3 position, vec3 normal, vec3 light_location, vec3 material, float intensity )
{
	 // Compute the surface to light position
    vec3 surface_to_light =   normalize( light_location - position );     

	 // Diffuse color                                                                                          
    float diffuse_coefficient = max( dot(normal, surface_to_light), 0.0);      

	vec3 out_diffuse_color = material  * diffuse_coefficient * intensity;      

	return out_diffuse_color;
}


void main(void)            
{   
	vec3 locationLight1 = vec3(-15.0, 15.0, 0.0);
	vec3 locationLight2 = vec3(15.0, -15.0, 2.0);


	vec3 out_diffuse_color =  0.6* useDiffuseLight(pass_PosWorld, pass_Normal, locationLight1, vec3(0.4, 0.4, 0.4) , 4.0);
	out_diffuse_color += 0.6 * useDiffuseLight(pass_PosWorld, pass_Normal, locationLight2, vec3(0.4, 0.4, 0.4) , 4.0);

	color = vec4(out_diffuse_color, 1.0);
}                                           