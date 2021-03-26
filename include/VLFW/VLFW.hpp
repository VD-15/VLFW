#ifndef VLFW_VLFW_HPP
#define VLFW_VLFW_HPP

#include "ValkyrieEngine/ValkyrieEngine.hpp"

#include "Keyboard.hpp"
#include "Monitor.hpp"
#include "Mouse.hpp"
#include "Window.hpp"

#include "ValkyrieEngine/EventBus.hpp"

#include <mutex>

namespace vlk
{
	namespace vlfw
	{
		struct ErrorEvent
		{
			Int errorCode;
			std::string what;
		};

		enum class WaitMode
		{
			//! Wait for an event to be posted before the window updates
			Wait =           0x00000000,

			//! Update the window anyway, processing any events that have been posted
			Poll =           0x00000001
		};

		struct VLFWMainArgs
		{
			WaitMode waitMode = WaitMode::Poll;
			Double waitTimeout = 0.0;
			Int swapInterval = 0;
		};

		class VLFWMain : 
			public EventListener<vlk::PostUpdateEvent>
		{
			std::unique_lock<std::mutex> lock;
			Double waitTimeout;
			WaitMode waitMode;

			public:
			inline VLFWMain() : VLFWMain(VLFWMainArgs{}) {}
			VLFWMain(const VLFWMainArgs& args);
			~VLFWMain();

			VLFWMain(const VLFWMain&) = delete;
			VLFWMain(VLFWMain&&) = delete;
			VLFWMain& operator=(const VLFWMain&) = delete;
			VLFWMain& operator=(VLFWMain&&) = delete;

			void OnEvent(const vlk::PostUpdateEvent& ev);
			void SendEmptyEvent();
			void SetSwapInterval(Int interval);
		};
	}
}

#endif
