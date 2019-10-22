#version 410 core                                                 

uniform sampler2D tex; //this is the texture
uniform int mipmap_level_override;
uniform int mipmap_false_color;

in vec3 pass_Normal;  // the normal vectors                                   
in vec2 pass_Texture; // the texture coordinates

out vec4 color;

uniform int texture_blend;

void main(void)                                                   
{
    // This calculates the current midmap level/
    // This only works for GLSL > 4.0. See the code for 3.3
    float mipmapLevel = textureQueryLod(tex, pass_Texture).x;

    vec4 tex_color;
    // This function finds the color component for each texture coordinate.
    if(mipmap_level_override != 0) 
        tex_color =  textureLod(tex, pass_Texture, mipmap_level_override); // for manual override
    else
        tex_color =  textureLod(tex, pass_Texture, mipmapLevel); // using textureQueryLod
    
    // output the color. 
    if(mipmap_false_color == 1)
        color =  vec4(1.0, 0.0, 0.0, 1.0) / float(mipmapLevel);
    else 
        color = tex_color;
    
    
}