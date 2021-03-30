#ifndef VLFW_MONITOR_HPP
#define VLFW_MONITOR_HPP

#include "ValkyrieEngineCommon/Types.hpp"
#include <vector>
#include <string>

namespace vlk
{
	namespace vlfw
	{
		/*!
		 * \brief Monitor video mode
		 */
		struct VideoMode
		{
			//! Size of the video mode in screen coordinates
			Point<> size;

			//! Bit depth of the red channel
			Int redBits;

			//! Bit depth of the green channel
			Int greenBits;

			//! Bit depth of the blue channel
			Int blueBits;

			//! Refresh rate of the video mode, in Hz
			Int refreshRate;
		};

		/*!
		 * \brief Monitor wrapper class.
		 *
		 * Certain functions of this class require an instance of VLFWMain
		 * to be constructed prior in order to function properly.
		 */
		class Monitor
		{
			public:
			typedef void* MonitorHandle;

			//! Sent when a monitor is connected to the system
			struct ConnectEvent
			{
				//! Pointer to the monitor object.
				const Monitor* monitor;
			};

			/*!
			 *  Sent when a monitor is disconnected from the system
			 *
			 * If a window is fullscreen on this monitor when it is disconnected,
			 * it is automatically switched back to windowed mode before this
			 * event is sent
			 */
			struct DisconnectEvent
			{
				/*!
				 * \brief Pointer to the monitor object.
				 *
				 * This pointer is not considered safe to dereference when this event is sent.
				 */
				const Monitor* monitor;
			};

			private:
			MonitorHandle nativeHandle;
			std::vector<VideoMode> supportedModes;
			std::vector<Color> gammaRamp;

			public:

			Monitor(MonitorHandle handle);

			Monitor() = delete;
			Monitor(const Monitor&) = delete;
			Monitor(Monitor&&) = delete;
			Monitor& operator=(const Monitor&) = delete;
			Monitor& operator=(Monitor&&) = delete;
			~Monitor() = default;

			/*!
			 * \brief Returns a handle to the underlying monitor object
			 *
			 * This handle is owned by GLFW and functions as a valid
			 * <tt>GLFWmonitor*</tt> object should you want to use the library
			 * directly.
			 */
			inline void* GetHandle() { return nativeHandle; }

			/*!
			 * \brief Returns the user's preferred monitor.
			 *
			 * The returned pointer is owned by VLFWMain
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static Monitor* GetPrimaryMonitor();

			/*!
			 * \brief Returns the number of currently connected monitors.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static Int GetMonitorCount();

			/*!
			 * \brief Returns every currently connected monitor.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static const std::vector<Monitor*>& GetMonitors();

			/*!
			 * \brief Gets the position of the monitor in screen coordinates.
			 *
			 * The returned point is the distance between the top-left corner
			 * of the primary monitor to the top-left corner of this monitor
			 */
			Point<Int> GetPosition() const;

			/*!
			 * \brief Gets the physical size of the monitor in millimeters
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			Point<Int> GetPhysicalSize() const;

			/*!
			 * \brief Gets the content scale of the monitor
			 *
			 * \glfw
			 * This function retrieves the content scale for the specified
			 * monitor. The content scale is the ratio between the current DPI
			 * and the platform's default DPI. This is especially important for
			 * text and any UI elements. If the pixel dimensions of your UI
			 * scaled by this look appropriate on your machine then it should
			 * appear at a reasonable size on other machines regardless of
			 * their DPI and scaling settings. This relies on the system DPI
			 * and scaling settings being somewhat correct. The content
			 * scale may depend on both the monitor resolution and pixel
			 * density and on user settings. It may be very different from the
			 * raw DPI calculated from the physical size and current
			 * resolution.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			Vector2 GetContentScale() const;

			/*!
			 * \brief Gets the available working area of the monitor in screen coordinates
			 *
			 * \glfw
			 * The work area is defined as the area of the monitor not occluded
			 * by the operating system task bar where present. If no task bar
			 * exists then the work area is the monitor resolution in screen
			 * coordinates.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			Area<Int> GetWorkingArea() const;

			/*!
			 * \brief Gets the human-readable name of the monitor
			 *
			 * \glfw
			 * This function returns a human-readable name, encoded as UTF-8,
			 * of the specified monitor. The name typically reflects the make
			 * and model of the monitor and is not guaranteed to be unique
			 * among the connected monitors.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			std::string GetName() const;

			/*!
			 * \brief Sets the gamma exponent of the monitor
			 *
			 * This function auto-generates a gamma ramp from the specified
			 * exponent value and applies it to the monitor. More granular
			 * gamma control can be achieved through
			 * SetGammaRamp()
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SetGamma(double gamma);

			/*!
			 * \brief Gets the gamma ramp of the monitor
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			inline const std::vector<Color>& GetGammaRamp() const { return gammaRamp; }

			/*!
			 * \brief Sets the full gamma ramp of the monitor
			 *
			 * For best results, this gamma ramp should have the same length as
			 * that initially returned by GetGammaRamp().
			 *
			 * \glfw
			 * The software controlled gamma ramp is applied in addition to the
			 * hardware gamma correction, which today is usually an
			 * approximation of sRGB gamma. This means that setting a perfectly
			 * linear ramp, or gamma 1.0, will produce the default (usually
			 * sRGB-like) behavior.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SetGammaRamp(const std::vector<Color>& ramp);

			/*!
			 * \brief Gets the current output mode of the monitor
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			VideoMode GetVideoMode() const;

			/*!
			 * \brief Gets all supported output modes of the monitor
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			inline const std::vector<VideoMode>& GetSupportedVideoModes() const { return supportedModes; }
		};
	}
}

#endif
