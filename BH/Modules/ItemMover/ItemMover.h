#pragma once
#include "../../D2Structs.h"
#include "../../Drawing.h"
#include "../Module.h"
#include "../../Config.h"
#include "../../Common.h"
#include "../../BitReader.h"
#include "../Item/ItemDisplay.h"
#include "../../MPQInit.h"

extern int INVENTORY_WIDTH;
extern int INVENTORY_HEIGHT;
extern int STASH_WIDTH;
extern int LOD_STASH_HEIGHT;
extern int CLASSIC_STASH_HEIGHT;
extern int CUBE_WIDTH;
extern int CUBE_HEIGHT;

extern int INVENTORY_LEFT;
extern int INVENTORY_TOP;
extern int STASH_LEFT;
extern int LOD_STASH_TOP;
extern int CLASSIC_STASH_TOP;
extern int CUBE_LEFT;
extern int CUBE_TOP;
extern int CELL_SIZE;


struct ItemPacketData {
	unsigned int itemId;
	unsigned int x;
	unsigned int y;
	ULONGLONG startTicks;
	unsigned int destination;
};

class ItemMover : public Module {
private:
	bool FirstInit;
	int *InventoryItemIds;
	int *StashItemIds;
	int *CubeItemIds;
	int tp_warn_quantity;
	unsigned int TpKey;
	unsigned int HealKey;
	unsigned int ManaKey;
	unsigned int JuvKey;
	ItemPacketData ActivePacket;
	CRITICAL_SECTION crit;
	Drawing::UITab* settingsTab;
public:
	ItemMover() : Module("Item Mover"),
		ActivePacket(),
		FirstInit(false),
		InventoryItemIds(NULL),
		StashItemIds(NULL),
		CubeItemIds(NULL),
	  tp_warn_quantity(3){

		InitializeCriticalSection(&crit);
	};

	~ItemMover() {
		if (InventoryItemIds) {
			delete [] InventoryItemIds;
		}
		if (StashItemIds) {
			delete [] StashItemIds;
		}
		if (CubeItemIds) {
			delete [] CubeItemIds;
		}
		DeleteCriticalSection(&crit);
	};

	void Init();

	void Lock() { EnterCriticalSection(&crit); };
	void Unlock() { LeaveCriticalSection(&crit); };

	bool LoadInventory(UnitAny *unit, int xpac, int source, int sourceX, int sourceY, bool shiftState, bool ctrlState, int stashUI, int invUI);
	bool FindDestination(int xpac, int destination, unsigned int itemId, BYTE xSize, BYTE ySize);
	void PickUpItem();
	void PutItemInContainer();
	void PutItemOnGround();

	void LoadConfig();

	void OnLoad();
	void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);
	void OnLeftClick(bool up, int x, int y, bool* block);
	void OnRightClick(bool up, int x, int y, bool* block);
	void OnGamePacketRecv(BYTE* packet, bool *block);
	void OnGameExit();
};


void ParseItem(const unsigned char *data, ItemInfo *ii, bool *success);
bool ProcessStat(unsigned int statId, BitReader &reader, ItemProperty &itemProp);
