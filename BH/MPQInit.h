#pragma once
#include <Windows.h>
#include <algorithm>
#include <locale>
#include <cstdlib>
#include <fstream>
#include <map>
#include <unordered_map>
#include "Constants.h"
#include "Common.h"
#include "D2Structs.h"


#define ITEM_GROUP_HELM					0x00000001
#define ITEM_GROUP_ARMOR				0x00000002
#define ITEM_GROUP_SHIELD				0x00000004
#define ITEM_GROUP_GLOVES				0x00000008
#define ITEM_GROUP_BOOTS				0x00000010
#define ITEM_GROUP_BELT					0x00000020
#define ITEM_GROUP_DRUID_PELT			0x00000040
#define ITEM_GROUP_BARBARIAN_HELM		0x00000080
#define ITEM_GROUP_PALADIN_SHIELD		0x00000100
#define ITEM_GROUP_NECROMANCER_SHIELD	0x00000200
#define ITEM_GROUP_AXE					0x00000400
#define ITEM_GROUP_MACE					0x00000800
#define ITEM_GROUP_SWORD				0x00001000
#define ITEM_GROUP_DAGGER				0x00002000
#define ITEM_GROUP_THROWING				0x00004000
#define ITEM_GROUP_JAVELIN				0x00008000
#define ITEM_GROUP_SPEAR				0x00010000
#define ITEM_GROUP_POLEARM				0x00020000
#define ITEM_GROUP_BOW					0x00040000
#define ITEM_GROUP_CROSSBOW				0x00080000
#define ITEM_GROUP_STAFF				0x00100000
#define ITEM_GROUP_WAND					0x00200000
#define ITEM_GROUP_SCEPTER				0x00400000
#define ITEM_GROUP_ASSASSIN_KATAR		0x00800000
#define ITEM_GROUP_SORCERESS_ORB		0x01000000
#define ITEM_GROUP_AMAZON_WEAPON		0x02000000
#define ITEM_GROUP_NORMAL				0x04000000
#define ITEM_GROUP_EXCEPTIONAL			0x08000000
#define ITEM_GROUP_ELITE				0x10000000
#define ITEM_GROUP_ALLARMOR				0x20000000
#define ITEM_GROUP_ALLWEAPON			0x40000000
#define ITEM_GROUP_CIRCLET				0x80000000

#define ITEM_GROUP_CHIPPED				0x00000001
#define ITEM_GROUP_FLAWED				0x00000002
#define ITEM_GROUP_REGULAR				0x00000004
#define ITEM_GROUP_FLAWLESS				0x00000008
#define ITEM_GROUP_PERFECT				0x00000010
#define ITEM_GROUP_AMETHYST				0x00000020
#define ITEM_GROUP_DIAMOND				0x00000040
#define ITEM_GROUP_EMERALD				0x00000080
#define ITEM_GROUP_RUBY					0x00000100
#define ITEM_GROUP_SAPPHIRE				0x00000200
#define ITEM_GROUP_TOPAZ				0x00000400
#define ITEM_GROUP_SKULL				0x00000800
#define ITEM_GROUP_RUNE					0x00001000


/*
 * MPQInit handles the data we can initialize from MPQ files, provided we
 * are able to load StormLib. It also provides defaults in case we cannot
 * read the MPQ archive.
 */

extern unsigned int STAT_MAX;
extern unsigned int SKILL_MAX;

// Item attributes from ItemTypes.txt and Weapon/Armor/Misc.txt
struct ItemAttributes {
	std::wstring name;
	char code[4];
	std::string category;
	BYTE width;
	BYTE height;
	BYTE stackable;
	BYTE useable;
	BYTE throwable;
	BYTE itemLevel;		// 1=normal, 2=exceptional, 3=elite
	BYTE unusedFlags;
	unsigned int flags;
	unsigned int flags2;
	BYTE qualityLevel;
	BYTE magicLevel;
};

// Properties from ItemStatCost.txt that we need for parsing incoming 0x9c packets, among other things
struct StatProperties {
	std::string name;
	BYTE saveBits;
	BYTE saveParamBits;
	BYTE saveAdd;
	BYTE op;
	BYTE sendParamBits;
	unsigned short ID;
};

struct CharStats {
	int toHitFactor;
};

extern std::vector<StatProperties*> AllStatList;
extern std::unordered_map<std::string, StatProperties*> StatMap;
extern std::vector<CharStats*> CharList;
extern std::map<std::string, ItemAttributes*> ItemAttributeMap;
extern std::map<std::string, InventoryLayout*> InventoryLayoutMap;


#define STAT_NUMBER(name) (StatMap[name]->ID)

bool IsInitialized();
void InitializeMPQData();
