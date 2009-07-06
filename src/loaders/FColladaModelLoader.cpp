#include <olectl.h>	
#include "../matrixlib/Vector3D.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../data/static/StaticObject.h"
#include "../managers/ObjectManager.h"
#include "ModelLoader.h"
#include "../managers/TextureManager.h"
#include "FCVector4D.h"
#include "FColladaModelLoader.h"

//constructor
FColladaModelLoader::FColladaModelLoader(void){
	
	//initialize the Fcollada namespace
	FCollada::Initialize(); 

	//initialize the values
	m_is_file_opened=false;
	m_document=NULL;
}

//destructor
FColladaModelLoader::~FColladaModelLoader(void){
	delete m_document;
}

//load the model from a path of a fcollada file
//return ture if load success
//set the document value while load the model
//it also contain three methed storeVertices, storeTexture, storeMaterials. which
//store the vertices, texture, materials to my own structure 

bool FColladaModelLoader::loadModel(int kind, char* szPathName){

	//it is a class value, which administrate the 
	objectManager = ObjectManager::getInstance();

	// new dae file
	m_document = FCollada::NewTopDocument();

	//path in unicode
	OLECHAR		wszPath[MAX_PATH+1];

	//temp path
	char		szPath[MAX_PATH+1];	

	//if the path contains the http:// , then copy the path to the szPath 
	if (strstr(szPathName, "http://"))									
	{
		strcpy(szPath, szPathName);										
	}
	// else load the model from the file
	else																
	{
		// get the current path
		GetCurrentDirectoryA(MAX_PATH, szPath);	

		//add the '\'
		strcat(szPath, "\\");

		//add the relative path
		strcat(szPath, szPathName);										
	}

	//convert ascii to unicode
	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);

	//convert the type of path to fstring
	fstring fpath=FUStringConversion::ToFString(wszPath);

	//load the document from the file. return true, if success
	bool ret=FCollada::LoadDocumentFromFile(m_document, fpath);	

	if (ret==false || m_document==NULL) {
		m_is_file_opened=false;
		return false;
	} else {
		// here, file was opened sucusefully
		m_is_file_opened=true;
	}

	//store the vertices, normals, texturecoords and create staticobjects to display
	storeVertices(m_document);

	//store all the textures that contains in the document
	storeTexture(m_document);

	//store all the materials that contains in the document
	storeMaterials(m_document);
	return true;
}

//store the vertices, normals, texturecoords and create staticobjects to display
void FColladaModelLoader::storeVertices(FCDocument* m_document)
{
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
			FCDGeometryMesh* mesh=geo->GetMesh();
			// triangulate this mesh if it was not triangulated
			if (!mesh->IsTriangles()) 
			{
				FCDGeometryPolygonsTools::Triangulate(mesh);
			}

			// create my own polygons
			int m_num_polygons=(int) mesh->GetPolygonsCount();

			// count the total number of vertexs
			int vbo_vertexs_count=0;

			//get the total number of vertexs
			for (int i=0;i<m_num_polygons;i++) {
				FCDGeometryPolygons* ptr_polygons = mesh->GetPolygons(i);
				FCDGeometryPolygonsInput* geometrypolygonsinput;
				geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::POSITION);
				int m_num_vertices=(int) geometrypolygonsinput->GetIndexCount();
				vbo_vertexs_count+=m_num_vertices;
			}

			//init the total data of the vertexs and normals
			GLfloat * m_vbo_vertices = new GLfloat[vbo_vertexs_count * 3];
			GLfloat * m_vbo_normals = new GLfloat[vbo_vertexs_count * 3];

			//reset the counter 
			vbo_vertexs_count=0;

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
				Vector3D *m_ptrs_vertices = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );

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
				bool m_has_normals;
				Vector3D *m_ptrs_normals;
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

				/*store the texture coordinates*/

				// do the same for texcture coordinates
				source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::TEXCOORD);
				bool m_has_texcoords;
				// maybe they are not present
				if (source==NULL) {
					m_has_texcoords=false;
				} else {
					m_has_texcoords=true;

					// indices
					geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::TEXCOORD);
					indices = geometrypolygonsinput->GetIndices();

					// allocate memory for triangles and its vertex (a triangle has 3 vertex)
					Vector3D *m_ptrs_texcoords = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );
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

				//convert the Vector3D vertexs data to my structures 
				for(int i=vbo_vertexs_count; i <  vbo_vertexs_count+m_num_vertices; i++)
				{
					m_vbo_vertices[3*i] = (m_ptrs_vertices+i-vbo_vertexs_count)->x;
					m_vbo_vertices[3*i+1] = (m_ptrs_vertices+i-vbo_vertexs_count)->y;
					m_vbo_vertices[3*i+2] = (m_ptrs_vertices+i-vbo_vertexs_count)->z;
				}

				//convert the Vector3D normals data to my structures 
				for(int i=vbo_vertexs_count; i <  vbo_vertexs_count+m_num_vertices; i++)
				{
					m_vbo_normals[3*i] = (m_ptrs_normals+i-vbo_vertexs_count)->x;
					m_vbo_normals[3*i+1] = (m_ptrs_normals+i-vbo_vertexs_count)->y;
					m_vbo_normals[3*i+2] = (m_ptrs_normals+i-vbo_vertexs_count)->z;
				}

				//reflash the counter
				vbo_vertexs_count+=m_num_vertices;
			}

			//create the staticobject with vertexs
			StaticObject* objects = new StaticObject(m_vbo_vertices, vbo_vertexs_count * 3);

			//set the normals of the staticobject
			objects->setNormals(m_vbo_normals, vbo_vertexs_count * 3);

			//add the objects to objectmanage
			objectManager->addVBOObject(objects);
		}
	}
}




//store all the textures that contains in the document
void FColladaModelLoader::storeTexture(FCDocument* m_document){
	//get the image from the document
	FCDImageLibrary* imagelib=m_document->GetImageLibrary();
	//get the number of the image
	m_num_textures=(int) imagelib->GetEntityCount();
	// copy textures to my structures
	FCDImage* image;
	TextureManager* textureManager = TextureManager::getInstance();
	for (int i=0; i<m_num_textures; i++) {
		image = imagelib->GetEntity(i);
		//GLuint tex = textureManager->getTextureId(image->GetFilename().c_str());
	}
}

//store all the materials that contains in the document
void FColladaModelLoader::storeMaterials(FCDocument* m_document){
	//get the material from the document
	FCDMaterialLibrary* materiallib=m_document->GetMaterialLibrary();
	//get the number of the material
	m_num_materials=(int) materiallib->GetEntityCount();
	// copy lights to my structures	
	FCDMaterial* material;

	for (int i=0; i<m_num_materials; i++) {
		material = materiallib->GetEntity(i);

		// if common profile does not exist, the set as a default materiqal
		FCDEffect* fx = material->GetEffect();
		FCDEffectProfile* profile = fx->FindProfile(FUDaeProfileType::COMMON);

		if (profile==NULL) {
			return;
		}

		// copy properties of common profile
		FCDEffectStandard* standardProfile=dynamic_cast<FCDEffectStandard*>(profile);

		// configure alpha value in diffuse material
		float m_transparency=standardProfile->GetTranslucencyFactor();

		float m_shininess;
		FCVector4D m_emission;
		FCVector4D m_ambient;
		FCVector4D m_diffuse;
		FCVector4D m_specular;

		// ambient
		m_ambient.Set(
			standardProfile->GetAmbientColor().x,
			standardProfile->GetAmbientColor().y,
			standardProfile->GetAmbientColor().z,
			standardProfile->GetAmbientColor().w			  
			);

		// diffuse component
		m_diffuse.Set(
			standardProfile->GetDiffuseColor().x,
			standardProfile->GetDiffuseColor().y,
			standardProfile->GetDiffuseColor().z,
			1.0 // opaque for opengl, use m_transparency on polygon::render 		  
			);

		// specular
		float specular_factor=standardProfile->GetSpecularFactor();
		m_specular.Set(
			specular_factor*standardProfile->GetSpecularColor().x,
			specular_factor*standardProfile->GetSpecularColor().y,
			specular_factor*standardProfile->GetSpecularColor().z,
			specular_factor*standardProfile->GetSpecularColor().w			  
			);

		// shininess
		m_shininess=standardProfile->GetShininess();

		// emission
		if (standardProfile->IsEmissionFactor()) {
			m_emission.Set(
				standardProfile->GetEmissionFactor(),
				standardProfile->GetEmissionFactor(),
				standardProfile->GetEmissionFactor(),
				standardProfile->GetEmissionFactor()
				);
		} else {
			m_emission.Set(
				standardProfile->GetEmissionColor().x*standardProfile->GetEmissionFactor(),
				standardProfile->GetEmissionColor().y*standardProfile->GetEmissionFactor(),
				standardProfile->GetEmissionColor().z*standardProfile->GetEmissionFactor(),
				standardProfile->GetEmissionColor().w*standardProfile->GetEmissionFactor()
				);
		}
		//store the five data to my structures
	}
}