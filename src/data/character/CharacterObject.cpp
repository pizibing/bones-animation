#include <assert.h>
#include "../matrixlib/Vector3D.h"
#include "../matrixlib/quaternion.h"
#include "../matrixlib/matrix.h"
#include "../data/MoveSelfObject.h"
#include "../data/LineObject.h"
#include "../data/VBOMesh.h"
#include "../data/VBOObject.h"
#include "../data/character/ChVBOMesh.h"
#include "CharacterObject.h"
#include "../data/character/ChSkeleton.h"
#include "../data/character/ChSkin.h"
#include "../data/character/ChAnimationManager.h"
#include "../data/character/ChSkeletonInstance.h"
#include "../data/character/ChSkinInstance.h"
#include "../managers/ObjectManager.h"

CharacterObject::CharacterObject(void){
	// type is OBJECT_TYPE_STATIC
	type = OBJECT_TYPE_CHARACTER;

	// generate id
	ObjectManager* om = ObjectManager::getInstance();
	id = om->generateID();

	meshSize = 0;
	// init matrix to identity matrix
	chMatrixInstance = Matrix();
	// init instance
	chSkeletonInstance = NULL;
	chSkinInstance = NULL;
	// init instance physics
	v = 0;
	direction = 0;
	// init data
	skeleton = NULL;
	skin = NULL;
	animations = NULL;
	// init vbomesh
	vbomeshes = 0;
}

CharacterObject::~CharacterObject(void){
	// delete vbomeshes
	for(int i = 0; i < meshSize; i++){
		delete chvbomeshes[i];
	}
	if(vbomeshes) delete[] vbomeshes;
	if(chvbomeshes) delete[] chvbomeshes;
	// delete instance
	if(chSkeletonInstance) delete chSkeletonInstance;
	if(chSkinInstance) delete chSkinInstance;
	// delete data
	if(skeleton) delete skeleton;
	if(skin) delete skin;
	if(animations) delete animations;
}

// return a list of VBOMeshes that this VBOObject represent
// this function return a pointer to the first VBOMesh of the 
// list and num will be changed to the size of the list
VBOMesh* CharacterObject::representInVBOMesh(int* num){
	*num = meshSize;
	return vbomeshes;
}

//return the type of this object
int CharacterObject::getType(){
	return type;
}

//return the id of this object
int CharacterObject::getID(){
	return id;
}

// return the line array that this object represent
// num will be changed into the length of the line array
SimpleLine* CharacterObject::representInLine(int* num){
	return chSkeletonInstance->getBoneLines(num,chMatrixInstance);
}

// this function change the position matrix of the 
// class by multiplying change
void CharacterObject::moveSelf(Matrix change){
	chMatrixInstance = change * chMatrixInstance;
}

// this function is the factory function of skeleton
// user should use the return pointer to initialize the skeleton
// if skeleton is never created before, a new ChSkeleton will be 
// created and be returned.
// else the existed skeleton will be returned
ChSkeleton* CharacterObject::getSkeleton(){
	// if skeleton is NULL
	if(!skeleton){
		skeleton = new ChSkeleton();
	}
	return skeleton;
}

// this function is the factory function of animation
// user should use the return pointer to initialize the animations
// if animations is never created before and skeleton has been 
// created, then a new ChAnimationManager will be created in which
// every animation's track number is the same as bone number in
// skeleton
// else the existed animations will be returned 
ChAnimationManager* CharacterObject::getAnimations(){
	// skeleton should be initialized before calling this function
	assert(skeleton);

	// if animations is NULL
	if(!animations){
		animations = new ChAnimationManager();
		// every animation's track number is the same as bone number in
		// skeleton, so animations should know the skeleton
		animations->setSkeleton(skeleton);
	}
	return animations;
}

// this function is the factory function of skin
// user should use the return pointer to initialize the skin
// if skin is never created before, a new ChSkin will be created and
// be returned,
// else the existed skin will be returned
ChSkin* CharacterObject::getSkin(){
	// if skin is NULL
	if(!skin){
		skin = new ChSkin(skeleton);
	}
	return skin;
}

// initialize the chSkeletonInstance and chSkinInstance 
// this function should be called after initial of skeleton and skin
void CharacterObject::initInstance(){
	// this function should be called after initial of skeleton and skin
	assert(skeleton);
	assert(skin);

	// init skeleton and skin instance
	chSkeletonInstance = new ChSkeletonInstance(skeleton);
	chSkinInstance = new ChSkinInstance(skin);
}

// initial function of vbomeshes
// if vbomeshes is never created before, a new array of VBOMesh will
// be created whose length is the given num
// else nothing will happen
// num should be bigger than 0.
void CharacterObject::initVBOs(const int num){
	// num should be bigger than 0
	assert(num > 0);

	// if vbomeshes has been initialized
	if(vbomeshes) return;
	else{
		vbomeshes = new VBOMesh[num];
		chvbomeshes = new ChVBOMesh*[num];
		meshSize = num;
	}
}

// initial function of the vmsNum-th VBOMesh of vbomeshes
// function initVBOs should have been called before calling this function,
// if not, error will be reported
// vmsNum should not be bigger or equal to the num parameter used in initVBOs
// vSize is the length of vertices, it can be divided exactly by 3
// vertices is an array of ids of ChVertex that is contained in this VBO, duplicated
// ChVertex id is allowed.
void CharacterObject::initVBO(const int* vertices, const int vSize, const int vmsNum){
	// vbomeshes should have been initialized
	assert(vbomeshes);
	// vmsNum should be smaller than meshSize
	assert(vmsNum < meshSize);
	// vSize can be divided exactly by 3
	int remain = vSize % 3;
	assert(remain == 0);

	// initialize chvbomeshes and vbomeshes
	chvbomeshes[vmsNum] = new ChVBOMesh(vertices,vSize,skin);
	vbomeshes[vmsNum] = *chvbomeshes[vmsNum]->getVBOMesh();
}

// set the texture of the vmsNum-th VBOMesh of vbomeshes
// the vmsNum-th VBOMesh should have been initialized by calling initVBO before calling
// this function.
// texCoord is the texture coordinates, tcSize is its size, tcSize should be 2 times of vSize
// used in function initVBO which initialize vmsNum-th VBOMesh of vbomeshes.
// texId is the id of the texture to use on this VBOMesh
void CharacterObject::setVBOTexture(const float* texCoord, const int tcSize, GLuint texId, const int vmsNum){
	// vbomeshes should have been initialized
	assert(chvbomeshes);
	assert(chvbomeshes[vmsNum]);

	chvbomeshes[vmsNum]->getVBOMesh()->setTextures(texCoord,tcSize,GL_STATIC_DRAW_ARB,texId);
}

// set the material of the vmsNum-th VBOMesh of vbomeshes
void CharacterObject::setVBOMaterial(const GLfloat am[4], const GLfloat di[4], const GLfloat sp[4],
									 const GLfloat em[4], GLfloat sh, const int vmsNum){
	// vbomeshes should have been initialized
	assert(chvbomeshes);
	assert(chvbomeshes[vmsNum]);

	chvbomeshes[vmsNum]->getVBOMesh()->setMaterial(am,di,sp,em,sh);
}

// set the chMatrixInstance, the default matrix is an identity 4*4 matrix
void CharacterObject::setMatrix(float* matrix){
	// set matrix
	for(int i = 0; i < 16; i++)
		chMatrixInstance[i] = matrix[i];
	// update chvbomeshes
	for(int i = 0; i < meshSize; i++){
		// update chvbomeshes according to matrix instance
		chvbomeshes[i]->updateVBO(chMatrixInstance.get());
	}
}

// set the current gesture of the character
// animation is the name of the animation to use
// time_ms is the play time's frame number of the animation
void CharacterObject::setGesture(const char* animation, int time){
	// calculate skeleton instance and get the move self matrix
	Matrix change = 
		chSkeletonInstance->calSkeletonInstance(animations,time,animation);
	// use return change matrix to change matrix instance
	this->moveSelf(change);
	// calculate skin instance
	chSkinInstance->calSkinInstance(chSkeletonInstance,skin);
	// update chvbomeshes
	for(int i = 0; i < meshSize; i++){
		// update chvbomeshes according to vertex instance
		// normals and position maybe updated
		chvbomeshes[i]->updateVBO(chSkinInstance);
		// update chvbomeshes according to matrix instance
		chvbomeshes[i]->updateVBO(chMatrixInstance.get());
	}
	// update vbomeshes according to chvbomeshes
	for(int i = 0; i < meshSize; i++){
		vbomeshes[i] = *chvbomeshes[i]->getVBOMesh();
	}
	//
	/*for(int i = 0; i < 16; i++)
		printf("%f ",change[i]);
	printf("\n");*/
}

// set the current gesture of the character
// animation1 and 2 are the names of the animation to use
// time1,time2 are the play frame number of the animation1 and 2
// power is the power of animation 1, animation 2 's power will be 1-power
void CharacterObject::setGesture(const char* animaiton1, const char* animation2, int time1, int time2, float power1){
	// calculate skeleton instance and get the move self matrix
	Matrix change = 
		chSkeletonInstance->calSkeletonInstance(animations,time1,time2,animaiton1,animation2,power1);
	// use return change matrix to change matrix instance
	this->moveSelf(change);
	// calculate skin instance
	chSkinInstance->calSkinInstance(chSkeletonInstance,skin);
	// update chvbomeshes
	for(int i = 0; i < meshSize; i++){
		// update chvbomeshes according to vertex instance
		// normals and position maybe updated
		chvbomeshes[i]->updateVBO(chSkinInstance);
		// update chvbomeshes according to matrix instance
		chvbomeshes[i]->updateVBO(chMatrixInstance.get());
	}
	// update vbomeshes according to chvbomeshes
	for(int i = 0; i < meshSize; i++){
		vbomeshes[i] = *chvbomeshes[i]->getVBOMesh();
	}
}

// change v according to change
// actually just v add change is the result, of course
// v should not exceed 0 to 3
void CharacterObject::changeV(float change){
	// change v
	v += change;

	// modified v to fit into 0 to 3
	if(v > 3) v = 3;
	if(v < 0) v = 0;
}

// get function of v
float CharacterObject::getV(){
	return v;
}

// rotate this character, direction will be changed
// as the matrix instance will be rotate the same way
// angle is the angle to rotate, it is in degree
// positive angle rotate anticlockwise, negative angle
// rotate clockwise
void CharacterObject::rotateCharacter(float angle){
	// change this-> angle according to the given angle
	direction += angle;
	// if angle is bigger than 0, just get the remain
	if(direction >= 0){
		// get int divide result 
		int r = (int)(direction / 360);
		// get the remain
		direction = direction - 360 * r;
	}
	// if angle is smaller than 0
	else{
		// get int divide result 
		int r = (int)(direction / 360);
		// get the remain
		direction = direction - 360 * r;
		// 360 minus angle
		direction = 360 + direction;
	}

	// rotate matrix instance according to angle
	chMatrixInstance.Rotate(0,0,1,angle);
}

// get function of direction
float CharacterObject::getDirection(){
	return direction;
}

// get the position of that character
float* CharacterObject::getPosition(){
	// matrix instance's 12-th, 13-th, 14-th is the position
	return chMatrixInstance.get() + 12;
}

// set the height position of the character
// i.e. the z axis translation of the character
void CharacterObject::setHeight(float height){
	// 14-th element of matrix instance is the z axis translation
	chMatrixInstance[14] = height; 
}