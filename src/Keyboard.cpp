#include "VLFW/Keyboard.hpp"

#include "GLFW/glfw3.h"
#include <stdexcept>
#include <unordered_set>

using namespace vlk;
using namespace vlfw;

namespace
{
	// Keys pressed this frame
	std::unordered_set<Int> pressedSet;

	// Keys released this frame
	std::unordered_set<Int> releasedSet;

	// Keys that are currently down
	std::unordered_set<Int> downSet;

	// KeyboardMain access
	std::mutex mtx;
}

Int Keyboard::GetKeyScancode(Key key)
{
	return glfwGetKeyScancode(static_cast<Int>(key));
}

const std::string& Keyboard::GetKeyName(Key key)
{
	static const std::string unknown = "UNKNOWN";

	/* IDs 32 - 96 */
	static const std::string printables[] =
	{
		"Space",        // 32
		unknown,        // 33
		unknown,        // 34
		unknown,        // 35
		unknown,        // 36
		unknown,        // 37
		unknown,        // 38
		"'",            // 39
		unknown,        // 40
		unknown,        // 41
		unknown,        // 42
		unknown,        // 43
		",",            // 44
		"-",            // 45
		".",            // 46
		"/",            // 47
		"0",            // 48
		"1",            // 49
		"2",            // 50
		"3",            // 51
		"4",            // 52
		"5",            // 53
		"6",            // 54
		"7",            // 55
		"8",            // 56
		"9",            // 57
		unknown,        // 58
		";",            // 59
		unknown,        // 60
		"=",            // 61
		unknown,        // 62
		unknown,        // 63
		unknown,        // 64
		"A",            // 65
		"B",            // 66
		"C",            // 67
		"D",            // 68
		"E",            // 69
		"F",            // 70
		"G",            // 71
		"H",            // 72
		"I",            // 73
		"J",            // 74
		"K",            // 75
		"L",            // 76
		"M",            // 77
		"N",            // 78
		"O",            // 79
		"P",            // 80
		"Q",            // 81
		"R",            // 82
		"S",            // 83
		"T",            // 84
		"U",            // 85
		"V",            // 86
		"W",            // 87
		"X",            // 88
		"Y",            // 89
		"Z",            // 90
		"[",            // 91
		"\\",           // 92
		"]",            // 93
		unknown,        // 94
		unknown,        // 95
		"`",            // 96
	};

	/* IDs 256 - 348*/
	static const std::string functions[] =
	{
		"Escape",       // 256
		"Enter",        // 257
		"Tab",          // 258
		"Backspace",    // 259
		"Insert",       // 260
		"Delete",       // 261
		"Right Arrow",  // 262
		"Left Arrow",   // 263
		"Down Arrow",   // 264
		"Up Arrow",     // 265
		"Page Up",      // 266
		"Page Down",    // 267
		"Home",         // 268
		"End",          // 269
		unknown,        // 270
		unknown,        // 271
		unknown,        // 272
		unknown,        // 273
		unknown,        // 274
		unknown,        // 275
		unknown,        // 276
		unknown,        // 277
		unknown,        // 278
		unknown,        // 279
		"Caps Lock",    // 280
		"Scroll Lock",  // 281
		"Num Lock",     // 282
		"Print Screen", // 283
		"Pause",        // 284
		unknown,        // 285
		unknown,        // 286
		unknown,        // 287
		unknown,        // 288
		unknown,        // 289
		"F1",           // 290
		"F2",           // 291
		"F3",           // 292
		"F4",           // 293
		"F5",           // 294
		"F6",           // 295
		"F7",           // 296
		"F8",           // 297
		"F9",           // 298
		"F10",          // 299
		"F11",          // 300
		"F12",          // 301
		"F13",          // 302
		"F14",          // 303
		"F15",          // 304
		"F16",          // 305
		"F17",          // 306
		"F18",          // 307
		"F19",          // 308
		"F20",          // 309
		"F21",          // 310
		"F22",          // 311
		"F23",          // 312
		"F24",          // 313
		"F25",          // 314
		unknown,        // 315
		unknown,        // 316
		unknown,        // 317
		unknown,        // 318
		unknown,        // 319
		"Num 0",        // 320
		"Num 1",        // 321
		"Num 2",        // 322
		"Num 3",        // 323
		"Num 4",        // 324
		"Num 5",        // 325
		"Num 6",        // 326
		"Num 7",        // 327
		"Num 8",        // 328
		"Num 9",        // 329
		"Num Decimal",  // 330
		"Num Divide",   // 331
		"Num Multiply", // 332
		"Num Subtract", // 333
		"Num Add",      // 334
		"Num Enter",    // 335
		"Num Equal",    // 336
		unknown,        // 337
		unknown,        // 338
		unknown,        // 339
		"Left Shift",   // 340
		"Left Control", // 341
		"Left Alt",     // 342
		"Left Super",   // 343
		"Right Shift",  // 344
		"Right Control",// 345
		"Right Alt",    // 346
		"Right Super",  // 347
		"Menu",         // 348
	};

	Int k = static_cast<Int>(key);

	if (k < 32) return unknown;
	if (k < 97) return printables[k - 32];
	if (k < 256) return unknown;
	if (k < 349) return functions[k - 256];
	return unknown;
}

bool Keyboard::IsKeyDown(Int scancode)
{
	return downSet.find(scancode) != downSet.end();
}

bool Keyboard::IsKeyUp(Int scancode)
{
	return downSet.find(scancode) == downSet.end();
}

bool Keyboard::IsKeyPressed(Int scancode)
{
	return pressedSet.find(scancode) != pressedSet.end();
}

bool Keyboard::IsKeyReleased(Int scancode)
{
	return releasedSet.find(scancode) != releasedSet.end();
}

KeyboardMain::KeyboardMain(const KeyboardMainArgs&) :
	lock(mtx, std::try_to_lock)
{
	if (!lock.owns_lock())
	{
		throw std::runtime_error("Multiple concurrent instances of KeyboardMain are disallowed.");
		return;
	}

	pressedSet.clear();
	releasedSet.clear();
	downSet.clear();
}

// Keys should be pressed for at least one frame
// Keys should be released for at least one frame
// Keys can be pressed and released in the same frame
// These functions should only get invoked during a PreUpdateEvent,
// so I don't think we need to worry about synchronization here

void KeyboardMain::OnEvent(const Window::KeyDownEvent& ev)
{
	pressedSet.insert(ev.scancode);
	downSet.insert(ev.scancode);
}

void KeyboardMain::OnEvent(const Window::KeyUpEvent& ev)
{
	releasedSet.insert(ev.scancode);
	downSet.erase(ev.scancode);
}

void KeyboardMain::OnEvent(const PostUpdateEvent& ev)
{
	pressedSet.clear();
	releasedSet.clear();
}
