#pragma once

class matrix44;

// classes implement MoveSelfObject Interface can move its
// position in world space by itself
class MoveSelfObject{

protected:
	// this function change the position matrix of the 
	// class by multiplying change
	virtual void moveSelf(matrix44 change) = 0;
};