#include "../matrixlib/mtxlib.h"
#include "ChBoneInstance.h"


ChBoneInstance::ChBoneInstance(int id)
{
}

ChBoneInstance::~ChBoneInstance(void)
{
}

// set function of matrix
void ChBoneInstance::setMatrix(const matrix44 &matrix){
	this->matrix = matrix;
}

// get function of matrix
const matrix44 & ChBoneInstance::getMatrix(){
	return matrix;
}
