#pragma once

#define BH_VERSION "BH 1.9.10-b3"

#define CODE_PAGE 1252 // windows-1252	ANSI Latin 1; Western European (Windows)

enum TextColor {
	Disabled=-1,
	White=0,
	Red,
	Green,
	Blue,
	Gold,
	Grey,
	Black,
	Tan,
	Orange,
	Yellow,
	DarkGreen,
	Purple,
	Silver=15
};

//////////////////////////////////////////////////////////////////////
// Attack Types
//////////////////////////////////////////////////////////////////////
#define ATTACKTYPE_SHIFTLEFT		0xE5
#define ATTACKTYPE_UNITLEFT			0xC5
#define ATTACKTYPE_SHIFTRIGHT		0x66
#define ATTACKTYPE_RIGHT			0x46

///////////////////////////////////////////////////
// Game UI Flags
///////////////////////////////////////////////////
#define	UI_GAME					0x00
#define UI_INVENTORY			0x01
#define UI_CHARACTER			0x02
#define UI_MINISKILL			0x03
#define UI_SKILLTREE			0x04
#define UI_CHAT_CONSOLE			0x05
#define UI_UNK					0x06   //auto disabled
#define UI_UNK_EX				0x07   //auto disabled
#define UI_NPCMENU				0x08	
#define UI_DIALOG				0x08
#define UI_ESCMENU_MAIN			0x09
#define UI_AUTOMAP				0x0A
#define UI_HOTKEY_CONFIG		0x0B
#define UI_NPCSHOP				0x0C
#define UI_ALTDOWN				0x0D
#define UI_GROUND_ITEMS			0x0D
#define UI_ANVIL				0x0E
#define UI_QUEST				0x0F
#define UI_BARKSCROLL			0x10
#define UI_QUEST_LOG			0x11
#define UI_STATUS_AREA			0x12
#define UI_ESCMENU_EX			0x13
#define UI_WPMENU				0x14 //Waypoints Menu
#define UI_MINIPANEL			0x15
#define UI_PARTY				0x16
#define UI_TRADE				0x17
#define UI_MESSAGE_LOG			0x18
#define UI_STASH				0x19
#define UI_CUBE					0x1A
#define UI_UNUSED				0x1B
#define UI_INVENTORY_EX			0x1C
#define UI_INVENTORY_EXX		0x1D
#define UI_UNUSED_EX			0x1E
#define UI_BELT					0x1F
#define UI_UNUSED_EXX			0x20
#define UI_HELP_MENU			0x21
#define UI_HELP_BUTTON			0x22
#define UI_ESCMENU				0x23 //unused??
#define UI_MERC					0x24
#define UI_SCROLL				0x25

///////////////////////////////////////////////////
// Control types
///////////////////////////////////////////////////
#define CONTROL_EDITBOX			0x01
#define CONTROL_IMAGE			0x02
#define CONTROL_UNUSED			0x03
#define CONTROL_TEXTBOX			0x04
#define CONTROL_SCROLLBAR		0x05
#define CONTROL_BUTTON			0x06
#define CONTROL_LIST			0x07
#define CONTROL_TIMER			0x08
#define CONTROL_SMACK			0x09
#define CONTROL_PROGRESSBAR		0x0a
#define CONTROL_POPUP			0x0b
#define CONTROL_ACCOUNTLIST		0x0c

//////////////////////////////////////////////////////////////////////
// PVP Flags
//////////////////////////////////////////////////////////////////////
#define PVP_INVITED_YOU			0x01 // Invited you, but you may not have accepted
#define PVP_INVITED_BY_YOU		0x02 // Invited by you, but he may not have accepted
#define PVP_ALLIED				0x04 // Allied
#define PVP_HOSTILED_YOU		0x08 // Hostiled you, but you may not have hostiled him
#define PVP_HOSTILED_BY_YOU		0x10 // Hostiled by you, but he may not have hostiled you

//////////////////////////////////////////////////////////////////////////
// Player Type Flags
//////////////////////////////////////////////////////////////////////////
#define PLAYER_TYPE_HARDCORE	4
#define PLAYER_TYPE_DEAD		8
#define PLAYER_TYPE_EXPANSION	32
#define PLAYER_TYPE_LADDER		64

//////////////////////////////////////////////////////////////////////////
// Resistance Penalties
//////////////////////////////////////////////////////////////////////////
#define RES_PENALTY_CLS_NORM		0
#define RES_PENALTY_CLS_NM			-20
#define RES_PENALTY_CLS_HELL		-50
#define RES_PENALTY_XPAC_NORM		(2*RES_PENALTY_CLS_NORM)
#define RES_PENALTY_XPAC_NM			(2*RES_PENALTY_CLS_NM)
#define RES_PENALTY_XPAC_HELL		(2*RES_PENALTY_CLS_HELL)

//////////////////////////////////////////////////////////////////////////
// Trade Flags
//////////////////////////////////////////////////////////////////////////
#define TRADE_OPEN				3
#define TRADE_ACCEPTED			5
#define TRADE_ACCEPT			7

///////////////////////////////////////////////////
// Mercenary Class ID's
///////////////////////////////////////////////////
#define MERC_A1		0x010f
#define MERC_A2		0x0152
#define MERC_A3		0x0167
#define MERC_A5		0x0231

///////////////////////////////////////////////////
// Unit Stat ID Definition
// (Completed using the heroin_glands D2 library)
///////////////////////////////////////////////////
#define STAT_STRENGTH				0		// str
#define	STAT_ENERGY					1		// energy
#define	STAT_DEXTERITY				2		// dexterity
#define	STAT_VITALITY				3		// vitality
#define STAT_STATPOINTSLEFT			4
#define STAT_NEWSKILLS				5
#define	STAT_HP						6		// life
#define	STAT_MAXHP					7		// max life
#define	STAT_MANA					8		// mana
#define	STAT_MAXMANA				9		// max mana
#define	STAT_STAMINA				10		// stamina
#define	STAT_MAXSTAMINA				11		// max stamina
#define	STAT_LEVEL					12		// level
#define STAT_EXP					13		// experience
#define	STAT_GOLD					14		// gold
#define	STAT_GOLDBANK				15		// stash gold
#define STAT_ENHANCEDDEFENSE		16
#define STAT_ENHANCEDMAXIMUMDAMAGE	17
#define STAT_ENHANCEDMINIMUMDAMAGE	18
#define STAT_ATTACKRATING			19
#define	STAT_TOBLOCK				20		// to block
#define STAT_MINIMUMDAMAGE			21
#define STAT_MAXIMUMDAMAGE			22
#define STAT_SECONDARYMINIMUMDAMAGE	23
#define STAT_SECONDARYMAXIMUMDAMAGE	24
#define STAT_ENHANCEDDAMAGE			25
#define STAT_MANARECOVERY			26
#define STAT_MANARECOVERYBONUS		27
#define STAT_STAMINARECOVERYBONUS	28
#define STAT_LASTEXPERIENCE			29
#define STAT_NEXTEXPERIENCE			30
#define STAT_DEFENSE				31
#define STAT_DEFENSEVSMISSILES		32
#define STAT_DEFENSEVSMELEE			33
#define STAT_DMGREDUCTION			34
#define	STAT_MAGICDMGREDUCTION		35		// magic damage reduction
#define	STAT_DMGREDUCTIONPCT		36		// damage reduction
#define	STAT_MAGICDMGREDUCTIONPCT	37		// magic damage reduction percentage
#define	STAT_MAXMAGICDMGREDUCTPCT	38		// max magic damage reduction percentage
#define	STAT_FIRERESIST				39		// fire resist
#define	STAT_MAXFIRERESIST			40		// max fire resist
#define	STAT_LIGHTNINGRESIST		41		// lightning resist
#define	STAT_MAXLIGHTNINGRESIST		42		// max lightning resist
#define	STAT_COLDRESIST				43		// cold resist
#define	STAT_MAXCOLDRESIST			44		// max cold resist
#define	STAT_POISONRESIST			45		// poison resist
#define	STAT_MAXPOISONRESIST		46		// max poison resist
#define STAT_DAMAGEAURA				47
#define STAT_MINIMUMFIREDAMAGE		48
#define STAT_MAXIMUMFIREDAMAGE		49
#define STAT_MINIMUMLIGHTNINGDAMAGE	50
#define STAT_MAXIMUMLIGHTNINGDAMAGE	51
#define STAT_MINIMUMMAGICALDAMAGE	52
#define STAT_MAXIMUMMAGICALDAMAGE	53
#define STAT_MINIMUMCOLDDAMAGE		54
#define STAT_MAXIMUMCOLDDAMAGE		55
#define STAT_COLDDAMAGELENGTH		56
#define STAT_MINIMUMPOISONDAMAGE	57
#define STAT_MAXIMUMPOISONDAMAGE	58
#define STAT_POISONDAMAGELENGTH		59
#define STAT_LIFELEECH				60		// Life Leech (min life stolen per hit)
#define STAT_MAXLIFESTOLENPERHIT	61
#define STAT_MANALEECH				62		// Mana Leech (min mana stolen per hit)
#define STAT_MAXMANASTOLENPERHIT	63
#define STAT_MINIMUMSTAMINADRAIN	64
#define STAT_MAXIMUMSTAMINADRAIN	65
#define STAT_STUNLENGTH				66
#define	STAT_VELOCITYPERCENT		67		// effective run/walk
#define STAT_ATTACKRATE				68
#define STAT_OTHERANIMATIONRATE		69
#define STAT_AMMOQUANTITY			70		// ammo quantity(arrow/bolt/throwing)
#define STAT_VALUE					71
#define STAT_DURABILITY				72		// item durability
#define STAT_MAXDURABILITY			73		// max item durability
#define STAT_REPLENISHLIFE			74
#define STAT_ENHANCEDMAXDURABILITY	75
#define STAT_ENHANCEDLIFE			76
#define STAT_ENHANCEDMANA			77
#define STAT_ATTACKERTAKESDAMAGE	78
#define STAT_GOLDFIND				79		// Gold find (GF)
#define	STAT_MAGICFIND				80		// magic find (MF)
#define STAT_KNOCKBACK				81
#define STAT_TIMEDURATION			82
#define STAT_CLASSSKILLS			83
#define STAT_UNSENTPARAMETER		84
#define STAT_ADDEXPERIENCE			85
#define STAT_LIFEAFTEREACHKILL		86
#define STAT_REDUCEVENDORPRICES		87
#define STAT_DOUBLEHERBDURATION		88
#define STAT_LIGHTRADIUS			89
#define STAT_LIGHTCOLOUR			90
#define STAT_REDUCEDREQUIREMENTS	91
#define STAT_REDUCEDLEVELREQ		92
#define	STAT_IAS					93		// IAS
#define STAT_REDUCEDLEVELREQPCT		94
#define STAT_LASTBLOCKFRAME			95
#define	STAT_FASTERRUNWALK			96		// faster run/walk
#define STAT_NONCLASSSKILL			97
#define STAT_STATE					98
#define	STAT_FASTERHITRECOVERY		99		// faster hit recovery
#define STAT_MONSTERPLAYERCOUNT		100
#define STAT_SKILLPOISONOVERRIDELEN	101
#define	STAT_FASTERBLOCK			102		// faster block rate
#define STAT_SKILLBYPASSUNDEAD		103
#define STAT_SKILLBYPASSDEMONS		104
#define	STAT_FASTERCAST				105		// faster cast rate
#define STAT_SKILLBYPASSBEASTS		106
#define STAT_SINGLESKILL			107
#define STAT_SLAINMONSTERSRIP		108
#define STAT_CURSERESISTANCE		109
#define STAT_POISONLENGTHREDUCTION	110		// Poison length reduction
#define STAT_ADDSDAMAGE				111
#define STAT_HITCAUSESMONSTERTOFLEE	112
#define STAT_HITBLINDSTARGET		113
#define STAT_DAMAGETOMANA			114
#define STAT_IGNORETARGETSDEFENSE	115
#define STAT_REDUCETARGETSDEFENSE	116
#define STAT_PREVENTMONSTERHEAL		117
#define STAT_HALFFREEZEDURATION		118
#define STAT_TOHITPERCENT			119
#define STAT_MONSTERDEFDUCTPERHIT	120
#define STAT_DAMAGETODEMONS			121
#define STAT_DAMAGETOUNDEAD			122
#define STAT_ATTACKRATINGVSDEMONS	123
#define STAT_ATTACKRATINGVSUNDEAD	124
#define STAT_THROWABLE				125
#define STAT_ELEMENTALSKILLS		126
#define STAT_ALLSKILLS				127
#define STAT_ATTACKERTAKESLTNGDMG	128
#define STAT_IRONMAIDENLEVEL		129
#define STAT_LIFETAPLEVEL			130
#define STAT_THORNSPERCENT			131
#define STAT_BONEARMOR				132
#define STAT_MAXIMUMBONEARMOR		133
#define STAT_FREEZESTARGET			134
#define STAT_OPENWOUNDS				135		// Open Wounds
#define	STAT_CRUSHINGBLOW			136		// crushing blow
#define STAT_KICKDAMAGE				137
#define STAT_MANAAFTEREACHKILL		138
#define STAT_LIFEAFTEREACHDEMONKILL	139
#define STAT_EXTRABLOOD				140
#define	STAT_DEADLYSTRIKE			141		// deadly strike
#define	STAT_FIREABSORBPERCENT		142		// fire absorb %
#define	STAT_FIREABSORB				143		// fire absorb
#define	STAT_LIGHTNINGABSORBPERCENT	144		// lightning absorb %
#define	STAT_LIGHTNINGABSORB		145		// lightning absorb
#define	STAT_MAGICABSORBPERCENT		146		// magic absorb %
#define STAT_MAGICABSORB			147
#define	STAT_COLDABSORBPERCENT		148		// cold absorb %
#define	STAT_COLDABSORB				149		// cold absorb
#define STAT_SLOW					150		// slow %
#define STAT_AURA					151
#define STAT_INDESTRUCTIBLE			152
#define STAT_CANNOTBEFROZEN			153
#define STAT_STAMINADRAINPERCENT	154
#define STAT_REANIMATE				155
#define STAT_PIERCINGATTACK			156
#define STAT_FIRESMAGICARROWS		157
#define STAT_FIREEXPLOSIVEARROWS	158
#define STAT_MINIMUMTHROWINGDAMAGE	159
#define STAT_MAXIMUMTHROWINGDAMAGE	160
#define STAT_SKILLHANDOFATHENA		161
#define STAT_SKILLSTAMINAPERCENT	162
#define STAT_SKILLPASSIVESTAMINAPCT	163
#define STAT_CONCENTRATION			164
#define STAT_ENCHANT				165
#define STAT_PIERCE					166
#define STAT_CONVICTION				167
#define STAT_CHILLINGARMOR			168
#define STAT_FRENZY					169
#define STAT_DECREPIFY				170
#define STAT_SKILLARMORPERCENT		171
#define STAT_ALIGNMENT				172
#define STAT_TARGET0				173
#define STAT_TARGET1				174
#define STAT_GOLDLOST				175
#define STAT_CONVERSIONLEVEL		176
#define STAT_CONVERSIONMAXIMUMLIFE	177
#define STAT_UNITDOOVERLAY			178
#define STAT_ATTCKRTNGVSMONSTERTYPE	179
#define STAT_DAMAGETOMONSTERTYPE	180
#define STAT_FADE					181
#define STAT_ARMOROVERRIDEPERCENT	182
#define STAT_UNUSED183				183
#define STAT_UNUSED184				184
#define STAT_UNUSED185				185
#define STAT_UNUSED186				186
#define STAT_UNUSED187				187
#define STAT_SKILLTAB				188
#define STAT_UNUSED189				189
#define STAT_UNUSED190				190
#define STAT_UNUSED191				191
#define STAT_UNUSED192				192
#define STAT_UNUSED193				193
#define STAT_SOCKETS				194
#define STAT_SKILLONSTRIKING		195
#define STAT_SKILLONKILL			196
#define STAT_SKILLONDEATH			197
#define STAT_SKILLONHIT				198
#define STAT_SKILLONLEVELUP			199
#define STAT_UNUSED200				200
#define STAT_SKILLWHENSTRUCK		201
#define STAT_UNUSED202				202
#define STAT_UNUSED203				203
#define STAT_CHARGED				204
#define STAT_UNUSED204				205
#define STAT_UNUSED205				206
#define STAT_UNUSED206				207
#define STAT_UNUSED207				208
#define STAT_UNUSED208				209
#define STAT_UNUSED209				210
#define STAT_UNUSED210				211
#define STAT_UNUSED211				212
#define STAT_UNUSED212				213
#define STAT_DEFENSEPERLEVEL		214
#define STAT_ENHANCEDDEFENSEPERLVL	215
#define STAT_LIFEPERLEVEL			216
#define STAT_MANAPERLEVEL			217
#define STAT_MAXDAMAGEPERLEVEL		218
#define STAT_MAXENHANCEDDMGPERLEVEL	219
#define STAT_STRENGTHPERLEVEL		220
#define STAT_DEXTERITYPERLEVEL		221
#define STAT_ENERGYPERLEVEL			222
#define STAT_VITALITYPERLEVEL		223
#define STAT_ATTACKRATINGPERLEVEL	224
#define STAT_BONUSATTCKRTNGPERLEVEL	225
#define STAT_MAXCOLDDMGPERLVL		226
#define STAT_MAXFIREDMGPERLVL		227
#define STAT_MAXLIGHTNINGDMGPERLVL	228
#define STAT_MAXPOISONDMGPERLVL		229
#define STAT_COLDRESPERLEVEL		230
#define STAT_FIRERESPERLEVEL		231
#define STAT_LIGHTNINGRESPERLEVEL	232
#define STAT_POISONRESPERLEVEL		233
#define STAT_COLDABSORBPERLVL		234
#define STAT_FIREABSORBPERLVL		235
#define STAT_LIGHTNINGABSORBPERLVL	236
#define STAT_POISONABSORBPERLVL		237
#define STAT_THORNSPERLEVEL			238
#define STAT_EXTRAGOLDPERLEVEL		239
#define STAT_MAGICFINDPERLEVEL		240
#define STAT_STAMINAREGENPERLEVEL	241
#define STAT_STAMINAPERLEVEL		242
#define STAT_DAMAGETODEMONSPERLEVEL	243
#define STAT_DAMAGETOUNDEADPERLEVEL	244
#define STAT_ATTKRTNGVSDEMONSPERLVL	245
#define STAT_ATTKRTNGVSUNDEADPERLVL	246
#define STAT_CRUSHINGBLOWPERLEVEL	247
#define STAT_OPENWOUNDSPERLEVEL		248
#define STAT_KICKDAMAGEPERLEVEL		249
#define STAT_DEADLYSTRIKEPERLEVEL	250
#define STAT_FINDGEMSPERLEVEL		251
#define STAT_REPAIRSDURABILITY		252
#define STAT_REPLENISHESQUANTITY	253
#define STAT_INCREASEDSTACKSIZE		254
#define STAT_FINDITEM				255
#define STAT_SLASHDAMAGE			256
#define STAT_SLASHDAMAGEPERCENT		257
#define STAT_CRUSHDAMAGE			258
#define STAT_CRUSHDAMAGEPERCENT		259
#define STAT_THRUSTDAMAGE			260
#define STAT_THRUSTDAMAGEPERCENT	261
#define STAT_SLASHDAMAGEABSORPTION	262
#define STAT_CRUSHDAMAGEABSORPTION	263
#define STAT_THRUSTDAMAGEABSORPTION	264
#define STAT_SLASHDAMAGEABSORBPCT	265
#define STAT_CRUSHDAMAGEABSORBPCT	266
#define STAT_THRUSTDAMAGEABSORBPCT	267
#define STAT_DEFENSEPERTIME			268
#define STAT_ENHANCEDDEFENSEPERTIME	269
#define STAT_LIFEPERTIME			270
#define STAT_MANAPERTIME			271
#define STAT_MAXDAMAGEPERTIME		272
#define STAT_MAXENHANCEDDMGPERTIME	273
#define STAT_STRENGTHPERTIME		274
#define STAT_DEXTERITYPERTIME		275
#define STAT_ENERGYPERTIME			276
#define STAT_VITALITYPERTIME		277
#define STAT_ATTACKRATINGPERTIME	278
#define STAT_CHANCETOHITPERTIME		279
#define STAT_MAXCOLDDAMAGEPERTIME	280
#define STAT_MAXFIREDAMAGEPERTIME	281
#define STAT_MAXLIGHTNINGDMGPERTIME	282
#define STAT_MAXDAMAGEPERPOISON		283
#define STAT_COLDRESPERTIME			284
#define STAT_FIRERESPERTIME			285
#define STAT_LIGHTNINGRESPERTIME	286
#define STAT_POISONRESPERTIME		287
#define STAT_COLDABSORPTIONPERTIME	288
#define STAT_FIREABSORPTIONPERTIME	289
#define STAT_LIGHTNINGABSORBPERTIME	290
#define STAT_POISONABSORBPERTIME	291
#define STAT_EXTRAGOLDPERTIME		292
#define STAT_MAGICFINDPERTIME		293
#define STAT_REGENSTAMINAPERTIME	294
#define STAT_STAMINAPERTIME			295
#define STAT_DAMAGETODEMONSPERTIME	296
#define STAT_DAMAGETOUNDEADPERTIME	297
#define STAT_ATTRTNGVSDEMONSPERTIME	298
#define STAT_ATTRTNGVSUNDEADPERTIME	299
#define STAT_CRUSHINGBLOWPERTIME	300
#define STAT_OPENWOUNDSPERTIME		301
#define STAT_KICKDAMAGEPERTIME		302
#define STAT_DEADLYSTRIKEPERTIME	303
#define STAT_FINDGEMSPERTIME		304
#define STAT_ENEMYCOLDRESREDUCTION	305
#define STAT_ENEMYFIRERESREDUCTION	306
#define STAT_ENEMYLIGHTRESREDUCTION	307
#define STAT_ENEMYPSNRESREDUCTION	308
#define STAT_DAMAGEVSMONSTERS		309
#define STAT_ENHANCEDDMGVSMONSTERS	310
#define STAT_ATTACKRATINGVSMONSTERS	311
#define STAT_BONUSATTRTNGVSMONSTERS	312
#define STAT_DEFENSEVSMONSTERS		313
#define STAT_ENHANCEDDEFVSMONSTERS	314
#define STAT_FIREDAMAGELENGTH		315
#define STAT_MINFIREDAMAGELENGTH	316
#define STAT_MAXFIREDAMAGELENGTH	317
#define STAT_PROGRESSIVEDAMAGE		318
#define STAT_PROGRESSIVESTEAL		319
#define STAT_PROGRESSIVEOTHER		320
#define STAT_PROGRESSIVEFIRE		321
#define STAT_PROGRESSIVECOLD		322
#define STAT_PROGRESSIVELIGHTNING	323
#define STAT_EXTRACHARGES			324
#define STAT_PROGRESSIVEATTACKRTNG	325
#define STAT_POISONCOUNT			326
#define STAT_DAMAGEFRAMERATE		327
#define STAT_PIERCEIDX				328
#define STAT_FIREMASTERY			329
#define STAT_LIGHTNINGMASTERY		330
#define STAT_COLDMASTERY			331
#define STAT_POISONMASTERY			332
#define STAT_PSENEMYFIRERESREDUC	333		// passive enemy fire resist reduction
#define STAT_PSENEMYLIGHTNRESREDUC	334		// passive enemy lightning resist reduction
#define STAT_PSENEMYCOLDRESREDUC	335		// passive enemy cold resist reduction
#define STAT_PSENEMYPSNRESREDUC		336		// passive enemy poison resist reduction
#define STAT_CRITICALSTRIKE			337
#define STAT_DODGE					338
#define STAT_AVOID					339
#define STAT_EVADE					340
#define STAT_WARMTH					341
#define STAT_MELEEARMASTERY			342		// melee attack rating mastery
#define STAT_MELEEDAMAGEMASTERY		343
#define STAT_MELEECRITHITMASTERY	344
#define STAT_THROWNWEAPONARMASTERY	345		// thrown weapon attack rating mastery
#define STAT_THROWNWEAPONDMGMASTERY	346
#define STAT_THROWNCRITHITMASTERY	347		// thrown weapon critical hit mastery
#define STAT_WEAPONBLOCK			348
#define STAT_SUMMONRESIST			349
#define STAT_MODIFIERLISTSKILL		350
#define STAT_MODIFIERLISTLEVEL		351
#define STAT_LASTSENTLIFEPERCENT	352
#define STAT_SOURCEUNITTYPE			353
#define STAT_SOURCEUNITID			354
#define STAT_SHORTPARAMETER1		355
#define STAT_QUESTITEMDIFFICULTY	356
#define STAT_PASSIVEMAGICDMGMASTERY	357
#define STAT_PASSIVEMAGICRESREDUC	358

///////////////////////////////////////////////////
// Unit Enchantments
///////////////////////////////////////////////////
#define ENCH_EXTRA_STRONG			5
#define ENCH_EXTRA_FAST				6
#define ENCH_CURSED					7
#define ENCH_MAGIC_RESISTANT		8
#define ENCH_FIRE_ENCHANTED			9
#define ENCH_LIGHTNING_ENCHANTED	17
#define ENCH_COLD_ENCHANTED			18
#define ENCH_MANA_BURN				25
#define ENCH_TELEPORTATION			26
#define ENCH_SPECTRAL_HIT			27
#define ENCH_STONE_SKIN				28
#define ENCH_MULTIPLE_SHOTS			29

///////////////////////////////////////////////////
// Skill Tab Definitions
// (for GetUnitStat with STAT_SKILLTAB)
///////////////////////////////////////////////////
#define SKILLTAB_AMAZON_BOW				0
#define SKILLTAB_AMAZON_PASSIVE			1
#define SKILLTAB_AMAZON_JAVELIN			2
#define SKILLTAB_SORCERESS_FIRE			8
#define SKILLTAB_SORCERESS_LIGHTNING	9
#define SKILLTAB_SORCERESS_COLD			10
#define SKILLTAB_NECROMANCER_CURSES		16
#define SKILLTAB_NECROMANCER_POISON		17
#define SKILLTAB_NECROMANCER_SUMMON		18
#define SKILLTAB_PALADIN_COMBAT			24
#define SKILLTAB_PALADIN_OFFENSIVE		25
#define SKILLTAB_PALADIN_DEFENSIVE		26
#define SKILLTAB_BARBARIAN_COMBAT		32
#define SKILLTAB_BARBARIAN_MASTERIES	33
#define SKILLTAB_BARBARIAN_WARCRIES		34
#define SKILLTAB_DRUID_SUMMONING		40
#define SKILLTAB_DRUID_SHAPESHIFTING	41
#define SKILLTAB_DRUID_ELEMENTAL		42
#define SKILLTAB_ASSASSIN_TRAPS			48
#define SKILLTAB_ASSASSIN_SHADOWDISC	49
#define SKILLTAB_ASSASSIN_MARTIALARTS	50
#define SKILLTAB_MAX					50

///////////////////////////////////////////////////
// NodePages
///////////////////////////////////////////////////
#define NODEPAGE_STORAGE	1
#define NODEPAGE_BELTSLOTS	2
#define NODEPAGE_EQUIP		3

///////////////////////////////////////////////////
// Item Actions
///////////////////////////////////////////////////
#define ITEM_ACTION_NEW_GROUND			0x00
#define ITEM_ACTION_PICK_UP				0x01
#define ITEM_ACTION_DROP				0x02
#define ITEM_ACTION_OLD_GROUND			0x03
#define ITEM_ACTION_TO_STORAGE			0x04
#define ITEM_ACTION_FROM_STORAGE		0x05
#define ITEM_ACTION_EQUIP				0x06
#define ITEM_ACTION_INDIRECT_SWAP_BODY	0x07
#define ITEM_ACTION_UNEQUIP				0x08
#define ITEM_ACTION_SWAP_BODY			0x09
#define ITEM_ACTION_ADD_QUANTITY		0x0A
#define ITEM_ACTION_TO_STORE			0x0B
#define ITEM_ACTION_FROM_STORE			0x0C
#define ITEM_ACTION_SWAP_IN_CONTAINER	0x0D
#define ITEM_ACTION_PLACE_BELT			0x0E
#define ITEM_ACTION_REMOVE_BELT			0x0F
#define ITEM_ACTION_SWAP_BELT			0x10
#define ITEM_ACTION_AUTO_UNEQUIP		0x11
#define ITEM_ACTION_TO_CURSOR			0x12
#define ITEM_ACTION_ITEM_IN_SOCKET		0x13
#define ITEM_ACTION_UPDATE_STATS		0x15
#define ITEM_ACTION_WEAPON_SWITCH		0x17

///////////////////////////////////////////////////
// Item Containers
///////////////////////////////////////////////////
#define CONTAINER_UNSPECIFIED			0x00
#define CONTAINER_INVENTORY				0x02
#define CONTAINER_TRADER_OFFER			0x04
#define CONTAINER_FOR_TRADE				0x06
#define CONTAINER_CUBE					0x08
#define CONTAINER_STASH					0x0A
#define CONTAINER_BELT					0x20
#define CONTAINER_ITEM					0x40
#define CONTAINER_ARMOR_TAB				0x82
#define CONTAINER_WEAPON_TAB_1			0x84
#define CONTAINER_WEAPON_TAB_2			0x86
#define CONTAINER_MISC_TAB				0x88

///////////////////////////////////////////////////
// Body Locations
///////////////////////////////////////////////////
enum BodyLocation {
	EQUIP_NONE = 0,			// Not equipped
	EQUIP_HEAD,				// Head
	EQUIP_AMULET,			// Amulet
	EQUIP_BODY,				// Body armor
	EQUIP_RIGHT_PRIMARY,	// Right primary slot
	EQUIP_LEFT_PRIMARY,		// Left primary slot
	EQUIP_RIGHT_RING,		// Right ring
	EQUIP_LEFT_RING,		// Left ring
	EQUIP_BELT,				// Belt
	EQUIP_FEET,				// Feet
	EQUIP_GLOVES,			// Gloves
	EQUIP_RIGHT_SECONDARY,	// Right secondary slot
	EQUIP_LEFT_SECONDARY,	// Left secondary slot
};

///////////////////////////////////////////////////
// Storage Buffers
///////////////////////////////////////////////////
enum StorageBuffer {
	STORAGE_INVENTORY	= 0x00,
	STORAGE_EQUIP		= 0x01,
	STORAGE_BELT		= 0x02,
	STORAGE_CUBE		= 0x03,
	STORAGE_STASH		= 0x04,
	STORAGE_NULL		= 0xFF
};

///////////////////////////////////////////////////
// Item Quality
///////////////////////////////////////////////////
#define	ITEM_QUALITY_NONE				0x00
#define	ITEM_QUALITY_INFERIOR			0x01
#define	ITEM_QUALITY_NORMAL				0x02
#define	ITEM_QUALITY_SUPERIOR			0x03
#define	ITEM_QUALITY_MAGIC				0x04
#define	ITEM_QUALITY_SET				0x05
#define	ITEM_QUALITY_RARE				0x06
#define	ITEM_QUALITY_UNIQUE				0x07
#define	ITEM_QUALITY_CRAFT				0x08

///////////////////////////////////////////////////
// Player Mode Definition
///////////////////////////////////////////////////
enum PlayerMode {
	PLAYER_MODE_DEATH = 0,				// death
	PLAYER_MODE_STAND_OUTTOWN,			// standing outside town
	PLAYER_MODE_WALK_OUTTOWN,			// walking outside town
	PLAYER_MODE_RUN,					// running
	PLAYER_MODE_BEING_HIT,				// being hit
	PLAYER_MODE_STAND_INTOWN,			// standing inside town
	PLAYER_MODE_WALK_INTOWN,			// walking outside town
	PLAYER_MODE_ATTACK1,				// attacking 1
	PLAYER_MODE_ATTACK2,				// attacking 2
	PLAYER_MODE_BLOCK,					// blocking
	PLAYER_MODE_CAST,					// casting spell
	PLAYER_MODE_THROW,					// throwing
	PLAYER_MODE_KICK,					// kicking (assassin)
	PLAYER_MODE_USESKILL1,				// using skill 1
	PLAYER_MODE_USESKILL2,				// using skill 2
	PLAYER_MODE_USESKILL3,				// using skill 3
	PLAYER_MODE_USESKILL4,				// using skill 4
	PLAYER_MODE_DEAD,					// dead
	PLAYER_MODE_SEQUENCE,				// sequence
	PLAYER_MODE_BEING_KNOCKBACK			// being knocked back
};

///////////////////////////////////////////////////
// NPC Mode Definition
///////////////////////////////////////////////////
enum NpcMode {
	NPC_MODE_DEATH = 0,			// NPC death
	NPC_MODE_STAND,				// NPC standing still
	NPC_MODE_WALK,				// NPC walking
	NPC_MODE_BEING_HIT,			// NPC getting hit
	NPC_MODE_ATTACK1,			// NPC attacking 1
	NPC_MODE_ATTACK2,			// NPC attacking 2
	NPC_MODE_BLOCK,				// NPC blocking
	NPC_MODE_CAST,				// NPC casting spell skill
	NPC_MODE_USESKILL1,			// NPC using skill 1
	NPC_MODE_USESKILL2,			// NPC using skill 2
	NPC_MODE_USESKILL3,			// NPC using skill 3
	NPC_MODE_USESKILL4,			// NPC using skill 4
	NPC_MODE_DEAD,				// NPC dead
	NPC_MODE_BEING_KNOCKBACK,	// NPC being knocked back
	NPC_MODE_SEQUENCE,			// NPC sequence
	NPC_MODE_RUN				// NPC running
};

///////////////////////////////////////////////////
// Object Mode Definition
///////////////////////////////////////////////////
enum ObjectMode {
	OBJ_MODE_IDLE = 0,			// Object idle
	OBJ_MODE_OPERATING,			// Object operating
	OBJ_MODE_OPENED,			// Object opened
	OBJ_MODE_SPECIAL1,			// Object special 1
	OBJ_MODE_SPECIAL2,			// Object special 2
	OBJ_MODE_SPECIAL3,			// Object special 3
	OBJ_MODE_SPECIAL4,			// Object special 4
	OBJ_MODE_SPECIAL5			// Object special 5
};

///////////////////////////////////////////////////
// Item Mode Definition
///////////////////////////////////////////////////
enum ItemMode {
	ITEM_MODE_INV_STASH_CUBE_STORE = 0,		// Item inven stash cube store
	ITEM_MODE_EQUIPPED,						// Item equipped self or merc
	ITEM_MODE_IN_BELT,						// Item in belt
	ITEM_MODE_ON_GROUND,					// Item on ground
	ITEM_MODE_ON_CURSOR,					// Item on cursor
	ITEM_MODE_BEING_DROPPED,				// Item being dropped
	ITEM_MODE_SOCKETED_IN_ITEM				// Item socketed in item
};

///////////////////////////////////////////////////
// Unit Types
///////////////////////////////////////////////////
enum Type {
	UNIT_PLAYER = 0,
	UNIT_MONSTER,
	UNIT_OBJECT,
	UNIT_MISSILE,
	UNIT_ITEM,
	UNIT_TILE,
	EXIT,
	XY,
  EXIT_MULTI
};

///////////////////////////////////////////////////
// Spell Stats
///////////////////////////////////////////////////
#define DSI_PASSIVE			0x00000001 // Spell is passive
#define DSI_RIGHTONLY		0x00000002 // Spell must be on right side, like paladin auras
#define DSI_TARGETABLE		0x00000004 // Spell cannot target, like Nova
#define DSI_NEEDCORPSE		0x00000008 // Spell requires a corpse, like Revive
#define DSI_ENABLEINTOWN	0x00000010 // Spell is enabled in town
#define DSI_PHYSICAL		0x00000020 // Spell causes physical damage
#define DSI_MAGICAL			0x00000040 // Spell causes magical damage
#define DSI_FIRE			0x00000080 // Spell causes fire damage
#define DSI_COLD			0x00000100 // Spell causes cold damage
#define DSI_LIGHTNING		0x00000200 // Spell causes lightning damage
#define DSI_POISON			0x00000400 // Spell causes poison damage
#define DSI_KNOCKBACK		0x00000800 // Spell knocks target(s) back
#define DSI_STUN			0x00001000 // Spell stuns target(s)
#define DSI_AFFECTGROUP		0x00002000 // Spell affects multuple targets
#define DSI_MELEE			0x00004000 // Spell only affects target(s) within melee range
#define DSI_SUMMON			0x00008000 // Spell summons minion(s), like Valkyrie
#define DSI_PARTYCAST		0x00010000 // Spell castable on other party members
#define DSI_GUIDED			0x00020000 // Spell is guided to enemies

///////////////////////////////////////////////////
// Character Classes
///////////////////////////////////////////////////
enum CharacterClass {
	CLASS_AMA = 0,
	CLASS_SOR,
	CLASS_NEC,
	CLASS_PAL,
	CLASS_BAR,
	CLASS_DRU,
	CLASS_ASN,
	CLASS_NA
};

///////////////////////////////////////////////////
// Map Definition
///////////////////////////////////////////////////
#define MAP_UNKNOWN								0x00

///////////////////////////////////////////////////
// Act 1 Maps
///////////////////////////////////////////////////
#define MAP_A1_ROGUE_ENCAMPMENT					0x01
#define MAP_A1_BLOOD_MOOR						0x02
#define MAP_A1_COLD_PLAINS						0x03
#define MAP_A1_STONY_FIELD						0x04
#define MAP_A1_DARK_WOOD						0x05
#define MAP_A1_BLACK_MARSH						0x06
#define MAP_A1_TAMOE_HIGHLAND					0x07
#define MAP_A1_DEN_OF_EVIL						0x08
#define MAP_A1_CAVE_LEVEL_1						0x09 // Cave in Cold plains
#define MAP_A1_UNDERGROUND_PASSAGE_LEVEL_1		0x0a
#define MAP_A1_HOLE_LEVEL_1						0x0b
#define MAP_A1_PIT_LEVEL_1						0x0c
#define MAP_A1_CAVE_LEVEL_2						0x0d // Cave in Cold plains
#define MAP_A1_UNDERGROUND_PASSAGE_LEVEL_2		0x0e
#define MAP_A1_HOLE_LEVEL_2						0x0f
#define MAP_A1_PIT_LEVEL_2						0x10
#define MAP_A1_BURIAL_GROUNDS					0x11
#define MAP_A1_CRYPT							0x12
#define MAP_A1_MAUSOLEUM						0x13
#define MAP_A1_FORGOTTEN_TOWER					0x14
#define MAP_A1_TOWER_CELLAR_LEVEL_1				0x15
#define MAP_A1_TOWER_CELLAR_LEVEL_2				0x16
#define MAP_A1_TOWER_CELLAR_LEVEL_3				0x17
#define MAP_A1_TOWER_CELLAR_LEVEL_4				0x18
#define MAP_A1_TOWER_CELLAR_LEVEL_5				0x19
#define MAP_A1_MONASTERY_GATE					0x1a
#define MAP_A1_OUTER_CLOISTER					0x1b
#define MAP_A1_BARRACKS							0x1c
#define MAP_A1_JAIL_LEVEL_1						0x1d
#define MAP_A1_JAIL_LEVEL_2						0x1e
#define MAP_A1_JAIL_LEVEL_3						0x1f
#define MAP_A1_INNER_CLOISTER					0x20
#define MAP_A1_CATHEDRAL						0x21
#define MAP_A1_CATACOMBS_LEVEL_1				0x22
#define MAP_A1_CATACOMBS_LEVEL_2				0x23
#define MAP_A1_CATACOMBS_LEVEL_3				0x24
#define MAP_A1_CATACOMBS_LEVEL_4				0x25
#define MAP_A1_TRISTRAM							0x26
#define MAP_A1_THE_SECRET_COW_LEVEL				0x27

///////////////////////////////////////////////////
// Act 2 Maps
///////////////////////////////////////////////////
#define MAP_A2_LUT_GHOLEIN						0x28
#define MAP_A2_ROCKY_WASTE						0x29
#define MAP_A2_DRY_HILLS						0x2a
#define MAP_A2_FAR_OASIS						0x2b
#define MAP_A2_LOST_CITY						0x2c
#define MAP_A2_VALLEY_OF_SNAKES					0x2d
#define MAP_A2_CANYON_OF_THE_MAGI				0x2e
#define MAP_A2_SEWERS_LEVEL_1					0x2f
#define MAP_A2_SEWERS_LEVEL_2					0x30
#define MAP_A2_SEWERS_LEVEL_3					0x31
#define MAP_A2_HAREM_LEVEL_1					0x32
#define MAP_A2_HAREM_LEVEL_2					0x33
#define MAP_A2_PALACE_CELLAR_LEVEL_1			0x34
#define MAP_A2_PALACE_CELLAR_LEVEL_2			0x35
#define MAP_A2_PALACE_CELLAR_LEVEL_3			0x36
#define MAP_A2_STONY_TOMB_LEVEL_1				0x37
#define MAP_A2_HALLS_OF_THE_DEAD_LEVEL_1		0x38
#define MAP_A2_HALLS_OF_THE_DEAD_LEVEL_2		0x39
#define MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_1		0x3a
#define MAP_A2_STONY_TOMB_LEVEL_2				0x3b
#define MAP_A2_HALLS_OF_THE_DEAD_LEVEL_3		0x3c
#define MAP_A2_CLAW_VIPER_TEMPLE_LEVEL_2		0x3d
#define MAP_A2_MAGGOT_LAIR_LEVEL_1				0x3e
#define MAP_A2_MAGGOT_LAIR_LEVEL_2				0x3f
#define MAP_A2_MAGGOT_LAIR_LEVEL_3				0x40
#define MAP_A2_ANCIENT_TUNNELS					0x41
#define MAP_A2_TAL_RASHAS_TOMB_1				0x42
#define MAP_A2_TAL_RASHAS_TOMB_2				0x43
#define MAP_A2_TAL_RASHAS_TOMB_3				0x44
#define MAP_A2_TAL_RASHAS_TOMB_4				0x45
#define MAP_A2_TAL_RASHAS_TOMB_5				0x46
#define MAP_A2_TAL_RASHAS_TOMB_6				0x47
#define MAP_A2_TAL_RASHAS_TOMB_7				0x48
#define MAP_A2_TAL_RASHAS_CHAMBER				0x49
#define MAP_A2_ARCANE_SANCTUARY					0x4a

///////////////////////////////////////////////////
// Act 3 Maps
///////////////////////////////////////////////////
#define MAP_A3_KURAST_DOCKS						0x4b
#define MAP_A3_SPIDER_FOREST					0x4c
#define MAP_A3_GREAT_MARSH						0x4d
#define MAP_A3_FLAYER_JUNGLE					0x4e
#define MAP_A3_LOWER_KURAST						0x4f
#define MAP_A3_KURAST_BAZAAR					0x50
#define MAP_A3_UPPER_KURAST						0x51
#define MAP_A3_KURAST_CAUSEWAY					0x52
#define MAP_A3_TRAVINCAL						0x53
#define MAP_A3_ARCHNID_LAIR						0x54
#define MAP_A3_SPIDER_CAVERN					0x55
#define MAP_A3_SWAMPY_PIT_LEVEL_1				0x56
#define MAP_A3_SWAMPY_PIT_LEVEL_2				0x57
#define MAP_A3_FLAYER_DUNGEON_LEVEL_1			0x58
#define MAP_A3_FLAYER_DUNGEON_LEVEL_2			0x59
#define MAP_A3_SWAMPY_PIT_LEVEL_3				0x5a
#define MAP_A3_FLAYER_DUNGEON_LEVEL_3			0x5b
#define MAP_A3_SEWERS_LEVEL_1					0x5c
#define MAP_A3_SEWERS_LEVEL_2					0x5d
#define MAP_A3_RUINED_TEMPLE					0x5e
#define MAP_A3_DISUSED_FANE						0x5f
#define MAP_A3_FORGOTTEN_RELIQUARY				0x60
#define MAP_A3_FORGOTTEN_TEMPLE					0x61
#define MAP_A3_RUINED_FANE						0x62
#define MAP_A3_DISUSED_RELIQUARY				0x63
#define MAP_A3_DURANCE_OF_HATE_LEVEL_1			0x64
#define MAP_A3_DURANCE_OF_HATE_LEVEL_2			0x65
#define MAP_A3_DURANCE_OF_HATE_LEVEL_3			0x66

///////////////////////////////////////////////////
// Act 4 Maps
///////////////////////////////////////////////////
#define MAP_A4_THE_PANDEMONIUM_FORTRESS			0x67
#define MAP_A4_OUTER_STEPPES					0x68
#define MAP_A4_PLAINS_OF_DESPAIR				0x69
#define MAP_A4_CITY_OF_THE_DAMNED				0x6a
#define MAP_A4_RIVER_OF_FLAME					0x6b
#define MAP_A4_THE_CHAOS_SANCTUARY				0x6c

///////////////////////////////////////////////////
// Act 5 Maps
///////////////////////////////////////////////////
#define MAP_A5_HARROGATH						0x6d
#define MAP_A5_THE_BLOODY_FOOTHILLS				0x6e
#define MAP_A5_FRIGID_HIGHLANDS					0x6f
#define MAP_A5_ARREAT_PLATEAU					0x70
#define MAP_A5_CRYSTALLINE_PASSAGE				0x71
#define MAP_A5_FROZEN_RIVER						0x72
#define MAP_A5_GLACIAL_TRAIL					0x73
#define MAP_A5_DRIFTER_CAVERN					0x74
#define MAP_A5_FROZEN_TUNDRA					0x75
#define MAP_A5_THE_ANCIENTS_WAY					0x76
#define MAP_A5_ICY_CELLAR						0x77
#define MAP_A5_ARREAT_SUMMIT					0x78
#define MAP_A5_NIHLATHAKS_TEMPLE				0x79
#define MAP_A5_HALLS_OF_ANGUISH					0x7a
#define MAP_A5_HALLS_OF_PAIN					0x7b
#define MAP_A5_HALLS_OF_VAUGHT					0x7c
#define MAP_A5_ABADDON							0x7d
#define MAP_A5_PIT_OF_ACHERON					0x7e
#define MAP_A5_INFERNAL_PIT						0x7f
#define MAP_A5_WORLDSTONE_KEEP_LEVEL_1			0x80
#define MAP_A5_WORLDSTONE_KEEP_LEVEL_2			0x81
#define MAP_A5_WORLDSTONE_KEEP_LEVEL_3			0x82
#define MAP_A5_THRONE_OF_DESTRUCTION			0x83
#define MAP_A5_WORLDSTONE_KEEP					0x84
#define MAP_A5_MATRONS_DEN						0x85
#define MAP_A5_FORGOTTEN_SANDS					0x86
#define MAP_A5_FURNACE_OF_PAIN					0x87
#define MAP_A5_TRISTRAM							0x88

///////////////////////////////////////////////////
// Quests
///////////////////////////////////////////////////
#define SPOKE_TO_WARRIV							0
#define DEN_OF_EVIL								1
#define SISTERS_BURIAL_GROUNDS					2
#define TOOLS_OF_THE_TRADE						3
#define THE_SEARCH_FOR_CAIN						4
#define THE_FORGOTTEN_TOWER						5
#define SISTERS_TO_THE_SLAUGHTER				6
#define CAN_GO_TO_ACT2							7
#define SPOKE_TO_JERHYN							8
#define RADAMENTS_LAIR							9
#define THE_HORADRIC_STAFF						10
#define TAINTED_SUN								11
#define ARCANE_SANCTUARY						12
#define THE_SUMMONER							13
#define THE_SEVEN_TOMBS							14
#define CAN_GO_TO_ACT3							15
#define SPOKE_TO_HRATLI							16
#define LAM_ESENS_TOME							17
#define KHALIMS_WILL							18
#define BLADE_OF_THE_OLD_RELIGION				19
#define THE_GOLDEN_BIRD							20
#define THE_BLACKENED_TEMPLE					21
#define THE_GUARDIAN							22
#define CAN_GO_TO_ACT4							23
#define SPOKE_TO_TYRAEL							24
#define THE_FALLEN_ANGEL						25
#define TERRORS_END								26
#define HELLS_FORGE								27
#define CAN_GO_TO_ACT5							28
#define UNKNOWN_29								29
#define UNKNOWN_30								30
#define UNKNOWN_31								31
#define THE_SECRET_COW_LEVEL					32
#define UNKNOWN_33								33
#define UNKNOWN_34								34
#define SIEGE_ON_HARROGATH						35
#define RESCUE_ON_MOUNT_ARREAT					36
#define PRISON_OF_ICE							37
#define BETRAYAL_OF_HARROGATH					38
#define RITE_OF_PASSAGE							39
#define EVE_OF_DESTRUCTION						40

///////////////////////////////////////////////////
// Quest Flags
///////////////////////////////////////////////////
enum QuestFlags {
	QFLAG_REWARD_GRANTED                = 0,
	QFLAG_REWARD_PENDING                = 1,
	QFLAG_QUEST_STARTED                 = 2,
	QFLAG_QUEST_LEAVE_TOWN              = 3,
	QFLAG_QUEST_ENTER_AREA              = 4,
	QFLAG_CUSTOM_1                      = 5,
	QFLAG_CUSTOM_2                      = 6,
	QFLAG_CUSTOM_3                      = 7,
	QFLAG_CUSTOM_4                      = 8,
	QFLAG_CUSTOM_5                      = 9,
	QFLAG_CUSTOM_6                      = 10,
	QFLAG_CUSTOM_7                      = 11,
	QFLAG_UPDATE_QUEST_LOG              = 12,
	QFLAG_PRIMARY_GOAL_ACHIEVED         = 13,
	QFLAG_QUEST_COMPLETED_NOW           = 14,
	QFLAG_QUEST_COMPLETED_BEFORE        = 15
};

///////////////////////////////////////////////////
// Town NPC Class ID's
///////////////////////////////////////////////////
#define NPCID_Cain1                             265                        //Act 1
#define NPCID_Kashya                            150                        //Act 1
#define NPCID_Akara                             148                        //Act 1
#define NPCID_Warriv1                           155                        //Act 1
#define NPCID_Gheed                             147                        //Act 1
#define NPCID_Charsi                            154                        //Act 1
#define NPCID_Greiz                             198                        //Act 2
#define NPCID_Drognan                           177                        //Act 2
#define NPCID_Fara                              178                        //Act 2
#define NPCID_Cain2                             244                        //Act 2
#define NPCID_Meshif1                           210                        //Act 2
#define NPCID_Cain3                             245                        //Act 3
#define NPCID_Ormus                             255                        //Act 3
#define NPCID_Asheara                           252                        //Act 3
#define NPCID_Cain4                             246                        //Act 4
#define NPCID_Tyrael                            367                        //Act 4
#define NPCID_Jamella                           405                        //Act 4
#define NPCID_Halbu                             257                        //Act 4
#define NPCID_Larzuk                            511                        //Act 5
#define NPCID_Cain5                             520                        //Act 5
#define NPCID_QualKehk                          515                        //Act 5
#define NPCID_Malah                             513                        //Act 5
#define NPCID_Anya                              512                        //Act 5

///////////////////////////////////////////////////
// Item Attributes (From D2jsp scripting document)
///////////////////////////////////////////////////
#define ITEM_IDENTIFIED			0x00000010 // Identified
#define ITEM_SWITCHIN			0x00000040 // Switched in(activated)
#define ITEM_SWITCHOUT			0x00000080 // Switched out(deactivated)
#define ITEM_BROKEN				0x00000100 // Broken(0 durability)
#define ITEM_HASSOCKETS			0x00000800 // Has sockets
#define ITEM_INSTORE			0x00002000 // In npc store or gamble screen TODO: remove this?
#define ITEMFLAG_NEW			0x00002000 // This flag is called 'new' by Nefarius
#define ITEM_ISEAR				0x00010000 // Player's ear
#define ITEM_STARTITEM			0x00020000 // Start item(1 selling/repair value)
#define ITEM_COMPACTSAVE		0x00200000 
#define ITEM_ETHEREAL			0x00400000 // Ethreal
#define ITEM_PERSONALIZED		0x01000000 // Personalized
#define ITEM_RUNEWORD			0x04000000 // Runeword

///////////////////////////////////////////////////
// Item Groups (mainly used in ItemDisplay)
///////////////////////////////////////////////////
#define ITEM_GROUP_HELM					0x00000001
#define ITEM_GROUP_ARMOR				0x00000002
#define ITEM_GROUP_SHIELD				0x00000004
#define ITEM_GROUP_GLOVES				0x00000008
#define ITEM_GROUP_BOOTS				0x00000010
#define ITEM_GROUP_BELT					0x00000020
#define ITEM_GROUP_DRUID_PELT			0x00000040
#define ITEM_GROUP_BARBARIAN_HELM		0x00000080
#define ITEM_GROUP_PALADIN_SHIELD		0x00000100
#define ITEM_GROUP_NECROMANCER_SHIELD	0x00000200
#define ITEM_GROUP_AXE					0x00000400
#define ITEM_GROUP_MACE					0x00000800
#define ITEM_GROUP_SWORD				0x00001000
#define ITEM_GROUP_DAGGER				0x00002000
#define ITEM_GROUP_THROWING				0x00004000
#define ITEM_GROUP_JAVELIN				0x00008000
#define ITEM_GROUP_SPEAR				0x00010000
#define ITEM_GROUP_POLEARM				0x00020000
#define ITEM_GROUP_BOW					0x00040000
#define ITEM_GROUP_CROSSBOW				0x00080000
#define ITEM_GROUP_STAFF				0x00100000
#define ITEM_GROUP_WAND					0x00200000
#define ITEM_GROUP_SCEPTER				0x00400000
#define ITEM_GROUP_ASSASSIN_KATAR		0x00800000
#define ITEM_GROUP_SORCERESS_ORB		0x01000000
#define ITEM_GROUP_AMAZON_WEAPON		0x02000000
#define ITEM_GROUP_NORMAL				0x04000000
#define ITEM_GROUP_EXCEPTIONAL			0x08000000
#define ITEM_GROUP_ELITE				0x10000000
#define ITEM_GROUP_ALLARMOR				0x20000000
#define ITEM_GROUP_ALLWEAPON			0x40000000
#define ITEM_GROUP_CIRCLET				0x80000000

#define ITEM_GROUP_CHIPPED				0x00000001
#define ITEM_GROUP_FLAWED				0x00000002
#define ITEM_GROUP_REGULAR				0x00000004
#define ITEM_GROUP_FLAWLESS				0x00000008
#define ITEM_GROUP_PERFECT				0x00000010
#define ITEM_GROUP_AMETHYST				0x00000020
#define ITEM_GROUP_DIAMOND				0x00000040
#define ITEM_GROUP_EMERALD				0x00000080
#define ITEM_GROUP_RUBY					0x00000100
#define ITEM_GROUP_SAPPHIRE				0x00000200
#define ITEM_GROUP_TOPAZ				0x00000400
#define ITEM_GROUP_SKULL				0x00000800
#define ITEM_GROUP_RUNE					0x00001000

#define ITEM_GROUP_GEM (ITEM_GROUP_AMETHYST|ITEM_GROUP_DIAMOND|ITEM_GROUP_EMERALD|ITEM_GROUP_RUBY|ITEM_GROUP_SAPPHIRE|ITEM_GROUP_TOPAZ|ITEM_GROUP_SKULL)

//https://d2mods.info/forum/viewtopic.php?p=487011#p487011
//UnitAny 0xC4
enum UNITFLAG
{
	UNITFLAG_MODE = 0x00000001,       // Sends mode updates via D2GAME.6FC4C520.
	UNITFLAG_SELECTABLE = 0x00000002,       // Unit is selectable.
	UNITFLAG_ATTACKABLE = 0x00000004,       // Unit is attackable.
	UNITFLAG_SEARCHABLE = 0x00000008,       // Unit can be seen by target searching functions (aura, curse).
	UNITFLAG_NEW = 0x00000010,       // Sends entire unit to the client.
	UNITFLAG_NO_SHADOW = 0x00000020,       // Unit doesn't have a shadow.
	UNITFLAG_FINISHED = 0x00000040,       // Unit has executed the current skill's 'do' function.
	UNITFLAG_PREOPERATED = 0x00000080,       // Tells the game that this object has been pre-operated (that is, it should not change its mode again during initialization).
	UNITFLAG_CHATMESSAGE = 0x00000100,       // Sends pUnit->pChatMessage (overlay text).
	UNITFLAG_HIRELING = 0x00000200,       // Unit is a hireling.
	UNITFLAG_SOUND = 0x00000400,       // Sends pUnit->eSoundMessage.
	UNITFLAG_UMOD = 0x00000800,       // Sends the monster's UMods to the client if MONSTERUPDATE_UMOD is set in pMonsterData->dwUpdateMask.
	UNITFLAG_FLIPPY = 0x00001000,       // When adding an item to a room this tells the game to send ITEMMODE_DROP instead of ITEMMODE_ROOM.
	UNITFLAG_UPDATE = 0x00002000,       // Set for units that're inside the pRoom->pUnitUpdateList update queue (which is the core controller for updates).
	UNITFLAG_SEQUENCE_TRANSITION = 0x00004000,       // Set when a animation sequence transitions from one mode to the next.
	UNITFLAG_SOFTHIT = 0x00008000,       // Sends the unit's current HP percent and hitclass (pUnit->dwHitClass) to the client.
	UNITFLAG_DEAD = 0x00010000,       // The unit is to be considered dead even if its mode is not DD or DT.
	UNITFLAG_NO_TREASURE = 0x00020000,       // This unit never drops items, even if it has a treasure class.
	UNITFLAG_MONSTERMODE = 0x00080000,       // Set when SMONSTER_SetMode successfully changes the monster's mode.
	UNITFLAG_PREDRAW = 0x00100000,       // Draw this unit as if it were a ground tile.
	UNITFLAG_ASYNC = 0x00200000,       // This unit exists only on the client (critters).
	UNITFLAG_CLIENT = 0x00400000,       // This is set for all units created on the client.
	UNITFLAG_DELETE = 0x00800000,       // When this is set CUNIT_DoFrame will not process this unit and delete it if it is an async unit.
	UNITFLAG_PRESET = 0x01000000,       // Set for units that're created through GAME_SpawnPresets. Some other places also set this, usually related to quest units.
	UNITFLAG_RESTORE = 0x02000000,       // Unit will always be saved by SUNIT_Deactivate even if it is a corpse or opened object.
	UNITFLAG_NO_EXPERIENCE = 0x04000000,       // This unit doesn't give experience when slain.
	UNITFLAG_SEEN = 0x10000000,       // This unit was drawn.
	UNITFLAG_REVEALED = 0x20000000,       // This unit was added to the automap (shrines).
	UNITFLAG_PETIGNORE = 0x40000000,       // This unit is ignored by pets.
	UNITFLAG_PET = 0x80000000        // This unit is a pet.
};

//UnitAny 0xC8
enum UNITFLAGEX
{
	UNITFLAGEX_INVENTORY = 0x00000001,       // Dispatches hInventory->hUpdates. Objects also check this which will lead to undefined behavior when set because they don't have an inventory.
	UNITFLAGEX_2 = 0x00000002,       // Set by D2SetInventoryUpdateMessage alongside UNITFLAGEX_INVENTORY if the unit is a player.
	UNITFLAGEX_MERCHANT_INVENTORY_PUT = 0x00000004,       // Adds the item to an NPC's trade inventory (sell to NPC).
	UNITFLAGEX_DISGUISED = 0x00000008,       // This unit is using another unit's graphics (shapechange).
	UNITFLAGEX_MERCHANT_INVENTORY_TAKE = 0x00000010,       // Removes an item from an NPC's trade inventory (buy from NPC).
	UNITFLAGEX_REMOVE = 0x00000020,       // If this is set alongside UNITFLAG_DELETE the client will send CMESSAGE_TELEPORT which instructs the server to set UNITFLAGEX_TELEPORTED. Must be some legacy junk.
	UNITFLAGEX_CONVERTING = 0x00000040,       // Set while the cursor is in conversion mode (legacy).
	UNITFLAGEX_VISIBLE = 0x00000080,       // Unit is within line of sight of the player.
	UNITFLAGEX_DEACTIVATED = 0x00000100,       // Set for units that have been deactivated by SUNIT_Deactivate.
	UNITFLAGEX_REACTIVATED = 0x00000200,       // Set for units that have been reactivated by SUNIT_Reactivate.    
	UNITFLAGEX_OWNED = 0x00000400,       // This unit has an owner stored at pUnit->dwOwnerID and pUnit->eOwnerTOU (usually used by missile, but also set for pets).
	UNITFLAGEX_POSITIONED = 0x00000800,       // SMESSAGE_SETUNITPOS, bFadeOutScreen = FALSE.
	UNITFLAGEX_MOVING = 0x00002000,       // Set for critters while they're moving on the client.
	UNITFLAGEX_TEMPORARY = 0x00008000,       // Items with this bit set are not saved to the D2S.
	UNITFLAGEX_TELEPORTED = 0x00010000,       // SMESSAGE_SETUNITPOS, bFadeOutScreen = TRUE.
	UNITFLAGEX_ATTACKED = 0x00020000,       // The unit has been attacked and is storing details of the attacker in pUnit->dwAttackerID and pUnit->eAttackerTOU.
	UNITFLAGEX_INVISIBLE = 0x00040000,       // The unit is invisible and will be ignored by the drawing code completely.
	UNITFLAGEX_EXPANSION = 0x02000000,       // This is a unit in an expansion game.
	UNITFLAGEX_SERVER = 0x04000000,       // This is set for all units created on the server.
};

#define MONSTAT_ALIGN_ENEMY				0x0
#define MONSTAT_ALIGN_ALLY				0x1
#define MONSTAT_ALIGN_NEUTRAL			0x2

// Toggles for GOODSK and GOODCLSK in ItemDisplay
#define CLASS_SKILLS					0
#define CLASS_TAB_SKILLS				1

#define ITEM_TYPE_ALLARMOR 50

///////////////////////////////////////////////////
// Video Mode
///////////////////////////////////////////////////
// This applies to Diablo II since version 1.00
enum D2_VideoMode {
  VIDEO_MODE_GDI = 1,
  VIDEO_MODE_SOFTWARE,
  VIDEO_MODE_DIRECT_DRAW,
  VIDEO_MODE_GLIDE,
  VIDEO_MODE_OPENGL,
  VIDEO_MODE_DIRECT3D,
  VIDEO_MODE_RAVE
};
