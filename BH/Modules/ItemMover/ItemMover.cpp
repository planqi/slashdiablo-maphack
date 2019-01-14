#include "ItemMover.h"
#include "../../BH.h"
#include "../../D2Ptrs.h"
#include "../../D2Stubs.h"

// This module was inspired by the RedVex plugin "Item Mover", written by kaiks.
// Thanks to kaiks for sharing his code.

int INVENTORY_WIDTH = 10;
int INVENTORY_HEIGHT = 4;
int STASH_WIDTH = 6;
int LOD_STASH_HEIGHT = 8;
int CLASSIC_STASH_HEIGHT = 4;
int CUBE_WIDTH = 3;
int CUBE_HEIGHT = 4;

// These are pixel positions
int INVENTORY_LEFT = 417;
int INVENTORY_TOP = 315;
int STASH_LEFT = 153;
int LOD_STASH_TOP = 143;
int CLASSIC_STASH_TOP = 334;
int CUBE_LEFT = 197;
int CUBE_TOP = 199;
int CELL_SIZE = 29;

int quality_to_color[] = {
	White, // none
	White, // inferior
	White, // normal
	White, // superior
	Blue, // magic
	Green, // set
	Yellow, // rare
	Gold, // unique
	Orange // craft
};

void ItemMover::Init() {
	// We should be able to get the layout from *p_D2CLIENT_StashLayout and friends,
	// but doesn't seem to be working at the moment so use the mpq data.

	InventoryLayout *classicStashLayout;
	InventoryLayout *lodStashLayout;
	InventoryLayout *inventoryLayout;
	InventoryLayout *cubeLayout;

	// Pull screen sizing info from the mpq data
	int screenWidth = *p_D2CLIENT_ScreenSizeX;
	int screenHeight = *p_D2CLIENT_ScreenSizeY;
	//PrintText(1, "Got screensize %d, %d", screenWidth, screenHeight);

	if (screenWidth != 800 || screenHeight != 600) {
		classicStashLayout = InventoryLayoutMap["Bank Page 1"];
		lodStashLayout = InventoryLayoutMap["Big Bank Page 1"];
		inventoryLayout = InventoryLayoutMap["Amazon"];  // all character types have the same layout
		cubeLayout = InventoryLayoutMap["Transmogrify Box Page 1"];

		INVENTORY_LEFT = ((inventoryLayout->Left - 320) + (*p_D2CLIENT_ScreenSizeX / 2));
		INVENTORY_TOP = ((*p_D2CLIENT_ScreenSizeY / 2) - 240) + inventoryLayout->Top;
		STASH_LEFT = ((*p_D2CLIENT_ScreenSizeX / 2) - 320) + lodStashLayout->Left;
		LOD_STASH_TOP = ((*p_D2CLIENT_ScreenSizeY / 2) - 240) + lodStashLayout->Top;
		CLASSIC_STASH_TOP = ((*p_D2CLIENT_ScreenSizeY / 2) - 240) + classicStashLayout->Top;
		CUBE_LEFT = ((*p_D2CLIENT_ScreenSizeX / 2) - 320) + cubeLayout->Left;
		CUBE_TOP = ((*p_D2CLIENT_ScreenSizeY / 2) - 240) + cubeLayout->Top;
	} else {
		classicStashLayout = InventoryLayoutMap["Bank Page2"];
		lodStashLayout = InventoryLayoutMap["Big Bank Page2"];
		inventoryLayout = InventoryLayoutMap["Amazon2"];  // all character types have the same layout
		cubeLayout = InventoryLayoutMap["Transmogrify Box2"];

		INVENTORY_LEFT = inventoryLayout->Left;
		INVENTORY_TOP = inventoryLayout->Top;
		STASH_LEFT = lodStashLayout->Left;
		LOD_STASH_TOP = lodStashLayout->Top;
		CLASSIC_STASH_TOP = classicStashLayout->Top;
		CUBE_LEFT = cubeLayout->Left;
		CUBE_TOP = cubeLayout->Top;
	}

	CELL_SIZE = inventoryLayout->SlotPixelHeight;

	INVENTORY_WIDTH = inventoryLayout->SlotWidth;
	INVENTORY_HEIGHT = inventoryLayout->SlotHeight;
	STASH_WIDTH = lodStashLayout->SlotWidth;
	LOD_STASH_HEIGHT = lodStashLayout->SlotHeight;
	CLASSIC_STASH_HEIGHT = classicStashLayout->SlotHeight;
	CUBE_WIDTH = cubeLayout->SlotWidth;
	CUBE_HEIGHT = cubeLayout->SlotHeight;

	if (!InventoryItemIds) {
		InventoryItemIds = new int[INVENTORY_WIDTH * INVENTORY_HEIGHT];
	}
	if (!StashItemIds) {
		StashItemIds = new int[STASH_WIDTH * LOD_STASH_HEIGHT];
	}
	if (!CubeItemIds) {
		CubeItemIds = new int[CUBE_WIDTH * CUBE_HEIGHT];
	}

	//PrintText(1, "Got positions: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
	//	INVENTORY_WIDTH,
	//	INVENTORY_HEIGHT,
	//	STASH_WIDTH,
	//	LOD_STASH_HEIGHT,
	//	CLASSIC_STASH_HEIGHT,
	//	CUBE_WIDTH,
	//	CUBE_HEIGHT,
	//	INVENTORY_LEFT,
	//	INVENTORY_TOP,
	//	STASH_LEFT,
	//	LOD_STASH_TOP,
	//	CLASSIC_STASH_TOP,
	//	CUBE_LEFT,
	//	CUBE_TOP,
	//	CELL_SIZE
	//);
}

bool ItemMover::LoadInventory(UnitAny *unit, int xpac, int source, int sourceX, int sourceY, bool shiftState, bool ctrlState, int stashUI, int invUI) {
	bool returnValue = false;

	memset(InventoryItemIds, 0, INVENTORY_WIDTH * INVENTORY_HEIGHT * sizeof(int));
	memset(StashItemIds, 0, STASH_WIDTH * LOD_STASH_HEIGHT * sizeof(int));
	memset(CubeItemIds, 0, CUBE_WIDTH * CUBE_HEIGHT * sizeof(int));

	if (!xpac) {
		for (int x = 0; x < STASH_WIDTH; x++) {
			for (int y = CLASSIC_STASH_HEIGHT; y < LOD_STASH_HEIGHT; y++) {
				StashItemIds[y*STASH_WIDTH + x] = -1;
			}
		}
	}

	unsigned int itemId = 0;
	BYTE itemXSize, itemYSize;
	bool cubeInInventory = false, cubeAnywhere = false;
	for (UnitAny *pItem = unit->pInventory->pFirstItem; pItem; pItem = pItem->pItemData->pNextInvItem) {
		int *p, width;
		if (pItem->pItemData->ItemLocation == STORAGE_INVENTORY) {
			p = InventoryItemIds;
			width = INVENTORY_WIDTH;
		} else if (pItem->pItemData->ItemLocation == STORAGE_STASH) {
			p = StashItemIds;
			width = STASH_WIDTH;
		} else if (pItem->pItemData->ItemLocation == STORAGE_CUBE) {
			p = CubeItemIds;
			width = CUBE_WIDTH;
		} else {
			continue;
		}

		bool box = false;
		char *code = D2COMMON_GetItemText(pItem->dwTxtFileNo)->szCode;
		if (code[0] == 'b' && code[1] == 'o' && code[2] == 'x') {
			if (pItem->pItemData->ItemLocation == STORAGE_INVENTORY) {
				cubeInInventory = true;
				cubeAnywhere = true;
			}
			if (pItem->pItemData->ItemLocation == STORAGE_STASH) {
				cubeAnywhere = true;
			}
			box = true;
		}

		int xStart = pItem->pObjectPath->dwPosX;
		int yStart = pItem->pObjectPath->dwPosY;
		BYTE xSize = D2COMMON_GetItemText(pItem->dwTxtFileNo)->xSize;
		BYTE ySize = D2COMMON_GetItemText(pItem->dwTxtFileNo)->ySize;
		for (int x = xStart; x < xStart + xSize; x++) {
			for (int y = yStart; y < yStart + ySize; y++) {
				p[y*width + x] = pItem->dwUnitId;

				// If you click to move the cube into itself, your character ends up in
				// the amusing (and apparently permanent) state where he has no visible
				// cube and yet is unable to pick one up. Logging out does not fix it.
				// So we disable all cube movements to be on the safe side.
				if (x == sourceX && y == sourceY && pItem->pItemData->ItemLocation == source && !box) {
					// This is the item we want to move
					itemId = pItem->dwUnitId;
					itemXSize = xSize;
					itemYSize = ySize;
				}
			}
		}
	}

	int destination;
	if (ctrlState && shiftState && ((stashUI && cubeAnywhere) || (invUI && cubeInInventory)) && source != STORAGE_CUBE) {
		destination = STORAGE_CUBE;
	} else if (ctrlState) {
		destination = STORAGE_NULL;  // i.e. the ground
	} else if (source == STORAGE_STASH || source == STORAGE_CUBE) {
		destination = STORAGE_INVENTORY;
	} else if (source == STORAGE_INVENTORY && D2CLIENT_GetUIState(UI_STASH)) {
		destination = STORAGE_STASH;
	} else if (source == STORAGE_INVENTORY && D2CLIENT_GetUIState(UI_CUBE)) {
		destination = STORAGE_CUBE;
	} else {
		return false;
	}

	// Find a spot for the item in the destination container
	if (itemId > 0) {
		returnValue = FindDestination(xpac, destination, itemId, itemXSize, itemYSize);
	}

	FirstInit = true;
	return returnValue;
}

bool ItemMover::FindDestination(int xpac, int destination, unsigned int itemId, BYTE xSize, BYTE ySize) {
	int *p, width = 0, height = 0;
	if (destination == STORAGE_INVENTORY) {
		p = InventoryItemIds;
		width = INVENTORY_WIDTH;
		height = INVENTORY_HEIGHT;
	} else if (destination == STORAGE_STASH) {
		p = StashItemIds;
		width = STASH_WIDTH;
		height = xpac ? LOD_STASH_HEIGHT : CLASSIC_STASH_HEIGHT;
	} else if (destination == STORAGE_CUBE) {
		p = CubeItemIds;
		width = CUBE_WIDTH;
		height = CUBE_HEIGHT;
	}

	bool found = false;
	int destX = 0, destY = 0;
	if (width) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				bool abort = false;
				int vacancies = 0;
				for (int testx = x; testx < x + xSize && testx < width; testx++) {
					for (int testy = y; testy < y + ySize && testy < height; testy++) {
						if (p[testy*width + testx]) {
							abort = true;
							break;
						} else {
							vacancies++;
						}
					}
					if (abort) {
						break;
					}
				}
				if (vacancies == xSize * ySize) {
					// Found an empty spot that's big enough for the item
					found = true;
					destX = x;
					destY = y;
					break;
				}
			}
			if (found) {
				break;
			}
		}
	} else {
		found = true;
	}

	if (found) {
		Lock();
		if (ActivePacket.startTicks == 0) {
			ActivePacket.itemId = itemId;
			ActivePacket.x = destX;
			ActivePacket.y = destY;
			ActivePacket.startTicks = BHGetTickCount();
			ActivePacket.destination = destination;
		}
		Unlock();
	}

	return found;
}

void ItemMover::PickUpItem() {
	BYTE PacketData[5] = {0x19,0,0,0,0};
	*reinterpret_cast<int*>(PacketData + 1) = ActivePacket.itemId;
	D2NET_SendPacket(5, 1, PacketData);
}

void ItemMover::PutItemInContainer() {
	BYTE PacketData[17] = {0x18,0,0,0,0};
	*reinterpret_cast<int*>(PacketData + 1) = ActivePacket.itemId;
	*reinterpret_cast<int*>(PacketData + 5) = ActivePacket.x;
	*reinterpret_cast<int*>(PacketData + 9) = ActivePacket.y;
	*reinterpret_cast<int*>(PacketData + 13)= ActivePacket.destination;
	D2NET_SendPacket(17, 1, PacketData);
}

void ItemMover::PutItemOnGround() {
	BYTE PacketData[5] = {0x17,0,0,0,0};
	*reinterpret_cast<int*>(PacketData + 1) = ActivePacket.itemId;
	D2NET_SendPacket(5, 1, PacketData);
}

void ItemMover::OnRightClick(bool up, int x, int y, bool* block) {
	BnetData* pData = (*p_D2LAUNCH_BnData);
	UnitAny *unit = D2CLIENT_GetPlayerUnit();
	bool shiftState = ((GetKeyState(VK_LSHIFT) & 0x80) || (GetKeyState(VK_RSHIFT) & 0x80));
	bool ctrlState = ((GetKeyState(VK_LCONTROL) & 0x80) || (GetKeyState(VK_RCONTROL) & 0x80));
	if (up || !pData || !unit || !(shiftState || ctrlState)) {
		return;
	}

	Init();

	int xpac = pData->nCharFlags & PLAYER_TYPE_EXPANSION;

	int inventoryRight = INVENTORY_LEFT + (CELL_SIZE * INVENTORY_WIDTH);
	int inventoryBottom = INVENTORY_TOP + (CELL_SIZE * INVENTORY_HEIGHT);
	int stashRight = STASH_LEFT + (CELL_SIZE * STASH_WIDTH);
	int stashTop = xpac ? LOD_STASH_TOP : CLASSIC_STASH_TOP;
	int stashHeight = xpac ? LOD_STASH_HEIGHT : CLASSIC_STASH_HEIGHT;
	int stashBottom = stashTop + (CELL_SIZE * stashHeight);
	int cubeRight = CUBE_LEFT + (CELL_SIZE * CUBE_WIDTH);
	int cubeBottom = CUBE_TOP + (CELL_SIZE * CUBE_HEIGHT);

	int source, sourceX, sourceY;
	int invUI = D2CLIENT_GetUIState(UI_INVENTORY);
	int stashUI = D2CLIENT_GetUIState(UI_STASH);
	int cubeUI = D2CLIENT_GetUIState(UI_CUBE);
	if ((invUI || stashUI || cubeUI) && x >= INVENTORY_LEFT && x <= inventoryRight && y >= INVENTORY_TOP && y <= inventoryBottom) {
		source = STORAGE_INVENTORY;
		sourceX = (x - INVENTORY_LEFT) / CELL_SIZE;
		sourceY = (y - INVENTORY_TOP) / CELL_SIZE;
	} else if (stashUI && x >= STASH_LEFT && x <= stashRight && y >= stashTop && y <= stashBottom) {
		source = STORAGE_STASH;
		sourceX = (x - STASH_LEFT) / CELL_SIZE;
		sourceY = (y - stashTop) / CELL_SIZE;
	} else if (cubeUI && x >= CUBE_LEFT && x <= cubeRight && y >= CUBE_TOP && y <= cubeBottom) {
		source = STORAGE_CUBE;
		sourceX = (x - CUBE_LEFT) / CELL_SIZE;
		sourceY = (y - CUBE_TOP) / CELL_SIZE;
	} else {
		return;
	}

	bool moveItem = LoadInventory(unit, xpac, source, sourceX, sourceY, shiftState, ctrlState, stashUI, invUI);
	if (moveItem) {
		PickUpItem();
	}
	*block = true;
}

void ItemMover::LoadConfig() {
	BH::config->ReadKey("Use Healing Potion", "VK_NUMPADMULTIPLY", HealKey);
	BH::config->ReadKey("Use Mana Potion", "VK_NUMPADSUBTRACT", ManaKey);
}

void ItemMover::OnLoad() {
	LoadConfig();
}

void ItemMover::OnKey(bool up, BYTE key, LPARAM lParam, bool* block)  {
	UnitAny *unit = D2CLIENT_GetPlayerUnit();
	if (!unit)
		return;

	if (!up && (key == HealKey || key == ManaKey)) {
		char firstChar = key == HealKey ? 'h' : 'm';
		char minPotion = 127;
		DWORD minItemId = 0;
		bool isBelt = false;
		for (UnitAny *pItem = unit->pInventory->pFirstItem; pItem; pItem = pItem->pItemData->pNextInvItem) {
			if (pItem->pItemData->ItemLocation == STORAGE_INVENTORY ||
				pItem->pItemData->ItemLocation == STORAGE_NULL && pItem->pItemData->NodePage == NODEPAGE_BELTSLOTS) {
				char* code = D2COMMON_GetItemText(pItem->dwTxtFileNo)->szCode;
				if (code[0] == firstChar && code[1] == 'p' && code[2] < minPotion) {
					minPotion = code[2];
					minItemId = pItem->dwUnitId;
					isBelt = pItem->pItemData->NodePage == NODEPAGE_BELTSLOTS;
				}
			}
			//char *code = D2COMMON_GetItemText(pItem->dwTxtFileNo)->szCode;
			//if (code[0] == 'b' && code[1] == 'o' && code[2] == 'x') {
			//	// Hack to pick up cube to fix cube-in-cube problem
			//	BYTE PacketDataCube[5] = {0x19,0,0,0,0};
			//	*reinterpret_cast<int*>(PacketDataCube + 1) = pItem->dwUnitId;
			//	D2NET_SendPacket(5, 1, PacketDataCube);
			//	break;
			//}
		}
		if (minItemId > 0) {
			if (isBelt){
				BYTE PacketData[13] = { 0x26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				*reinterpret_cast<int*>(PacketData + 1) = minItemId;
				D2NET_SendPacket(13, 0, PacketData);
			}
			else{
				//PrintText(1, "Sending packet %d, %d, %d", minItemId, unit->pPath->xPos, unit->pPath->yPos);
				BYTE PacketData[13] = { 0x20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				*reinterpret_cast<int*>(PacketData + 1) = minItemId;
				*reinterpret_cast<WORD*>(PacketData + 5) = (WORD)unit->pPath->xPos;
				*reinterpret_cast<WORD*>(PacketData + 9) = (WORD)unit->pPath->yPos;
				D2NET_SendPacket(13, 0, PacketData);
			}
			*block = true;
		}
	}
}

void ItemMover::OnGamePacketRecv(BYTE* packet, bool* block) {
	switch (packet[0])
	{
	case 0x9c:
		{
			// We get this packet after placing an item in a container or on the ground
			if (FirstInit) {
				BYTE action = packet[1];
				unsigned int itemId = *(unsigned int*)&packet[4];
				Lock();
				if (itemId == ActivePacket.itemId) {
					//PrintText(1, "Placed item id %d", itemId);
					ActivePacket.itemId = 0;
					ActivePacket.x = 0;
					ActivePacket.y = 0;
					ActivePacket.startTicks = 0;
					ActivePacket.destination = 0;
				}
				Unlock();
			}

			if ((*BH::MiscToggles2)["Advanced Item Display"].state) {
				bool success = true;
				ItemInfo item = {};
				ParseItem((unsigned char*)packet, &item, &success);
				//PrintText(1, "Item packet: %s, %s, %X, %d, %d", item.name.c_str(), item.code, item.attrs->flags, item.sockets, GetDefense(&item));
				if ((item.action == ITEM_ACTION_NEW_GROUND || item.action == ITEM_ACTION_OLD_GROUND) && success) {
					bool showOnMap = false;
					auto color = UNDEFINED_COLOR;

					for (vector<Rule*>::iterator it = MapRuleList.begin(); it != MapRuleList.end(); it++) {
						if ((*it)->Evaluate(NULL, &item)) {
							color = (*it)->action.notifyColor;
							showOnMap = true;
							// if we leave this break here, we can't set notify colors as nicely
							// for multiline 'building' configs
							/* break; */
						}
					}

					//PrintText(1, "Item on ground: %s, %s, %s, %X", item.name.c_str(), item.code, item.attrs->category.c_str(), item.attrs->flags);
					if(showOnMap) {
						if (color == UNDEFINED_COLOR) {
							color = quality_to_color[item.quality];
						}
						if ((*BH::MiscToggles2)["Item Drop Notifications"].state &&
								item.action == ITEM_ACTION_NEW_GROUND &&
								color != DEAD_COLOR
							 ) {
							PrintText(color, "%s%s",
									item.name.c_str(),
									(*BH::MiscToggles2)["Verbose Notifications"].state ? " \377c5drop" : ""
									);
						}
						if ((*BH::MiscToggles2)["Item Close Notifications"].state &&
								item.action == ITEM_ACTION_OLD_GROUND &&
								color != DEAD_COLOR
							 ) {
							PrintText(color, "%s%s",
									item.name.c_str(),
									(*BH::MiscToggles2)["Verbose Notifications"].state ? " \377c5close" : ""
									);
						}
					}
					else {
						for (vector<Rule*>::iterator it = IgnoreRuleList.begin(); it != IgnoreRuleList.end(); it++) {
							if ((*it)->Evaluate(NULL, &item)) {
								*block = true;
								//PrintText(1, "Blocking item: %s, %s, %d", item.name.c_str(), item.code, item.amount);
								break;
							}
						}
					}
				}
			}
			break;
		}
	case 0x9d:
		{
			// We get this packet after picking up an item
			if (FirstInit) {
				BYTE action = packet[1];
				unsigned int itemId = *(unsigned int*)&packet[4];
				Lock();
				if (itemId == ActivePacket.itemId) {
					//PrintText(2, "Picked up item id %d", itemId);
					if (ActivePacket.destination == STORAGE_NULL) {
						PutItemOnGround();
					} else {
						PutItemInContainer();
					}
				}
				Unlock();
			}
			break;
		}
	default:
		break;
	}
	return;
}

void ItemMover::OnGameExit() {
	ActivePacket.itemId = 0;
	ActivePacket.x = 0;
	ActivePacket.y = 0;
	ActivePacket.startTicks = 0;
	ActivePacket.destination = 0;
}

// Code for reading the 0x9c bitstream (borrowed from heroin_glands)
void ParseItem(const unsigned char *data, ItemInfo *item, bool *success) {
	*success = true;
	try {
		BitReader reader(data);
		unsigned long packet = reader.read(8);
		item->action = reader.read(8);
		unsigned long messageSize = reader.read(8);
		item->category = reader.read(8); // item type
		item->id = reader.read(32);

		if (packet == 0x9d) {
			reader.read(32);
			reader.read(8);
		}

		item->equipped = reader.readBool();
		reader.readBool();
		reader.readBool();
		item->inSocket = reader.readBool();
		item->identified = reader.readBool();
		reader.readBool();
		item->switchedIn = reader.readBool();
		item->switchedOut = reader.readBool();

		item->broken = reader.readBool();
		reader.readBool();
		item->potion = reader.readBool();
		item->hasSockets = reader.readBool();
		reader.readBool();
		item->inStore = reader.readBool();
		item->notInSocket = reader.readBool();
		reader.readBool();

		item->ear = reader.readBool();
		item->startItem = reader.readBool();
		reader.readBool();
		reader.readBool();
		reader.readBool();
		item->simpleItem = reader.readBool();
		item->ethereal = reader.readBool();
		reader.readBool();

		item->personalized = reader.readBool();
		item->gambling = reader.readBool();
		item->runeword = reader.readBool();
		reader.read(5);

		item->version = static_cast<unsigned int>(reader.read(8));

		reader.read(2);
		unsigned long destination = reader.read(3);

		item->ground = (destination == 0x03);

		if (item->ground) {
			item->x = reader.read(16);
			item->y = reader.read(16);
		} else {
			item->directory = reader.read(4);
			item->x = reader.read(4);
			item->y = reader.read(3);
			item->container = static_cast<unsigned int>(reader.read(4));
		}

		item->unspecifiedDirectory = false;

		if (item->action == ITEM_ACTION_TO_STORE || item->action == ITEM_ACTION_FROM_STORE) {
			long container = static_cast<long>(item->container);
			container |= 0x80;
			if (container & 1) {
				container--; //remove first bit
				item->y += 8;
			}
			item->container = static_cast<unsigned int>(container);
		} else if (item->container == CONTAINER_UNSPECIFIED) {
			if (item->directory == EQUIP_NONE) {
				if (item->inSocket) {
					//y is ignored for this container type, x tells you the index
					item->container = CONTAINER_ITEM;
				} else if (item->action == ITEM_ACTION_PLACE_BELT || item->action == ITEM_ACTION_REMOVE_BELT) {
					item->container = CONTAINER_BELT;
					item->y = item->x / 4;
					item->x %= 4;
				}
			} else {
				item->unspecifiedDirectory = true;
			}
		}

		if (item->ear) {
			item->earClass = static_cast<BYTE>(reader.read(3));
			item->earLevel = reader.read(7);
			item->code[0] = 'e';
			item->code[1] = 'a';
			item->code[2] = 'r';
			item->code[3] = 0;
			for (std::size_t i = 0; i < 16; i++) {
				char letter = static_cast<char>(reader.read(7));
				if (letter == 0) {
					break;
				}
				item->earName.push_back(letter);
			}
			item->attrs = ItemAttributeMap[item->code];
			item->name = item->attrs->name;
			item->width = item->attrs->width;
			item->height = item->attrs->height;
			//PrintText(1, "Ear packet: %s, %s, %d, %d", item->earName.c_str(), item->code, item->earClass, item->earLevel);
			return;
		}

		for (std::size_t i = 0; i < 4; i++) {
			item->code[i] = static_cast<char>(reader.read(8));
		}
		item->code[3] = 0;

		if (ItemAttributeMap.find(item->code) == ItemAttributeMap.end()) {
			HandleUnknownItemCode(item->code, "from packet");
			*success = false;
			return;
		}
		item->attrs = ItemAttributeMap[item->code];
		item->name = item->attrs->name;
		item->width = item->attrs->width;
		item->height = item->attrs->height;

		item->isGold = (item->code[0] == 'g' && item->code[1] == 'l' && item->code[2] == 'd');

		if (item->isGold) {
			bool big_pile = reader.readBool();
			if (big_pile) {
				item->amount = reader.read(32);
			} else {
				item->amount = reader.read(12);
			}
			return;
		}

		item->usedSockets = (BYTE)reader.read(3);

		if (item->simpleItem || item->gambling) {
			return;
		}

		item->level = (BYTE)reader.read(7);
		item->quality = static_cast<unsigned int>(reader.read(4));

		item->hasGraphic = reader.readBool();;
		if (item->hasGraphic) {
			item->graphic = reader.read(3);
		}

		item->hasColor = reader.readBool();
		if (item->hasColor) {
			item->color = reader.read(11);
		}

		if (item->identified) {
			switch(item->quality) {
			case ITEM_QUALITY_INFERIOR:
				item->prefix = reader.read(3);
				break;

			case ITEM_QUALITY_SUPERIOR:
				item->superiority = static_cast<unsigned int>(reader.read(3));
				break;

			case ITEM_QUALITY_MAGIC:
				item->prefix = reader.read(11);
				item->suffix = reader.read(11);
				break;

			case ITEM_QUALITY_CRAFT:
			case ITEM_QUALITY_RARE:
				item->prefix = reader.read(8) - 156;
				item->suffix = reader.read(8) - 1;
				break;

			case ITEM_QUALITY_SET:
				item->setCode = reader.read(12);
				break;

			case ITEM_QUALITY_UNIQUE:
				if (item->code[0] != 's' || item->code[1] != 't' || item->code[2] != 'd') { //standard of heroes exception?
					item->uniqueCode = reader.read(12);
				}
				break;
			}
		}

		if (item->quality == ITEM_QUALITY_RARE || item->quality == ITEM_QUALITY_CRAFT) {
			for (unsigned long i = 0; i < 3; i++) {
				if (reader.readBool()) {
					item->prefixes.push_back(reader.read(11));
				}
				if (reader.readBool()) {
					item->suffixes.push_back(reader.read(11));
				}
			}
		}

		if (item->runeword) {
			item->runewordId = reader.read(12);
			item->runewordParameter = reader.read(4);
		}

		if (item->personalized) {
			for (std::size_t i = 0; i < 16; i++) {
				char letter = static_cast<char>(reader.read(7));
				if (letter == 0) {
					break;
				}
				item->personalizedName.push_back(letter);
			}
			//PrintText(1, "Personalized packet: %s, %s", item->personalizedName.c_str(), item->code);
		}

		item->isArmor = (item->attrs->flags & ITEM_GROUP_ALLARMOR) > 0;
		item->isWeapon = (item->attrs->flags & ITEM_GROUP_ALLWEAPON) > 0;

		if (item->isArmor) {
			item->defense = reader.read(11) - 10;
		}

		/*if(entry.throwable)
		{
			reader.read(9);
			reader.read(17);
		} else */
		//special case: indestructible phase blade
		if (item->code[0] == '7' && item->code[1] == 'c' && item->code[2] == 'r') {
			reader.read(8);
		} else if (item->isArmor || item->isWeapon) {
			item->maxDurability = reader.read(8);
			item->indestructible = item->maxDurability == 0;
			/*if (!item->indestructible) {
				item->durability = reader.read(8);
				reader.readBool();
			}*/
			//D2Hackit always reads it, hmmm. Appears to work.
			item->durability = reader.read(8);
			reader.readBool();
		}

		if (item->hasSockets) {
			item->sockets = (BYTE)reader.read(4);
		}

		if (!item->identified) {
			return;
		}

		if (item->attrs->stackable) {
			if (item->attrs->useable) {
				reader.read(5);
			}
			item->amount = reader.read(9);
		}

		if (item->quality == ITEM_QUALITY_SET) {
			unsigned long set_mods = reader.read(5);
		}

		while (true) {
			unsigned long stat_id = reader.read(9);
			if (stat_id == 0x1ff) {
				break;
			}
			ItemProperty prop = {};
			if (!ProcessStat(stat_id, reader, prop) &&
					!(*BH::MiscToggles2)["Suppress Invalid Stats"].state) {
				PrintText(1, "Invalid stat: %d, %c%c%c", stat_id, item->code[0], item->code[1], item->code[2]);
				*success = false;
				break;
			}
			item->properties.push_back(prop);
		}
	} catch (int e) {
		PrintText(1, "Int exception parsing item: %c%c%c, %d", item->code[0], item->code[1], item->code[2], e);
	} catch (std::exception const & ex) {
		PrintText(1, "Exception parsing item: %c%c%c, %s", item->code[0], item->code[1], item->code[2], ex.what());
	} catch(...) {
		PrintText(1, "Miscellaneous exception parsing item: %c%c%c", item->code[0], item->code[1], item->code[2]);
		*success = false;
	}
	return;
}

bool ProcessStat(unsigned int stat, BitReader &reader, ItemProperty &itemProp) {
	if (stat > STAT_MAX) {
		return false;
	}

	StatProperties *bits = GetStatProperties(stat);
	unsigned int saveBits = bits->saveBits;
	unsigned int saveParamBits = bits->saveParamBits;
	unsigned int saveAdd = bits->saveAdd;
	itemProp.stat = stat;

	if (saveParamBits > 0) {
		switch (stat) {
			case STAT_CLASSSKILLS:
			{
				itemProp.characterClass = reader.read(saveParamBits);
				itemProp.value = reader.read(saveBits);
				return true;
			}
			case STAT_NONCLASSSKILL:
			case STAT_SINGLESKILL:
			{
				itemProp.skill = reader.read(saveParamBits);
				itemProp.value = reader.read(saveBits);
				return true;
			}
			case STAT_ELEMENTALSKILLS:
			{
				ulong element = reader.read(saveParamBits);
				itemProp.value = reader.read(saveBits);
				return true;
			}
			case STAT_AURA:
			{
				itemProp.skill = reader.read(saveParamBits);
				itemProp.value = reader.read(saveBits);
				return true;
			}
			case STAT_REANIMATE:
			{
				itemProp.monster = reader.read(saveParamBits);
				itemProp.value = reader.read(saveBits);
				return true;
			}
			case STAT_SKILLTAB:
			{
				itemProp.tab = reader.read(3);
				itemProp.characterClass = reader.read(3);
				ulong unknown = reader.read(10);
				itemProp.value = reader.read(saveBits);
				return true;
			}
			case STAT_SKILLONDEATH:
			case STAT_SKILLONHIT:
			case STAT_SKILLONKILL:
			case STAT_SKILLONLEVELUP:
			case STAT_SKILLONSTRIKING:
			case STAT_SKILLWHENSTRUCK:
			{
				itemProp.level = reader.read(6);
				itemProp.skill = reader.read(10);
				itemProp.skillChance = reader.read(saveBits);
				return true;
			}
			case STAT_CHARGED:
			{
				itemProp.level = reader.read(6);
				itemProp.skill = reader.read(10);
				itemProp.charges = reader.read(8);
				itemProp.maximumCharges = reader.read(8);
				return true;
			}
			case STAT_STATE:
			case STAT_ATTCKRTNGVSMONSTERTYPE:
			case STAT_DAMAGETOMONSTERTYPE:
			{
				// For some reason heroin_glands doesn't read these, even though
				// they have saveParamBits; maybe they don't occur in practice?
				itemProp.value = reader.read(saveBits) - saveAdd;
				return true;
			}
			default:
				reader.read(saveParamBits);
				reader.read(saveBits);
				return true;
		}
	}

	if (bits->op >= 2 && bits->op <= 5) {
		itemProp.perLevel = reader.read(saveBits);
		return true;
	}

	switch (stat) {
		case STAT_ENHANCEDMAXIMUMDAMAGE:
		case STAT_ENHANCEDMINIMUMDAMAGE:
		{
			itemProp.minimum = reader.read(saveBits);
			itemProp.maximum = reader.read(saveBits);
			return true;
		}
		case STAT_MINIMUMFIREDAMAGE:
		{
			itemProp.minimum = reader.read(saveBits);
			itemProp.maximum = reader.read(GetStatProperties(STAT_MAXIMUMFIREDAMAGE)->saveBits);
			return true;
		}
		case STAT_MINIMUMLIGHTNINGDAMAGE:
		{
			itemProp.minimum = reader.read(saveBits);
			itemProp.maximum = reader.read(GetStatProperties(STAT_MAXIMUMLIGHTNINGDAMAGE)->saveBits);
			return true;
		}
		case STAT_MINIMUMMAGICALDAMAGE:
		{
			itemProp.minimum = reader.read(saveBits);
			itemProp.maximum = reader.read(GetStatProperties(STAT_MAXIMUMMAGICALDAMAGE)->saveBits);
			return true;
		}
		case STAT_MINIMUMCOLDDAMAGE:
		{
			itemProp.minimum = reader.read(saveBits);
			itemProp.maximum = reader.read(GetStatProperties(STAT_MAXIMUMCOLDDAMAGE)->saveBits);
			itemProp.length = reader.read(GetStatProperties(STAT_COLDDAMAGELENGTH)->saveBits);
			return true;
		}
		case STAT_MINIMUMPOISONDAMAGE:
		{
			itemProp.minimum = reader.read(saveBits);
			itemProp.maximum = reader.read(GetStatProperties(STAT_MAXIMUMPOISONDAMAGE)->saveBits);
			itemProp.length = reader.read(GetStatProperties(STAT_POISONDAMAGELENGTH)->saveBits);
			return true;
		}
		case STAT_REPAIRSDURABILITY:
		case STAT_REPLENISHESQUANTITY:
		{
			itemProp.value = reader.read(saveBits);
			return true;
		}
		default:
		{
			itemProp.value = reader.read(saveBits) - saveAdd;
			return true;
		}
	}
}
