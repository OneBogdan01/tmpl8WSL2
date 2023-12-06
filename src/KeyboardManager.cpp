#include "KeyboardManager.h"


KeyboardManager::KeyboardManager()
{
}


// Update the state of the keypackets.
void KeyboardManager::Update()
{
	for (KeyPacket& packet : keyPackets_)
	{
		if (packet.wasPressed && !packet.wasReleased)
		{
			packet.isJustPressed = !packet.isPressed;
			packet.isPressed = true;
			packet.isJustReleased = false;
		}
		else if (packet.wasReleased && !packet.wasPressed)
		{
			packet.isJustReleased = packet.isPressed;
			packet.isPressed = false;
			packet.isJustPressed = false;
		}
		else
		{
			packet.isJustPressed = false;
			packet.isJustReleased = false;
		}

		packet.Reset();
	}
}


// Setting keystate obtained by Game using GLFW keycodes.
void KeyboardManager::KeyPressed(const KeySym keycode)
{
	int index{GetPacketIndex(keycode)};

	if (index != -1)
	{
		keyPackets_[GetPacketIndex(keycode)].wasPressed = true;
	}
}


void KeyboardManager::KeyReleased(const KeySym keycode)
{
	int index{GetPacketIndex(keycode)};

	if (index != -1)
	{
		keyPackets_[GetPacketIndex(keycode)].wasReleased = true;
	}
}


// Returning keystate to objects using Action enum.
const bool KeyboardManager::IsPressed(const Action action) const
{
	return keyPackets_[action].isPressed;
}


const bool KeyboardManager::IsJustPressed(const Action action) const
{
	return keyPackets_[action].isJustPressed;
}


const bool KeyboardManager::IsJustReleased(const Action action) const
{
	return keyPackets_[action].isJustReleased;
}


void KeyboardManager::SetKeyBinding(const Action, const int)
{
}


void KeyboardManager::ReleaseAll()
{
	for (int index{0}; index < Action::Count; ++index)
	{
		keyPackets_[index].BigReset();
	}
}


const int KeyboardManager::GetPacketIndex(const KeySym keycode) const
{
	for (int index{0}; index < Action::Count; ++index)
	{
		if (keycode == keyPackets_[index].boundKeycode_)
		{
			return index;
		}
	}

	return -1;
}
