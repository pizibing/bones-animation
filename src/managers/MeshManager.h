#pragma once

#include <vector>

#define MESH_KIND_NUM 10
#define MESH_KIND_CHARACTER 0
#define MESH_KIND_TERRAIN 1
#define MESH_KIND_STATIC_OBJECT 2

class Mesh;

class MeshManager
{
public:
	/* 
     * para: kind: the kind of mesh
	 *       newMesh: the point of Mesh to be added
     * func: add the newMesh with the kind into the mesh manager
	 * return: void
	 */
	virtual void addMesh(int kind, Mesh* newMesh) = 0;

	/*
	 * para: kind: the kind of mesh
	 *       i: the i-th mesh of kind 
	 * func: delete the i-th mesh* of kind from the manager
	 * return: true if deleted successfully
	 */
	virtual bool deleteMesh(int kind, int i) = 0;

	/*
	 * para: kind: the kind of mesh
	 *       i: the i-th mesh of kind 
	 * func: get the i-th Mesh* of kind from the manager
	 * return: the i-th Mesh* of kind
	 */
	virtual Mesh* getMesh(int kind, int i) = 0;

	/*
	* para: kind: the kind of mesh
	* func: get all meshes of kind from the manager
	* return: a vector of point to mesh
	*/
	virtual std::vector<Mesh*>* getMeshes(int kind) = 0;
};