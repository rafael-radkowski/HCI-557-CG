#version 410 core                                                 
#define MAX_LIGHTS 10

// Transformations for the projections
uniform mat4 projectionMatrixBox;
uniform mat4 viewMatrixBox;
uniform mat4 modelMatrixBox;
uniform mat4 inverseViewMatrix;

uniform int numLights;

// The light sources
uniform struct Light {
    vec4 light_position;
    float diffuse_intensity;
    float ambient_intensity;
    float specular_intensity;
    float attenuationCoefficient;
    float cone_angle;
    vec3 cone_direction;
} allLights[MAX_LIGHTS];


// The material parameters
uniform struct Material {
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    vec3 emissive;
    float shininess;
    float transparency;
} allMaterials[1];




uniform sampler2D texture_background; //this is the texture
uniform sampler2D texture_foreground; //this is the texture

in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;
out vec4 color;

uniform int texture_blend;
uniform float texture_delta;
uniform int bump_mode;

in vec3 pass_Normal;
in vec4 pass_transformedNormal;
in vec4 pass_surfacePostion;





vec4 useLight(Light light, vec4 surfacePostion, vec4 normal_transformed, vec3 normal, Material material )
{
    normal_transformed = normalize(normal_transformed);
    normal = normalize(normal);
    
    // Calculate the vector from surface to the current light
    vec4 surface_to_light =   normalize( light.light_position -  surfacePostion );
    if(light.light_position.w == 0.0){
        surface_to_light =   normalize( light.light_position);
    }
    
    // Diffuse color
    float diffuse_coefficient = max( dot(normal_transformed, surface_to_light), 0.0);
    vec3 out_diffuse_color = material.diffuse  * diffuse_coefficient * light.diffuse_intensity;
    
    
    // Ambient color
    vec3 out_ambient_color = material.ambient * light.ambient_intensity;
    
    
    // Specular color
    vec3 incidenceVector = -surface_to_light.xyz;
    vec3 reflectionVector = reflect(incidenceVector, normal.xyz);
    vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz);
    float cosAngle = max( dot(surfaceToCamera, reflectionVector), 0.0);
    float specular_coefficient = pow(cosAngle, material.shininess);
    vec3 out_specular_color = material.specular * specular_coefficient * light.specular_intensity;
    
    
    //attenuation
    float distanceToLight = length(light.light_position.xyz - surfacePostion.xyz);
    float attenuation = 1.0 / (1.0 + light.attenuationCoefficient * pow(distanceToLight, 2));
    
    
    if(light.light_position.w == 1.0)
    {
        
        //////////////////////////////////////////////////////////////////////////////////////////////
        // Spotlight
        // 1. Normalize the cone direction
        vec3 cone_direction_norm = normalize(light.cone_direction);
        
        // 2. Calculate the ray direction. We already calculated the surface to light direction.
        // 	  All what we need to do is to inverse this value
        vec3 ray_direction = -surface_to_light.xyz;
        
        // 3. Calculate the angle between light and surface using the dot product again.
        //    To simplify our understanding, we use the degrees
        float light_to_surface_angle = degrees(acos(dot(ray_direction, cone_direction_norm))) ;
        
        // 4. Last, we compare the angle with the current direction and
        //    reduce the attenuation to 0.0 if the light is outside the angle.
        if(light_to_surface_angle > light.cone_angle){
            attenuation = 0.0;
        }
        
    }
    else if(light.light_position.w == 0.0) {
        //////////////////////////////////////////////////////////////////////////////////////////////
        // Directional light
        
        // 1. the values that we store as light position is our light direction.
        vec3 light_direction = normalize(light.light_position.xyz);
        
        // 2. We check the angle of our light to make sure that only parts towards our light get illuminated
        float light_to_surface_angle = dot(light_direction, normal_transformed.xyz);
        
        // 3. Check the angle, if the angle is smaller than 0.0, the surface is not directed towards the light.
        // if(light_to_surface_angle > 0.0)attenuation = 1.0;
        // else attenuation = 0.0;
        attenuation = 1.0;
    }
    

    
    // Calculate the linear color
    vec3 linearColor =  out_ambient_color  + attenuation * ( out_diffuse_color + out_specular_color);
    
    // adds transparency to the object
    return vec4(linearColor, material.transparency);
}









void main(void)                                                   
{
   
    
    vec4 tex_displacement = vec4(0.0,0.0,0.0,0.0);
    
    if (bump_mode == 0)
    {
        tex_displacement = texture(texture_foreground, pass_TexCoord);
    }
    
    vec2 displacement = texture_delta * tex_displacement.xy;
    
    
    // This function finds the color component for each texture coordinate. 
    vec4 tex_color =  texture(texture_background, pass_TexCoord + displacement);
    
    
    
    
    // Calculate the color
    vec4 linearColor = vec4(0.0,0.0,0.0,0.0);
    
    for (int i=0; i<numLights; i++) {
        vec4 new_light = useLight(allLights[i], pass_surfacePostion, pass_transformedNormal + tex_displacement, pass_Normal+tex_displacement.xyz, allMaterials[0] );
        linearColor = linearColor + new_light;
    }
    
    

    color = 0.1 * linearColor + tex_color;
    

}