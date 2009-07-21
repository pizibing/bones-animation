#pragma once

class VBOMesh;
class VBOObject;
class VBOMesh;

class TerrainObject :
	public VBOObject
{
public:
	TerrainObject(void);
	~TerrainObject(void);

	// return a list of VBOMeshes that this VBOObject represent
	// this function return a pointer to the first VBOMesh of the 
	// list and num will be changed to the size of the list
	VBOMesh* representInVBOMesh(int* num);

	//return the type of this object
	int getType();

	//return the id of this object
	int getID();

private:
	// vbomesh that represent the terrain
	VBOMesh* vbomesh;
};
