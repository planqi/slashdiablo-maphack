/**
*
* Item.h
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

#pragma once
#include "../Module.h"
#include "../../Constants.h"
#include "../../Config.h"
#include "../../Drawing.h"

struct UnitAny;

class Item : public Module {
	private:
		static map<std::string, Toggle> Toggles;
		unsigned int showPlayer;
		static UnitAny* viewingUnit;
		Drawing::UITab* settingsTab;
		static unsigned int filterLevelSetting;
		static unsigned int pingLevelSetting;

		void ResetPatches();
	public:

		Item() : Module("Item") {};

		void OnLoad();
		void OnUnload();

		void LoadConfig();
		void DrawSettings();

		void OnGameJoin();

		void OnLoop();
		void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);
		void OnLeftClick(bool up, int x, int y, bool* block);
		std::map<string, Toggle>* GetToggles() { return &Toggles; }

		static void __fastcall ItemNamePatch(wchar_t *name, UnitAny *item);
		static void OrigGetItemName(UnitAny *item, string &itemName, char *code);
		static void __stdcall OnProperties(wchar_t *wTxt);
		static BOOL __stdcall OnDamagePropertyBuild(UnitAny* pItem, DamageStats* pDmgStats, int nStat, wchar_t* wOut);
		static void __stdcall OnPropertyBuild(wchar_t* wOut, int nStat, UnitAny* pItem, int nStatParam);

		static BOOL PermShowItemsPatch1();
		static BOOL PermShowItemsPatch2();
		static BOOL PermShowItemsPatch3();

		static UnitAny* GetViewUnit();

		static unsigned int GetFilterLevel() { return filterLevelSetting; }
		static unsigned int GetPingLevel() { return pingLevelSetting; }

};

void ItemName_Interception();
void __fastcall GetProperties_Interception();
void GetItemPropertyStringDamage_Interception();
void GetItemPropertyString_Interception();
void ViewInventoryPatch1_ASM();
void ViewInventoryPatch2_ASM();
void ViewInventoryPatch3_ASM();

void PermShowItemsPatch1_ASM();
void PermShowItemsPatch2_ASM();
void PermShowItemsPatch3_ASM();
void PermShowItemsPatch4_ASM();

struct UnitItemInfo;
int CreateUnitItemInfo(UnitItemInfo *uInfo, UnitAny *item);

// reset all rule lookup caches
void ResetCaches();

