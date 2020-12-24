#include "MPQInit.h"
#include <functional>
#include <set>

unsigned int STAT_MAX;
unsigned int SKILL_MAX;
bool initialized = false;

std::vector<StatProperties*> AllStatList;
std::unordered_map<std::string, StatProperties*> StatMap;
std::vector<CharStats*> CharList;
std::map<std::string, ItemAttributes*> ItemAttributeMap;
std::map<std::string, InventoryLayout*> InventoryLayoutMap;

// These are the standard item attributes (if we can't read the patch mpq file)
#pragma region DEFAULTS
ItemAttributes ItemAttributeList[] = {
	{L"Cap", "cap", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 1},
	{L"Skull Cap", "skp", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 5},
	{L"Helm", "hlm", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 11},
	{L"Full Helm", "fhl", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 15},
	{L"Great Helm", "ghm", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 23},
	{L"Crown", "crn", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 29},
	{L"Mask", "msk", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 19},
	{L"Quilted Armor", "qui", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{L"Leather Armor", "lea", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 3},
	{L"Hard Leather Armor", "hla", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 5},
	{L"Studded Leather", "stu", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 8},
	{L"Ring Mail", "rng", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 11},
	{L"Scale Mail", "scl", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 13},
	{L"Chain Mail", "chn", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{L"Breast Plate", "brs", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 18},
	{L"Splint Mail", "spl", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 20},
	{L"Plate Mail", "plt", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Field Plate", "fld", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 28},
	{L"Gothic Plate", "gth", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 32},
	{L"Full Plate Mail", "ful", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 37},
	{L"Ancient Armor", "aar", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 40},
	{L"Light Plate", "ltp", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 35},
	{L"Buckler", "buc", "Shield", 2, 2, 0, 0, 0, 1, 0, 0, 0, 1},
	{L"Small Shield", "sml", "Shield", 2, 2, 0, 0, 0, 1, 0, 0, 0, 5},
	{L"Large Shield", "lrg", "Shield", 2, 3, 0, 0, 0, 1, 0, 0, 0, 11},
	{L"Kite Shield", "kit", "Shield", 2, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{L"Tower Shield", "tow", "Shield", 2, 3, 0, 0, 0, 1, 0, 0, 0, 22},
	{L"Gothic Shield", "gts", "Shield", 2, 4, 0, 0, 0, 1, 0, 0, 0, 30},
	{L"Leather Gloves", "lgl", "Gloves", 2, 2, 0, 0, 0, 1, 0, 0, 0, 3},
	{L"Heavy Gloves", "vgl", "Gloves", 2, 2, 0, 0, 0, 1, 0, 0, 0, 7},
	{L"Chain Gloves", "mgl", "Gloves", 2, 2, 0, 0, 0, 1, 0, 0, 0, 12},
	{L"Light Gauntlets", "tgl", "Gloves", 2, 2, 0, 0, 0, 1, 0, 0, 0, 20},
	{L"Gauntlets", "hgl", "Gloves", 2, 2, 0, 0, 0, 1, 0, 0, 0, 27},
	{L"Boots", "lbt", "Boots", 2, 2, 0, 0, 0, 1, 0, 0, 0, 3},
	{L"Heavy Boots", "vbt", "Boots", 2, 2, 0, 0, 0, 1, 0, 0, 0, 7},
	{L"Chain Boots", "mbt", "Boots", 2, 2, 0, 0, 0, 1, 0, 0, 0, 12},
	{L"Light Plated Boots", "tbt", "Boots", 2, 2, 0, 0, 0, 1, 0, 0, 0, 20},
	{L"Greaves", "hbt", "Boots", 2, 2, 0, 0, 0, 1, 0, 0, 0, 27},
	{L"Sash", "lbl", "Belt", 2, 1, 0, 0, 0, 1, 0, 0, 0, 3},
	{L"Light Belt", "vbl", "Belt", 2, 1, 0, 0, 0, 1, 0, 0, 0, 7},
	{L"Belt", "mbl", "Belt", 2, 1, 0, 0, 0, 1, 0, 0, 0, 12},
	{L"Heavy Belt", "tbl", "Belt", 2, 1, 0, 0, 0, 1, 0, 0, 0, 20},
	{L"Plated Belt", "hbl", "Belt", 2, 1, 0, 0, 0, 1, 0, 0, 0, 27},
	{L"Bone Helm", "bhm", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 22},
	{L"Bone Shield", "bsh", "Shield", 2, 3, 0, 0, 0, 1, 0, 0, 0, 19},
	{L"Spiked Shield", "spk", "Shield", 2, 3, 0, 0, 0, 1, 0, 0, 0, 11},
	{L"War Hat", "xap", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 34},
	{L"Sallet", "xkp", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 37},
	{L"Casque", "xlm", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 42},
	{L"Basinet", "xhl", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 45},
	{L"Winged Helm", "xhm", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 51},
	{L"Grand Crown", "xrn", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 55},
	{L"Death Mask", "xsk", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 48},
	{L"Ghost Armor", "xui", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 34},
	{L"Serpentskin Armor", "xea", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 36},
	{L"Demonhide Armor", "xla", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 37},
	{L"Trellised Armor", "xtu", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 40},
	{L"Linked Mail", "xng", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 42},
	{L"Tigulated Mail", "xcl", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 43},
	{L"Mesh Armor", "xhn", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 45},
	{L"Cuirass", "xrs", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 47},
	{L"Russet Armor", "xpl", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 49},
	{L"Templar Coat", "xlt", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 52},
	{L"Sharktooth Armor", "xld", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 55},
	{L"Embossed Plate", "xth", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 58},
	{L"Chaos Armor", "xul", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 61},
	{L"Ornate Plate", "xar", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 64},
	{L"Mage Plate", "xtp", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 60},
	{L"Defender", "xuc", "Shield", 2, 2, 0, 0, 0, 2, 0, 0, 0, 34},
	{L"Round Shield", "xml", "Shield", 2, 2, 0, 0, 0, 2, 0, 0, 0, 37},
	{L"Scutum", "xrg", "Shield", 2, 3, 0, 0, 0, 2, 0, 0, 0, 42},
	{L"Dragon Shield", "xit", "Shield", 2, 3, 0, 0, 0, 2, 0, 0, 0, 45},
	{L"Pavise", "xow", "Shield", 2, 3, 0, 0, 0, 2, 0, 0, 0, 50},
	{L"Ancient Shield", "xts", "Shield", 2, 4, 0, 0, 0, 2, 0, 0, 0, 56},
	{L"Demonhide Gloves", "xlg", "Gloves", 2, 2, 0, 0, 0, 2, 0, 0, 0, 33},
	{L"Sharkskin Gloves", "xvg", "Gloves", 2, 2, 0, 0, 0, 2, 0, 0, 0, 39},
	{L"Heavy Bracers", "xmg", "Gloves", 2, 2, 0, 0, 0, 2, 0, 0, 0, 43},
	{L"Battle Gauntlets", "xtg", "Gloves", 2, 2, 0, 0, 0, 2, 0, 0, 0, 49},
	{L"War Gauntlets", "xhg", "Gloves", 2, 2, 0, 0, 0, 2, 0, 0, 0, 54},
	{L"Demonhide Boots", "xlb", "Boots", 2, 2, 0, 0, 0, 2, 0, 0, 0, 36},
	{L"Sharkskin Boots", "xvb", "Boots", 2, 2, 0, 0, 0, 2, 0, 0, 0, 39},
	{L"Mesh Boots", "xmb", "Boots", 2, 2, 0, 0, 0, 2, 0, 0, 0, 43},
	{L"Battle Boots", "xtb", "Boots", 2, 2, 0, 0, 0, 2, 0, 0, 0, 49},
	{L"War Boots", "xhb", "Boots", 2, 2, 0, 0, 0, 2, 0, 0, 0, 54},
	{L"Demonhide Sash", "zlb", "Belt", 2, 1, 0, 0, 0, 2, 0, 0, 0, 36},
	{L"Sharkskin Belt", "zvb", "Belt", 2, 1, 0, 0, 0, 2, 0, 0, 0, 39},
	{L"Mesh Belt", "zmb", "Belt", 2, 1, 0, 0, 0, 2, 0, 0, 0, 43},
	{L"Battle Belt", "ztb", "Belt", 2, 1, 0, 0, 0, 2, 0, 0, 0, 49},
	{L"War Belt", "zhb", "Belt", 2, 1, 0, 0, 0, 2, 0, 0, 0, 54},
	{L"Grim Helm", "xh9", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 50},
	{L"Grim Shield", "xsh", "Shield", 2, 3, 0, 0, 0, 2, 0, 0, 0, 48},
	{L"Barbed Shield", "xpk", "Shield", 2, 3, 0, 0, 0, 2, 0, 0, 0, 42},
	{L"Wolf Head", "dr1", "Druid Pelt", 2, 2, 0, 0, 0, 1, 0, 0, 0, 4},
	{L"Hawk Helm", "dr2", "Druid Pelt", 2, 2, 0, 0, 0, 1, 0, 0, 0, 8},
	{L"Antlers", "dr3", "Druid Pelt", 2, 2, 0, 0, 0, 1, 0, 0, 0, 16},
	{L"Falcon Mask", "dr4", "Druid Pelt", 2, 2, 0, 0, 0, 1, 0, 0, 0, 20},
	{L"Spirit Mask", "dr5", "Druid Pelt", 2, 2, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Jawbone Cap", "ba1", "Barbarian Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 4},
	{L"Fanged Helm", "ba2", "Barbarian Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 8},
	{L"Horned Helm", "ba3", "Barbarian Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 16},
	{L"Assault Helmet", "ba4", "Barbarian Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 20},
	{L"Avenger Guard", "ba5", "Barbarian Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Targe", "pa1", "Paladin Shield", 2, 2, 0, 0, 0, 1, 0, 0, 0, 4},
	{L"Rondache", "pa2", "Paladin Shield", 2, 2, 0, 0, 0, 1, 0, 0, 0, 8},
	{L"Heraldic Shield", "pa3", "Paladin Shield", 2, 4, 0, 0, 0, 1, 0, 0, 0, 16},
	{L"Aerin Shield", "pa4", "Paladin Shield", 2, 4, 0, 0, 0, 1, 0, 0, 0, 20},
	{L"Crown Shield", "pa5", "Paladin Shield", 2, 2, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Preserved Head", "ne1", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 1, 0, 0, 0, 4},
	{L"Zombie Head", "ne2", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 1, 0, 0, 0, 8},
	{L"Unraveller Head", "ne3", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 1, 0, 0, 0, 16},
	{L"Gargoyle Head", "ne4", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 1, 0, 0, 0, 20},
	{L"Demon Head", "ne5", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Circlet", "ci0", "Circlet", 2, 2, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Coronet", "ci1", "Circlet", 2, 2, 0, 0, 0, 2, 0, 0, 0, 52},
	{L"Tiara", "ci2", "Circlet", 2, 2, 0, 0, 0, 3, 0, 0, 0, 70},
	{L"Diadem", "ci3", "Circlet", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Shako", "uap", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 58},
	{L"Hydraskull", "ukp", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 63},
	{L"Armet", "ulm", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 68},
	{L"Giant Conch", "uhl", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 54},
	{L"Spired Helm", "uhm", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 79},
	{L"Corona", "urn", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Demonhead", "usk", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 74},
	{L"Dusk Shroud", "uui", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 65},
	{L"Wyrmhide", "uea", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 67},
	{L"Scarab Husk", "ula", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 68},
	{L"Wire Fleece", "utu", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 70},
	{L"Diamond Mail", "ung", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 72},
	{L"Loricated Mail", "ucl", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 73},
	{L"Boneweave", "uhn", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 62},
	{L"Great Hauberk", "urs", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 75},
	{L"Balrog Skin", "upl", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 76},
	{L"Hellforge Plate", "ult", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 78},
	{L"Kraken Shell", "uld", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 81},
	{L"Lacquered Plate", "uth", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 82},
	{L"Shadow Plate", "uul", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 83},
	{L"Sacred Armor", "uar", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Archon Plate", "utp", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 84},
	{L"Heater", "uuc", "Shield", 2, 2, 0, 0, 0, 3, 0, 0, 0, 58},
	{L"Luna", "uml", "Shield", 2, 2, 0, 0, 0, 3, 0, 0, 0, 61},
	{L"Hyperion", "urg", "Shield", 2, 3, 0, 0, 0, 3, 0, 0, 0, 64},
	{L"Monarch", "uit", "Shield", 2, 3, 0, 0, 0, 3, 0, 0, 0, 72},
	{L"Aegis", "uow", "Shield", 2, 3, 0, 0, 0, 3, 0, 0, 0, 79},
	{L"Ward", "uts", "Shield", 2, 4, 0, 0, 0, 3, 0, 0, 0, 84},
	{L"Bramble Mitts", "ulg", "Gloves", 2, 2, 0, 0, 0, 3, 0, 0, 0, 57},
	{L"Vampirebone Gloves", "uvg", "Gloves", 2, 2, 0, 0, 0, 3, 0, 0, 0, 63},
	{L"Vambraces", "umg", "Gloves", 2, 2, 0, 0, 0, 3, 0, 0, 0, 69},
	{L"Crusader Gauntlets", "utg", "Gloves", 2, 2, 0, 0, 0, 3, 0, 0, 0, 76},
	{L"Ogre Gauntlets", "uhg", "Gloves", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Wyrmhide Boots", "ulb", "Boots", 2, 2, 0, 0, 0, 3, 0, 0, 0, 60},
	{L"Scarabshell Boots", "uvb", "Boots", 2, 2, 0, 0, 0, 3, 0, 0, 0, 66},
	{L"Boneweave Boots", "umb", "Boots", 2, 2, 0, 0, 0, 3, 0, 0, 0, 72},
	{L"Mirrored Boots", "utb", "Boots", 2, 2, 0, 0, 0, 3, 0, 0, 0, 81},
	{L"Myrmidon Greaves", "uhb", "Boots", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Spiderweb Sash", "ulc", "Belt", 2, 1, 0, 0, 0, 3, 0, 0, 0, 61},
	{L"Vampirefang Belt", "uvc", "Belt", 2, 1, 0, 0, 0, 3, 0, 0, 0, 68},
	{L"Mithril Coil", "umc", "Belt", 2, 1, 0, 0, 0, 3, 0, 0, 0, 75},
	{L"Troll Belt", "utc", "Belt", 2, 1, 0, 0, 0, 3, 0, 0, 0, 82},
	{L"Colossus Girdle", "uhc", "Belt", 2, 1, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Bone Visage", "uh9", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 84},
	{L"Troll Nest", "ush", "Shield", 2, 3, 0, 0, 0, 3, 0, 0, 0, 76},
	{L"Blade Barrier", "upk", "Shield", 2, 3, 0, 0, 0, 3, 0, 0, 0, 68},
	{L"Alpha Helm", "dr6", "Druid Pelt", 2, 2, 0, 0, 0, 2, 0, 0, 0, 35},
	{L"Griffon Headdress", "dr7", "Druid Pelt", 2, 2, 0, 0, 0, 2, 0, 0, 0, 40},
	{L"Hunter's Guise", "dr8", "Druid Pelt", 2, 2, 0, 0, 0, 2, 0, 0, 0, 46},
	{L"Sacred Feathers", "dr9", "Druid Pelt", 2, 2, 0, 0, 0, 2, 0, 0, 0, 50},
	{L"Totemic Mask", "dra", "Druid Pelt", 2, 2, 0, 0, 0, 2, 0, 0, 0, 55},
	{L"Jawbone Visor", "ba6", "Barbarian Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 33},
	{L"Lion Helm", "ba7", "Barbarian Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 38},
	{L"Rage Mask", "ba8", "Barbarian Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 44},
	{L"Savage Helmet", "ba9", "Barbarian Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 49},
	{L"Slayer Guard", "baa", "Barbarian Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 54},
	{L"Akaran Targe", "pa6", "Paladin Shield", 2, 2, 0, 0, 0, 2, 0, 0, 0, 35},
	{L"Akaran Rondache", "pa7", "Paladin Shield", 2, 2, 0, 0, 0, 2, 0, 0, 0, 40},
	{L"Protector Shield", "pa8", "Paladin Shield", 2, 4, 0, 0, 0, 2, 0, 0, 0, 46},
	{L"Gilded Shield", "pa9", "Paladin Shield", 2, 4, 0, 0, 0, 2, 0, 0, 0, 51},
	{L"Royal Shield", "paa", "Paladin Shield", 2, 2, 0, 0, 0, 2, 0, 0, 0, 55},
	{L"Mummified Trophy", "ne6", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 2, 0, 0, 0, 33},
	{L"Fetish Trophy", "ne7", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 2, 0, 0, 0, 39},
	{L"Sexton Trophy", "ne8", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 2, 0, 0, 0, 45},
	{L"Cantor Trophy", "ne9", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 2, 0, 0, 0, 49},
	{L"Hierophant Trophy", "nea", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 2, 0, 0, 0, 54},
	{L"Blood Spirit", "drb", "Druid Pelt", 2, 2, 0, 0, 0, 3, 0, 0, 0, 62},
	{L"Sun Spirit", "drc", "Druid Pelt", 2, 2, 0, 0, 0, 3, 0, 0, 0, 69},
	{L"Earth Spirit", "drd", "Druid Pelt", 2, 2, 0, 0, 0, 3, 0, 0, 0, 76},
	{L"Sky Spirit", "dre", "Druid Pelt", 2, 2, 0, 0, 0, 3, 0, 0, 0, 83},
	{L"Dream Spirit", "drf", "Druid Pelt", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Carnage Helm", "bab", "Barbarian Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 60},
	{L"Fury Visor", "bac", "Barbarian Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 66},
	{L"Destroyer Helm", "bad", "Barbarian Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 73},
	{L"Conqueror Crown", "bae", "Barbarian Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 80},
	{L"Guardian Crown", "baf", "Barbarian Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Sacred Targe", "pab", "Paladin Shield", 2, 2, 0, 0, 0, 3, 0, 0, 0, 63},
	{L"Sacred Rondache", "pac", "Paladin Shield", 2, 2, 0, 0, 0, 3, 0, 0, 0, 70},
	{L"Kurast Shield", "pad", "Paladin Shield", 2, 4, 0, 0, 0, 3, 0, 0, 0, 74},
	{L"Zakarum Shield", "pae", "Paladin Shield", 2, 4, 0, 0, 0, 3, 0, 0, 0, 82},
	{L"Vortex Shield", "paf", "Paladin Shield", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Minion Skull", "neb", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 3, 0, 0, 0, 59},
	{L"Hellspawn Skull", "neg", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 3, 0, 0, 0, 67},
	{L"Overseer Skull", "ned", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 3, 0, 0, 0, 66},
	{L"Succubus Skull", "nee", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 3, 0, 0, 0, 81},
	{L"Bloodlord Skull", "nef", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Elixir", "elx", "Elixir", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"hpo", "hpo", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"mpo", "mpo", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"hpf", "hpf", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"mpf", "mpf", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Stamina Potion", "vps", "Stamina Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Antidote Potion", "yps", "Antidote Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Rejuvenation Potion", "rvs", "Rejuvenation Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Full Rejuvenation Potion", "rvl", "Rejuvenation Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Thawing Potion", "wms", "Thawing Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Tome of Town Portal", "tbk", "Tome", 1, 2, 1, 1, 0, 0, 0, 0, 0, 0},
	{L"Tome of Identify", "ibk", "Tome", 1, 2, 1, 1, 0, 0, 0, 0, 0, 0},
	{L"Amulet", "amu", "Amulet", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Top of the Horadric Staff", "vip", "Amulet", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Ring", "rin", "Ring", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Gold", "gld", "Gold", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{L"Scroll of Inifuss", "bks", "Quest Item", 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Key to the Cairn Stones", "bkd", "Quest Item", 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Arrows", "aqv", "Arrows", 1, 3, 1, 0, 0, 0, 0, 0, 0, 0},
	{L"Torch", "tch", "Torch", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Bolts", "cqv", "Bolts", 1, 3, 1, 0, 0, 0, 0, 0, 0, 0},
	{L"Scroll of Town Portal", "tsc", "Scroll", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Scroll of Identify", "isc", "Scroll", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Heart", "hrt", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Brain", "brz", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Jawbone", "jaw", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Eye", "eyz", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Horn", "hrn", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Tail", "tal", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Flag", "flg", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Fang", "fng", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Quill", "qll", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Soul", "sol", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Scalp", "scz", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Spleen", "spe", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Key", "key", "Key", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{L"The Black Tower Key", "luv", "Key", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Potion of Life", "xyz", "Quest Item", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"A Jade Figurine", "j34", "Quest Item", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"The Golden Bird", "g34", "Quest Item", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Lam Esen's Tome", "bbb", "Quest Item", 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Horadric Cube", "box", "Quest Item", 2, 2, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Horadric Scroll", "tr1", "Quest Item", 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Mephisto's Soulstone", "mss", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Book of Skill", "ass", "Quest Item", 2, 2, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Khalim's Eye", "qey", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Khalim's Heart", "qhr", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Khalim's Brain", "qbr", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Ear", "ear", "Ear", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Chipped Amethyst", "gcv", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 33, 0},
	{L"Flawed Amethyst", "gfv", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 34, 0},
	{L"Amethyst", "gsv", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 36, 0},
	{L"Flawless Amethyst", "gzv", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 40, 0},
	{L"Perfect Amethyst", "gpv", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 48, 0},
	{L"Chipped Topaz", "gcy", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 1025, 0},
	{L"Flawed Topaz", "gfy", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 1026, 0},
	{L"Topaz", "gsy", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 1028, 0},
	{L"Flawless Topaz", "gly", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 1032, 0},
	{L"Perfect Topaz", "gpy", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 1040, 0},
	{L"Chipped Sapphire", "gcb", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 513, 0},
	{L"Flawed Sapphire", "gfb", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 514, 0},
	{L"Sapphire", "gsb", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 516, 0},
	{L"Flawless Sapphire", "glb", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 520, 0},
	{L"Perfect Sapphire", "gpb", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 528, 0},
	{L"Chipped Emerald", "gcg", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 129, 0},
	{L"Flawed Emerald", "gfg", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 130, 0},
	{L"Emerald", "gsg", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 132, 0},
	{L"Flawless Emerald", "glg", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 136, 0},
	{L"Perfect Emerald", "gpg", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 144, 0},
	{L"Chipped Ruby", "gcr", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 257, 0},
	{L"Flawed Ruby", "gfr", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 258, 0},
	{L"Ruby", "gsr", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 260, 0},
	{L"Flawless Ruby", "glr", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 264, 0},
	{L"Perfect Ruby", "gpr", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 272, 0},
	{L"Chipped Diamond", "gcw", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 65, 0},
	{L"Flawed Diamond", "gfw", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 66, 0},
	{L"Diamond", "gsw", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 68, 0},
	{L"Flawless Diamond", "glw", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 72, 0},
	{L"Perfect Diamond", "gpw", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 80, 0},
	{L"Minor Healing Potion", "hp1", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Light Healing Potion", "hp2", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Healing Potion", "hp3", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Greater Healing Potion", "hp4", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Super Healing Potion", "hp5", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Minor Mana Potion", "mp1", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Light Mana Potion", "mp2", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Mana Potion", "mp3", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Greater Mana Potion", "mp4", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Super Mana Potion", "mp5", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Chipped Skull", "skc", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 2049, 0},
	{L"Flawed Skull", "skf", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 2050, 0},
	{L"Skull", "sku", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 2052, 0},
	{L"Flawless Skull", "skl", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 2056, 0},
	{L"Perfect Skull", "skz", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 2064, 0},
	{L"Herb", "hrb", "Herb", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Small Charm", "cm1", "Small Charm", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Large Charm", "cm2", "Large Charm", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Grand Charm", "cm3", "Grand Charm", 1, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"rps", "rps", "Health Potion", 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{L"rpl", "rpl", "Health Potion", 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{L"bps", "bps", "Mana Potion", 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{L"bpl", "bpl", "Mana Potion", 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{L"El Rune", "r01", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Eld Rune", "r02", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Tir Rune", "r03", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Nef Rune", "r04", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Eth Rune", "r05", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Ith Rune", "r06", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Tal Rune", "r07", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Ral Rune", "r08", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Ort Rune", "r09", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Thul Rune", "r10", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Amn Rune", "r11", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Sol Rune", "r12", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Shael Rune", "r13", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Dol Rune", "r14", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Hel Rune", "r15", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Io Rune", "r16", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Lum Rune", "r17", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Ko Rune", "r18", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Fal Rune", "r19", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Lem Rune", "r20", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Pul Rune", "r21", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Um Rune", "r22", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Mal Rune", "r23", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Ist Rune", "r24", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Gul Rune", "r25", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Vex Rune", "r26", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Ohm Rune", "r27", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Lo Rune", "r28", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Sur Rune", "r29", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Ber Rune", "r30", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Jah Rune", "r31", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Cham Rune", "r32", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Zod Rune", "r33", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{L"Jewel", "jew", "Jewel", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Malah's Potion", "ice", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Scroll of Knowledge", "0sc", "Scroll", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Scroll of Resistance", "tr2", "Quest Item", 2, 2, 0, 1, 0, 0, 0, 0, 0, 0},
	{L"Key of Terror", "pk1", "Quest Item", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Key of Hate", "pk2", "Quest Item", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Key of Destruction", "pk3", "Quest Item", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Twisted Essence of Suffering", "tes", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Charged Essence of Hatred", "ceh", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Burning Essence of Terror", "bet", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Festering Essence of Destruction", "fed", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Token of Absolution", "toa", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Diablo's Horn", "dhn", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Baal's Eye", "bey", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Mephisto's Brain", "mbr", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Standard of Heroes", "std", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Hand Axe", "hax", "Axe", 1, 3, 0, 0, 0, 1, 0, 0, 0, 3},
	{L"Axe", "axe", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 7},
	{L"Double Axe", "2ax", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 13},
	{L"Military Pick", "mpi", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 19},
	{L"War Axe", "wax", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 25},
	{L"Large Axe", "lax", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 6},
	{L"Broad Axe", "bax", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 12},
	{L"Battle Axe", "btx", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 17},
	{L"Great Axe", "gax", "Axe", 2, 4, 0, 0, 0, 1, 0, 0, 0, 23},
	{L"Giant Axe", "gix", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 27},
	{L"Wand", "wnd", "Wand", 1, 2, 0, 0, 0, 1, 0, 0, 0, 2},
	{L"Yew Wand", "ywn", "Wand", 1, 2, 0, 0, 0, 1, 0, 0, 0, 12},
	{L"Bone Wand", "bwn", "Wand", 1, 2, 0, 0, 0, 1, 0, 0, 0, 18},
	{L"Grim Wand", "gwn", "Wand", 1, 2, 0, 0, 0, 1, 0, 0, 0, 26},
	{L"Club", "clb", "Club", 1, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{L"Scepter", "scp", "Scepter", 1, 3, 0, 0, 0, 1, 0, 0, 0, 3},
	{L"Grand Scepter", "gsc", "Scepter", 1, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{L"War Scepter", "wsp", "Scepter", 2, 3, 0, 0, 0, 1, 0, 0, 0, 21},
	{L"Spiked Club", "spc", "Club", 1, 3, 0, 0, 0, 1, 0, 0, 0, 4},
	{L"Mace", "mac", "Mace", 1, 3, 0, 0, 0, 1, 0, 0, 0, 8},
	{L"Morning Star", "mst", "Mace", 1, 3, 0, 0, 0, 1, 0, 0, 0, 13},
	{L"Flail", "fla", "Mace", 2, 3, 0, 0, 0, 1, 0, 0, 0, 19},
	{L"War Hammer", "whm", "Hammer", 2, 3, 0, 0, 0, 1, 0, 0, 0, 25},
	{L"Maul", "mau", "Hammer", 2, 4, 0, 0, 0, 1, 0, 0, 0, 21},
	{L"Great Maul", "gma", "Hammer", 2, 3, 0, 0, 0, 1, 0, 0, 0, 32},
	{L"Short Sword", "ssd", "Sword", 1, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{L"Scimitar", "scm", "Sword", 1, 3, 0, 0, 0, 1, 0, 0, 0, 5},
	{L"Sabre", "sbr", "Sword", 1, 3, 0, 0, 0, 1, 0, 0, 0, 8},
	{L"Falchion", "flc", "Sword", 1, 3, 0, 0, 0, 1, 0, 0, 0, 11},
	{L"Crystal Sword", "crs", "Sword", 2, 3, 0, 0, 0, 1, 0, 0, 0, 11},
	{L"Broad Sword", "bsd", "Sword", 2, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{L"Long Sword", "lsd", "Sword", 2, 3, 0, 0, 0, 1, 0, 0, 0, 20},
	{L"War Sword", "wsd", "Sword", 1, 3, 0, 0, 0, 1, 0, 0, 0, 27},
	{L"Two-Handed Sword", "2hs", "Sword", 1, 4, 0, 0, 0, 1, 0, 0, 0, 10},
	{L"Claymore", "clm", "Sword", 1, 4, 0, 0, 0, 1, 0, 0, 0, 17},
	{L"Giant Sword", "gis", "Sword", 1, 4, 0, 0, 0, 1, 0, 0, 0, 21},
	{L"Bastard Sword", "bsw", "Sword", 1, 4, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Flamberge", "flb", "Sword", 2, 4, 0, 0, 0, 1, 0, 0, 0, 27},
	{L"Great Sword", "gsd", "Sword", 2, 4, 0, 0, 0, 1, 0, 0, 0, 33},
	{L"Dagger", "dgr", "Dagger", 1, 2, 0, 0, 0, 1, 0, 0, 0, 3},
	{L"Dirk", "dir", "Dagger", 1, 2, 0, 0, 0, 1, 0, 0, 0, 9},
	{L"Kris", "kri", "Dagger", 1, 3, 0, 0, 0, 1, 0, 0, 0, 17},
	{L"Blade", "bld", "Dagger", 1, 3, 0, 0, 0, 1, 0, 0, 0, 23},
	{L"Throwing Knife", "tkf", "Throwing Knife", 1, 2, 1, 0, 1, 1, 0, 0, 0, 2},
	{L"Throwing Axe", "tax", "Throwing Axe", 1, 2, 1, 0, 1, 1, 0, 0, 0, 7},
	{L"Balanced Knife", "bkf", "Throwing Knife", 1, 2, 1, 0, 1, 1, 0, 0, 0, 13},
	{L"Balanced Axe", "bal", "Throwing Axe", 2, 3, 1, 0, 1, 1, 0, 0, 0, 7},
	{L"Javelin", "jav", "Javelin", 1, 3, 1, 0, 1, 1, 0, 0, 0, 1},
	{L"Pilum", "pil", "Javelin", 1, 3, 1, 0, 1, 1, 0, 0, 0, 10},
	{L"Short Spear", "ssp", "Javelin", 1, 3, 1, 0, 1, 1, 0, 0, 0, 15},
	{L"Glaive", "glv", "Javelin", 1, 4, 1, 0, 1, 1, 0, 0, 0, 23},
	{L"Throwing Spear", "tsp", "Javelin", 1, 4, 1, 0, 1, 1, 0, 0, 0, 29},
	{L"Spear", "spr", "Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 5},
	{L"Trident", "tri", "Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 9},
	{L"Brandistock", "brn", "Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 16},
	{L"Spetum", "spt", "Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 20},
	{L"Pike", "pik", "Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Bardiche", "bar", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 5},
	{L"Voulge", "vou", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 11},
	{L"Scythe", "scy", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 15},
	{L"Poleaxe", "pax", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 21},
	{L"Halberd", "hal", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 29},
	{L"War Scythe", "wsc", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 34},
	{L"Short Staff", "sst", "Staff", 1, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{L"Long Staff", "lst", "Staff", 1, 4, 0, 0, 0, 1, 0, 0, 0, 8},
	{L"Gnarled Staff", "cst", "Staff", 1, 4, 0, 0, 0, 1, 0, 0, 0, 12},
	{L"Battle Staff", "bst", "Staff", 1, 4, 0, 0, 0, 1, 0, 0, 0, 17},
	{L"War Staff", "wst", "Staff", 2, 4, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Short Bow", "sbw", "Bow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{L"Hunter's Bow", "hbw", "Bow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 5},
	{L"Long Bow", "lbw", "Bow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 8},
	{L"Composite Bow", "cbw", "Bow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 12},
	{L"Short Battle Bow", "sbb", "Bow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 18},
	{L"Long Battle Bow", "lbb", "Bow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 23},
	{L"Short War Bow", "swb", "Bow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 27},
	{L"Long War Bow", "lwb", "Bow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 31},
	{L"Light Crossbow", "lxb", "Crossbow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 6},
	{L"Crossbow", "mxb", "Crossbow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{L"Heavy Crossbow", "hxb", "Crossbow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Repeating Crossbow", "rxb", "Crossbow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 33},
	{L"Rancid Gas Potion", "gps", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{L"Oil Potion", "ops", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{L"Choking Gas Potion", "gpm", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{L"Exploding Potion", "opm", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{L"Strangling Gas Potion", "gpl", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{L"Fulminating Potion", "opl", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{L"Decoy Gidbinn", "d33", "Dagger", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"The Gidbinn", "g33", "Dagger", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Wirt's Leg", "leg", "Club", 1, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Horadric Malus", "hdm", "Hammer", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Hell Forge Hammer", "hfh", "Hammer", 2, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Horadric Staff", "hst", "Staff", 1, 4, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Shaft of the Horadric Staff", "msf", "Staff", 1, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Hatchet", "9ha", "Axe", 1, 3, 0, 0, 0, 2, 0, 0, 0, 31},
	{L"Cleaver", "9ax", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 34},
	{L"Twin Axe", "92a", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 39},
	{L"Crowbill", "9mp", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 43},
	{L"Naga", "9wa", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 48},
	{L"Military Axe", "9la", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 34},
	{L"Bearded Axe", "9ba", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 38},
	{L"Tabar", "9bt", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 42},
	{L"Gothic Axe", "9ga", "Axe", 2, 4, 0, 0, 0, 2, 0, 0, 0, 46},
	{L"Ancient Axe", "9gi", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 51},
	{L"Burnt Wand", "9wn", "Wand", 1, 2, 0, 0, 0, 2, 0, 0, 0, 31},
	{L"Petrified Wand", "9yw", "Wand", 1, 2, 0, 0, 0, 2, 0, 0, 0, 38},
	{L"Tomb Wand", "9bw", "Wand", 1, 2, 0, 0, 0, 2, 0, 0, 0, 43},
	{L"Grave Wand", "9gw", "Wand", 1, 2, 0, 0, 0, 2, 0, 0, 0, 49},
	{L"Cudgel", "9cl", "Club", 1, 3, 0, 0, 0, 2, 0, 0, 0, 30},
	{L"Rune Scepter", "9sc", "Scepter", 1, 3, 0, 0, 0, 2, 0, 0, 0, 31},
	{L"Holy Water Sprinkler", "9qs", "Scepter", 1, 3, 0, 0, 0, 2, 0, 0, 0, 4},
	{L"Divine Scepter", "9ws", "Scepter", 2, 3, 0, 0, 0, 2, 0, 0, 0, 45},
	{L"Barbed Club", "9sp", "Club", 1, 3, 0, 0, 0, 2, 0, 0, 0, 32},
	{L"Flanged Mace", "9ma", "Mace", 1, 3, 0, 0, 0, 2, 0, 0, 0, 35},
	{L"Jagged Star", "9mt", "Mace", 1, 3, 0, 0, 0, 2, 0, 0, 0, 39},
	{L"Knout", "9fl", "Mace", 2, 3, 0, 0, 0, 2, 0, 0, 0, 43},
	{L"Battle Hammer", "9wh", "Hammer", 2, 3, 0, 0, 0, 2, 0, 0, 0, 48},
	{L"War Club", "9m9", "Hammer", 2, 4, 0, 0, 0, 2, 0, 0, 0, 45},
	{L"Martel de Fer", "9gm", "Hammer", 2, 3, 0, 0, 0, 2, 0, 0, 0, 53},
	{L"Gladius", "9ss", "Sword", 1, 3, 0, 0, 0, 2, 0, 0, 0, 30},
	{L"Cutlass", "9sm", "Sword", 1, 3, 0, 0, 0, 2, 0, 0, 0, 43},
	{L"Shamshir", "9sb", "Sword", 1, 3, 0, 0, 0, 2, 0, 0, 0, 35},
	{L"Tulwar", "9fc", "Sword", 1, 3, 0, 0, 0, 2, 0, 0, 0, 37},
	{L"Dimensional Blade", "9cr", "Sword", 2, 3, 0, 0, 0, 2, 0, 0, 0, 37},
	{L"Battle Sword", "9bs", "Sword", 2, 3, 0, 0, 0, 2, 0, 0, 0, 40},
	{L"Rune Sword", "9ls", "Sword", 2, 3, 0, 0, 0, 2, 0, 0, 0, 44},
	{L"Ancient Sword", "9wd", "Sword", 1, 3, 0, 0, 0, 2, 0, 0, 0, 49},
	{L"Espandon", "92h", "Sword", 1, 4, 0, 0, 0, 2, 0, 0, 0, 37},
	{L"Dacian Falx", "9cm", "Sword", 1, 4, 0, 0, 0, 2, 0, 0, 0, 42},
	{L"Tusk Sword", "9gs", "Sword", 1, 4, 0, 0, 0, 2, 0, 0, 0, 45},
	{L"Gothic Sword", "9b9", "Sword", 1, 4, 0, 0, 0, 2, 0, 0, 0, 48},
	{L"Zweihander", "9fb", "Sword", 2, 4, 0, 0, 0, 2, 0, 0, 0, 49},
	{L"Executioner Sword", "9gd", "Sword", 2, 4, 0, 0, 0, 2, 0, 0, 0, 54},
	{L"Poignard", "9dg", "Dagger", 1, 2, 0, 0, 0, 2, 0, 0, 0, 31},
	{L"Rondel", "9di", "Dagger", 1, 2, 0, 0, 0, 2, 0, 0, 0, 36},
	{L"Cinquedeas", "9kr", "Dagger", 1, 3, 0, 0, 0, 2, 0, 0, 0, 42},
	{L"Stiletto", "9bl", "Dagger", 1, 3, 0, 0, 0, 2, 0, 0, 0, 46},
	{L"Battle Dart", "9tk", "Throwing Knife", 1, 2, 1, 0, 1, 2, 0, 0, 0, 31},
	{L"Francisca", "9ta", "Throwing Axe", 1, 2, 1, 0, 1, 2, 0, 0, 0, 34},
	{L"War Dart", "9bk", "Throwing Knife", 1, 2, 1, 0, 1, 2, 0, 0, 0, 39},
	{L"Hurlbat", "9b8", "Throwing Axe", 2, 3, 1, 0, 1, 2, 0, 0, 0, 41},
	{L"War Javelin", "9ja", "Javelin", 1, 3, 1, 0, 1, 2, 0, 0, 0, 30},
	{L"Great Pilum", "9pi", "Javelin", 1, 3, 1, 0, 1, 2, 0, 0, 0, 37},
	{L"Simbilan", "9s9", "Javelin", 1, 3, 1, 0, 1, 2, 0, 0, 0, 40},
	{L"Spiculum", "9gl", "Javelin", 1, 4, 1, 0, 1, 2, 0, 0, 0, 46},
	{L"Harpoon", "9ts", "Javelin", 1, 4, 1, 0, 1, 2, 0, 0, 0, 51},
	{L"War Spear", "9sr", "Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 33},
	{L"Fuscina", "9tr", "Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 36},
	{L"War Fork", "9br", "Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 41},
	{L"Yari", "9st", "Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 44},
	{L"Lance", "9p9", "Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 47},
	{L"Lochaber Axe", "9b7", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 33},
	{L"Bill", "9vo", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 37},
	{L"Battle Scythe", "9s8", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 40},
	{L"Partizan", "9pa", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 35},
	{L"Bec-de-Corbin", "9h9", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 51},
	{L"Grim Scythe", "9wc", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 55},
	{L"Jo Staff", "8ss", "Staff", 1, 3, 0, 0, 0, 2, 0, 0, 0, 30},
	{L"Quarterstaff", "8ls", "Staff", 1, 4, 0, 0, 0, 2, 0, 0, 0, 35},
	{L"Cedar Staff", "8cs", "Staff", 1, 4, 0, 0, 0, 2, 0, 0, 0, 38},
	{L"Gothic Staff", "8bs", "Staff", 1, 4, 0, 0, 0, 2, 0, 0, 0, 42},
	{L"Rune Staff", "8ws", "Staff", 2, 4, 0, 0, 0, 2, 0, 0, 0, 47},
	{L"Edge Bow", "8sb", "Bow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 30},
	{L"Razor Bow", "8hb", "Bow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 33},
	{L"Cedar Bow", "8lb", "Bow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 35},
	{L"Double Bow", "8cb", "Bow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 39},
	{L"Short Siege Bow", "8s8", "Bow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 43},
	{L"Large Siege Bow", "8l8", "Bow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 46},
	{L"Rune Bow", "8sw", "Bow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 49},
	{L"Gothic Bow", "8lw", "Bow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 52},
	{L"Arbalest", "8lx", "Crossbow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 34},
	{L"Siege Crossbow", "8mx", "Crossbow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 40},
	{L"Ballista", "8hx", "Crossbow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 47},
	{L"Chu-Ko-Nu", "8rx", "Crossbow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 54},
	{L"Khalim's Flail", "qf1", "Mace", 2, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Khalim's Will", "qf2", "Mace", 2, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{L"Katar", "ktr", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{L"Wrist Blade", "wrb", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 9},
	{L"Hatchet Hands", "axf", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 12},
	{L"Cestus", "ces", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{L"Claws", "clw", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 18},
	{L"Blade Talons", "btl", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 21},
	{L"Scissors Katar", "skr", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Quhab", "9ar", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 28},
	{L"Wrist Spike", "9wb", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 32},
	{L"Fascia", "9xf", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 36},
	{L"Hand Scythe", "9cs", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 41},
	{L"Greater Claws", "9lw", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 45},
	{L"Greater Talons", "9tw", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 50},
	{L"Scissors Quhab", "9qr", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 54},
	{L"Suwayyah", "7ar", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 59},
	{L"Wrist Sword", "7wb", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 62},
	{L"War Fist", "7xf", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 68},
	{L"Battle Cestus", "7cs", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 73},
	{L"Feral Claws", "7lw", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 78},
	{L"Runic Talons", "7tw", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 81},
	{L"Scissors Suwayyah", "7qr", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Tomahawk", "7ha", "Axe", 1, 3, 0, 0, 0, 3, 0, 0, 0, 54},
	{L"Small Crescent", "7ax", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 61},
	{L"Ettin Axe", "72a", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 70},
	{L"War Spike", "7mp", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 79},
	{L"Berserker Axe", "7wa", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Feral Axe", "7la", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 57},
	{L"Silver-edged Axe", "7ba", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 65},
	{L"Decapitator", "7bt", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 73},
	{L"Champion Axe", "7ga", "Axe", 2, 4, 0, 0, 0, 3, 0, 0, 0, 82},
	{L"Glorious Axe", "7gi", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Polished Wand", "7wn", "Wand", 1, 2, 0, 0, 0, 3, 0, 0, 0, 55},
	{L"Ghost Wand", "7yw", "Wand", 1, 2, 0, 0, 0, 3, 0, 0, 0, 65},
	{L"Lich Wand", "7bw", "Wand", 1, 2, 0, 0, 0, 3, 0, 0, 0, 75},
	{L"Unearthed Wand", "7gw", "Wand", 1, 2, 0, 0, 0, 3, 0, 0, 0, 86},
	{L"Truncheon", "7cl", "Club", 1, 3, 0, 0, 0, 3, 0, 0, 0, 52},
	{L"Mighty Scepter", "7sc", "Scepter", 1, 3, 0, 0, 0, 3, 0, 0, 0, 62},
	{L"Seraph Rod", "7qs", "Scepter", 1, 3, 0, 0, 0, 3, 0, 0, 0, 76},
	{L"Caduceus", "7ws", "Scepter", 2, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Tyrant Club", "7sp", "Club", 1, 3, 0, 0, 0, 3, 0, 0, 0, 57},
	{L"Reinforced Mace", "7ma", "Mace", 1, 3, 0, 0, 0, 3, 0, 0, 0, 63},
	{L"Devil Star", "7mt", "Mace", 1, 3, 0, 0, 0, 3, 0, 0, 0, 70},
	{L"Scourge", "7fl", "Mace", 2, 3, 0, 0, 0, 3, 0, 0, 0, 76},
	{L"Legendary Mallet", "7wh", "Hammer", 2, 3, 0, 0, 0, 3, 0, 0, 0, 82},
	{L"Ogre Maul", "7m7", "Hammer", 2, 4, 0, 0, 0, 3, 0, 0, 0, 69},
	{L"Thunder Maul", "7gm", "Hammer", 2, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Falcata", "7ss", "Sword", 1, 3, 0, 0, 0, 3, 0, 0, 0, 56},
	{L"Ataghan", "7sm", "Sword", 1, 3, 0, 0, 0, 3, 0, 0, 0, 61},
	{L"Elegant Blade", "7sb", "Sword", 1, 3, 0, 0, 0, 3, 0, 0, 0, 63},
	{L"Hydra Edge", "7fc", "Sword", 1, 3, 0, 0, 0, 3, 0, 0, 0, 69},
	{L"Phase Blade", "7cr", "Sword", 2, 3, 0, 0, 0, 3, 0, 0, 0, 73},
	{L"Conquest Sword", "7bs", "Sword", 2, 3, 0, 0, 0, 3, 0, 0, 0, 78},
	{L"Cryptic Sword", "7ls", "Sword", 2, 3, 0, 0, 0, 3, 0, 0, 0, 82},
	{L"Mythical Sword", "7wd", "Sword", 1, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Legend Sword", "72h", "Sword", 1, 4, 0, 0, 0, 3, 0, 0, 0, 59},
	{L"Highland Blade", "7cm", "Sword", 1, 4, 0, 0, 0, 3, 0, 0, 0, 66},
	{L"Balrog Blade", "7gs", "Sword", 1, 4, 0, 0, 0, 3, 0, 0, 0, 71},
	{L"Champion Sword", "7b7", "Sword", 1, 4, 0, 0, 0, 3, 0, 0, 0, 77},
	{L"Colossus Sword", "7fb", "Sword", 2, 4, 0, 0, 0, 3, 0, 0, 0, 80},
	{L"Colossus Blade", "7gd", "Sword", 2, 4, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Bone Knife", "7dg", "Dagger", 1, 2, 0, 0, 0, 3, 0, 0, 0, 58},
	{L"Mithril Point", "7di", "Dagger", 1, 2, 0, 0, 0, 3, 0, 0, 0, 70},
	{L"Fanged Knife", "7kr", "Dagger", 1, 3, 0, 0, 0, 3, 0, 0, 0, 83},
	{L"Legend Spike", "7bl", "Dagger", 1, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Flying Knife", "7tk", "Throwing Knife", 1, 2, 1, 0, 1, 3, 0, 0, 0, 64},
	{L"Flying Axe", "7ta", "Throwing Axe", 1, 2, 1, 0, 1, 3, 0, 0, 0, 56},
	{L"Winged Knife", "7bk", "Throwing Knife", 1, 2, 1, 0, 1, 3, 0, 0, 0, 77},
	{L"Winged Axe", "7b8", "Throwing Axe", 2, 3, 1, 0, 1, 3, 0, 0, 0, 80},
	{L"Hyperion Javelin", "7ja", "Javelin", 1, 3, 1, 0, 1, 3, 0, 0, 0, 54},
	{L"Stygian Pilum", "7pi", "Javelin", 1, 3, 1, 0, 1, 3, 0, 0, 0, 62},
	{L"Balrog Spear", "7s7", "Javelin", 1, 3, 1, 0, 1, 3, 0, 0, 0, 71},
	{L"Ghost Glaive", "7gl", "Javelin", 1, 4, 1, 0, 1, 3, 0, 0, 0, 79},
	{L"Winged Harpoon", "7ts", "Javelin", 1, 4, 1, 0, 1, 3, 0, 0, 0, 85},
	{L"Hyperion Spear", "7sr", "Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 58},
	{L"Stygian Pike", "7tr", "Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 66},
	{L"Mancatcher", "7br", "Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 74},
	{L"Ghost Spear", "7st", "Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 83},
	{L"War Pike", "7p7", "Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Ogre Axe", "7o7", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 60},
	{L"Colossus Voulge", "7vo", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 64},
	{L"Thresher", "7s8", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 71},
	{L"Cryptic Axe", "7pa", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 79},
	{L"Great Poleaxe", "7h7", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 84},
	{L"Giant Thresher", "7wc", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Walking Stick", "6ss", "Staff", 1, 3, 0, 0, 0, 3, 0, 0, 0, 58},
	{L"Stalagmite", "6ls", "Staff", 1, 4, 0, 0, 0, 3, 0, 0, 0, 66},
	{L"Elder Staff", "6cs", "Staff", 1, 4, 0, 0, 0, 3, 0, 0, 0, 74},
	{L"Shillelagh", "6bs", "Staff", 1, 4, 0, 0, 0, 3, 0, 0, 0, 83},
	{L"Archon Staff", "6ws", "Staff", 2, 4, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Spider Bow", "6sb", "Bow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 55},
	{L"Blade Bow", "6hb", "Bow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 60},
	{L"Shadow Bow", "6lb", "Bow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 63},
	{L"Great Bow", "6cb", "Bow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 68},
	{L"Diamond Bow", "6s7", "Bow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 72},
	{L"Crusader Bow", "6l7", "Bow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 77},
	{L"Ward Bow", "6sw", "Bow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 80},
	{L"Hydra Bow", "6lw", "Bow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Pellet Bow", "6lx", "Crossbow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 57},
	{L"Gorgon Crossbow", "6mx", "Crossbow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 67},
	{L"Colossus Crossbow", "6hx", "Crossbow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 75},
	{L"Demon Crossbow", "6rx", "Crossbow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 84},
	{L"Eagle Orb", "ob1", "Sorceress Orb", 1, 2, 0, 0, 0, 1, 0, 0, 0, 1},
	{L"Sacred Globe", "ob2", "Sorceress Orb", 1, 2, 0, 0, 0, 1, 0, 0, 0, 8},
	{L"Smoked Sphere", "ob3", "Sorceress Orb", 1, 2, 0, 0, 0, 1, 0, 0, 0, 12},
	{L"Clasped Orb", "ob4", "Sorceress Orb", 1, 2, 0, 0, 0, 1, 0, 0, 0, 17},
	{L"Jared's Stone", "ob5", "Sorceress Orb", 1, 3, 0, 0, 0, 1, 0, 0, 0, 24},
	{L"Stag Bow", "am1", "Amazon Bow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 18},
	{L"Reflex Bow", "am2", "Amazon Bow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 27},
	{L"Maiden Spear", "am3", "Amazon Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 18},
	{L"Maiden Pike", "am4", "Amazon Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 27},
	{L"Maiden Javelin", "am5", "Amazon Javelin", 1, 3, 1, 0, 1, 1, 0, 0, 0, 23},
	{L"Glowing Orb", "ob6", "Sorceress Orb", 1, 2, 0, 0, 0, 2, 0, 0, 0, 32},
	{L"Crystalline Globe", "ob7", "Sorceress Orb", 1, 2, 0, 0, 0, 2, 0, 0, 0, 37},
	{L"Cloudy Sphere", "ob8", "Sorceress Orb", 1, 2, 0, 0, 0, 2, 0, 0, 0, 41},
	{L"Sparkling Ball", "ob9", "Sorceress Orb", 1, 2, 0, 0, 0, 2, 0, 0, 0, 46},
	{L"Swirling Crystal", "oba", "Sorceress Orb", 1, 3, 0, 0, 0, 2, 0, 0, 0, 50},
	{L"Ashwood Bow", "am6", "Amazon Bow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 39},
	{L"Ceremonial Bow", "am7", "Amazon Bow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 47},
	{L"Ceremonial Spear", "am8", "Amazon Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 43},
	{L"Ceremonial Pike", "am9", "Amazon Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 51},
	{L"Ceremonial Javelin", "ama", "Amazon Javelin", 1, 3, 1, 0, 1, 2, 0, 0, 0, 35},
	{L"Heavenly Stone", "obb", "Sorceress Orb", 1, 2, 0, 0, 0, 3, 0, 0, 0, 59},
	{L"Eldritch Orb", "obc", "Sorceress Orb", 1, 2, 0, 0, 0, 3, 0, 0, 0, 67},
	{L"Demon Heart", "obd", "Sorceress Orb", 1, 2, 0, 0, 0, 3, 0, 0, 0, 75},
	{L"Vortex Orb", "obe", "Sorceress Orb", 1, 2, 0, 0, 0, 3, 0, 0, 0, 84},
	{L"Dimensional Shard", "obf", "Sorceress Orb", 1, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{L"Matriarchal Bow", "amb", "Amazon Bow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 53},
	{L"Grand Matron Bow", "amc", "Amazon Bow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 78},
	{L"Matriarchal Spear", "amd", "Amazon Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 61},
	{L"Matriarchal Pike", "ame", "Amazon Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 81},
	{L"Matriarchal Javelin", "amf", "Amazon Javelin", 1, 3, 1, 0, 1, 3, 0, 0, 0, 65}
};

StatProperties StatPropertiesList[] = {
	{"Strength", 8, 0, 32},
	{"Energy", 7, 0, 32},
	{"Dexterity", 7, 0, 32},
	{"Vitality", 7, 0, 32},
	{"All Attributes", 0, 0, 0},
	{"New Skills", 0, 0, 0},
	{"Life", 0, 0, 0},
	{"Maximum Life", 9, 0, 32},
	{"Mana", 0, 0, 0},
	{"Maximum Mana", 8, 0, 32},
	{"Stamina", 0, 0, 0},
	{"Maximum Stamina", 8, 0, 32},
	{"Level", 0, 0, 0},
	{"Experience", 0, 0, 0},
	{"Gold", 0, 0, 0},
	{"Bank", 0, 0, 0},
	{"Enhanced Defense", 9, 0, 0},
	{"Enhanced Maximum Damage", 9, 0, 0},
	{"Enhanced Minimum Damage", 9, 0, 0},
	{"Attack Rating", 10, 0, 0},
	{"Increased Blocking", 6, 0, 0},
	{"Minimum Damage", 6, 0, 0},
	{"Maximum Damage", 7, 0, 0},
	{"Secondary Minimum Damage", 6, 0, 0},
	{"Secondary Maximum Damage", 7, 0, 0},
	{"Enhanced Damage", 8, 0, 0},
	{"Mana Recovery", 8, 0, 0},
	{"Mana Recovery Bonus", 8, 0, 0},
	{"Stamina Recovery Bonus", 8, 0, 0},
	{"Last Experience", 0, 0, 0},
	{"Next Experience", 0, 0, 0},
	{"Defense", 11, 0, 10},
	{"Defense vs. Missiles", 9, 0, 0},
	{"Defense vs. Melee", 8, 0, 0},
	{"Damage Reduction", 6, 0, 0},
	{"Magical Damage Reduction", 6, 0, 0},
	{"Damage Reduction (Percent)", 8, 0, 0},
	{"Magical Damage Reduction (Percent)", 8, 0, 0},
	{"Maximum Magical Damage Reduction (Percent)", 5, 0, 0},
	{"Fire Resistance", 8, 0, 50},
	{"Maximum Fire Resistance", 5, 0, 0},
	{"Lightning Resistance", 8, 0, 50},
	{"Maximum Lightning Resistance", 5, 0, 0},
	{"Cold Resistance", 8, 0, 50},
	{"Maximum Cold Resistance", 5, 0, 0},
	{"Poison Resistance", 8, 0, 50},
	{"Maximum Poison Resistance", 5, 0, 0},
	{"Damage Aura", 0, 0, 0},
	{"Minimum Fire Damage", 8, 0, 0},
	{"Maximum Fire Damage", 9, 0, 0},
	{"Minimum Lightning Damage", 6, 0, 0},
	{"Maximum Lightning Damage", 10, 0, 0},
	{"Minimum Magical Damage", 8, 0, 0},
	{"Maximum Magical Damage", 9, 0, 0},
	{"Minimum Cold Damage", 8, 0, 0},
	{"Maximum Cold Damage", 9, 0, 0},
	{"Cold Damage Length", 8, 0, 0},
	{"Minimum Poison Damage", 10, 0, 0},
	{"Maximum Poison Damage", 10, 0, 0},
	{"Poison Damage length", 9, 0, 0},
	{"Minimum Life Stolen Per Hit", 7, 0, 0},
	{"Maximum Life Stolen Per Hit", 0, 0, 0},
	{"Minimum Mana Stolen Per Hit", 7, 0, 0},
	{"Maximum Mana Stolen Per Hit", 0, 0, 0},
	{"Minimum Stamina Drain", 0, 0, 0},
	{"Maximum Stamina Drain", 0, 0, 0},
	{"Stun Length", 0, 0, 0},
	{"Velocity Percent", 7, 0, 30},
	{"Attack Rate", 7, 0, 30},
	{"Other Animation Rate", 0, 0, 0},
	{"Quantity", 0, 0, 0},
	{"Value", 8, 0, 100},
	{"Durability", 9, 0, 0},
	{"Maximum Durability", 8, 0, 0},
	{"Replenish Life", 6, 0, 30},
	{"Enhanced Maximum Durability", 7, 0, 20},
	{"Enhanced Life", 6, 0, 10},
	{"Enhanced Mana", 6, 0, 10},
	{"Attacker Takes Damage", 7, 0, 0},
	{"Extra Gold", 9, 0, 100},
	{"Better Chance Of Getting Magic Item", 8, 0, 100},
	{"Knockback", 7, 0, 0},
	{"Time Duration", 9, 0, 20},
	{"Class Skills", 3, 3, 0},
	{"Unsent Parameter", 0, 0, 0},
	{"Add Experience", 9, 0, 50},
	{"Life After Each Kill", 7, 0, 0},
	{"Reduce Vendor Prices", 7, 0, 0},
	{"Double Herb Duration", 1, 0, 0},
	{"Light Radius", 4, 0, 4},
	{"Light Colour", 24, 0, 0},
	{"Reduced Requirements", 8, 0, 100},
	{"Reduced Level Requirement", 7, 0, 0},
	{"Increased Attack Speed", 7, 0, 20},
	{"Reduced Level Requirement (Percent)", 7, 0, 64},
	{"Last Block Frame", 0, 0, 0},
	{"Faster Run Walk", 7, 0, 20},
	{"Non Class Skill", 6, 9, 0},
	{"State", 1, 8, 0},
	{"Faster Hit Recovery", 7, 0, 20},
	{"Monster Player Count", 0, 0, 0},
	{"Skill Poison Override Length", 0, 0, 0},
	{"Faster Block Rate", 7, 0, 20},
	{"Skill Bypass Undead", 0, 0, 0},
	{"Skill Bypass Demons", 0, 0, 0},
	{"Faster Cast Rate", 7, 0, 20},
	{"Skill Bypass Beasts", 0, 0, 0},
	{"Single Skill", 3, 9, 0},
	{"Slain Monsters Rest In Peace", 1, 0, 0},
	{"Curse Resistance", 9, 0, 0},
	{"Poison Length Reduction", 8, 0, 20},
	{"Adds Damage", 9, 0, 20},
	{"Hit Causes Monster To Flee", 7, 0, -1},
	{"Hit Blinds Target", 7, 0, 0},
	{"Damage To Mana", 6, 0, 0},
	{"Ignore Target's Defense", 1, 0, 0},
	{"Reduce Target's Defense", 7, 0, 0},
	{"Prevent Monster Heal", 7, 0, 0},
	{"Half Freeze Duration", 1, 0, 0},
	{"To Hit Percent", 9, 0, 20},
	{"Monster Defense Reduction per Hit", 7, 0, 128},
	{"Damage To Demons", 9, 0, 20},
	{"Damage To Undead", 9, 0, 20},
	{"Attack Rating Against Demons", 10, 0, 128},
	{"Attack Rating Against Undead", 10, 0, 128},
	{"Throwable", 1, 0, 0},
	{"Elemental Skills", 3, 3, 0},
	{"All Skills", 3, 0, 0},
	{"Attacker Takes Lightning Damage", 5, 0, 0},
	{"Iron Maiden Level", 0, 0, 0},
	{"Lifetap Level", 0, 0, 0},
	{"Thorns Percent", 0, 0, 0},
	{"Bone Armor", 0, 0, 0},
	{"Maximum Bone Armor", 0, 0, 0},
	{"Freezes Target", 5, 0, 0},
	{"Open Wounds", 7, 0, 0},
	{"Crushing Blow", 7, 0, 0},
	{"Kick Damage", 7, 0, 0},
	{"Mana After Each Kill", 7, 0, 0},
	{"Life After Each Demon Kill", 7, 0, 0},
	{"Extra Blood", 7, 0, 0},
	{"Deadly Strike", 7, 0, 0},
	{"Fire Absorption (Percent)", 7, 0, 0},
	{"Fire Absorption", 7, 0, 0},
	{"Lightning Absorption (Percent)", 7, 0, 0},
	{"Lightning Absorption", 7, 0, 0},
	{"Magic Absorption (Percent)", 7, 0, 0},
	{"Magic Absorption", 7, 0, 0},
	{"Cold Absorption (Percent)", 7, 0, 0},
	{"Cold Absorption", 7, 0, 0},
	{"Slows Down Enemies", 7, 0, 0},
	{"Aura", 5, 9, 0},
	{"Indestructible", 1, 0, 0},
	{"Cannot Be Frozen", 1, 0, 0},
	{"Stamina Drain (Percent)", 7, 0, 20},
	{"Reanimate", 7, 10, 0},
	{"Piercing Attack", 7, 0, 0},
	{"Fires Magic Arrows", 7, 0, 0},
	{"Fire Explosive Arrows", 7, 0, 0},
	{"Minimum Throwing Damage", 6, 0, 0},
	{"Maximum Throwing Damage", 7, 0, 0},
	{"Skill Hand Of Athena", 0, 0, 0},
	{"Skill Stamina (Percent)", 0, 0, 0},
	{"Skill Passive Stamina (Percent)", 0, 0, 0},
	{"Concentration", 0, 0, 0},
	{"Enchant", 0, 0, 0},
	{"Pierce", 0, 0, 0},
	{"Conviction", 0, 0, 0},
	{"Chilling Armor", 0, 0, 0},
	{"Frenzy", 0, 0, 0},
	{"Decrepify", 0, 0, 0},
	{"Skill Armor Percent", 0, 0, 0},
	{"Alignment", 0, 0, 0},
	{"Target 0", 0, 0, 0},
	{"Target 1", 0, 0, 0},
	{"Gold Lost", 0, 0, 0},
	{"Conversion Level", 0, 0, 0},
	{"Conversion Maximum Life", 0, 0, 0},
	{"Unit Do Overlay", 0, 0, 0},
	{"Attack Rating Against Monster Type", 9, 10, 0},
	{"Damage To Monster Type", 9, 10, 0},
	{"Fade", 3, 0, 0},
	{"Armor Override Percent", 0, 0, 0},
	{"Unused 183", 0, 0, 0},
	{"Unused 184", 0, 0, 0},
	{"Unused 185", 0, 0, 0},
	{"Unused 186", 0, 0, 0},
	{"Unused 187", 0, 0, 0},
	{"Skill Tab", 3, 16, 0},
	{"Unused 189", 0, 0, 0},
	{"Unused 190", 0, 0, 0},
	{"Unused 191", 0, 0, 0},
	{"Unused 192", 0, 0, 0},
	{"Unused 193", 0, 0, 0},
	{"Socket Count", 4, 0, 0},
	{"Skill On Striking", 7, 16, 0},
	{"Skill On Kill", 7, 16, 0},
	{"Skill On Death", 7, 16, 0},
	{"Skill On Hit", 7, 16, 0},
	{"Skill On Level Up", 7, 16, 0},
	{"Unused 200", 0, 0, 0},
	{"Skill When Struck", 7, 16, 0},
	{"Unused 202", 0, 0, 0},
	{"Unused 203", 0, 0, 0},
	{"Charged", 16, 16, 0},
	{"Unused 204", 0, 0, 0},
	{"Unused 205", 0, 0, 0},
	{"Unused 206", 0, 0, 0},
	{"Unused 207", 0, 0, 0},
	{"Unused 208", 0, 0, 0},
	{"Unused 209", 0, 0, 0},
	{"Unused 210", 0, 0, 0},
	{"Unused 211", 0, 0, 0},
	{"Unused 212", 0, 0, 0},
	{"Defense Per Level", 6, 0, 0},
	{"Enhanced Defense Per Level", 6, 0, 0},
	{"Life Per Level", 6, 0, 0},
	{"Mana Per Level", 6, 0, 0},
	{"Maximum Damage Per Level", 6, 0, 0},
	{"Maximum Enhanced Damage Per Level", 6, 0, 0},
	{"Strength Per Level", 6, 0, 0},
	{"Dexterity Per Level", 6, 0, 0},
	{"Energy Per Level", 6, 0, 0},
	{"Vitality Per Level", 6, 0, 0},
	{"Attack Rating Per Level", 6, 0, 0},
	{"Bonus To Attack Rating Per Level", 6, 0, 0},
	{"Maximum Cold Damage Per Level", 6, 0, 0},
	{"Maximum Fire Damage Per Level", 6, 0, 0},
	{"Maximum Lightning Damage Per Level", 6, 0, 0},
	{"Maximum Poison Damage Per Level", 6, 0, 0},
	{"Cold Resistance Per Level", 6, 0, 0},
	{"Fire Resistance Per Level", 6, 0, 0},
	{"Lightning Resistance Per Level", 6, 0, 0},
	{"Poison Resistance Per Level", 6, 0, 0},
	{"Cold Absorption Per Level", 6, 0, 0},
	{"Fire Absorption Per Level", 6, 0, 0},
	{"Lightning Absorption Per Level", 6, 0, 0},
	{"Poison Absorption Per Level", 6, 0, 0},
	{"Thorns Per Level", 5, 0, 0},
	{"Extra Gold Per Level", 6, 0, 0},
	{"Better Chance Of Getting Magic Item Per Level", 6, 0, 0},
	{"Stamina Regeneration Per Level", 6, 0, 0},
	{"Stamina Per Level", 6, 0, 0},
	{"Damage To Demons Per Level", 6, 0, 0},
	{"Damage To Undead Per Level", 6, 0, 0},
	{"Attack Rating Against Demons Per Level", 6, 0, 0},
	{"Attack Rating Against Undead Per Level", 6, 0, 0},
	{"Crushing Blow Per Level", 6, 0, 0},
	{"Open Wounds Per Level", 6, 0, 0},
	{"Kick Damage Per Level", 6, 0, 0},
	{"Deadly Strike Per Level", 6, 0, 0},
	{"Find Gems Per Level", 0, 0, 0},
	{"Repairs Durability", 6, 0, 0},
	{"Replenishes Quantity", 6, 0, 0},
	{"Increased Stack Size", 8, 0, 0},
	{"Find Item", 0, 0, 0},
	{"Slash Damage", 0, 0, 0},
	{"Slash Damage (Percent)", 0, 0, 0},
	{"Crush Damage", 0, 0, 0},
	{"Crush Damage (Percent)", 0, 0, 0},
	{"Thrust Damage", 0, 0, 0},
	{"Thrust Damage (Percent)", 0, 0, 0},
	{"Slash Damage Absorption", 0, 0, 0},
	{"Crush Damage Absorption", 0, 0, 0},
	{"Thrust Damage Absorption", 0, 0, 0},
	{"Slash Damage Absorption (Percent)", 0, 0, 0},
	{"Crush Damage Absorption (Percent)", 0, 0, 0},
	{"Thrust Damage Absorption (Percent)", 0, 0, 0},
	{"Defense Per Time", 22, 0, 0},
	{"Enhanced Defense Per Time", 22, 0, 0},
	{"Life Per Time", 22, 0, 0},
	{"Mana Per Time", 22, 0, 0},
	{"Maximum Damage Per Time", 22, 0, 0},
	{"Maximum Enhanced Damage Per Time", 22, 0, 0},
	{"Strength Per Time", 22, 0, 0},
	{"Dexterity Per Time", 22, 0, 0},
	{"Energy Per Time", 22, 0, 0},
	{"Vitality Per Time", 22, 0, 0},
	{"Attack Rating Per Time", 22, 0, 0},
	{"Chance To Hit Per Time", 22, 0, 0},
	{"Maximum Cold Damage Per Time", 22, 0, 0},
	{"Maximum Fire Damage Per Time", 22, 0, 0},
	{"Maximum Lightning Damage Per Time", 22, 0, 0},
	{"Maximum Damage Per Poison", 22, 0, 0},
	{"Cold Resistance Per Time", 22, 0, 0},
	{"Fire Resistance Per Time", 22, 0, 0},
	{"Lightning Resistance Per Time", 22, 0, 0},
	{"Poison Resistance Per Time", 22, 0, 0},
	{"Cold Absorption Per Time", 22, 0, 0},
	{"Fire Absorption Per Time", 22, 0, 0},
	{"Lightning Absorption Per Time", 22, 0, 0},
	{"Poison Absorption Per Time", 22, 0, 0},
	{"Extra Gold Per Time", 22, 0, 0},
	{"Better Chance Of Getting magic Item Per Time", 22, 0, 0},
	{"Regenerate Stamina Per Time", 22, 0, 0},
	{"Stamina Per Time", 22, 0, 0},
	{"Damage To Demons Per Time", 22, 0, 0},
	{"Damage To Undead Per Time", 22, 0, 0},
	{"Attack Rating Against Demons Per Time", 22, 0, 0},
	{"Attack Rating Against Undead Per Time", 22, 0, 0},
	{"Crushing Blow Per Time", 22, 0, 0},
	{"Open Wounds Per Time", 22, 0, 0},
	{"Kick Damage Per Time", 22, 0, 0},
	{"Deadly Strike Per Time", 22, 0, 0},
	{"Find Gems Per Time", 0, 0, 0},
	{"Enemy Cold Resistance Reduction", 8, 0, 50},
	{"Enemy Fire Resistance Reduction", 8, 0, 50},
	{"Enemy Lightning Resistance Reduction", 8, 0, 50},
	{"Enemy Poison Resistance Reduction", 8, 0, 50},
	{"Damage vs. Monsters", 0, 0, 0},
	{"Enhanced Damage vs. Monsters", 0, 0, 0},
	{"Attack Rating Against Monsters", 0, 0, 0},
	{"Bonus To Attack Rating Against Monsters", 0, 0, 0},
	{"Defense vs. Monsters", 0, 0, 0},
	{"Enhanced Defense vs. Monsters", 0, 0, 0},
	{"Fire Damage Length", 0, 0, 0},
	{"Minimum Fire Damage Length", 0, 0, 0},
	{"Maximum Fire Damage Length", 0, 0, 0},
	{"Progressive Damage", 0, 0, 0},
	{"Progressive Steal", 0, 0, 0},
	{"Progressive Other", 0, 0, 0},
	{"Progressive Fire", 0, 0, 0},
	{"Progressive Cold", 0, 0, 0},
	{"Progressive Lightning", 0, 0, 0},
	{"Extra Charges", 6, 0, 0},
	{"Progressive Attack Rating", 0, 0, 0},
	{"Poison Count", 0, 0, 0},
	{"Damage Framerate", 0, 0, 0},
	{"Pierce IDX", 0, 0, 0},
	{"Fire Mastery", 9, 0, 50},
	{"Lightning Mastery", 9, 0, 50},
	{"Cold Mastery", 9, 0, 50},
	{"Poison Mastery", 9, 0, 50},
	{"Passive Enemy Fire Resistance Reduction", 8, 0, 0},
	{"Passive Enemy Lightning Resistance Reduction", 8, 0, 0},
	{"Passive Enemy Cold Resistance Reduction", 8, 0, 0},
	{"Passive Enemy Poison Resistance Reduction", 8, 0, 0},
	{"Critical Strike", 8, 0, 0},
	{"Dodge", 7, 0, 0},
	{"Avoid", 7, 0, 0},
	{"Evade", 7, 0, 0},
	{"Warmth", 8, 0, 0},
	{"Melee Attack Rating Mastery", 8, 0, 0},
	{"Melee Damage Mastery", 8, 0, 0},
	{"Melee Critical Hit Mastery", 8, 0, 0},
	{"Thrown Weapon Attack Rating Mastery", 8, 0, 0},
	{"Thrown Weapon Damage Mastery", 8, 0, 0},
	{"Thrown Weapon Critical Hit Mastery", 8, 0, 0},
	{"Weapon Block", 8, 0, 0},
	{"Summon Resist", 8, 0, 0},
	{"Modifier List Skill", 0, 0, 0},
	{"Modifier List Level", 0, 0, 0},
	{"Last Sent Life Percent", 0, 0, 0},
	{"Source Unit Type", 0, 0, 0},
	{"Source Unit ID", 0, 0, 0},
	{"Short Parameter 1", 0, 0, 0},
	{"Quest Item Difficulty", 2, 0, 0},
	{"Passive Magical Damage Mastery", 9, 0, 50},
	{"Passive Magical Resistance Reduction", 8, 0, 0}
};


#pragma endregion

bool IsInitialized() {
	return initialized;
}

// If we find the temp file with MPQ info, use it; otherwise, fall back on the hardcoded lists.
void InitializeMPQData() {
	if (initialized) return;
	
	char* end;
	short lastID = -1;

	//cannot read sgptDataTable data until in game.
	while (!D2CLIENT_GetPlayerUnit()) {
		Sleep(100);
	}


	//pretty sure this can just be (*p_D2COMMON_sgptDataTable)->dwSkillsRecs
	//but ill leave it doing the same thing the mpq parser was
	for (auto i = 0; i < (*p_D2COMMON_sgptDataTable)->dwSkillsRecs; i++) {
		SkillsTxt* d = &(*p_D2COMMON_sgptDataTable)->pSkillsTxt[i];
		if (!d) {
			continue;
		}
		unsigned short id = d->wSkillId;
		if (id > SKILL_MAX) {
			SKILL_MAX = id;
		}
	}

	for (auto i = 0; i < (*p_D2COMMON_sgptDataTable)->dwCharsStatsRecs; i++) {
		CharStatsTxt* d = &(*p_D2COMMON_sgptDataTable)->pCharStatsTxt[i];
		if (!d) {
			continue;
		}
		CharStats* bits = new CharStats();
		bits->toHitFactor = d->dwToHitFactor;
		CharList.push_back(bits);
	}

	for (auto i = 0; i < (*p_D2COMMON_sgptDataTable)->dwItemStatCostRecs; i++) {
		ItemStatCostTxt* d = &(*p_D2COMMON_sgptDataTable)->pItemStatCostTxt[i];
		if (!d) {
			continue;
		}
		unsigned short id = d->wStat;
		if (id > STAT_MAX) {
			STAT_MAX = id;
		}

		for (int missing = lastID + 1; missing < id; missing++) {
			StatProperties* mbits = new StatProperties();
			mbits->name.assign("missing_id");
			mbits->ID = missing;
			mbits->sendParamBits = mbits->saveBits = mbits->saveAdd = mbits->saveParamBits = mbits->op = 0;
			AllStatList.push_back(mbits);
			StatMap[mbits->name] = mbits;
		}

		StatProperties* bits = new StatProperties();
		//MOV R32,DWORD [R32+0xBCC]
		bits->name = "unknown";
		std::transform(bits->name.begin(), bits->name.end(), bits->name.begin(), tolower);
		bits->ID = id;
		bits->sendParamBits = d->bSendParamBits;
		bits->saveBits = d->bSaveBits;
		bits->saveAdd = d->dwSaveAdd;
		bits->saveParamBits = d->dwSaveParamBits;
		bits->op = d->bOp;
		AllStatList.push_back(bits);
		StatMap[bits->name] = bits;
		lastID = (short)id;
	}


	//Weapons, Armors, and Misc.txts
	for (auto i = 0; i < *p_D2COMMON_ItemsTxtRecords; i++) {
		ItemsTxt* d = D2COMMON_GetItemsTxt(i);
		if (!d) {
			continue;
		}
		unsigned int flags = 0, flags2 = 0;

		BYTE throwable = 0;

		std::set<std::string> allTypeCodes;
		std::function<void(WORD)> computeTypes;
		computeTypes = [&](WORD t) {
			if (t == 0) return;
			ItemTypesTxt* type = &(*p_D2COMMON_sgptDataTable)->pItemsTypeTxt[t];
			if (type->bThrowable) {
				throwable = type->bThrowable;
			}
			allTypeCodes.insert(Trim(std::string(reinterpret_cast<char*>(type->szCode), 4)));
			computeTypes(type->wEquiv[0]);
			computeTypes(type->wEquiv[1]);
		};
		computeTypes(d->wtype);
		computeTypes(d->wtype2);

		if (allTypeCodes.find("weap") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_ALLWEAPON;
		} else if (allTypeCodes.find("armo") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_ALLARMOR;
		}

		if (allTypeCodes.find("circ") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_CIRCLET;
		} //not sure why elseif here, but dont wanna change func... a circlet is also a helm
		else if (allTypeCodes.find("helm") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_HELM;
		}
		else if (allTypeCodes.find("tors") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_ARMOR;
		}
		else if (allTypeCodes.find("glov") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_GLOVES;
		}
		else if (allTypeCodes.find("boot") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_BOOTS;
		}
		else if (allTypeCodes.find("belt") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_BELT;
		}
		else if (allTypeCodes.find("shld") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_SHIELD;
		}

		if (allTypeCodes.find("amaz") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_AMAZON_WEAPON;
		}
		else if (allTypeCodes.find("barb") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_BARBARIAN_HELM;
		}
		else if (allTypeCodes.find("necr") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_NECROMANCER_SHIELD;
		}
		else if (allTypeCodes.find("pala") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_PALADIN_SHIELD;
		}
		else if (allTypeCodes.find("sorc") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_SORCERESS_ORB;
		}
		else if (allTypeCodes.find("assn") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_ASSASSIN_KATAR;
		}
		else if (allTypeCodes.find("drui") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_DRUID_PELT;
		}

		if (allTypeCodes.find("club") != allTypeCodes.end() ||
			allTypeCodes.find("hamm") != allTypeCodes.end() ||
			allTypeCodes.find("mace") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_MACE;
		}
		else if (allTypeCodes.find("wand") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_WAND;
		}
		else if (allTypeCodes.find("staf") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_STAFF;
		}
		else if (allTypeCodes.find("bow") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_BOW;
		}
		else if (allTypeCodes.find("axe") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_AXE;
		}
		else if (allTypeCodes.find("scep") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_SCEPTER;
		}
		else if (allTypeCodes.find("swor") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_SWORD;
		}
		else if (allTypeCodes.find("knif") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_DAGGER;
		}
		else if (allTypeCodes.find("spea") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_SPEAR;
		}
		else if (allTypeCodes.find("pole") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_POLEARM;
		}
		else if (allTypeCodes.find("xbow") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_CROSSBOW;
		}
		else if (allTypeCodes.find("jave") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_JAVELIN;
		}
		if (allTypeCodes.find("thro") != allTypeCodes.end()) {
			flags |= ITEM_GROUP_THROWING;
		}
		if (allTypeCodes.find("rune") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_RUNE;
		}
		if (allTypeCodes.find("gem0") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_CHIPPED;
		}
		else if (allTypeCodes.find("gem1") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_FLAWED;
		}
		else if (allTypeCodes.find("gem2") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_REGULAR;
		}
		else if (allTypeCodes.find("gem3") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_FLAWLESS;
		}
		else if (allTypeCodes.find("gem4") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_PERFECT;
		}
		if (allTypeCodes.find("gema") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_AMETHYST;
		}
		else if (allTypeCodes.find("gemd") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_DIAMOND;
		}
		else if (allTypeCodes.find("geme") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_EMERALD;
		}
		else if (allTypeCodes.find("gemr") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_RUBY;
		}
		else if (allTypeCodes.find("gems") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_SAPPHIRE;
		}
		else if (allTypeCodes.find("gemt") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_TOPAZ;
		}
		else if (allTypeCodes.find("gemz") != allTypeCodes.end()) {
			flags2 |= ITEM_GROUP_SKULL;
		}

		if (d->dwultracode) {
			flags |= ITEM_GROUP_ELITE;
		} else if(d->dwubercode) {
			flags |= ITEM_GROUP_ELITE;
		} else {
			flags |= ITEM_GROUP_NORMAL;
		}
		ItemTypesTxt* mainType = &(*p_D2COMMON_sgptDataTable)->pItemsTypeTxt[d->wtype];

		ItemAttributes* attrs = new ItemAttributes();
		std::string code(d->szcode, 4);
		attrs->name = D2LANG_GetLocaleTextFromString(Trim(code).c_str());
		std::strncpy(attrs->code, d->szcode, 4);
		attrs->category = std::string(reinterpret_cast<char*>(mainType->szCode), 4);
		attrs->width = d->binvwidth;
		attrs->height = d->binvheight;
		attrs->stackable = d->bstackable;
		attrs->useable = d->buseable;
		attrs->throwable = throwable;
		attrs->itemLevel = 0;
		attrs->unusedFlags = 0;
		attrs->flags = flags;
		attrs->flags2 = flags2;
		attrs->qualityLevel = d->blevel;
		attrs->magicLevel = d->bmagiclvl;
		ItemAttributeMap[Trim(std::string(reinterpret_cast<char*>(attrs->code), 4))] = attrs;
	}

	initialized = true;
}
