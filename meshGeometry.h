// -----------------------------------------------------------
/**
 * \file meshGeometry.h
 * \author Vratislav Bartonicek
 * \date Fall semester 2014
 * \brief Definition of mesh geometry
*/
// ----------------------------------------------------------
#pragma once

class MeshGeometry{
public:
	GLuint vertexBufferObject;  /// The vertex buffer object
	GLuint elementBufferObject; /// The element buffer object
	GLuint vertexArrayObject;   /// The vertex array object
	unsigned int numVertices;   /// Number of vertices
	unsigned int numTriangles;  /// Number of triangles
	
	//material
	glm::vec3 ambient;  /// The ambient color
	glm::vec3 diffuse;  /// The diffuse color
	glm::vec3 specular; /// The specular color
	float shininess;	/// The shininess attribute
	GLuint texture; /// The texture index
};