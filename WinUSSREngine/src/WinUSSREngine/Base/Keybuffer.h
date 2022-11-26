#pragma once

#include <mutex>
#include <bitset>
#include <queue>
#include <string>
#include <vector>

namespace WinUSSREngine
{
	class Keybuffer
	{
	public:
		Keybuffer() = default;
		Keybuffer(Keybuffer&&) = delete;
		Keybuffer(const Keybuffer&) = delete;
		Keybuffer& operator=(const Keybuffer&) = delete;

		std::string toString() noexcept;

		void onKeyPressed(unsigned char keycode) noexcept;
		void onKeyReleased(unsigned char keycode) noexcept;

		void clear() noexcept;
		void flush() noexcept;
		bool empty() const noexcept;
	private:
		bool isAlphabetic(unsigned char keycode) const noexcept;
		bool isSpecialKey(unsigned char keycode) noexcept;

		bool isShift() const noexcept;
		bool isCapslock() const noexcept;
		unsigned char toLower(unsigned char keycode) noexcept;
		unsigned char toUpper(unsigned char keycode) noexcept;
		template<typename T>
		static void trimBuffer(std::queue<T> buffer);
	private:
		std::mutex m_bufferMutex;
		static constexpr const unsigned char m_maxbuffSize = 16u;
		std::bitset<256u> m_keystates;
		std::queue<unsigned char> m_keybuffer;
		std::vector<unsigned char> m_keycodes;
		std::string m_toString;
	};
}