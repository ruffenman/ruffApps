/**
 * Copyright (c) 2013, Bart van Strien <bart.bes@gmail.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#include <windows.h>
#include <xinput.h>
#include <lua.hpp>
#include <cstring>

#ifdef _DEBUG
#define LUAXINPUT_ENTRYPOINT luaopen_luaXinputD
#else
#define LUAXINPUT_ENTRYPOINT luaopen_luaXinput
#endif

namespace
{
namespace h
{

// user is an integer between 0 and 3
static int checkuser(lua_State *L, int idx, bool hasAny = false)
{
	int user = luaL_checkinteger(L, 1);
	if (hasAny && user == -1)
		return XUSER_INDEX_ANY;

	if (user < 0 || user > 3)
		return luaL_error(L, "User index needs to be between 0 and 3");

	return user;
}

// flags is a string that can contain one of:
// - gamepad
static int getflags(const char *str)
{
	int flags = 0;
	if (!str)
		return flags;

	if (!strcmp(str, "gamepad"))
		flags = 0; // INPUT_FLAG_GAMEPAD; TODO

	return flags;
}

static int winerror(lua_State *L, int err)
{
	lua_pushnil(L);

	switch (err)
	{
	case ERROR_DEVICE_NOT_CONNECTED:
		lua_pushstring(L, "Device not connected");
		break;
	case ERROR_EMPTY:
		return 1;
	default:
		lua_pushnumber(L, err);
		break;
	}

	return 2;
}

// virtualkey is one of:
// - a
// - b
// - x
// - y
// - leftShoulder
// - rightShoulder
// - leftTrigger
// - rightTrigger
// - dpadUp
// - dpadDown
// - dpadLeft
// - dpadRight
// - start
// - back
// - leftThumbPress
// - leftThumbUp
// - leftThumbDown
// - leftThumbLeft
// - leftThumbRight
// - leftThumbUpLeft
// - leftThumbUpRight
// - leftThumbDownLeft
// - leftThumbDownRight
// - rightThumbPress
// - rightThumbUp
// - rightThumbDown
// - rightThumbLeft
// - rightThumbRight
// - rightThumbUpLeft
// - rightThumbUpRight
// - rightThumbDownLeft
// - rightThumbDownRight
static void pushvirtualkey(lua_State *L, WORD key)
{
	switch(key)
	{
	case VK_PAD_A:
		lua_pushstring(L, "a");
		break;
	case VK_PAD_B:
		lua_pushstring(L, "b");
		break;
	case VK_PAD_X:
		lua_pushstring(L, "x");
		break;
	case VK_PAD_Y:
		lua_pushstring(L, "y");
		break;

	case VK_PAD_LSHOULDER:
		lua_pushstring(L, "leftShoulder");
		break;
	case VK_PAD_RSHOULDER:
		lua_pushstring(L, "rightShoulder");
		break;

	case VK_PAD_LTRIGGER:
		lua_pushstring(L, "leftTrigger");
		break;
	case VK_PAD_RTRIGGER:
		lua_pushstring(L, "rightTrigger");
		break;

	case VK_PAD_DPAD_UP:
		lua_pushstring(L, "dpadUp");
		break;
	case VK_PAD_DPAD_DOWN:
		lua_pushstring(L, "dpadDown");
		break;
	case VK_PAD_DPAD_LEFT:
		lua_pushstring(L, "dpadLeft");
		break;
	case VK_PAD_DPAD_RIGHT:
		lua_pushstring(L, "dpadRight");
		break;

	case VK_PAD_START:
		lua_pushstring(L, "start");
		break;
	case VK_PAD_BACK:
		lua_pushstring(L, "back");
		break;

	case VK_PAD_LTHUMB_PRESS:
		lua_pushstring(L, "leftThumbPress");
		break;
	case VK_PAD_LTHUMB_UP:
		lua_pushstring(L, "leftThumbUp");
		break;
	case VK_PAD_LTHUMB_DOWN:
		lua_pushstring(L, "leftThumbDown");
		break;
	case VK_PAD_LTHUMB_LEFT:
		lua_pushstring(L, "leftThumbLeft");
		break;
	case VK_PAD_LTHUMB_RIGHT:
		lua_pushstring(L, "leftThumbRight");
		break;
	case VK_PAD_LTHUMB_UPLEFT:
		lua_pushstring(L, "leftThumbUpLeft");
		break;
	case VK_PAD_LTHUMB_UPRIGHT:
		lua_pushstring(L, "leftThumbUpRight");
		break;
	case VK_PAD_LTHUMB_DOWNLEFT:
		lua_pushstring(L, "leftThumbDownLeft");
		break;
	case VK_PAD_LTHUMB_DOWNRIGHT:
		lua_pushstring(L, "leftThumbDownRight");
		break;

	case VK_PAD_RTHUMB_PRESS:
		lua_pushstring(L, "rightThumbPress");
		break;
	case VK_PAD_RTHUMB_UP:
		lua_pushstring(L, "rightThumbUp");
		break;
	case VK_PAD_RTHUMB_DOWN:
		lua_pushstring(L, "rightThumbDown");
		break;
	case VK_PAD_RTHUMB_LEFT:
		lua_pushstring(L, "rightThumbLeft");
		break;
	case VK_PAD_RTHUMB_RIGHT:
		lua_pushstring(L, "rightThumbRight");
		break;
	case VK_PAD_RTHUMB_UPLEFT:
		lua_pushstring(L, "rightThumbUpLeft");
		break;
	case VK_PAD_RTHUMB_UPRIGHT:
		lua_pushstring(L, "rightThumbUpRight");
		break;
	case VK_PAD_RTHUMB_DOWNLEFT:
		lua_pushstring(L, "rightThumbDownLeft");
		break;
	case VK_PAD_RTHUMB_DOWNRIGHT:
		lua_pushstring(L, "rightThumbDownRight");
		break;

	default:
		lua_pushnil(L);
		break;
	}
}

// button is one of:
// - dpadUp
// - dpadDown
// - dpadLeft
// - dpadRight
// - start
// - back
// - leftThumb
// - rightThumb
// - leftShoulder
// - rightShoulder
// - a
// - b
// - x
// - y
static void pushbutton(lua_State *L, WORD button)
{
	switch(button)
	{
	case XINPUT_GAMEPAD_DPAD_UP:
		lua_pushstring(L, "dpadUp");
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		lua_pushstring(L, "dpadDown");
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		lua_pushstring(L, "dpadLeft");
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		lua_pushstring(L, "dpadRight");
		break;

	case XINPUT_GAMEPAD_START:
		lua_pushstring(L, "start");
		break;
	case XINPUT_GAMEPAD_BACK:
		lua_pushstring(L, "back");
		break;

	case XINPUT_GAMEPAD_LEFT_THUMB:
		lua_pushstring(L, "leftThumb");
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		lua_pushstring(L, "rightThumb");
		break;

	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		lua_pushstring(L, "leftShoulder");
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		lua_pushstring(L, "rightShoulder");
		break;

	case XINPUT_GAMEPAD_A:
		lua_pushstring(L, "a");
		break;
	case XINPUT_GAMEPAD_B:
		lua_pushstring(L, "b");
		break;
	case XINPUT_GAMEPAD_X:
		lua_pushstring(L, "x");
		break;
	case XINPUT_GAMEPAD_Y:
		lua_pushstring(L, "y");
		break;

	default:
		lua_pushstring(L, "unknown");
		break;
	}
}

// controllerType is a string, that is one of:
// - gamepad
static void pushControllerType(lua_State *L, unsigned char type)
{
	switch(type)
	{
	case XINPUT_DEVTYPE_GAMEPAD:
	default:
		lua_pushstring(L, "gamepad");
		break;
	}
}

// controllerSubtype is a string, that is one of:
// - wheel
// - arcadeStick
// - flightStick
// - dancePad
// - guitar
// - guitarAlternate
// - guitarBass
// - drumkit
// - gamepad
// - unknown
static void pushControllerSubtype(lua_State *L, unsigned char type)
{
	switch(type)
	{
	case XINPUT_DEVSUBTYPE_WHEEL:
		lua_pushstring(L, "wheel");
		break;
	case XINPUT_DEVSUBTYPE_ARCADE_STICK:
		lua_pushstring(L, "arcadeStick");
		break;
	case XINPUT_DEVSUBTYPE_FLIGHT_SICK:
		lua_pushstring(L, "flightStick");
		break;
	case XINPUT_DEVSUBTYPE_DANCE_PAD:
		lua_pushstring(L, "dancePad");
		break;
	case XINPUT_DEVSUBTYPE_GUITAR:
		lua_pushstring(L, "guitar");
		break;
	case XINPUT_DEVSUBTYPE_DRUM_KIT:
		lua_pushstring(L, "drumkit");
		break;
#ifdef XINPUT_HAS_ALL_THE_THINGS
	case XINPUT_DEVSUBTYPE_GUITAR_ALTERNATE:
		lua_pushstring(L, "guitarAlternate");
		break;
	case XINPUT_DEVSUBTYPE_GUITAR_BASS:
		lua_pushstring(L, "guitarBass");
		break;
	case XINPUT_DEVSUBTYPE_UNKNOWN:
	default:
		lua_pushstring(L, "unknown");
		break;
#else
	default:
#endif
	case XINPUT_DEVSUBTYPE_GAMEPAD:
		lua_pushstring(L, "gamepad");
		break;
	}
}

#define pushControllerFlags_pushFlag(FLAG, NAME) if (flags & FLAG) \
{ \
	lua_pushstring(L, NAME); \
	lua_pushboolean(L, 1); \
	lua_settable(L, -3); \
}

// controllerFlags are a table with in it as keys the capability flags
// and as value whether they are supported or not
//
// The flags are one of:
// - voice
// - forceFeedback
// - wireless
// - plugins
// - noNavigation
static void pushControllerFlags(lua_State *L, WORD flags)
{
	lua_newtable(L);

	pushControllerFlags_pushFlag(XINPUT_CAPS_VOICE_SUPPORTED, "voice");
#ifdef XINPUT_HAS_ALL_THE_THINGS
	pushControllerFlags_pushFlag(XINPUT_CAPS_FFB_SUPPORTED, "forceFeedback");
	pushControllerFlags_pushFlag(XINPUT_CAPS_WIRELESS, "wireless");
	pushControllerFlags_pushFlag(XINPUT_CAPS_PMD_SUPPORTED, "plugins");
	pushControllerFlags_pushFlag(XINPUT_CAPS_NO_NAVIGATION, "noNavigation");
#endif
}

} // namespace h

namespace w
{

// enable(boolean on = true)
static int enable(lua_State *L)
{
	bool on = true;
	if (lua_isboolean(L, 1))
		on = static_cast<bool>(lua_toboolean(L, 1));

	XInputEnable(on);
	return 0;
}

// string type, string level = getBatteryInformation(user user, string devtype)
// NOTE: Can also return nothing, in case of an error
//
// devtype is a string containing one of:
// - gamepad
// - headset
//
// type is a string containing one of:
// - disconnected
// - wired
// - alkaline
// - nimh
// - unknown
//
// level is a string containing one of:
// - empty
// - low
// - medium
// - full
//
// NOTE: level may not be valid if there's no battery information, see type
static int getBatteryInformation(lua_State *L)
{
	int user = h::checkuser(L, 1);
	const char *type = luaL_checkstring(L, 2);
	int devType;

	if (!strcmp(type, "gamepad"))
		devType = BATTERY_DEVTYPE_GAMEPAD;
	else if (!strcmp(type, "headset"))
		devType = BATTERY_DEVTYPE_HEADSET;
	else
		return luaL_error(L, "Invalid device type");

	XINPUT_BATTERY_INFORMATION info;
	if (XInputGetBatteryInformation(user, devType, &info) != ERROR_SUCCESS)
		return h::winerror(L, ERROR_EMPTY);

	switch (info.BatteryType)
	{
	case BATTERY_TYPE_DISCONNECTED:
		lua_pushstring(L, "disconnected");
		break;
	case BATTERY_TYPE_WIRED:
		lua_pushstring(L, "wired");
		break;
	case BATTERY_TYPE_ALKALINE:
		lua_pushstring(L, "alkaline");
		break;
	case BATTERY_TYPE_NIMH:
		lua_pushstring(L, "nimh");
		break;
	case BATTERY_TYPE_UNKNOWN:
	default:
		lua_pushstring(L, "unknown");
		break;
	}

	switch(info.BatteryLevel)
	{
	case BATTERY_LEVEL_EMPTY:
		lua_pushstring(L, "empty");
		break;
	case BATTERY_LEVEL_LOW:
		lua_pushstring(L, "low");
		break;
	case BATTERY_LEVEL_MEDIUM:
		lua_pushstring(L, "medium");
		break;
	case BATTERY_LEVEL_FULL:
	default:
		lua_pushstring(L, "full");
		break;
	}

	return 2;
}

// table capabilities = getCapabilities(user user, flags type)
// NOTE: Can error in usual nil, error fashion
//
// The returned error can be either:
// - "Device not connected"
// - a number as defined in winerror.h
//
// TODO: Expose current keystate and vibration
static int getCapabilities(lua_State *L)
{
	int user = h::checkuser(L, 1);
	const char *type = luaL_optstring(L, 2, 0);
	int flags = h::getflags(type);

	XINPUT_CAPABILITIES caps;
	int err = XInputGetCapabilities(user, flags, &caps);
	if (err != ERROR_SUCCESS)
		return h::winerror(L, err);

	lua_newtable(L);

	h::pushControllerType(L, caps.Type);
	lua_setfield(L, -2, "type");

	h::pushControllerSubtype(L, caps.SubType);
	lua_setfield(L, -2, "subtype");

	h::pushControllerFlags(L, caps.Flags);
	lua_setfield(L, -2, "flags");

	return 1;
}

// int user, boolean down, boolean repeat, virtualkey key = getKeystroke(user userFilter, flags type)
// NOTE: Can error in usual nil, error fashion
//
// The userFilter can also be -1, for any user
static int getKeystroke(lua_State *L)
{
	int user = h::checkuser(L, 1, true);
	const char *type = luaL_optstring(L, 2, 0);
	int flags = h::getflags(type);

	XINPUT_KEYSTROKE keystroke;
	int err = XInputGetKeystroke(user, flags, &keystroke);
	if (err != ERROR_SUCCESS)
		return h::winerror(L, err);

	lua_pushnumber(L, keystroke.UserIndex);
	lua_pushboolean(L, flags & XINPUT_KEYSTROKE_KEYDOWN);
	lua_pushboolean(L, flags & XINPUT_KEYSTROKE_REPEAT);
	h::pushvirtualkey(L, keystroke.VirtualKey);

	return 4;
}

// int state, table keys, int leftTrigger, int rightTrigger, int leftX, int leftY, int rightX, int rightY = getState(user user)
// With keys a table that has a button as key and a boolean as value.
// NOTE: Can error in usual nil, error fashion
//
// leftTrigger and rightTrigger are between 0 and 255
// leftX, leftY, rightX and rightY are between -32768 and 32767
static int getState(lua_State *L)
{
	int user = h::checkuser(L, 1);

	XINPUT_STATE state;
	int err = XInputGetState(user, &state);
	if (err != ERROR_SUCCESS)
		return h::winerror(L, err);
	
	lua_pushnumber(L, state.dwPacketNumber);
	/*
	lua_newtable(L);
	WORD buttonTest = 1;
	do
	{
		h::pushbutton(L, buttonTest);
		lua_pushboolean(L, state.Gamepad.wButtons & buttonTest);
		lua_settable(L, -3);
	} while (buttonTest <<= 1);
	*/
	lua_pushnumber(L, state.Gamepad.bLeftTrigger);
	lua_pushnumber(L, state.Gamepad.bRightTrigger);
	lua_pushnumber(L, state.Gamepad.sThumbLX);
	lua_pushnumber(L, state.Gamepad.sThumbLY);
	lua_pushnumber(L, state.Gamepad.sThumbRX);
	lua_pushnumber(L, state.Gamepad.sThumbRY);

	return 7;
}

// boolean success = setState(user user, int leftMotor, int rightMotor)
// NOTE: Can error in usual nil, error fashion
//
// leftMotor and rightMotor are between 0 and 65535
static int setState(lua_State *L)
{
	int user = h::checkuser(L, 1);

	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = luaL_checkinteger(L, 2);
	vibration.wRightMotorSpeed = luaL_checkinteger(L, 3);

	int ret = XInputSetState(user, &vibration);
	if (ret != ERROR_SUCCESS)
		return h::winerror(L, ret);

	lua_pushboolean(L, 1);
	return 1;
}

} // namespace w

static luaL_Reg funcs[] = {
	{"enable", w::enable},
	{"getBatteryInformation", w::getBatteryInformation},
	{"getCapabilities", w::getCapabilities},
	{"getKeystroke", w::getKeystroke},
	{"getState", w::getState},
	{"setState", w::setState},
	{0, 0},
};

} // namespace

extern "C" {
	__declspec(dllexport) int LUAXINPUT_ENTRYPOINT (lua_State *L) {
		luaL_newlibtable(L, funcs);
		luaL_setfuncs(L, funcs, 0);
		return 1;
	}
}
