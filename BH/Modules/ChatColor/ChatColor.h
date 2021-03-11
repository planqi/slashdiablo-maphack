#pragma once
#include "../../D2Structs.h"
#include "../Module.h"
#include "../../Config.h"
#include "../../Common.h"
#include <atomic>

class ChatColor : public Module {
private:
	std::atomic<bool> inGame;
	std::map<string, unsigned int> whisperColors;
	void UpdateInGame();
public:
	ChatColor() : Module("Chat Color") {};

	void Init();

	void OnLoad();
	void LoadConfig();
	void OnGameJoin();
	void OnGameExit();
	void OnChatPacketRecv(BYTE* packet, bool *block);
};
