/**
*
* D2DataTables.h
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

#include <Windows.h>

#pragma once
#ifndef D2DATATABLES_H
#define D2DATATABLES_H

struct ArenaTxt
{
	DWORD dwSuicide;
	DWORD dwPlayerKill;
	DWORD dwPKPercent;
	DWORD dwMonsterKill;
	DWORD dwPlayerDeath;
	DWORD dwPlayerDeathPercent;
	DWORD dwMonsterDeath;
};

#pragma pack(push,1)

struct LvlMazeTxt
{
	DWORD dwLevelId;                //0x00
	DWORD dwRooms[3];               //0x04
	DWORD dwSizeX;                  //0x10
	DWORD dwSizeY;                  //0x14
	DWORD dwMerge;                  //0x18
};

struct LvlWarpTxt
{
	DWORD dwLevelId;
	DWORD dwSelectX;
	DWORD dwSelectY;
	DWORD dwSelectDX;
	DWORD dwSelectDY;
	DWORD dwExitWalkX;
	DWORD dwExitWalkY;
	DWORD dwOffsetX;
	DWORD dwOffsetY;
	DWORD dwLitVersion;
	DWORD dwTiles;
	DWORD dwDirection;
};


struct PropertiesTxt
{
	WORD wProp;                   //0x00
	BYTE nSet[8];                 //0x02
	WORD wVal[7];                 //0x0A
	BYTE nFunc[8];                //0x18
	WORD wStat[7];                //0x20
};

struct ItemsTxtStat
{
	DWORD dwProp;                 //0x00
	DWORD dwPar;                  //0x04
	int dwMin;					  //0x08
	int dwMax;			          //0x0C
};

/*
	 Valid for Automagic.txt, MagicSuffix.txt, MagicPrefix.txt
	 */
struct AutoMagicTxt
{
	char szName[31];                //0x00
	BYTE _1;						//0x1F
	WORD wIndex;	                //0x20
	WORD wVersion;                  //0x22
	ItemsTxtStat hMods[3];          //0x24
	WORD wSpawnable;                //0x54
	WORD wTransformColor;           //0x56
	DWORD dwLevel;                  //0x58
	DWORD dwGroup;                  //0x5C
	DWORD dwMaxLevel;               //0x60
	BYTE nRare;                     //0x64
	BYTE nLevelReq;                 //0x65
	BYTE nClassSpecific;            //0x66
	BYTE nClass;                    //0x67
	WORD wClassLevelReq;            //0x68
	WORD wIType[7];                 //0x6A
	WORD wEType[5];                 //0x78
	BYTE bFrequency;                //0x82
	BYTE _2;						//0x83
	DWORD dwDivide;                 //0x84
	DWORD dwMultiply;               //0x88
	DWORD dwAdd;                    //0x8C
};


struct UniqueItemsTxt
{
	WORD _1;	                  //0x00
	char szName[34];              //0x02
	DWORD dwVersion;              //0x24
	union
	{
		DWORD dwCode;
		char szCode[4];   
	};							  //0x28
	DWORD dwUniqueItemFlags;      //0x2C
	DWORD dwRarity;               //0x30
	WORD wLvl;                    //0x34
	WORD wLvlReq;                 //0x36
	BYTE nChrTransform;           //0x38
	BYTE nInvTransform;           //0x39
	char szFlippyFile[32];        //0x3A
	char szInvFile[34];           //0x5A
	DWORD dwCostMult;             //0x7C
	DWORD dwCostAdd;              //0x80
	WORD wDropSound;              //0x84
	WORD wUseSound;               //0x86
	DWORD dwDropSfxFrame;         //0x88   
	ItemsTxtStat hStats[12];      //0x90
};

struct SetItemsTxt
{
	WORD wSetItemId;               //0x00
	char szName[32];               //0x02
	WORD _1;	                   //0x22
	DWORD dwTblIndex;              //0x24
	union
	{
		DWORD dwCode;
		char szCode[4];
	};							   //0x28
	DWORD _2;	                   //0x2C
	WORD wLvl;                     //0x30
	WORD wLvlReq;                  //0x32
	DWORD dwRarity;                //0x34
	DWORD dwCostMult;              //0x38
	DWORD dwCostAdd;               //0x3C
	BYTE nChrTransform;            //0x40
	BYTE nInvTransform;            //0x41
	char szFlippyFile[32];         //0x42
	char szInvFile[32];            //0x62
	WORD wDropSound;               //0x82
	WORD wUseSound;                //0x84
	BYTE nDropSfxFrame;            //0x86
	BYTE nAddFunc;                 //0x87
	ItemsTxtStat hStats[9];        //0x88
	ItemsTxtStat hGreenStats[10];   //0x118
};

struct RunesTxt
{
	char szName[64];					 //0x00
	char szRuneName[64];                 //0x40
	BYTE bComplete;                      //0x80
	BYTE bServer;                        //0x81
	DWORD dwRwId;						 //0x82
	WORD dwIType[6];                     //0x86
	WORD dwEType[3];                     //0x92
	DWORD dwRune[6];                     //0x98
	ItemsTxtStat hStats[7];              //0xB0
};

struct PetTypesTxt
{
	WORD pet_type; // 0x0
	struct {
		BYTE warp:1; // 0x4
		BYTE range:1; // 0x4
		BYTE partysend:1; // 0x4
		BYTE unsummon:1; // 0x4
		BYTE automap:1; // 0x4
		BYTE drawhp:1; // 0x4
	} dwFlags;
	BYTE _pad[5];
	WORD group; // 0x8
	WORD basemax; // 0xa
	WORD name; // 0xc
	BYTE icontype; // 0xe
	char baseicon[32]; // 0xf
	char micon1[32]; // 0x2f
	char micon2[32]; // 0x4f
	char micon3[32]; // 0x6f
	char micon4[32]; // 0x8f
	BYTE _1[3];
	WORD mclass1; // 0xb2
	WORD mclass2; // 0xb4
	WORD mclass3; // 0xb6
	WORD mclass4; // 0xb8
};

/*
	 Time-saving credit goes to Mnw1995
http://d2mods.info/forum/viewtopic.php?f=8&t=61189
Isn't simpler now?
*/

struct InventoryLayout {
	BYTE SlotWidth;
	BYTE SlotHeight;
	BYTE unk1;
	BYTE unk2;
	DWORD Left;
	DWORD Right;
	DWORD Top;
	DWORD Bottom;
	BYTE SlotPixelWidth;
	BYTE SlotPixelHeight;
};
/*
	 struct InventoryLayout //sizeof 0x14
	 {
	 DWORD dwLeft;		//0x00
	 DWORD dwRight;		//0x04
	 DWORD dwTop;		//0x08
	 DWORD dwBottom;		//0x0C
	 union 
	 {
	 struct 
	 {
	 BYTE nGridX;		//0x10
	 BYTE nGridY;		//0x11
	 };
	 struct
	 {
	 BYTE nWidth;	//0x10
	 BYTE nHeight;	//0x11
	 };
	 };
	 WORD _align;		//0x12
	 };

	 struct InventoryTxt //sizeof 0xF0
	 {
	 InventoryLayout Inventory;		//0x00
	 InventoryLayout Grid;			//0x14
	 union
	 {
	 struct 
	 {
	 InventoryLayout RightArm;		//0x28
	 InventoryLayout Torso;			//0x3C
	 InventoryLayout LeftArm;		//0x50
	 InventoryLayout Head;			//0x64
	 InventoryLayout Neck;			//0x78
	 InventoryLayout RightHand;		//0x8C
	 InventoryLayout LeftHand;		//0xA0
	 InventoryLayout Belt;			//0xB4
	 InventoryLayout Feet;			//0xC8
	 InventoryLayout Gloves;			//0xDC
	 };
	 InventoryLayout hItem[9];
	 };
	 };*/

struct BeltBox
{
	DWORD dwBoxLeft;			//0x00
	DWORD dwBoxRight;			//0x04
	DWORD dwBoxTop;				//0x08
	DWORD dwBoxBottom;			//0x0C
};

struct BeltsTxt //sizeof 0x108
{
	DWORD _unusedName;			    //0x00 placeholder for name field
	DWORD dwNumBoxes;               //0x04
	BeltBox hBox[16];				//0x08
};

struct AutomapTxt
{
	char szLevelName[16];            //0x00
	char szTileName[8];               //0x10
	BYTE nStyle;                  //0x18
	BYTE nStartSequence;            //0x19
	WORD wEndSequence;               //0x1A
	DWORD dwCel1;                  //0x1C
	DWORD dwCel2;                  //0x20
	DWORD dwCel3;                  //0x24
	DWORD dwCel4;                  //0x28
};

struct BodyLocsTxt
{
	union
	{
		DWORD dwCode;               //0x00
		char szCode[4];               //0x00
	};
};

struct StatesTxt
{
	WORD wState;                  //0x00
	WORD wOverlay[4];               //0x02
	WORD wCastOverlay;               //0x0A
	WORD wRemoveOverlay;            //0x0C
	WORD wPrgOverlay;               //0x0E
	struct {
		BYTE	bNosend : 1;	//1
		BYTE	bAura : 1;	//2
		BYTE	bHide : 1;	//3
		BYTE	bTransform : 1;	//4
		BYTE	bPgsv : 1;	//5
		BYTE	bActive : 1;	//6
		BYTE	bRemhit : 1;	//7
		BYTE	bDamblue : 1;	//8
		BYTE	bDamred : 1;	//9
		BYTE	bAttblue : 1;	//10
		BYTE	bAttred : 1;	//11
		BYTE	bCurse : 1;	//12
		BYTE	bCurable : 1;	//13
		BYTE	bPlrstaydeath : 1;	//14
		BYTE	bMonstaydeath : 1;	//15
		BYTE	bBossstaydeath : 1;	//16
		BYTE	bDisguise : 1;	//17
		BYTE	bRestrict : 1;	//18
		BYTE	bBlue : 1;	//19
		BYTE	bArmblue : 1;	//20
		BYTE	bRfblue : 1;	//21
		BYTE	bRcblue : 1;	//22
		BYTE	bRlblue : 1;	//23
		BYTE	bRpblue : 1;	//24
		BYTE	bStambarblue : 1;	//25
		BYTE	bArmred : 1;	//26
		BYTE	bRfred : 1;	//27
		BYTE	bRcred : 1;	//28
		BYTE	bRlred : 1;	//29
		BYTE	bRpred : 1;	//30
		BYTE	bExp : 1;	//31
		BYTE	bShatter : 1;	//32
	} bStateFlags; //0x10
	struct {
		BYTE	bLife : 1;	//1
		BYTE	bUdead : 1;	//2
		BYTE	bGreen : 1;	//3
		BYTE	bNooverlays : 1;	//4
		BYTE	bNoclear : 1;	//5
		BYTE	bBossinv : 1;	//6
		BYTE	bMeleeonly : 1;	//7
		BYTE	bNotondead : 1;	//8
	} bStateFlagsEx; //0x14
	BYTE _1[3];	//0x15
	WORD wStat;                     //0x18
	WORD wSetFunc;                  //0x1A
	WORD wRemFunc;                  //0x1C
	WORD wGroup;                  //0x1E
	BYTE nColorPri;                  //0x20
	BYTE nColorShift;               //0x21
	BYTE nLightRGB[4];               //0x22
	WORD wOnSound;                  //0x26
	WORD wOffSound;                  //0x28
	WORD wItemType;                  //0x2A
	BYTE nItemTrans;               //0x2C
	BYTE nGfxType;                  //0x2D
	WORD wGfxClass;                  //0x2E
	WORD wCltEvent;                  //0x30
	WORD wCltEventFunc;               //0x32
	WORD wCltActiveFunc;            //0x34
	WORD wSrvActiveFunc;            //0x36
	WORD wSkill;                  //0x38
	WORD wMissile;                  //0x3A
};

struct ObjectTxt
{
	CHAR szName[0x40];				//0x00
	WCHAR wszName[0x40];			//0x40
	BYTE _1[4];						//0xC0
	BYTE nSelectable0;				//0xC4
	BYTE _2[0x87];					//0xC5
	BYTE nOrientation;				//0x14C
	BYTE _2b[0x19];					//0x14D
	BYTE nSubClass;					//0x166
	BYTE _3[0x11];					//0x167
	BYTE nParm0;					//0x178
	BYTE _4[0x39];					//0x179
	BYTE nPopulateFn;				//0x1B2
	BYTE nOperateFn;				//0x1B3
	BYTE _5[8];						//0x1B4
	DWORD nAutoMap;					//0x1BB
}; 

struct LevelsTxt
{
	WORD wLevelNo;
	BYTE nPal;
	BYTE nAct;
	BYTE nTeleport;
	BYTE nRain;
	BYTE nMud;
	BYTE nNoPer;
	BYTE nIsInside;
	BYTE nDrawEdges;
	WORD unk0x0A;
	DWORD dwWarpDist;
	WORD wMonLvl[3];
	WORD wMonLvlEx[3];
	DWORD dwMonDen[3];
	BYTE nMonUMin[3];
	BYTE nMonUMax[3];
	BYTE nMonWndr;
	BYTE nMonSpcWalk;
	BYTE nQuest;
	BYTE nRangedSpawn;
	DWORD dwNumMon;
	WORD wMon[25];
	WORD wNMon[25];
	WORD wUMon[25];
	WORD wCMon[4];
	WORD wCPct[4];
	WORD wCAmt[4];
	BYTE nWaypoint;
	BYTE nObjGroup[8];
	BYTE nObjPrb[8];
	char szLevelName[40];
	char szLevelWarp[40];
	char szEntryFile[41];
	wchar_t wszLevelName[40];
	wchar_t wszLevelWarp[41];
	DWORD dwThemes;
	DWORD dwFloorFilter;
	DWORD dwBlankScreen;
	DWORD dwSoundEnv;
};

struct ItemStatCostTxt  //size 0x144
{
	WORD	wStat;				//0x00
	WORD 	_1;					//0x02
	struct {
		BYTE	bSendOther : 1;			//1
		BYTE	bSigned : 1;			//2
		BYTE	bDamageRelated : 1;		//3
		BYTE	bItemSpecific : 1;		//4
		BYTE	bDirect : 1;			//5
		BYTE	_1 : 1;					//6
		BYTE    _2 : 1;					//7
		BYTE    _3 : 1;					//8
		BYTE    _4 : 1;					//9	
		BYTE	bUpdateanimrate : 1;	//10
		BYTE	bFmin : 1;				//11
		BYTE	bFcallback : 1;			//12
		BYTE	bSaved : 1;				//13
		BYTE	bCsvsigned : 1;			//14
		BYTE	_6 : 1;					//15
		BYTE	_7 : 1;					//16
	} bItemStatFlags; 		 //0x04
	WORD 	_2;			  		 //0x06
	BYTE	bSendBits;			//0x08
	BYTE	bSendParamBits;		//0x09
	BYTE	bCsvBits;			//0x0A
	BYTE	bCsvParam;			//0x0B
	DWORD	dwDivide;			//0x0C
	DWORD	dwMultiply;			//0x10
	DWORD	dwAdd;				//0x14
	BYTE	bValShift;			//0x18
	BYTE	bSaveBits;			//0x19
	BYTE	b09SaveBits;		//0x1A
	BYTE	_3;					//0x1B
	DWORD	dwSaveAdd;			//0x1C
	DWORD	dw09SaveAdd;		//0x20
	DWORD	dwSaveParamBits;	//0x24
	DWORD 	_4;					//0x28
	DWORD	dwMinAccr;			//0x2C
	BYTE	bEncode;			//0x30
	BYTE	_5;					//0x31
	WORD	wMaxAtat;			//0x32
	WORD	wDescPriority;		//0x34
	BYTE	bDescFunc;			//0x36
	BYTE	bDescVal;			//0x37
	WORD	wDescStrPos;		//0x38
	WORD	wDescStrNeg;		//0x3A
	WORD	wDescStr2;			//0x3C
	WORD	wDGrp;				//0x3E
	BYTE	bDGrpFunc;			//0x40
	BYTE	bDGrpVal;			//0x41
	WORD	wDGrpStrPos;		//0x42
	WORD	wDGrpStrNeg;		//0x44
	WORD	wDGrpStr2;			//0x46
	WORD	wItemEvent1;		//0x48
	WORD	wItemEvent2;		//0x4A
	WORD	wItemEventFunc1;	//0x4C
	WORD	wItemEventFunc2;	//0x4E
	DWORD	bKeepZero;			//0x50
	BYTE	bOp;				//0x54
	BYTE	bOpParam;			//0x55
	WORD	wOpBase;			//0x56
	WORD	wOpStat1;			//0x58
	WORD	wOpStat2;			//0x5A
	WORD	wOpStat3;			//0x5C
	BYTE 	_Stuff[230];		//0x5E
};

struct MissilesTxt
{
	DWORD dwId;                     //0x00
	DWORD dwMissileFlags;            //0x04
	WORD wCltDoFunc;               //0x08
	WORD wCltHitFunc;               //0x0A
	WORD wSrvDoFunc;               //0x0C
	WORD wSrvHitFunc;               //0x0E
	WORD wSrvDmgFunc;               //0x10
	WORD wTravelSound;               //0x12
	WORD wHitSound;                  //0x14
	WORD wExplosionMissile;            //0x16
	WORD wSubMissile[3];            //0x18
	WORD wCltSubMissile[3];            //0x1E
	WORD wHitSubMissile[4];            //0x24
	WORD wCltHitSubMissile[4];         //0x2C
	WORD wProgSound;               //0x34
	WORD wProgOverlay;               //0x36
	DWORD dwParam[5];               //0x38
	DWORD dwHitPar[3];               //0x4C
	DWORD dwCltParam[5];            //0x58
	DWORD dwCltHitPar[3];            //0x6C
	DWORD dwDmgParam[2];            //0x78
	DWORD dwSrvCalc;               //0x80
	DWORD dwCltCalc;               //0x84
	DWORD dwHitCalc;               //0x88
	DWORD dwCltHitCalc;               //0x8C
	DWORD dwDmgCalc;               //0x90
	WORD wHitClass;                  //0x94
	WORD wRange;                  //0x96
	WORD wLevRange;                  //0x98
	BYTE nVel;                     //0x9A
	BYTE nVelLev;                  //0x9B
	WORD wMaxVel;                  //0x9C
	WORD wAccel;                  //0x9E
	WORD wAnimRate;                  //0xA0
	WORD wXoffset;                  //0xA2
	WORD wYoffset;                  //0xA4
	WORD wZoffset;                  //0xA6
	DWORD dwHitFlags;               //0xA8
	WORD wResultFlags;               //0xAC
	WORD wKnockBack;               //0xAE
	DWORD dwMinDamage;               //0xB0
	DWORD dwMaxDamage;               //0xB4
	DWORD dwMinLevDam[5];            //0xB8
	DWORD dwMaxLevDam[5];            //0xCC
	DWORD dwDmgSymPerCalc;            //0xE0
	DWORD dwElemType;               //0xE4
	DWORD dwElemMin;               //0xE8
	DWORD dwElemMax;               //0xEC
	DWORD dwMinElemLev[5];            //0xF0
	DWORD dwMaxElemLev[5];            //0x104
	DWORD dwElemDmgSymPerCalc;         //0x118
	DWORD dwElemLen;               //0x11C
	DWORD dwElemLevLen[3];            //0x120
	BYTE unk0x12C;                  //0x12C
	BYTE nSrcDamage;               //0x12D
	BYTE nSrcMissDmg;               //0x12E
	BYTE nHoly;                     //0x12F
	BYTE nLight;                  //0x130
	BYTE nFlicker;                  //0x131
	BYTE nRGB[3];                  //0x132
	BYTE nInitSteps;               //0x135
	BYTE nActivate;                  //0x136
	BYTE nLoopAnim;                  //0x137
	char szCelFile[64];               //0x138
	DWORD dwAnimLen;               //0x178
	DWORD dwRandStart;               //0x17C
	BYTE nSubLoop;                  //0x180
	BYTE nSubStart;                  //0x181
	BYTE nSubStop;                  //0x182
	BYTE nCollideType;               //0x183
	BYTE nCollision;               //0x184
	BYTE nClientCol;               //0x185
	BYTE nCollideKill;               //0x186
	BYTE nCollideFriend;            //0x187
	BYTE nNextHit;                  //0x188
	BYTE nNextDelay;               //0x189
	BYTE nSize;                     //0x18A
	BYTE nToHit;                  //0x18B
	BYTE nAlwaysExplode;            //0x18C
	BYTE nTrans;                  //0x18D
	WORD wQty;                     //0x18E
	DWORD dwSpecialSetup;            //0x190
	WORD wSkill;                  //0x194
	BYTE nHitShift;                  //0x196
	BYTE unk0x197[5];               //0x197
	DWORD dwDamageRate;               //0x19C
	BYTE nNumDirections;            //0x1A0
	BYTE nAnimSpeed;               //0x1A1
	BYTE nLocalBlood;               //0x1A2
	BYTE unk;                     //0x1A3
};

#pragma pack(pop)

struct DifficultyLevelsTxt //size 0x58
{
	DWORD ResistPenalty;			//0x00
	DWORD DeathExpPenalty;			//0x04
	DWORD UberCodeOddsNorm;			//0x08
	DWORD UberCodeOddsGood;			//0x0C
	DWORD MonsterSkillBonus;		//0x10
	DWORD MonsterFreezeDiv;			//0x14
	DWORD MonsterColdDiv;			//0x18
	DWORD AiCurseDiv;				//0x1C
	DWORD UltraCodeOddsNorm;		//0x20
	DWORD UltraCodeOddsGood;		//0x24
	DWORD LifeStealDiv;				//0x28
	DWORD ManaStealDiv;				//0x2C
	DWORD UniqueDmgBonus;			//0x30
	DWORD ChampionDmgBonus;			//0x34
	DWORD HireableBossDmgPercent;	//0x38
	DWORD MonsterCEDmgPercent;		//0x3C
	DWORD StaticFieldMin;			//0x40
	DWORD GambleRare;				//0x44
	DWORD GambleSet;				//0x48
	DWORD GambleUniq;				//0x4C
	DWORD GambleUber;				//0x50
	DWORD GambleUltra;				//0x54
};

struct CubeInputItem		//size (0x08)
{
	struct {
		BYTE	bCodeFlag:1;			//1
		BYTE	bClassFlag:1;			//2
		BYTE	bAnyFlag:1;				//3
		BYTE	bSocketFlag:1;			//4
		BYTE	bNoEtherealFlag:1;		//5
		BYTE	bRepairFlag:1;			//6
		BYTE	bUniqueItemNameFlag:1;	//7
		BYTE	bUpgFlag:1;				//8
	} bInputFlags; 				//0x00
	BYTE	ItemType;			//0x01
	WORD	Item;				//0x02
	WORD	ItemID;				//0x04
	BYTE	Quality;			//0x06
	BYTE	Quantity;			//0x07
};

struct CubeOutputItem //size 0x54
{
	BYTE	bItemFlags;			//0x00
	BYTE	ItemType;			//0x01
	WORD	Item;				//0x02
	WORD	ItemID;				//0x04
	union
	{
		struct
		{
			BYTE nQuality;
			BYTE nQuantity;
		};
		struct
		{
			BYTE nLevel;
			BYTE nAct;				
		};						   //for CUBE_OUTPUT_PORTAL
		WORD nParam;               //0x06
	};
	BYTE	Type;				//0x08
	BYTE	Lvl;				//0x09
	BYTE	PLvl;				//0x0A
	BYTE	ILvl;				//0x0B
	WORD	PrefixId[3];		//0x0C
	WORD	SuffixId[3];		//0x12
	struct {							//size 0x0C
		DWORD	dwMod;				//0x00
		WORD	wModParam;			//0x04
		WORD	wModMin;			//0x06
		WORD	wModMax;			//0x08
		WORD	bModChance;			//0x0A
	} sMods[5]; 			//0x18	
};

struct CubeMainTxt		//size 0x148
{
	BYTE	bEnabled;			//0x00
	BYTE	bLadder;			//0x01
	BYTE	bMindiff;			//0x02
	BYTE	bClass;				//0x03
	BYTE	bOp;				//0x04
	BYTE	_1[3];				//0x05
	DWORD	dwParam;			//0x08
	DWORD	dwValue;			//0x0C
	WORD	bNuminputs;			//0x10
	WORD	wVersion;			//0x12
	CubeInputItem InputItem[7];	//0x14
	CubeOutputItem OutputItem[3];//0x4C
};

struct CharStatsTxt
{
	char	szwClass[32];	//0x0
	char	szClass[15];	//0x20
	BYTE	bStr;	//0x30
	BYTE	bDex;	//0x31
	BYTE	bEne;	//0x32
	BYTE	bVita;	//0x33
	BYTE	bStamina;	//0x34
	BYTE	bHpAdd;	//0x35
	BYTE	bPercentStr;	//0x36
	BYTE	bPercentInt;	//0x37
	BYTE	bPercentDex;	//0x38
	BYTE	bPercentVit;	//0x39
	BYTE	bManaRegen;	//0x3A
	BYTE 	_1;			//0x3B
	DWORD	dwToHitFactor;	//0x3C
	BYTE	bWalkVelocity;	//0x40
	BYTE	bRunVelocity;	//0x41
	BYTE	bRunDrain;	//0x42
	BYTE	bLifePerLevel;	//0x43
	BYTE	bStaminaPerLevel;	//0x44
	BYTE	bManaPerLevel;	//0x45
	BYTE	bLifePerVitality;	//0x46
	BYTE	bStaminaPerVitality;	//0x47
	BYTE	bManaPerMagic;	//0x48
	BYTE	bBlockFactor;	//0x49
	BYTE	_2[2];			//0x4A
	DWORD	dwBaseWClass;	//0x4C
	BYTE	bStatPerLevel;	//0x50
	BYTE	_3;				//0x51
	WORD	wStrAllSkills;	//0x52
	WORD	wStrSkillTab1;	//0x54
	WORD	wStrSkillTab2;	//0x56
	WORD	wStrSkillTab3;	//0x58
	WORD	wStrClassOnly;	//0x5A
	DWORD	dwItem1;	//0x5C
	BYTE	bItem1loc;	//0x60
	BYTE	bItem1count;	//0x61
	BYTE	_4[2];			//0x62
	DWORD	dwItem2;	//0x64
	BYTE	bItem2loc;	//0x68
	BYTE	bItem2count;	//0x69
	BYTE	_5[2];			//0x6A
	DWORD	dwItem3;	//0x6C
	BYTE	bItem3loc;	//0x70
	BYTE	bItem3count;	//0x71
	BYTE	_6[2];			//0x72
	DWORD	dwItem4;	//0x74
	BYTE	bItem4loc;	//0x78
	BYTE	bItem4count;	//0x79
	BYTE	_7[2];			//0x7A
	DWORD	dwItem5;	//0x7C
	BYTE	bItem5loc;	//0x80
	BYTE	bItem5count;	//0x81
	BYTE	_8[2];			//0x82
	DWORD	dwItem6;	//0x84
	BYTE	bItem6loc;	//0x88
	BYTE	bItem6count;	//0x89
	BYTE	_9[2];			//0x8A
	DWORD	dwItem7;	//0x8C
	BYTE	bItem7loc;	//0x90
	BYTE	bItem7count;	//0x91
	BYTE	_10[2];			//0x92
	DWORD	dwItem8;	//0x94
	BYTE	bItem8loc;	//0x98
	BYTE	bItem8count;	//0x99
	BYTE	_11[2];			//0x9A
	DWORD	dwItem9;	//0x9C
	BYTE	bItem9loc;	//0xA0
	BYTE	bItem9count;	//0xA1
	BYTE	_12[2];			//0xA2
	DWORD	dwItem10;	//0xA4
	BYTE	bItem10loc;	//0xA8
	BYTE	bItem10count;	//0xA9
	BYTE	_13[2];			//0xAA
	WORD	wStartSkill;	//0xAC
	WORD	wSkill1;	//0xAE
	WORD	wSkill2;	//0xB0
	WORD	wSkill3;	//0xB2
	WORD	wSkill4;	//0xB4
	WORD	wSkill5;	//0xB6
	WORD	wSkill6;	//0xB8
	WORD	wSkill7;	//0xBA
	WORD	wSkill8;	//0xBC
	WORD	wSkill9;	//0xBE
	WORD	wSkill10;	//0xC0
};

struct MonStatsTxt // size 0x1A8
{
	WORD	wId;						//0x00
	WORD	wBaseId;					//0x02
	WORD	wNextInClass;				//0x04
	WORD	wNameStr;					//0x06
	WORD	wDescStr;					//0x08
	WORD	_1a;						//0x0A
	struct {
		BYTE	bisSpawn:1;						//1
		BYTE	bisMelee:1;						//2
		BYTE	bnoRatio:1;						//3
		BYTE	bOpenDoors:1;					//4
		BYTE	bSetBoss:1;						//5
		BYTE	bBossXfer:1;					//6
		BYTE	bBossXfer2:1;					//7
		BYTE	bPrimeEvil:1;					//8
		BYTE	bNPC:1;							//9
		BYTE	bInteract:1;					//10
		BYTE	binTown:1;						//11
		BYTE	blUndead:1;						//12
		BYTE	bhUndead:1;						//13
		BYTE	bDemon:1;						//14
		BYTE	bFlying:1;						//15
		BYTE	bKillable:1;					//16
		BYTE	bSwitchAI:1;					//17
		BYTE	bNoMultiShot:1;					//18
		BYTE	bNeverCount:1;					//19
		BYTE	bPetIgnore:1;					//20
		BYTE	bDeathDmg:1;					//21
		BYTE	bGenericSpawn:1;				//22
		BYTE	bZoo:1;							//23
		BYTE	bPlaceSpawn:1;					//24
		BYTE	bInventory:1;					//25
		BYTE	bEnabled:1;						//26
		BYTE	bNoShldBlock:1;					//27
		BYTE	bNoAura:1;						//28
		BYTE	bRangedType:1;					//29
	} dwFlags;							//0x0C
	DWORD	dwCode;						//0x10
	WORD	wMonSound;					//0x14
	WORD	wUMonSound;					//0x16
	WORD	wMonStatsEx;				//0x18
	WORD	wMonProp;					//0x1A
	WORD	wMonType;					//0x1C
	WORD	wAI;						//0x1E
	WORD	wSpawn;						//0x20
	BYTE	bSpawnX;					//0x22
	BYTE	bSpawnY;					//0x23
	WORD	bSpawnMode;					//0x24
	WORD	wMinion1;					//0x26
	WORD	wMinion2;					//0x28
	WORD	_1;							//0x2A
	BYTE	bPartyMin;					//0x2C
	BYTE	bPartyMax;					//0x2D
	BYTE	bRarity;					//0x2E
	BYTE	bMinGrp;					//0x2F
	BYTE	bMaxGrp;					//0x30
	BYTE	bSparsePopulate;			//0x31
	WORD	wVelocity;					//0x32
	WORD	wRun;						//0x34
	WORD	_2;							//0x36
	WORD	_2b;						//0x38
	WORD	wMissA1;					//0x3A
	WORD	wMissA2;					//0x3C
	WORD	wMissS1;					//0x3E
	WORD	wMissS2;					//0x40
	WORD	wMissS3;					//0x42
	WORD	wMissS4;					//0x44
	WORD	wMissC;						//0x46
	WORD	wMissSQ;					//0x48
	WORD	_3;							//0x4A
	BYTE	bAlign;						//0x4C
	BYTE	bTransLvl;					//0x4D
	BYTE	bThreat;					//0x4E
	BYTE	bAIdel;						//0x4F
	BYTE	bAIdel_N;					//0x50
	BYTE	bAIdel_H;					//0x51
	BYTE	bAiDist;					//0x52
	BYTE	bAiDist_N;					//0x53
	WORD	bAiDist_H;					//0x54
	WORD	wAiP1;						//0x56
	WORD	wAiP1_N;					//0x58
	WORD	wAiP1_H;					//0x5A
	WORD	wAiP2;						//0x5C
	WORD	wAiP2_N;					//0x5E
	WORD	wAiP2_H;					//0x60
	WORD	wAiP3;						//0x62
	WORD	wAiP3_N;					//0x64
	WORD	wAiP3_H;					//0x66
	WORD	wAiP4;						//0x68
	WORD	wAiP4_N;					//0x6A
	WORD	wAiP4_H;					//0x6C
	WORD	wAiP5;						//0x6E
	WORD	wAiP5_N;					//0x70
	WORD	wAiP5_H;					//0x72
	WORD	wAiP6;						//0x74
	WORD	wAiP6_N;					//0x76
	WORD	wAiP6_H;					//0x78
	WORD	wAiP7;						//0x7A
	WORD	wAiP7_N;					//0x7C
	WORD	wAiP7_H;					//0x7E
	WORD	wAiP8;						//0x80
	WORD	wAiP8_N;					//0x82
	WORD	wAiP8_H;					//0x84
	WORD	wTreasureClass1;			//0x86
	WORD	wTreasureClass2;			//0x88
	WORD	wTreasureClass3;			//0x8A
	WORD	wTreasureClass4;			//0x8C
	WORD	wTreasureClass1_N;			//0x8E
	WORD	wTreasureClass2_N;			//0x90
	WORD	wTreasureClass3_N;			//0x92
	WORD	wTreasureClass4_N;			//0x94
	WORD	wTreasureClass1_H;			//0x96
	WORD	wTreasureClass2_H;			//0x98
	WORD	wTreasureClass3_H;			//0x9A
	WORD	wTreasureClass4_H;			//0x9C
	BYTE	bTCQuestId;					//0x9E
	BYTE	bTCQuestCP;					//0x9F
	BYTE	bDrain;						//0xA0
	BYTE	bDrain_N;					//0xA1
	BYTE	bDrain_H;					//0xA2
	BYTE	bToBlock;					//0xA3
	BYTE	bToBlock_N;					//0xA4
	BYTE	bToBlock_H;					//0xA5
	WORD	bCrit;						//0xA6
	WORD	wSkillDamage;				//0xA8
	WORD	wLevel;						//0xAA
	WORD	wLevel_N;					//0xAC
	WORD	wLevel_H;					//0xAE
	WORD	wMinHP;						//0xB0
	WORD	wMinHP_N;					//0xB2
	WORD	wMinHP_H;					//0xB4
	WORD	wMaxHP;						//0xB6
	WORD	wMaxHP_N;					//0xB8
	WORD	wMaxHP_H;					//0xBA
	WORD	wAC;						//0xBC
	WORD	wAC_N;						//0xBE
	WORD	wAC_H;						//0xC0
	WORD	wA1TH;						//0xC2
	WORD	wA1TH_N;					//0xC4
	WORD	wA1TH_H;					//0xC6
	WORD	wA2TH;						//0xC8
	WORD	wA2TH_N;					//0xCA
	WORD	wA2TH_H;					//0xCC
	WORD	wS1TH;						//0xCE
	WORD	wS1TH_N;					//0xD0
	WORD	wS1TH_H;					//0xD2
	WORD	wExp;						//0xD4
	WORD	wExp_N;						//0xD6
	WORD	wExp_H;						//0xD8
	WORD	wA1MinD;					//0xDA
	WORD	wA1MinD_N;					//0xDC
	WORD	wA1MinD_H;					//0xDE
	WORD	wA1MaxD;					//0xE0
	WORD	wA1MaxD_N;					//0xE2
	WORD	wA1MaxD_H;					//0xE4
	WORD	wA2MinD;					//0xE6
	WORD	wA2MinD_N;					//0xE8
	WORD	wA2MinD_H;					//0xEA
	WORD	wA2MaxD;					//0xEC
	WORD	wA2MaxD_N;					//0xEE
	WORD	wA2MaxD_H;					//0xF0
	WORD	wS1MinD;					//0xF2
	WORD	wS1MinD_N;					//0xF4
	WORD	wS1MinD_H;					//0xF6
	WORD	wS1MaxD;					//0xF8
	WORD	wS1MaxD_N;					//0xFA
	WORD	wS1MaxD_H;					//0xFC
	BYTE	bEl1Mode;					//0xFE
	BYTE	bEl2Mode;					//0xFF
	BYTE	bEl3Mode;					//0x100
	BYTE	bEl1Type;					//0x101
	BYTE	bEl2Type;					//0x102
	BYTE	bEl3Type;					//0x103
	BYTE	bEl1Pct;					//0x104
	BYTE	bEl1Pct_N;					//0x105
	BYTE	bEl1Pct_H;					//0x106
	BYTE	bEl2Pct;					//0x107
	BYTE	bEl2Pct_N;					//0x108
	BYTE	bEl2Pct_H;					//0x109
	BYTE	bEl3Pct;					//0x10A
	BYTE	bEl3Pct_N;					//0x10B
	BYTE	bEl3Pct_H;					//0x10C
	BYTE	_4;							//0x10D
	WORD	wEl1MinD;					//0x10E
	WORD	wEl1MinD_N;					//0x110
	WORD	wEl1MinD_H;					//0x112
	WORD	wEl2MinD;					//0x114
	WORD	wEl2MinD_N;					//0x116
	WORD	wEl2MinD_H;					//0x118
	WORD	wEl3MinD;					//0x11A
	WORD	wEl3MinD_N;					//0x11C
	WORD	wEl3MinD_H;					//0x11E
	WORD	wEl1MaxD;					//0x120
	WORD	wEl1MaxD_N;					//0x122
	WORD	wEl1MaxD_H;					//0x124
	WORD	wEl2MaxD;					//0x126
	WORD	wEl2MaxD_N;					//0x128
	WORD	wEl2MaxD_H;					//0x12A
	WORD	wEl3MaxD;					//0x12C
	WORD	wEl3MaxD_N;					//0x12E
	WORD	wEl3MaxD_H;					//0x130
	WORD	wEl1Dur;					//0x132
	WORD	wEl1Dur_N;					//0x134
	WORD	wEl1Dur_H;					//0x136
	WORD	wEl2Dur;					//0x138
	WORD	wEl2Dur_N;					//0x13A
	WORD	wEl2Dur_H;					//0x13C
	WORD	wEl3Dur;					//0x13E
	WORD	wEl3Dur_N;					//0x140
	WORD	wEl3Dur_H;					//0x142
	WORD	wResDmg;					//0x144
	WORD	wResDmg_N;					//0x146
	WORD	wResDmg_H;					//0x148
	WORD	wResMagic;					//0x14A
	WORD	wResMagic_N;				//0x14C
	WORD	wResMagic_H;				//0x14E
	WORD	wResFire;					//0x150
	WORD	wResFire_N;					//0x152
	WORD	wResFire_H;					//0x154
	WORD	wResLight;					//0x156
	WORD	wResLight_N;				//0x158
	WORD	wResLight_H;				//0x15A
	WORD	wResCold;					//0x15C
	WORD	wResCold_N;					//0x15E
	WORD	wResCold_H;					//0x160
	WORD	wResPoison;					//0x162
	WORD	wResPoison_N;				//0x164
	WORD	wResPoiosn_H;				//0x166
	BYTE	bColdEffect;				//0x168
	BYTE	bColdEffect_N;				//0x169
	WORD	bColdEffect_H;				//0x16A
	DWORD	dwSendSkills;				//0x16C
	WORD	wSkill1;					//0x170
	WORD	wSkill2;					//0x172
	WORD	wSkill3;					//0x174
	WORD	wSkill4;					//0x176
	WORD	wSkill5;					//0x178
	WORD	wSkill6;					//0x17A
	WORD	wSkill7;					//0x17C
	WORD	wSkill8;					//0x17E
	DWORD   _5[6];						//0x180
	BYTE	bSk1lvl;					//0x198
	BYTE	bSk2lvl;					//0x199
	BYTE	bSk3lvl;					//0x19A
	BYTE	bSk4lvl;					//0x19B
	BYTE	bSk5lvl;					//0x19C
	BYTE	bSk6lvl;					//0x19D
	BYTE	bSk7lvl;					//0x19E
	BYTE	bSk8lvl;					//0x19F
	DWORD	dwDamageRegen;				//0x1A0
	BYTE	bSplEndDeath;				//0x1A4
	BYTE	bSplGetModeChart;			//0x1A5
	BYTE	bSplEndGeneric;				//0x1A6
	BYTE	bSplClientEnd;				//0x1A7
};

struct SkillsTxt //size 0x23C
{
	DWORD wSkillId; 			//0x00
	struct 
	{
		BYTE bDecquant:1;				//0
		BYTE bLob:1;					//1
		BYTE bProgressive:1;			//2
		BYTE bFinishing:1;				//3
		BYTE bPassive:1;				//4
		BYTE bAura:1;					//5
		BYTE bPeriodic:1;				//6
		BYTE bPrgStack:1;				//7
		BYTE bInTown:1;					//8
		BYTE bKick:1;					//9
		BYTE bInGame:1;					//10
		BYTE bRepeat:1;					//11
		BYTE bStSuccessOnly:1;			//12
		BYTE bStSoundDelay:1;			//13
		BYTE bWeaponSnd:1;				//14
		BYTE bImmediate:1;				//15
		BYTE bNoAmmo:1;					//16
		BYTE bEnhanceable:1;			//17
		BYTE bDurability:1;				//18
		BYTE bUseAttackRating:1;		//19
		BYTE bTargetableOnly:1;			//20
		BYTE bSearchEnemyXY:1;			//21
		BYTE bSearchEnemyNear:1;		//22
		BYTE bSearchOpenXY:1;			//23
		BYTE bTargetCorpse:1;			//24
		BYTE bTargetPet:1;				//25
		BYTE bTargetAlly:1;				//26
		BYTE bTargetItem:1;				//27
		BYTE bAttackNoMana:1;			//28
		BYTE bItemTgtDo:1;				//29
		BYTE bLeftSkill:1;				//30
		BYTE bInterrupt:1;				//31
		BYTE bTgtPlaceCheck:1;			//32
		BYTE bItemCheckStart:1;			//33
		BYTE bItemCltCheckStart:1;		//34
		BYTE bGeneral:1;				//35
		BYTE bScroll:1;					//36
		BYTE bUseManaOnDo:1;			//37
		BYTE bWarp:1;					//38
	} dwFlags;				//0x04 (5 bytes)
	DWORD bClassId;			//0x0C
	BYTE bAnim;				//0x10
	BYTE bMonAnim;			//0x11
	BYTE bSeqTrans;			//0x12
	BYTE bSeqNum;			//0x13
	BYTE bRange;			//0x14
	BYTE bSelectProc;		//0x15
	BYTE bSeqInput;			//0x16
	BYTE _1;				//0x17
	WORD wITypeA1;			//0x18
	WORD wITypeA2;			//0x1A
	WORD wITypeA3;			//0x1C
	WORD wITypeB1;			//0x1E
	WORD wITypeB2;			//0x20
	WORD wITypeB3;			//0x22
	WORD wETypeA1;			//0x24
	WORD wETypeA2;			//0x26
	WORD wETypeB1;			//0x28
	WORD wETypeB2;			//0x2A
	WORD wSrvStartFunc;		//0x2C
	WORD wSrvDoFunc;		//0x2E
	WORD wSrvPrgFunc1;		//0x30
	WORD wSrvPrgFunc2;		//0x32
	WORD wSrvPrgFunc3;		//0x34
	WORD _2;				//0x36
	DWORD dwPrgCalc1;		//0x38
	DWORD dwPrgCalc2;		//0x3C
	DWORD dwPrgCalc3;		//0x40
	WORD bPrgDamage;		//0x44
	WORD wSrvMissile;		//0x46
	WORD wSrvMissileA;		//0x48
	WORD wSrvMissileB;		//0x4A
	WORD wSrvMissileC;		//0x4C
	WORD wSrvOverlay;		//0x4E
	DWORD dwAuraFilter;		//0x50
	WORD wAuraStat1;		//0x54
	WORD wAuraStat2;		//0x56
	WORD wAuraStat3;		//0x58
	WORD wAuraStat4;		//0x5A
	WORD wAuraStat5;		//0x5C
	WORD wAuraStat6;		//0x5E
	DWORD dwAuraLenCalc;	//0x60
	DWORD dwAuraRangeCalc;	//0x64
	DWORD dwAuraStatCalc1;	//0x68
	DWORD dwAuraStatCalc2;	//0x6C
	DWORD dwAuraStatCalc3;	//0x70
	DWORD dwAuraStatCalc4;	//0x74
	DWORD dwAuraStatCalc5;	//0x78
	DWORD dwAuraStatCalc6;	//0x7C
	WORD wAuraState;		//0x80
	WORD wAuraTargetState;	//0x82
	WORD wAuraEvent1;		//0x84
	WORD wAuraEvent2;		//0x86
	WORD wAuraEvent3;		//0x88
	WORD wAuraEventFunc1;	//0x8A
	WORD wAuraEventFunc2;	//0x8C
	WORD wAuraEventFunc3;	//0x8E
	WORD wAuraTgtEvent;		//0x90
	WORD wAuraTgtEventFunc;	//0x92
	WORD wPassiveState;		//0x94
	WORD wPassiveiType;		//0x96
	WORD wPassiveStat1;		//0x98
	WORD wPassiveStat2;		//0x9A
	WORD wPassiveStat3;		//0x9C
	WORD wPassiveStat4;		//0x9E
	WORD wPassiveStat5;		//0xA0
	WORD _3;				//0xA2
	DWORD dwPassiveCalc1;	//0xA4
	DWORD dwPassiveCalc2;	//0xA8
	DWORD dwPassiveCalc3;	//0xAC
	DWORD dwPassiveCalc4;	//0xB0
	DWORD dwPassiveCalc5;	//0xB4
	WORD wPassiveEvent;		//0xB8
	WORD wPassiveEventFunc; //0xBA
	WORD wSummon;			//0xBC
	BYTE wPetType;			//0xBE
	BYTE bSumMode;			//0xBF
	DWORD dwPetMax;			//0xC0
	WORD wSumSkill1;		//0xC4
	WORD wSumSkill2;		//0xC6
	WORD wSumSkill3;		//0xC8
	WORD wSumSkill4;		//0xCA
	WORD wSumSkill5;		//0xCC
	WORD _4;				//0xCE
	DWORD dwSumSkCalc1;		//0xD0
	DWORD dwSumSkCalc2;		//0xD4
	DWORD dwSumSkCalc3;		//0xD8
	DWORD dwSumSkCalc4;		//0xDC
	DWORD dwSumSkCalc5;		//0xE0
	WORD wSumUMod;			//0xE4
	WORD wSumOverlay;		//0xE6
	WORD wCltMissile;		//0xE8
	WORD wCltMissileA;		//0xEA
	WORD wCltMissileB;		//0xEC
	WORD wCltMissileC;		//0xEE
	WORD wCltMissileD;		//0xF0
	WORD wCltStFunc;		//0xF2
	WORD wCltDoFunc;		//0xF4
	WORD wCltPrgFunc1;		//0xF6
	WORD wCltPrgFunc2;		//0xF8
	WORD wCltPrgFunc3;		//0xFA
	WORD wStSound;			//0xFC
	WORD _5;				//0xFE
	WORD wDoSound;			//0x100
	WORD wDoSoundA;			//0x102
	WORD wDoSoundB;			//0x104
	WORD wCastOverlay;		//0x106
	WORD wTgtOverlay;		//0x108
	WORD wTgtSound;			//0x10A
	WORD wPrgOverlay;		//0x10C
	WORD wPrgSound;			//0x10E
	WORD wCltOverlayA;		//0x110
	WORD wCltOverlayB;		//0x112
	DWORD dwCltCalc1;		//0x114
	DWORD dwCltCalc2;		//0x118
	DWORD dwCltCalc3;		//0x11C
	WORD bItemTarget;		//0x120
	WORD wItemCastSound;	//0x122
	DWORD wItemCastOverlay;	//0x124
	DWORD dwPerDelay;		//0x128
	WORD wMaxLvl;			//0x12C
	WORD wResultFlags;		//0x12E
	DWORD dwHitFlags;		//0x130
	DWORD dwHitClass;		//0x134
	DWORD dwCalc1;			//0x138
	DWORD dwCalc2;			//0x13C
	DWORD dwCalc3;			//0x140
	DWORD dwCalc4;			//0x144
	DWORD dwParam1;			//0x148
	DWORD dwParam2;			//0x14C
	DWORD dwParam3;			//0x150
	DWORD dwParam4;			//0x154
	DWORD dwParam5;			//0x158
	DWORD dwParam6;			//0x15C
	DWORD dwParam7;			//0x160
	DWORD dwParam8;			//0x164
	WORD bWeapSel;			//0x168
	WORD wItemEffect;		//0x16A
	DWORD wItemCltEffect;	//0x16C
	DWORD dwSkPoints;		//0x170
	WORD wReqLevel;			//0x174
	WORD wReqStr;			//0x176
	WORD wReqDex;			//0x178
	WORD wReqInt;			//0x17A
	WORD wReqVit;			//0x17C
	WORD wReqSkill1;		//0x17E
	WORD wReqSkill2;		//0x180
	WORD wReqSkill3;		//0x182
	WORD wStartMana;		//0x184
	WORD wMinMana;			//0x186
	WORD wManaShift;		//0x188
	WORD wMana;				//0x18A
	WORD wLevelMana;		//0x18C
	BYTE bAttackRank;		//0x18E
	BYTE bLineOfSight;		//0x18F
	DWORD dwDelay;			//0x190
	DWORD wSkillDesc;		//0x194
	DWORD dwToHit;			//0x198
	DWORD dwLevToHit;		//0x19C
	DWORD dwToHitCalc;		//0x1A0
	BYTE bToHitShift;		//0x1A4
	BYTE bSrcDam;			//0x1A5
	WORD _6;				//0x1A6
	DWORD dwMinDam;			//0x1A8
	DWORD dwMaxDam;			//0x1AC
	DWORD dwMinLvlDam1;		//0x1B0
	DWORD dwMinLvlDam2;		//0x1B4
	DWORD dwMinLvlDam3;		//0x1B8
	DWORD dwMinLvlDam4;		//0x1BC
	DWORD dwMinLvlDam5;		//0x1C0
	DWORD dwMaxLvlDam1;		//0x1C4
	DWORD dwMaxLvlDam2;		//0x1C8
	DWORD dwMaxLvlDam3;		//0x1CC
	DWORD dwMaxLvlDam4;		//0x1D0
	DWORD dwMaxLvlDam5;		//0x1D4
	DWORD dwDmgSymPerCalc;	//0x1D8
	WORD bEType;			//0x1DC
	WORD _7;				//0x1DE
	DWORD dwEMin;			//0x1E0
	DWORD dwEMax;			//0x1E4
	DWORD dwEMinLev1;		//0x1E8
	DWORD dwEMinLev2;		//0x1EC
	DWORD dwEMinLev3;		//0x1F0
	DWORD dwEMinLev4;		//0x1F4
	DWORD dwEMinLev5;		//0x1F8
	DWORD dwEMaxLev1;		//0x1FC
	DWORD dwEMaxLev2;		//0x200
	DWORD dwEMaxLev3;		//0x204
	DWORD dwEMaxLev4;		//0x208
	DWORD dwEMaxLev5;		//0x20C
	DWORD dwEDmgSymPerCalc;	//0x210
	DWORD dwELen;			//0x214
	DWORD dwELevLen1;		//0x218
	DWORD dwELevLen2;		//0x21C
	DWORD dwELevLen3;		//0x220
	DWORD dwELenSymPerCalc;	//0x224
	WORD bRestrict;			//0x228
	WORD wState1;			//0x22A
	WORD wState2;			//0x22C
	WORD wState3;			//0x22E
	WORD bAiType;			//0x230
	WORD wAiBonus;			//0x232
	DWORD dwCostMult;		//0x234
	DWORD dwCostAdd;		//0x238
}; 

#pragma pack(push,1)
struct SkillDescTxt
{
	WORD wSkillDesc;               //0x00
	BYTE bSkillPage;               //0x02
	BYTE bSkillRow;                  //0x03
	BYTE bSkillColumn;               //0x04
	BYTE bListRow;                  //0x05
	BYTE bListPool;                  //0x06
	BYTE bIconCel;                  //0x07
	WORD wStrName;                  //0x08
	WORD wStrShort;                  //0x0A
	WORD wStrLong;                  //0x0C
	WORD wStrAlt;                  //0x0E
	WORD wStrMana;                  //0x10
	WORD wDescDam;                  //0x12
	WORD wDescAtt;                  //0x14
	WORD _1;					  //0x16
	DWORD dwDamCalc[2];               //0x18
	BYTE bPrgDamElem[4];            //0x20
	DWORD dwProgDmgMin[3];            //0x24
	DWORD dwProgDmgMax[3];            //0x30
	WORD wDescMissile[3];            //0x3C
	BYTE bDescLine[18];               //0x42
	WORD wDescTextA[17];            //0x54
	WORD wDescTextB[17];            //0x76
	DWORD dwDescCalcA[17];            //0x98
	DWORD dwDescCalcB[17];            //0xDC
};

#pragma pack(pop)

struct ItemsTxt //size = 0x1A8, Valid for Weapons, Armors, Misc.txts
{
	char	szflippyfile[32];		//0x00
	char	szinvfile[32];			//0x20
	char	szuniqueinvfile[32];	//0x40
	char	szsetinvfile[32];		//0x60
	union{
		DWORD	dwcode;					//0x80
		char	szcode[4];				//0x80
	};
	DWORD	dwnormcode;				//0x84
	DWORD	dwubercode;				//0x88
	DWORD	dwultracode;			//0x8C
	DWORD	dwalternategfx;			//0x90
	DWORD	dwpSpell;				//0x94
	WORD	wstate;					//0x98
	WORD	wcstate1;				//0x9A
	WORD	wcstate2;				//0x9C
	WORD	wstat1;					//0x9E
	WORD	wstat2;					//0xA0
	WORD	wstat3;					//0xA2
	DWORD	dwcalc1;				//0xA4
	DWORD	dwcalc2;				//0xA8
	DWORD	dwcalc3;				//0xAC
	DWORD	dwlen;					//0xB0
	WORD	bspelldesc;				//0xB4
	WORD	wspelldescstr;			//0xB6
	DWORD	dwspelldesccalc;		//0xB8
	DWORD	dwBetterGem;			//0xBC
	DWORD	dwwclass;				//0xC0
	DWORD	dw2handedwclass;		//0xC4
	DWORD	dwTMogType;				//0xC8
	DWORD	dwminac;				//0xCC
	DWORD	dwmaxac;				//0xD0
	DWORD	dwgamblecost;			//0xD4
	DWORD	dwspeed;				//0xD8
	DWORD	dwbitfield1;			//0xDC
	DWORD	dwcost;					//0xE0
	DWORD	dwminstack;				//0xE4
	DWORD	dwmaxstack;				//0xE8
	DWORD	dwspawnstack;			//0xEC
	DWORD	dwgemoffset;			//0xF0
	WORD	wnamestr;				//0xF4
	WORD	wversion;				//0xF6
	WORD	wautoprefix;			//0xF8
	WORD	wmissiletype;			//0xFA
	BYTE	brarity;				//0xFC
	BYTE	blevel;					//0xFD
	BYTE	bmindam;				//0xFE
	BYTE	bmaxdam;				//0xFF
	BYTE	bminmisdam;				//0x100
	BYTE	bmaxmisdam;				//0x101
	BYTE	b2handmindam;			//0x102
	BYTE	b2handmaxdam;			//0x103
	WORD	brangeadder;			//0x104
	WORD	wstrbonus;				//0x106
	WORD	wdexbonus;				//0x108
	WORD	wreqstr;				//0x10A
	WORD	wreqdex;				//0x10C
	BYTE	babsorbs;				//0x10E
	BYTE	binvwidth;				//0x10F
	BYTE	binvheight;				//0x110
	BYTE	bblock;					//0x111
	BYTE	bdurability;			//0x112
	BYTE	bnodurability;			//0x113
	BYTE	bmissile;				//0x114
	BYTE	bcomponent;				//0x115
	BYTE	brArm;					//0x116
	BYTE	blArm;					//0x117
	BYTE	btorso;					//0x118
	BYTE	blegs;					//0x119
	BYTE	brspad;					//0x11A
	BYTE	blspad;					//0x11B
	BYTE	b2handed;				//0x11C
	BYTE	buseable;				//0x11D
	WORD	wtype;					//0x11E
	WORD	wtype2;					//0x120
	WORD	bsubtype;				//0x122
	WORD	wdropsound;				//0x124
	WORD	wusesound;				//0x126
	BYTE	bdropsfxframe;			//0x128
	BYTE	bunique;				//0x129
	BYTE	bquest;					//0x12A
	BYTE	bquestdiffcheck;		//0x12B
	BYTE	btransparent;			//0x12C
	BYTE	btranstbl;				//0x12D
	BYTE 	_1;						//0x12E
	BYTE	blightradius;			//0x12F
	BYTE	bbelt;					//0x130
	BYTE	bautobelt;				//0x131
	BYTE	bstackable;				//0x132
	BYTE	bspawnable;				//0x133
	BYTE	bspellicon;				//0x134
	BYTE	bdurwarning;			//0x135
	BYTE	bqntwarning;			//0x136
	BYTE	bhasinv;				//0x137
	BYTE	bgemsockets;			//0x138
	BYTE	bTransmogrify;			//0x139
	BYTE	bTMogMin;				//0x13A
	BYTE	bTMogMax;				//0x13B
	BYTE	bhitclass;				//0x13C
	BYTE	b1or2handed;			//0x13D
	BYTE	bgemapplytype;			//0x13E
	BYTE	blevelreq;				//0x13F
	BYTE	bmagiclvl;				//0x140
	BYTE	bTransform;				//0x141
	BYTE	bInvTrans;				//0x142
	BYTE	bcompactsave;			//0x143
	BYTE	bSkipName;				//0x144
	BYTE	bNameable;				//0x145
	BYTE	bAkaraMin;				//0x146
	BYTE	bGheedMin;				//0x147
	BYTE	bCharsiMin;				//0x148
	BYTE	bFaraMin;				//0x149
	BYTE	bLysanderMin;			//0x14A
	BYTE	bDrognanMin;			//0x14B
	BYTE	bHraltiMin;				//0x14C
	BYTE	bAlkorMin;				//0x14D
	BYTE	bOrmusMin;				//0x14E
	BYTE	bElzixMin;				//0x14F
	BYTE	bAshearaMin;			//0x150
	BYTE	bCainMin;				//0x151
	BYTE	bHalbuMin;				//0x152
	BYTE	bJamellaMin;			//0x153
	BYTE	bMalahMin;				//0x154
	BYTE	bLarzukMin;				//0x155
	BYTE	bDrehyaMin;				//0x156
	BYTE	bAkaraMax;				//0x157
	BYTE	bGheedMax;				//0x158
	BYTE	bCharsiMax;				//0x159
	BYTE	bFaraMax;				//0x15A
	BYTE	bLysanderMax;			//0x15B
	BYTE	bDrognanMax;			//0x15C
	BYTE	bHraltiMax;				//0x15D
	BYTE	bAlkorMax;				//0x15E
	BYTE	bOrmusMax;				//0x15F
	BYTE	bElzixMax;				//0x160
	BYTE	bAshearaMax;			//0x161
	BYTE	bCainMax;				//0x162
	BYTE	bHalbuMax;				//0x163
	BYTE	bJamellaMax;			//0x164
	BYTE	bMalahMax;				//0x165
	BYTE	bLarzukMax;				//0x166
	BYTE	bDrehyaMax;				//0x167
	BYTE	bAkaraMagicMin;			//0x168
	BYTE	bGheedMagicMin;			//0x169
	BYTE	bCharsiMagicMin;		//0x16A
	BYTE	bFaraMagicMin;			//0x16B
	BYTE	bLysanderMagicMin;		//0x16C
	BYTE	bDrognanMagicMin;		//0x16D
	BYTE	bHraltiMagicMin;		//0x16E
	BYTE	bAlkorMagicMin;			//0x16F
	BYTE	bOrmusMagicMin;			//0x170
	BYTE	bElzixMagicMin;			//0x171
	BYTE	bAshearaMagicMin;		//0x172
	BYTE	bCainMagicMin;			//0x173
	BYTE	bHalbuMagicMin;			//0x174
	BYTE	bJamellaMagicMin;		//0x175
	BYTE	bMalahMagicMin;			//0x176
	BYTE	bLarzukMagicMin;		//0x177
	BYTE	bDrehyaMagicMin;		//0x178
	BYTE	bAkaraMagicMax;			//0x179
	BYTE	bGheedMagicMax;			//0x17A
	BYTE	bCharsiMagicMax;		//0x17B
	BYTE	bFaraMagicMax;			//0x17C
	BYTE	bLysanderMagicMax;		//0x17D
	BYTE	bDrognanMagicMax;		//0x17E
	BYTE	bHraltiMagicMax;		//0x17F
	BYTE	bAlkorMagicMax;			//0x180
	BYTE	bOrmusMagicMax;			//0x181
	BYTE	bElzixMagicMax;			//0x182
	BYTE	bAshearaMagicMax;		//0x183
	BYTE	bCainMagicMax;			//0x184
	BYTE	bHalbuMagicMax;			//0x185
	BYTE	bJamellaMagicMax;		//0x186
	BYTE	bMalahMagicMax;			//0x187
	BYTE	bLarzukMagicMax;		//0x188
	BYTE	bDrehyaMagicMax;		//0x189
	BYTE	bAkaraMagicLvl;			//0x18A
	BYTE	bGheedMagicLvl;			//0x18B
	BYTE	bCharsiMagicLvl;		//0x18C
	BYTE	bFaraMagicLvl;			//0x18D
	BYTE	bLysanderMagicLvl;		//0x18E
	BYTE	bDrognanMagicLvl;		//0x18F
	BYTE	bHraltiMagicLvl;		//0x190
	BYTE	bAlkorMagicLvl;			//0x191
	BYTE	bOrmusMagicLvl;			//0x192
	BYTE	bElzixMagicLvl;			//0x193
	BYTE	bAshearaMagicLvl;		//0x194
	BYTE	bCainMagicLvl;			//0x195
	BYTE	bHalbuMagicLvl;			//0x196
	BYTE	bJamellaMagicLvl;		//0x197
	BYTE	bMalahMagicLvl;			//0x198
	BYTE	bLarzukMagicLvl;		//0x199
	WORD	bDrehyaMagicLvl;		//0x19A
	DWORD	dwNightmareUpgrade;		//0x19C
	DWORD	dwHellUpgrade;			//0x1A0
	BYTE	bPermStoreItem;			//0x1A4
	BYTE	bmultibuy;				//0x1A5
	WORD   _ALIGN;					//0x1A6
};

struct sgptDataTable {
	BYTE*	pPlayerClass;			//0x00
	DWORD	dwPlayerClassRecords;	//0x04
	BodyLocsTxt*	pBodyLocs;		//0x08
	DWORD	dwBodyLocsRecords;		//0x0C
	BYTE*	pStorePage;				//0x10
	DWORD	dwStorePageRecords;		//0x14
	BYTE*	pElemTypes;				//0x18
	DWORD	dwElemTypesRecords;		//0x1C
	BYTE*	pHitClass;				//0x20
	DWORD	dwHitClassRecords;		//0x24
	BYTE*	pMonMode;				//0x28
	DWORD	dwMonModeTxt;			//0x2C
	BYTE*	pPlrMode;				//0x30
	DWORD	dwPlrModeRecords;		//0x34
	BYTE*	pSkillCalc;				//0x38
	DWORD	dwSkillCalcRecords;		//0x3C
	BYTE*	pSkillCalcCache;		//0x40
	DWORD	dwSkillCalcCacheRecs;	//0x44
	DWORD	dwSkillCalcCacheDelta;	//0x48
	BYTE*	pSkillDescCalcCache;	//0x4C
	DWORD	dwSkillDescCalcCacheRecs;//0x50
	DWORD	dwSkillDescCalcCacheDelta;//0x54
	BYTE*	pMissCalc;				//0x58
	DWORD	dwMissCalcRecords;		//0x5C
	BYTE*	pMissCalcCache;			//0x60
	DWORD	dwMissCalcCacheRecs;	//0x64
	DWORD	dwMissCalcCacheDelta;	//0x68
	BYTE*	pSkillCodes;			//0x6C
	DWORD	dwSkillCodesRecs;		//0x70
	BYTE*	pEvents;				//0x74
	DWORD	dwEventsRecs;			//0x78
	BYTE*	pCompCodes;				//0x7C
	DWORD	dwCompCodesRecs;		//0x80
	DWORD	dwCompCodes;			//0x84
	BYTE*	pMonAi;					//0x88
	DWORD	dwMonAiRecs;			//0x8C
	DWORD	dwMonAi;				//0x90
	BYTE*	pItem;					//0x94
	BYTE*	pItemCalcCache;			//0x98
	DWORD	dwItemCalcCacheRecs;	//0x9C
	DWORD	dwItemCalcCache;		//0xA0
	PropertiesTxt*	pPropertiesTxt;	//0xA4
	BYTE*	dwProperties;			//0xA8
	DWORD	dwProportiesRecs;		//0xAC
	BYTE*	pRunes;					//0xB0
	BYTE*	pHireDescs;				//0xB4
	DWORD	dwHireDescsRecs;		//0xB8
	StatesTxt*	pStatesTxt;			//0xBC
	DWORD	dwStatesTxtRecs;		//0xC0
	DWORD	dwStates;				//0xC4
	BYTE*	pStateMaskFirst;		//0xC8
	BYTE*	pStateMaskArr[40];		//0xCC
	BYTE*	pProgressiveStates;		//0x16C
	DWORD	dwProgressiveStatesRecs;//0x170
	BYTE*	pCurseStates;			//0x174
	DWORD	dwCurseStatesRecs;		//0x178
	BYTE*	pTransStates;			//0x17C
	DWORD	dwTransStatesRecs;		//0x180
	BYTE*	pActionStates;			//0x184
	DWORD	dwActionStatesRecs;		//0x188
	BYTE*	pColorStates;			//0x18C
	DWORD	dwColorStatesRecs;		//0x190
	BYTE*	pSoundCodes;			//0x194
	BYTE*	pSoundCodesTxt;			//0x198
	DWORD	dwSoundCodesRecs;		//0x19C
	BYTE*	pHirelings;				//0x1A0
	DWORD	dwHirelingsRecs;		//0x1A4
	DWORD	HireNameStart[256];		//0x1A8
	DWORD	HireNameEnd[256];		//0x5A8
	BYTE*	pNpcs;					//0x9A8
	DWORD	dwNpcsRecs;				//0x9AC
	BYTE*	pColorsTxt;				//0x9B0
	BYTE*	pColors;				//0x9B4
	BYTE*	pTreasureClassEx;		//0x9B8
	BYTE*	pTreasureClassExTxt;	//0x9BC
	DWORD	dwTreasureClassExRecs;	//0x9C0
	BYTE*	pChestTreasureClassLst[45];	//0x9C4
	MonStatsTxt* pMonStatsTxt;		//0xA78
	BYTE*	pMonStats;				//0xA7C
	DWORD	dwMonStatsRecs;			//0xA80
	BYTE*	pMonSoundsTxt;			//0xA84
	BYTE*	pMonSounds;				//0xA88
	DWORD	dwMonSoundsRecs;		//0xA8C
	BYTE*	pMonStats2Txt;			//0xA90
	BYTE*	pMonStats2;				//0xA94
	DWORD	dwMonStats2Recs;		//0xA98
	BYTE*	pMonPlaceTxt;			//0xA9C
	BYTE*	pMonPlace;				//0xAA0
	DWORD	dwMonPlaceRecs;			//0xAA4
	BYTE*	pMonPresetTxt;			//0xAA8
	BYTE*	pMonPresetActLst[5];	//0xAAC
	DWORD	dwMonPresetActRecs[5];	//0xAC0
	BYTE*	pSuperUniquesTxt;		//0xAD4
	BYTE*	pSuperUniques;			//0xAD8
	DWORD	dwSuperUniquesRecs;		//0xADC
	WORD	SuperUniqeIdxList[66];	//0xAE0
	MissilesTxt*	pMissilesTxt;	//0xB64
	BYTE*	pMissiles;				//0xB68
	DWORD	dwMissilesRecs;			//0xB6C
	BYTE*	pMonLvl;				//0xB70
	DWORD	dwMonLvlRecs;			//0xB74
	BYTE*	pMonSeqTxt;				//0xB78
	BYTE*	pMonSeq;				//0xB7C
	DWORD	dwMonSeqRecs;			//0xB80
	BYTE*	pMonSequences;			//0xB84
	DWORD	dwMonSequencesRecs;		//0xB88
	SkillDescTxt*	pSkillDescTxt;	//0xB8C
	BYTE*	pSkillDesc;				//0xB90
	DWORD	dwSkillDescRecs;		//0xB94
	SkillsTxt*pSkillsTxt;			//0xB98
	BYTE*	pSkills;				//0xB9C
	DWORD	dwSkillsRecs;			//0xBA0
	BYTE*	pPlayerSkillCount;		//0xBA4
	DWORD	dwPlayerSkillCount;		//0xBA8
	BYTE*	pPlayerSkillList;		//0xBAC
	DWORD	dwPassiveSkillCount;	//0xBB0
	BYTE*	pPasssiveSkillList;		//0xBB4
	BYTE*	pOverlayTxt;			//0xBB8
	BYTE*	pOverlay;				//0xBBC
	DWORD	dwOverlayRecs;			//0xBC0
	CharStatsTxt*	pCharStatsTxt;	//0xBC4
	DWORD	dwCharsStatsRecs;		//0xBC8
	ItemStatCostTxt*pItemStatCostTxt;//0xBCC
	BYTE*	pItemStatCost;			//0xBD0
	DWORD	dwItemStatCostRecs;		//0xBD4
	BYTE*	pOpStatNesting;			//0xBD8
	DWORD	dwOpStatNestingRecs;	//0xBDC
	BYTE*	pMonEquip;				//0xBE0
	DWORD	dwMonEquipRecs;			//0xBE4
	BYTE*	pPetTypesTxt;			//0xBE8
	BYTE*	pPetTypes;				//0xBEC
	DWORD	dwPetTypesRecs;			//0xBF0
	BYTE*	pItemsType;				//0xBF4
	BYTE*	pItemsTypeTxt;			//0xBF8
	DWORD	dwItemsTypeRecs;		//0xBFC
	DWORD	dwItemsTypeNesting;		//0xC00
	BYTE*	pItemsTypeNesting;		//0xC04
	BYTE*	pSets;					//0xC08
	BYTE*	pSetsTxt;				//0xC0C
	DWORD	dwSetsRecs;				//0xC10
	BYTE*	pSetItems;				//0xC14
	SetItemsTxt* pSetItemsTxt;		//0xC18
	DWORD	dwSetItemsRecs;			//0xC1C
	BYTE*	pUniqueItems;			//0xC20
	UniqueItemsTxt*	pUniqueItemsTxt;//0xC24
	DWORD	dwUniqItemsRecs;		//0xC28
	BYTE*	pMonProp;				//0xC2C
	BYTE*	pMonPropTxt;			//0xC30
	DWORD	dwMonPropRecs;			//0xC34
	BYTE*	pMonType;				//0xC38
	BYTE*	pMonTypeTxt;			//0xC3C
	DWORD	dwMonTypeRecs;			//0xC40
	BYTE*	pMonTypeNesting;		//0xC44
	DWORD	dwMonTypeNestingRecs;	//0xC48
	BYTE*	pMonUMod;				//0xC4C
	BYTE*	pMonUModTxt;			//0xC50
	DWORD	dwMonUModRecs;			//0xC54
	LevelsTxt*	pLevelsTxt;			//0xC58
	DWORD	dwLevelsRecs;			//0xC5C
	BYTE*	pLvlDefs;				//0xC60
	BYTE*	pLvlPrest;				//0xC64
	DWORD	dwLvlPrestRecs;			//0xC68
	DWORD	ItemStatCostStuff[2];	//0xC6C
	BYTE*	pAnimTables;			//0xC74
	BYTE*	pExperience;			//0xC78
	DifficultyLevelsTxt*pDiffLvlsTxt;//0xC7C
	DWORD	dwDiffLvlsRecs;			//0xC80
	DWORD	_1;						//0xC84
	BYTE*	pCharTemp;				//0xC88
	DWORD	dwCharTempRecs;			//0xC8C
	ArenaTxt*pArena;				//0xC90
	CubeMainTxt*	pCubeMain;		//0xC94
	DWORD	dwCubeMainRecs;			//0xC98
	DWORD   _2;						//0xC9C
	BYTE*	pLvlSubExtra;			//0xCA0
	BYTE*	pExpFieldD2;			//0xCA4
	DWORD   ExpFieldData[5];		//0xCA8
	BYTE*	pLvlTypes;				//0xCBC
	BYTE*   pWaypoints;				//0xCC0
	DWORD	dwWaypointsRecs;		//0xCC4
	DWORD	dwLvlTypes;				//0xCC8
	BYTE*	pLvlWarp;				//0xCCC
	DWORD	dwLvlWarpRecs;			//0xCD0
	LvlMazeTxt*	pLvlMaze;			//0xCD4
	DWORD	dwLvlMazeRecs;			//0xCD8
	BYTE*	pLvlSub;				//0xCDC
	DWORD	dwLvlSubRecs;			//0xCE0
	DWORD   pLvlSubCache;			//0xCE4 aka sgpnTileSubsTypeStart
	DWORD	_5[5];					//0xCE8
	BYTE*	pMapCache;				//0xCFC
	DWORD	dwMapCacheRecs;			//0xD00
	DWORD	_6[2];					//0xD04
	DWORD*  pIgnoreList;            //0xD0C
};

#endif
