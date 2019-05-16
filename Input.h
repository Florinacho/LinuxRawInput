#ifndef __INPUT_H__
#define __INPUT_H__

#include "Event.h"

extern const unsigned int KEY_ESCAPE;

struct Input {
	int keyboardFD;
	int mouseFD;

	int mouseX;
	int mouseY;

	Input();

	~Input();

	bool initKeyboard(const char* path);

	void uninitKeyboard();

	bool initMouse(const char* path);

	void uninitMouse();

	bool getEvent(Event& event);
};

#endif //__INPUT_H__
