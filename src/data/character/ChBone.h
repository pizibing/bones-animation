#pragma once

// bone for a character,such as head,hand,foot etc.
// has a parent bone pointer and a array for child bone pointers
// has a string name as id
// must set child number when create a new bone

class ChBone
{
public:
	ChBone(void);
	~ChBone(void);
};
