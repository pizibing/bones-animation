#pragma once

// SimpleLine class is a class represent line
// in 3D
// it contains two dots in 3D which have no sequence
class SimpleLine
{
public:
	// constructor
	SimpleLine(void);
	// destructor
	~SimpleLine(void);

	// set function of dot1
	void setDot1(const float* dot1);

	// set function of dot2
	void setDot2(const float* dot2);

	// get function of dot1
	const float* getDot1();

	// get function of dot2
	const float* getDot2();

private:
	// two dots
	float dot1[3];
	float dot2[3];
};
