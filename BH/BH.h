#pragma once
#include <string>
#include <Windows.h>
#include <thread>
#include <chrono>
#include "Modules/ModuleManager.h"
#include "Config.h"
#include "Drawing.h"
#include "Patch.h"

using namespace std;

//boosts  hash_combine
//https://stackoverflow.com/a/19195373/597419
template <class T>
inline void hash_combine(std::size_t& s, const T& v)
{
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

struct cGuardModule
{	
	union {
		HMODULE hModule;
		DWORD dwBaseAddress;
	};
	DWORD _1;
	char szPath[MAX_PATH];
};

namespace BH {
	extern string path;
	extern HINSTANCE instance;
	extern ModuleManager* moduleManager;
	extern Config* config;
	extern Config* itemConfig;
	extern Drawing::UI* settingsUI;
	extern Drawing::StatsDisplay* statsDisplay;
	extern WNDPROC OldWNDPROC;
	extern map<string, Toggle>* MiscToggles;
	extern map<string, Toggle>* MiscToggles2;
	extern map<string, bool>* BnetBools;
	extern map<string, bool>* GamefilterBools;
	extern map<size_t, string> drops;
	extern bool cGuardLoaded;
	extern bool initialized;
	extern Patch* oogDraw;

	extern bool Startup(HINSTANCE instance, VOID* reserved);
	extern "C" __declspec(dllexport) void Initialize();
	extern bool Shutdown();
	extern bool ReloadConfig();
};
