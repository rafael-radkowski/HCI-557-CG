#version 410 core        

in vec3 pass_Normal;                                  
in vec2 pass_Texture;		
in vec3 pass_Position; 							

uniform sampler2D tex; //this is the texture
uniform sampler2D tex_normalmap; //this is the texture

uniform int  use_normalmap;
uniform int  mode;

out vec4 color; 

void main(void)            
{           
	// light position
	const vec3 light = vec3(6.0, -6.0, 5.0);

	// textures
	vec4 tex_color =  texture(tex, pass_Texture);
	vec4 tex_normal  =  texture(tex_normalmap, pass_Texture);

	// tex_norm range runs from [0, 1], normal vector needs to run
	// from [-1, 1]
	vec3 bump_norm = (2.0*tex_normal.xyz)-1.0; 

	if(use_normalmap == 0) bump_norm = vec3(0.0,0.0,0.0);

	//  diffuse light
	vec3 L =  normalize( light - pass_Position );  
	vec3 N =  normalize(pass_Normal + bump_norm);                                                                                       
    float d_coeff = max( dot(N, L), 0.0);                         
    vec3 out_diffuse_color = vec3(0.36,0.30,0.27)  * d_coeff * 1.5; 

	//  ambient light
	vec3 out_ambient_color = vec3(0.36,0.30,0.27) * 0.5; 

	// specular light
	N =  normalize(pass_Normal);   
	vec3 R = normalize(reflect(-L+bump_norm, N));     
	vec3 E = normalize(pass_Position); // local object space, camera is at 0.0 location
	float s_angle = max( dot(  E, R), 0.0);
	float s_coeff = pow(s_angle, 5.0);  
	vec3 out_specular_color = vec3(1.0,1.0,1.0) * s_coeff * 0.4; 

	// final color
	if(mode == 0)
		color =  0.6 * tex_color + max( vec4( out_ambient_color, 1.0),  vec4( out_diffuse_color + out_specular_color, 1.0) ) ;//+ 0.5 * tex_norm ;
	else if(mode == 1)
		color = tex_normal;
	else if(mode == 2)
		color = vec4(pass_Normal, 1.0);
	else if(mode == 3)
		color = vec4( normalize(pass_Normal + bump_norm), 1.0);
	else if(mode == 4)
		color = vec4( out_diffuse_color, 1.0);
	else if(mode == 5)
		color = vec4( out_specular_color, 1.0);

}                                           