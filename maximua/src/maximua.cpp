#include "lua.hpp"
#include "maximilian.h"

#ifdef _DEBUG
#define MAXIMUA_ENTRYPOINT luaopen_maximuaD
#else
#define MAXIMUA_ENTRYPOINT luaopen_maximua
#endif

namespace {

static maxiOsc osc;

static int oscSine(lua_State *L)
{
	double frequency = static_cast<double>(luaL_checknumber(L, 1));
	lua_pop(L, 1);

	double sample = osc.sinewave(frequency);

	lua_pushnumber(L, sample);
	return 1;
}

static luaL_Reg funcs[] = {
	{"osc.sine", oscSine},
	{0, 0},
};

} // namespace

extern "C" {
	__declspec(dllexport) int MAXIMUA_ENTRYPOINT (lua_State *L)	{
		luaL_newlibtable(L, funcs);
		luaL_setfuncs(L, funcs, 0);
		return 1;
	}
}