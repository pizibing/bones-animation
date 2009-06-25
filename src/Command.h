#pragma once

class Command
{
public:
	/*
	 * para: none
	 * function: accomplish this command's task
	 * return: void
	 */
	virtual void doTask() = 0;
};