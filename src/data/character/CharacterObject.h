#pragma once

class Matrix;
class SimpleLine;
class MoveSelfObject;
class LineObject;
class VBOMesh;
class ChVBOMesh;
class VBOObject;
class ChSkeletonInstance;
class ChSkinInstance;
class ChSkeleton;
class ChAnimationManager;
class ChSkin;

// CharacterObject is a kind of VBOObject
// its type is OBJECT_TYPE_CHARACTER
// this kind of VBOObject is usually an object with a skeleton,
// some animations of the skeleton and skin.
// it changes from frame to frame according to its animation,
// so its instance must be update every single frame.
// it is always represented by several VBOMeshes
//
// steps to initialize CharacterObject(you should follow the steps exactly)
// 1. getSkeleton(); use the return pointer to initialize the skeleton
// 2. getAnimations(); use the return pointer to initialize the animations
// 3. getSkin(); use the return pointer to initialize the skin
// 4. initInstance();
// 5. initVBOs();
// 6. for each VBO{ initVBO(); setVBOTexture(); setVBOMaterail();(the last two
//                  function is optional)}
class CharacterObject : public VBOObject, public LineObject, MoveSelfObject
{
public:
	// constructor
	CharacterObject(void);
	// destructor
	~CharacterObject(void);

	// return a list of VBOMeshes that this VBOObject represent
	// this function return a pointer to the first VBOMesh of the 
	// list and num will be changed to the size of the list
	VBOMesh* representInVBOMesh(int* num);

	// return the type of this object
	int getType();

	// return the id of this object
	int getID();

	// return the line array that this object represent
	// num will be changed into the length of the line array
	SimpleLine* representInLine(int* num);

	// this function change the position matrix of the 
	// class by multiplying change
	void moveSelf(Matrix change);

	// this function is the factory function of skeleton
	// user should use the return pointer to initialize the skeleton
	// if skeleton is never created before, a new ChSkeleton will be 
	// created and be returned.
	// else the existed skeleton will be returned
	ChSkeleton* getSkeleton();

	// this function is the factory function of animation
	// user should use the return pointer to initialize the animations
	// if animations is never created before and skeleton has been 
	// created, then a new ChAnimationManager will be created in which
	// every animation's track number is the same as bone number in
	// skeleton
	// else the existed animations will be returned 
	ChAnimationManager* getAnimations();

	// this function is the factory function of skin
	// user should use the return pointer to initialize the skin
	// if skin is never created before, a new ChSkin will be created and
	// be returned,
	// else the existed skin will be returned
	ChSkin* getSkin();

	// initialize the chSkeletonInstance and chSkinInstance 
	// this function should be called after initial of skeleton and skin
	void initInstance();

	// initial function of vbomeshes
	// if vbomeshed is never created before, a new array of VBOMesh will
	// be created whose length is the given num
	// else nothing will happen
	// num should be bigger than 0.
	void initVBOs(const int num);

	// initial function of the vmsNum-th VBOMesh of vbomeshes
	// function initVBOs should have been called before calling this function,
	// if not, error will be reported
	// vmsNum should not be bigger or equal to the num parameter used in initVBOs
	// vSize is the length of vertices, it can be divided exactly by 3
	// vertices is an array of ids of ChVertex that is contained in this VBO, duplicated
	// ChVertex id is allowed.
	void initVBO(const int* vertices, const int vSize, const int vmsNum);

	// set the texture of the vmsNum-th VBOMesh of vbomeshes
	// the vmsNum-th VBOMesh should have been initialized by calling initVBO before calling
	// this function.
	// texCoord is the texture coordinates, tcSize is its size, tcSize should be 2/3 of vSize
	// used in function initVBO which initialize vmsNum-th VBOMesh of vbomeshes.
	// texId is the id of the texture to use on this VBOMesh
	void setVBOTexture(const float* texCoord, const int tcSize, GLuint texId, const int vmsNum);

	// set the material of the vmsNum-th VBOMesh of vbomeshes
	void setVBOMaterial(const GLfloat am[4], const GLfloat di[4], const GLfloat sp[4],
		const GLfloat em[4], GLfloat sh, const int vmsNum);

	// set the chMatrixInstance, the default matrix is an identity 4*4 matrix
	void setMatrix(float* matrix);

	// set the current gesture of the character
	// animation is the name of the animation to use
	// time_ms is the play frame number of the animation
	void setGesture(const char* animation, int time);

	// set the current gesture of the character
	// animation1 and 2 are the names of the animation to use
	// time1,time2 are the play frame number of the animation1 and 2
	// power is the power of animation 1, animation 2 's power will be 1-power
	void setGesture(const char* animaiton1, const char* animation2, int time1, int time2, float power1);

	// change v according to change
	// actually just v add change is the result, of course
	// v should not exceed 0 to 3
	void changeV(float change);

	// get function of v
	float getV();

	// rotate this character, direction will be changed
	// as the matrix instance will be rotate the same way
	// angle is the angle to rotate, it is in degree
	// positive angle rotate anticlockwise, negative angle
	// rotate clockwise
	void rotateCharacter(float angle);

	// get function of direction
	float getDirection();

private:
	/* display */
	// a pointer to an array that stores all the VBOMeshes of this 
	// object represents.
	// they should be updated every frame according to the matrixInstance,
	// chBoneInstance, chSkinInstance
	VBOMesh* vbomeshes;
	// ChVBOMesh
	ChVBOMesh** chvbomeshes;
	// the length of the array vbomeshes
	int meshSize;

	/* instances */
	// matrix instance that may be changed every frame
	Matrix chMatrixInstance;
	// bone instance that may be changed every frame
	ChSkeletonInstance* chSkeletonInstance;
	// skin instance that may be changed every frame
	ChSkinInstance* chSkinInstance;
	/* instance physics */
	// v is a float between 0 and 2
	// 0: idle; 0-1: walk; 1-2: walk and run; 2: run;
	float v;
	// direction is an angle in degree between 0 and 360
	// direction is the angle from +x axis to the direction
	// the character is facing
	float direction;

	/* data */
	// skeleton of CharacterObject which describes the bone structure
	ChSkeleton* skeleton;
	// skin of CharacterObject which describes the relationship between
	// bones and each part of the skin
	ChSkin* skin;
	// animations of CharacterObject that contains all the animations
	ChAnimationManager* animations;
};
