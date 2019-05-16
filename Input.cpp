#include <unistd.h> // for close
#include <fcntl.h> // for open
#include <linux/input.h>

#include "Input.h"

const unsigned int KEY_ESCAPE = KEY_ESC;

Input::Input() {
	keyboardFD = -1;
	mouseFD = -1;

	mouseX = 0;
	mouseY = 0;
}

Input::~Input() {
	uninitKeyboard();
	uninitMouse();
}

bool Input::initKeyboard(const char* path) {
	uninitKeyboard();

	keyboardFD = open(path, O_RDONLY);
	return (keyboardFD != -1);
}

void Input::uninitKeyboard() {
	if (keyboardFD == -1) {
		return;
	}
	close(keyboardFD);
	keyboardFD = -1;
}

bool Input::initMouse(const char* path) {
	uninitMouse();
	mouseFD = open(path, O_RDONLY);
	return (mouseFD != -1);
}

void Input::uninitMouse() {
	if (mouseFD == -1) {
		return;
	}

	close(mouseFD);
	mouseFD = -1;
}

bool Input::getEvent(Event& event) {
	event.type = Event::UNKNOWN;

	input_event inputEvent;

	if (keyboardFD == -1) {
		return false;
	}

	fd_set set;
	FD_ZERO(&set);
	FD_SET(keyboardFD, &set);
	FD_SET(mouseFD, &set);

	const int maxFD = keyboardFD > mouseFD ? keyboardFD : mouseFD;
	if (maxFD < 0) {
		return false;
	}

	if (select(maxFD + 1, &set, NULL, NULL, NULL) > 0) {
		if (FD_ISSET(keyboardFD, &set)) {
			if (read(keyboardFD, &inputEvent, sizeof(inputEvent)) != sizeof(inputEvent)) {
				uninitKeyboard();
				return false;
			}
			if (inputEvent.code == 0) {
				return false;
			}
			event.type = Event::KEYBOARD;
			event.keyCode = inputEvent.code;
			event.value = (inputEvent.value > 0);
			return true;
		} else
		if (FD_ISSET(mouseFD, &set)) {
			if (read(mouseFD, &inputEvent, sizeof(inputEvent)) != sizeof(inputEvent)) {
				uninitMouse();
				return false;
			}
			switch (inputEvent.type) {
			case EV_KEY :
				event.type = Event::MOUSE_BUTTON;
				event.value = inputEvent.value;
				switch (inputEvent.code) {
				case BTN_LEFT :
					event.button = 1;
					return true;
				case BTN_MIDDLE :
					event.button = 2;
					return true;
				case BTN_RIGHT :
					event.button = 3;
					return true;
				}
				break;
			case EV_REL :
				switch (inputEvent.code) {
				case REL_WHEEL :
					event.type = Event::MOUSE_WHEEL;
					event.value = inputEvent.value;
					return true;
				case REL_X :
					mouseX += inputEvent.value;

					event.type = Event::MOUSE_MOVE;
					event.x = mouseX;
					event.y = mouseY;
					return true;
				case REL_Y :
					mouseY += inputEvent.value;

					event.type = Event::MOUSE_MOVE;
					event.x = mouseX;
					event.y = mouseY;
					return true;
				}
				break;
			}
		}
	}
	return false;
}
