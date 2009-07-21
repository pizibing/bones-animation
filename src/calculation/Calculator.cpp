#include <vector>

#include "Calculator.h"
#include "../matrixlib/matrix.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../data/LineObject.h"
#include "../data/MoveSelfObject.h"
#include "../data/character/CharacterObject.h"
#include "../managers/ObjectManager.h"

// the speed of the character to turn around in degree
const float TURN_AROUND_SPEED = 6;
// the amount the character's speed slow down when there's no move
const float SLOW_DOWN_NO_MOVE = 0.05;
// the amount the character's speed slow down when changing direction
const float SLOW_DOWN_CHANGE_DIRECTION = 0.01;
// the amount the character's speed go up every frame
const float SPEED_UP = 0.01;

// define the animation name of character's idle, walk and run
const char* CHARACTER_IDLE = "";
const char* CHARACTER_WALK = "";
const char* CHARACTER_RUN = "";

//constructor
Calculator::Calculator(void){
	// initialize objectManager
	objectManager = ObjectManager::getInstance();
}

//destructor
Calculator::~Calculator(void)
{
}

// make the character stored in ObjectManager to move 
// forward in the given direction
// angle is an angle in degree between -1 and 360
// angle is the angle from +x axis to the direction
// if angle is -1, then character remain its direction
// but the speed will slow down
// if angle is the same as default character will speed up
// if angle is different from the character's direction
// the speed will slow down a little too.
void Calculator::moveCharacter(float angle){
	// if there's no character, return
	std::vector<VBOObject*>* characters = 
			objectManager->getVBOObjects(OBJECT_TYPE_CHARACTER);
	if(characters->size() == 0) return;

	// every time the v of character is decreased to 0, frameControl
	// is set to 0
	// else frameControl increase by 1 every call to this function
	static int frameControl = 0;

	// get the character
	CharacterObject* character = 
		(CharacterObject*)objectManager->getVBOObject(OBJECT_TYPE_CHARACTER,0);
	// get direction of the character
	float direction = character->getDirection();

	/* change speed and direction of the character */
	// if angle is -1 , slow down a lot
	if(angle == -1){
		character->changeV(-SLOW_DOWN_NO_MOVE);
	}
	// if angle equals to direction, speed up
	else if(angle == direction){
		character->changeV(SPEED_UP);
	}
	// if angle doesn't equal to direction ,slow down a little
	// and change direction
	else{
		// change speed
		character->changeV(-SLOW_DOWN_CHANGE_DIRECTION);

		/* change direction */
		/* choose the shorter way to change direction */
		// calculate anticlockwise and clockwise change distance
		float anticw = 0;
		float cw = 0;
		// if angle is bigger than direction
		if(angle > direction){
			anticw = angle - direction;
			cw = 360 - (angle - direction);
		}
		// if angle is smaller than direction
		else{
			cw = direction - angle;
			anticw = 360 - (direction - angle);
		}
		// choose one direction 
		bool turnClockwise = (cw < anticw);
		/* change direction */
		// turn clockwise
		if(turnClockwise){
			if(cw <= TURN_AROUND_SPEED)
				character->rotateCharacter(-cw);
			else
				character->rotateCharacter(-TURN_AROUND_SPEED);
		}
		// turn anticlockwise
		else{
			if(anticw <= TURN_AROUND_SPEED)
				character->rotateCharacter(anticw);
			else
				character->rotateCharacter(TURN_AROUND_SPEED);
		}
	}

	// choose the animation to use
	float v = character->getV();
	// idle
	if(v == 0){
		frameControl = 0;
		character->setGesture(CHARACTER_IDLE,frameControl);
	}
	// walk
	else if(v < 1){
		character->setGesture(CHARACTER_WALK,frameControl);
	}
	// walk and run
	else if(v < 2){
		// calculate power of run
		float runPower = v - 1;
		character->setGesture(CHARACTER_RUN,CHARACTER_WALK,
			frameControl,frameControl,runPower);
	}
	// run
	else if(v <= 3){
		character->setGesture(CHARACTER_RUN,frameControl);
	}
	
	// add frame control
	frameControl++;
}
