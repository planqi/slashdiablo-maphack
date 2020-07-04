#pragma once
#include <Windows.h>

DWORD __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny);
DWORD __fastcall D2CLIENT_InitAutomapLayer(DWORD nLayerNo);
DWORD __fastcall TestPvpFlag_STUB(DWORD planum1, DWORD planum2, DWORD flagmask);
DWORD __fastcall D2CLIENT_GetLevelName_STUB(DWORD levelId);
DWORD __fastcall D2CLIENT_GetUIVar_STUB(DWORD varno);
CellFile* __fastcall D2CLIENT_LoadUiImage(CHAR* szPath);
DWORD __fastcall D2CLIENT_ClickParty_ASM(RosterUnit* RosterUnit, DWORD Mode);
void __fastcall D2CLIENT_PlaySound(int SoundNo);
void __fastcall  D2CLIENT_SetSelectedUnit_STUB(UnitAny* pUnit);
