#pragma once

class Matrix;

// classes implement MoveSelfObject Interface can move its
// position in world space by itself
class MoveSelfObject{

protected:
	// this function change the position matrix of the 
	// class by multiplying change
	virtual void moveSelf(Matrix change) = 0;
};