#include "ShaderProgram.h"

#ifdef _WIN32
#else
#include <sys/stat.h>
#endif


bool cs557::ShaderProgramUtils::CheckShader(GLuint shader, GLenum shader_type)
{
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    
    if (!compiled)
    {
        GLint info_len = 0;
        
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
        
        if (info_len)
        {
            char* buf = (char*) malloc(info_len);
            if (buf) {
                glGetShaderInfoLog(shader, info_len, NULL, buf);
                cout << "Could not compile shader shader_type " << shader_type << ":\n" <<  buf << endl;
                free(buf);
            }
            glDeleteShader(shader);
            shader = 0;
        }
        return false;
    }
    return true;

}


/*!
Check the shader code for errors after linking
@param shader: the shader program id
*/
//static 
bool cs557::ShaderProgramUtils::CheckLinker(GLuint shader)
{
    GLint linked = 0;
    glGetProgramiv(shader, GL_LINK_STATUS, &linked);
    
    if (linked == GL_FALSE)
    {
        GLint info_len = 0;
        
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &info_len);
        
        if (info_len)
        {
            char* buf = (char*) malloc(info_len);
            if (buf) {
                glGetProgramInfoLog(shader, info_len, NULL, buf);
                cout << "Could not link the shaders code: " << " GL_LINK_STATUS " << "error: \n" <<  buf << endl;
                free(buf);
            }
            glDeleteShader(shader);
            shader = 0;
        }
        return false;
    }
    return true;
}



/*
 Creates a shader program
 */
GLuint cs557::CreateShaderProgram(string vertex_source, string fragment_source)
{
    GLuint  program;
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    // static const string vertex_code = vs_string_CoordSystem;
    if(vertex_source.length() == 0){
        program = -1;
		return program;
    }
    const char * vs_source = vertex_source.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    // static const string fragment_code = fs_string_CoordSystem;
    if(fragment_source.length() == 0){
        program = -1;
		return program;
    }
    const char * fs_source = fragment_source.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();

	if (program == 0)
	{
		cout << "[ERROR] - CreateShaderProgram: Did not create a shader program." << endl;
        program = -1;
		return program;
	}
    
    // We create a shader with our fragment shader source code and compile it.
    // This function returns 0 if an error occurs creating the shader object.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    bool ret = ShaderProgramUtils::CheckShader(fs, GL_FRAGMENT_SHADER);
    if(!ret){
        cout << "Problems compiling GL_FRAGMENT_SHADER" << endl;
        program = -1;
        return program;
    }
    
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    ret = ShaderProgramUtils::CheckShader(vs, GL_VERTEX_SHADER);
    if(!ret){
        cout << "Problems compiling GL_VERTEX_SHADER" << endl;
        program = -1;
        return program;
    }
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    // Link the program
    glLinkProgram(program);

    // check the linker program for errors
    bool link_status = ShaderProgramUtils::CheckLinker( program);
    if(!link_status)
    {
        cout << "Problems link the shader program" << endl;
        program = -1;
        return program;
    }
    
    glUseProgram(program);
    
    return program;
}



/*!
 Load a shader program from a file and creates the related program
 @param vertex_file -  the file which stores the vertex shader code
 @param fragment_file -  the file which stores the fragment shader code
 @return - Gluint of the shader program
 */
GLuint cs557::LoadAndCreateShaderProgram(string vertex_file, string fragment_file)
{

    string vertex_source = cs557::ShaderProgramUtils::LoadFromFile(vertex_file);
    string fragment_source= cs557::ShaderProgramUtils::LoadFromFile(fragment_file);


    // create and return the pgoram.
    return CreateShaderProgram(vertex_source, fragment_source);

}


/*!
 Verifies wheterh a file [name] exits
 @param name - the path and the name of the file.
 */
bool cs557::ShaderProgramUtils::Exists (const std::string& name)
{
   #ifdef _WIN32
   #if _MSC_VER > 1900	
    if (std::filesystem::exists(name)) {
	#else
	if (std::experimental::filesystem::exists(name)) {
	#endif
        return true;
    } else {
		return false;
    }
    #else
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    #endif
}





/*!
 Opens a file and loads the code from this file.
 */
string cs557::ShaderProgramUtils::LoadFromFile(string path_and_file)
{
	// Check if the file exits. 
	if(!Exists(path_and_file)) {
		
		// extract the filename.
		int idx = path_and_file.find_last_of("/");
		string file;
		if (idx > 0) {
			file = path_and_file.substr(idx+1, path_and_file.length() - idx - 1);
		}
		if(!Exists(file)) {
			cout << "[ERROR] Cannot find shader program " << path_and_file << "." <<endl;
			return "";
		}

		path_and_file = file;
	}
    
    
    ifstream in(path_and_file);
    
    if(in.is_open())
    {
        
        std::string str;
        
        in.seekg(0, std::ios::end);
        str.reserve(in.tellg());
        in.seekg(0, std::ios::beg);
        
        str.assign((std::istreambuf_iterator<char>(in)),
                   std::istreambuf_iterator<char>());
        
        return str;
    }
    return "";
}




/*
Create a shader program
@param vertex source - the vertex shader source code as string. 
@param geometry source - the geometry shader source code as string. 
@param fragment_source - the fragment shader source code as string
@return - Gluint of the shader program, must be larger than -1. 
0 indicates a problem
*/
GLuint cs557::CreateShaderProgram(string vertex_source, string geometry_source, string fragment_source)
{

    GLuint  program;
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    // static const string vertex_code = vs_string_CoordSystem;
    if(vertex_source.length() == 0){
        program = 0;
		return program;
    }
    const char * vs_source = vertex_source.c_str();


    // Geometry shader source code. 
    if(geometry_source.length() == 0){
        program = 0;
		return program;
    }
    const char * gs_source = geometry_source.c_str();

    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    // static const string fragment_code = fs_string_CoordSystem;
    if(fragment_source.length() == 0){
        program = 0;
		return program;
    }
    const char * fs_source = fragment_source.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();

	if (program == 0)
	{
		cout << "[ERROR] - CreateShaderProgram: Did not create a shader program." << endl;
        program = 0;
		return program;
	}
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    bool ret = ShaderProgramUtils::CheckShader(fs, GL_FRAGMENT_SHADER);
    if(!ret){
        cout << "Problems compiling GL_FRAGMENT_SHADER" << endl;
        program = 0;
        return program;
    }

    // We create a shader with our geometry shader source code and compile it.
    
    GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gs, 1, &gs_source, NULL);
    glCompileShader(gs);
    ret = ShaderProgramUtils::CheckShader(gs, GL_GEOMETRY_SHADER);
    if(!ret){
        cout << "Problems compiling GL_GEOMETRY_SHADER" << endl;
        program = 0;
        return program;
    }
    
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    ret = ShaderProgramUtils::CheckShader(vs, GL_VERTEX_SHADER);
    if(!ret){
        cout << "Problems compiling GL_VERTEX_SHADER" << endl;
        program = 0;
        return program;
    }
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    GLint compiled = 0;
    glGetShaderiv(program, GL_LINK_STATUS, &compiled);


    if (!compiled)
    {
        GLint info_len = 0;
        
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &info_len);
        
        if (info_len)
        {
            char* buf = (char*) malloc(info_len);
            if (buf) {
                glGetShaderInfoLog(program, info_len, NULL, buf);
                cout << "Could not link shader\n" <<  buf << endl;
                free(buf);
            }
            glDeleteShader(program);
            program = 0;
        }
        
        return program;
    }

    glUseProgram(program);

    return program;
}





/*!
Load a shader program from a file and creates the related program
@param vertex_file -  the file which stores the vertex shader code
@param geometry_file -  the file which stores the geometry shader code
@param fragment_file -  the file which stores the fragment shader code
@return - Gluint of the shader program, must be larger than 0. 
0 indicates a problem
*/
GLuint cs557::LoadAndCreateShaderProgram(string vertex_file, string geometry_file, string fragment_file)
{
    string vertex_source = cs557::ShaderProgramUtils::LoadFromFile(vertex_file);
    string geometry_source = cs557::ShaderProgramUtils::LoadFromFile(geometry_file);
    string fragment_source= cs557::ShaderProgramUtils::LoadFromFile(fragment_file);

    // create and return the pgoram.
    return CreateShaderProgram(vertex_source, geometry_source, fragment_source);
}
