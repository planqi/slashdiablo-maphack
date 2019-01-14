#include "ItemDisplay.h"
#include "Item.h"

// All the types able to be combined with the + operator
#define COMBO_STATS					\
	{"LIFE", STAT_MAXHP},			\
	{"MANA", STAT_MAXMANA},			\
	{"STR", STAT_STRENGTH},			\
	{"DEX", STAT_DEXTERITY},		\
	{"CRES", STAT_COLDRESIST},		\
	{"FRES", STAT_FIRERESIST},		\
	{"LRES", STAT_LIGHTNINGRESIST},	\
	{"PRES", STAT_POISONRESIST},	\
	{"MINDMG", STAT_MINIMUMDAMAGE},	\
	{"MAXDMG", STAT_MAXIMUMDAMAGE},	\

// All colors here must also be defined in MAP_COLOR_REPLACEMENTS
#define COLOR_REPLACEMENTS	\
	{"WHITE", "ÿc0"},		\
	{"RED", "ÿc1"},			\
	{"GREEN", "ÿc2"},		\
	{"BLUE", "ÿc3"},		\
	{"GOLD", "ÿc4"},		\
	{"GRAY", "ÿc5"},		\
	{"BLACK", "ÿc6"},		\
	{"TAN", "ÿc7"},			\
	{"ORANGE", "ÿc8"},		\
	{"YELLOW", "ÿc9"},		\
	{"PURPLE", "ÿc;"},		\
	{"DARK_GREEN", "ÿc:"}

#define MAP_COLOR_WHITE     0x20

#define MAP_COLOR_REPLACEMENTS	\
	{"WHITE", 0x20},		\
	{"RED", 0x0A},			\
	{"GREEN", 0x84},		\
	{"BLUE", 0x97},			\
	{"GOLD", 0x0D},			\
	{"GRAY", 0xD0},			\
	{"BLACK", 0x00},		\
	{"TAN", 0x5A},			\
	{"ORANGE", 0x60},		\
	{"YELLOW", 0x0C},		\
	{"PURPLE", 0x9B},		\
	{"DARK_GREEN", 0x76}

enum Operation {
	EQUAL,
	GREATER_THAN,
	LESS_THAN,
	NONE
};

SkillReplace skills[] = { 
	COMBO_STATS 
};

std::map<std::string, int> UnknownItemCodes;
vector<pair<string, string>> rules;
vector<Rule*> RuleList;
vector<Rule*> MapRuleList;
vector<Rule*> IgnoreRuleList;
BYTE LastConditionType;

TrueCondition *trueCondition = new TrueCondition();
FalseCondition *falseCondition = new FalseCondition();

// Helper function to get a list of strings
vector<string> split(const string &s, char delim) {
	vector<string> result;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		result.push_back(item);
	}
	return result;
}

char* GemLevels[] = {
	"NONE",
	"Chipped",
	"Flawed",
	"Normal",
	"Flawless",
	"Perfect"
};

char* GemTypes[] = {
	"NONE",
	"Amethyst",
	"Diamond",
	"Emerald",
	"Ruby",
	"Sapphire",
	"Topaz",
	"Skull"
};

bool IsGem(ItemAttributes *attrs) {
	return (attrs->flags2 & ITEM_GROUP_GEM) > 0;
}

BYTE GetGemLevel(ItemAttributes *attrs) {
	if (attrs->flags2 & ITEM_GROUP_CHIPPED) {
		return 1;
	} else if (attrs->flags2 & ITEM_GROUP_FLAWED) {
		return 2;
	} else if (attrs->flags2 & ITEM_GROUP_REGULAR) {
		return 3;
	} else if (attrs->flags2 & ITEM_GROUP_FLAWLESS) {
		return 4;
	} else if (attrs->flags2 & ITEM_GROUP_PERFECT) {
		return 5;
	}
	return 0;
}

char *GetGemLevelString(BYTE level) {
	return GemLevels[level];
}

BYTE GetGemType(ItemAttributes *attrs) {
	if (attrs->flags2 & ITEM_GROUP_AMETHYST) {
		return 1;
	} else if (attrs->flags2 & ITEM_GROUP_DIAMOND) {
		return 2;
	} else if (attrs->flags2 & ITEM_GROUP_EMERALD) {
		return 3;
	} else if (attrs->flags2 & ITEM_GROUP_RUBY) {
		return 4;
	} else if (attrs->flags2 & ITEM_GROUP_SAPPHIRE) {
		return 5;
	} else if (attrs->flags2 & ITEM_GROUP_TOPAZ) {
		return 6;
	} else if (attrs->flags2 & ITEM_GROUP_SKULL) {
		return 7;
	}
	return 0;
}

char *GetGemTypeString(BYTE type) {
	return GemTypes[type];
}

bool IsRune(ItemAttributes *attrs) {
	return (attrs->flags2 & ITEM_GROUP_RUNE) > 0;
}

BYTE RuneNumberFromItemCode(char *code){
	return (BYTE)(((code[1] - '0') * 10) + code[2] - '0');
}

void GetItemName(UnitItemInfo *uInfo, string &name) {
	for (vector<Rule*>::iterator it = RuleList.begin(); it != RuleList.end(); it++) {
		if ((*it)->Evaluate(uInfo, NULL)) {
			SubstituteNameVariables(uInfo, name, &(*it)->action);
			if ((*it)->action.stopProcessing) {
				break;
			}
		}
	}
}

void SubstituteNameVariables(UnitItemInfo *uInfo, string &name, Action *action) {
	char origName[128], sockets[4], code[4], ilvl[4], alvl[4], runename[16] = "", runenum[4] = "0";
	char gemtype[16] = "", gemlevel[16] = "", sellValue[16] = "", statVal[16] = "";
	char lvlreq[4], wpnspd[4], rangeadder[4];

	UnitAny *item = uInfo->item;
	ItemText *txt = D2COMMON_GetItemText(item->dwTxtFileNo);
	char *szCode = txt->szCode;
	code[0] = szCode[0];
	code[1] = szCode[1];
	code[2] = szCode[2];
	code[3] = '\0';
	sprintf_s(sockets, "%d", D2COMMON_GetUnitStat(item, STAT_SOCKETS, 0));
	sprintf_s(ilvl, "%d", item->pItemData->dwItemLevel);
	sprintf_s(alvl, "%d", GetAffixLevel((BYTE)item->pItemData->dwItemLevel, (BYTE)uInfo->attrs->qualityLevel, uInfo->attrs->magicLevel));
	sprintf_s(origName, "%s", name.c_str());

	sprintf_s(lvlreq, "%d", GetRequiredLevel(uInfo->item));
	sprintf_s(wpnspd, "%d", txt->speed); //Add these as matchable stats too, maybe?
	sprintf_s(rangeadder, "%d", txt->rangeadder);

	UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	if (pUnit && txt->fQuest == 0) {
		sprintf_s(sellValue, "%d", D2COMMON_GetItemPrice(pUnit, item, D2CLIENT_GetDifficulty(), (DWORD)D2CLIENT_GetQuestInfo(), 0x201, 1));
	}

	if (IsRune(uInfo->attrs)) {
		sprintf_s(runenum, "%d", RuneNumberFromItemCode(code));
		sprintf_s(runename, name.substr(0, name.find(' ')).c_str());
	} else if (IsGem(uInfo->attrs)) {
		sprintf_s(gemlevel, "%s", GetGemLevelString(GetGemLevel(uInfo->attrs)));
		sprintf_s(gemtype, "%s", GetGemTypeString(GetGemType(uInfo->attrs)));
	}
	ActionReplace replacements[] = {
		{"NAME", origName},
		{"SOCKETS", sockets},
		{"RUNENUM", runenum},
		{"RUNENAME", runename},
		{"GEMLEVEL", gemlevel},
		{"GEMTYPE", gemtype},
		{"ILVL", ilvl},
		{"ALVL", alvl},
		{"LVLREQ", lvlreq},
		{"WPNSPD", wpnspd},
		{"RANGE", rangeadder},
		{"CODE", code},
		{"NL", "\n"},
		{"PRICE", sellValue},
		COLOR_REPLACEMENTS
	};
	name.assign(action->name);
	for (int n = 0; n < sizeof(replacements) / sizeof(replacements[0]); n++) {
		while (name.find("%" + replacements[n].key + "%") != string::npos) {
			name.replace(name.find("%" + replacements[n].key + "%"), replacements[n].key.length() + 2, replacements[n].value);
		}
	}

	// stat replacements
	if (name.find("%STAT-") != string::npos) {
		std::regex stat_reg("%STAT-([0-9]{1,4})%", std::regex_constants::ECMAScript);
		std::smatch stat_match;

		while (std::regex_search(name, stat_match, stat_reg)) {
			int stat = stoi(stat_match[1].str(), nullptr, 10);
			statVal[0] = '\0';
			if (stat <= (int)STAT_MAX) {
				auto value = D2COMMON_GetUnitStat(item, stat, 0);
				// Hp and mana need adjusting
				if (stat == 7 || stat == 9)
					value /= 256;
				sprintf_s(statVal, "%d", value);
			}
			name.replace(
					stat_match.prefix().length(),
					stat_match[0].length(), statVal);
		}
	}
}

BYTE GetAffixLevel(BYTE ilvl, BYTE qlvl, BYTE mlvl) {
	if (ilvl > 99) {
		ilvl = 99;
	}
	if (qlvl > ilvl) {
		ilvl = qlvl;
	}
	if (mlvl > 0) {
		return ilvl + mlvl > 99 ? 99 : ilvl + mlvl;
	}
	return ((ilvl) < (99 - ((qlvl)/2)) ? (ilvl) - ((qlvl)/2) : (ilvl) * 2 - 99);
}

// Returns the (lowest) level requirement (for any class) of an item
BYTE GetRequiredLevel(UnitAny* item) {
	// Some crafted items can supposedly go above 100, but it's practically the same as 100
	BYTE rlvl = 100;

	// The unit for which the required level is calculated
	UnitAny* character = D2CLIENT_GetPlayerUnit();

	// Extra checks for these as they can have charges
	if (item->pItemData->dwQuality == ITEM_QUALITY_RARE || item->pItemData->dwQuality == ITEM_QUALITY_MAGIC) {

		// Save the original class of the character (0-6)
		DWORD temp = character->dwTxtFileNo;

		// Pretend to be every class once, use the lowest req lvl (for charged items)
		for (DWORD i = 0; i < 7; i++) {

			character->dwTxtFileNo = i;
			BYTE temprlvl = (BYTE)D2COMMON_GetItemLevelRequirement(item, character);

			if (temprlvl < rlvl) {

				rlvl = temprlvl;
				//Only one class will have a lower req than the others, so if a lower one is found we can stop
				if (i > 0) { break; }
			}
		}
		// Go back to being original class
		character->dwTxtFileNo = temp;
	} else {
		rlvl = (BYTE)D2COMMON_GetItemLevelRequirement(item, character);
	}

	return rlvl;
}

BYTE GetOperation(string *op) {
	if (op->length() < 1) {
		return NONE;
	} else if ((*op)[0] == '=') {
		return EQUAL;
	} else if ((*op)[0] == '<') {
		return LESS_THAN;
	} else if ((*op)[0] == '>') {
		return GREATER_THAN;
	}
	return NONE;
}

unsigned int GetItemCodeIndex(char codeChar) {
	// Characters '0'-'9' map to 0-9, and a-z map to 10-35
	return codeChar - (codeChar < 90 ? 48 : 87);
}

bool IntegerCompare(unsigned int Lvalue, BYTE operation, unsigned int Rvalue) {
	switch (operation) {
	case EQUAL:
		return Lvalue == Rvalue;
	case GREATER_THAN:
		return Lvalue > Rvalue;
	case LESS_THAN:
		return Lvalue < Rvalue;
	default:
		return false;
	}
}

namespace ItemDisplay {
	bool item_display_initialized = false;
	void InitializeItemRules() {
		if (item_display_initialized) return;
		if (!IsInitialized()){
			return;
		}



		item_display_initialized = true;
		rules.clear();
		BH::config->ReadMapList("ItemDisplay", rules);
		for (unsigned int i = 0; i < rules.size(); i++) {
			string buf;
			stringstream ss(rules[i].first);
			vector<string> tokens;
			while (ss >> buf) {
				tokens.push_back(buf);
			}

			LastConditionType = CT_None;
			Rule *r = new Rule();
			vector<Condition*> RawConditions;
			for (vector<string>::iterator tok = tokens.begin(); tok < tokens.end(); tok++) {
				Condition::BuildConditions(RawConditions, (*tok));
			}
			Condition::ProcessConditions(RawConditions, r->conditions);
			BuildAction(&(rules[i].second), &(r->action));

			RuleList.push_back(r);
			if (r->action.colorOnMap != UNDEFINED_COLOR ||
					r->action.borderColor != UNDEFINED_COLOR ||
					r->action.dotColor != UNDEFINED_COLOR ||
					r->action.pxColor != UNDEFINED_COLOR ||
					r->action.lineColor != UNDEFINED_COLOR) {
				MapRuleList.push_back(r);
			}
			else if (r->action.name.length() == 0) {
				IgnoreRuleList.push_back(r);
			}
		}
	}

	void UninitializeItemRules() {
		item_display_initialized = false;
		RuleList.clear();
		MapRuleList.clear();
		IgnoreRuleList.clear();
	}
}

void BuildAction(string *str, Action *act) {
	act->name = string(str->c_str());

	// upcase all text in a %replacement_string%
	// for some reason \w wasn't catching _, so I added it to the groups
	std::regex replace_reg(
			R"(^(?:(?:%[^%]*%)|[^%])*%((?:\w|_|-)*?[a-z]+?(?:\w|_|-)*?)%)",
			std::regex_constants::ECMAScript);
	std::smatch replace_match;
	while (std::regex_search(act->name, replace_match, replace_reg)) {
		auto offset = replace_match[1].first - act->name.begin();
		std::transform(
				replace_match[1].first,
				replace_match[1].second,
				act->name.begin() + offset,
				toupper
				);
	}

	// new stuff:
	act->borderColor = ParseMapColor(act, "BORDER");
	act->colorOnMap = ParseMapColor(act, "MAP");
	act->dotColor = ParseMapColor(act, "DOT");
	act->pxColor = ParseMapColor(act, "PX");
	act->lineColor = ParseMapColor(act, "LINE");
	act->notifyColor = ParseMapColor(act, "NOTIFY");

	// legacy support:
	size_t map = act->name.find("%MAP%");
	if (map != string::npos) {
		int mapColor = MAP_COLOR_WHITE;
		size_t lastColorPos = 0;
		ColorReplace colors[] = {
			MAP_COLOR_REPLACEMENTS
		};
		for (int n = 0; n < sizeof(colors) / sizeof(colors[0]); n++) {
			size_t pos = act->name.find("%" + colors[n].key + "%");
			if (pos != string::npos && pos < map && pos >= lastColorPos) {
				mapColor = colors[n].value;
				lastColorPos = pos;
			}
		}

		act->name.replace(map, 5, "");
		act->colorOnMap = mapColor;
		if (act->borderColor == UNDEFINED_COLOR)
			act->borderColor = act->colorOnMap;
	}

	size_t done = act->name.find("%CONTINUE%");
	if (done != string::npos) {
		act->name.replace(done, 10, "");
		act->stopProcessing = false;
	}
}

int ParseMapColor(Action *act, const string& key_string) {
	std::regex pattern("%" + key_string + "-([a-f0-9]{1,4})%",
		std::regex_constants::ECMAScript | std::regex_constants::icase);
	int color = UNDEFINED_COLOR;
	std::smatch the_match;

	if (std::regex_search(act->name, the_match, pattern)) {
		color = stoi(the_match[1].str(), nullptr, 16);
		act->name.replace(
				the_match.prefix().length(),
				the_match[0].length(), "");
	}
	return color;
}

const string Condition::tokenDelims = "<=>";

// Implements the shunting-yard algorithm to evaluate condition expressions
// Returns a vector of conditions in Reverse Polish Notation
void Condition::ProcessConditions(vector<Condition*> &inputConditions, vector<Condition*> &processedConditions) {
	vector<Condition*> conditionStack;
	unsigned int size = inputConditions.size();
	for (unsigned int c = 0; c < size; c++) {
		Condition *input = inputConditions[c];
		if (input->conditionType == CT_Operand) {
			processedConditions.push_back(input);
		} else if (input->conditionType == CT_BinaryOperator || input->conditionType == CT_NegationOperator) {
			bool go = true;
			while (go) {
				if (conditionStack.size() > 0) {
					Condition *stack = conditionStack.back();
					if ((stack->conditionType == CT_NegationOperator || stack->conditionType == CT_BinaryOperator) &&
						input->conditionType == CT_BinaryOperator) {
						conditionStack.pop_back();
						processedConditions.push_back(stack);
					} else {
						go = false;
					}
				} else {
					go = false;
				}
			}
			conditionStack.push_back(input);
		} else if (input->conditionType == CT_LeftParen) {
			conditionStack.push_back(input);
		} else if (input->conditionType == CT_RightParen) {
			bool foundLeftParen = false;
			while (conditionStack.size() > 0 && !foundLeftParen) {
				Condition *stack = conditionStack.back();
				conditionStack.pop_back();
				if (stack->conditionType == CT_LeftParen) {
					foundLeftParen = true;
					break;
				} else {
					processedConditions.push_back(stack);
				}
			}
			if (!foundLeftParen) {
				// TODO: find a way to report error
				return;
			}
		}
	}
	while (conditionStack.size() > 0) {
		Condition *next = conditionStack.back();
		conditionStack.pop_back();
		if (next->conditionType == CT_LeftParen || next->conditionType == CT_RightParen) {
			// TODO: find a way to report error
			break;
		} else {
			processedConditions.push_back(next);
		}
	}
}

void Condition::BuildConditions(vector<Condition*> &conditions, string token) {
	vector<Condition*> endConditions;
	int i;

	// Since we don't have a real parser, things will break if [!()] appear in
	// the middle of a token (e.g. "(X AND Y)(A AND B)")

	// Look for syntax characters at the beginning of the token
	for (i = 0; i < (int)token.length(); i++) {
		if (token[i] == '!') {
			Condition::AddNonOperand(conditions, new NegationOperator());
		} else if (token[i] == '(') {
			Condition::AddNonOperand(conditions, new LeftParen());
		} else if (token[i] == ')') {
			Condition::AddNonOperand(conditions, new RightParen());
		} else {
			break;
		}
	}
	token.erase(0, i);

	// Look for syntax characters at the end of the token
	for (i = token.length() - 1; i >= 0; i--) {
		if (token[i] == '!') {
			endConditions.insert(endConditions.begin(), new NegationOperator());
		} else if (token[i] == '(') {
			endConditions.insert(endConditions.begin(), new LeftParen());
		} else if (token[i] == ')') {
			endConditions.insert(endConditions.begin(), new RightParen());
		} else {
			break;
		}
	}
	if (i < (int)(token.length() - 1)) {
		token.erase(i + 1, string::npos);
	}

	size_t delPos = token.find_first_of(tokenDelims);
	string key;
	string delim = "";
	int value = 0;
	if (delPos != string::npos) {
		key = Trim(token.substr(0, delPos));
		delim = token.substr(delPos, 1);
		string valueStr = Trim(token.substr(delPos + 1));
		if (valueStr.length() > 0) {
			stringstream ss(valueStr);
			if ((ss >> value).fail()) {
				return;  // TODO: returning errors
			}
		}
	} else {
		key = token;
	}
	BYTE operation = GetOperation(&delim);

	unsigned int keylen = key.length();
	if (key.compare(0, 3, "AND") == 0 || key.compare(0, 2, "&&") == 0) {
		Condition::AddNonOperand(conditions, new AndOperator());
	} else if (key.compare(0, 2, "OR") == 0 || key.compare(0, 2, "||") == 0) {
		Condition::AddNonOperand(conditions, new OrOperator());
	} else if (keylen >= 3 && !(isupper(key[0]) || isupper(key[1]) || isupper(key[2]))) {
		Condition::AddOperand(conditions, new ItemCodeCondition(key.substr(0, 3).c_str()));
	} else if (key.find('+') != std::string::npos) {
		Condition::AddOperand(conditions, new AddCondition(key, operation, value));
	} else if (key.compare(0, 3, "ETH") == 0) {
		Condition::AddOperand(conditions, new FlagsCondition(ITEM_ETHEREAL));
	} else if (key.compare(0, 4, "SOCK") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_SOCKETS, 0, operation, value));
	} else if (key.compare(0, 3, "SET") == 0) {
		Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_SET));
	} else if (key.compare(0, 3, "MAG") == 0) {
		Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_MAGIC));
	} else if (key.compare(0, 4, "RARE") == 0) {
		Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_RARE));
	} else if (key.compare(0, 3, "UNI") == 0) {
		Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_UNIQUE));
	} else if (key.compare(0, 2, "RW") == 0) {
		Condition::AddOperand(conditions, new FlagsCondition(ITEM_RUNEWORD));
	} else if (key.compare(0, 4, "NMAG") == 0) {
		Condition::AddOperand(conditions, new NonMagicalCondition());
	} else if (key.compare(0, 3, "SUP") == 0) {
		Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_SUPERIOR));
	} else if (key.compare(0, 3, "INF") == 0) {
		Condition::AddOperand(conditions, new QualityCondition(ITEM_QUALITY_INFERIOR));
	} else if (key.compare(0, 4, "NORM") == 0) {
		Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_NORMAL));
	} else if (key.compare(0, 3, "EXC") == 0) {
		Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_EXCEPTIONAL));
	} else if (key.compare(0, 3, "ELT") == 0) {
		Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ELITE));
	} else if (key.compare(0, 2, "ID") == 0) {
		Condition::AddOperand(conditions, new FlagsCondition(ITEM_IDENTIFIED));
	} else if (key.compare(0, 4, "ILVL") == 0) {
		Condition::AddOperand(conditions, new ItemLevelCondition(operation, value));
	} else if (key.compare(0, 4, "ALVL") == 0) {
		Condition::AddOperand(conditions, new AffixLevelCondition(operation, value));
	} else if (key.compare(0, 4, "CLVL") == 0) {
		Condition::AddOperand(conditions, new CharStatCondition(STAT_LEVEL, 0, operation, value));
	} else if (key.compare(0, 4, "DIFF") == 0) {
		Condition::AddOperand(conditions, new DifficultyCondition(operation, value));
	} else if (key.compare(0, 4, "RUNE") == 0) {
		Condition::AddOperand(conditions, new RuneCondition(operation, value));
	} else if (key.compare(0, 4, "GOLD") == 0) {
		Condition::AddOperand(conditions, new GoldCondition(operation, value));
	} else if (key.compare(0, 7, "GEMTYPE") == 0) {
		Condition::AddOperand(conditions, new GemTypeCondition(operation, value));
	} else if (key.compare(0, 3, "GEM") == 0) {
		Condition::AddOperand(conditions, new GemLevelCondition(operation, value));
	} else if (key.compare(0, 2, "ED") == 0) {
		Condition::AddOperand(conditions, new EDCondition(operation, value));
	} else if (key.compare(0, 3, "DEF") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_DEFENSE, 0, operation, value));
	} else if (key.compare(0, 6, "MAXDUR") == 0) {
		Condition::AddOperand(conditions, new DurabilityCondition(operation, value));
	} else if (key.compare(0, 3, "RES") == 0) {
		Condition::AddOperand(conditions, new ResistAllCondition(operation, value));
	} else if (key.compare(0, 4, "FRES") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_FIRERESIST, 0, operation, value));
	} else if (key.compare(0, 4, "CRES") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_COLDRESIST, 0, operation, value));
	} else if (key.compare(0, 4, "LRES") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_LIGHTNINGRESIST, 0, operation, value));
	} else if (key.compare(0, 4, "PRES") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_POISONRESIST, 0, operation, value));
	} else if (key.compare(0, 3, "IAS") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_IAS, 0, operation, value));
	} else if (key.compare(0, 3, "FCR") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_FASTERCAST, 0, operation, value));
	} else if (key.compare(0, 3, "FHR") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_FASTERHITRECOVERY, 0, operation, value));
	} else if (key.compare(0, 3, "FBR") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_FASTERBLOCK, 0, operation, value));
	} else if (key.compare(0, 4, "LIFE") == 0) {
		// For unknown reasons, the game's internal HP stat is 256 for every 1 displayed on item
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_MAXHP, 0, operation, value * 256));
	} else if (key.compare(0, 4, "MANA") == 0) {
		// For unknown reasons, the game's internal Mana stat is 256 for every 1 displayed on item
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_MAXMANA, 0, operation, value * 256));
	} else if (key.compare(0, 6, "GOODSK") == 0) {
		Condition::AddOperand(conditions, new SkillListCondition(operation, CLASS_SKILLS, value));
	}else if (key.compare(0, 8, "GOODTBSK") == 0) {
		Condition::AddOperand(conditions, new SkillListCondition(operation, CLASS_TAB_SKILLS, value));
	} else if (key.compare(0, 5, "FOOLS") == 0) {
		Condition::AddOperand(conditions, new FoolsCondition());
	} else if (key.compare(0, 6, "LVLREQ") == 0) {
		Condition::AddOperand(conditions, new RequiredLevelCondition(operation, value));
	} else if (key.compare(0, 5, "ARPER") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_TOHITPERCENT, 0, operation, value));
	} else if (key.compare(0, 5, "MFIND") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_MAGICFIND, 0, operation, value));
	} else if (key.compare(0, 5, "GFIND") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_GOLDFIND, 0, operation, value));
	} else if (key.compare(0, 3, "STR") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_STRENGTH, 0, operation, value));
	} else if (key.compare(0, 3, "DEX") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_DEXTERITY, 0, operation, value));
	} else if (key.compare(0, 3, "FRW") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_FASTERRUNWALK, 0, operation, value));
	} else if (key.compare(0, 6, "MINDMG") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_MINIMUMDAMAGE, 0, operation, value));
	} else if (key.compare(0, 6, "MAXDMG") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_MAXIMUMDAMAGE, 0, operation, value));
	} else if (key.compare(0, 2, "AR") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_ATTACKRATING, 0, operation, value));
	} else if (key.compare(0, 3, "DTM") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_DAMAGETOMANA, 0, operation, value));
	} else if (key.compare(0, 4, "MAEK") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_MANAAFTEREACHKILL, 0, operation, value));
	} else if (key.compare(0, 7, "REPLIFE") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_REPLENISHLIFE, 0, operation, value));
	} else if (key.compare(0, 8, "REPQUANT") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_REPLENISHESQUANTITY, 0, operation, value));
	} else if (key.compare(0, 6, "REPAIR") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_REPAIRSDURABILITY, 0, operation, value));
	} else if (key.compare(0, 5, "ARMOR") == 0) {
		Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ALLARMOR));
	} else if (key.compare(0, 2, "EQ") == 0 && keylen == 3) {
		if (key[2] == '1') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_HELM));
		} else if (key[2] == '2') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ARMOR));
		} else if (key[2] == '3') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SHIELD));
		} else if (key[2] == '4') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_GLOVES));
		} else if (key[2] == '5') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BOOTS));
		} else if (key[2] == '6') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BELT));
		} else if (key[2] == '7') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_CIRCLET));
		}
	} else if (key.compare(0, 2, "CL") == 0 && keylen == 3) {
		if (key[2] == '1') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_DRUID_PELT));
		} else if (key[2] == '2') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BARBARIAN_HELM));
		} else if (key[2] == '3') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_PALADIN_SHIELD));
		} else if (key[2] == '4') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_NECROMANCER_SHIELD));
		} else if (key[2] == '5') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ASSASSIN_KATAR));
		} else if (key[2] == '6') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SORCERESS_ORB));
		} else if (key[2] == '7') {
			Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_AMAZON_WEAPON));
		}
	} else if (key.compare(0, 6, "WEAPON") == 0) {
		Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_ALLWEAPON));
	} else if (key.compare(0, 2, "WP") == 0) {
		if (keylen >= 3) {
			if (key[2] == '1') {
				if (keylen >= 4 && key[3] == '0') {
					Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_CROSSBOW));
				} else if (keylen >= 4 && key[3] == '1') {
					Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_STAFF));
				} else if (keylen >= 4 && key[3] == '2') {
					Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_WAND));
				} else if (keylen >= 4 && key[3] == '3') {
					Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SCEPTER));
				} else {
					Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_AXE));
				}
			} else if (key[2] == '2') {
				Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_MACE));
			} else if (key[2] == '3') {
				Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SWORD));
			} else if (key[2] == '4') {
				Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_DAGGER));
			} else if (key[2] == '5') {
				Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_THROWING));
			} else if (key[2] == '6') {
				Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_JAVELIN));
			} else if (key[2] == '7') {
				Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_SPEAR));
			} else if (key[2] == '8') {
				Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_POLEARM));
			} else if (key[2] == '9') {
				Condition::AddOperand(conditions, new ItemGroupCondition(ITEM_GROUP_BOW));
			}
		}
	} else if (key.compare(0, 2, "SK") == 0) {
		int num = -1;
		stringstream ss(key.substr(2));
		if ((ss >> num).fail() || num < 0 || num > (int)SKILL_MAX) {
			return;
		}
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_SINGLESKILL, num, operation, value));
	} else if (key.compare(0, 2, "OS") == 0) {
		int num = -1;
		stringstream ss(key.substr(2));
		if ((ss >> num).fail() || num < 0 || num > (int)SKILL_MAX) {
			return;
		}
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_NONCLASSSKILL, num, operation, value));
	} else if (key.compare(0, 4, "CLSK") == 0) {
		int num = -1;
		stringstream ss(key.substr(4));
		if ((ss >> num).fail() || num < 0 || num >= CLASS_NA) {
			return;
		}
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_CLASSSKILLS, num, operation, value));
	} else if (key.compare(0, 5, "ALLSK") == 0) {
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_ALLSKILLS, 0, operation, value));
	} else if (key.compare(0, 5, "TABSK") == 0) {
		int num = -1;
		stringstream ss(key.substr(5));
		if ((ss >> num).fail() || num < 0 || num > SKILLTAB_MAX) {
			return;
		}
		Condition::AddOperand(conditions, new ItemStatCondition(STAT_SKILLTAB, num, operation, value));
	} else if (key.compare(0, 4, "STAT") == 0) {
		int num = -1;
		stringstream ss(key.substr(4));
		if ((ss >> num).fail() || num < 0 || num > (int)STAT_MAX) {
			return;
		}
		Condition::AddOperand(conditions, new ItemStatCondition(num, 0, operation, value));
	} else if (key.compare(0, 8, "CHARSTAT") == 0) {
		int num = -1;
		stringstream ss(key.substr(8));
		if ((ss >> num).fail() || num < 0 || num > (int)STAT_MAX) {
			return;
		}
		Condition::AddOperand(conditions, new CharStatCondition(num, 0, operation, value));
	} else if (key.compare(0, 5, "MULTI") == 0) {

		std::regex multi_reg("([0-9]{1,10}),([0-9]{1,10})",
			std::regex_constants::ECMAScript | std::regex_constants::icase);
		std::smatch multi_match;
		if (std::regex_search(key, multi_match, multi_reg)) {
			int stat1, stat2;
			stat1 = stoi(multi_match[1].str(), nullptr, 10);
			stat2 = stoi(multi_match[2].str(), nullptr, 10);

			Condition::AddOperand(conditions, new ItemStatCondition(stat1, stat2, operation, value));
		}

	} else if (key.compare(0, 5, "PRICE") == 0) {
		Condition::AddOperand(conditions, new ItemPriceCondition(operation, value));
	}

	for (vector<Condition*>::iterator it = endConditions.begin(); it != endConditions.end(); it++) {
		Condition::AddNonOperand(conditions, (*it));
	}
}

// Insert extra AND operators to stay backwards compatible with old config
// that implicitly ANDed all conditions
void Condition::AddOperand(vector<Condition*> &conditions, Condition *cond) {
	if (LastConditionType == CT_Operand || LastConditionType == CT_RightParen) {
		conditions.push_back(new AndOperator());
	}
	conditions.push_back(cond);
	LastConditionType = CT_Operand;
}

void Condition::AddNonOperand(vector<Condition*> &conditions, Condition *cond) {
	if ((cond->conditionType == CT_NegationOperator || cond->conditionType == CT_LeftParen) &&
		(LastConditionType == CT_Operand || LastConditionType == CT_RightParen)) {
		conditions.push_back(new AndOperator());
	}
	conditions.push_back(cond);
	LastConditionType = cond->conditionType;
}

bool Condition::Evaluate(UnitItemInfo *uInfo, ItemInfo *info, Condition *arg1, Condition *arg2) {
	// Arguments will vary based on where we're called from.
	// We will have either *info set (if called on reception of packet 0c9c, in which case
	// the normal item structures won't have been set up yet), or *uInfo otherwise.
	if (info) {
		return EvaluateInternalFromPacket(info, arg1, arg2);
	}
	return EvaluateInternal(uInfo, arg1, arg2);
}

bool TrueCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return true;
}
bool TrueCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return true;
}

bool FalseCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return false;
}
bool FalseCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return false;
}

bool NegationOperator::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return !arg1->Evaluate(uInfo, NULL, arg1, arg2);
}
bool NegationOperator::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return !arg1->Evaluate(NULL, info, arg1, arg2);
}

bool LeftParen::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return false;
}
bool LeftParen::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return false;
}

bool RightParen::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return false;
}
bool RightParen::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return false;
}

bool AndOperator::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return arg1->Evaluate(uInfo, NULL, NULL, NULL) && arg2->Evaluate(uInfo, NULL, NULL, NULL);
}
bool AndOperator::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return arg1->Evaluate(NULL, info, NULL, NULL) && arg2->Evaluate(NULL, info, NULL, NULL);
}

bool OrOperator::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return arg1->Evaluate(uInfo, NULL, NULL, NULL) || arg2->Evaluate(uInfo, NULL, NULL, NULL);
}
bool OrOperator::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return arg1->Evaluate(NULL, info, NULL, NULL) || arg2->Evaluate(NULL, info, NULL, NULL);
}

bool ItemCodeCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return (targetCode[0] == uInfo->itemCode[0] && targetCode[1] == uInfo->itemCode[1] && targetCode[2] == uInfo->itemCode[2]);
}
bool ItemCodeCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return (targetCode[0] == info->code[0] && targetCode[1] == info->code[1] && targetCode[2] == info->code[2]);
}

bool FlagsCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return ((uInfo->item->pItemData->dwFlags & flag) > 0);
}
bool FlagsCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	switch (flag) {
	case ITEM_ETHEREAL:
		return info->ethereal;
	case ITEM_IDENTIFIED:
		return info->identified;
	case ITEM_RUNEWORD:
		return info->runeword;
	}
	return false;
}

bool QualityCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return (uInfo->item->pItemData->dwQuality == quality);
}
bool QualityCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return (info->quality == quality);
}

bool NonMagicalCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return (uInfo->item->pItemData->dwQuality == ITEM_QUALITY_INFERIOR ||
			uInfo->item->pItemData->dwQuality == ITEM_QUALITY_NORMAL ||
			uInfo->item->pItemData->dwQuality == ITEM_QUALITY_SUPERIOR);
}
bool NonMagicalCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return (info->quality == ITEM_QUALITY_INFERIOR ||
			info->quality == ITEM_QUALITY_NORMAL ||
			info->quality == ITEM_QUALITY_SUPERIOR);
}

bool GemLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	if (IsGem(uInfo->attrs)) {
		return IntegerCompare(GetGemLevel(uInfo->attrs), operation, gemLevel);
	}
	return false;
}
bool GemLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	if (IsGem(info->attrs)) {
		return IntegerCompare(GetGemLevel(info->attrs), operation, gemLevel);
	}
	return false;
}

bool GemTypeCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	if (IsGem(uInfo->attrs)) {
		return IntegerCompare(GetGemType(uInfo->attrs), operation, gemType);
	}
	return false;
}
bool GemTypeCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	if (IsGem(info->attrs)) {
		return IntegerCompare(GetGemType(info->attrs), operation, gemType);
	}
	return false;
}

bool RuneCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	if (IsRune(uInfo->attrs)) {
		return IntegerCompare(RuneNumberFromItemCode(uInfo->itemCode), operation, runeNumber);
	}
	return false;
}
bool RuneCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	if (IsRune(info->attrs)) {
		return IntegerCompare(RuneNumberFromItemCode(info->code), operation, runeNumber);
	}
	return false;
}

bool GoldCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return false; // can only evaluate this from packet data
}
bool GoldCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	if (info->code[0] == 'g' && info->code[1] == 'l' && info->code[2] == 'd') {
		return IntegerCompare(info->amount, operation, goldAmount);
	}
	return false;
}

bool ItemLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return IntegerCompare(uInfo->item->pItemData->dwItemLevel, operation, itemLevel);
}
bool ItemLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return IntegerCompare(info->level, operation, itemLevel);
}

bool AffixLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	BYTE qlvl = uInfo->attrs->qualityLevel;
	BYTE alvl = GetAffixLevel((BYTE)uInfo->item->pItemData->dwItemLevel, (BYTE)uInfo->attrs->qualityLevel, uInfo->attrs->magicLevel);
	return IntegerCompare(alvl, operation, affixLevel);
}
bool AffixLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	int qlvl = info->attrs->qualityLevel;
	BYTE alvl = GetAffixLevel(info->level, info->attrs->qualityLevel, info->attrs->magicLevel);
	return IntegerCompare(alvl, operation, affixLevel);
}

bool RequiredLevelCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	unsigned int rlvl = GetRequiredLevel(uInfo->item);

	return IntegerCompare(rlvl, operation, requiredLevel);
}
bool RequiredLevelCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {

	//Not Done Yet (is it necessary? I think this might have something to do with the exact moment something drops?)

	return true;
}

bool ItemGroupCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return ((uInfo->attrs->flags & itemGroup) > 0);
}
bool ItemGroupCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return ((info->attrs->flags & itemGroup) > 0);
}

bool EDCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	// Either enhanced defense or enhanced damage depending on item type
	WORD stat;
	if (uInfo->attrs->flags & ITEM_GROUP_ALLARMOR) {
		stat = STAT_ENHANCEDDEFENSE;
	} else {
		// Normal %ED will have the same value for STAT_ENHANCEDMAXIMUMDAMAGE and STAT_ENHANCEDMINIMUMDAMAGE
		stat = STAT_ENHANCEDMAXIMUMDAMAGE;
	}

	// Pulled from JSUnit.cpp in d2bs
	DWORD value = 0;
	Stat aStatList[256] = { NULL };
	StatList* pStatList = D2COMMON_GetStatList(uInfo->item, NULL, 0x40);
	if (pStatList) {
		DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
		for (UINT i = 0; i < dwStats; i++) {
			if (aStatList[i].wStatIndex == stat && aStatList[i].wSubIndex == 0) {
				value += aStatList[i].dwStatValue;
			}
		}
	}
	return IntegerCompare(value, operation, targetED);
}
bool EDCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	// Either enhanced defense or enhanced damage depending on item type
	WORD stat;
	if (info->attrs->flags & ITEM_GROUP_ALLARMOR) {
		stat = STAT_ENHANCEDDEFENSE;
	} else {
		// Normal %ED will have the same value for STAT_ENHANCEDMAXIMUMDAMAGE and STAT_ENHANCEDMINIMUMDAMAGE
		stat = STAT_ENHANCEDMAXIMUMDAMAGE;
	}

	DWORD value = 0;
	for (vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
		if (prop->stat == stat) {
			value += prop->value;
		}
	}
	return IntegerCompare(value, operation, targetED);
}

bool DurabilityCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	// Pulled from JSUnit.cpp in d2bs
	DWORD value = 0;
	Stat aStatList[256] = { NULL };
	StatList* pStatList = D2COMMON_GetStatList(uInfo->item, NULL, 0x40);
	if (pStatList) {
		DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
		for (UINT i = 0; i < dwStats; i++) {
			if (aStatList[i].wStatIndex == STAT_ENHANCEDMAXDURABILITY && aStatList[i].wSubIndex == 0) {
				value += aStatList[i].dwStatValue;
			}
		}
	}
	return IntegerCompare(value, operation, targetDurability);
}
bool DurabilityCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	DWORD value = 0;
	for (vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
		if (prop->stat == STAT_ENHANCEDMAXDURABILITY) {
			value += prop->value;
		}
	}
	return IntegerCompare(value, operation, targetDurability);
}

bool FoolsCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	// 1 = MAX DMG / level
	// 2 = AR / level
	// 3 = Fools

	// Pulled from JSUnit.cpp in d2bs
	unsigned int value = 0;
	Stat aStatList[256] = { NULL };
	StatList* pStatList = D2COMMON_GetStatList(uInfo->item, NULL, 0x40);
	if (pStatList) {
		DWORD dwStats = D2COMMON_CopyStatList(pStatList, (Stat*)aStatList, 256);
		for (UINT i = 0; i < dwStats; i++) {
			if (aStatList[i].wStatIndex == STAT_MAXDAMAGEPERLEVEL && aStatList[i].wSubIndex == 0) {
				value += 1;
			}
			if (aStatList[i].wStatIndex == STAT_ATTACKRATINGPERLEVEL && aStatList[i].wSubIndex == 0) {
				value += 2;
			}
		}
	}
	// We are returning a comparison on 3 here instead of any the actual number because the way it is setup is
	// to just write FOOLS in the mh file instead of FOOLS=3 this could be changed to accept 1-3 for the different
	// types it can produce
	return IntegerCompare(value, (BYTE)EQUAL, 3);
}
bool FoolsCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	// 1 = MAX DMG / level
	// 2 = AR / level
	// 3 = Fools

	unsigned int value = 0;
	for (vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
		if (prop->stat == STAT_MAXDAMAGEPERLEVEL) {
			value += 1;
		}
		if (prop->stat == STAT_ATTACKRATINGPERLEVEL) {
			value += 2;
		}
	}
	// We are returning a comparison on 3 here instead of any the actual number because the way it is setup is
	// to just write FOOLS in the mh file instead of FOOLS=3 this could be changed to accept 1-3 for the different
	// types it can produce
	return IntegerCompare(value, (BYTE)EQUAL, 3);
}

void SkillListCondition::Init() {
	// Clear lists
	classSkillList.clear();
	skillList.clear();
	goodClassSkills.clear();
	goodTabSkills.clear();

	// Build character skills list
	BH::config->ReadAssoc("ClassSkillsList", skillList);
	for (auto it = skillList.cbegin(); it != skillList.cend(); it++) {
		if (StringToBool((*it).second)) {
			goodClassSkills.push_back(stoi((*it).first));
		}
	}

	// Build tab skills list
	BH::config->ReadAssoc("TabSkillsList", classSkillList);
	for (auto it = classSkillList.cbegin(); it != classSkillList.cend(); it++) {
		if (StringToBool((*it).second)) {
			goodTabSkills.push_back(stoi((*it).first));
		}
	}
}

bool SkillListCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	int value = 0;
	if (type == CLASS_SKILLS) {
		for (unsigned int i = 0; i < goodClassSkills.size(); i++) {
			value += D2COMMON_GetUnitStat(uInfo->item, STAT_CLASSSKILLS, goodClassSkills.at(i));
		}
	}
	else if (type == CLASS_TAB_SKILLS) {
		for (unsigned int i = 0; i < goodTabSkills.size(); i++) {
			value += D2COMMON_GetUnitStat(uInfo->item, STAT_SKILLTAB, goodTabSkills.at(i));
		}
	}

	return IntegerCompare(value, operation, targetStat);
}

bool SkillListCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	// TODO: Implement later
	return false;
}

bool CharStatCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return IntegerCompare(D2COMMON_GetUnitStat(D2CLIENT_GetPlayerUnit(), stat1, stat2), operation, targetStat);
}
bool CharStatCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return IntegerCompare(D2COMMON_GetUnitStat(D2CLIENT_GetPlayerUnit(), stat1, stat2), operation, targetStat);
}

bool DifficultyCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return IntegerCompare(D2CLIENT_GetDifficulty(), operation, targetDiff);
}
bool DifficultyCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	return IntegerCompare(D2CLIENT_GetDifficulty(), operation, targetDiff);
}

bool ItemStatCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return IntegerCompare(D2COMMON_GetUnitStat(uInfo->item, itemStat, itemStat2), operation, targetStat);
}
bool ItemStatCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	int num = 0;
	switch (itemStat) {
	case STAT_SOCKETS:
		return IntegerCompare(info->sockets, operation, targetStat);
	case STAT_DEFENSE:
		return IntegerCompare(GetDefense(info), operation, targetStat);
	case STAT_NONCLASSSKILL:
		for (vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
			if (prop->stat == STAT_NONCLASSSKILL && prop->skill == itemStat2) {
				num += prop->value;
			}
		}
		return IntegerCompare(num, operation, targetStat);
	case STAT_SINGLESKILL:
		for (vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
			if (prop->stat == STAT_SINGLESKILL && prop->skill == itemStat2) {
				num += prop->value;
			}
		}
		return IntegerCompare(num, operation, targetStat);
	case STAT_CLASSSKILLS:
		for (vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
			if (prop->stat == STAT_CLASSSKILLS && prop->characterClass == itemStat2) {
				num += prop->value;
			}
		}
		return IntegerCompare(num, operation, targetStat);
	case STAT_SKILLTAB:
		for (vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
			if (prop->stat == STAT_SKILLTAB && (prop->characterClass * 8 + prop->tab) == itemStat2) {
				num += prop->value;
			}
		}
		return IntegerCompare(num, operation, targetStat);
	default:
		for (vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
			if (prop->stat == itemStat) {
				num += prop->value;
			}
		}
		return IntegerCompare(num, operation, targetStat);
	}
	return false;
}

bool ItemPriceCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	return IntegerCompare(D2COMMON_GetItemPrice(D2CLIENT_GetPlayerUnit(), uInfo->item, D2CLIENT_GetDifficulty(), (DWORD)D2CLIENT_GetQuestInfo(), 0x201, 1), operation, targetStat);
}
bool ItemPriceCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	// TODO: Implement later
	return false;
}

bool ResistAllCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	int fRes = D2COMMON_GetUnitStat(uInfo->item, STAT_FIRERESIST, 0);
	int lRes = D2COMMON_GetUnitStat(uInfo->item, STAT_LIGHTNINGRESIST, 0);
	int cRes = D2COMMON_GetUnitStat(uInfo->item, STAT_COLDRESIST, 0);
	int pRes = D2COMMON_GetUnitStat(uInfo->item, STAT_POISONRESIST, 0);
	return (IntegerCompare(fRes, operation, targetStat) &&
			IntegerCompare(lRes, operation, targetStat) &&
			IntegerCompare(cRes, operation, targetStat) &&
			IntegerCompare(pRes, operation, targetStat));
}
bool ResistAllCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	int fRes = 0, lRes = 0, cRes = 0, pRes = 0;
	for (vector<ItemProperty>::iterator prop = info->properties.begin(); prop < info->properties.end(); prop++) {
		if (prop->stat == STAT_FIRERESIST) {
			fRes += prop->value;
		} else if (prop->stat == STAT_LIGHTNINGRESIST) {
			lRes += prop->value;
		} else if (prop->stat == STAT_COLDRESIST) {
			cRes += prop->value;
		} else if (prop->stat == STAT_POISONRESIST) {
			pRes += prop->value;
		}
	}
	return (IntegerCompare(fRes, operation, targetStat) &&
			IntegerCompare(lRes, operation, targetStat) &&
			IntegerCompare(cRes, operation, targetStat) &&
			IntegerCompare(pRes, operation, targetStat));
}

void AddCondition::Init() {
	codes.clear();
	codes = split(key, '+');
	for (auto code : codes) {
		for (int j = 0; j < sizeof(skills) / sizeof(skills[0]); j++) {
			if (code == skills[j].key)
				stats.push_back(skills[j].value);
		}
	}
}

bool AddCondition::EvaluateInternal(UnitItemInfo *uInfo, Condition *arg1, Condition *arg2) {
	int value = 0;
	for (unsigned int i = 0; i < stats.size(); i++) {
		int tmpVal = D2COMMON_GetUnitStat(uInfo->item, stats[i], 0);
		if (stats[i] == STAT_MAXHP || stats[i] == STAT_MAXMANA)
			tmpVal /= 256;
		value += tmpVal;
	}

	return IntegerCompare(value, operation, targetStat);
}

bool AddCondition::EvaluateInternalFromPacket(ItemInfo *info, Condition *arg1, Condition *arg2) {
	// TODO: Implement later
	return false;
}

int GetDefense(ItemInfo *item) {
	int def = item->defense;
	for (vector<ItemProperty>::iterator prop = item->properties.begin(); prop < item->properties.end(); prop++) {
		if (prop->stat == STAT_ENHANCEDDEFENSE) {
			def *= (prop->value + 100);
			def /= 100;
		}
	}
	return def;
}

void HandleUnknownItemCode(char *code, char *tag) {
	// If the MPQ files arent loaded yet then this is expected
	if (!IsInitialized()){
		return;
	}

	// Avoid spamming endlessly
	if (UnknownItemCodes.size() > 10 || (*BH::MiscToggles2)["Allow Unknown Items"].state) {
		return;
	}
	if (UnknownItemCodes.find(code) == UnknownItemCodes.end()) {
		PrintText(1, "Unknown item code %s: %c%c%c\n", tag, code[0], code[1], code[2]);
		UnknownItemCodes[code] = 1;
	}
}

StatProperties *GetStatProperties(unsigned int stat) {
	return AllStatList.at(stat);
}
