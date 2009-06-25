#include "Command.h"
#include "ConcreteDisplay.h"
#include "SimpleModelLoader.h"
#include "SimpleMeshManager.h"

Command::Command(void){
	display = new ConcreteDisplay();
	modelLoader = new SimpleModelLoader();
	caculator = new Caculator();
	meshManager = new SimpleMeshManager();
}

Command::~Command(void){
}

void Command::drawScene(){

	display->display(true,)
}
