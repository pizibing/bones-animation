#pragma once

// SimpleLine class is a class represent line
// in 3D
// it contains two dots in 3D which have no sequence
class SimpleLine
{
public:
	// constructor
	SimpleLine(void){
		for(int i=0;i<3;i++){
			dot1[i] = dot2[i]= 0;
		}
	}
	// destructor
	~SimpleLine(void){}

	// set function of dot1
	void setDot1(const float* dot1);

	// set function of dot1
	inline void setDot1(float x,float y,float z){
		dot1[0] = x;
		dot1[1] = y;
		dot1[2] = z;
	}

	// set function of dot2
	void setDot2(const float* dot2);

	// set function of dot2
	inline void setDot2(float x,float y,float z){
		dot2[0] = x;
		dot2[1] = y;
		dot2[2] = z;
	}

	// get function of dot1
	inline const float* getDot1(){
		return dot1;
	}

	// get function of dot2
	inline const float* getDot2(){
		return dot2;
	}

private:
	// two dots
	float dot1[3];
	float dot2[3];
};
