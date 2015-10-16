//
//  GLAppearance.cpp
//  HCI557_Spotlight
//
//  Created by Rafael Radkowski on 9/26/15.
//
//

#include "GLAppearance.h"
#include "Shaders.h"



#ifdef WIN32
string GLMaterial::_glsl_names[6] = { "ambient", "diffuse",  "specular",  "shininess", "emissive", "transparency"};
string GLMaterial::_glsl_struct = "allMaterials";

string GLLightSource::_glsl_names[5] = { "specular_intensity", "diffuse_intensity",  "ambient_intensity",  "attenuationCoefficient", "light_position"};
string GLLightSource::_glsl_object = "allLights";

string GLSpotLightSource::_glsl_names[2] = { "cone_angle", "cone_direction"};
#endif



/*!
Checks whether a uniform variable has been correctly added.
@param idx - the unifrom locaiton
@param name - the name of the uniform variable.
*/
bool GLVariable::checkUniform(int idx, string name)
{
    if(idx == -1)
    {
        cerr << "[GLVariable] Uniform variable " << name << " was not added to the shader program." << endl;
        return false;
    }
    return true;
};



/*!
 Add all the variables of this material object to the shader program "program".
 It expects that the program already exits and that the names in _glsl_names are used
 @param program - the glsl shader program integer id
 */
bool GLMaterial::addVariablesToProgram(GLuint program, int variable_index)
{

    if(program == -1)return false; // no program exits
    
    
    GLint params;
    glGetProgramiv( program, GL_LINK_STATUS, &params);
    if(params == GL_FALSE)
    {
        cerr << "[GLMaterial] Program " << program << " has not been linked. Materials cannot be added." << endl;
        return false;
    }
    
    
    // enable the program
    glUseProgram(program);
    
    glGetProgramiv( program, GL_ACTIVE_UNIFORMS, &params);
 

    GetVariableName(_glsl_struct, _glsl_names[0], variable_index);
    
    // get the location of a uniform variable. Note, the program must be linked at this position.
    _ambientColorPos = glGetUniformLocation(program, GetVariableName(_glsl_struct, _glsl_names[0], variable_index).c_str());
                        checkUniform(_ambientColorPos, _glsl_names[0]);
    
    _diffuseColorPos = glGetUniformLocation(program, GetVariableName(_glsl_struct, _glsl_names[1], variable_index).c_str());
                        checkUniform(_diffuseColorPos, _glsl_names[1]);
    
    _specularColorPos = glGetUniformLocation(program, GetVariableName(_glsl_struct, _glsl_names[2], variable_index).c_str());
                        checkUniform(_specularColorPos, _glsl_names[2]);
    
    _shininessIdx = glGetUniformLocation(program, GetVariableName(_glsl_struct, _glsl_names[3], variable_index).c_str());
                        checkUniform(_shininessIdx, _glsl_names[3]);
    
    _emissiveIdx = glGetUniformLocation(program, GetVariableName(_glsl_struct, _glsl_names[4], variable_index).c_str());
                        checkUniform(_emissiveIdx, _glsl_names[4]);
    
    _transparencyIdx = glGetUniformLocation(program, GetVariableName(_glsl_struct, _glsl_names[5], variable_index).c_str());
                    checkUniform(_transparencyIdx, _glsl_names[5]);
    
    
    // Send the material to your shader program
    dirty(program);
    
    // disable the program
    // glUseProgram(0);
    
    return true;
}


/*!
 Updates all the variables with new values
 */
bool GLMaterial::dirty(GLuint program)
{
    
    // enable the program
    glUseProgram(program);
    
    // Send the material to your shader program
    glUniform3fv(_ambientColorPos, 1, &_ambient_material[0] );
    glUniform3fv(_diffuseColorPos, 1, &_diffuse_material[0]);
    glUniform3fv(_specularColorPos, 1, &_specular_material[0]);
    glUniform3fv(_emissiveIdx, 1, &_emissive_material[0]);
    glUniform1f(_shininessIdx, _shininess);
    glUniform1f(_transparencyIdx, _transparency);
    
    // disable the program
    glUseProgram(0);
    
    return true;
    
}



/*!
 Add all the variables of this material object to the shader program "program".
 It expects that the program already exits and that the names in _glsl_names are used
 @param program - the glsl shader program integer id
 */
bool GLLightSource::addVariablesToProgram(GLuint program, int variable_index)
{

    if(program == -1)return false; // no program exits
    
    
    GLint params;
    glGetProgramiv( program, GL_LINK_STATUS, &params);
    if(params == GL_FALSE)
    {
        cerr << "[GLMaterial] Program " << program << " has not been linked. Materials cannot be added." << endl;
        return false;
    }
    
    
    // enable the program
    glUseProgram(program);
    
    
    // get the location of a uniform variable. Note, the program must be linked at this position.
    _ambientIdx = glGetUniformLocation(program, GetVariableName(_glsl_object, _glsl_names[2], variable_index).c_str());
                    checkUniform(_ambientIdx, GetVariableName(_glsl_object,_glsl_names[2], variable_index));
    
    _diffuseIdx = glGetUniformLocation(program, GetVariableName(_glsl_object,_glsl_names[1], variable_index).c_str()  );
                    checkUniform(_diffuseIdx,  GetVariableName(_glsl_object,_glsl_names[1], variable_index));
    
    
    _specularIdx = glGetUniformLocation(program, GetVariableName(_glsl_object,_glsl_names[0], variable_index).c_str() );
                    checkUniform(_specularIdx, GetVariableName(_glsl_object,_glsl_names[0], variable_index).c_str());
    
    _attenuation_coeffIdx = glGetUniformLocation(program, GetVariableName(_glsl_object,_glsl_names[3], variable_index).c_str());
                    checkUniform(_attenuation_coeffIdx, GetVariableName(_glsl_object,_glsl_names[3], variable_index));
    
    
    _lightPosIdx = glGetUniformLocation(program, GetVariableName(_glsl_object,_glsl_names[4], variable_index).c_str());
                    checkUniform(_lightPosIdx, GetVariableName(_glsl_object,_glsl_names[4], variable_index));

    
    // Send the light information to your shader program
    glUniform1f(_ambientIdx, _ambient_intensity );
    glUniform1f(_diffuseIdx, _diffuse_intensity);
    glUniform1f(_specularIdx, _specular_intensity);
    glUniform1f(_attenuation_coeffIdx, _attenuation_coeff);
    glUniform4fv(_lightPosIdx, 1, &_lightPos[0]);
    
    // disable the program
    // glUseProgram(0);
    
    return true;
}


/*!
 Updates all the variables with new values
 */
bool GLLightSource::dirty(GLuint program)
{
    
    // enable the program
    glUseProgram(program);
    
    // Send the light information to your shader program
    glUniform1f(_ambientIdx, _ambient_intensity );
    glUniform1f(_diffuseIdx, _diffuse_intensity);
    glUniform1f(_specularIdx, _specular_intensity);
    glUniform1f(_attenuation_coeffIdx, _attenuation_coeff);
    glUniform4fv(_lightPosIdx, 1, &_lightPos[0]);
    
    // disable the program
    glUseProgram(0);
    
    return true;
    
}



/*!
 Add all the variables of this material object to the shader program "program".
 It expects that the program already exits and that the names in _glsl_names are used
 @param program - the glsl shader program integer id
 */
bool GLSpotLightSource::addVariablesToProgram(GLuint program, int variable_index)
{
    
    if(program == -1)return false; // no program exits
    
    
    GLint params;
    glGetProgramiv( program, GL_LINK_STATUS, &params);
    if(params == GL_FALSE)
    {
        cerr << "[GLMaterial] Program " << program << " has not been linked. Materials cannot be added." << endl;
        return false;
    }
    
    
    // enable the program
    glUseProgram(program);
    
    
    // get the location of a uniform variable. Note, the program must be linked at this position.
    _cone_angleIdx = glGetUniformLocation(program, GetVariableName(_glsl_object, _glsl_names[0], variable_index).c_str());
                    checkUniform(_cone_angleIdx, GetVariableName(_glsl_object, _glsl_names[0], variable_index));
    _cone_directionIdx = glGetUniformLocation(program, GetVariableName(_glsl_object, _glsl_names[1], variable_index).c_str());
                    checkUniform(_cone_directionIdx, GetVariableName(_glsl_object, _glsl_names[1], variable_index));

    
    // call the funciton of the base function.
    GLLightSource::addVariablesToProgram(program,variable_index);
    
    // Send the light information to your shader program
    glUniform1f(_cone_angleIdx, _cone_angle);
    glUniform3fv(_cone_directionIdx, 1, &_cone_direction[0]);
    
    return true;
}



/*!
 Updates all the variables with new values
 */
bool GLSpotLightSource::dirty(GLuint program)
{
    
    
    // enable the program
    glUseProgram(program);
    
    // call the funciton of the base function.
    GLLightSource::dirty(program);
    
    // Send the light information to your shader program
    glUniform1f(_cone_angleIdx, _cone_angle);
    glUniform3fv(_cone_directionIdx, 1, &_cone_direction[0]);
    
    // disable the program
    glUseProgram(0);
    
    return true;
    
}





GLAppearance::GLAppearance(string vertex_shader_file, string fragment_shader_file)
{
    
    // This loads the shader program from a file
    _program = LoadAndCreateShaderProgram(vertex_shader_file, fragment_shader_file);
    
    GLint params;
    glGetProgramiv( _program, GL_LINK_STATUS, &params);
    if(params == GL_FALSE)
    {
        cerr << "[GLAppearance] Program " << _program << " has not been linked. " << endl;
    }
    
    // enable the program
    glUseProgram(_program);
    
    _num_light_sources = 0;
    
    _exists = true;
    _finalized = false;
}


GLAppearance::GLAppearance()
{
    _program = -1;
    _exists = false;
}



GLAppearance::~GLAppearance()
{


}


    
/*!
 Add a light source that should illuminate this object.
You can add multiple light sources
@param light_source - a light source reference
*/
void GLAppearance::addLightSource(GLLightSource& light_source)
{
    
    if(_finalized)
    {
        cerr << "Apperance already finalized. Light cannot be added at this point." << endl;
        return;
    }
    
    _light_sources.push_back(&light_source);
    _num_light_sources++;
    
    light_source.addVariablesToProgram(_program, _num_light_sources-1);
    
}
    
/*!
Set the material for this object
@param material - a reference to a material object
*/
void GLAppearance::setMaterial(GLMaterial& material)
{
    
    if(_finalized)
    {
        cerr << "Apperance already finalized. Material cannot be set" << endl;
        return;
    }
    
    _material = &material;
    _material->addVariablesToProgram(_program, 0);
}
    
    

/*!
 Update all the materials if values changes
 */
void GLAppearance::updateMaterial(void)
{
    _material->dirty(_program);
}

/*!
 Updates all the light sources
 */
void GLAppearance::updateLightSources(void)
{

    for (vector<GLLightSource*>::iterator i = _light_sources.begin();
         i != _light_sources.end();
         i++)
    {
        (*i)->dirty(_program);
        
    }
}

/*!
 Updates all texture variables
 */
void GLAppearance::updateTextures(void)
{
    for (vector<GLTextureBase*>::iterator i = _textures.begin();
         i != _textures.end();
         i++)
    {
        if ( (*i)->is_dirty() ) {
            (*i)->dirty(_program);
        }
    }
}


/*!
 Add all the variables of this material object to the shader program "program".
 It expects that the program already exits and that the names in _glsl_names are used
 */
bool GLAppearance::addVariablesToProgram(void)
{
 
    // enable the program
    glUseProgram(_program);
    
    
    // get the location of a uniform variable. Note, the program must be linked at this position.
    int num_lights_idx = glGetUniformLocation(_program,"numLights");
    if(num_lights_idx == -1)
    {
        cerr << "[GLAppearance] Uniform variable numLights was not added to the shader program." << endl;
        return false;
    }
    
    
    // Send the light information to your shader program
    glUniform1i(num_lights_idx, _num_light_sources);
    
    // disable the program
    glUseProgram(0);
    
    return true;
}


/*!
 Finalize the program and all variables.
 */
void GLAppearance::finalize(void)
{
    if(_finalized) return;
    
    _finalized = true;
    addVariablesToProgram();
}


/*!
 Add a texture to the appearance.
 */
void GLAppearance::setTexture(GLTexture* texture)
{
    if(_finalized)
    {
        cerr << "Apperance already finalized. Material cannot be set" << endl;
        return;
    }
    
    _textures.push_back(texture);
    texture->addVariablesToProgram(_program, -1);

}


void GLAppearance::setTexture(GLMultiTexture* texture)
{
    if(_finalized)
    {
        cerr << "Apperance already finalized. Material cannot be set" << endl;
        return;
    }
    
    _textures.push_back(texture);
    texture->addVariablesToProgram(_program, -1);

}



