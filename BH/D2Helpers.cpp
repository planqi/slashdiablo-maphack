#include "D2Helpers.h"
#include "D2Ptrs.h"
#include "D2Stubs.h"
#include "Common.h"
#include "Constants.h"

RosterUnit* FindPlayerRoster(DWORD unitId) {
	for (RosterUnit* roster = (*p_D2CLIENT_PlayerUnitList); roster; roster = roster->pNext) {
		if (roster->dwUnitId == unitId)
			return roster;
	}
	return NULL;
}

int GetRelation(UnitAny* unit) {
	UnitAny* player = D2CLIENT_GetPlayerUnit();
	RosterUnit* roster;
	RosterUnit* playerRoster = FindPlayerRoster(player->dwUnitId);

	//Neutral = 2, Partied = 3, You = 1, Hostile = 4
	if (!unit || !player)
		return 2;

	switch(unit->dwType) {
		case UNIT_PLAYER://Player

			// Check if we are the unit.
			if (unit->dwUnitId == player->dwUnitId)
				return 1;//You
			
			// Check if we are in a party with unit.
			roster = FindPlayerRoster(unit->dwUnitId);
			if (playerRoster && roster && playerRoster->wPartyId == roster->wPartyId && roster->wPartyId != INVALID_PARTY_ID)
				return 3;//Partied
			
			// Check if unit is hostile towards us
			if (TestPvpFlag_STUB(unit->dwUnitId, player->dwUnitId, 8))
				return 4;

			return 2;
		break;
		case UNIT_MONSTER://Monster or NPC or Summon
		case UNIT_MISSILE://Missile

			if (unit->dwOwnerId < 0 || unit->dwOwnerType < 0 || unit->dwOwnerType > 5)
				return 4;

			if (unit->dwType == UNIT_MONSTER && unit->dwOwnerType == UNIT_MONSTER)
				return 4;
			
			// Find the owner of the unit.
			UnitAny* owner = D2CLIENT_FindServerSideUnit(unit->dwOwnerId, unit->dwOwnerType);
			if (!owner || owner->dwType != unit->dwOwnerType || owner->dwUnitId != unit->dwOwnerId)
				return 4;

			return GetRelation(owner);
		break;
	}

	return 2;//Neutral
}

bool IsValidMonster(UnitAny *pUnit)
{
	//Have we even been given a unit?
	if (!pUnit)
		return false;

	//Check to see if unit is dead.
	if ((pUnit->dwMode == 0) || (pUnit->dwMode == 12))
		return false;

	if ((pUnit->dwTxtFileNo == 608) && (pUnit->dwMode == 8))
		return false;
	
	if ((pUnit->dwTxtFileNo == 68) && (pUnit->dwMode == 14))// Ignore burrowing maggots
		return false;

	//Hide the water creepers
	if ((pUnit->dwTxtFileNo == 258 || (pUnit->dwTxtFileNo == 261)) && (pUnit->dwMode == 14))
		return false;

	DWORD badMonIds[] = {227, 283, 326, 327, 328, 329, 330, 410, 411, 412, 413, 414, 415, 416, 366, 406,
						 351, 352, 353, 266, 408, 516, 517, 518, 519, 522, 523, 543, 543, 545};

	for (DWORD n = 0; n < 30; n++)
	{
		if (pUnit->dwTxtFileNo == badMonIds[n])
			return false;
	}

	if (D2COMMON_GetUnitStat(pUnit, 172, 0) == 2) 
		return false;

	wchar_t* name = D2CLIENT_GetUnitName(pUnit);
	char* tmp = UnicodeToAnsi(name);

	if ((strcmp(tmp,"an evil force") == 0) || (strcmp(tmp, "dummy") == 0) || (strcmp(tmp, "Maggot") == 0)) {
		delete[] tmp;
		return false;
	}
	delete[] tmp;

	return true;
}


bool IsGameReady() {
	UnitAny* player = D2CLIENT_GetPlayerUnit();

	return player != NULL &&
		player->pPath != NULL &&
		player->pPath->pRoom1 != NULL &&
		player->pPath->pRoom1->pRoom2 != NULL &&
		player->pPath->pRoom1->pRoom2->pLevel != NULL &&
		player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo > 0 &&
		player->pAct != NULL &&
		player->pAct->pRoom1 != NULL &&
		player->pInventory != NULL &&
		player->pPath->xPos > 0 &&
		player->pPath->yPos > 0;
}

void Print(char* format, ...)
{
	va_list vaArgs;
	va_start(vaArgs, format);
	int len = _vscprintf(format, vaArgs) + 1;
	char* str = new char[len];
	vsprintf_s(str, len, format, vaArgs);
	va_end(vaArgs);

	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CODE_PAGE, 0, str, -1, wstr, len);
	if (IsGameReady())
		D2CLIENT_PrintGameString(wstr, 0);
	delete[] wstr;

	delete[] str;
}

void PartyPrint(char* format, ...)
{
	va_list vaArgs;
	va_start(vaArgs, format);
	int len = _vscprintf(format, vaArgs) + 1;
	char* str = new char[len];
	vsprintf_s(str, len, format, vaArgs);
	va_end(vaArgs);

	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CODE_PAGE, 0, str, -1, wstr, len);
	if (IsGameReady())
		D2CLIENT_PrintPartyString(wstr, 0);
	delete[] wstr;

	delete[] str;
}

CellFile *LoadBmpCellFile(BYTE *buf1, int width, int height)
{
	BYTE *buf2 = new BYTE[(width*height * 2) + height], *dest = buf2;

	for (int i = 0; i < height; i++) {
		BYTE *src = buf1 + (i*((width + 3)&-4)), *limit = src + width;
		while (src < limit) {
			BYTE *start = src, *limit2 = min(limit, src + 0x7f), trans = !*src;
			do src++; while ((trans == (BYTE)!*src) && (src < limit2));
			if (!trans || (src < limit)) *dest++ = (trans ? 0x80 : 0) + (src - start);
			if (!trans) while (start < src) *dest++ = *start++;
		}
		*dest++ = 0x80;
	}

	static DWORD dc6head[] = { 6, 1, 0, 0xeeeeeeee, 1, 1, 0x1c, 0, -1, -1, 0, 0, 0, -1, -1 };
	dc6head[8] = width;
	dc6head[9] = height;
	BYTE *ret = new BYTE[dc6head[13] = sizeof(dc6head) + (dc6head[14] = dest - buf2) + 3];
	memset(memcpy2(memcpy2(ret, dc6head, sizeof(dc6head)), buf2, dc6head[14]), 0xee, 3);
	delete buf2;

	return (CellFile *)ret;
}

CellFile *LoadBmpCellFile(char *filename)
{
	BYTE *ret = 0;

	BYTE *buf1 = AllocReadFile(filename);
	BITMAPFILEHEADER *bmphead1 = (BITMAPFILEHEADER *)buf1;
	BITMAPINFOHEADER *bmphead2 = (BITMAPINFOHEADER *)(buf1 + sizeof(BITMAPFILEHEADER));
	if (buf1 && (bmphead1->bfType == 'MB') && (bmphead2->biBitCount == 8) && (bmphead2->biCompression == BI_RGB)) {
		ret = (BYTE *)LoadBmpCellFile(buf1 + bmphead1->bfOffBits, bmphead2->biWidth, bmphead2->biHeight);
	}
	delete buf1;

	return (CellFile *)ret;
}

CellFile *InitCellFile(CellFile *cf)
{
	if (cf) D2CMP_InitCellFile(cf, &cf, "?", 0, -1, "?");
	return cf;
}

void DeleteCellFile(CellFile *cf)
{
	if (cf) {
		D2CMP_DeleteCellFile(cf);
		delete cf;
	}
}

DWORD GetDistanceSquared(DWORD x1, DWORD y1, DWORD x2, DWORD y2) {
	return (DWORD)sqrt((double)(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2))));
}

bool GetSkill(WORD wSkillId)
{
	if (!D2CLIENT_GetPlayerUnit())
		return 0;

	for (Skill* pSkill = D2CLIENT_GetPlayerUnit()->pInfo->pFirstSkill; pSkill; pSkill = pSkill->pNextSkill)
		if (pSkill->pSkillInfo->wSkillId == wSkillId)
			return 1;//D2COMMON_GetSkillLevel(D2CLIENT_GetPlayerUnit(), pSkill, TRUE);

	return 0;
}

bool SetSkill(WORD wSkillId, bool Left)
{
	if (!D2CLIENT_GetPlayerUnit())
		return 0;

	BYTE PutSkill[9] = { 0x3c, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF };
	if (GetSkill(wSkillId))
	{
		*(WORD*)&PutSkill[1] = wSkillId;
		if (Left)
			*(BYTE*)&PutSkill[4] = 0x80;
		D2NET_SendPacket(9, 0, PutSkill);
	}
	else return 0;

	return 1;
}



bool CastOnMap(WORD x, WORD y, bool Left) {
	LPBYTE aPacket = new BYTE[5];	//create packet
	*(BYTE*)&aPacket[0] = Left ? (BYTE)0x08 : (BYTE)0x0F;	//casting with left or right?
	*(WORD*)&aPacket[1] = x;	//x
	*(WORD*)&aPacket[3] = y;	//y
	D2NET_SendPacket(5, 0, aPacket);

	delete[] aPacket;	//clearing up data

	return 1;
}

bool RunTo(WORD x, WORD y) {
	LPBYTE aPacket = new BYTE[5];	//create packet
	*(BYTE*)&aPacket[0] = 0x03;
	*(WORD*)&aPacket[1] = x;	//x
	*(WORD*)&aPacket[3] = y;	//y
	D2NET_SendPacket(5, 0, aPacket);

	delete[] aPacket;	//clearing up data

	return 1;
}

bool Interact(DWORD UnitId, DWORD UnitType) {
	LPBYTE aPacket = new BYTE[9];
	*(BYTE*)&aPacket[0] = (BYTE)0x13;
	*(DWORD*)&aPacket[1] = UnitType;
	*(DWORD*)&aPacket[5] = UnitId;
	D2NET_SendPacket(9, 1, aPacket);

	delete[] aPacket;

	return 1;
}

std::string GetItemCode(int dwTxtFileNo) {
	ItemText* text = D2COMMON_GetItemText(dwTxtFileNo);
	std::string code = text->szCode;
	return code.substr(0, 3);
}

std::string GetItemName(UnitAny* item) {
	wchar_t buffer[256] = L"";
	D2CLIENT_GetItemName(item, buffer, 256);
	return UnicodeToAnsi(buffer);
}

bool IsTown(DWORD levelId) {
	switch (levelId) {
	case 1:
	case 40:
	case 75:
	case 103:
	case 109:
		return true;
	}
	return false;
}


DWORD GetPlayerArea() {
	UnitAny* player = D2CLIENT_GetPlayerUnit();
	if (!IsGameReady())
		return 0;
	return player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo;
}