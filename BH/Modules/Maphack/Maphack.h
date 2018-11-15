#pragma once
#include "../../D2Structs.h"
#include "../Module.h"
#include "../../Config.h"
#include "../../Drawing.h"

enum MaphackReveal {
	MaphackRevealGame = 0,
	MaphackRevealAct,
	MaphackRevealLevel
};

struct LevelList {
	unsigned int levelId;
	unsigned int x, y, act;
};

struct BaseSkill {
	WORD Skill;
	BYTE Level;
};

class Maphack : public Module {
	private:
		int monsterResistanceThreshold;
		unsigned int revealType;
		unsigned int maxGhostSelection;
		unsigned int reloadConfig;
		bool revealedGame, revealedAct[6], revealedLevel[255];
		std::map<string, string> MonsterColors;
		std::map<string, string> MonsterLines;
		std::map<string, string> MonsterHides;
		std::map<string, unsigned int> TextColorMap; 
		std::map<string, unsigned int> monsterColors;
		std::map<string, unsigned int> missileColors;
		std::map<int, unsigned int> automapMonsterColors;
		std::map<int, unsigned int> automapMonsterLines;
		std::list<int> automapHiddenMonsters;
		std::list<LevelList*> automapLevels;
		map<std::string, Toggle> Toggles;
		Drawing::UITab* settingsTab;
		std::map<DWORD, std::vector<BaseSkill>> Skills;

	public:
	Maphack();

	void ReadConfig();
	void OnLoad();
	void OnUnload();

	void LoadConfig();

	void OnLoop();
	void OnAutomapDraw();
	void OnGameJoin(const string& name, const string& pass, int diff);
	void OnGamePacketRecv(BYTE* packet, bool *block);

	void ResetRevealed();
	void ResetPatches();

	void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);

	void RevealGame();
	void RevealAct(int act);
	void RevealLevel(Level* level);
	void RevealRoom(Room2* room);

	static Level* GetLevel(Act* pAct, int level);
	static AutomapLayer* InitLayer(int level);
};

void Weather_Interception();
void Lighting_Interception();
void Infravision_Interception();
void __stdcall Shake_Interception(LPDWORD lpX, LPDWORD lpY);
void HoverObject_Interception();
