#include "Command.h"
#include "../view/ConcreteDisplay.h"
#include "../loaders/SimpleModelLoader.h"
#include "../managers/SimpleMeshManager.h"

Command::Command(void){
	display = new ConcreteDisplay();
	modelLoader = new SimpleModelLoader();
	calculator = new Calculator();
	meshManager = new SimpleMeshManager();
}

Command::~Command(void){
}

void Command::drawScene(){

	display->display(true,)
}
