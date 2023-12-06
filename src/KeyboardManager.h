#pragma once

#include <X11/keysym.h>
#include <X11/Xlib.h>
//keyboard manager from Cupak, Erik (231059)

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
	Quit,

	Count,
	NoAction,
};


class KeyboardManager
{
public:
	KeyboardManager();

	void Update();

	void KeyPressed(const KeySym keycode);

	void KeyReleased(const KeySym keycode);

	const bool IsPressed(const Action action) const;

	const bool IsJustPressed(const Action action) const;

	const bool IsJustReleased(const Action action) const;

	void SetKeyBinding(const Action action, const int newKeycode);

	void ReleaseAll();

private:
	const int GetPacketIndex(const KeySym keycode) const;

	// Initial keybinding should match the action of the keyPacket.
	// keyPackets_[0] is jump and should have a corresponding keybinding.
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

		// Exit
		XK_Escape,
	};
};
