#ifndef VLFW_MONITOR_HPP
#define VLFW_MONITOR_HPP

#include "ValkyrieEngineCommon/Types.hpp"
#include <vector>
#include <string>

namespace vlk
{
	namespace vlfw
	{
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

		class Monitor
		{
			public:
			typedef void* MonitorHandle;

			struct ConnectEvent
			{
				Monitor* monitor;
			};

			struct DisconnectEvent
			{
				Monitor* monitor;
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

			inline void* GetHandle() { return nativeHandle; }

			/*!
			 * \brief Returns the user's preferred monitor.
			 */
			static Monitor* GetPrimaryMonitor();

			/*!
			 * \brief Returns the number of currently connected monitors.
			 */
			static Int GetMonitorCount();

			/*!
			 * \brief Returns every currently connected monitor.
			 */
			static const std::vector<Monitor*>& GetMonitors();

			Point<Int> GetPosition() const;

			/*!
			 * \brief Gets the physical size of the monitor in millimeters
			 */
			Point<Int> GetPhysicalSize() const;

			/*!
			 * \brief Gets the content scale of the monitor
			 */
			Vector2 GetContentScale() const;

			/*!
			 * \brief Gets the available working area of the monitor in screen coordinates
			 */
			Area<Int> GetWorkingArea() const;

			/*!
			 * \brief Gets the human-readable name of the monitor
			 */
			std::string GetName() const;

			/*!
			 * \brief Sets the gamma exponent of the monitor
			 */
			void SetGamma(double gamma);

			/*!
			 * \brief Gets the gamma ramp of the monitor
			 */
			inline const std::vector<Color>& GetGammaRamp() const { return gammaRamp; }

			/*!
			 * \brief Sets the full gamma ramp of the monitor
			 */
			void SetGammaRamp(const std::vector<Color>& ramp);

			/*!
			 * \brief Gets the current output mode of the monitor
			 */
			VideoMode GetVideoMode() const;

			/*!
			 * \brief Gets all supported output modes of the monitor
			 */
			inline const std::vector<VideoMode>& GetSupportedVideoModes() const { return supportedModes; }
		};
	}
}

#endif
