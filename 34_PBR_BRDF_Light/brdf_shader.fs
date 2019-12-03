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

Dec 4, 2019, RR:
- Changed the variable type for pass_Color from vec3 to vec4. 
 
*/
#version 410 core    
#define MAX_LIGHTS 8

// the incoming variables
in vec3 pass_Normal;
in vec3 pass_Position;
in vec4 pass_Color;

uniform mat4 projectionMatrix;                                    
uniform mat4 viewMatrix;                                           
uniform mat4 modelMatrix;  


float PI = 3.1415;                                                          

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


// The parameters for a BRDF materail
uniform struct BRDFMaterial {
    vec3 albedo; // the general diffuse color
    float metallic; // metalic factor runs in a range from 0 to 1
 	float roughness; // roughness factor runs in a range from 0 to 1
 	float ao; // ambient intensity
    vec3 lightColor; // color of the light. 
    vec3 F0; 

    float k1; // linear attenuation
    float k2; // quadratic attenuation
} brdf[1];

   
// the final color
out vec4 frag_out;   




float calculateAttenuation(vec3 light_position, vec3 fragment_position, float k1, float k2)
{
    float distance    = length(light_position - fragment_position);
    float attenuation = 1.0 / (1.0 + k1 * distance + 
                    k2 * (distance * distance));  
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
    // eye position 
	vec3 E = normalize( vec3(viewMatrix[3][0], viewMatrix[3][1], viewMatrix[3][2]) );
     
    vec3 F0 = brdf[0].F0;
    F0 = mix(F0, brdf[0].albedo, brdf[0].metallic);

    // camera view to fragment position
    vec3 V = normalize(E -pass_Position);

     // normal vector
    vec3 N = normalize(pass_Normal);

    vec3 Lo = vec3(0.0);
    for (int i=0; i<MAX_LIGHTS; i++){

        if(light[i].used == false) continue;

        // Light in view space
        vec3 L_viewspace = vec3(viewMatrix * vec4(light[i].position, 1.0));

        // light to fragment position
        vec3 L = normalize(L_viewspace - pass_Position);

          // halfway -vector
        vec3 H = normalize(V + L);

        // calculate the radiance comming from the light source. 
        vec3  lightColor  = brdf[0].lightColor;
        float cosTheta    = max(dot(pass_Normal, L), 0.0);
        float attenuation = calculateAttenuation(pass_Position, L_viewspace, brdf[0].k1, brdf[0].k2);
        vec3  radiance    = lightColor * attenuation * cosTheta;
        
         // cook-torrance reflection brdf
        float NDF = DistributionGGX(N, H, brdf[0].roughness);        
        float G   = GeometrySmith(N, V, L, brdf[0].roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0); 

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular     = numerator / max(denominator, 0.001);  
     
        // Diffuse and ambient term
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - brdf[0].metallic;	    

         // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);  
        Lo += (kD * brdf[0].albedo / PI + specular) * radiance * NdotL; 

        //Lo = vec3(G );
    }

    //ambient add
    vec3 ambient = vec3(0.03) * brdf[0].albedo * brdf[0].ao;
    vec3 color = ambient + Lo;
	
    // gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

    
	frag_out = vec4(color, 1.0);       
   // frag_out = vec4(Lo, 1.0);
}                                                           