#pragma once

#include <memory>

#define BIT(X) (1 << X)
#define BIND_EVENT_FN(x) [this](auto&&... args) -> decltype(auto) { return this->x(std::forward<decltype(args)>(args)...); }

namespace WinUSSREngine
{
	// This is me being lazy and keeping this in because I don't feel like writing whatever is below, every time.
	typedef unsigned char PAYLOAD; // This is used for payloads
	typedef unsigned char XorKey;  // This is used for Xor Keys
	typedef unsigned char unchar;  // This is because I'm just that lazy
	typedef unsigned short int int16;		   // This is because I'm just that lazy


	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
}

namespace WinUSSREngine::Utils
{
	template<typename T, typename F, typename... Args>
	static T genericFunc(F func, Args&&... args) {
		return static_cast<T>(func(args...));
	}
}