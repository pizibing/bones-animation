#include "Calculator.h"
#include "../data/VBOMesh.h"
#include "../managers/ObjectManager.h"

//constructor
Calculator::Calculator(void){
	// initialize objectManager
	objectManager = ObjectManager::getInstance();
}

//destructor
Calculator::~Calculator(void)
{
}
