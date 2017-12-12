
#include "LoadAndCreateObj.h"


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <experimental\filesystem>

bool LoadAndCreateObj::extractNextFace1(string& in, string& out, int& pointIdx0, int& pointIdx1, int& pointIdx2)
{
	GLuint a, b, c;


	// remove the first space
	int idv1 = in.find_first_of(" ");
	string v1(in.substr(idv1 + 1, in.length() - idv1)); // remove it

	sscanf(v1.c_str(), " %d %d %d", &a, &b, &c);

	return true;
}




bool LoadAndCreateObj::extractNextFace3(string& in, string& out, int& pointIdx, int& texIdx, int& normalIdx)
{
	GLuint a, b, c;

	// first
	int idv1 = in.find_first_of(" ");
	string v1(in.substr(idv1 + 1, in.length() - idv1));

	sscanf(v1.c_str(), " %d/%d/%d", &a, &b, &c);

	int idv2 = v1.find_first_of(" ");
	if (idv2 != -1)
	{
		out = v1.substr(idv2, v1.length() - idv2); // with blank
	}
	else
	{
		out = "";
	}

	/*
	int idx = v1.find_first_of("/");
	istringstream c1(v1.substr(0,idx));


	c1 >> a;

	// second
	string v2 = v1.substr(idx+1, v1.length()-idx);

	int idv2 = v2.find_first_of("/");
	istringstream c2(v2.substr(0,idv2));
	c2 >> b;


	// third
	string v3 = v2.substr(idv2+1, v2.length()-idv2);

	int idv3 = v3.find_first_of(" ");
	istringstream c3(v3.substr(0,idv3));

	c3 >> c;

	if(idv3 != -1)
	out = v3.substr(idv3, in.length()-idv3); // with blank
	else
	out = "";
	*/

	pointIdx = a;
	texIdx = b;
	normalIdx = c;

	return true;

}




bool LoadAndCreateObj::Load(const char* filename, vector<glm::vec3> &vertices, vector<glm::vec3> &normals, vector<GLuint> &elements)
{
	ifstream in(filename, ios::in);
	if (!in)
	{
		cerr << "Cannot open " << filename << endl;
		return false;
	}



	vector<glm::vec3> temp_normals;
	vector<glm::vec3> temp_vertices;
	vector<glm::vec4> temp_color;
	vector<glm::vec3> temp_index_normal2point;
	vector<glm::vec3> temp_index_triangle;
	vector<glm::vec3> temp_index_textures;

	string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			istringstream s(line.substr(2));
			glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;// v.w = 1.0f;
			temp_vertices.push_back(v);
			temp_color.push_back(glm::vec4(1.0, 0.0, 0.0, 1.0));

		}
		else if (line.substr(0, 2) == "f ")
		{
			int idx = line.find_first_of("/");
			if (idx == -1)
			{
				istringstream s(line.substr(2));
				GLushort a, b, c;
				s >> a; s >> b; s >> c;
				cout << a << " : " << b << " : " << c << endl;;
				a--; b--; c--;

				elements.push_back(a); elements.push_back(b); elements.push_back(c);
			}
			else
			{

				size_t n = std::count(line.begin(), line.end(), '/');
				if (n == 0)
				{
					string new_line;
					int p0, p1, p2;
					extractNextFace1(line, new_line, p0, p1, p2);

					temp_index_triangle.push_back(glm::vec3(p0, p1, p2));
					temp_index_normal2point.push_back(glm::vec3(p0, p1, p2)); // same normal vectors.

				}
				else if (n == 3)
				{
					string new_line;
					int p0, p1, p2;
					extractNextFace1(line, new_line, p0, p1, p2);

					temp_index_triangle.push_back(glm::vec3(p0, p1, p2));
					temp_index_normal2point.push_back(glm::vec3(p0, p1, p2)); // same normal vectors.


				}
				else if (n == 6)
				{
					string new_line;
					int p0, t0, n0;
					int p1, t1, n1;
					int p2, t2, n2;
					int p3, t3, n3;

					// first
					extractNextFace3(line, new_line, p0, t0, n0);

					// second
					extractNextFace3(new_line, new_line, p1, t1, n1);

					// third
					extractNextFace3(new_line, new_line, p2, t2, n2);

					// Quads and not triangles
					if (new_line.length() > 2)
					{
						// fourth
						extractNextFace3(new_line, new_line, p3, t3, n3);
					}

					// remember the normal vectors
					temp_index_triangle.push_back(glm::vec3(p0, p1, p2));
					temp_index_normal2point.push_back(glm::vec3(n0, n1, n2));
					temp_index_textures.push_back(glm::vec3(t0, t1, t2));

					elements.push_back(p0);
					elements.push_back(p1);
					elements.push_back(p2);

				}


			}
		}
		else if (line.substr(0, 3) == "vn ")
		{
			istringstream s(line.substr(3));
			glm::vec3 n; s >> n.x; s >> n.y; s >> n.z;
			temp_normals.push_back(n);
		}
		else if (line.substr(0, 7) == "mtllib ")
		{
			string _material_file;
			istringstream s(line.substr(7));
			s >> _material_file;
			/* ignoring this line */
		}
		else if (line.substr(0, 2) == "g ")
		{
			string _model_name;
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


	/////////////////////////////////////////////
	// assign normals to points and points to triangles.

	vertices.clear();
	normals.clear();
	for (int i = 0; i<temp_index_triangle.size(); i++)
	{
		glm::vec3 pointIdx = temp_index_triangle[i];
		glm::vec3 normalIdx = temp_index_normal2point[i];

		// the three points of one triangle
		// -1 since obj does not use the index 0
		vertices.push_back(temp_vertices[pointIdx.x - 1]);
		vertices.push_back(temp_vertices[pointIdx.y - 1]);
		vertices.push_back(temp_vertices[pointIdx.z - 1]);

		normals.push_back(temp_normals[normalIdx.x - 1]);
		normals.push_back(temp_normals[normalIdx.y - 1]);
		normals.push_back(temp_normals[normalIdx.z - 1]);

	}







	if (normals.size() > 0) return true; // loaded normal vectors

	normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	for (int i = 0; i < elements.size(); i += 3)
	{
		GLushort ia = elements[i];
		GLushort ib = elements[i + 1];
		GLushort ic = elements[i + 2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
			glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
		normals[ia] = normals[ib] = normals[ic] = normal;


		// _vertex_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
		//  _vertex_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
		// _vertex_colors.push_back(glm::vec4(1.0,0.0,0.0,1.0));
	}

	return true;
}