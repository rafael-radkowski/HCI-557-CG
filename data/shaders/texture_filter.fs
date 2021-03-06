#version 410 core                                                 


const float textureSize = 512.0; //size of the texture
const float texelSize = 1.0 / textureSize; //size of one texel

in vec2 pass_TexCoord; //this is the texture coord
in vec3 pass_Normal;     
in vec3 pass_Position;   
in vec4 pass_Color;   

// The material parameters
uniform struct LightSource {
    vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float cutoff_in; // inner
	float cutoff_out; // outer
	float k1;    //attenuation
	float k2;    //attenuation
	bool used;
	int  type;  //0:point, 1:spot, 2:directional
} light[MAX_LIGHTS];


// The material parameters
uniform struct Material {
    vec3  diffColor;
 	float diffInt;
 	vec3  ambColor;
 	float ambInt;
 	vec3  specColor;
 	float specInt;
 	float shininess;
} mat[1];


uniform int texture_blend;
uniform sampler2D tex; //this is the texture


out vec4 color;

/*
Per-fragment light. 
Note that all vectors need to be in camera/eye-space. 
@param L - vector between light and fragment position.
@param E - eye point
@param s - the light source
@param m - the material 
*/
vec4 useLight(vec3 L, vec3 E, vec3 N, LightSource s, Material m)
{
	vec4 color = vec4(0.0,0.0,0.0,0.0);

	// diffuse light
	vec3 Idiff =  m.diffInt * m.diffColor *  max(dot(L, N), 0.0); 
	Idiff = clamp(Idiff, 0.0, 1.0); 

	// ambient light
	vec4 Iamb = vec4( m.ambColor, 1.0) * m.ambInt;

	// specular light
	vec3 R = reflect(L, N);
	vec3 Ispec =  m.specInt * m.specColor *  pow(max(dot(R, E), 0.0), m.shininess); 

	// calculate color                                     
	color = max(vec4( ( Idiff + Ispec) * s.color, 1.0), Iamb)  * s.intensity;  

	// attenuation 
	float dist = length(L);
	float Iatt = 1 / (1 + s.k1 * dist + s.k2*s.k2 * dist);

	if(s.type == 0)//pointlight
	{
		color = color * Iatt;
	}
	else if(s.type == 1)// spotlight
	{
		vec4 K = viewMatrix * vec4(normalize(s.direction - s.position), 0.0);
		float a = dot(-L,K.xyz); // angle between light dir and fragment position. 

		float c = smoothstep(1.0-s.cutoff_out, 1.0-s.cutoff_in, a); 
		Iatt = Iatt * c; // multiple with attenuation to maintain the distance effect. 

		color = color * Iatt; // attenutation
	}


	return color;
}






vec4 texture2DBilinear( sampler2D textureSampler, vec2 uv )
{
    vec4 tl = texture(textureSampler, uv);
    vec4 tr = texture(textureSampler, uv + vec2(texelSize, 0));
    vec4 bl = texture(textureSampler, uv + vec2(0, texelSize));
    vec4 br = texture(textureSampler, uv + vec2(texelSize , texelSize));
    
    vec2 f = fract( uv.xy * textureSize ); //  the decimal part
    vec4 tA = mix( tl, tr, f.x ); //  interpolate the red dot in the image
    vec4 tB = mix( bl, br, f.x ); //  interpolate the blue dot in the image
    return mix( tA, tB, f.y ); //  interpolate the green dot in the image
}


void main(void)                                                   
{
    // This function finds the color component for each texture coordinate. 
    vec4 tex_color =  texture2DBilinear(tex, pass_TexCoord);
    
    // This mixes the background color with the texture color.
    // The GLSL shader code replaces the former envrionment. It is now up to us
    // to figure out how we like to blend a texture with the background color.
    if(texture_blend == 0)
    {
        color = pass_Color + tex_color;
    }
    else if(texture_blend == 1)
    {
        color = pass_Color * tex_color;
    }
    else if(texture_blend == 3)
    {
        color = (1-pass_Color.w)*pass_Color + tex_color;
    }
    else
    {
        color = pass_Color + tex_color;
    }
    
}