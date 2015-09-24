

#include "ModelObj.h"



ModelObj::ModelObj(string filename)
{
    
    bool ret =load_obj(filename.c_str(), _vertices, _normals, _elements);
    if(ret)
    {
        initVBO();
    }
}

ModelObj::ModelObj()
{

}


ModelObj::~ModelObj()
{

}






bool ModelObj::load_obj(const char* filename, vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements)
{
    ifstream in(filename, ios::in);
    if (!in)
    {
        cerr << "Cannot open " << filename << endl;
        return false;
    }
    

    
    string line;
    while (getline(in, line))
    {
        if (line.substr(0,2) == "v ")
        {
            istringstream s(line.substr(2));
            glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;// v.w = 1.0f;
            vertices.push_back(v);
            
            
        }
        else if (line.substr(0,2) == "f ")
        {
            int idx = line.find_first_of("/");
            if(idx == -1)
            {
                istringstream s(line.substr(2));
                GLushort a,b,c;
                s >> a; s >> b; s >> c;
                cout << a << " : " << b << " : " << c << endl;;
                a--; b--; c--;
            
                elements.push_back(a); elements.push_back(b); elements.push_back(c);
            }
            else
            {
                string s(line.substr(2));
                
                // firsst
                int idv1 = s.find_first_of(" ");
                string v1(s.substr(0,idv1));
                
                int idx = v1.find_first_of("/");
                istringstream c1(v1.substr(0,idx));
                GLushort a,b,c;
                c1 >> a;
                
                
                // second
                s = s.substr(idv1+1, s.length()-idv1);
                
                int idv2 = s.find_first_of(" ");
                string v2(s.substr(0,idv2));
                
                idx = v2.find_first_of("/");
                istringstream c2(v2.substr(0,idx));
                c2 >> b;
                
                // third
                string v3 = s.substr(idv2+1, s.length()-idv2);
                
                idx = v3.find_first_of("/");
                istringstream c3(v3.substr(0,idx));
                c3 >> c;
                
               // cout << a << " : " << b << " : " << c << endl;;
                elements.push_back(a); elements.push_back(b); elements.push_back(c);
            }
        }
        else if (line.substr(0,3) == "vn ")
        {
            istringstream s(line.substr(3));
            glm::vec3 n; s >> n.x; s >> n.y; s >> n.z;
            //normals.push_back(n);
        }
        else if (line.substr(0,7) == "mtllib ")
        {
            istringstream s(line.substr(7));
            s >> _material_file;
            /* ignoring this line */
        }
        else if (line.substr(0,2) == "g ")
        {
            istringstream s(line.substr(2));
            s >> _model_name;
            /* ignoring this line */
        }
        else if (line[0] == '#')
        {
            /* ignoring this line */
        }
        else
        {
            /* ignoring this line */
        }
    }
    
   // if(normals.size() > 0) return; // loaded normal vectors
    
    normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
    for (int i = 0; i < elements.size(); i+=3)
    {
        GLushort ia = elements[i];
        GLushort ib = elements[i+1];
        GLushort ic = elements[i+2];
        glm::vec3 normal = glm::normalize(glm::cross(
                                                     glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
                                                     glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
        normals[ia] = normals[ib] = normals[ic] = normal;
        
        
        _vertices2.push_back(glm::vec3(vertices[elements[i]]));
        _vertices2.push_back(glm::vec3(vertices[elements[i+1]]));
        _vertices2.push_back(glm::vec3(vertices[elements[i]+2]));
        
        _vertex_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
        _vertex_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
        _vertex_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
    }
    
    return true;
}


/*!
 Create the vertex buffer object for this element
 */
void ModelObj::initVBO(void)
{
    
    float* v = new float[_vertices2.size() * 3];
    for(int i=0; i<_vertices2.size() ; i++)
    {
        glm::vec3 t = _vertices2[i];
        for (int j=0; j<3; j++) {
            v[(i*3)+j] = t[j];
        }
    }
    
    
    glGenVertexArrays(1, _vaoID); // Create our Vertex Array Object
    glBindVertexArray(_vaoID[0]); // Bind our Vertex Array Object so we can use it
    
    
    glGenBuffers(3, _vboID); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _vertices2.size() * 3 * sizeof(glm::vec3), v, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(0); //
    
    
    //Color
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _vertex_colors.size() * sizeof(GLfloat), &_vertex_colors[0], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(1); //
    
    
     //Normals
    glBindBuffer(GL_ARRAY_BUFFER, _vboID[2]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(GLfloat), &_normals[0], GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(2); //
    

    glBindVertexArray(0); // Disable our Vertex Buffer Object


}



/*!
 Draw the objects
 */
void ModelObj::draw(void)
{

    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(_vaoID[0]);
    

    // Draw the triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertices2.size());
    
    
    // Unbind our Vertex Array Object
    glBindVertexArray(0);
    
}
