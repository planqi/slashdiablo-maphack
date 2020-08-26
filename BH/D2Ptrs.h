/**
*
* D2Ptrs.h
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
// Macros adapted from lord2800's macros.
#include "Patch.h"
#include "D2Version.h"
#include "D2Structs.h"
#include "D2Stubs.h"


#ifdef _DEFINE_PTRS
#define FUNCPTR(dll, name, callingret, args, ...) \
	static Offsets f##dll##_##name##_offsets = { __VA_ARGS__ }; \
	__declspec(naked) callingret dll##_##name##args \
	{ \
		static DWORD f##dll##_##name = NULL; \
		if(f##dll##_##name == NULL) \
		{ \
		__asm { pushad } \
		f##dll##_##name = Patch::GetDllOffset(dll, *(&f##dll##_##name##_offsets._113c + D2Version::GetGameVersionID())); \
		__asm { popad } \
		} \
		__asm jmp [f##dll##_##name] \
	}

#define ASMPTR(dll, name, ...) \
	DWORD* Asm_##dll##_##name##(VOID) \
	{ \
		static DWORD f##Asm_##dll##_##name = NULL; \
		if(f##Asm_##dll##_##name## == NULL) \
		{ \
		static Offsets f##Asm_##_##name##_offsets = { __VA_ARGS__ }; \
		static int address = *(&f##Asm_##_##name##_offsets._113c + D2Version::GetGameVersionID()); \
		f##Asm_##dll##_##name## = Patch::GetDllOffset(dll, address); \
		} \
		return &##f##Asm_##dll##_##name; \
	} 

#define VARPTR(dll, name, type, ...) \
	type** Var_##dll##_##name##(VOID) \
	{ \
		static DWORD f##Var_##dll##_##name = NULL; \
		if(f##Var_##dll##_##name## == NULL) \
		{ \
		static Offsets f##Var_##_##name##_offsets = { __VA_ARGS__ }; \
		static int address = *(&f##Var_##_##name##_offsets._113c + D2Version::GetGameVersionID()); \
		f##Var_##dll##_##name## = Patch::GetDllOffset(dll, address); \
		} \
		return (type**)&##f##Var_##dll##_##name; \
	} 

#else
#define FUNCPTR(dll, name, callingret, args, ...) extern callingret dll##_##name##args;
#define ASMPTR(dll, name, ...) extern DWORD* Asm_##dll##_##name##(VOID); static DWORD dll##_##name = *Asm_##dll##_##name##();
#define VARPTR(dll, name, type, ...) extern type** Var_##dll##_##name##(VOID); static type* p##_##dll##_##name = (type*)*Var_##dll##_##name##();
#endif

FUNCPTR(D2CLIENT, GetQuestInfo, void* __stdcall, (void), 0x45A00, 0x78A80)

FUNCPTR(D2CLIENT, SubmitItem, void __fastcall, (DWORD dwItemId), 0x45FB0, 0x79670)
FUNCPTR(D2CLIENT, Transmute, void __fastcall, (void), 0x8CB90, 0x94370)

FUNCPTR(D2CLIENT, FindClientSideUnit, UnitAny* __fastcall, (DWORD dwId, DWORD dwType), 0xA5B20, 0x620B0)
FUNCPTR(D2CLIENT, FindServerSideUnit, UnitAny* __fastcall, (DWORD dwId, DWORD dwType), 0xA5B40, 0x620D0)
FUNCPTR(D2CLIENT, GetCurrentInteractingNPC, UnitAny* __fastcall, (void), 0x46150, 0x790D0)
FUNCPTR(D2CLIENT, GetSelectedUnit, UnitAny * __stdcall, (void), 0x51A80, 0x17280)
FUNCPTR(D2CLIENT, GetCursorItem, UnitAny* __fastcall, (void), 0x16020, 0x144A0)
FUNCPTR(D2CLIENT, GetMercUnit, UnitAny* __fastcall, (void), 0x97CD0, 0x9C0A0)
// FUNCPTR(D2CLIENT, UnitTestSelect, DWORD __stdcall, (UnitAny* pUnit, DWORD _1, DWORD _2, DWORD _3), 0x8D030) // unused but we need to use it

FUNCPTR(D2CLIENT, SetSelectedUnit_I, void __fastcall, (UnitAny *pUnit), 0x51860, 0x17060)
FUNCPTR(D2CLIENT, GetItemName, BOOL __stdcall, (UnitAny* pItem, wchar_t* wBuffer, DWORD dwSize), 0x914F0, 0x958C0)
// FUNCPTR(D2CLIENT, GetItemNameString, void __stdcall, (UnitAny *pItem, wchar_t *wItemName, int nLen), 0x914F0)
FUNCPTR(D2CLIENT, LoadItemDesc, BOOL __stdcall, (UnitAny* pPlayer, int type), 0x97820) // 1.13d

FUNCPTR(D2CLIENT, GetMonsterOwner, DWORD __fastcall, (DWORD nMonsterId), 0x216A0, 0x8E3D0)
FUNCPTR(D2CLIENT, GetUnitHPPercent, DWORD __fastcall, (DWORD dwUnitId), 0x21580, 0x8E2B0)
FUNCPTR(D2CLIENT, InitInventory, void __fastcall, (void), 0x908C0, 0x93280)
FUNCPTR(D2CLIENT, SetUIVar, DWORD __fastcall, (DWORD varno, DWORD howset, DWORD unknown1), 0xC2790, 0x1C190)
FUNCPTR(D2CLIENT, GetUnitX, int __fastcall, (UnitAny* pUnit), 0x1630, 0x1210)
FUNCPTR(D2CLIENT, GetUnitY, int __fastcall, (UnitAny* pUnit), 0x1660, 0x1240)

FUNCPTR(D2CLIENT, ShopAction, void __fastcall, (UnitAny* pItem, UnitAny* pNpc, UnitAny* pNpc2, DWORD dwSell, DWORD dwItemCost, DWORD dwMode, DWORD _2, DWORD _3), 0x47D60, 0x7D030)

FUNCPTR(D2CLIENT, CloseNPCInteract, void __fastcall, (void), 0x492F0, 0x7BC10)
//FUNCPTR(D2CLIENT, ClearScreen, void __fastcall, (void), 0x492F0) // unused but I want to look into using it // wrong function
FUNCPTR(D2CLIENT, CloseInteract, void __fastcall, (void), 0x43870, 0x44980)

FUNCPTR(D2CLIENT, GetAutomapSize, DWORD __stdcall, (void), 0x5F080, 0x6FDD0)
FUNCPTR(D2CLIENT, NewAutomapCell, AutomapCell * __fastcall, (void), 0x5F6B0, 0x703C0)
FUNCPTR(D2CLIENT, AddAutomapCell, void __fastcall, (AutomapCell *aCell, AutomapCell **node), 0x61320, 0x71EA0)
FUNCPTR(D2CLIENT, RevealAutomapRoom, void __stdcall, (Room1 *pRoom1, DWORD dwClipFlag, AutomapLayer *aLayer), 0x62580, 0x73160)
FUNCPTR(D2CLIENT, InitAutomapLayer_I, AutomapLayer* __fastcall, (DWORD nLayerNo), 0x62710, 0x733D0)

FUNCPTR(D2CLIENT, ClickMap, void __stdcall, (DWORD MouseFlag, DWORD x, DWORD y, DWORD Type), 0x1BF20, 0x2B420)
FUNCPTR(D2CLIENT, LeftClickItem, void __stdcall, (UnitAny* pPlayer, Inventory* pInventory, int x, int y, DWORD dwClickType, InventoryLayout* pLayout, DWORD Location), 0x96AA0, 0x9AFF0) // 1.12

FUNCPTR(D2CLIENT, GetMouseXOffset, DWORD __fastcall, (void), 0x3F6C0, 0x5BC20)
FUNCPTR(D2CLIENT, GetMouseYOffset, DWORD __fastcall, (void), 0x3F6D0, 0x5BC30)

FUNCPTR(D2CLIENT, PrintPartyString, void __stdcall, (wchar_t *wMessage, int nColor), 0x7D610, 0x75C70) // unused but I want to look into using it too
FUNCPTR(D2CLIENT, PrintGameString, void __stdcall, (wchar_t *wMessage, int nColor), 0x7D850, 0x75EB0)

FUNCPTR(D2CLIENT, LeaveParty, void __fastcall, (void), 0x9E5D0, 0xA26A0)

FUNCPTR(D2CLIENT, AcceptTrade, void __fastcall, (void), 0x59600, 0x11F70)
FUNCPTR(D2CLIENT, CancelTrade, void __fastcall, (void), 0x595C0, 0x11F30)

FUNCPTR(D2CLIENT, GetDifficulty, BYTE __stdcall, (void), 0x41930, 0x42980)

FUNCPTR(D2CLIENT, ExitGame, void __fastcall, (void), 0x42850, 0x43870)

FUNCPTR(D2CLIENT, GetUiVar_I, DWORD __fastcall, (DWORD dwVarNo), 0xBE400, 0x17C50)

FUNCPTR(D2CLIENT, DrawRectFrame, void __fastcall, (DWORD Rect), 0xBE4C0, 0x17D10)

FUNCPTR(D2CLIENT, PerformGoldDialogAction, void __fastcall, (void), 0xBFDF0, 0x197F0)

FUNCPTR(D2CLIENT, GetPlayerUnit, UnitAny* __stdcall, (void), 0xA4D60, 0x613C0)

FUNCPTR(D2CLIENT, GetLevelName_I, wchar_t* __fastcall, (DWORD levelId), 0xBE240, 0x18250)

FUNCPTR(D2GFX, DrawAutomapCell, void __stdcall, (CellContext *context, DWORD xpos, DWORD ypos, RECT *cliprect, DWORD bright), -10079, -10060)
ASMPTR(D2CLIENT, OverrideShrinePatch_ORIG, 0x1155B8, 0x101B08)//Updated 1.13c

////////////////////////////////////////////////////////////////////////////////////////////////
// D2Client Globals
////////////////////////////////////////////////////////////////////////////////////////////////

VARPTR(D2CLIENT, ScreenSizeX, DWORD, 0xDBC48, 0xF7034)
VARPTR(D2CLIENT, ScreenSizeY, DWORD, 0xDBC4C, 0xF7038)

VARPTR(D2CLIENT, CursorHoverX, DWORD, 0xE0EB8, 0xEE4AC)
VARPTR(D2CLIENT, CursorHoverY, DWORD, 0xE0EBC, 0xEE4B0)

VARPTR(D2CLIENT, MouseX, DWORD, 0x11B828, 0x11C950)
VARPTR(D2CLIENT, MouseY, DWORD, 0x11B824, 0x11C94C)

VARPTR(D2CLIENT, MouseOffsetY, int, 0x11995C, 0x106840)
VARPTR(D2CLIENT, MouseOffsetX, int, 0x119960, 0x106844)

VARPTR(D2CLIENT, AutomapOn, DWORD, 0xFADA8, 0x11C8B8)
VARPTR(D2CLIENT, AutomapMode, int, 0xF16B0, 0xF34F8)
VARPTR(D2CLIENT, Offset, POINT, 0x11C1F8, 0x11CF5C)
VARPTR(D2CLIENT, xShake, int, 0x11BF00, 0x11CA6C) //ScreenShake	
VARPTR(D2CLIENT, yShake, int, 0x10B9DC, 0xFC3DC) //ScreenShake
VARPTR(D2CLIENT, AutomapLayer, AutomapLayer*, 0x11C1C4, 0x11CF28)

VARPTR(D2CLIENT, MercStrIndex, WORD, 0xF23E8, 0xF02D8)
VARPTR(D2CLIENT, MercReviveCost, DWORD, 0x11C334, 0x11CEE8)

VARPTR(D2CLIENT, GoldDialogAction, DWORD, 0xFAD5C, 0x11C86C)
VARPTR(D2CLIENT, GoldDialogAmount, DWORD, 0x11BBB0, 0x11D568)

VARPTR(D2CLIENT, NPCMenu, NPCMenu*, 0xF3BA0, 0xF1A90)
VARPTR(D2CLIENT, NPCMenuAmount, DWORD, 0xF42F0, 0xF21E0)

VARPTR(D2CLIENT, TradeLayout, InventoryLayout, 0x10B288, 0x101598)
VARPTR(D2CLIENT, StashLayout, InventoryLayout, 0x10B2D0, 0x1015E0)
VARPTR(D2CLIENT, ClassicStashLayout, InventoryLayout, 0x10B398, 0x1016A8)
VARPTR(D2CLIENT, StoreLayout, InventoryLayout, 0x10B3B0, 0x1016C0)
VARPTR(D2CLIENT, CubeLayout, InventoryLayout, 0x10B3C8, 0x1016D8)
VARPTR(D2CLIENT, InventoryLayout, InventoryLayout, 0x10B3E0, 0x1016F0)
VARPTR(D2CLIENT, MercLayout, InventoryLayout, 0x11BD94, 0x11CC84)
ASMPTR(D2CLIENT, clickParty_I, 0x9E180, 0xA2250)

VARPTR(D2CLIENT, PanelOffsetX, int, 0x11B9A0, 0x11D354);

VARPTR(D2CLIENT, RegularCursorType, DWORD, 0x11B864, 0x11C98C)
VARPTR(D2CLIENT, ShopCursorType, DWORD, 0x11BC34, 0x11CB24)


VARPTR(D2CLIENT, Ping, DWORD, 0x119804, 0x108764)
VARPTR(D2CLIENT, FPS, DWORD, 0x11C2AC, 0x11CE10)
VARPTR(D2CLIENT, Skip, DWORD, 0x119810, 0x108770)
VARPTR(D2CLIENT, Divisor, int, 0xF16B0, 0xF34F8)

VARPTR(D2CLIENT, OverheadTrigger, DWORD, 0x113ACE, 0x101ABE)

VARPTR(D2CLIENT, RecentInteractId, DWORD, 0x11971D, 0x101895)

VARPTR(D2CLIENT, ItemPriceList, DWORD, 0x11973B, 0x1018B3)

VARPTR(D2CLIENT, TransactionDialog, void*, 0x11975B, 0x1018D3)
VARPTR(D2CLIENT, TransactionDialogs, DWORD, 0x11BC08, 0x11D58C)
VARPTR(D2CLIENT, TransactionDialogs_2, DWORD, 0x11BC04, 0x11D588)

VARPTR(D2CLIENT, GameInfo, GameStructInfo*, 0x11B980, 0x109738)

VARPTR(D2CLIENT, WaypointTable, DWORD, 0xFCDD1, 0x1088FD)

VARPTR(D2CLIENT, PlayerUnit, UnitAny*, 0x11BBFC, 0x11D050)
VARPTR(D2CLIENT, SelectedInvItem, UnitAny*, 0x11BC38, 0x11CB28)
//VARPTR(D2CLIENT, SelectedUnit, UnitAny*, 0x11C4D8) // unused, but can we use it somewhere maybe? // 1.12 still
VARPTR(D2CLIENT, PlayerUnitList, RosterUnit*, 0x11BC14, 0x11CB04)

VARPTR(D2CLIENT, bWeapSwitch, DWORD, 0x11BC94, 0x11CB84)

VARPTR(D2CLIENT, bTradeAccepted, DWORD, 0x11BE64, 0x11CD54)
VARPTR(D2CLIENT, bTradeBlock, DWORD, 0x11BE74, 0x11CD64)
// VARPTR(D2CLIENT, RecentTradeName, wchar_t*, 0x12334C)
VARPTR(D2CLIENT, RecentTradeId, DWORD, 0x11C2CC, 0x11D5AC)

VARPTR(D2CLIENT, ExpCharFlag, DWORD, 0x119854, 0x1087B4)

VARPTR(D2CLIENT, MapId, DWORD, 0x11C3BC, 0x11D204) // unused but I want to add it

VARPTR(D2CLIENT, AlwaysRun, DWORD, 0x11C3EC, 0x11D234)
VARPTR(D2CLIENT, NoPickUp, DWORD, 0x11C2F0, 0x11D574) // unused but I want to add it

// VARPTR(D2CLIENT, ScreenCovered, DWORD, 0x1E8F9) // unused, appears to be an int specifying which screens (if any) are opened...

VARPTR(D2CLIENT, ChatMsg, wchar_t*, 0x11EC80, 0x11D650)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Client Stubs
////////////////////////////////////////////////////////////////////////////////////////////////

ASMPTR(D2CLIENT, TakeWaypoint_I, 0xAA8B3, 0x3F5F3)

ASMPTR(D2CLIENT, ClickShopItem_I, 0x46EE0, 0x7A180)
ASMPTR(D2CLIENT, ClickBelt_I, 0x28260, 0x6E310)
ASMPTR(D2CLIENT, ClickBeltRight_I, 0x29990, 0x6FA40)
ASMPTR(D2CLIENT, ClickItemRight_I, 0x98B60, 0x9CF30)
ASMPTR(D2CLIENT, MercItemAction_I, 0x14A10, 0xB6B00)

ASMPTR(D2CLIENT, Interact_I, 0x1BDE0, 0x2B2E0)

ASMPTR(D2CLIENT, ClickParty_I, 0x9E180, 0xA2250)
ASMPTR(D2CLIENT, ClickParty_II, 0x773A0, 0x88A50)

ASMPTR(D2CLIENT, ShopAction_I, 0x47D60, 0x7D030)

ASMPTR(D2CLIENT, GetUnitName_I, 0xA5D90, 0x622E0)
ASMPTR(D2CLIENT, GetItemDesc_I, 0x560B0, 0x2E380)

ASMPTR(D2CLIENT, AssignPlayer_I, 0xA7630, 0x63C70)

//ASMPTR(D2CLIENT, TestPvpFlag_I, 0x4FCE0)
ASMPTR(D2CLIENT, TestPvpFlag_I, 0x4FD90, 0x6A720)

ASMPTR(D2CLIENT, PlaySound_I, 0x88A70, 0x26270)

ASMPTR(D2CLIENT, InputCall_I, 0x147A0, 0xB6890)

ASMPTR(D2CLIENT, Say_I, 0x70EC6, 0xB27A6)

ASMPTR(D2CLIENT, BodyClickTable, 0xE0EC4, 0xEE4B8)

ASMPTR(D2CLIENT, LoadUiImage_I, 0x2B420, 0xA9480)

ASMPTR(D2CLIENT, GetMinionCount_I, 0x217E0, 0x8E5B0)

ASMPTR(D2CLIENT, GameLeave_I, 0x5D110, 0xB4370)

ASMPTR(D2CLIENT, ClickMap_I, 0xFADA4, 0x11C8B4)
ASMPTR(D2CLIENT, ClickMap_II, 0x11C4D8, 0x11D2CC)
ASMPTR(D2CLIENT, ClickMap_III, 0x3F5F0, 0x5BB50)
ASMPTR(D2CLIENT, ClickMap_IV, 0x1BF99, 0x2B499)

ASMPTR(D2CLIENT, GameAddUnit_I, 0xA6500, 0x628E0)

ASMPTR(D2CLIENT, LoadAct_1, 0x62AA0, 0x737F0)
ASMPTR(D2CLIENT, LoadAct_2, 0x62760, 0x2B420)

ASMPTR(D2CLIENT, GetUnitFromId_I, 0xA4E20, 0x61480)
VARPTR(D2CLIENT, pUnitTable, POINT, 0x10A608, 0x1047B8)

ASMPTR(D2CLIENT, HoveredUnit_I, 0x51A80, 0x17280)
ASMPTR(D2WIN, DrawTextBuffer, 0x12940, 0x134D0)

ASMPTR(D2CLIENT, ParseStats_J, 0x54E10, 0x2CE40)

////////////////////////////////////////////////////////////////////////////////////////////////
// D2Common Ordinals
////////////////////////////////////////////////////////////////////////////////////////////////

FUNCPTR(D2COMMON, InitLevel, void __stdcall, (Level *pLevel), 0x2E360, 0x6DDF0)
FUNCPTR(D2COMMON, UnloadAct, unsigned __stdcall, (Act* pAct), -10868, -10228)
FUNCPTR(D2COMMON, GetObjectTxt, ObjectTxt * __stdcall, (DWORD objno), 0x3E980, 0x1ADC0)
FUNCPTR(D2COMMON, LoadAct, Act* __stdcall, (DWORD ActNumber, DWORD MapId, DWORD Unk, DWORD Unk_2, DWORD Unk_3, DWORD Unk_4, DWORD TownLevelId, DWORD Func_1, DWORD Func_2), 0x3CB30, 0x24810)

FUNCPTR(D2COMMON, GetLevelText, LevelText * __stdcall, (DWORD levelno), -10014, -10142)
FUNCPTR(D2COMMON, GetObjectText, ObjectTxt * __stdcall, (DWORD objno), -10688, -10319)
FUNCPTR(D2COMMON, GetItemText, ItemText *__stdcall, (DWORD dwItemNo), -10695, -10994)

FUNCPTR(D2COMMON, GetLayer, AutomapLayer2* __fastcall, (DWORD dwLevelNo), -10749, -10087)
FUNCPTR(D2COMMON, GetLevel, Level * __fastcall, (ActMisc *pMisc, DWORD dwLevelNo), -10207, -10287)

FUNCPTR(D2COMMON, GetStatList, StatList* __stdcall, (UnitAny* pUnit, DWORD dwUnk, DWORD dwMaxEntries ), -10930, -10449)
FUNCPTR(D2COMMON, CopyStatList, DWORD __stdcall, (StatList* pStatList, Stat* pStatArray, DWORD dwMaxEntries), -10658, -10195)
FUNCPTR(D2COMMON, GetBaseStatSigned, int __stdcall, (UnitAny *pUnit, int nStat, int nLayer), -10587, -10216)
FUNCPTR(D2COMMON, GetUnitStat, DWORD __stdcall, (UnitAny* pUnit, DWORD dwStat, DWORD dwStat2), -10973, -10550)
FUNCPTR(D2COMMON, GetUnitState, int __stdcall, (UnitAny *pUnit, DWORD dwStateNo), -10494, -10706)

FUNCPTR(D2COMMON, CheckUnitCollision, DWORD __stdcall, (UnitAny* pUnitA, UnitAny* pUnitB, DWORD dwBitMask), -10839, -10221)
FUNCPTR(D2COMMON, GetRoomFromUnit,  Room1* __stdcall, (UnitAny * ptUnit), -10331, -10846)
FUNCPTR(D2COMMON, GetTargetUnitType, Path* __stdcall, (Path* pPath), -10392, -10704)

FUNCPTR(D2COMMON, GetSkillLevel, int __stdcall, (UnitAny* pUnit, Skill* pSkill, BOOL bTotal), -10306, -10007)

FUNCPTR(D2COMMON, GetItemLevelRequirement, DWORD __stdcall, (UnitAny* pItem, UnitAny* pPlayer), -11015, -10929)
FUNCPTR(D2COMMON, GetItemPrice, DWORD __stdcall, (UnitAny* MyUnit, UnitAny* pItem, DWORD U1_,DWORD U2_,DWORD U3_,DWORD U4_), -10107, -10186)
FUNCPTR(D2COMMON, GetRepairCost, DWORD __stdcall, (DWORD _1, UnitAny* pUnit, DWORD dwNpcId, DWORD dwDifficulty, DWORD dwItemPriceList, DWORD _2), -10071, -10807)
FUNCPTR(D2COMMON, GetItemMagicalMods, AutoMagicTxt* __stdcall, (int wPrefixNum), -10248, -10174)
FUNCPTR(D2COMMON, GetItemFromInventory, UnitAny *__stdcall, (Inventory* inv), -10460, -11132)
FUNCPTR(D2COMMON, GetNextItemFromInventory, UnitAny *__stdcall, (UnitAny *pItem), -10464, -10879)
FUNCPTR(D2COMMON, GetItemType, int __stdcall, (UnitAny *pItem), -11088, -10121)
FUNCPTR(D2COMMON, IsMatchingType, BOOL __stdcall, (UnitAny *pItem, int iType), -10744, -10601)

FUNCPTR(D2COMMON, GetRunesTxt, RunesTxt* __stdcall, (int RecordNo), -10006, -10405)
FUNCPTR(D2COMMON, GetRunesTxtRecords, int* __stdcall, (void), -11032, -10981)

FUNCPTR(D2COMMON, GenerateOverheadMsg, OverheadMsg* __stdcall, (DWORD dwUnk, char* szMsg, DWORD dwTrigger), -10454, -10538)
FUNCPTR(D2COMMON, FixOverheadMsg, void __stdcall, (OverheadMsg* pMsg, DWORD dwUnk), -10097, -10417)

FUNCPTR(D2COMMON, AddRoomData, void __stdcall, (Act * ptAct, int LevelId, int Xpos, int Ypos, Room1 * pRoom), -10401, -10890)
FUNCPTR(D2COMMON, RemoveRoomData, void __stdcall, (Act* ptAct, int LevelId, int Xpos, int Ypos, Room1* pRoom), -11099, -10208)

FUNCPTR(D2COMMON, GetQuestFlag, int __stdcall, (void* QuestInfo, DWORD dwAct, DWORD dwQuest), -10174, -10156)
FUNCPTR(D2COMMON, GetQuestFlag2, int __stdcall, (void *QuestInfo, DWORD quest, DWORD flag), -10174, -10156)

FUNCPTR(D2COMMON, AbsScreenToMap, void __stdcall, (long *pX, long *pY), -10474, -10720)
FUNCPTR(D2COMMON, MapToAbsScreen, void __stdcall, (long *pX, long *pY), -11087, -10115)

FUNCPTR(D2COMMON, CheckWaypoint, DWORD __stdcall, (DWORD WaypointTable, DWORD dwLevelId), -10373, -11029)

FUNCPTR(D2COMMON, IsTownByLevelNo, BOOL __stdcall, (DWORD dwLevelNo), -10416, -10367)
FUNCPTR(D2COMMON, IsTownByRoom, BOOL __stdcall, (Room1* pRoom1), -10057, -10691)

////////////////////////////////////////////////////////////////////////////////////////////////
// D2Common Globals
////////////////////////////////////////////////////////////////////////////////////////////////

VARPTR(D2COMMON, sgptDataTable, sgptDataTable*, 0x99E1C, 0xA33F0)
VARPTR(D2COMMON, AutoMagicTxt, AutoMagicTxt*, 0x9FBC8, 0xA4CE4)

VARPTR(D2COMMON, ArmorTxt, ItemsTxt*, 0x9FBA4, 0xA4CC0)
VARPTR(D2COMMON, ArmorTxtRecords, DWORD, 0x9FBA8, 0xA4CC4)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Common Stubs
////////////////////////////////////////////////////////////////////////////////////////////////

ASMPTR(D2COMMON, DisplayOverheadMsg_I, -10422, -10298)
ASMPTR(D2COMMON, GetLevelIdFromRoom_I, 0x3C000, 0x23B80)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Net Functions
////////////////////////////////////////////////////////////////////////////////////////////////

FUNCPTR(D2NET, SendPacket, void __stdcall, (size_t aLen, DWORD arg1, BYTE* aPacket), -10024, -10015)
FUNCPTR(D2NET, ReceivePacket_I, void __stdcall, (BYTE *aPacket, DWORD aLen), -10033, -10001)

// See http://www.blizzhackers.cc/viewtopic.php?f=182&t=449390&p=4498608&hilit=receivepacket#p4498608
//FUNCPTR(D2NET, ReceivePacket_II, void __fastcall, (DWORD* pExpectedSize, void *_1, BYTE *aPacket, DWORD aLen), 0x6BD0)
//FUNCPTR(D2NET, ReceivePacket_III, BOOL __fastcall, (DWORD* pExpectedSize, BYTE *aPacket, DWORD aLen), 0x63C0)

////////////////////////////////////////////////////////////////////////////////////////////////
// D2Net Globals
////////////////////////////////////////////////////////////////////////////////////////////////

VARPTR(D2NET, CriticalPacketSection, CRITICAL_SECTION, 0xB400, 0xB400) // unused but we need to use it


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Gfx Ordinals
////////////////////////////////////////////////////////////////////////////////////////////////

FUNCPTR(D2GFX, DrawLine, void __stdcall, (int X1, int Y1, int X2, int Y2, DWORD dwColor, DWORD dwUnk), -10010, -10013)
FUNCPTR(D2GFX, DrawRectangle, void __stdcall, (int X1, int Y1, int X2, int Y2, DWORD dwColor, DWORD dwTrans), -10014, -10028)
FUNCPTR(D2GFX, DrawAutomapCell2, void __stdcall, (CellContext* context, DWORD xpos, DWORD ypos, DWORD bright2, DWORD bright, BYTE *coltab), -10041, -10042)
FUNCPTR(D2GFX, DrawCellContextEx, void __stdcall, (CellContext *context, int Xpos, int Ypos, int dwl, int nTransLvl, BYTE Color), -10019, -10067)

FUNCPTR(D2GFX, GetScreenSize, DWORD __stdcall, (void), -10031, -10012) 

FUNCPTR(D2GFX, GetHwnd, HWND __stdcall, (void), -10048, -10007)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Gfx Globals
////////////////////////////////////////////////////////////////////////////////////////////////

VARPTR(D2GFX, VideoMode, WORD, 0x11258, 0x14A38)

////////////////////////////////////////////////////////////////////////////////////////////////
// D2Multi Functions
////////////////////////////////////////////////////////////////////////////////////////////////

FUNCPTR(D2MULTI, DoChat, void __fastcall, (void), 0xCB30, 0x11770)
FUNCPTR(D2MULTI, PrintChannelText, void __stdcall, (char *szText, DWORD dwColor), 0xFC90, 0x13F30)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Multi Globals
////////////////////////////////////////////////////////////////////////////////////////////////

VARPTR(D2MULTI, ChatBoxMsg, char*, 0x38F18, 0x1C150)
VARPTR(D2MULTI, GameListControl, Control*, 0x39CC0, 0x39FF0)//1.13c - Unchanged
VARPTR(D2MULTI, EditboxPreferences, ControlPreferences*, 0x19C60, 0x19C60)
VARPTR(D2MULTI, PassBox, Control*, 0x39CD8, 0x3A060)
VARPTR(D2MULTI, ChatInputBox, Control*, 0x3A0D0, 0x39FC0)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Multi Stubs
////////////////////////////////////////////////////////////////////////////////////////////////

ASMPTR(D2MULTI, ChannelChat_I, 0x108A6, 0x14BE6)
ASMPTR(D2MULTI, ChannelEmote_I, 0x107A6, 0x14850)
ASMPTR(D2MULTI, ChannelWhisper_I, 0x10000, 0x142F0)
ASMPTR(D2MULTI, ChannelInput_I, 0xD5B0, 0x11B80)

////////////////////////////////////////////////////////////////////////////////////////////////
// D2Cmp Ordinals
////////////////////////////////////////////////////////////////////////////////////////////////

FUNCPTR(D2CMP, InitCellFile, void __stdcall, (void *cellfile, CellFile **outptr, char *srcfile, DWORD lineno, DWORD filever, char *filename), -10006, -10008)
FUNCPTR(D2CMP, DeleteCellFile, void __stdcall, (CellFile *cellfile), -10106, -10084)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Lang Ordinals
////////////////////////////////////////////////////////////////////////////////////////////////

FUNCPTR(D2LANG, GetLocaleText, wchar_t* __fastcall, (WORD nLocaleTxtNo), -10003, -10004)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Lang Stubs
////////////////////////////////////////////////////////////////////////////////////////////////

ASMPTR(D2LANG, Say_II, 0xB0B0, 0x8C60)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Launch Globals
////////////////////////////////////////////////////////////////////////////////////////////////

VARPTR(D2LAUNCH, BnData, BnetData *, 0x25ABC, 0x25B30)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Win Functions
////////////////////////////////////////////////////////////////////////////////////////////////

FUNCPTR(D2WIN, DrawSprites, void __fastcall, (void), 0x18750, 0xEAA0)
FUNCPTR(D2WIN, LoadCellFile, CellFile* __fastcall, (const char* szFile, int Type), -10111, -10023)
FUNCPTR(D2WIN, DrawCellFile, void __fastcall, (CellFile * pCellFile, int xPos, int yPos, int div, int trans, int Color), -10138, -10172)


////////////////////////////////////////////////////////////////////////////////////////////////
// D2Win Ordinals
////////////////////////////////////////////////////////////////////////////////////////////////

FUNCPTR(D2WIN, DrawText, void __fastcall, (const wchar_t *wStr, int xPos, int yPos, DWORD dwColor, DWORD dwUnk), -10150, -10076)

FUNCPTR(D2WIN, GetTextSize, DWORD __fastcall, (wchar_t *wStr, DWORD* dwWidth, DWORD* dwFileNo), -10177, -10179)
FUNCPTR(D2WIN, SetTextSize, DWORD __fastcall, (DWORD dwSize), -10184, -10047)

FUNCPTR(D2WIN, SetControlText, void* __fastcall, (Control* box, wchar_t* txt), -10042, -10007)
FUNCPTR(D2WIN, GetTextWidthFileNo, DWORD __fastcall, (wchar_t *wStr, DWORD* dwWidth, DWORD* dwFileNo), -10177, -10179)

FUNCPTR(D2WIN, CreateEditBox, Control* __fastcall, (DWORD dwPosX, DWORD dwPosY, DWORD _1, DWORD _2, DWORD _3, DWORD _4, DWORD _5, BOOL(__stdcall *pCallback)(wchar_t* wText), DWORD _6, DWORD _7, ControlPreferences* pPreferences), 0x161B0, 0x11A10)//1.13c
FUNCPTR(D2WIN, DestroyEditBox, VOID __fastcall, (Control* pControl), 0x159E0, 0xF320)//1.13c
FUNCPTR(D2WIN, DestroyControl, VOID __stdcall, (Control* pControl), 0x18490, 0xE5F0)//1.13c
FUNCPTR(D2WIN, SetEditBoxCallback, VOID __fastcall, (Control* pControl, BOOL(__stdcall *FunCallBack)(Control* pControl, DWORD dwInputType, char* pChar)), 0x13970, 0xF1D0)//1.13c
FUNCPTR(D2WIN, SetEditBoxProc, void __fastcall, (Control* box, BOOL(__stdcall *FunCallBack)(Control*, DWORD, DWORD)), 0x13970, 0xF1D0)//Updated 1.13c
FUNCPTR(D2WIN, SelectEditBoxText, void __fastcall, (Control* box), 0x7708, 0xEF80) //Updated 1.13c
FUNCPTR(D2WIN, InitMPQ, DWORD __stdcall, (char *dll, const char *mpqfile, char *mpqname, int v4, int v5), 0x7E60, 0x7E50)

////////////////////////////////////////////////////////////////////////////////////////////////
// D2Win Globals
////////////////////////////////////////////////////////////////////////////////////////////////

VARPTR(D2WIN, FirstControl, Control*, 0x214A0, 0x8DB34)
VARPTR(D2WIN, FocusedControl, Control*, 0x214B0, 0x8DB44) // unused, but we ought to use it

////////////////////////////////////////////////////////////////////////////////////////////////
// D2Game Functions
////////////////////////////////////////////////////////////////////////////////////////////////

//FUNCPTR(D2GAME, Rand, DWORD __fastcall, (DWORD* seed), 0x1160)

FUNCPTR(D2MCPCLIENT, ParseGameListPacket, VOID __fastcall, (BYTE* pPacket), 0x6E30, 0x6640)

#undef FUNCPTR
#undef ASMPTR
#undef VARPTR

#define D2CLIENT_TestPvpFlag(dwId1, dwId2, dwFlag)	(TestPvpFlag_STUB(dwId1, dwId2, dwFlag))
#define D2CLIENT_GetUnitName(x)				(wchar_t*)D2CLIENT_GetUnitName_STUB((DWORD)x)
#define GetUnitFromId(unitid, unittype)		((UnitAny *)D2CLIENT_GetUnitFromId_STUB(unitid, unittype))
#define D2CLIENT_GetLevelName(LevelId) (wchar_t*)D2CLIENT_GetLevelName_STUB(LevelId)
#define D2CLIENT_GetUIState(dwVarNo)					(D2CLIENT_GetUIVar_STUB(dwVarNo))
#define D2CLIENT_ClickParty(RosterUnit, Mode)		(D2CLIENT_ClickParty_ASM(RosterUnit, Mode))
#define D2CLIENT_xShake (*p_D2CLIENT_xShake)	//GameShake
#define D2CLIENT_yShake (*p_D2CLIENT_yShake)	//GameShake
