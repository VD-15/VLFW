#ifndef VLFW_INPUT_HPP
#define VLFW_INPUT_HPP

namespace vlk
{
	namespace vlfw
	{
	
		/*!
		 * \brief Represents a named, platform-independant keyboard key
		 *
		 * Not all keys will be represented here, in some circumstances, it may
		 * be useful to use a key's scancode instead, though this method is not
		 * reliable across platforms.
		 */
		enum class Key
		{
			// Values taken from glfw.h

			Unknown =          -1,

			/* Printable keys */
			Space =            32,
			Apostrophe =       39, /* ' */
			Comma =            44, /* , */
			Minus =            45, /* - */
			Period =           46, /* . */
			FwdSlash =         47, /* / */
			Num0 =             48,
			Num1 =             49,
			Num2 =             50,
			Num3 =             51,
			Num4 =             52,
			Num5 =             53,
			Num6 =             54,
			Num7 =             55,
			Num8 =             56,
			Num9 =             57,
			Semicolon =        59, /* ; */
			Equal =            61, /* = */
			A =                65,
			B =                66,
			C =                67,
			D =                68,
			E =                69,
			F =                70,
			G =                71,
			H =                72,
			I =                73,
			J =                74,
			K =                75,
			L =                76,
			M =                77,
			N =                78,
			O =                79,
			P =                80,
			Q =                81,
			R =                82,
			S =                83,
			T =                84,
			U =                85,
			V =                86,
			W =                87,
			X =                88,
			Y =                89,
			Z =                90,
			LeftBracket =      91, /* [ */
			BackSlash =        92, /* \ */
			RightBracket =     93, /* ] */
			Grave =            96, /* ` */
			World1 =           161,/* non-US #1 */
			World2 =           162,/* non-US #2 */

			/* Function keys */
			Escape =           256,
			Enter =            257,
			Tab =              258,
			Backspace =        259,
			Insert =           260,
			Delete =           261,
			RightArrow =       262,
			LeftArrow =        263,
			DownArrow =        264,
			UpArrow =          265,
			PageUp =           266,
			PageDown =         267,
			Home =             268,
			End =              269,
			CapsLock =         280,
			ScrollLock =       281,
			NumLock =          282,
			PrintScreen =      283,
			Pause =            284,
			F1 =               290,
			F2 =               291,
			F3 =               292,
			F4 =               293,
			F5 =               294,
			F6 =               295,
			F7 =               296,
			F8 =               297,
			F9 =               298,
			F10 =              299,
			F11 =              300,
			F12 =              301,
			F13 =              302,
			F14 =              303,
			F15 =              304,
			F16 =              305,
			F17 =              306,
			F18 =              307,
			F19 =              308,
			F20 =              309,
			F21 =              310,
			F22 =              311,
			F23 =              312,
			F24 =              313,
			F25 =              314,
			Numpad0 =          320,
			Numpad1 =          321,
			Numpad2 =          322,
			Numpad3 =          323,
			Numpad4 =          324,
			Numpad5 =          325,
			Numpad6 =          326,
			Numpad7 =          327,
			Numpad8 =          328,
			Numpad9 =          329,
			NumpadDecimal =    330,
			NumpadDivide =     331, 
			NumpadMultiply =   332,
			NumpadSubtract =   333,
			NumpadAdd =        334,
			NumpadEnter =      335,
			NumpadEqual =      336,
			LeftShift =        340,
			LeftControl =      341,
			LeftAlt =          342,
			LeftSuper =        343,
			RightShift =       344,
			RightControl =     345,
			RightAlt =         346,
			RightSuper =       347,
			Menu =             348,

			First =            32,
			Last =             Menu,
		};

		/*!
		 * \brief Represents a generic mouse button
		 */
		enum class MouseButton
		{
			Button0 =   0,
			Button1 =   1,
			Button2 =   2,
			Button3 =   3,
			Button4 =   4,
			Button5 =   5,
			Button6 =   6,
			Button7 =   7,
			Left =      Button0,
			Right =     Button1,
			Middle =    Button2
		};
	}
}

#endif
