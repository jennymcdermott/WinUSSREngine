#pragma once

namespace WinUSSREngine
{
	class Key
	{
	public:
		enum class KeyState : unsigned int
		{
			Invalid,
			Pressed,
			Released
		};
	private:
		KeyState m_type;
		unsigned char m_keycode;
	public:
		Key() noexcept
			: m_type(KeyState::Invalid), m_keycode(0u) { }

		Key(KeyState type, unsigned char keycode)
			noexcept : m_type(type), m_keycode(keycode) { }

		bool isInvalid() const noexcept { return m_type == KeyState::Invalid; }
		bool isPressed() const noexcept { return m_type == KeyState::Pressed; }
		bool isReleased() const noexcept { return m_type == KeyState::Released; }
		char keycode() noexcept { return m_keycode; }
	};
}