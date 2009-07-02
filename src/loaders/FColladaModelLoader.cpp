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
	if (strstr(szPathName, "http://"))									// 如果路径包含 http:// 则...
	{
		strcpy(szPath, szPathName);										// 把路径拷贝到 szPath
	}
	else																// 否则从文件导入图片
	{
		GetCurrentDirectoryA(MAX_PATH, szPath);							// 取得当前路径
		strcat(szPath, "\\");											// 添加字符"\"
		strcat(szPath, szPathName);										// 添加图片的相对路径
	}

	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);		// 把ASCII码转化为Unicode标准码
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
	m_num_geometries=0;
	for (int i=0; i< (int) geolib->GetEntityCount(); i++)
	{
		if (geolib->GetEntity(i)->IsMesh()==true) 
		{
			m_num_geometries++;
		}
	}

	// copy all geometry objects from fcollada to my structures
	FCDGeometry* geo;
	for (int i=0; i<(int) geolib->GetEntityCount(); i++) { // REWVISAR hacer algo como m_num_meshse
		geo = geolib->GetEntity(i);

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

			for (i=0;i<m_num_polygons;i++) {
				FCDGeometryPolygons* ptr_polygons = mesh->GetPolygons(i);
				FCDGeometrySource* source;
				FCDGeometryPolygonsInput* geometrypolygonsinput;
				uint32* indices;
				// indices to vertex
				geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::POSITION);
				indices = geometrypolygonsinput->GetIndices();

				//m_num_vertices=(int) indices->size();
				int m_num_vertices=(int) geometrypolygonsinput->GetIndexCount();

				// source of vertex
				source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::POSITION);

				Vector3D *m_ptrs_vertices;
				// allocate memory for triangles and its vertex (a triangle has 3 vertex)
				m_ptrs_vertices = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );

				// look for vertices using indices
				// 3 contiguiusos vertex form a triangle

				for (int i=0; i<m_num_vertices; i++) {
					// a vertex index
					int index=(int) indices[i];
					// a vertex values from it index
					float *p=&source->GetData()[index*3];
					m_ptrs_vertices[i].x=p[0]/100;
					m_ptrs_vertices[i].y=p[1]/100;
					m_ptrs_vertices[i].z=p[2]/100;
				}

				for (int i=0; i<m_num_vertices; i+=3) {
				MyPolygon* myPolygon = new MyPolygon();
				myPolygon->vertices->push_back(m_ptrs_vertices+i);
				myPolygon->vertices->push_back(m_ptrs_vertices+i+1);
				myPolygon->vertices->push_back(m_ptrs_vertices+i+2);
				meshes->polygons->push_back(myPolygon);
				}
			}
			meshManager->addMesh(0,meshes);		
		}
	}

	return true;
}
