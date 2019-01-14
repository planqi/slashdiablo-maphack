/**
 *
 * Item.cpp
 * BH: Copyright 2011 (C) McGod
 * SlashDiablo Maphack: Copyright (C) SlashDiablo Community
 *
 *  This file is part of SlashDiablo Maphack.
 *
 *  SlashDiablo Maphack is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *   ==========================================================
 *   D2Ex2
 *   https://github.com/lolet/D2Ex2
 *   ==========================================================
 *   Copyright (c) 2011-2014 Bartosz Jankowski
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *   ==========================================================
 *
 */

#include "Item.h"
#include "../../D2Ptrs.h"
#include "../../D2Strings.h"
#include "../../BH.h"
#include "../../D2Stubs.h"
#include "ItemDisplay.h"
#include "../../MPQInit.h"

ItemsTxtStat* GetAllStatModifier(ItemsTxtStat* pStats, int nStats, int nStat, ItemsTxtStat* pOrigin);
ItemsTxtStat* GetItemsTxtStatByMod(ItemsTxtStat* pStats, int nStats, int nStat, int nStatParam);
RunesTxt* GetRunewordTxtById(int rwId);

map<std::string, Toggle> Item::Toggles;
UnitAny* Item::viewingUnit;

Patch* itemNamePatch = new Patch(Call, D2CLIENT, { 0x92366, 0x96736 }, (int)ItemName_Interception, 6);
Patch* itemPropertiesPatch = new Patch(Jump, D2CLIENT, { 0x5612C, 0x2E3FC }, (int)GetProperties_Interception, 6);
Patch* itemPropertyStringDamagePatch = new Patch(Call, D2CLIENT, { 0x55D7B, 0x2E04B }, (int)GetItemPropertyStringDamage_Interception, 5);
Patch* itemPropertyStringPatch = new Patch(Call, D2CLIENT, { 0x55D9D, 0x2E06D }, (int) GetItemPropertyString_Interception, 5);
Patch* viewInvPatch1 = new Patch(Call, D2CLIENT, { 0x953E2, 0x997B2 }, (int)ViewInventoryPatch1_ASM, 6);
Patch* viewInvPatch2 = new Patch(Call, D2CLIENT, { 0x94AB4, 0x98E84 }, (int)ViewInventoryPatch2_ASM, 6);
Patch* viewInvPatch3 = new Patch(Call, D2CLIENT, { 0x93A6F, 0x97E3F }, (int)ViewInventoryPatch3_ASM, 5);

using namespace Drawing;

void Item::OnLoad() {
	LoadConfig();

	viewInvPatch1->Install();
	viewInvPatch2->Install();
	viewInvPatch3->Install();

	itemPropertyStringDamagePatch->Install();
	itemPropertyStringPatch->Install();

	if (Toggles["Show Ethereal"].state || Toggles["Show Sockets"].state || Toggles["Show iLvl"].state || Toggles["Color Mod"].state ||
		Toggles["Show Rune Numbers"].state || Toggles["Alt Item Style"].state || Toggles["Shorten Item Names"].state || Toggles["Advanced Item Display"].state)
		itemNamePatch->Install();

	if (Toggles["Always Show Item Stat Ranges"].state) {
		itemPropertiesPatch->Install();
		itemPropertyStringDamagePatch->Install();
		itemPropertyStringPatch->Install();
	}

	DrawSettings();
}

void Item::LoadConfig() {
	BH::config->ReadToggle("Show Ethereal", "None", true, Toggles["Show Ethereal"]);
	BH::config->ReadToggle("Show Sockets", "None", true, Toggles["Show Sockets"]);
	BH::config->ReadToggle("Show ILvl", "None", true, Toggles["Show iLvl"]);
	BH::config->ReadToggle("Show Rune Numbers", "None", true, Toggles["Show Rune Numbers"]);
	BH::config->ReadToggle("Alt Item Style", "None", true, Toggles["Alt Item Style"]);
	BH::config->ReadToggle("Color Mod", "None", false, Toggles["Color Mod"]);
	BH::config->ReadToggle("Shorten Item Names", "None", false, Toggles["Shorten Item Names"]);
	BH::config->ReadToggle("Advanced Item Display", "None", false, Toggles["Advanced Item Display"]);
	BH::config->ReadToggle("Item Drop Notifications", "None", false, Toggles["Item Drop Notifications"]);
	BH::config->ReadToggle("Item Close Notifications", "None", false, Toggles["Item Close Notifications"]);
	BH::config->ReadToggle("Verbose Notifications", "None", false, Toggles["Verbose Notifications"]);
	BH::config->ReadToggle("Allow Unknown Items", "None", false, Toggles["Allow Unknown Items"]);
	BH::config->ReadToggle("Suppress Invalid Stats", "None", false, Toggles["Suppress Invalid Stats"]);
	BH::config->ReadToggle("Always Show Item Stat Ranges", "None", true, Toggles["Always Show Item Stat Ranges"]);

	ItemDisplay::UninitializeItemRules();

	//InitializeMPQData();

	BH::config->ReadKey("Show Players Gear", "VK_0", showPlayer);
}

void Item::DrawSettings() {
	settingsTab = new UITab("Item", BH::settingsUI);
	int y = 10;
	int keyhook_x = 230;

	new Checkhook(settingsTab, 4, y, &Toggles["Show Ethereal"].state, "Show Ethereal");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Show Ethereal"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Show Sockets"].state, "Show Sockets");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Show Sockets"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Show iLvl"].state, "Show iLvl");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Show iLvl"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Show Rune Numbers"].state, "Show Rune #");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Show Rune Numbers"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Alt Item Style"].state, "Alt Style");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Alt Item Style"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Color Mod"].state, "Color Mod");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Color Mod"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Shorten Item Names"].state, "Shorten Names");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Shorten Item Names"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Always Show Item Stat Ranges"].state, "Always Show Item Stat Ranges");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Always Show Item Stat Ranges"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Advanced Item Display"].state, "Advanced Item Display");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Advanced Item Display"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Item Drop Notifications"].state, "Item Drop Notifications");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Item Drop Notifications"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Item Close Notifications"].state, "Item Close Notifications");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Item Close Notifications"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Verbose Notifications"].state, "Verbose Notifications");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Verbose Notifications"].toggle, "");
	y += 15;

	new Checkhook(settingsTab, 4, y, &Toggles["Suppress Invalid Stats"].state, "Suppress Invalid Stats");
	new Keyhook(settingsTab, keyhook_x, y+2, &Toggles["Suppress Invalid Stats"].toggle, "");
	y += 15;

	new Keyhook(settingsTab, 4, y+2, &showPlayer, "Show Players Gear");
}

void Item::OnUnload() {
	itemNamePatch->Remove();
	itemPropertyStringDamagePatch->Remove();
	itemPropertyStringPatch->Remove();
	viewInvPatch1->Remove();
	viewInvPatch2->Remove();
	viewInvPatch3->Remove();
	ItemDisplay::UninitializeItemRules();
}

void Item::OnLoop() {
	if (!D2CLIENT_GetUIState(0x01))
		viewingUnit = NULL;
	
	if (Toggles["Advanced Item Display"].state) {
		ItemDisplay::InitializeItemRules();
	}

	if (viewingUnit && viewingUnit->dwUnitId) {
		if (!viewingUnit->pInventory){
			viewingUnit = NULL;
			D2CLIENT_SetUIVar(0x01, 1, 0);			
		} else if (!D2CLIENT_FindServerSideUnit(viewingUnit->dwUnitId, viewingUnit->dwType)) {
			viewingUnit = NULL;
			D2CLIENT_SetUIVar(0x01, 1, 0);
		}
	}
}

void Item::OnKey(bool up, BYTE key, LPARAM lParam, bool* block) {
	if (key == showPlayer) {
		*block = true;
		if (up)
			return;
		UnitAny* selectedUnit = D2CLIENT_GetSelectedUnit();
		if (selectedUnit && selectedUnit->dwMode != 0 && selectedUnit->dwMode != 17 && ( // Alive
					selectedUnit->dwType == 0 ||					// Player
					selectedUnit->dwTxtFileNo == 291 ||		// Iron Golem
					selectedUnit->dwTxtFileNo == 357 ||		// Valkerie
					selectedUnit->dwTxtFileNo == 418)) {	// Shadow Master
			viewingUnit = selectedUnit;
			if (!D2CLIENT_GetUIState(0x01))
				D2CLIENT_SetUIVar(0x01, 0, 0);
			return;
		}
	}
	for (map<string,Toggle>::iterator it = Toggles.begin(); it != Toggles.end(); it++) {
		if (key == (*it).second.toggle) {
			*block = true;
			if (up) {
				(*it).second.state = !(*it).second.state;
			}
			return;
		}
	}
}

void Item::OnLeftClick(bool up, int x, int y, bool* block) {
	if (up)
		return;
	if (D2CLIENT_GetUIState(0x01) && viewingUnit != NULL && x >= 400)
		*block = true;
}

void __fastcall Item::ItemNamePatch(wchar_t *name, UnitAny *item)
{
	char* szName = UnicodeToAnsi(name);
	string itemName = szName;
	char* code = D2COMMON_GetItemText(item->dwTxtFileNo)->szCode;

	if (Toggles["Advanced Item Display"].state) {
		UnitItemInfo uInfo;
		uInfo.itemCode[0] = code[0]; uInfo.itemCode[1] = code[1]; uInfo.itemCode[2] = code[2]; uInfo.itemCode[3] = 0;
		uInfo.item = item;
		if (ItemAttributeMap.find(uInfo.itemCode) != ItemAttributeMap.end()) {
			uInfo.attrs = ItemAttributeMap[uInfo.itemCode];
			GetItemName(&uInfo, itemName);
		} else {
			HandleUnknownItemCode(uInfo.itemCode, "name");
		}
	} else {
		OrigGetItemName(item, itemName, code);
	}

	// Some common color codes for text strings (see TextColor enum):
	// ÿc; (purple)
	// ÿc0 (white)
	// ÿc1 (red)
	// ÿc2 (green)
	// ÿc3 (blue)
	// ÿc4 (gold)
	// ÿc5 (gray)
	// ÿc6 (black)
	// ÿc7 (tan)
	// ÿc8 (orange)
	// ÿc9 (yellow)

	/* Test code to display item codes */
	//string test3 = test_code;
	//itemName += " {" + test3 + "}";

	MultiByteToWideChar(CODE_PAGE, MB_PRECOMPOSED, itemName.c_str(), itemName.length(), name, itemName.length());
	name[itemName.length()] = 0;  // null-terminate the string since MultiByteToWideChar doesn't
	delete[] szName;
}

void Item::OrigGetItemName(UnitAny *item, string &itemName, char *code)
{
	bool displayItemLevel = Toggles["Show iLvl"].state;
	if (Toggles["Shorten Item Names"].state)
	{
		// We will also strip ilvls from these items
		if (code[0] == 't' && code[1] == 's' && code[2] == 'c')  // town portal scroll
		{
			itemName = "ÿc2**ÿc0TP";
			displayItemLevel = false;
		}
		else if (code[0] == 'i' && code[1] == 's' && code[2] == 'c')  // identify scroll
		{
			itemName = "ÿc2**ÿc0ID";
			displayItemLevel = false;
		}
		else if (code[0] == 'v' && code[1] == 'p' && code[2] == 's')  // stamina potion
		{
			itemName = "Stam";
			displayItemLevel = false;
		}
		else if (code[0] == 'y' && code[1] == 'p' && code[2] == 's')  // antidote potion
		{
			itemName = "Anti";
			displayItemLevel = false;
		}
		else if (code[0] == 'w' && code[1] == 'm' && code[2] == 's')  // thawing potion
		{
			itemName = "Thaw";
			displayItemLevel = false;
		}
		else if (code[0] == 'g' && code[1] == 'p' && code[2] == 's')  // rancid gas potion
		{
			itemName = "Ranc";
			displayItemLevel = false;
		}
		else if (code[0] == 'o' && code[1] == 'p' && code[2] == 's')  // oil potion
		{
			itemName = "Oil";
			displayItemLevel = false;
		}
		else if (code[0] == 'g' && code[1] == 'p' && code[2] == 'm')  // choking gas potion
		{
			itemName = "Chok";
			displayItemLevel = false;
		}
		else if (code[0] == 'o' && code[1] == 'p' && code[2] == 'm')  // exploding potion
		{
			itemName = "Expl";
			displayItemLevel = false;
		}
		else if (code[0] == 'g' && code[1] == 'p' && code[2] == 'l')  // strangling gas potion
		{
			itemName = "Stra";
			displayItemLevel = false;
		}
		else if (code[0] == 'o' && code[1] == 'p' && code[2] == 'l')  // fulminating potion
		{
			itemName = "Fulm";
			displayItemLevel = false;
		}
		else if (code[0] == 'h' && code[1] == 'p')  // healing potions
		{
			if (code[2] == '1')
			{
				itemName = "ÿc1**ÿc0Min Heal";
				displayItemLevel = false;
			}
			else if (code[2] == '2')
			{
				itemName = "ÿc1**ÿc0Lt Heal";
				displayItemLevel = false;
			}
			else if (code[2] == '3')
			{
				itemName = "ÿc1**ÿc0Heal";
				displayItemLevel = false;
			}
			else if (code[2] == '4')
			{
				itemName = "ÿc1**ÿc0Gt Heal";
				displayItemLevel = false;
			}
			else if (code[2] == '5')
			{
				itemName = "ÿc1**ÿc0Sup Heal";
				displayItemLevel = false;
			}
		}
		else if (code[0] == 'm' && code[1] == 'p')  // mana potions
		{
			if (code[2] == '1')
			{
				itemName = "ÿc3**ÿc0Min Mana";
				displayItemLevel = false;
			}
			else if (code[2] == '2')
			{
				itemName = "ÿc3**ÿc0Lt Mana";
				displayItemLevel = false;
			}
			else if (code[2] == '3')
			{
				itemName = "ÿc3**ÿc0Mana";
				displayItemLevel = false;
			}
			else if (code[2] == '4')
			{
				itemName = "ÿc3**ÿc0Gt Mana";
				displayItemLevel = false;
			}
			else if (code[2] == '5')
			{
				itemName = "ÿc3**ÿc0Sup Mana";
				displayItemLevel = false;
			}
		}
		else if (code[0] == 'r' && code[1] == 'v')  // rejuv potions
		{
			if (code[2] == 's')
			{
				itemName = "ÿc;**ÿc0Rejuv";
				displayItemLevel = false;
			}
			else if (code[2] == 'l')
			{
				itemName = "ÿc;**ÿc0Full";
				displayItemLevel = false;
			}
		}
		else if (code[1] == 'q' && code[2] == 'v')
		{
			if (code[0] == 'a')  // arrows
			{
				displayItemLevel = false;
			}
			else if (code[0] == 'c')  // bolts
			{
				displayItemLevel = false;
			}
		}
		else if (code[0] == 'k' && code[1] == 'e' && code[2] == 'y')  // key
		{
			displayItemLevel = false;
		}
	}

	/*Suffix Color Mod*/
	if( Toggles["Color Mod"].state )
	{
		/*Essences*/
		if( code[0] == 't' && code[1] == 'e' && code[2] == 's' )
		{
			itemName = itemName + " (Andariel/Duriel)";
		}
		if( code[0] == 'c' && code[1] == 'e' && code[2] == 'h' )
		{
			itemName = itemName + " (Mephtisto)";
		}
		if( code[0] == 'b' && code[1] == 'e' && code[2] == 't' )
		{
			itemName = itemName + " (Diablo)";
		}
		if( code[0] == 'f' && code[1] == 'e' && code[2] == 'd' )
		{
			itemName = itemName + " (Baal)";
		}
	}

	if( Toggles["Alt Item Style"].state )
	{
		if (Toggles["Show Rune Numbers"].state && D2COMMON_GetItemText(item->dwTxtFileNo)->nType == 74)
		{
			itemName = to_string(item->dwTxtFileNo - 609) + " - " + itemName;
		}
		else
		{
			if (Toggles["Show Sockets"].state)
			{
				int sockets = D2COMMON_GetUnitStat(item, STAT_SOCKETS, 0);
				if (sockets > 0)
				{
					itemName += "(" + to_string(sockets) + ")";
				}
			}

			if (Toggles["Show Ethereal"].state && item->pItemData->dwFlags & ITEM_ETHEREAL)
			{
				itemName = "Eth " + itemName;
			}
	
			/*show iLvl unless it is equal to 1*/
			if (displayItemLevel && item->pItemData->dwItemLevel != 1)
			{
				itemName += " L" + to_string(item->pItemData->dwItemLevel);
			}
		}
	}
	else
	{
		if (Toggles["Show Sockets"].state) {
			int sockets = D2COMMON_GetUnitStat(item, STAT_SOCKETS, 0);
			if (sockets > 0)
				itemName += "(" + to_string(sockets) + ")";
		}
		if (Toggles["Show Ethereal"].state && item->pItemData->dwFlags & ITEM_ETHEREAL)
			itemName += "(Eth)";

		if (displayItemLevel)
			itemName += "(L" + to_string(item->pItemData->dwItemLevel) + ")";

		if (Toggles["Show Rune Numbers"].state && D2COMMON_GetItemText(item->dwTxtFileNo)->nType == 74)
			itemName = "[" + to_string(item->dwTxtFileNo - 609) + "]" + itemName;
	}

	/*Affix (Colors) Color Mod*/
	if( Toggles["Color Mod"].state )
	{
		///*Flawless Gems*/
		//if( (code[0] == 'g' && code[1] == 'l'					) ||
		//	(code[0] == 's' && code[1] == 'k' && code[2] == 'l' ) )
		//{
		//	itemName = "ÿc:" + itemName;
		//}
		///*Perfect Gems*/
		//if( (code[0] == 'g' && code[1] == 'p'                   ) ||
		//	(code[0] == 's' && code[1] == 'k' && code[2] == 'p' ) )
		//{
		//	itemName = "ÿc<" + itemName;
		//}
		/*Ethereal*/
		if( item->pItemData->dwFlags & 0x400000 )
		{
			/*Turn ethereal elite armors (and paladin shields) purple*/
			if( (code[0] == 'u'                                    ) ||
				(code[0] == 'p' && code[1] == 'a' && code[2] >= 'b') )
			{
				itemName = "ÿc;" + itemName;
			}
		}
		/*Runes*/
		if( code[0] == 'r' )
		{
			if( code[1] == '0' )
			{
				itemName = "ÿc0" + itemName;
			}
			else if( code[1] == '1' )
			{
				if( code[2] <= '6')
				{
					itemName = "ÿc4" + itemName;
				}
				else
				{
					itemName = "ÿc8" + itemName;
				}
			}
			else if( code[1] == '2' )
			{
				if( code[2] <= '2' )
				{
					itemName = "ÿc8" + itemName;
				}
				else
				{
					itemName = "ÿc1" + itemName;
				}
			}
			else if( code[1] == '3' )
			{
				itemName = "ÿc1" + itemName;
			}
		}
	}
}

static ItemsTxt* GetArmorText(UnitAny* pItem) {
	ItemText* itemTxt = D2COMMON_GetItemText(pItem->dwTxtFileNo);
	int armorTxtRecords = *p_D2COMMON_ArmorTxtRecords;
	for (int i = 0; i < armorTxtRecords; i++) {
		ItemsTxt* armorTxt = &(*p_D2COMMON_ArmorTxt)[i];
		if (strcmp(armorTxt->szcode, itemTxt->szCode) == 0) {
			return armorTxt;
		}
	}
	return NULL;
}

void __stdcall Item::OnProperties(wchar_t * wTxt)
{
	UnitAny* pItem = *p_D2CLIENT_SelectedInvItem;
	if (!pItem) return;
	//Any Armor ItemTypes.txt
	if (D2COMMON_IsMatchingType(pItem, ITEM_TYPE_ALLARMOR)) {
		int aLen = 0;
		aLen = wcslen(wTxt);
		ItemsTxt* armorTxt = GetArmorText(pItem);
		DWORD base = D2COMMON_GetBaseStatSigned(pItem, STAT_DEFENSE, 0);
		BOOL isED = TRUE;
		//TODO: items with enhanced def mod can roll max def +1. below does not get the stat
		//isED = D2COMMON_GetBaseStatSigned(pItem, STAT_ENHANCEDDEFENSE, 0) > 0;
		DWORD min = armorTxt->dwminac;
		DWORD max = armorTxt->dwmaxac + (isED ? 1 : 0);
		if (pItem->pItemData->dwFlags & ITEM_ETHEREAL) {
			min = (DWORD)(min * 1.50);
			max = (DWORD)(max * 1.50);
		}
		//hack... if not in range we assume it is ebugged
		if (base > max) {
			swprintf_s(wTxt + aLen, 1024 - aLen, L"%sBase Defense: %d [%d-%d]%s\n", L"ÿc9", base, (DWORD)(armorTxt->dwminac * 2.25), (DWORD)(armorTxt->dwmaxac * 2.25), L"ÿc3");
		}
		else {
			swprintf_s(wTxt + aLen, 1024 - aLen, L"%sBase Defense: %d [%d-%d]%s\n", L"ÿc9", base, min, max, L"ÿc3");
		}
	}
}

BOOL __stdcall Item::OnDamagePropertyBuild(UnitAny* pItem, DamageStats* pDmgStats, int nStat, wchar_t* wOut) {
	wchar_t newDesc[128];

	// Ignore a max stat, use just a min dmg prop to gen the property string
	if (nStat == STAT_MAXIMUMFIREDAMAGE || nStat == STAT_MAXIMUMCOLDDAMAGE || nStat == STAT_MAXIMUMLIGHTNINGDAMAGE|| nStat == STAT_MAXIMUMMAGICALDAMAGE ||
		nStat == STAT_MAXIMUMPOISONDAMAGE || nStat == STAT_POISONDAMAGELENGTH || nStat == STAT_ENHANCEDMAXIMUMDAMAGE)
		return TRUE;

	int stat_min, stat_max;
	wchar_t* szProp = nullptr;
	bool ranged = true;
	if (nStat == STAT_MINIMUMFIREDAMAGE) {
		if (pDmgStats->nFireDmgRange == 0)
			return FALSE;
		stat_min = pDmgStats->nMinFireDmg;
		stat_max = pDmgStats->nMaxFireDmg;
		if (stat_min >= stat_max) {
			szProp = D2LANG_GetLocaleText(D2STR_STRMODFIREDAMAGE);
			ranged = false;
		}
		else {
			szProp = D2LANG_GetLocaleText(D2STR_STRMODFIREDAMAGERANGE);
		}
	}
	else if (nStat == STAT_MINIMUMCOLDDAMAGE) {
		if (pDmgStats->nColdDmgRange == 0)
			return FALSE;
		stat_min = pDmgStats->nMinColdDmg;
		stat_max = pDmgStats->nMaxColdDmg;
		if (stat_min >= stat_max) {
			szProp = D2LANG_GetLocaleText(D2STR_STRMODCOLDDAMAGE);
			ranged = false;
		}
		else {
			szProp = D2LANG_GetLocaleText(D2STR_STRMODCOLDDAMAGERANGE);
		}
	}
	else if (nStat == STAT_MINIMUMLIGHTNINGDAMAGE) {
		if (pDmgStats->nLightDmgRange == 0)
			return FALSE;
		stat_min = pDmgStats->nMinLightDmg;
		stat_max = pDmgStats->nMaxLightDmg;
		if (stat_min >= stat_max) {
			szProp = D2LANG_GetLocaleText(D2STR_STRMODLIGHTNINGDAMAGE);
			ranged = false;
		}
		else {
			szProp = D2LANG_GetLocaleText(D2STR_STRMODLIGHTNINGDAMAGERANGE);
		}
	}
	else if (nStat == STAT_MINIMUMMAGICALDAMAGE) {
		if (pDmgStats->nMagicDmgRange == 0)
			return FALSE;
		stat_min = pDmgStats->nMinMagicDmg;
		stat_max = pDmgStats->nMaxMagicDmg;
		if (stat_min >= stat_max) {
			szProp = D2LANG_GetLocaleText(D2STR_STRMODMAGICDAMAGE);
			ranged = false;
		}
		else {
			szProp = D2LANG_GetLocaleText(D2STR_STRMODMAGICDAMAGERANGE);
		}
	}
	else if (nStat == STAT_MINIMUMPOISONDAMAGE) {
		if (pDmgStats->nPsnDmgRange == 0)
			return FALSE;
		if (pDmgStats->nPsnCount <= 0)
			pDmgStats->nPsnCount = 1;

		pDmgStats->nPsnLen = pDmgStats->nPsnLen / pDmgStats->nPsnCount;

		pDmgStats->nMinPsnDmg = stat_min = ((pDmgStats->nMinPsnDmg * pDmgStats->nPsnLen) + 128) / 256;
		pDmgStats->nMaxPsnDmg = stat_max = ((pDmgStats->nMaxPsnDmg * pDmgStats->nPsnLen) + 128) / 256;

		if (stat_min >= stat_max) {
			szProp = D2LANG_GetLocaleText(D2STR_STRMODPOISONDAMAGE);
			swprintf_s(newDesc, 128, szProp, stat_max, pDmgStats->nPsnLen / 25); // Per frame
		}
		else {
			szProp = D2LANG_GetLocaleText(D2STR_STRMODPOISONDAMAGERANGE);
			swprintf_s(newDesc, 128, szProp, stat_min, stat_max, pDmgStats->nPsnLen / 25);
		}
		wcscat_s(wOut, 1024, newDesc);
		return TRUE;
	}
	else if (nStat == STAT_SECONDARYMAXIMUMDAMAGE) {
		if (pDmgStats->dword14)
			return TRUE;
		return pDmgStats->nDmgRange != 0;
	}
	else if (nStat == STAT_MINIMUMDAMAGE || nStat == STAT_MAXIMUMDAMAGE || nStat == STAT_SECONDARYMINIMUMDAMAGE) {
		if (pDmgStats->dword14)
			return TRUE;
		if (!pDmgStats->nDmgRange)
			return FALSE;

		stat_min = pDmgStats->nMinDmg;
		stat_max = pDmgStats->nMaxDmg;

		if (stat_min >= stat_max) {
			return FALSE;
		}
		else {
			pDmgStats->dword14 = TRUE;
			szProp = D2LANG_GetLocaleText(D2STR_STRMODMINDAMAGERANGE);

		}
	}
	else if (nStat == STAT_ENHANCEDMINIMUMDAMAGE) {
		if (!pDmgStats->nDmgPercentRange)
			return FALSE;
		stat_min = pDmgStats->nMinDmgPercent;
		stat_max = (int) (D2LANG_GetLocaleText(10023)); // "Enhanced damage"
		szProp = L"+%d%% %s\n";
	}

	if (szProp == nullptr) {
		return FALSE;
	}

	if (ranged) {
		swprintf_s(newDesc, 128, szProp, stat_min, stat_max);
	}
	else {
		swprintf_s(newDesc, 128, szProp, stat_max);
	}

	// <!--
	if (newDesc[wcslen(newDesc) - 1] == L'\n')
		newDesc[wcslen(newDesc) - 1] = L'\0';
	if (newDesc[wcslen(newDesc) - 1] == L'\n')
		newDesc[wcslen(newDesc) - 1] = L'\0';

	OnPropertyBuild(newDesc, nStat, pItem, 0);
	// Beside this add-on the function is almost 1:1 copy of Blizzard's one -->
	wcscat_s(wOut, 1024, newDesc);
	wcscat_s(wOut, 1024, L"\n");

	return TRUE;
}

void __stdcall Item::OnPropertyBuild(wchar_t* wOut, int nStat, UnitAny* pItem, int nStatParam) {
	if (!(Toggles["Always Show Item Stat Ranges"].state || GetKeyState(VK_CONTROL) & 0x8000) || pItem == nullptr || pItem->dwType != UNIT_ITEM) {
		return;
	}

	ItemsTxtStat* stat = nullptr;
	ItemsTxtStat* all_stat = nullptr; // Stat for common modifer like all-res, all-stats

	switch (pItem->pItemData->dwQuality) {
	case ITEM_QUALITY_SET:
	{
		SetItemsTxt * pTxt = &(*p_D2COMMON_sgptDataTable)->pSetItemsTxt[pItem->pItemData->dwFileIndex];
		if (!pTxt)
			break;
		stat = GetItemsTxtStatByMod(pTxt->hStats, 9 + 10, nStat, nStatParam);
		if (stat)
			all_stat = GetAllStatModifier(pTxt->hStats, 9 + 10, nStat, stat);
	}
	case ITEM_QUALITY_UNIQUE:
	{
		if (pItem->pItemData->dwQuality == ITEM_QUALITY_UNIQUE) {
			UniqueItemsTxt * pTxt = &(*p_D2COMMON_sgptDataTable)->pUniqueItemsTxt[pItem->pItemData->dwFileIndex];
			if (pTxt == nullptr) {
				break;
			}

			stat = GetItemsTxtStatByMod(pTxt->hStats, 12, nStat, nStatParam);

			if (stat != nullptr) {
				all_stat = GetAllStatModifier(pTxt->hStats, 12, nStat, stat);
			}
		}
		
		if (stat != nullptr) {
			int statMin = stat->dwMin;
			int statMax = stat->dwMax;

			if (all_stat != nullptr) {
				statMin += all_stat->dwMin;
				statMax += all_stat->dwMax;
			}

			if (statMin < statMax) {
				int	aLen = wcslen(wOut);
				int leftSpace = 128 - aLen > 0 ? 128 - aLen : 0;

				if (nStat == STAT_LIFEPERLEVEL || nStat == STAT_MANAPERLEVEL || nStat == STAT_MAXENHANCEDDMGPERLEVEL || nStat == STAT_MAXDAMAGEPERLEVEL)
				{
					statMin = D2COMMON_GetBaseStatSigned(D2CLIENT_GetPlayerUnit(), STAT_LEVEL, 0) * statMin >> 3;
					statMax = D2COMMON_GetBaseStatSigned(D2CLIENT_GetPlayerUnit(), STAT_LEVEL, 0) * statMax >> 3;
				}
				if (leftSpace) {
					swprintf_s(wOut + aLen, leftSpace, L" %s[%d - %d]%s", GetColorCode(TextColor::DarkGreen).c_str(), statMin, statMax, GetColorCode(TextColor::Blue).c_str());
				}
			}
		}
	} break;
	default:
	{
		if (pItem->pItemData->dwFlags & ITEM_RUNEWORD) {
			RunesTxt* pTxt = GetRunewordTxtById(pItem->pItemData->wPrefix[0]);
			if (!pTxt)
				break;
			stat = GetItemsTxtStatByMod(pTxt->hStats, 7, nStat, nStatParam);
			if (stat) {
				int statMin = stat->dwMin;
				int statMax = stat->dwMax;

				all_stat = GetAllStatModifier(pTxt->hStats, 7, nStat, stat);

				if (all_stat) {
					statMin += all_stat->dwMin;
					statMax += all_stat->dwMax;
				}

				if (stat->dwMin != stat->dwMax) {
					int	aLen = wcslen(wOut);
					int leftSpace = 128 - aLen > 0 ? 128 - aLen : 0;

					if (nStat == STAT_LIFEPERLEVEL || nStat == STAT_MANAPERLEVEL || nStat == STAT_MAXENHANCEDDMGPERLEVEL || nStat == STAT_MAXDAMAGEPERLEVEL)
					{
						statMin = D2COMMON_GetBaseStatSigned(D2CLIENT_GetPlayerUnit(), STAT_LEVEL, 0) * statMin >> 3;
						statMax = D2COMMON_GetBaseStatSigned(D2CLIENT_GetPlayerUnit(), STAT_LEVEL, 0) * statMax >> 3;
					}
					if (leftSpace)
						swprintf_s(wOut + aLen, leftSpace, L" %s[%d - %d]%s", GetColorCode(TextColor::DarkGreen).c_str(), statMin, statMax, GetColorCode(TextColor::Blue).c_str());
				}
			}
		}
		else if (pItem->pItemData->dwQuality == ITEM_QUALITY_MAGIC || pItem->pItemData->dwQuality == ITEM_QUALITY_RARE || pItem->pItemData->dwQuality == ITEM_QUALITY_CRAFT)
		{
			int nAffixes = *p_D2COMMON_AutoMagicTxt - D2COMMON_GetItemMagicalMods(1); // Number of affixes without Automagic
			int min = 0, max = 0;
			int type = D2COMMON_GetItemType(pItem);
			for (int i = 1;; ++i) {
				if (!pItem->pItemData->wAutoPrefix && i > nAffixes) // Don't include Automagic.txt affixes if item doesn't use them
					break;
				AutoMagicTxt* pTxt = D2COMMON_GetItemMagicalMods(i);
				if (!pTxt)
					break;
				//Skip if stat level is > 99 or affix is prelod
				if (pTxt->dwLevel > 99 || !pTxt->wVersion)
					continue;
				//Skip if stat is not spawnable
				if (pItem->pItemData->dwQuality < ITEM_QUALITY_CRAFT && !pTxt->wSpawnable)
					continue;
				//Skip for rares+
				if (pItem->pItemData->dwQuality >= ITEM_QUALITY_RARE  && !pTxt->nRare)
					continue;
				//Firstly check Itemtype
				bool found_itype = false;
				bool found_etype = false;

				for (int j = 0; j < 5; ++j)
				{
					if (!pTxt->wEType[j] || pTxt->wEType[j] == 0xFFFF)
						break;
					if (D2COMMON_IsMatchingType(pItem, pTxt->wEType[j])) {
						found_etype = true;
						break;
					}
				}
				if (found_etype) // next if excluded type
					continue;

				for (int j = 0; j < 7; ++j)
				{
					if (!pTxt->wIType[j] || pTxt->wIType[j] == 0xFFFF)
						break;
					if (D2COMMON_IsMatchingType(pItem, pTxt->wIType[j])) {
						found_itype = true;
						break;
					}
				}
				if (!found_itype)
					continue;

				stat = GetItemsTxtStatByMod(pTxt->hMods, 3, nStat, nStatParam);
				if (!stat)
					continue;
				min = min == 0 ? stat->dwMin : ((stat->dwMin < min) ? stat->dwMin : min);
				max = (stat->dwMax > max) ? stat->dwMax : max;
				//DEBUGMSG(L"%s: update min to %d, and max to %d (record #%d)", wOut, min, max, i)
			}
			if (min < max) {
				int	aLen = wcslen(wOut);
				int leftSpace = 128 - aLen > 0 ? 128 - aLen : 0;
				if (nStat == STAT_MAXENHANCEDDMGPERLEVEL || nStat == STAT_MAXDAMAGEPERLEVEL || nStat == STAT_LIFEPERLEVEL || nStat == STAT_MANAPERLEVEL)
				{
					min = D2COMMON_GetBaseStatSigned(D2CLIENT_GetPlayerUnit(), STAT_LEVEL, 0) * min >> 3;
					max = D2COMMON_GetBaseStatSigned(D2CLIENT_GetPlayerUnit(), STAT_LEVEL, 0) * max >> 3;
				}
				if (leftSpace)
					swprintf_s(wOut + aLen, leftSpace, L" %s[%d - %d]%s", GetColorCode(TextColor::DarkGreen).c_str(), min, max, GetColorCode(TextColor::Blue).c_str());
			}
		}

	} break;

	}
}

/*
	Search mod used in MagicPrefix.txt, UniqueItemsTxt, RunesTxt, etc. (index from Properties.txt) by ItemStatCost.txt stat index
	@param nStatParam - param column for property (skill id etc)
	@param nStat - ItemStatCost.txt record id
	@param nStats - number of pStats
	@param pStats - pointer to ItemsTxtStat* array [PropertiesTxt Id, min, max val)
*/
ItemsTxtStat* GetItemsTxtStatByMod(ItemsTxtStat* pStats, int nStats, int nStat, int nStatParam)
{
	if (nStat == STAT_SKILLONKILL || nStat == STAT_SKILLONHIT || nStat == STAT_SKILLONSTRIKING || nStat == STAT_SKILLONDEATH ||
		nStat == STAT_SKILLONLEVELUP || nStat == STAT_SKILLWHENSTRUCK || nStat == STAT_CHARGED ||
		nStat == STAT_MINIMUMCOLDDAMAGE || nStat == STAT_MINIMUMLIGHTNINGDAMAGE || nStat == STAT_MINIMUMFIREDAMAGE || nStat == STAT_MINIMUMPOISONDAMAGE || nStat == STAT_MINIMUMMAGICALDAMAGE) // Skip skills without ranges
	{
		return nullptr;
	}
	for (int i = 0; i<nStats; ++i) {
		if (pStats[i].dwProp == 0xffffffff) {
			break;
		}
		PropertiesTxt * pProp = &(*p_D2COMMON_sgptDataTable)->pPropertiesTxt[pStats[i].dwProp];
		if (pProp == nullptr) {
			break;
		}
		if (pProp->wStat[0] == 0xFFFF && pProp->nFunc[0] == 7 && (nStat == STAT_ENHANCEDDAMAGE || nStat == STAT_ENHANCEDMINIMUMDAMAGE || nStat == STAT_ENHANCEDMAXIMUMDAMAGE ||
			nStat == STAT_MAXENHANCEDDMGPERTIME || nStat == STAT_MAXENHANCEDDMGPERLEVEL)) {
			return &pStats[i];
		}
		else if (pProp->wStat[0] == 0xFFFF && pProp->nFunc[0] == 6 && (nStat == STAT_MAXIMUMDAMAGE || nStat == STAT_SECONDARYMAXIMUMDAMAGE ||
			nStat == STAT_MAXDAMAGEPERTIME || nStat == STAT_MAXDAMAGEPERLEVEL)) {
			return &pStats[i];
		}
		else if (pProp->wStat[0] == 0xFFFF && pProp->nFunc[0] == 5 && (nStat == STAT_MINIMUMDAMAGE || nStat == STAT_SECONDARYMINIMUMDAMAGE)) {
			return &pStats[i];
		}
		for (int j = 0; j < 7; ++j)
		{
			if (pProp->wStat[j] == 0xFFFF) {
				break;
			}
			if (pProp->wStat[j] == nStat && pStats[i].dwPar == nStatParam) {
				return &pStats[i];
			}
		}
	}
	return nullptr;
}

/*
	Find other mod that inflates the original
	@param pOrigin  - original stat
	@param nStat - ItemStatCost.txt record id
	@param nStats - number of pStats
	@param pStats - pointer to ItemsTxtStat* array [PropertiesTxt Id, min, max val)
*/
ItemsTxtStat* GetAllStatModifier(ItemsTxtStat* pStats, int nStats, int nStat, ItemsTxtStat* pOrigin)
{
	for (int i = 0; i<nStats; ++i) {
		if (pStats[i].dwProp == 0xffffffff)
			break;
		if (pStats[i].dwProp == pOrigin->dwProp)
			continue;

		PropertiesTxt * pProp = &(*p_D2COMMON_sgptDataTable)->pPropertiesTxt[pStats[i].dwProp];
		if (pProp == nullptr) {
			break;
		}

		for (int j = 0; j < 7; ++j) {
			if (pProp->wStat[j] == 0xFFFF) {
				break;
			}
			if (pProp->wStat[j] == nStat) {
				return &pStats[i];
			}
		}
	}
	return nullptr;
}

RunesTxt* GetRunewordTxtById(int rwId)
{
	int n = *(D2COMMON_GetRunesTxtRecords());
	for (int i = 1; i < n; ++i)
	{
		RunesTxt* pTxt = D2COMMON_GetRunesTxt(i);
		if (!pTxt)
			break;
		if (pTxt->dwRwId == rwId)
			return pTxt;
	}
	return 0;
}

UnitAny* Item::GetViewUnit ()
{
	UnitAny* view = (viewingUnit) ? viewingUnit : D2CLIENT_GetPlayerUnit();
	if (view->dwUnitId == D2CLIENT_GetPlayerUnit()->dwUnitId)
		return D2CLIENT_GetPlayerUnit();

	Drawing::Texthook::Draw(*p_D2CLIENT_PanelOffsetX + 160 + 320, 300, Drawing::Center, 0, White, "%s", viewingUnit->pPlayerData->szName);
	return viewingUnit;
}

void __declspec(naked) ItemName_Interception()
{
	__asm {
		mov ecx, edi
		mov edx, ebx
		call Item::ItemNamePatch
		mov al, [ebp+0x12a]
		ret
	}
}


__declspec(naked) void __fastcall GetProperties_Interception()
{
	__asm
	{
		push eax
		call Item::OnProperties
		add esp, 0x808
		ret 12
	}
}

/*	Wrapper over D2CLIENT.0x2E04B (1.13d)
	BOOL __userpurge ITEMS_BuildDamagePropertyDesc@<eax>(DamageStats *pStats@<eax>, int nStat, wchar_t *wOut)
	Function is pretty simple so I decided to rewrite it.
	@esp-0x20:	pItem
*/
void __declspec(naked) GetItemPropertyStringDamage_Interception()
{
	__asm {
		push[esp + 8]			// wOut
		push[esp + 8]			// nStat
		push eax				// pStats
		push[esp - 0x20 + 12]	// pItem

		call Item::OnDamagePropertyBuild

		ret 8
	}
}

/* Wrapper over D2CLIENT.0x2E06D (1.13d)
	As far I know this: int __userpurge ITEMS_ParseStats_6FADCE40<eax>(signed __int32 nStat<eax>, wchar_t *wOut<esi>, UnitAny *pItem, StatListEx *pStatList, DWORD nStatParam, DWORD nStatValue, int a7)
	Warning: wOut is 128 words length only!
	@ebx the nStat value
	@edi pStatListEx
	@esp-0x10 seems to always keep pItem *careful*
*/
void __declspec(naked) GetItemPropertyString_Interception()
{
	static DWORD rtn = 0; // if something is stupid but works then it's not stupid!
	__asm
	{
		pop rtn
		// Firstly generate string using old function
		call D2CLIENT_ParseStats_J
		push rtn

		push [esp - 4] // preserve nStatParam

		push eax // Store result
		mov eax, [esp - 0x10 + 8 + 4] // pItem
		push ecx
		push edx

		// Then pass the output to our func
		push [esp + 12] // nStatParam
		push eax // pItem
		push ebx // nStat
		push esi // wOut

		call Item::OnPropertyBuild

		pop edx
		pop ecx
		pop eax

		add esp, 4 // clean nStatParam

		ret
	}
}

void __declspec(naked) ViewInventoryPatch1_ASM()
{
	__asm {
		push eax;
		call Item::GetViewUnit;
		mov esi, eax;
		pop eax;
		ret;
	}
}
void __declspec(naked) ViewInventoryPatch2_ASM()
{
	__asm {
		push eax;
		call Item::GetViewUnit;
		mov ebx, eax;
		pop eax;
		ret;
	}
}
void __declspec(naked) ViewInventoryPatch3_ASM()
{
	__asm
	{
		push eax;
		push ebx;
		call Item::GetViewUnit;

		mov ebx, [edi];
		cmp ebx, 1;
		je OldCode;

		mov edi, eax;

		OldCode:
		pop ebx;
		pop eax;
		test eax, eax;
		mov ecx, dword ptr [edi + 0x60];

		ret;
	}
}
