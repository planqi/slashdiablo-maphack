#include "MPQInit.h"
#include "BH.h"
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
	{"Cap", "cap", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 1},
	{"Skull Cap", "skp", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 5},
	{"Helm", "hlm", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 11},
	{"Full Helm", "fhl", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 15},
	{"Great Helm", "ghm", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 23},
	{"Crown", "crn", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 29},
	{"Mask", "msk", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 19},
	{"Quilted Armor", "qui", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{"Leather Armor", "lea", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 3},
	{"Hard Leather Armor", "hla", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 5},
	{"Studded Leather", "stu", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 8},
	{"Ring Mail", "rng", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 11},
	{"Scale Mail", "scl", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 13},
	{"Chain Mail", "chn", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{"Breast Plate", "brs", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 18},
	{"Splint Mail", "spl", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 20},
	{"Plate Mail", "plt", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Field Plate", "fld", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 28},
	{"Gothic Plate", "gth", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 32},
	{"Full Plate Mail", "ful", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 37},
	{"Ancient Armor", "aar", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 40},
	{"Light Plate", "ltp", "Armor", 2, 3, 0, 0, 0, 1, 0, 0, 0, 35},
	{"Buckler", "buc", "Shield", 2, 2, 0, 0, 0, 1, 0, 0, 0, 1},
	{"Small Shield", "sml", "Shield", 2, 2, 0, 0, 0, 1, 0, 0, 0, 5},
	{"Large Shield", "lrg", "Shield", 2, 3, 0, 0, 0, 1, 0, 0, 0, 11},
	{"Kite Shield", "kit", "Shield", 2, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{"Tower Shield", "tow", "Shield", 2, 3, 0, 0, 0, 1, 0, 0, 0, 22},
	{"Gothic Shield", "gts", "Shield", 2, 4, 0, 0, 0, 1, 0, 0, 0, 30},
	{"Leather Gloves", "lgl", "Gloves", 2, 2, 0, 0, 0, 1, 0, 0, 0, 3},
	{"Heavy Gloves", "vgl", "Gloves", 2, 2, 0, 0, 0, 1, 0, 0, 0, 7},
	{"Chain Gloves", "mgl", "Gloves", 2, 2, 0, 0, 0, 1, 0, 0, 0, 12},
	{"Light Gauntlets", "tgl", "Gloves", 2, 2, 0, 0, 0, 1, 0, 0, 0, 20},
	{"Gauntlets", "hgl", "Gloves", 2, 2, 0, 0, 0, 1, 0, 0, 0, 27},
	{"Boots", "lbt", "Boots", 2, 2, 0, 0, 0, 1, 0, 0, 0, 3},
	{"Heavy Boots", "vbt", "Boots", 2, 2, 0, 0, 0, 1, 0, 0, 0, 7},
	{"Chain Boots", "mbt", "Boots", 2, 2, 0, 0, 0, 1, 0, 0, 0, 12},
	{"Light Plated Boots", "tbt", "Boots", 2, 2, 0, 0, 0, 1, 0, 0, 0, 20},
	{"Greaves", "hbt", "Boots", 2, 2, 0, 0, 0, 1, 0, 0, 0, 27},
	{"Sash", "lbl", "Belt", 2, 1, 0, 0, 0, 1, 0, 0, 0, 3},
	{"Light Belt", "vbl", "Belt", 2, 1, 0, 0, 0, 1, 0, 0, 0, 7},
	{"Belt", "mbl", "Belt", 2, 1, 0, 0, 0, 1, 0, 0, 0, 12},
	{"Heavy Belt", "tbl", "Belt", 2, 1, 0, 0, 0, 1, 0, 0, 0, 20},
	{"Plated Belt", "hbl", "Belt", 2, 1, 0, 0, 0, 1, 0, 0, 0, 27},
	{"Bone Helm", "bhm", "Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 22},
	{"Bone Shield", "bsh", "Shield", 2, 3, 0, 0, 0, 1, 0, 0, 0, 19},
	{"Spiked Shield", "spk", "Shield", 2, 3, 0, 0, 0, 1, 0, 0, 0, 11},
	{"War Hat", "xap", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 34},
	{"Sallet", "xkp", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 37},
	{"Casque", "xlm", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 42},
	{"Basinet", "xhl", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 45},
	{"Winged Helm", "xhm", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 51},
	{"Grand Crown", "xrn", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 55},
	{"Death Mask", "xsk", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 48},
	{"Ghost Armor", "xui", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 34},
	{"Serpentskin Armor", "xea", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 36},
	{"Demonhide Armor", "xla", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 37},
	{"Trellised Armor", "xtu", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 40},
	{"Linked Mail", "xng", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 42},
	{"Tigulated Mail", "xcl", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 43},
	{"Mesh Armor", "xhn", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 45},
	{"Cuirass", "xrs", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 47},
	{"Russet Armor", "xpl", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 49},
	{"Templar Coat", "xlt", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 52},
	{"Sharktooth Armor", "xld", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 55},
	{"Embossed Plate", "xth", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 58},
	{"Chaos Armor", "xul", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 61},
	{"Ornate Plate", "xar", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 64},
	{"Mage Plate", "xtp", "Armor", 2, 3, 0, 0, 0, 2, 0, 0, 0, 60},
	{"Defender", "xuc", "Shield", 2, 2, 0, 0, 0, 2, 0, 0, 0, 34},
	{"Round Shield", "xml", "Shield", 2, 2, 0, 0, 0, 2, 0, 0, 0, 37},
	{"Scutum", "xrg", "Shield", 2, 3, 0, 0, 0, 2, 0, 0, 0, 42},
	{"Dragon Shield", "xit", "Shield", 2, 3, 0, 0, 0, 2, 0, 0, 0, 45},
	{"Pavise", "xow", "Shield", 2, 3, 0, 0, 0, 2, 0, 0, 0, 50},
	{"Ancient Shield", "xts", "Shield", 2, 4, 0, 0, 0, 2, 0, 0, 0, 56},
	{"Demonhide Gloves", "xlg", "Gloves", 2, 2, 0, 0, 0, 2, 0, 0, 0, 33},
	{"Sharkskin Gloves", "xvg", "Gloves", 2, 2, 0, 0, 0, 2, 0, 0, 0, 39},
	{"Heavy Bracers", "xmg", "Gloves", 2, 2, 0, 0, 0, 2, 0, 0, 0, 43},
	{"Battle Gauntlets", "xtg", "Gloves", 2, 2, 0, 0, 0, 2, 0, 0, 0, 49},
	{"War Gauntlets", "xhg", "Gloves", 2, 2, 0, 0, 0, 2, 0, 0, 0, 54},
	{"Demonhide Boots", "xlb", "Boots", 2, 2, 0, 0, 0, 2, 0, 0, 0, 36},
	{"Sharkskin Boots", "xvb", "Boots", 2, 2, 0, 0, 0, 2, 0, 0, 0, 39},
	{"Mesh Boots", "xmb", "Boots", 2, 2, 0, 0, 0, 2, 0, 0, 0, 43},
	{"Battle Boots", "xtb", "Boots", 2, 2, 0, 0, 0, 2, 0, 0, 0, 49},
	{"War Boots", "xhb", "Boots", 2, 2, 0, 0, 0, 2, 0, 0, 0, 54},
	{"Demonhide Sash", "zlb", "Belt", 2, 1, 0, 0, 0, 2, 0, 0, 0, 36},
	{"Sharkskin Belt", "zvb", "Belt", 2, 1, 0, 0, 0, 2, 0, 0, 0, 39},
	{"Mesh Belt", "zmb", "Belt", 2, 1, 0, 0, 0, 2, 0, 0, 0, 43},
	{"Battle Belt", "ztb", "Belt", 2, 1, 0, 0, 0, 2, 0, 0, 0, 49},
	{"War Belt", "zhb", "Belt", 2, 1, 0, 0, 0, 2, 0, 0, 0, 54},
	{"Grim Helm", "xh9", "Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 50},
	{"Grim Shield", "xsh", "Shield", 2, 3, 0, 0, 0, 2, 0, 0, 0, 48},
	{"Barbed Shield", "xpk", "Shield", 2, 3, 0, 0, 0, 2, 0, 0, 0, 42},
	{"Wolf Head", "dr1", "Druid Pelt", 2, 2, 0, 0, 0, 1, 0, 0, 0, 4},
	{"Hawk Helm", "dr2", "Druid Pelt", 2, 2, 0, 0, 0, 1, 0, 0, 0, 8},
	{"Antlers", "dr3", "Druid Pelt", 2, 2, 0, 0, 0, 1, 0, 0, 0, 16},
	{"Falcon Mask", "dr4", "Druid Pelt", 2, 2, 0, 0, 0, 1, 0, 0, 0, 20},
	{"Spirit Mask", "dr5", "Druid Pelt", 2, 2, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Jawbone Cap", "ba1", "Barbarian Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 4},
	{"Fanged Helm", "ba2", "Barbarian Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 8},
	{"Horned Helm", "ba3", "Barbarian Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 16},
	{"Assault Helmet", "ba4", "Barbarian Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 20},
	{"Avenger Guard", "ba5", "Barbarian Helm", 2, 2, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Targe", "pa1", "Paladin Shield", 2, 2, 0, 0, 0, 1, 0, 0, 0, 4},
	{"Rondache", "pa2", "Paladin Shield", 2, 2, 0, 0, 0, 1, 0, 0, 0, 8},
	{"Heraldic Shield", "pa3", "Paladin Shield", 2, 4, 0, 0, 0, 1, 0, 0, 0, 16},
	{"Aerin Shield", "pa4", "Paladin Shield", 2, 4, 0, 0, 0, 1, 0, 0, 0, 20},
	{"Crown Shield", "pa5", "Paladin Shield", 2, 2, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Preserved Head", "ne1", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 1, 0, 0, 0, 4},
	{"Zombie Head", "ne2", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 1, 0, 0, 0, 8},
	{"Unraveller Head", "ne3", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 1, 0, 0, 0, 16},
	{"Gargoyle Head", "ne4", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 1, 0, 0, 0, 20},
	{"Demon Head", "ne5", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Circlet", "ci0", "Circlet", 2, 2, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Coronet", "ci1", "Circlet", 2, 2, 0, 0, 0, 2, 0, 0, 0, 52},
	{"Tiara", "ci2", "Circlet", 2, 2, 0, 0, 0, 3, 0, 0, 0, 70},
	{"Diadem", "ci3", "Circlet", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Shako", "uap", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 58},
	{"Hydraskull", "ukp", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 63},
	{"Armet", "ulm", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 68},
	{"Giant Conch", "uhl", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 54},
	{"Spired Helm", "uhm", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 79},
	{"Corona", "urn", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Demonhead", "usk", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 74},
	{"Dusk Shroud", "uui", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 65},
	{"Wyrmhide", "uea", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 67},
	{"Scarab Husk", "ula", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 68},
	{"Wire Fleece", "utu", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 70},
	{"Diamond Mail", "ung", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 72},
	{"Loricated Mail", "ucl", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 73},
	{"Boneweave", "uhn", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 62},
	{"Great Hauberk", "urs", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 75},
	{"Balrog Skin", "upl", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 76},
	{"Hellforge Plate", "ult", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 78},
	{"Kraken Shell", "uld", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 81},
	{"Lacquered Plate", "uth", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 82},
	{"Shadow Plate", "uul", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 83},
	{"Sacred Armor", "uar", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Archon Plate", "utp", "Armor", 2, 3, 0, 0, 0, 3, 0, 0, 0, 84},
	{"Heater", "uuc", "Shield", 2, 2, 0, 0, 0, 3, 0, 0, 0, 58},
	{"Luna", "uml", "Shield", 2, 2, 0, 0, 0, 3, 0, 0, 0, 61},
	{"Hyperion", "urg", "Shield", 2, 3, 0, 0, 0, 3, 0, 0, 0, 64},
	{"Monarch", "uit", "Shield", 2, 3, 0, 0, 0, 3, 0, 0, 0, 72},
	{"Aegis", "uow", "Shield", 2, 3, 0, 0, 0, 3, 0, 0, 0, 79},
	{"Ward", "uts", "Shield", 2, 4, 0, 0, 0, 3, 0, 0, 0, 84},
	{"Bramble Mitts", "ulg", "Gloves", 2, 2, 0, 0, 0, 3, 0, 0, 0, 57},
	{"Vampirebone Gloves", "uvg", "Gloves", 2, 2, 0, 0, 0, 3, 0, 0, 0, 63},
	{"Vambraces", "umg", "Gloves", 2, 2, 0, 0, 0, 3, 0, 0, 0, 69},
	{"Crusader Gauntlets", "utg", "Gloves", 2, 2, 0, 0, 0, 3, 0, 0, 0, 76},
	{"Ogre Gauntlets", "uhg", "Gloves", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Wyrmhide Boots", "ulb", "Boots", 2, 2, 0, 0, 0, 3, 0, 0, 0, 60},
	{"Scarabshell Boots", "uvb", "Boots", 2, 2, 0, 0, 0, 3, 0, 0, 0, 66},
	{"Boneweave Boots", "umb", "Boots", 2, 2, 0, 0, 0, 3, 0, 0, 0, 72},
	{"Mirrored Boots", "utb", "Boots", 2, 2, 0, 0, 0, 3, 0, 0, 0, 81},
	{"Myrmidon Greaves", "uhb", "Boots", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Spiderweb Sash", "ulc", "Belt", 2, 1, 0, 0, 0, 3, 0, 0, 0, 61},
	{"Vampirefang Belt", "uvc", "Belt", 2, 1, 0, 0, 0, 3, 0, 0, 0, 68},
	{"Mithril Coil", "umc", "Belt", 2, 1, 0, 0, 0, 3, 0, 0, 0, 75},
	{"Troll Belt", "utc", "Belt", 2, 1, 0, 0, 0, 3, 0, 0, 0, 82},
	{"Colossus Girdle", "uhc", "Belt", 2, 1, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Bone Visage", "uh9", "Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 84},
	{"Troll Nest", "ush", "Shield", 2, 3, 0, 0, 0, 3, 0, 0, 0, 76},
	{"Blade Barrier", "upk", "Shield", 2, 3, 0, 0, 0, 3, 0, 0, 0, 68},
	{"Alpha Helm", "dr6", "Druid Pelt", 2, 2, 0, 0, 0, 2, 0, 0, 0, 35},
	{"Griffon Headdress", "dr7", "Druid Pelt", 2, 2, 0, 0, 0, 2, 0, 0, 0, 40},
	{"Hunter's Guise", "dr8", "Druid Pelt", 2, 2, 0, 0, 0, 2, 0, 0, 0, 46},
	{"Sacred Feathers", "dr9", "Druid Pelt", 2, 2, 0, 0, 0, 2, 0, 0, 0, 50},
	{"Totemic Mask", "dra", "Druid Pelt", 2, 2, 0, 0, 0, 2, 0, 0, 0, 55},
	{"Jawbone Visor", "ba6", "Barbarian Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 33},
	{"Lion Helm", "ba7", "Barbarian Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 38},
	{"Rage Mask", "ba8", "Barbarian Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 44},
	{"Savage Helmet", "ba9", "Barbarian Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 49},
	{"Slayer Guard", "baa", "Barbarian Helm", 2, 2, 0, 0, 0, 2, 0, 0, 0, 54},
	{"Akaran Targe", "pa6", "Paladin Shield", 2, 2, 0, 0, 0, 2, 0, 0, 0, 35},
	{"Akaran Rondache", "pa7", "Paladin Shield", 2, 2, 0, 0, 0, 2, 0, 0, 0, 40},
	{"Protector Shield", "pa8", "Paladin Shield", 2, 4, 0, 0, 0, 2, 0, 0, 0, 46},
	{"Gilded Shield", "pa9", "Paladin Shield", 2, 4, 0, 0, 0, 2, 0, 0, 0, 51},
	{"Royal Shield", "paa", "Paladin Shield", 2, 2, 0, 0, 0, 2, 0, 0, 0, 55},
	{"Mummified Trophy", "ne6", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 2, 0, 0, 0, 33},
	{"Fetish Trophy", "ne7", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 2, 0, 0, 0, 39},
	{"Sexton Trophy", "ne8", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 2, 0, 0, 0, 45},
	{"Cantor Trophy", "ne9", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 2, 0, 0, 0, 49},
	{"Hierophant Trophy", "nea", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 2, 0, 0, 0, 54},
	{"Blood Spirit", "drb", "Druid Pelt", 2, 2, 0, 0, 0, 3, 0, 0, 0, 62},
	{"Sun Spirit", "drc", "Druid Pelt", 2, 2, 0, 0, 0, 3, 0, 0, 0, 69},
	{"Earth Spirit", "drd", "Druid Pelt", 2, 2, 0, 0, 0, 3, 0, 0, 0, 76},
	{"Sky Spirit", "dre", "Druid Pelt", 2, 2, 0, 0, 0, 3, 0, 0, 0, 83},
	{"Dream Spirit", "drf", "Druid Pelt", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Carnage Helm", "bab", "Barbarian Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 60},
	{"Fury Visor", "bac", "Barbarian Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 66},
	{"Destroyer Helm", "bad", "Barbarian Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 73},
	{"Conqueror Crown", "bae", "Barbarian Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 80},
	{"Guardian Crown", "baf", "Barbarian Helm", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Sacred Targe", "pab", "Paladin Shield", 2, 2, 0, 0, 0, 3, 0, 0, 0, 63},
	{"Sacred Rondache", "pac", "Paladin Shield", 2, 2, 0, 0, 0, 3, 0, 0, 0, 70},
	{"Kurast Shield", "pad", "Paladin Shield", 2, 4, 0, 0, 0, 3, 0, 0, 0, 74},
	{"Zakarum Shield", "pae", "Paladin Shield", 2, 4, 0, 0, 0, 3, 0, 0, 0, 82},
	{"Vortex Shield", "paf", "Paladin Shield", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Minion Skull", "neb", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 3, 0, 0, 0, 59},
	{"Hellspawn Skull", "neg", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 3, 0, 0, 0, 67},
	{"Overseer Skull", "ned", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 3, 0, 0, 0, 66},
	{"Succubus Skull", "nee", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 3, 0, 0, 0, 81},
	{"Bloodlord Skull", "nef", "Necromancer Shrunken Head", 2, 2, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Elixir", "elx", "Elixir", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"hpo", "hpo", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"mpo", "mpo", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"hpf", "hpf", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"mpf", "mpf", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Stamina Potion", "vps", "Stamina Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Antidote Potion", "yps", "Antidote Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Rejuvenation Potion", "rvs", "Rejuvenation Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Full Rejuvenation Potion", "rvl", "Rejuvenation Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Thawing Potion", "wms", "Thawing Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Tome of Town Portal", "tbk", "Tome", 1, 2, 1, 1, 0, 0, 0, 0, 0, 0},
	{"Tome of Identify", "ibk", "Tome", 1, 2, 1, 1, 0, 0, 0, 0, 0, 0},
	{"Amulet", "amu", "Amulet", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Top of the Horadric Staff", "vip", "Amulet", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Ring", "rin", "Ring", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Gold", "gld", "Gold", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{"Scroll of Inifuss", "bks", "Quest Item", 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Key to the Cairn Stones", "bkd", "Quest Item", 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Arrows", "aqv", "Arrows", 1, 3, 1, 0, 0, 0, 0, 0, 0, 0},
	{"Torch", "tch", "Torch", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Bolts", "cqv", "Bolts", 1, 3, 1, 0, 0, 0, 0, 0, 0, 0},
	{"Scroll of Town Portal", "tsc", "Scroll", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Scroll of Identify", "isc", "Scroll", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Heart", "hrt", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Brain", "brz", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Jawbone", "jaw", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Eye", "eyz", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Horn", "hrn", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Tail", "tal", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Flag", "flg", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Fang", "fng", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Quill", "qll", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Soul", "sol", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Scalp", "scz", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Spleen", "spe", "Body Part", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Key", "key", "Key", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{"The Black Tower Key", "luv", "Key", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Potion of Life", "xyz", "Quest Item", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"A Jade Figurine", "j34", "Quest Item", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"The Golden Bird", "g34", "Quest Item", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Lam Esen's Tome", "bbb", "Quest Item", 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Horadric Cube", "box", "Quest Item", 2, 2, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Horadric Scroll", "tr1", "Quest Item", 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Mephisto's Soulstone", "mss", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Book of Skill", "ass", "Quest Item", 2, 2, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Khalim's Eye", "qey", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Khalim's Heart", "qhr", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Khalim's Brain", "qbr", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Ear", "ear", "Ear", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Chipped Amethyst", "gcv", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 33, 0},
	{"Flawed Amethyst", "gfv", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 34, 0},
	{"Amethyst", "gsv", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 36, 0},
	{"Flawless Amethyst", "gzv", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 40, 0},
	{"Perfect Amethyst", "gpv", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 48, 0},
	{"Chipped Topaz", "gcy", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 1025, 0},
	{"Flawed Topaz", "gfy", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 1026, 0},
	{"Topaz", "gsy", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 1028, 0},
	{"Flawless Topaz", "gly", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 1032, 0},
	{"Perfect Topaz", "gpy", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 1040, 0},
	{"Chipped Sapphire", "gcb", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 513, 0},
	{"Flawed Sapphire", "gfb", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 514, 0},
	{"Sapphire", "gsb", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 516, 0},
	{"Flawless Sapphire", "glb", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 520, 0},
	{"Perfect Sapphire", "gpb", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 528, 0},
	{"Chipped Emerald", "gcg", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 129, 0},
	{"Flawed Emerald", "gfg", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 130, 0},
	{"Emerald", "gsg", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 132, 0},
	{"Flawless Emerald", "glg", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 136, 0},
	{"Perfect Emerald", "gpg", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 144, 0},
	{"Chipped Ruby", "gcr", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 257, 0},
	{"Flawed Ruby", "gfr", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 258, 0},
	{"Ruby", "gsr", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 260, 0},
	{"Flawless Ruby", "glr", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 264, 0},
	{"Perfect Ruby", "gpr", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 272, 0},
	{"Chipped Diamond", "gcw", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 65, 0},
	{"Flawed Diamond", "gfw", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 66, 0},
	{"Diamond", "gsw", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 68, 0},
	{"Flawless Diamond", "glw", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 72, 0},
	{"Perfect Diamond", "gpw", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 80, 0},
	{"Minor Healing Potion", "hp1", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Light Healing Potion", "hp2", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Healing Potion", "hp3", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Greater Healing Potion", "hp4", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Super Healing Potion", "hp5", "Health Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Minor Mana Potion", "mp1", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Light Mana Potion", "mp2", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Mana Potion", "mp3", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Greater Mana Potion", "mp4", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Super Mana Potion", "mp5", "Mana Potion", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Chipped Skull", "skc", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 2049, 0},
	{"Flawed Skull", "skf", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 2050, 0},
	{"Skull", "sku", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 2052, 0},
	{"Flawless Skull", "skl", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 2056, 0},
	{"Perfect Skull", "skz", "Gem", 1, 1, 0, 0, 0, 0, 0, 0, 2064, 0},
	{"Herb", "hrb", "Herb", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Small Charm", "cm1", "Small Charm", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Large Charm", "cm2", "Large Charm", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Grand Charm", "cm3", "Grand Charm", 1, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{"rps", "rps", "Health Potion", 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{"rpl", "rpl", "Health Potion", 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{"bps", "bps", "Mana Potion", 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{"bpl", "bpl", "Mana Potion", 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
	{"El Rune", "r01", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Eld Rune", "r02", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Tir Rune", "r03", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Nef Rune", "r04", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Eth Rune", "r05", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Ith Rune", "r06", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Tal Rune", "r07", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Ral Rune", "r08", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Ort Rune", "r09", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Thul Rune", "r10", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Amn Rune", "r11", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Sol Rune", "r12", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Shael Rune", "r13", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Dol Rune", "r14", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Hel Rune", "r15", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Io Rune", "r16", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Lum Rune", "r17", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Ko Rune", "r18", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Fal Rune", "r19", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Lem Rune", "r20", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Pul Rune", "r21", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Um Rune", "r22", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Mal Rune", "r23", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Ist Rune", "r24", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Gul Rune", "r25", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Vex Rune", "r26", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Ohm Rune", "r27", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Lo Rune", "r28", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Sur Rune", "r29", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Ber Rune", "r30", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Jah Rune", "r31", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Cham Rune", "r32", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Zod Rune", "r33", "Rune", 1, 1, 0, 0, 0, 0, 0, 0, 4096, 0},
	{"Jewel", "jew", "Jewel", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Malah's Potion", "ice", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Scroll of Knowledge", "0sc", "Scroll", 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Scroll of Resistance", "tr2", "Quest Item", 2, 2, 0, 1, 0, 0, 0, 0, 0, 0},
	{"Key of Terror", "pk1", "Quest Item", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Key of Hate", "pk2", "Quest Item", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Key of Destruction", "pk3", "Quest Item", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Twisted Essence of Suffering", "tes", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Charged Essence of Hatred", "ceh", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Burning Essence of Terror", "bet", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Festering Essence of Destruction", "fed", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Token of Absolution", "toa", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Diablo's Horn", "dhn", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Baal's Eye", "bey", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Mephisto's Brain", "mbr", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Standard of Heroes", "std", "Quest Item", 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Hand Axe", "hax", "Axe", 1, 3, 0, 0, 0, 1, 0, 0, 0, 3},
	{"Axe", "axe", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 7},
	{"Double Axe", "2ax", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 13},
	{"Military Pick", "mpi", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 19},
	{"War Axe", "wax", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 25},
	{"Large Axe", "lax", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 6},
	{"Broad Axe", "bax", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 12},
	{"Battle Axe", "btx", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 17},
	{"Great Axe", "gax", "Axe", 2, 4, 0, 0, 0, 1, 0, 0, 0, 23},
	{"Giant Axe", "gix", "Axe", 2, 3, 0, 0, 0, 1, 0, 0, 0, 27},
	{"Wand", "wnd", "Wand", 1, 2, 0, 0, 0, 1, 0, 0, 0, 2},
	{"Yew Wand", "ywn", "Wand", 1, 2, 0, 0, 0, 1, 0, 0, 0, 12},
	{"Bone Wand", "bwn", "Wand", 1, 2, 0, 0, 0, 1, 0, 0, 0, 18},
	{"Grim Wand", "gwn", "Wand", 1, 2, 0, 0, 0, 1, 0, 0, 0, 26},
	{"Club", "clb", "Club", 1, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{"Scepter", "scp", "Scepter", 1, 3, 0, 0, 0, 1, 0, 0, 0, 3},
	{"Grand Scepter", "gsc", "Scepter", 1, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{"War Scepter", "wsp", "Scepter", 2, 3, 0, 0, 0, 1, 0, 0, 0, 21},
	{"Spiked Club", "spc", "Club", 1, 3, 0, 0, 0, 1, 0, 0, 0, 4},
	{"Mace", "mac", "Mace", 1, 3, 0, 0, 0, 1, 0, 0, 0, 8},
	{"Morning Star", "mst", "Mace", 1, 3, 0, 0, 0, 1, 0, 0, 0, 13},
	{"Flail", "fla", "Mace", 2, 3, 0, 0, 0, 1, 0, 0, 0, 19},
	{"War Hammer", "whm", "Hammer", 2, 3, 0, 0, 0, 1, 0, 0, 0, 25},
	{"Maul", "mau", "Hammer", 2, 4, 0, 0, 0, 1, 0, 0, 0, 21},
	{"Great Maul", "gma", "Hammer", 2, 3, 0, 0, 0, 1, 0, 0, 0, 32},
	{"Short Sword", "ssd", "Sword", 1, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{"Scimitar", "scm", "Sword", 1, 3, 0, 0, 0, 1, 0, 0, 0, 5},
	{"Sabre", "sbr", "Sword", 1, 3, 0, 0, 0, 1, 0, 0, 0, 8},
	{"Falchion", "flc", "Sword", 1, 3, 0, 0, 0, 1, 0, 0, 0, 11},
	{"Crystal Sword", "crs", "Sword", 2, 3, 0, 0, 0, 1, 0, 0, 0, 11},
	{"Broad Sword", "bsd", "Sword", 2, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{"Long Sword", "lsd", "Sword", 2, 3, 0, 0, 0, 1, 0, 0, 0, 20},
	{"War Sword", "wsd", "Sword", 1, 3, 0, 0, 0, 1, 0, 0, 0, 27},
	{"Two-Handed Sword", "2hs", "Sword", 1, 4, 0, 0, 0, 1, 0, 0, 0, 10},
	{"Claymore", "clm", "Sword", 1, 4, 0, 0, 0, 1, 0, 0, 0, 17},
	{"Giant Sword", "gis", "Sword", 1, 4, 0, 0, 0, 1, 0, 0, 0, 21},
	{"Bastard Sword", "bsw", "Sword", 1, 4, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Flamberge", "flb", "Sword", 2, 4, 0, 0, 0, 1, 0, 0, 0, 27},
	{"Great Sword", "gsd", "Sword", 2, 4, 0, 0, 0, 1, 0, 0, 0, 33},
	{"Dagger", "dgr", "Dagger", 1, 2, 0, 0, 0, 1, 0, 0, 0, 3},
	{"Dirk", "dir", "Dagger", 1, 2, 0, 0, 0, 1, 0, 0, 0, 9},
	{"Kris", "kri", "Dagger", 1, 3, 0, 0, 0, 1, 0, 0, 0, 17},
	{"Blade", "bld", "Dagger", 1, 3, 0, 0, 0, 1, 0, 0, 0, 23},
	{"Throwing Knife", "tkf", "Throwing Knife", 1, 2, 1, 0, 1, 1, 0, 0, 0, 2},
	{"Throwing Axe", "tax", "Throwing Axe", 1, 2, 1, 0, 1, 1, 0, 0, 0, 7},
	{"Balanced Knife", "bkf", "Throwing Knife", 1, 2, 1, 0, 1, 1, 0, 0, 0, 13},
	{"Balanced Axe", "bal", "Throwing Axe", 2, 3, 1, 0, 1, 1, 0, 0, 0, 7},
	{"Javelin", "jav", "Javelin", 1, 3, 1, 0, 1, 1, 0, 0, 0, 1},
	{"Pilum", "pil", "Javelin", 1, 3, 1, 0, 1, 1, 0, 0, 0, 10},
	{"Short Spear", "ssp", "Javelin", 1, 3, 1, 0, 1, 1, 0, 0, 0, 15},
	{"Glaive", "glv", "Javelin", 1, 4, 1, 0, 1, 1, 0, 0, 0, 23},
	{"Throwing Spear", "tsp", "Javelin", 1, 4, 1, 0, 1, 1, 0, 0, 0, 29},
	{"Spear", "spr", "Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 5},
	{"Trident", "tri", "Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 9},
	{"Brandistock", "brn", "Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 16},
	{"Spetum", "spt", "Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 20},
	{"Pike", "pik", "Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Bardiche", "bar", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 5},
	{"Voulge", "vou", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 11},
	{"Scythe", "scy", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 15},
	{"Poleaxe", "pax", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 21},
	{"Halberd", "hal", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 29},
	{"War Scythe", "wsc", "Polearm", 2, 4, 0, 0, 0, 1, 0, 0, 0, 34},
	{"Short Staff", "sst", "Staff", 1, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{"Long Staff", "lst", "Staff", 1, 4, 0, 0, 0, 1, 0, 0, 0, 8},
	{"Gnarled Staff", "cst", "Staff", 1, 4, 0, 0, 0, 1, 0, 0, 0, 12},
	{"Battle Staff", "bst", "Staff", 1, 4, 0, 0, 0, 1, 0, 0, 0, 17},
	{"War Staff", "wst", "Staff", 2, 4, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Short Bow", "sbw", "Bow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{"Hunter's Bow", "hbw", "Bow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 5},
	{"Long Bow", "lbw", "Bow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 8},
	{"Composite Bow", "cbw", "Bow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 12},
	{"Short Battle Bow", "sbb", "Bow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 18},
	{"Long Battle Bow", "lbb", "Bow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 23},
	{"Short War Bow", "swb", "Bow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 27},
	{"Long War Bow", "lwb", "Bow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 31},
	{"Light Crossbow", "lxb", "Crossbow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 6},
	{"Crossbow", "mxb", "Crossbow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{"Heavy Crossbow", "hxb", "Crossbow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Repeating Crossbow", "rxb", "Crossbow", 2, 3, 0, 0, 0, 1, 0, 0, 0, 33},
	{"Rancid Gas Potion", "gps", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{"Oil Potion", "ops", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{"Choking Gas Potion", "gpm", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{"Exploding Potion", "opm", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{"Strangling Gas Potion", "gpl", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{"Fulminating Potion", "opl", "Throwing Potion", 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{"Decoy Gidbinn", "d33", "Dagger", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"The Gidbinn", "g33", "Dagger", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Wirt's Leg", "leg", "Club", 1, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Horadric Malus", "hdm", "Hammer", 1, 2, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Hell Forge Hammer", "hfh", "Hammer", 2, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Horadric Staff", "hst", "Staff", 1, 4, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Shaft of the Horadric Staff", "msf", "Staff", 1, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Hatchet", "9ha", "Axe", 1, 3, 0, 0, 0, 2, 0, 0, 0, 31},
	{"Cleaver", "9ax", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 34},
	{"Twin Axe", "92a", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 39},
	{"Crowbill", "9mp", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 43},
	{"Naga", "9wa", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 48},
	{"Military Axe", "9la", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 34},
	{"Bearded Axe", "9ba", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 38},
	{"Tabar", "9bt", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 42},
	{"Gothic Axe", "9ga", "Axe", 2, 4, 0, 0, 0, 2, 0, 0, 0, 46},
	{"Ancient Axe", "9gi", "Axe", 2, 3, 0, 0, 0, 2, 0, 0, 0, 51},
	{"Burnt Wand", "9wn", "Wand", 1, 2, 0, 0, 0, 2, 0, 0, 0, 31},
	{"Petrified Wand", "9yw", "Wand", 1, 2, 0, 0, 0, 2, 0, 0, 0, 38},
	{"Tomb Wand", "9bw", "Wand", 1, 2, 0, 0, 0, 2, 0, 0, 0, 43},
	{"Grave Wand", "9gw", "Wand", 1, 2, 0, 0, 0, 2, 0, 0, 0, 49},
	{"Cudgel", "9cl", "Club", 1, 3, 0, 0, 0, 2, 0, 0, 0, 30},
	{"Rune Scepter", "9sc", "Scepter", 1, 3, 0, 0, 0, 2, 0, 0, 0, 31},
	{"Holy Water Sprinkler", "9qs", "Scepter", 1, 3, 0, 0, 0, 2, 0, 0, 0, 4},
	{"Divine Scepter", "9ws", "Scepter", 2, 3, 0, 0, 0, 2, 0, 0, 0, 45},
	{"Barbed Club", "9sp", "Club", 1, 3, 0, 0, 0, 2, 0, 0, 0, 32},
	{"Flanged Mace", "9ma", "Mace", 1, 3, 0, 0, 0, 2, 0, 0, 0, 35},
	{"Jagged Star", "9mt", "Mace", 1, 3, 0, 0, 0, 2, 0, 0, 0, 39},
	{"Knout", "9fl", "Mace", 2, 3, 0, 0, 0, 2, 0, 0, 0, 43},
	{"Battle Hammer", "9wh", "Hammer", 2, 3, 0, 0, 0, 2, 0, 0, 0, 48},
	{"War Club", "9m9", "Hammer", 2, 4, 0, 0, 0, 2, 0, 0, 0, 45},
	{"Martel de Fer", "9gm", "Hammer", 2, 3, 0, 0, 0, 2, 0, 0, 0, 53},
	{"Gladius", "9ss", "Sword", 1, 3, 0, 0, 0, 2, 0, 0, 0, 30},
	{"Cutlass", "9sm", "Sword", 1, 3, 0, 0, 0, 2, 0, 0, 0, 43},
	{"Shamshir", "9sb", "Sword", 1, 3, 0, 0, 0, 2, 0, 0, 0, 35},
	{"Tulwar", "9fc", "Sword", 1, 3, 0, 0, 0, 2, 0, 0, 0, 37},
	{"Dimensional Blade", "9cr", "Sword", 2, 3, 0, 0, 0, 2, 0, 0, 0, 37},
	{"Battle Sword", "9bs", "Sword", 2, 3, 0, 0, 0, 2, 0, 0, 0, 40},
	{"Rune Sword", "9ls", "Sword", 2, 3, 0, 0, 0, 2, 0, 0, 0, 44},
	{"Ancient Sword", "9wd", "Sword", 1, 3, 0, 0, 0, 2, 0, 0, 0, 49},
	{"Espandon", "92h", "Sword", 1, 4, 0, 0, 0, 2, 0, 0, 0, 37},
	{"Dacian Falx", "9cm", "Sword", 1, 4, 0, 0, 0, 2, 0, 0, 0, 42},
	{"Tusk Sword", "9gs", "Sword", 1, 4, 0, 0, 0, 2, 0, 0, 0, 45},
	{"Gothic Sword", "9b9", "Sword", 1, 4, 0, 0, 0, 2, 0, 0, 0, 48},
	{"Zweihander", "9fb", "Sword", 2, 4, 0, 0, 0, 2, 0, 0, 0, 49},
	{"Executioner Sword", "9gd", "Sword", 2, 4, 0, 0, 0, 2, 0, 0, 0, 54},
	{"Poignard", "9dg", "Dagger", 1, 2, 0, 0, 0, 2, 0, 0, 0, 31},
	{"Rondel", "9di", "Dagger", 1, 2, 0, 0, 0, 2, 0, 0, 0, 36},
	{"Cinquedeas", "9kr", "Dagger", 1, 3, 0, 0, 0, 2, 0, 0, 0, 42},
	{"Stiletto", "9bl", "Dagger", 1, 3, 0, 0, 0, 2, 0, 0, 0, 46},
	{"Battle Dart", "9tk", "Throwing Knife", 1, 2, 1, 0, 1, 2, 0, 0, 0, 31},
	{"Francisca", "9ta", "Throwing Axe", 1, 2, 1, 0, 1, 2, 0, 0, 0, 34},
	{"War Dart", "9bk", "Throwing Knife", 1, 2, 1, 0, 1, 2, 0, 0, 0, 39},
	{"Hurlbat", "9b8", "Throwing Axe", 2, 3, 1, 0, 1, 2, 0, 0, 0, 41},
	{"War Javelin", "9ja", "Javelin", 1, 3, 1, 0, 1, 2, 0, 0, 0, 30},
	{"Great Pilum", "9pi", "Javelin", 1, 3, 1, 0, 1, 2, 0, 0, 0, 37},
	{"Simbilan", "9s9", "Javelin", 1, 3, 1, 0, 1, 2, 0, 0, 0, 40},
	{"Spiculum", "9gl", "Javelin", 1, 4, 1, 0, 1, 2, 0, 0, 0, 46},
	{"Harpoon", "9ts", "Javelin", 1, 4, 1, 0, 1, 2, 0, 0, 0, 51},
	{"War Spear", "9sr", "Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 33},
	{"Fuscina", "9tr", "Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 36},
	{"War Fork", "9br", "Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 41},
	{"Yari", "9st", "Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 44},
	{"Lance", "9p9", "Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 47},
	{"Lochaber Axe", "9b7", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 33},
	{"Bill", "9vo", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 37},
	{"Battle Scythe", "9s8", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 40},
	{"Partizan", "9pa", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 35},
	{"Bec-de-Corbin", "9h9", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 51},
	{"Grim Scythe", "9wc", "Polearm", 2, 4, 0, 0, 0, 2, 0, 0, 0, 55},
	{"Jo Staff", "8ss", "Staff", 1, 3, 0, 0, 0, 2, 0, 0, 0, 30},
	{"Quarterstaff", "8ls", "Staff", 1, 4, 0, 0, 0, 2, 0, 0, 0, 35},
	{"Cedar Staff", "8cs", "Staff", 1, 4, 0, 0, 0, 2, 0, 0, 0, 38},
	{"Gothic Staff", "8bs", "Staff", 1, 4, 0, 0, 0, 2, 0, 0, 0, 42},
	{"Rune Staff", "8ws", "Staff", 2, 4, 0, 0, 0, 2, 0, 0, 0, 47},
	{"Edge Bow", "8sb", "Bow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 30},
	{"Razor Bow", "8hb", "Bow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 33},
	{"Cedar Bow", "8lb", "Bow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 35},
	{"Double Bow", "8cb", "Bow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 39},
	{"Short Siege Bow", "8s8", "Bow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 43},
	{"Large Siege Bow", "8l8", "Bow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 46},
	{"Rune Bow", "8sw", "Bow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 49},
	{"Gothic Bow", "8lw", "Bow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 52},
	{"Arbalest", "8lx", "Crossbow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 34},
	{"Siege Crossbow", "8mx", "Crossbow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 40},
	{"Ballista", "8hx", "Crossbow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 47},
	{"Chu-Ko-Nu", "8rx", "Crossbow", 2, 3, 0, 0, 0, 2, 0, 0, 0, 54},
	{"Khalim's Flail", "qf1", "Mace", 2, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Khalim's Will", "qf2", "Mace", 2, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	{"Katar", "ktr", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 1},
	{"Wrist Blade", "wrb", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 9},
	{"Hatchet Hands", "axf", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 12},
	{"Cestus", "ces", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 15},
	{"Claws", "clw", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 18},
	{"Blade Talons", "btl", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 21},
	{"Scissors Katar", "skr", "Assassin Katar", 1, 3, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Quhab", "9ar", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 28},
	{"Wrist Spike", "9wb", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 32},
	{"Fascia", "9xf", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 36},
	{"Hand Scythe", "9cs", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 41},
	{"Greater Claws", "9lw", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 45},
	{"Greater Talons", "9tw", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 50},
	{"Scissors Quhab", "9qr", "Assassin Katar", 1, 3, 0, 0, 0, 2, 0, 0, 0, 54},
	{"Suwayyah", "7ar", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 59},
	{"Wrist Sword", "7wb", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 62},
	{"War Fist", "7xf", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 68},
	{"Battle Cestus", "7cs", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 73},
	{"Feral Claws", "7lw", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 78},
	{"Runic Talons", "7tw", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 81},
	{"Scissors Suwayyah", "7qr", "Assassin Katar", 1, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Tomahawk", "7ha", "Axe", 1, 3, 0, 0, 0, 3, 0, 0, 0, 54},
	{"Small Crescent", "7ax", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 61},
	{"Ettin Axe", "72a", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 70},
	{"War Spike", "7mp", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 79},
	{"Berserker Axe", "7wa", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Feral Axe", "7la", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 57},
	{"Silver-edged Axe", "7ba", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 65},
	{"Decapitator", "7bt", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 73},
	{"Champion Axe", "7ga", "Axe", 2, 4, 0, 0, 0, 3, 0, 0, 0, 82},
	{"Glorious Axe", "7gi", "Axe", 2, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Polished Wand", "7wn", "Wand", 1, 2, 0, 0, 0, 3, 0, 0, 0, 55},
	{"Ghost Wand", "7yw", "Wand", 1, 2, 0, 0, 0, 3, 0, 0, 0, 65},
	{"Lich Wand", "7bw", "Wand", 1, 2, 0, 0, 0, 3, 0, 0, 0, 75},
	{"Unearthed Wand", "7gw", "Wand", 1, 2, 0, 0, 0, 3, 0, 0, 0, 86},
	{"Truncheon", "7cl", "Club", 1, 3, 0, 0, 0, 3, 0, 0, 0, 52},
	{"Mighty Scepter", "7sc", "Scepter", 1, 3, 0, 0, 0, 3, 0, 0, 0, 62},
	{"Seraph Rod", "7qs", "Scepter", 1, 3, 0, 0, 0, 3, 0, 0, 0, 76},
	{"Caduceus", "7ws", "Scepter", 2, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Tyrant Club", "7sp", "Club", 1, 3, 0, 0, 0, 3, 0, 0, 0, 57},
	{"Reinforced Mace", "7ma", "Mace", 1, 3, 0, 0, 0, 3, 0, 0, 0, 63},
	{"Devil Star", "7mt", "Mace", 1, 3, 0, 0, 0, 3, 0, 0, 0, 70},
	{"Scourge", "7fl", "Mace", 2, 3, 0, 0, 0, 3, 0, 0, 0, 76},
	{"Legendary Mallet", "7wh", "Hammer", 2, 3, 0, 0, 0, 3, 0, 0, 0, 82},
	{"Ogre Maul", "7m7", "Hammer", 2, 4, 0, 0, 0, 3, 0, 0, 0, 69},
	{"Thunder Maul", "7gm", "Hammer", 2, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Falcata", "7ss", "Sword", 1, 3, 0, 0, 0, 3, 0, 0, 0, 56},
	{"Ataghan", "7sm", "Sword", 1, 3, 0, 0, 0, 3, 0, 0, 0, 61},
	{"Elegant Blade", "7sb", "Sword", 1, 3, 0, 0, 0, 3, 0, 0, 0, 63},
	{"Hydra Edge", "7fc", "Sword", 1, 3, 0, 0, 0, 3, 0, 0, 0, 69},
	{"Phase Blade", "7cr", "Sword", 2, 3, 0, 0, 0, 3, 0, 0, 0, 73},
	{"Conquest Sword", "7bs", "Sword", 2, 3, 0, 0, 0, 3, 0, 0, 0, 78},
	{"Cryptic Sword", "7ls", "Sword", 2, 3, 0, 0, 0, 3, 0, 0, 0, 82},
	{"Mythical Sword", "7wd", "Sword", 1, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Legend Sword", "72h", "Sword", 1, 4, 0, 0, 0, 3, 0, 0, 0, 59},
	{"Highland Blade", "7cm", "Sword", 1, 4, 0, 0, 0, 3, 0, 0, 0, 66},
	{"Balrog Blade", "7gs", "Sword", 1, 4, 0, 0, 0, 3, 0, 0, 0, 71},
	{"Champion Sword", "7b7", "Sword", 1, 4, 0, 0, 0, 3, 0, 0, 0, 77},
	{"Colossus Sword", "7fb", "Sword", 2, 4, 0, 0, 0, 3, 0, 0, 0, 80},
	{"Colossus Blade", "7gd", "Sword", 2, 4, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Bone Knife", "7dg", "Dagger", 1, 2, 0, 0, 0, 3, 0, 0, 0, 58},
	{"Mithril Point", "7di", "Dagger", 1, 2, 0, 0, 0, 3, 0, 0, 0, 70},
	{"Fanged Knife", "7kr", "Dagger", 1, 3, 0, 0, 0, 3, 0, 0, 0, 83},
	{"Legend Spike", "7bl", "Dagger", 1, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Flying Knife", "7tk", "Throwing Knife", 1, 2, 1, 0, 1, 3, 0, 0, 0, 64},
	{"Flying Axe", "7ta", "Throwing Axe", 1, 2, 1, 0, 1, 3, 0, 0, 0, 56},
	{"Winged Knife", "7bk", "Throwing Knife", 1, 2, 1, 0, 1, 3, 0, 0, 0, 77},
	{"Winged Axe", "7b8", "Throwing Axe", 2, 3, 1, 0, 1, 3, 0, 0, 0, 80},
	{"Hyperion Javelin", "7ja", "Javelin", 1, 3, 1, 0, 1, 3, 0, 0, 0, 54},
	{"Stygian Pilum", "7pi", "Javelin", 1, 3, 1, 0, 1, 3, 0, 0, 0, 62},
	{"Balrog Spear", "7s7", "Javelin", 1, 3, 1, 0, 1, 3, 0, 0, 0, 71},
	{"Ghost Glaive", "7gl", "Javelin", 1, 4, 1, 0, 1, 3, 0, 0, 0, 79},
	{"Winged Harpoon", "7ts", "Javelin", 1, 4, 1, 0, 1, 3, 0, 0, 0, 85},
	{"Hyperion Spear", "7sr", "Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 58},
	{"Stygian Pike", "7tr", "Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 66},
	{"Mancatcher", "7br", "Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 74},
	{"Ghost Spear", "7st", "Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 83},
	{"War Pike", "7p7", "Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Ogre Axe", "7o7", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 60},
	{"Colossus Voulge", "7vo", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 64},
	{"Thresher", "7s8", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 71},
	{"Cryptic Axe", "7pa", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 79},
	{"Great Poleaxe", "7h7", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 84},
	{"Giant Thresher", "7wc", "Polearm", 2, 4, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Walking Stick", "6ss", "Staff", 1, 3, 0, 0, 0, 3, 0, 0, 0, 58},
	{"Stalagmite", "6ls", "Staff", 1, 4, 0, 0, 0, 3, 0, 0, 0, 66},
	{"Elder Staff", "6cs", "Staff", 1, 4, 0, 0, 0, 3, 0, 0, 0, 74},
	{"Shillelagh", "6bs", "Staff", 1, 4, 0, 0, 0, 3, 0, 0, 0, 83},
	{"Archon Staff", "6ws", "Staff", 2, 4, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Spider Bow", "6sb", "Bow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 55},
	{"Blade Bow", "6hb", "Bow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 60},
	{"Shadow Bow", "6lb", "Bow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 63},
	{"Great Bow", "6cb", "Bow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 68},
	{"Diamond Bow", "6s7", "Bow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 72},
	{"Crusader Bow", "6l7", "Bow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 77},
	{"Ward Bow", "6sw", "Bow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 80},
	{"Hydra Bow", "6lw", "Bow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Pellet Bow", "6lx", "Crossbow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 57},
	{"Gorgon Crossbow", "6mx", "Crossbow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 67},
	{"Colossus Crossbow", "6hx", "Crossbow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 75},
	{"Demon Crossbow", "6rx", "Crossbow", 2, 3, 0, 0, 0, 3, 0, 0, 0, 84},
	{"Eagle Orb", "ob1", "Sorceress Orb", 1, 2, 0, 0, 0, 1, 0, 0, 0, 1},
	{"Sacred Globe", "ob2", "Sorceress Orb", 1, 2, 0, 0, 0, 1, 0, 0, 0, 8},
	{"Smoked Sphere", "ob3", "Sorceress Orb", 1, 2, 0, 0, 0, 1, 0, 0, 0, 12},
	{"Clasped Orb", "ob4", "Sorceress Orb", 1, 2, 0, 0, 0, 1, 0, 0, 0, 17},
	{"Jared's Stone", "ob5", "Sorceress Orb", 1, 3, 0, 0, 0, 1, 0, 0, 0, 24},
	{"Stag Bow", "am1", "Amazon Bow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 18},
	{"Reflex Bow", "am2", "Amazon Bow", 2, 4, 0, 0, 0, 1, 0, 0, 0, 27},
	{"Maiden Spear", "am3", "Amazon Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 18},
	{"Maiden Pike", "am4", "Amazon Spear", 2, 4, 0, 0, 0, 1, 0, 0, 0, 27},
	{"Maiden Javelin", "am5", "Amazon Javelin", 1, 3, 1, 0, 1, 1, 0, 0, 0, 23},
	{"Glowing Orb", "ob6", "Sorceress Orb", 1, 2, 0, 0, 0, 2, 0, 0, 0, 32},
	{"Crystalline Globe", "ob7", "Sorceress Orb", 1, 2, 0, 0, 0, 2, 0, 0, 0, 37},
	{"Cloudy Sphere", "ob8", "Sorceress Orb", 1, 2, 0, 0, 0, 2, 0, 0, 0, 41},
	{"Sparkling Ball", "ob9", "Sorceress Orb", 1, 2, 0, 0, 0, 2, 0, 0, 0, 46},
	{"Swirling Crystal", "oba", "Sorceress Orb", 1, 3, 0, 0, 0, 2, 0, 0, 0, 50},
	{"Ashwood Bow", "am6", "Amazon Bow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 39},
	{"Ceremonial Bow", "am7", "Amazon Bow", 2, 4, 0, 0, 0, 2, 0, 0, 0, 47},
	{"Ceremonial Spear", "am8", "Amazon Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 43},
	{"Ceremonial Pike", "am9", "Amazon Spear", 2, 4, 0, 0, 0, 2, 0, 0, 0, 51},
	{"Ceremonial Javelin", "ama", "Amazon Javelin", 1, 3, 1, 0, 1, 2, 0, 0, 0, 35},
	{"Heavenly Stone", "obb", "Sorceress Orb", 1, 2, 0, 0, 0, 3, 0, 0, 0, 59},
	{"Eldritch Orb", "obc", "Sorceress Orb", 1, 2, 0, 0, 0, 3, 0, 0, 0, 67},
	{"Demon Heart", "obd", "Sorceress Orb", 1, 2, 0, 0, 0, 3, 0, 0, 0, 75},
	{"Vortex Orb", "obe", "Sorceress Orb", 1, 2, 0, 0, 0, 3, 0, 0, 0, 84},
	{"Dimensional Shard", "obf", "Sorceress Orb", 1, 3, 0, 0, 0, 3, 0, 0, 0, 85},
	{"Matriarchal Bow", "amb", "Amazon Bow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 53},
	{"Grand Matron Bow", "amc", "Amazon Bow", 2, 4, 0, 0, 0, 3, 0, 0, 0, 78},
	{"Matriarchal Spear", "amd", "Amazon Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 61},
	{"Matriarchal Pike", "ame", "Amazon Spear", 2, 4, 0, 0, 0, 3, 0, 0, 0, 81},
	{"Matriarchal Javelin", "amf", "Amazon Javelin", 1, 3, 1, 0, 1, 3, 0, 0, 0, 65}
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

//D2Common.#10943(guessed Arg1, Arg2, Arg3)
//$ + 66346   \.C2 0C00       RETN 0C


// If we find the temp file with MPQ info, use it; otherwise, fall back on the hardcoded lists.
void InitializeMPQData() {
	if (initialized) return;
	
	char* end;
	short lastID = -1;


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
		bits->name = to_string(id);
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

		if (strncmp(d->szultracode, d->szcode, 4) == 0) {
			flags |= ITEM_GROUP_ELITE;
		} else if(strncmp(d->szubercode, d->szcode, 4) == 0) {
			flags |= ITEM_GROUP_EXCEPTIONAL;
		} else {
			flags |= ITEM_GROUP_NORMAL;
		}
		ItemTypesTxt* mainType = &(*p_D2COMMON_sgptDataTable)->pItemsTypeTxt[d->wtype];

		ItemAttributes* attrs = new ItemAttributes();
		std::string code(d->szcode, 4);
		attrs->name = UnicodeToAnsi(D2LANG_GetLocaleTextFromString(Trim(code).c_str()));
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
	BH::moduleManager->MpqLoaded();
}
