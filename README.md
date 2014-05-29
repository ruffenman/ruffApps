ruffApps - openFrameworks apps to play with
===========================================

Overview
--------

Hello!

The idea of this repo is to provide a small sandbox that makes it easy to experiment with openFrameworks and other stuffs that might be interesting to use together.

As of now this is very much a Windows/Visual Studio endeavor. Other platforms and compilers can be used with the same source code, but I don't currently have any plans to set up projects for them. Currently.


Setup
-----

Go to http://openframeworks.cc/download/ to download openFrameworks for Visual Studio and to see how to download and set up the Visual Studio IDE itself.

Unzip your openFrameworks installation somewhere that makes you feel warm and fuzzy.

Create a new folder called 'ruffApps' within the 'apps/' folder of your openFrameworks installation (right next to the default 'myApps/' folder that's already set up for you).

Clone the repo into this folder.

Open 'ruffApps/jank/jank.sln' in Visual Studio by right clicking it and selecting 'Open With...', then choosing the version of Visual Studio that you were instructed to install. You can also open Visual Studio first and use the File menu or drag the .sln file itself into the IDE to open it.

Build the jank project to build everything or build each project individually as desired.


Projects
========


Lua Standalone Interpreter - ruffApps/lua
-----------------------------------------

Lets you run Lua scripts you pass to it, or work with it interactively. http://www.lua.org/manual/5.2/manual.html#7

The Lua interpreter will be created in 'ruffApps/lua/bin'. In Windows Explorer, Shift-RightClick in this folder and open a terminal here to run it. If you run the interpreter with no arguments/options, you will enter INTERACTIVE MODE. You can leave this mode by pressing Ctrl-C. Shell scripts, AutoHotKey scripts, and .bat files can make this more convenient and/or interesting.


Lua Script Compiler - ruffApps/luac
-----------------------------------

Translates programs written in the Lua programming language into binary files that can be loaded and executed with lua_dofile in C or with dofile in Lua. http://www.lua.org/manual/4.0/luac.html

The Lua script compiler will be created in 'ruffApps/luac/bin'. Using similar methods as above, run it in a terminal.


Lua Standard Libraries - ruffApps/luaLib
----------------------------------------

The standard libraries used by Lua and by applications that want to work with Lua. http://www.lua.org/manual/5.2/manual.html#6

The Lua library is a .dll that will be created in 'ruffApps/luaLib/bin'. The luaLib project can be added as a project reference in other projects to allow it to be linked against. If you wish to access the full set of symbols in the Lua source (the luac project does this), a static library with this information will be built and placed in 'ruffApps/luaLib/lib' that can be linked against. This shouldn't be necessary in most cases.


Jank - ruffApps/jank
--------------------

A randomly named test project centered loosely around the idea of using Lua as a high level control layer for a real time audio synthesis application/instrument. Audio synthesis is being done through the use of the Maximilian library. https://github.com/micknoise/Maximilian

The Jank executable will be created in 'ruffApps/jank/bin'. At the moment performance may be a little iffy depending on your sound card/drivers. I've had luck using the ASIO4ALL driver. http://www.asio4all.com/

luaXinput - ruffApps/luaXinput
------------------------------

A Lua module that provides some basic functionality of DirectX Xinput. Needs access to the xinput.dll appropriate for your platform. (I'm using the June 2010 DirectX SDK on Windows 7)

The luaXinput.dll will be created in 'ruffApps/luaXinput/bin'.

maximua - ruffApps/maximua
--------------------------

A Lua module that provides access to Maximilian's functionality from script.

The maximua.dll will be created in 'ruffApps/maximua/bin'.
