// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#include "core/aimbot/aimbot.h"
#include "core/esp/drawing.h"
#include "core/extra/init.h"
#include "config/config.h"
#include "imgui/imgui.h"
#include "mem/mem.h"
#include "proc/proc.h"
#include "ProtoMorph.cpp"

#include "main.h"
#include "hook.h"
#include "helpers.h"
#include "menu.h"
#include "offsets.h"
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <vector>

// reference additional headers your program requires here