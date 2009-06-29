#pragma once
#include "Display.h"

class ConcreteDisplay : public Display
{
public:
	ConcreteDisplay(void);
	~ConcreteDisplay(void);

	void display(bool isWire,const std::vector<Mesh*>* toDisplay);

private:
	void displayWire(const std::vector<Mesh*>* toDisplay);
	void displaySolid(const std::vector<Mesh*>* toDisplay);
};
