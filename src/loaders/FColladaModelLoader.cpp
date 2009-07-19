#include <olectl.h>	
#include <assert.h>
#include <string>
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
	storeVertices();

	storeLight();

	//store all the textures that contains in the document
	storeTexture();

	//store all the materials that contains in the document
	storeMaterials();

	storeCamera();

	storeAnimation();

	//build the scene
	FCDSceneNode* ptr_root=m_document->GetVisualSceneRoot();
	if(ptr_root!=NULL)
	{
		buildScene(ptr_root);
		drawLine(ptr_root);
	}

	return true;
}


//store the vertices, normals, texturecoords and create staticobjects to display
void FColladaModelLoader::storeVertices()
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

			for (int p=0;p<m_num_polygons;p++) {
				FCDGeometryPolygons* ptr_polygons = mesh->GetPolygons(p);
				FCDGeometrySource* source;
				FCDGeometryPolygonsInput* geometrypolygonsinput;
				uint32* indices;
				// indices to vertex
				geometrypolygonsinput=ptr_polygons->FindInput(FUDaeGeometryInput::POSITION);
				indices = geometrypolygonsinput->GetIndices();

				//m_num_vertices=(int) indices->size();
				m_num_vertices=(int) geometrypolygonsinput->GetIndexCount();
				/*store the vertex*/

				// source of vertex
				source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::POSITION);

				// allocate memory for triangles and its vertex (a triangle has 3 vertex)
				m_ptrs_vertices = (Vector3D*)malloc( m_num_vertices * sizeof(Vector3D) );

				// look for vertices using indices
				// 3 vertex form a triangle

				for (int i=0; i<m_num_vertices; i++) {
					// a vertex index
					int index=(int) indices[i];
					// a vertex values from it index
					float *p=&source->GetData()[index*3];
					m_ptrs_vertices[i].x=p[0];
					m_ptrs_vertices[i].y=p[1];
					m_ptrs_vertices[i].z=p[2];
				}

				/*store the normal*/

				// source of vertex
				source = ptr_polygons->GetParent()->FindSourceByType(FUDaeGeometryInput::NORMAL);

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

				//init the total data of the vertexs and normals
				GLfloat * m_vbo_vertices = new GLfloat[m_num_vertices * 3];
				GLfloat * m_vbo_normals = new GLfloat[m_num_vertices * 3];
				GLfloat * m_vbo_texcoords = new GLfloat[m_num_vertices * 2];

				//convert the Vector3D vertexs data to my structures 
				for(int i=0; i<m_num_vertices; i++)
				{
					m_vbo_vertices[3*i] = (m_ptrs_vertices+i)->x;
					m_vbo_vertices[3*i+1] = (m_ptrs_vertices+i)->y;
					m_vbo_vertices[3*i+2] = (m_ptrs_vertices+i)->z;
				}
				if(m_has_normals == true)
				{
					//convert the Vector3D normals data to my structures 
					for(int i=0; i<m_num_vertices; i++)
					{
						m_vbo_normals[3*i] = (m_ptrs_normals+i)->x;
						m_vbo_normals[3*i+1] = (m_ptrs_normals+i)->y;
						m_vbo_normals[3*i+2] = (m_ptrs_normals+i)->z;
					}
				}
				if(m_has_texcoords == true)
				{
					//convert the Vector3D texcoords data to my structures 
					for(int i=0; i<m_num_vertices; i++)
					{
						m_vbo_texcoords[2*i] = (m_ptrs_texcoords+i)->x;
						m_vbo_texcoords[2*i+1] = (m_ptrs_texcoords+i)->y;
					}
				}
				//create the staticobject with vertexs
				StaticObject* objects = new StaticObject(m_vbo_vertices, m_num_vertices * 3);

				//set the normals of the staticobject
				objects->setNormals(m_vbo_normals, m_num_vertices * 3);

				//set the texcoords of the staticobject
				m_total_texcoords.push_back(m_vbo_texcoords);

				m_size_texcoords.push_back(m_num_vertices * 2);

				//add the objects to objectmanage
				objectManager->addVBOObject(objects);
			}
			m_ptrs_geometries.push_back(mesh);
		}
	}
}





void FColladaModelLoader::storeLight(){
	// how many lights there are?
	FCDLightLibrary* lightlib=m_document->GetLightLibrary();
	m_num_lights=(int) lightlib->GetEntityCount();

	// copy lights to my structures
	FCDLight* light;

	for (int i=0; i<m_num_lights; i++) {
		light = lightlib->GetEntity(i);

		// ambiental light
		if (light->GetLightType()==FCDLight::AMBIENT) {
			// add our new light
			m_ptrs_Ambientlights.push_back(light);
			continue;
		}

		// spot light (luz focal)
		if (light->GetLightType()==FCDLight::SPOT) {
			// add our new light
			m_ptrs_Spotlights.push_back(light);
			continue;
		}

		// directional  light
		if (light->GetLightType()==FCDLight::DIRECTIONAL) {
			// add our new light
			m_ptrs_Directionallights.push_back(light);
			continue;
		}

		// point light
		if (light->GetLightType()==FCDLight::POINT) {
			// add our new light
			m_ptrs_Pointlights.push_back(light);
			continue;
		}
	}
}

//store all the textures that contains in the document
void FColladaModelLoader::storeTexture(){
	//get the image from the document
	FCDImageLibrary* imagelib=m_document->GetImageLibrary();
	//get the number of the image
	m_num_textures=(int) imagelib->GetEntityCount();
	// copy textures to my structures
	FCDImage* image;
	for (int i=0; i<m_num_textures; i++) {
		image = imagelib->GetEntity(i);
		m_ptrs_textures.push_back(image);
	}
}

//store all the materials that contains in the document
void FColladaModelLoader::storeMaterials(){
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

void FColladaModelLoader::storeCamera(){
	// copy cameras to my structures
	// cameras
	FCDCameraLibrary* cameralib=m_document->GetCameraLibrary();
	m_num_cameras=(int) cameralib->GetEntityCount();

	FCDCamera* camera;
	for (int i=0; i<m_num_cameras; i++) {
		camera = cameralib->GetEntity(i);

		// perspective camera
		if (camera->IsPerspective()==true) {
			// add our new camera
			m_ptrs_Perspectivecameras.push_back(camera);
			continue;
		}

		// orthographic camera
		if (camera->IsOrthographic()==true) {
			// add our new camera
			m_ptrs_Orthographiccameras.push_back(camera);
			continue;
		}
	}

}

void FColladaModelLoader::storeAnimation(){
	// how many animations are there?
	FCDAnimationLibrary* animationlib=m_document->GetAnimationLibrary();
	m_num_animations=(int) animationlib->GetEntityCount();

	FCDAnimation* animation;
	for (int i=0; i<m_num_animations; i++) {
		animation = animationlib->GetEntity(i);
		m_ptrs_animation.push_back(animation);
	}
}

//build the scene include the material of the polygons, the texture of the polygons and the matrix of the bone.
void FColladaModelLoader::buildScene(FCDSceneNode* node_origin)
{
	//child scene node
	FCDSceneNode* child_origin;

	FCDTransform*  trans_origin;

	// copy node transformations
	for (int i=0; i<(int)node_origin->GetTransformCount(); i++) {
		trans_origin=node_origin->GetTransform(i);

		// rotation
		if (trans_origin->GetType()== FCDTransform::ROTATION) {
			FCDTRotation* trans_rot=dynamic_cast<FCDTRotation*>(trans_origin); // casting

			// initialize curves
			FCDAnimationCurve* m_x_curve=NULL;
			FCDAnimationCurve* m_y_curve=NULL;
			FCDAnimationCurve* m_z_curve=NULL;
			FCDAnimationCurve* m_angle_curve=NULL;

			// is there any animation ?
			if (trans_rot->IsAnimated()) {

				FCDAnimationCurve* curve;

				// look for x animation
				curve=trans_rot->GetAnimated()->FindCurve(".X");
				if (curve!=NULL) {

				}

				// look for y animation
				curve=trans_rot->GetAnimated()->FindCurve(".Y");
				if (curve!=NULL){

				}

				// look for z animation
				curve=trans_rot->GetAnimated()->FindCurve(".Z");
				if (curve!=NULL){

				}

				// look for angle animation
				curve=trans_rot->GetAnimated()->FindCurve(".ANGLE");
				if (curve!=NULL){

				}
			}
			continue;
		}

		// translation
		if (trans_origin->GetType()== FCDTransform::TRANSLATION) {
			FCDTTranslation* trans_trans=dynamic_cast<FCDTTranslation*>(trans_origin); // casting
			// initialize curves
			FCDAnimationCurve* m_x_curve=NULL;
			FCDAnimationCurve* m_y_curve=NULL;
			FCDAnimationCurve* m_z_curve=NULL;

			// is there any animation ?
			if (trans_trans->IsAnimated()) {

				FCDAnimationCurve* curve;

				// look for x animation
				curve=trans_trans->GetAnimated()->FindCurve(".X");
				if (curve!=NULL){

				}

				// look for y animation
				curve=trans_trans->GetAnimated()->FindCurve(".Y");
				if (curve!=NULL) {

				}

				// look for z animation
				curve=trans_trans->GetAnimated()->FindCurve(".Z");
				if (curve!=NULL) {

				}
			}	
			continue;
		}

		// scale
		if (trans_origin->GetType()== FCDTransform::SCALE) {
			FCDTScale* trans_scale=dynamic_cast<FCDTScale*>(trans_origin); // casting 
			// initialize curves
			FCDAnimationCurve* m_x_curve=NULL;
			FCDAnimationCurve* m_y_curve=NULL;
			FCDAnimationCurve* m_z_curve=NULL;

			// is there any animation ?
			if (trans_scale->IsAnimated()) {

				FCDAnimationCurve* curve;

				// look for x animation
				curve=trans_scale->GetAnimated()->FindCurve(".X");
				if (curve!=NULL) {

				}

				// look for y animation
				curve=trans_scale->GetAnimated()->FindCurve(".Y");
				if (curve!=NULL) {

				}

				// look for z animation
				curve=trans_scale->GetAnimated()->FindCurve(".Z");
				if (curve!=NULL) {

				}
			}	
			continue; // actually, not necessary
		}

		// matrix
		if (trans_origin->GetType()== FCDTransform::MATRIX) {
			FCDTMatrix* trans_matrix=dynamic_cast<FCDTMatrix*>(trans_origin); // casting
			// animation
			FCDAnimated* animated;
			//int num_curves;
			if (trans_matrix->IsAnimated()) {		
				animated=trans_matrix->GetAnimated();
			}
			continue;
		}


	}

	// copy node instances
	FCDEntityInstance* instance;

	//the entity of the instance, and the name of the entity
	FCDEntity* entity;
	std::string name;

	//instance flag to check which the instance id found in the library
	bool flag_found;

	//look through the instance
	for (int i=0; i<(int)node_origin->GetInstanceCount(); i++) {

		//set or reset the parameter
		flag_found=false;
		instance=node_origin->GetInstance(i);
		entity=instance->GetEntity();
		name=entity->GetDaeId().c_str();

		//check if the instance is controller instance
		if(instance->GetType()==FCDEntityInstance::CONTROLLER)
		{
			//get the controller instance
			FCDControllerInstance* controllerInstance = dynamic_cast<FCDControllerInstance*>(instance);

			//get the skin controller
			FCDSkinController* skin = dynamic_cast<FCDController*>(controllerInstance->GetEntity())->GetSkinController();

			// look for this name in geo library
			for (int j=0; j<(int)m_ptrs_geometries.size(); j++) {

				//get the target geometries
				if (m_ptrs_geometries[j]->GetDaeId().c_str()==skin->GetTarget()->GetDaeId().c_str()) {
					
					//get the polygon index
					int meshIndex=0;
					for(int p=0; p<j; p++)
					{
						meshIndex+=(int)m_ptrs_geometries[p]->GetPolygonsCount();
					}

					//get the geometry mesh
					FCDGeometryMesh* mesh=m_ptrs_geometries[j];
					FCDMaterial* material;

					//the id of the pointed material
					std::string id_material;

					//look through the mesh
					for (int i=0; i<(int)mesh->GetPolygonsCount(); i++) {

						material=NULL;
						// look for this material_semantic in geometry_instance
						for (int k=0; k<(int)controllerInstance->GetMaterialInstanceCount(); k++) {
							// look for this material in my material lib

							FCDMaterialInstance* materialInstance = controllerInstance->GetMaterialInstance(k);
							//get the material instance from the geo instance

							//check whether the semantic of the material instance is equals to the semantic of the polygon 
							if (materialInstance->GetSemantic()==mesh->GetPolygons(i)->GetMaterialSemantic()) {
								//get the material id of the pointed material
								id_material=materialInstance->GetMaterial()->GetDaeId().c_str();

								// look for a pointer for that material in my material lib
								for (int j=0; j<m_num_materials; j++){
									if (m_ptrs_materials[j]->GetDaeId().c_str()==id_material) {
										//set the material of the polygon
										setFCMaterial(j, meshIndex+i);
									}
								}
							}
						}
					}
				}
			}

			/*
			for(int i = 0; i < (int)skin->GetJointCount(); i++)
			{
				FCDSkinControllerJoint* joint = skin->GetJoint(i);
				FMMatrix44 inverse = joint->GetBindPoseInverse() * skin->GetBindShapeTransform();
			}

			for(int i = 0; i < (int)skin->GetInfluenceCount(); i++)
			{
				FCDSkinControllerVertex* influences = skin->GetVertexInfluence(i);
				for(int j =0; j < (int)influences->GetPairCount(); j++)
				{
					FCDJointWeightPair* pairs = influences->GetPair(j);
				}
			}
			*/
		}
		// look for this name in geo library
		for (int j=0; j<(int)m_ptrs_geometries.size(); j++) {
			if (m_ptrs_geometries[j]->GetDaeId().c_str()==name) {
				//get the Geo instance
				FCDGeometryInstance* geometry_instance=dynamic_cast<FCDGeometryInstance*>(instance);
				//get the mesh
				FCDGeometryMesh* mesh=m_ptrs_geometries[j];
				//get the polygon index
				int meshIndex=0;
				for(int p=0; p<j; p++)
				{
					meshIndex+=(int)m_ptrs_geometries[p]->GetPolygonsCount();
				}
				//get the material
				setMeshFCMaterial(geometry_instance, mesh, meshIndex);
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

//set the material of the mesh
void FColladaModelLoader::setMeshFCMaterial(FCDGeometryInstance* geometry_instance, FCDGeometryMesh* mesh, int meshIndex)
{
	// fill this mesh instance with polygons instances

	//the id of the pointed material
	std::string id_material;

	//material and material instance
	FCDMaterial* material;
	FCDMaterialInstance* materialinstance;

	//look through the mesh
	for (int i=0; i<(int)mesh->GetPolygonsCount(); i++) {

		material=NULL;
		// look for this material_semantic in geometry_instance
		for (int k=0; k<(int)geometry_instance->GetMaterialInstanceCount(); k++) {
			// look for this material in my material lib

			//get the material instance from the geo instance
			materialinstance=geometry_instance->GetMaterialInstance(k);

			//check whether the semantic of the material instance is equals to the semantic of the polygon 
			if (materialinstance->GetSemantic()==mesh->GetPolygons(i)->GetMaterialSemantic()) {
				//get the material id of the pointed material
				id_material=materialinstance->GetMaterial()->GetDaeId().c_str();

				// look for a pointer for that material in my material lib
				for (int j=0; j<m_num_materials; j++){
					if (m_ptrs_materials[j]->GetDaeId().c_str()==id_material) {
						//set the material of the polygon
						setFCMaterial(j, meshIndex+i);
					}
				}
			}
		}
	}
}

//set the target material to the polygon
void FColladaModelLoader::setFCMaterial(int target, int index)
{
	//target material
	FCDMaterial* material = m_ptrs_materials[target];

	// pointer to diffuse texture
	FCDImage* m_texture_diffuse;
	bool m_has_diffuse_texture;

	// pointer to texture reflective
	FCDImage* m_texture_reflective;
	bool m_has_reflective_texture;

	// pointer to texture transparent
	FCDImage* m_texture_transparent;
	bool m_has_transparent_texture;

	// get the material effect and the profile
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

	//store the data to my structure
	GLfloat am[4] = {m_ambient.GetX(), m_ambient.GetY(), m_ambient.GetZ(), m_ambient.GetW()};
	GLfloat di[4] = {m_diffuse.GetX(), m_diffuse.GetY(), m_diffuse.GetZ(), m_diffuse.GetW()};
	GLfloat sp[4] = {m_specular.GetX(), m_specular.GetY(), m_specular.GetZ(), m_specular.GetW()};
	GLfloat em[4] = {m_emission.GetX(), m_emission.GetY(), m_emission.GetZ(), m_emission.GetW()};
	GLfloat sh = (GLfloat)m_shininess;

	/*texture*/
	// textures
	FCDTexture* texture;
	FCDImage *image;

	// diffuse textures
	m_has_diffuse_texture=false;
	m_texture_diffuse=NULL;
	image=NULL;

	// diffuse texture
	if (standardProfile->GetTextureCount(FUDaeTextureChannel::DIFFUSE)>0) {
		texture=standardProfile->GetTexture(FUDaeTextureChannel::DIFFUSE,0);
		if (texture!=NULL) {
			image=texture->GetImage();
			if (image!=NULL) {
				m_texture_diffuse=SearchTextureByName(image->GetDaeId());
				if (m_texture_diffuse!=NULL)
					m_has_diffuse_texture=true;
			}
		}
	}	

	// reflective texture
	m_has_reflective_texture=false;
	m_texture_reflective=NULL;
	image=NULL;

	// reflective texture
	if (standardProfile->GetTextureCount(FUDaeTextureChannel::REFLECTION)>0) {
		texture=standardProfile->GetTexture(FUDaeTextureChannel::REFLECTION,0);
		if (texture!=NULL) {
			image=texture->GetImage();
			if (image!=NULL) {
				m_texture_reflective=SearchTextureByName(image->GetDaeId());
				if (m_texture_reflective!=NULL)
					m_has_reflective_texture=true;
			}
		}
	}

	// transparent textures
	m_has_transparent_texture=false;
	m_texture_transparent=NULL;
	image=NULL;

	// transparent texture
	if (standardProfile->GetTextureCount(FUDaeTextureChannel::TRANSPARENT)>0) {
		texture=standardProfile->GetTexture(FUDaeTextureChannel::TRANSPARENT,0);
		if (texture!=NULL) {
			image=texture->GetImage();
			if (image!=NULL) {
				m_texture_transparent=SearchTextureByName(image->GetDaeId());
				if (m_texture_transparent!=NULL)
					m_has_transparent_texture=true;
			}
		}
	}

	//create the texture manage
	TextureManager* textureManager = TextureManager::getInstance();

	GLuint tex = NULL;

	//diffuse texture
	if(m_texture_diffuse!=NULL)
	{
		//the absolute file path
		char szPath[512];
		//convert unicode to string
		WideCharToMultiByte(CP_ACP, 0, m_texture_diffuse->GetFilename().c_str(), -1, szPath, MAX_PATH, NULL, NULL);	
		//temp path 
		char		tempPath[MAX_PATH+1];		
		// get the current path
		GetCurrentDirectoryA(MAX_PATH, tempPath);		
		//convert char* to string
		std::string tempDir=tempPath;
		//get the size of the string
		int szDirSize = tempDir.size();
		//convert char* to string
		std::string szFile=szPath;
		//get the relative file path
		szFile = szFile.substr(szDirSize+1);

		//get the texture from the relative path
		tex = textureManager->getTextureId(szFile.c_str());
	}

	//reflective texture
	if(m_texture_reflective!=NULL)
	{
		//the absolute file path
		char szPath[512];
		//convert unicode to string
		WideCharToMultiByte(CP_ACP, 0, m_texture_reflective->GetFilename().c_str(), -1, szPath, MAX_PATH, NULL, NULL);	
		//temp path 
		char		tempPath[MAX_PATH+1];		
		// get the current path
		GetCurrentDirectoryA(MAX_PATH, tempPath);		
		//convert char* to string
		std::string tempDir=tempPath;
		//get the size of the string
		int szDirSize = tempDir.size();
		//convert char* to string
		std::string szFile=szPath;
		//get the relative file path
		szFile = szFile.substr(szDirSize+1);

		//get the texture from the relative path
		tex = textureManager->getTextureId(szFile.c_str());
	}

	//transparent texture
	if(m_texture_transparent!=NULL)
	{
		//the absolute file path
		char szPath[512];
		//convert unicode to string
		WideCharToMultiByte(CP_ACP, 0, m_texture_transparent->GetFilename().c_str(), -1, szPath, MAX_PATH, NULL, NULL);	
		//temp path 
		char		tempPath[MAX_PATH+1];		
		// get the current path
		GetCurrentDirectoryA(MAX_PATH, tempPath);		
		//convert char* to string
		std::string tempDir=tempPath;
		//get the size of the string
		int szDirSize = tempDir.size();
		//convert char* to string
		std::string szFile=szPath;
		//get the relative file path
		szFile = szFile.substr(szDirSize+1);
		//get the texture from the relative path
		tex = textureManager->getTextureId(szFile.c_str());
	}

	//if has texture, set the texture
	if(tex != NULL)
	{
		((StaticObject *) objectManager->getVBOObject(2,index))->setTextures(m_total_texcoords[index], m_size_texcoords[index], tex);
	}

	//set the material
	((StaticObject *) objectManager->getVBOObject(2,index))->setMaterial(am, di, sp, em, sh);
}

//search the texture in the texture lib by texture id
FCDImage *FColladaModelLoader::SearchTextureByName(fm::string textureid){

	// look for this texture
	for (int i=0; i<m_num_textures; i++)
	{
		if (m_ptrs_textures[i]->GetDaeId()==textureid) 
		{
			return m_ptrs_textures[i];
		}
	}
	// if fail, null
	return NULL;
}





void FColladaModelLoader::drawLine(FCDSceneNode* node_origin)
{
	bool flag_found = false;

	flag_found = checkBone(node_origin);

	if(flag_found == true){
		FMVector3 v = node_origin->CalculateWorldTransform().GetTranslation();
	}
	for (int i=0; i<(int)node_origin->GetChildrenCount(); i++) {
		FCDSceneNode* child_origin=node_origin->GetChild(i);
		drawLine(child_origin);
	}
}

bool FColladaModelLoader::checkBone(FCDSceneNode* node_origin)
{
	FCDEntityInstance* instance;

	//the entity of the instance, and the name of the entity
	FCDEntity* entity;
	std::string name;

	//instance flag to check which the instance id found in the library
	bool flag_found = false;

	//look through the instance
	for (int i=0; i<(int)node_origin->GetInstanceCount(); i++) {

		//set or reset the parameter
		flag_found=false;
		instance=node_origin->GetInstance(i);
		entity=instance->GetEntity();
		name=entity->GetDaeId().c_str();

		// look for this name in geo library
		for (int j=0; j<(int)m_ptrs_geometries.size(); j++) {
			if (m_ptrs_geometries[j]->GetDaeId().c_str()==name) {
				flag_found = true;
			}
		}
	}
	return flag_found;
}