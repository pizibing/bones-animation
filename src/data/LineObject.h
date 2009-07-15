#pragma once

class SimpleLine;

// LineObject is a kind of object that can be represented
// by a group of lines
// all LineObject should implement this interface
class LineObject
{
public:
	// return the line array that this object represent
	// num will be changed into the length of the line array
	virtual SimpleLine* representInLine(int* num) = 0;
};
