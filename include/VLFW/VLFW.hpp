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
		//! Sent when GLFW generates an error
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

		//! Arguments for VLFWMain
		struct VLFWMainArgs
		{
			//! How to wait for incoming events
			WaitMode waitMode = WaitMode::Poll;

			//! Max timeout when waiting for events
			Double waitTimeout = 0.0;

			//! How many monitor refreshes should we wait to swap window buffers?
			Int swapInterval = 0;
		};

		/*!
		 * \brief Handles window-related events
		 *
		 * VLFWMain handles the initialization of GLFW and event processing
		 * for any constructed window components. Only one instance of
		 * VLFWMain may be constructed at once and various parts of VLFW will
		 * not work without it, so you should construct one before doing
		 * anything else.
		 */
		class VLFWMain : 
			public EventListener<vlk::PreUpdateEvent>
		{
			std::unique_lock<std::mutex> lock;

			public:
			Double waitTimeout;
			WaitMode waitMode;

			inline VLFWMain() : VLFWMain(VLFWMainArgs{}) {}
			VLFWMain(const VLFWMainArgs& args);
			~VLFWMain();

			VLFWMain(const VLFWMain&) = delete;
			VLFWMain(VLFWMain&&) = delete;
			VLFWMain& operator=(const VLFWMain&) = delete;
			VLFWMain& operator=(VLFWMain&&) = delete;

			void OnEvent(const vlk::PreUpdateEvent& ev) override;

			void SendEmptyEvent();
			void SetSwapInterval(Int interval);
		};
	}
}

#endif
