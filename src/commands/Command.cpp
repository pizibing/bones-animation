#include "Command.h"
#include "../view/ConcreteDisplay.h"
#include "../loaders/SimpleModelLoader.h"
#include "../managers/SimpleMeshManager.h"

Command::Command(void){
	display = new ConcreteDisplay();
	modelLoader = new SimpleModelLoader();
	calculator = new Calculator();
	meshManager = SimpleMeshManager::getInstance();
	cameraManager = new CameraManager();
	lightManager = new LightManager();
}

Command::~Command(void){
	delete display;
	delete modelLoader;
	delete calculator;
	delete cameraManager;
	delete lightManager;
}

void Command::drawScene(){
	cameraManager->look();
	display->display(true,meshManager->getMeshes(MESH_KIND_CHARACTER));
}

bool Command::loadModel(){
	return modelLoader->loadModel(0,"");
}
