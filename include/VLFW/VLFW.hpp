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

			//! Send a VLFWMain::RenderWaitEvent to wait for the renderer to complete before swapping buffers?
			bool waitForRenderer = true;
		};

		/*!
		 * \brief Pointer to a Vulkan API function
		 */
		typedef void(* VulkanProcess)();

		/*!
		 * \brief Handles window-related events
		 *
		 * VLFWMain handles the initialization of GLFW and event processing
		 * for any constructed window components. Only one instance of
		 * VLFWMain may be constructed at once and various parts of VLFW will
		 * not work without it, so you should construct one before doing
		 * anything else.
		 */
		class VLFWMain final : 
			public EventListener<vlk::PreUpdateEvent>,
			public EventListener<vlk::PostUpdateEvent>
		{
			std::unique_lock<std::mutex> lock;

			public:
	
			/*!
			 * \brief Sent when VLFWMain is about to swap the buffers of it's windows.
			 *
			 * Listeners of this event should block the calling thread until
			 * any pending rendering is completed and the default framebuffer
			 * is ready to swap.
			 *
			 * \sa VLFWMainArgs::waitForRenderer
			 */
			struct RenderWaitEvent
			{ };

			bool waitForRenderer;
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
			void OnEvent(const vlk::PostUpdateEvent& ev) override;

			/*!
			 * \brief Posts an empty event to the window
			 *
			 * This may be used to trigger an update manually if using
			 * <tt>WaitMode::Wait</tt>
			 */
			void SendEmptyEvent();

			/*!
			 * \brief Sets the swap interval to use when swapping buffers of
			 * the current OpenGL context.
			 *
			 * \sa VLFWMainArgs::swapInterval
			 */
			void SetSwapInterval(Int interval);

			/*!
			 * \brief Returns true if vulkan is at least minimally supported
			 *
			 * \glfw
			 * This function returns whether the Vulkan loader and any
			 * minimally functional ICD have been found. The availability of a
			 * Vulkan loader and even an ICD does not by itself guarantee that
			 * surface creation or even instance creation is possible. For
			 * example, on Fermi systems Nvidia will install an ICD that
			 * provides no actual Vulkan support.
			 *
			 * \sa GetRequiredInstanceExtensions()
			 */
			bool IsVulkanSupported() const;

			/*!
			 * \brief Gets the contents of the system clipboard in the form of
			 * a UTF-8 encoded string
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			std::string GetClipboard() const;

			/*!
			 * \brief Writes to the system clipboard
			 *
			 * \param data A UTF-8 encoded string to write to che clipboard
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SetClipboard(const std::string& data);
		};
	}
}

#endif
