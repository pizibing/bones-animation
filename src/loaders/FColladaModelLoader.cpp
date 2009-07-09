#include <olectl.h>	
#include <assert.h>
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

bool FColladaModelLoader::loadModel(int kind,const char* szPathName){

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

	assert(ret);

	//store the vertices, normals, texturecoords and create staticobjects to display
	storeVertices(m_document);

	//store all the textures that contains in the document
	storeTexture(m_document);

	//store all the materials that contains in the document
	storeMaterials(m_document);

	//build the scene
	FCDSceneNode* ptr_root=m_document->GetVisualSceneRoot();
	if(ptr_root!=NULL)
	{
		buildScene(ptr_root);
	}

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
	m_ptrs_geometries.reserve(m_num_geometries);

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
			GLfloat * m_vbo_texcoords = new GLfloat[vbo_vertexs_count * 3];

			//reset the counter 
			vbo_vertexs_count=0;

			for (int p=0;p<m_num_polygons;p++) {
				FCDGeometryPolygons* ptr_polygons = mesh->GetPolygons(p);
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
				Vector3D *m_ptrs_texcoords;
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

				//convert the Vector3D vertexs data to my structures 
				for(int i=vbo_vertexs_count; i <  vbo_vertexs_count+m_num_vertices; i++)
				{
					m_vbo_vertices[3*i] = (m_ptrs_vertices+i-vbo_vertexs_count)->x;
					m_vbo_vertices[3*i+1] = (m_ptrs_vertices+i-vbo_vertexs_count)->y;
					m_vbo_vertices[3*i+2] = (m_ptrs_vertices+i-vbo_vertexs_count)->z;
				}
				if(m_has_normals == true)
				{
					//convert the Vector3D normals data to my structures 
					for(int i=vbo_vertexs_count; i <  vbo_vertexs_count+m_num_vertices; i++)
					{
						m_vbo_normals[3*i] = (m_ptrs_normals+i-vbo_vertexs_count)->x;
						m_vbo_normals[3*i+1] = (m_ptrs_normals+i-vbo_vertexs_count)->y;
						m_vbo_normals[3*i+2] = (m_ptrs_normals+i-vbo_vertexs_count)->z;
					}
				}
				if(m_has_texcoords == true)
				{
					//convert the Vector3D texcoords data to my structures 
					for(int i=vbo_vertexs_count; i <  vbo_vertexs_count+m_num_vertices; i++)
					{
						m_vbo_texcoords[2*i] = (m_ptrs_texcoords+i-vbo_vertexs_count)->x;
						m_vbo_texcoords[2*i+1] = (m_ptrs_texcoords+i-vbo_vertexs_count)->y;
					}
				}
				//reflash the counter
				vbo_vertexs_count+=m_num_vertices;
			}

			//create the staticobject with vertexs
			StaticObject* objects = new StaticObject(m_vbo_vertices, vbo_vertexs_count * 3);

			//set the normals of the staticobject
			objects->setNormals(m_vbo_normals, vbo_vertexs_count * 3);

			TextureManager* textureManager = TextureManager::getInstance();
			GLuint tex = textureManager->getTextureId("resource/leaf2.jpg");
			//set the texcoords of the staticobject
			objects->setTextures(m_vbo_texcoords, vbo_vertexs_count * 2, tex);

			//add the objects to objectmanage
			objectManager->addVBOObject(objects);

			m_ptrs_geometries.push_back(mesh);
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
		m_ptrs_textures.push_back(image);
		//GLuint tex = textureManager->getTextureId(image->GetFilename().c_str());
	}
}

//store all the materials that contains in the document
void FColladaModelLoader::storeMaterials(FCDocument* m_document){
	//get the material from the document
	FCDMaterialLibrary* materiallib=m_document->GetMaterialLibrary();
	//get the number of the material
	m_num_materials=(int) materiallib->GetEntityCount();
	m_ptrs_materials.reserve(m_num_materials);

	// copy lights to my structures	
	FCDMaterial* material;

	for (int i=0; i<m_num_materials; i++) {
		material = materiallib->GetEntity(i);
		m_ptrs_materials.push_back(material);
	}
}

void FColladaModelLoader::buildScene(FCDSceneNode* node_origin)
{
	FCDSceneNode* child_origin;

	// copy node instances
	FCDEntityInstance* instance;
	FCDEntity* entity;
	std::string name;

	bool flag_found;
	for (int i=0; i<(int)node_origin->GetInstanceCount(); i++) {

		flag_found=false;
		instance=node_origin->GetInstance(i);
		entity=instance->GetEntity();
		name=entity->GetDaeId().c_str();
		// look for this name in geo library
		for (int j=0; j<(int)m_ptrs_geometries.size(); j++) {
			int target;
			if (m_ptrs_geometries[j]->GetDaeId().c_str()==name) {
				FCDGeometryInstance* geometry_instance=dynamic_cast<FCDGeometryInstance*>(instance);
				FCDGeometryMesh* mesh=dynamic_cast<FCDGeometryMesh*>(m_ptrs_geometries[j]);
				target = getFCMaterial(geometry_instance, mesh);
				if(target != -1)
				{
					setFCMaterial(j, target);
				}
				flag_found=true;
				break;
			}
		}
		if (flag_found) continue;
	}
	for (int i=0; i<(int)node_origin->GetChildrenCount(); i++) {
		child_origin=node_origin->GetChild(i);
		buildScene(child_origin);
	}
}

int FColladaModelLoader::getFCMaterial(FCDGeometryInstance* geometry_instance, FCDGeometryMesh* mesh)
{
	// fill this mesh instance with polygons instances
	std::string id_material;
	FCDMaterial* material;
	FCDMaterialInstance* materialinstance;

	for (int i=0; i<(int)mesh->GetPolygonsCount(); i++) {

		material=NULL;
		// look for this material_semantic in geometry_instance
		for (int k=0; k<(int)geometry_instance->GetMaterialInstanceCount(); k++) {
			// look for this material in my material lib, so I store a pointer
			materialinstance=geometry_instance->GetMaterialInstance(k);
			if (materialinstance->GetSemantic()==mesh->GetPolygons(i)->GetMaterialSemantic()) {
				id_material=materialinstance->GetMaterial()->GetDaeId().c_str();

				// look for a pointer for that material in my database
				for (int j=0; j<m_num_materials; j++){
					if (m_ptrs_materials[j]->GetDaeId().c_str()==id_material) {
						return j;
					}
				}
			}
		}

	}
	return -1;
}

void FColladaModelLoader::setFCMaterial(int j, int target)
{
	FCDMaterial* material = m_ptrs_materials[target];
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

	GLfloat am[4] = {m_ambient.GetX(), m_ambient.GetY(), m_ambient.GetZ(), m_ambient.GetW()};
	GLfloat di[4] = {m_diffuse.GetX(), m_diffuse.GetY(), m_diffuse.GetZ(), m_diffuse.GetW()};
	GLfloat sp[4] = {m_specular.GetX(), m_specular.GetY(), m_specular.GetZ(), m_specular.GetW()};
	GLfloat em[4] = {m_emission.GetX(), m_emission.GetY(), m_emission.GetZ(), m_emission.GetW()};
	GLfloat sh = (GLfloat)m_shininess;
	((StaticObject *) objectManager->getVBOObject(2,j))->setMaterial(am, di, sp, em, sh);
}