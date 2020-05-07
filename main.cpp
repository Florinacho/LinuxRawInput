#include <stdio.h>
#include "Input.h"
#include <linux/input.h>

//cat /proc/bus/input/devices
static const char* InputFileNames[] = {
	"/dev/input/event2",	// Keyboard
	"/dev/input/event4",	// Mouse
	"/dev/input/event5"	// Mouse
};

int main(int argc, char** argv) {
	Input input;
	Event event;
	bool run = true;

	for (unsigned int index = 0; index < 3; ++index) {
		if (input.addInput(InputFileNames[index]) == false) {
			printf("Warning! Cannot open \"%s\".\n", InputFileNames[index]);
		}
	}

	while(run) {
		while(input.getEvent(event)) {
			switch (event.type) {
			case Event::KEYBOARD :
				printf("Key %d | 0x%X is %s\n", event.keyCode, event.keyCode, event.state ? "pressed" : "released");
				switch (event.keyCode) {
				case KEY_ESC :
					run = false;
					break;
				}
				break;
			case Event::MOUSE_MOVE :
				printf("Mouse position: %d, %d\n", event.x, event.y);
				break;
			case Event::MOUSE_BUTTON :
				printf("Mouse button %d is %s\n", event.button, event.state ? "pressed" : "released");
				break;
			case Event::MOUSE_WHEEL :
				printf("Mouse wheel %s\n", event.state == 1 ? "up" : "down");
				break;
			}
		}
	}

	return 0;
}
