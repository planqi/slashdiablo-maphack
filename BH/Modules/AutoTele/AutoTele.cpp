#include "AutoTele.h"
#include "../../BH.h"
#include "ATIncludes\CMapIncludes.h"
#include "ATIncludes\Vectors.h"

#define VALIDPTR(x) ( (x) && (!IsBadReadPtr(x,sizeof(x))) )

int waypoints[] = {119,157,156,323,288,402,324,237,238,398,496,511,494};
int CSID = 0;
int CS[] = {392, 394, 396, 255};


using namespace Drawing;

void AutoTele::OnLoad() {
	LoadConfig();

	settingsTab = new UITab("Miscellaneous", BH::settingsUI);

	new Texthook(settingsTab, 60, 12, "Toggles");

	new Checkhook(settingsTab, 40, 27, &Toggles["Draw Destination"].state, "Draw Paths");

	new Checkhook(settingsTab, 40, 57, &Toggles["Quest Drop Warning"].state, "Quest Drop Warning");

	//this doesn't change the path.  I can't figure out how to make it work either.
	//new Checkhook(settingsTab, 40, 42, &Toggles["CP to cave"].state, "CP to cave");

	new Texthook(settingsTab, 250, 12, "Map Colors");

	new Colorhook(settingsTab, 250, 27, &Colors[0], "Path");

	new Colorhook(settingsTab, 250, 42, &Colors[1], "Next");

	new Colorhook(settingsTab, 250, 57, &Colors[2], "Other");

	new Colorhook(settingsTab, 250, 72, &Colors[3], "WP");

	new Colorhook(settingsTab, 250, 87, &Colors[4], "Prev");

	new Colorhook(settingsTab, 250, 102, &Colors[5], "Other Extra");
}

void AutoTele::LoadConfig() {
	BH::config->ReadToggle("CP to cave", "None", false, Toggles["CP to cave"]);
	BH::config->ReadToggle("Display Messages", "None", true, Toggles["Display Messages"]);
	BH::config->ReadToggle("Draw Path", "None", true, Toggles["Draw Path"]);
	BH::config->ReadToggle("Draw Destination", "None", true, Toggles["Draw Destination"]);
	BH::config->ReadToggle("Fast Teleport", "None", true, Toggles["Fast Teleport"]);
	BH::config->ReadToggle("Quest Drop Warning", "None", false, Toggles["Quest Drop Warning"]);

	BH::config->ReadKey("Next Tele", "VK_NUMPAD0", NextKey);
	BH::config->ReadKey("Other Tele", "VK_NUMPAD1", OtherKey);
	BH::config->ReadKey("Waypoint Tele", "VK_NUMPAD2", WPKey);
	BH::config->ReadKey("Prev Tele", "VK_NUMPAD3", PrevKey);
	BH::config->ReadInt("Path Color", Colors[0]);
	BH::config->ReadInt("Next Color", Colors[1]);
	BH::config->ReadInt("Other Color", Colors[2]);
	BH::config->ReadInt("WP Color", Colors[3]);
	BH::config->ReadInt("Prev Color", Colors[4]);
}

void AutoTele::OnAutomapDraw() {
	//if (WaitingForMapData()) {
	//	return;
	//}

	POINT MyPos, Pos;
	Drawing::Hook::ScreenToAutomap(&MyPos, D2CLIENT_GetUnitX(D2CLIENT_GetPlayerUnit()), D2CLIENT_GetUnitY(D2CLIENT_GetPlayerUnit()));
	//Drawing::Texthook::Draw(10, 10, 0, 13, Drawing::Orange, "%d, %d", D2CLIENT_GetUnitX(D2CLIENT_GetPlayerUnit()), D2CLIENT_GetUnitY(D2CLIENT_GetPlayerUnit()));
	if(TPath.GetSize() && Toggles["Draw Path"].state) {
		CArrayEx <POINT, POINT> DrawPath;
		DrawPath.Add(MyPos, 1);
		for(int i = 0;i<TPath.GetSize();i++) {
			Drawing::Hook::ScreenToAutomap(&Pos, TPath.ElementAt(i).x, TPath.ElementAt(i).y);
			DrawPath.Add(Pos, 1);
		}
		for(int i = 0;i<(DrawPath.GetSize()-1);i++) {
			Drawing::Linehook::Draw(DrawPath.ElementAt(i).x,DrawPath.ElementAt(i).y,DrawPath.ElementAt(i+1).x, DrawPath.ElementAt(i+1).y, Colors[0]);
			Drawing::Crosshook::Draw(DrawPath.ElementAt(i+1).x, DrawPath.ElementAt(i+1).y, Colors[0]);
		}
	}

	if(Toggles["Draw Destination"].state) {
		for(int i = 0;i<5;i++) {
			if(Vectors[i].x && Vectors[i].y) {
				Drawing::Hook::ScreenToAutomap(&Pos, Vectors[i].x, Vectors[i].y);
				Drawing::Linehook::Draw(MyPos.x, MyPos.y, Pos.x, Pos.y, Colors[i+1]);
				//PrintText(1, "player x %d player y %d", MyPos.x, MyPos.y);
				Drawing::Crosshook::Draw(Pos.x, Pos.y, Colors[i+1]);
			}
		}
	}
}

void AutoTele::OnLoop() {
	//if (WaitingForMapData()) {
	//	return;
	//}

	DWORD playerArea = GetPlayerArea();
	if(playerArea && LastArea != playerArea && D2CLIENT_GetPlayerUnit()) {
		LastArea = playerArea;
		if(LastArea != MAP_A4_THE_CHAOS_SANCTUARY)
			CSID = 0;
		GetVectors();
		//LoadHandle = CreateThread(0, 0, LoadNewArea, this, 0, 0);
		//return;
	}

	if(LastArea == MAP_A4_THE_CHAOS_SANCTUARY)
		if(vVector[LastArea*4].Id2 != (1337+CSID)) {
			vVector[LastArea*4].Id2 = 1337+CSID;
			GetVectors();
		}

	if(TPath.GetSize()) {
		End = TPath.GetLast();

		if(SetTele) {
			if(!SetSkill(0x36, 0)) {	//0x36 is teleport
				TPath.RemoveAll();
				PrintText(1, "ÿc4AutoTele:ÿc1 Failed to set teleport!");
			}
			_timer = GetTickCount();
			SetTele = 0;
		}

		if(D2CLIENT_GetPlayerUnit()->pInfo->pRightSkill->pSkillInfo->wSkillId == 0x36) {
			TeleActive = 1;
		} else {
			if(TeleActive) {
				TeleActive = 0;
				TPath.RemoveAll();
				PrintText(1, "ÿc4AutoTele:ÿc1 Aborting teleport, deselected teleport");
				return;
			}
			if((GetTickCount() - _timer) > 1000) {
				TPath.RemoveAll();
				PrintText(1, "ÿc4AutoTele:ÿc1 Failed to set teleport skill. Ping: %d", *p_D2CLIENT_Ping);
				return;
			}
			return;
		}

		if(CastTele) {
			CastTele = 0;
			_timer2 = GetTickCount();
			if(!CastOnMap(static_cast<WORD>(TPath.ElementAt(0).x),static_cast<WORD>(TPath.ElementAt(0).y), false)) {
				TPath.RemoveAll();
				return;
			}
		}

		if((GetTickCount() - _timer2) > 500) {
			if(Try >= 5) {
				PrintText(1, "ÿc4AutoTele:ÿc1 Failed to teleport after 5 tries");
				TPath.RemoveAll();
				Try = 0;
				DoInteract = 0;
				return;
			} else {
				Try++;
				CastTele = 1;
				return;
			}
		}

		if(GetDistanceSquared(D2CLIENT_GetUnitX(D2CLIENT_GetPlayerUnit()), D2CLIENT_GetUnitY(D2CLIENT_GetPlayerUnit()), TPath.ElementAt(0).x, TPath.ElementAt(0).y) <= 5) {
			TPath.RemoveAt(0, 1);
			CastTele = 1;
		}
			
		if(DoInteract) {
			if(GetDistanceSquared(D2CLIENT_GetUnitX(D2CLIENT_GetPlayerUnit()), D2CLIENT_GetUnitY(D2CLIENT_GetPlayerUnit()), End.x, End.y) <= 5) {
				_InteractTimer = GetTickCount();
				InteractId = GetUnitByXY(End.x, End.y, InteractRoom);
				SetInteract = 1;
				TPath.RemoveAll();
				return;
			}
		}
	} else {
		End.x = 0;
		End.y = 0;
		SetTele = 1;
		CastTele = 1;
		TeleActive = 0;
		Try = 0;
	}
	if(DoInteract && SetInteract && _InteractTimer && (GetTickCount() - _InteractTimer > 150)) {
		Interact(InteractId, InteractType);
		SetInteract = 0;
		_InteractTimer = 0;
	}
	return;
}

void AutoTele::OnKey(bool up, BYTE key, LPARAM lParam, bool* block) {
	if (key == NextKey) 
	{
	} 
	else if (key == OtherKey) 
	{
	} 
	else if (key == WPKey) 
	{
	}
	else if (key == PrevKey) 
	{
	}
}

void AutoTele::OnGamePacketRecv(BYTE* packet, bool* block) {

	if(packet[0] == 0x15) { 
		if(*(DWORD*)&packet[2] == D2CLIENT_GetPlayerUnit()->dwUnitId) {
			packet[10] = 0;  

			//if(Toggles["Fast Teleport"].state) {
			//	UnitAny* Me = D2CLIENT_GetPlayerUnit();

			//	if(Me->dwMode == PLAYER_MODE_DEATH || Me->dwMode == PLAYER_MODE_STAND_OUTTOWN ||
			//		Me->dwMode == PLAYER_MODE_WALK_OUTTOWN || Me->dwMode == PLAYER_MODE_RUN || 
			//		Me->dwMode == PLAYER_MODE_STAND_INTOWN || Me->dwMode == PLAYER_MODE_WALK_INTOWN ||
			//		Me->dwMode == PLAYER_MODE_DEAD || Me->dwMode == PLAYER_MODE_SEQUENCE ||
			//		Me->dwMode == PLAYER_MODE_BEING_KNOCKBACK)
			//		return;

			//	Me->dwFrameRemain = 0;
			//}
		}
	}
	return;
}

void AutoTele::GetVectors() {
	DWORD MyArea = GetPlayerArea();
	DWORD Areas[2] = {MyArea, 0};
	DWORD AreaCount = 1;
	BOOL buildCollisionMap = false;

	// Usually all markers/paths are in the current area (exceptions include River of Flame,
	// which has its destination marker in the Chaos Sanctuary). First scan the vectors to see
	// which areas we need to load.
	for (int i = 0; i < 4; i++) {
		Vector V = vVector[MyArea*4+i];
		if (V.dwType == EXIT || (MyArea == MAP_A2_CANYON_OF_THE_MAGI && V.dwType == 0 && V.Id == 0)) {
			if (V.Area) {
				Areas[1] = V.Area;
				AreaCount = 2;
			}
			buildCollisionMap = true;
		}
	}

	CCollisionMap g_collisionMap;
	if (buildCollisionMap) {
		buildCollisionMap = g_collisionMap.CreateMap(Areas, AreaCount);  //create a cmap for the current area
	}

  // hack to reset 'other extra'
  Vectors[4].x = 0;
  Vectors[4].y = 0;

	for(int i = 0;i<4;i++) {
		Vectors[i].x = 0;
		Vectors[i].y = 0;
		Vector V = vVector[MyArea*4+i];

		if(MyArea == MAP_A2_CANYON_OF_THE_MAGI) {
			if(V.dwType == 0 && V.Id == 0) {
				V.dwType = EXIT;
				V.Id = GetLevel(D2CLIENT_GetPlayerUnit()->pAct, MyArea)->pMisc->dwStaffTombLevel;
			}
		}

		if(V.dwType == UNIT_PLAYER)
			continue;

		if(V.dwType == XY) {
			Vectors[i].x = V.Id;
			Vectors[i].y = V.Id2;
		}

		if(V.dwType == UNIT_TILE || V.dwType == UNIT_OBJECT || V.dwType == UNIT_MONSTER) {
			if(V.Area)
				Vectors[i] = FindPresetLocation(V.dwType, V.Id, V.Area);
			else
				Vectors[i] = FindPresetLocation(V.dwType, V.Id, MyArea);
		}

		if(V.dwType == EXIT || V.dwType == EXIT_MULTI) {
			if (!buildCollisionMap) {
				continue;
			}

			LPLevelExit ExitArray[0x40];//declare an array of exits to store the exits in later

			int ExitCount = g_collisionMap.GetLevelExits(ExitArray); //getlevelexits returns the exitcount

			if(!ExitCount)//if there are 0 tele positions we can stop here :p
				continue;

      bool bIsMulti = V.dwType == EXIT_MULTI;

			for(int j = 0;j<ExitCount;j++) {//loop over evey exit to see if it matches our target
				if(ExitArray[j]->dwTargetLevel == V.Id) {
					Vectors[i] = ExitArray[j]->ptPos;
				} else if(bIsMulti && ExitArray[j]->dwTargetLevel == V.Id2) {
					Vectors[4] = ExitArray[j]->ptPos;
 				}

			  delete ExitArray[j];
			}
		}
	}
}

Level* AutoTele::GetLevel(Act* pAct, int level) {
	//Insure that the shit we are getting is good.
	if (level < 0 || !pAct)
		return NULL;

	//Loop all the levels in this act

	for(Level* pLevel = pAct->pMisc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel) {
		//Check if we have reached a bad level.
		if (!pLevel)
			break;

		//If we have found the level, return it!
		if (pLevel->dwLevelNo == level && pLevel->dwPosX > 0)
			return pLevel;
	}
	//Default old-way of finding level.
	return D2COMMON_GetLevel(pAct->pMisc, level);
}

void AutoTele::ManageTele(Vector T) {

	DWORD Areas[2];
	DWORD AreaCount = 1;

	Areas[0] = GetPlayerArea();
	if(Areas[0] == MAP_A2_CANYON_OF_THE_MAGI) {
		if(T.dwType == 0 && T.Id == 0) {
			T.dwType = EXIT;
			T.Id = GetLevel(D2CLIENT_GetPlayerUnit()->pAct, Areas[0])->pMisc->dwStaffTombLevel;
		}
	} else if(Areas[0] == MAP_A1_COLD_PLAINS) {
		if(Toggles["CP to cave"].state)
			if(T.Id == MAP_A1_BURIAL_GROUNDS)
				T.Id = MAP_A1_CAVE_LEVEL_1;
	} else if(Areas[0] == MAP_A4_THE_CHAOS_SANCTUARY && T.Id2 >= 1337) {
		CSID == 3 ? CSID = 0 : CSID++;
		vVector[Areas[0]*4].Id = CS[CSID];
	}

	if(!T.Id) {
		PrintText(1, "ÿc4AutoTele:ÿc1 Invalid destination");
		return;
	}

	if(T.Area) {
		Areas[1] = T.Area;
		AreaCount = 2;
	}

	DoInteract = 0;

	if(T.dwType == EXIT) {
		CCollisionMap g_collisionMap;	//init the collisionmap

		if(!g_collisionMap.CreateMap(Areas, AreaCount))		//create a cmap for the current area
			return;

		LPLevelExit ExitArray[0x40];	//declare an array of exits to store the exits in later

		int ExitCount = g_collisionMap.GetLevelExits(ExitArray);	//getlevelexits returns the exitcount

		if(!ExitCount)		//if there are 0 tele positions we can stop here :p
			return;

		for(int i = 0;i<ExitCount;i++) {	//loop over evey exit to see if it matches our target
			if(ExitArray[i]->dwTargetLevel == T.Id) {
				LevelText * lvltext =  D2COMMON_GetLevelText(T.Id);

				DoInteract = 0;
				if(ExitArray[i]->dwType == EXIT_TILE) {
					DoInteract = 1;
					InteractType = UNIT_TILE;
					InteractRoom = ExitArray[i]->pRoom;
				} else DoInteract = 0;

				int nodes = MakePath(ExitArray[i]->ptPos.x, ExitArray[i]->ptPos.y, Areas, AreaCount, ExitArray[i]->dwType == EXIT_LEVEL ? 1: 0);
				PrintText(1, "ÿc4AutoTele:ÿc1 Going to %s, %d nodes.", lvltext->szName, nodes);
				break;
			}
		}
		return;			
	}

	if(T.dwType == XY) {
		DoInteract = 0;
		if(!T.Id || !T.Id2) {
			PrintText(1, "ÿc4AutoTele:ÿc1 No X/Y value found");
			return;
		}
		int nodes = MakePath(T.Id, T.Id2, Areas, AreaCount, 0);
		PrintText(1, "ÿc4AutoTele:ÿc1 Going to X: %d, Y: %d, %d nodes", T.Id, T.Id2, nodes);
		return;
	}

	POINT PresetUnit;
	if(AreaCount == 2)
		PresetUnit = FindPresetLocation(T.dwType, T.Id, Areas[1]);
	else
		PresetUnit = FindPresetLocation(T.dwType, T.Id, Areas[0]);

	if(PresetUnit.x != 0 && PresetUnit.y != 0) {
		if(T.dwType == UNIT_TILE || (T.dwType == UNIT_OBJECT && T.Id == 298))
			DoInteract = 1;
		if(T.dwType == UNIT_OBJECT)
			for(int i = 0;i <= 13;i++)
				if(waypoints[i] == T.Id)
					DoInteract = 1;

		int nodes;
		if(nodes = MakePath(PresetUnit.x,PresetUnit.y, Areas, AreaCount, 0)) {
			if(T.dwType == UNIT_OBJECT) {
				ObjectTxt * ObjTxt = D2COMMON_GetObjectTxt(T.Id);
				PrintText(1, "ÿc4AutoTele:ÿc1 Going to %s, %d nodes", ObjTxt->szName, nodes);
			}
			InteractType = T.dwType;
		}
		else return;
	} else {
		PrintText(1, "ÿc4AutoTele:ÿc1 Can't find object");
		return;
	}
}

DWORD AutoTele::GetPlayerArea() {
	if(VALIDPTR(D2CLIENT_GetPlayerUnit())) {
		if(VALIDPTR(D2CLIENT_GetPlayerUnit()->pPath))
			if(VALIDPTR(D2CLIENT_GetPlayerUnit()->pPath->pRoom1))
				if(VALIDPTR(D2CLIENT_GetPlayerUnit()->pPath->pRoom1->pRoom2))
					if(VALIDPTR(D2CLIENT_GetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel))
						return D2CLIENT_GetPlayerUnit()->pPath->pRoom1->pRoom2->pLevel->dwLevelNo;
	}
	return 0;
}

void AutoTele::PrintText(DWORD Color, char *szText, ...) {
	if(Toggles["Display Messages"].state) {
		char szBuffer[152] = {0};
		va_list Args;
		va_start(Args, szText);
		vsprintf_s(szBuffer,152, szText, Args);
		va_end(Args); 
		wchar_t Buffer[0x130];
		MultiByteToWideChar(CODE_PAGE, 1, szBuffer, 152, Buffer, 304);
		D2CLIENT_PrintGameString(Buffer, Color);	
	}
}

int AutoTele::MakePath(int x, int y, DWORD Areas[], DWORD count, bool MoveThrough) {
	CCollisionMap g_collisionMap; //init the collisionmap
	DWORD dwCount;
	POINT aPath[255];

	if(!g_collisionMap.CreateMap(Areas, count))
		return false;

	POINT ptStart = {D2CLIENT_GetPlayerUnit()->pPath->xPos, D2CLIENT_GetPlayerUnit()->pPath->yPos};
	POINT ptEnd = {x, y};

	if(!g_collisionMap.IsValidAbsLocation(ptStart.x, ptStart.y))
		return false;

	if(!g_collisionMap.IsValidAbsLocation(ptEnd.x, ptEnd.y))
		return false;

	g_collisionMap.AbsToRelative(ptStart);
	g_collisionMap.AbsToRelative(ptEnd);

	WordMatrix matrix;

	if(!g_collisionMap.CopyMapData(matrix))
		return false;

	CTeleportPath tf(matrix.GetData(), matrix.GetCX(), matrix.GetCY());
	dwCount = tf.FindTeleportPath(ptStart, ptEnd, aPath, 255);

	if(dwCount == 0)
		return false;

	for(DWORD i = 0;i < dwCount;i++) {
		g_collisionMap.RelativeToAbs(aPath[i]);
	}

	if(MoveThrough) {
		if(aPath[dwCount-1].x > aPath[dwCount-2].x)
			aPath[dwCount].x = aPath[dwCount-1].x + 2;
		else
			aPath[dwCount].x = aPath[dwCount-1].x - 2;
		if(aPath[dwCount-1].y > aPath[dwCount-2].y)
			aPath[dwCount].y = aPath[dwCount-1].y + 2;
		else
			aPath[dwCount].y = aPath[dwCount-1].y - 2;

		dwCount++;

		if(GetDistanceSquared(aPath[dwCount-1].x, aPath[dwCount-1].y, aPath[dwCount-3].x, aPath[dwCount-3].y) <= 30) {
			aPath[dwCount-2] = aPath[dwCount-1];
			aPath[dwCount-1].x = 0;
			aPath[dwCount-1].y = 0;
			dwCount--;
		}
	}


	TPath.RemoveAll();
	for(DWORD i = 0;i<dwCount;i++)
		TPath.Add(aPath[i], 1);

	return dwCount;
}

POINT AutoTele::FindPresetLocation(DWORD dwType, DWORD dwTxtFileNo, DWORD Area)
{
	UnitAny* player = D2CLIENT_GetPlayerUnit();
	Level* pLevel = GetLevel(player->pAct, Area);

	POINT loc;
	loc.x = 0;
	loc.y = 0;
	DoInteract = 0;

	if(!VALIDPTR(pLevel))
		return loc;

	bool bAddedRoom = false;
	bool stoploop = false;
	for(Room2 *pRoom = pLevel->pRoom2First; pRoom; pRoom = pRoom->pRoom2Next)
	{
		bAddedRoom = false;
		if(!VALIDPTR(pRoom->pPreset) && !VALIDPTR(pRoom->pRoomTiles) && !VALIDPTR(pRoom->pRoom1))
		{
			D2COMMON_AddRoomData(player->pAct, pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, player->pPath->pRoom1);
			bAddedRoom = true;
		}

		for(PresetUnit* pUnit = pRoom->pPreset; pUnit; pUnit = pUnit->pPresetNext)
		{
			//PrintText(4, "X: %d, Y: %d, TxtFileNo: %d, Type: %d", pUnit->dwPosX, pUnit->dwPosY, pUnit->dwTxtFileNo, pUnit->dwType);
			if(((dwType == NULL) || (dwType == pUnit->dwType)) && (dwTxtFileNo == pUnit->dwTxtFileNo))
			{
				if(dwType == UNIT_TILE || (dwType == UNIT_OBJECT && dwTxtFileNo == 298))
				{
					InteractRoom = pRoom;
					InteractType = dwType;
					//DoInteract = 1;
				}

				if(dwType == UNIT_OBJECT)
				{
					for(int i = 0;i <= 13;i++)
					{
						if(waypoints[i] == dwTxtFileNo)
						{
							InteractRoom = pRoom;
							InteractType = dwType;
							//DoInteract = 1;
							stoploop = 1;//stop looping over the rooms
							break;
						}
					}
				}

				loc.x = (pUnit->dwPosX) + (pRoom->dwPosX * 5);
				loc.y = (pUnit->dwPosY) + (pRoom->dwPosY * 5);

				stoploop = 1;//stop looping over the rooms
				break;
			}
		}

		if(bAddedRoom)
		{
			D2COMMON_RemoveRoomData(D2CLIENT_GetPlayerUnit()->pAct, pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, D2CLIENT_GetPlayerUnit()->pPath->pRoom1);
			bAddedRoom = false;
		}
		if(stoploop)
		{
			stoploop = 0;
			break;
		}
	}

	return loc;
}

DWORD AutoTele::GetDistanceSquared(DWORD x1, DWORD y1, DWORD x2, DWORD y2) {
	return (DWORD)sqrt((double)( ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)))); }

bool AutoTele::SetSkill(WORD wSkillId, bool Left)
{
	if(!D2CLIENT_GetPlayerUnit())
		return 0;

	BYTE PutSkill[9] = {0x3c,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF};
	if(GetSkill(wSkillId))
	{
		*(WORD*)&PutSkill[1] = wSkillId;
		if(Left)
			*(BYTE*)&PutSkill[4] = 0x80;
		D2NET_SendPacket(9,0,PutSkill);
	}
	else return 0;

	return 1;
}

bool AutoTele::GetSkill(WORD wSkillId)
{
	if(!D2CLIENT_GetPlayerUnit()) 
		return 0;

	for(Skill* pSkill = D2CLIENT_GetPlayerUnit()->pInfo->pFirstSkill; pSkill; pSkill = pSkill->pNextSkill)
		if(pSkill->pSkillInfo->wSkillId == wSkillId)
			return 1;//D2COMMON_GetSkillLevel(D2CLIENT_GetPlayerUnit(), pSkill, TRUE);

	return 0;
}

bool AutoTele::CastOnMap(WORD x, WORD y, bool Left) {
	LPBYTE aPacket = new BYTE[5];	//create packet
	*(BYTE*)&aPacket[0] = Left ? (BYTE)0x08 : (BYTE)0x0F;	//casting with left or right?
	*(WORD*)&aPacket[1] = x;	//x
	*(WORD*)&aPacket[3] = y;	//y
	D2NET_SendPacket(5, 0, aPacket);

	delete [] aPacket;	//clearing up data

	return 1;
}

bool AutoTele::Interact(DWORD UnitId, DWORD UnitType) {
	LPBYTE aPacket = new BYTE[9];
	*(BYTE*)&aPacket[0] = (BYTE)0x13;
	*(DWORD*)&aPacket[1] = UnitType;
	*(DWORD*)&aPacket[5] = UnitId;
	D2NET_SendPacket(9, 1, aPacket);

	delete [] aPacket;

	return 1;
}

DWORD AutoTele::GetUnitByXY(DWORD X, DWORD Y, Room2* pRoom) {
	UnitAny* pUnit;

	if(!VALIDPTR(pRoom))
		return 0;

	if(!VALIDPTR(pRoom->pRoom1))
		return 0;

	if(!VALIDPTR(pRoom->pRoom1->pUnitFirst))
		return 0;

	pUnit = pRoom->pRoom1->pUnitFirst;

	if(!VALIDPTR(pUnit))
		return 0;

	if(!VALIDPTR(pUnit->pObjectPath))
		return 0;

	while(pUnit) {
		if(pUnit->dwType != UNIT_PLAYER)
			if(X == pUnit->pObjectPath->dwPosX)
				if(Y == pUnit->pObjectPath->dwPosY)
					return pUnit->dwUnitId;
		pUnit=pUnit->pListNext;
	}
	return 0;
}

bool AutoTele::WaitingForMapData() {
	if (LoadHandle) {
		DWORD wait = WaitForSingleObject(LoadHandle, 0);
		if (wait == WAIT_TIMEOUT) {
			return true;
		}
		CloseHandle(LoadHandle);
		LoadHandle = NULL;
	}
	return false;
}

DWORD WINAPI LoadNewArea(VOID* lpvoid) {
	AutoTele *at = (AutoTele*)lpvoid;
	at->GetVectors();
	return 0;
}
