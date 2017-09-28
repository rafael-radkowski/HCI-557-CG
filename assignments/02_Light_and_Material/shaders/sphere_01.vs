#version 330 core
#define MAX_LIGHTS 10

// The vertex buffer input
in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;

// Transformations for the projections
uniform mat4 projectionMatrixBox;
uniform mat4 viewMatrixBox;
uniform mat4 modelMatrixBox;
uniform mat4 inverseViewMatrix;

// variable to distinguish between renderer for the selection buffer and the regular renderer
uniform bool select_mode;
uniform vec4 select_color_id;
uniform bool is_selected; // to indicate that this object has been selected

// the color that shows up when an object has been picked
const vec3 select_color = vec3(1.0,0.0,0.0);

uniform int numLights;
 
// The light sources
// DO NOT CHANGE THE VARIABLE NAMES 
// THE HOST PROGRAM EXPECTS TO FIND THOSE NAMES
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
// DO NOT CHANGE THE VARIABLE NAMES 
// THE HOST PROGRAM EXPECTS TO FIND THOSE NAMES
uniform struct Material {
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    vec3 emissive;
    float shininess;
    float transparency;
} allMaterials[1];


// The output color
out vec4 pass_Color;



vec4 useLight(Light light, vec4 surfacePostion, vec4 normal_transformed, vec3 normal, Material material )
{
    // 1. Calculate the vector from surface to the current light/
	// Write the result into the variable surface_to_light
    vec4 surface_to_light =   
  
    
    // 2. Calculate your diffuse color coefficient
	// Write the result into the variable diffuse_coefficient
    float diffuse_coefficient = max( dot(normal_transformed, surface_to_light), 0.0);
	
	// 3. Use the diffuse color coefficient to calcualte a diffuse color.
	// Write the result into the variable out_diffuse_color
    vec3 out_diffuse_color = 
    
    
    // 4. Add some ambiend color.
	// Write the result into the variable out_ambient_color
    vec3 out_ambient_color = material.ambient * light.ambient_intensity;
    
    
    // 5. Specular color
	// For all components, use the variables that are provided and do not change them. 
    vec3 incidenceVector = 
	
	// 5.1 Calualte the reflection
    vec3 reflectionVector = reflect(    );
	
	// 5.2. Determine the camera position. Note, the inverse view matrix describes the position using the camera as origin. 
	// You need the global position
    vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);
    
	// 5.3, is already done. BUT CHECKE WHETHER THE DIRECTION ALIGNS WITH YOUR CALCULATION
	vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz);
	
	// 5.4 Determine the cosine angle between the reflection vector and the eye vectory. 
    float cosAngle = max( dot(surfaceToCamera, reflectionVector), 0.0);
	
	// 5.5 Calclate the specular coefficient
    float specular_coefficient = pow(cosAngle, material.shininess);
	
	// 5.6 And finale create a specular materials. 
    vec3 out_specular_color = material.specular * specular_coefficient * light.specular_intensity;
    
    
    6. Determine you attenuation
	6.1 Determine the distance between THIS vertex an the light position. 
    float distanceToLight = 
	
	6.2 Apply an attenuation model. 
    float attenuation = 
    
   
    7.0 Determine the final color using a linear color model. 
    vec3 linearColor = out_ambient_color  + attenuation * ( out_diffuse_color + out_specular_color);
    
    // return the value
    return vec4(linearColor, 1.0);
}





void main(void)
{
    // Caculate the normal vector and surface position in world coordinates
    vec3 normal = normalize(in_Normal);
    vec4 transformedNormal =  normalize(transpose(inverse(modelMatrixBox)) * vec4( normal, 1.0 ));
    vec4 surfacePostion = modelMatrixBox * vec4(in_Position, 1.0);
    


 
     // Calculate the color
     vec4 linearColor = vec4(0.0,0.0,0.0,0.0);
    
    for (int i=0; i<numLights; i++) {
        vec4 new_light = useLight(allLights[i], surfacePostion, transformedNormal, normal, allMaterials[0] );
        linearColor = linearColor + new_light;
    }
    
   
    // Gamma correction
    vec4 gamma = vec4(1.0/2.2);
    vec4 finalColor = pow(linearColor, gamma);
    
    // Pass the color
    pass_Color =  vec4(finalColor);

    
    // Passes the projected position to the fragment shader / rasterization process.
    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);
    
}






















