#include "FColladaModelLoader.h"
#include "../managers/SimpleMeshManager.h"

FColladaModelLoader::FColladaModelLoader(void){
	FCollada::Initialize(); 
	m_is_file_opened=false;
	m_document=NULL;
}

FColladaModelLoader::~FColladaModelLoader(void){
}

bool FColladaModelLoader::loadModel(int kind, char* szPathName){

	meshManager = SimpleMeshManager::getInstance();

	// new dae file
	m_document = FCollada::NewTopDocument();

	OLECHAR		wszPath[MAX_PATH+1];
	char		szPath[MAX_PATH+1];	
	if (strstr(szPathName, "http://"))									
	{
		strcpy(szPath, szPathName);										
	}
	else																
	{
		GetCurrentDirectoryA(MAX_PATH, szPath);							
		strcat(szPath, "\\");											
		strcat(szPath, szPathName);										
	}

	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);
	fstring fpath=FUStringConversion::ToFString(wszPath);
	bool ret=FCollada::LoadDocumentFromFile(m_document, fpath);	

	if (ret==false || m_document==NULL) {
		m_is_file_opened=false;
		return false;
	} else {
		// here, file was opened sucusefully
		m_is_file_opened=true;
	}

	// how many geometries there are?
	FCDGeometryLibrary* geolib=m_document->GetGeometryLibrary();

	// count the number of meshes (nurbs and splines are included in geolib, so descard them)
	int m_num_geometries=0;
	for (int i=0; i< (int) geolib->GetEntityCount(); i++)
	{
		if (geolib->GetEntity(i)->IsMesh()==true) 
		{
			m_num_geometries++;
		}
	}

	// copy all geometry objects from fcollada to my structures
	FCDGeometry* geo;
	for (int j=0; j<(int) geolib->GetEntityCount(); j++) { // REWVISAR hacer algo como m_num_meshse
		geo = geolib->GetEntity(j);

		// there are 3 types of geometries, nurbs, splines and meshes
		// at the moment JUST meshes, neither nurbs nor splines
		// meshes
		if (geo->IsMesh()) {
			Mesh* meshes = new Mesh();
			FCDGeometryMesh* mesh=geo->GetMesh();
			// triangulate this mesh if it was not triangulated
			if (!mesh->IsTriangles()) 
			{
				FCDGeometryPolygonsTools::Triangulate(mesh);
			}

			// create my own polygons
			int m_num_polygons=(int) mesh->GetPolygonsCount();

			for (int i=0;i<m_num_polygons;i++) {
				FCDGeometryPolygons* ptr_polygons = mesh->GetPolygons(i);
				FCDGeometrySource* source;
				FCDGeometryPolygonsInput* geometrypolygonsinput;
				uint32* indices;
				// indices to vertex
				geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::POSITION);
				indices = geometrypolygonsinput->GetIndices();

				//m_num_vertices=(int) indices->size();
				int m_num_vertices=(int) geometrypolygonsinput->GetIndexCount();

				/*store the vertex*/

				// source of vertex
				source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::POSITION);

				// allocate memory for triangles and its vertex (a triangle has 3 vertex)
				m_ptrs_vertices = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );

				// look for vertices using indices
				// 3 contiguiusos vertex form a triangle

				for (int i=0; i<m_num_vertices; i++) {
					// a vertex index
					int index=(int) indices[i];
					// a vertex values from it index
					float *p=&source->GetData()[index*3];
					m_ptrs_vertices[i].x=p[0]/50;
					m_ptrs_vertices[i].y=p[1]/50;
					m_ptrs_vertices[i].z=p[2]/50;
				}

				/*store the normal*/

				// source of vertex
				source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::NORMAL);

				// maybe they are not present
				if (source==NULL) {
					m_has_normals=false;
				} else {
					m_has_normals=true;
					// indices
					geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::NORMAL);
					indices = geometrypolygonsinput->GetIndices();

					// allocate memory for triangles and its vertex (a triangle has 3 vertex)
					m_ptrs_normals = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );

					// look for vertices using indices
					for (int i=0; i<m_num_vertices; i++) {
						// a vertex index
						int index=(int) indices[i];
						// a vertex values from it index
						float *p=&source->GetData()[index*3];
						m_ptrs_normals[i].x=p[0];
						m_ptrs_normals[i].y=p[1];
						m_ptrs_normals[i].z=p[2];
					}
				}

				/*store the texture*/

				// do the same for texcture coordinates
				source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::TEXCOORD);

				// maybe they are not present
				if (source==NULL) {
					m_has_texcoords=false;
				} else {
					m_has_texcoords=true;

					// indices
					geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::TEXCOORD);
					indices = geometrypolygonsinput->GetIndices();

					// allocate memory for triangles and its vertex (a triangle has 3 vertex)
					m_ptrs_texcoords = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );
					int stride;
					stride=source->GetStride();

					// 3ds max exports textures with 3 coordinates, but maya and Google Earth use 2 coordinates

					// look for vertices using indices
					for (int i=0; i<m_num_vertices; i++) {
						// a vertex index
						int index=(int) indices[i];
						// a vertex values from it index
						float *p=&source->GetData()[index*stride];
						m_ptrs_texcoords[i].x=p[0];
						m_ptrs_texcoords[i].y=p[1];
						if (stride==3) {
							m_ptrs_texcoords[i].z=p[2]; 
						}
						else{
							m_ptrs_texcoords[i].z=0.0f;
						}
					}
				}

				for (int i=0; i<m_num_vertices; i+=3) {
					MyPolygon* myPolygon = new MyPolygon();
					myPolygon->vertices->push_back(m_ptrs_vertices+i);
					myPolygon->vertices->push_back(m_ptrs_vertices+i+1);
					myPolygon->vertices->push_back(m_ptrs_vertices+i+2);
					myPolygon->hasTexture= m_has_texcoords;
					if(myPolygon->hasTexture==true)
					{
					myPolygon->verticeTexCoords->push_back(m_ptrs_texcoords+i);
					myPolygon->verticeTexCoords->push_back(m_ptrs_texcoords+i+1);
					myPolygon->verticeTexCoords->push_back(m_ptrs_texcoords+i+2);
					}
					myPolygon->hasNormals = m_has_normals;
					if(myPolygon->hasNormals ==true)
					{
					myPolygon->verticeNormals->push_back(m_ptrs_normals+i);
					myPolygon->verticeNormals->push_back(m_ptrs_normals+i+1);
					myPolygon->verticeNormals->push_back(m_ptrs_normals+i+2);
					}
					meshes->polygons->push_back(myPolygon);
				}
			}
			meshManager->addMesh(0,meshes);		
		}
	}
	return true;
}
