#ifndef __EVENT_H__
#define __EVENT_H__

struct Event {
	enum Type {
		UNKNOWN,

		KEYBOARD,

		MOUSE_MOVE,
		MOUSE_BUTTON,
		MOUSE_WHEEL
	};

	int type;
	union {
		int x;
		int keyCode;
		int button;
	};
	union {
		int y;
		int state;
	};

	Event(int ntype = UNKNOWN, int nx = 0, int ny = 0)
		:type(ntype), x(nx), y(ny) {
	}
};

#endif //__EVENT_H__
