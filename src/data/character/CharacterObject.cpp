#include <assert.h>
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

CharacterObject::CharacterObject(void){
	meshSize = 0;
	// init matrix to identity matrix
	for(int i = 1; i < 16; i++) chMatrixInstance[i] = 0;
	chMatrixInstance[0] = 1;
	chMatrixInstance[5] = 1;
	chMatrixInstance[10] = 1;
	chMatrixInstance[15] = 1;
	// init instance
	chSkeletonInstance = NULL;
	chSkinInstance = NULL;
	// init data
	skeleton = NULL;
	skin = NULL;
	animations = NULL;
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
		skin = new ChSkin();
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
	for(int i = 0; i < 16; i++)
		chMatrixInstance[i] = matrix[i];
}

// set the current gesture of the character
// animation is the name of the animation to use
// time_ms is the play time in millisecond of the animation
void CharacterObject::setGesture(const char* animation, int time_ms){
	// calculate skeleton instance
	chSkeletonInstance->calSkeletonInstance(animations,time_ms,animation);
	// calculate skin instance
	chSkinInstance->calSkinInstance(chSkeletonInstance,skin);
	// update chvbomeshes
	for(int i = 0; i < meshSize; i++){
		// update chvbomeshes according to vertex instance
		chvbomeshes[i]->updateVBO(chSkinInstance);
		// update chvbomeshes according to matrix instance
		chvbomeshes[i]->updateVBO(this->chMatrixInstance);
	}
	// update vbomeshes according to chvbomeshes
	for(int i = 0; i < meshSize; i++){
		vbomeshes[i] = *chvbomeshes[i]->getVBOMesh();
	}
}
