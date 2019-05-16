#include <stdio.h>
#include "Input.h"

static const char* KeyboardFileName = "/dev/input/event2";
static const char* MouseFileName = "/dev/input/event4";

int main(int argc, char** argv) {
	Input input;
	if (input.initKeyboard(KeyboardFileName) == false) {
		printf("Warning! Cannot open keyboard file \"%s\". No keyboard events will pe posted\n", KeyboardFileName);
		return 1;
	}
	if (input.initMouse(MouseFileName) == false) {
		printf("Wawrning! Cannot open mouse file \"%s\". No mouse events will pe posted\n", MouseFileName);
		return 1;
	}

	Event event;
	bool run = true;
	while(run) {
		if (input.getEvent(event) == false) {
			continue;
		}

		switch (event.type) {
		case Event::KEYBOARD :
			printf("Key %d is %s.\n", event.keyCode, event.value ? "pressed" : "released");
			if (event.keyCode == KEY_ESCAPE && event.value == 0) { // Key escape is released
				run = false;
			}
			break;
		case Event::MOUSE_MOVE :
			printf("Mouse position: %d, %d\n", event.x, event.y);
			break;
		case Event::MOUSE_BUTTON :
			printf("Mouse button %d is %s.\n", event.button, event.value ? "pressed" : "released");
			break;
		case Event::MOUSE_WHEEL :
			printf("Mouse wheel: %d\n", event.value);
			break;
		}
	}

	return 0;
}
