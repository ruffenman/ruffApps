#include "lua.hpp"
#include "maximilian.h"

#ifdef _DEBUG
#define MAXIMUA_ENTRYPOINT luaopen_maximuaD
#else
#define MAXIMUA_ENTRYPOINT luaopen_maximua
#endif

namespace  {

static maxiOsc baseOsc;

static int maximua_osc_new(lua_State *L);
static int maximua_osc_gc(lua_State *L); 
static int maximua_osc_sinewave(lua_State *L);

static luaL_Reg maximua_funcs[] = {
	{"oscNew", maximua_osc_new},
	{0, 0},
};

static luaL_Reg maximua_osc_meta[] = {
	{"__gc", maximua_osc_gc},
	{0, 0},
};

static luaL_Reg maximua_osc_methods[] = {
	{"sinewave", maximua_osc_sinewave},
	{0, 0},
};

static int maximua_osc_new(lua_State *L) {
	// Create osc table
	luaL_newlibtable(L, maximua_osc_methods);
	
	// Create C osc in user data
	maxiOsc *uData = (maxiOsc *)lua_newuserdata(L, sizeof(maxiOsc));
	// Copy from base osc
	*uData = baseOsc;
	// Set osc metatable on userdata
	luaL_setmetatable(L, "maximua.osc");

	// Set osc methods with the maxiOsc userdata as an upvalue
	luaL_setfuncs(L, maximua_osc_methods, 1);

	return 1;
}

static int maximua_osc_gc(lua_State *L) {
	// Clean up osc
	maxiOsc *osc = (maxiOsc *)lua_touserdata(L, 1);
	return 0;
}

static int maximua_osc_sinewave(lua_State *L) {
	maxiOsc *osc = (maxiOsc *)lua_touserdata(L, lua_upvalueindex(1));

	double frequency = static_cast<double>(luaL_checknumber(L, 1));
	lua_pop(L, 1);
	
	double sample = osc->sinewave(frequency);

	lua_pushnumber(L, sample);
	return 1;
}

} // namespace maximua

extern "C" {
	__declspec(dllexport) int MAXIMUA_ENTRYPOINT (lua_State *L)	{
		// Create maximua table
		luaL_newlibtable(L, maximua_funcs);
		// Set maximua functions to table
		luaL_setfuncs(L, maximua_funcs, 0);

		// Create osc userdata metatable
		luaL_newmetatable(L, "maximua.osc");
		luaL_setfuncs(L, maximua_osc_meta, 1);

		return 1;
	}
}