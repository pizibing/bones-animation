#include "FColladaModelLoader.h"
#include "../data/Mesh.h"
#include "../managers/MeshManager.h"
#include "../managers/SimpleMeshManager.h"

FColladaModelLoader::FColladaModelLoader(void){
}

FColladaModelLoader::~FColladaModelLoader(void){
}

bool FColladaModelLoader::loadModel(int kind, char* path){
	meshManager = SimpleMeshManager::getInstance();

	Mesh* mesh = new Mesh();

	// new dae file
	m_document = FCollada::NewTopDocument();

	// open dae file
	bool ret=FCollada::LoadDocumentFromFile(m_document,FUStringConversion::ToFString("filename") );	

	if (ret==false || m_document==NULL) {
		m_is_file_opened=false;
		return false;
	} else {
		// here, file was opened sucusefully
		m_is_file_opened=true;
	}

	meshManager->addMesh(0,mesh);

	return true;
}
