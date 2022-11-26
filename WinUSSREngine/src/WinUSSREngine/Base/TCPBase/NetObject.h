#pragma once

#include "NetTypes.h"

namespace WinUSSREngine::TCP
{
	class NetObject
	{
	public:
		virtual void setiNetData(const NetiData& data) noexcept = 0;
		virtual void initNetwork() noexcept = 0;
		virtual void create(const NetiData& data = NetiData()) = 0;
		virtual bool isInitialized() const noexcept = 0;
		virtual bool peerDisconnected() noexcept = 0;
		virtual void closeNetwork() = 0;
		virtual void destroy() noexcept = 0;
	};
}