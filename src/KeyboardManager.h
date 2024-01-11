#pragma once
#ifdef _WINDOWS
#include "glad.h"
#include <GLFW/glfw3.h>

typedef int KeySym;
#else
#include <X11/keysym.h>
#include <X11/Xlib.h>
#endif

//keyboard manager from Cupak, Erik (231059)
//sometimes on x11 a button keeps getting pressed
struct KeyPacket
{
	KeyPacket(const KeySym boundKeycode)
		: boundKeycode_{boundKeycode}
	{
	}


	KeySym boundKeycode_{0};


	void Reset()
	{
		wasPressed = false;
		wasReleased = false;
	}


	void BigReset()
	{
		wasPressed = false;
		wasReleased = false;

		isPressed = false;
		isJustPressed = false;
		isJustReleased = false;
	}


	bool wasPressed{false};
	bool wasReleased{false};

	bool isPressed{false};

	bool isJustPressed{false};
	bool isJustReleased{false};
};


// These actions are tied to the keyPackets of corresponding index.
// keyPackets_[0] will be the first action, etc.
enum Action
{
	PitchUp,
	PitchDown,

	YawLeft,
	YawRight,

	MoveLeft,
	MoveRight,

	MoveForward,
	MoveBackward,

	Jump,
	Whip,
	Quit,

	Count,
	NoAction,
};


class KeyboardManager
{
public:
	KeyboardManager();

	void Update();

	void KeyPressed( KeySym keycode);

	void KeyReleased( KeySym keycode);

	const bool IsPressed(const Action action) const;

	const bool IsJustPressed(const Action action) const;

	const bool IsJustReleased(const Action action) const;

	void SetKeyBinding(const Action action, const int newKeycode);

	void ReleaseAll();

private:
	const int GetPacketIndex(const KeySym keycode) const;

	// Initial keybinding should match the action of the keyPacket.
	// keyPackets_[0] is jump and should have a corresponding keybinding.
#ifdef _WINDOWS
	KeyPacket keyPackets_[Action::Count]
	{
		// Pitch
	GLFW_KEY_W,
	GLFW_KEY_S,

	// Yaw
	GLFW_KEY_A,
	GLFW_KEY_D,

	// Move
	GLFW_KEY_LEFT,
	GLFW_KEY_RIGHT,
	GLFW_KEY_UP,
	GLFW_KEY_DOWN,

	// Action / Jump
	GLFW_KEY_SPACE,
	GLFW_KEY_E,

	// Exit
	GLFW_KEY_ESCAPE,
	};
#else
	
	KeyPacket keyPackets_[Action::Count]
	{
		// Pitch
		XK_w,
		XK_s,

		// Yaw
		XK_a,
		XK_d,

		// Move
		XK_Left,
		XK_Right,
		XK_Up,
		XK_Down,

		// Action / Jump
		XK_space,
		XK_e,
		// Exit
		XK_Escape,
	};
#endif

};
