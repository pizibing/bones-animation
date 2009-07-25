#include <vector>

#include "Calculator.h"
#include "../matrixlib/matrix.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../data/LineObject.h"
#include "../data/MoveSelfObject.h"
#include "../data/character/CharacterObject.h"
#include "../data/terrain/TerrainObject.h"
#include "../managers/ObjectManager.h"
#include "../managers/CameraManager.h"

// the speed of the character to turn around in degree
const float TURN_AROUND_SPEED = 6;
// the amount the character's speed slow down when there's no move
const float SLOW_DOWN_NO_MOVE = 0.05;
// the amount the character's speed slow down when changing direction
const float SLOW_DOWN_CHANGE_DIRECTION = 0.01;
// the amount the character's speed go up every frame
const float SPEED_UP = 0.01;

// define the animation name of character's idle, walk and run
const char* CHARACTER_IDLE = "walk_niki";
const char* CHARACTER_WALK = "walk_niki";
const char* CHARACTER_RUN = "walk_niki";

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
//
// camera will follow the character
void Calculator::moveCharacter(float angle){
	// if there's no character, return
	std::vector<VBOObject*>* characters = 
			objectManager->getVBOObjects(OBJECT_TYPE_CHARACTER);
	if(characters->size() == 0) return;

	// frame number recorders
	static int frameControlIdle = 0;
	static int frameControlWalk = 0;
	static int frameControlRun = 0;

	// get the character
	CharacterObject* character = (CharacterObject*)(*characters)[0];
	// get direction of the character
	float direction = character->getDirection();

	// set character's position to make her stand on the terrain
	std::vector<VBOObject*>* terrains = 
		objectManager->getVBOObjects(OBJECT_TYPE_TERRAIN);
	if(terrains->size() != 0){
		// get terrain
		TerrainObject* terrain = (TerrainObject*)(*terrains)[0];
		// get terrain's height where character stand
		float height = terrain->getHeightAt(character->getPosition()[0],
			                                character->getPosition()[1]);
		// set character height in +z position
		character->setHeight(height);
	}

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
		// refresh walk frame control
		frameControlWalk = 0;

		character->setGesture(CHARACTER_IDLE,frameControlIdle);
		// add frame control
		frameControlIdle++;
	}
	// idle to walk
	else if(v < 0.5){
		// calculate power of idle
		float idlePower = (0.5 - v) * 2;
		character->setGesture(CHARACTER_IDLE,CHARACTER_WALK,
			frameControlIdle,frameControlWalk,idlePower);
		// add frame control
		frameControlIdle++;
		frameControlWalk++;
	}
	// walk
	else if(v < 1){
		// refresh idle and run frame control
		frameControlIdle = 0;
		frameControlRun = 0;

		character->setGesture(CHARACTER_WALK,frameControlWalk);
		// add frame control
		frameControlWalk++;
	}
	// walk and run
	else if(v < 2){
		// calculate power of run
		float runPower = v - 1;
		character->setGesture(CHARACTER_RUN,CHARACTER_WALK,
			frameControlRun,frameControlWalk,runPower);
		// add frame control
		frameControlRun++;
		frameControlWalk++;
	}
	// run
	else if(v <= 3){
		// refresh walk frame control
		frameControlWalk = 0;

		character->setGesture(CHARACTER_RUN,frameControlRun);
		// add frame control
		frameControlRun++;
	}
	
	// move camera
	CameraManager* camera = CameraManager::getInstance();
	camera->setTarget(character->getPosition());
}
