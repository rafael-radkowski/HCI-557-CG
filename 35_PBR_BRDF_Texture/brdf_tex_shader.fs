/*
@file: brdf_shader.fs
@brief: Implements a brdf shader

This is example code for ME/CS/CPRE 557 - Computer Graphics. 

Rafael Radkowski
Iowa State University
November 2018
rafael@iastate.edu

MIT License
------------------------------------------------
Last changes:
 
*/
#version 410 core    
#define MAX_LIGHTS 8

// the incoming variables
in vec3 pass_Normal;
smooth in vec3 pass_Position;
in vec3 pass_Color;
in vec2 pass_Texture;    

uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;  


float PI = 3.141592653;                                                          

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


uniform struct BRDFTexMaterial {
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;

    vec3 lightColor; // color of the light. 
    vec3 F0; 

    float k1; // linear attenuation
    float k2; // quadratic attenuation
} brdf_tex[1];

   
// the final color
out vec4 frag_out;   




float calculateAttenuation(vec3 light_position, vec3 fragment_position, float k1, float k2)
{
    float distance    = length(light_position - fragment_position);
    float attenuation = 1.0 / (1.0 + k1 * distance +  k2 * (distance * distance));  
    return attenuation;
}



vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}




void main(void)                                                  
{   
    // read the color values from the map
    vec3 albedo_color = pow(texture(  brdf_tex[0].albedoMap, pass_Texture).rgb, vec3(2.2));
    float metallic_value = texture(  brdf_tex[0].metallicMap, pass_Texture).r;
    float roughness_value = texture(  brdf_tex[0].roughnessMap, pass_Texture).r;
    float ao_value = texture(  brdf_tex[0].aoMap, pass_Texture).r;

    //vec3 roughness_color = texture(  brdf_tex[0].roughnessMap, pass_Texture).rgb;

    // eye position 
	vec3 E = ( vec3(viewMatrix[0][0], viewMatrix[1][3], viewMatrix[2][3]) );
     
    vec3 F0 = brdf_tex[0].F0;
    F0 = mix(F0, albedo_color, metallic_value);

    // camera view to fragment position
    vec3 V = normalize( -pass_Position);

     // normal vector
    vec3 N = normalize(pass_Normal);

    vec3 Lo = vec3(0.0);
    for (int i=0; i<MAX_LIGHTS; i++){

        if(light[i].used == false) continue;

        // Light in view space
        vec3 L_viewspace = (viewMatrix * vec4(light[i].position, 1.0)).rgb;

        // light to fragment position
        vec3 L = normalize(L_viewspace - pass_Position);

          // halfway -vector
        vec3 H = normalize(V + L);


        // calculate the radiance comming from the light source. 
        vec3  lightColor  = brdf_tex[0].lightColor;
        float cosTheta    = max(dot(N, L), 0.0);
        float attenuation = calculateAttenuation(pass_Position, L_viewspace, brdf_tex[0].k1, brdf_tex[0].k2);
        vec3  radiance    = lightColor * attenuation * cosTheta;
        

         // cook-torrance reflection brdf
        float NDF = DistributionGGX(N, H, roughness_value);        
        float G   = GeometrySmith(N, V, L, roughness_value);      
        vec3 F    = fresnelSchlick(max(dot(V, H), 0.0), F0); 

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular     = numerator / max(denominator, 0.001);  
     

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic_value;	    

         // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);  

        Lo += (kD * albedo_color / PI + specular) * radiance * NdotL; 

        //Lo = vec3(pass_View);
    }

    //ambient add
    //vec3 ambient = vec3(0.03) * brdf[0].albedo * brdf[0].ao;
    vec3 ambient = vec3(0.03) * albedo_color * ao_value;
    vec3 color = ambient + Lo;
	
    // gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

    
	frag_out = vec4(color, 1.0);       
   // frag_out = vec4(E, 1.0);
}                                                           