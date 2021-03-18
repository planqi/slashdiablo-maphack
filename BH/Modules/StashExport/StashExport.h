#pragma once
#include "../Module.h"
#include "../../Constants.h"
#include "../../Config.h"
#include "../../Drawing.h"
#include "../../JSONObject.h"
#include "../../Mustache.h"

#define MAX_FUNCTION 23

struct UnitAny;

struct IdNamePair{
	int id;
	std::string name;
};

class StashExport : public Module {
private:

	string dfltExprt;
	map<string, string> mustaches;
	unsigned int exportGear;
	unsigned int exportType;
	static UnitAny* viewingUnit;
	Drawing::UITab* settingsTab;
	std::vector<std::string> options;

	static void StashExport::fillStats(JSONArray* statsArray, JSONObject* itemDef, UnitAny* pItem, ItemsTxtStat stats[], int maxProps);
	static void fillStats(JSONArray* statsArray, JSONObject *itemDef, UnitAny *pItem, std::string codeKey, std::string paramKey, std::string minKey, std::string maxKey, int maxProps);
public:
	static map<std::string, Toggle> Toggles;
	static map<std::string, std::unique_ptr<Mustache::AMustacheTemplate>> MustacheTemplates;

	StashExport() : Module("StashExport"), exportType(0) { dfltExprt = "json"; };

	void OnLoad();
	void OnUnload();

	void LoadConfig();

	void OnLoop();
	void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);
	void WriteStash();
	std::map<string, Toggle>* GetToggles() { return &Toggles; }
	static void GetItemInfo(UnitAny* pItem, JSONObject* pBuffer);
	static JSONObject* getStatEntry(WORD statId, WORD statId2, DWORD statVal, DWORD min, DWORD max);
};

void ItemDesc_Interception();

std::function<JSONObject*(UnitAny*, JSONObject*, JSONObject*, JSONElement*, int, int)> STAT_FUNCTIONS[];
char* QUALITY_NAMES[];
IdNamePair NODE_PAGES[];
IdNamePair CLASS_NAMES[];
IdNamePair SKILL_TABS[];
