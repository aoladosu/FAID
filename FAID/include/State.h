#pragma once

// base state class
class State
{
public:

	State();

	// list of states
	const short int CURRENTSTATE = 0;

	// perform update actions and return the next state
	virtual int update();
	// perform any actions needed upon entering state
	virtual void enterState();
	// perform any actions needed upon exiting state
	virtual void exitState();

};

