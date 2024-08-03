#include "Gamefilter.h"
#include "../../D2Ptrs.h"
#include "../../D2Intercepts.h"
#include "../../D2Stubs.h"
#include <sstream>
#include "../../BH.h"

using namespace std;

list<GameListEntry*> Gamefilter::gameList;
vector<GameListEntry*> Gamefilter::filterVector;
vector<wchar_t*> Gamefilter::gServerVector;
Control* Gamefilter::filterBox;
int Gamefilter::refreshTime;

Patch* createGameBox = new Patch(Call, D2MULTI, { 0x149EF, 0xAD8F }, (int)D2MULTI_CreateGameBox_Interception, 5);
Patch* destoryGameList = new Patch(Call, D2MULTI, { 0x11DC3, 0x8413 }, (int)Gamefilter::DestroyGamelist, 5);
Patch* listRefresh = new Patch(Call, D2MULTI, { 0xDF4E, 0x121EE }, (int)D2MULTI_GameListRefresh_Interception, 5);

void Gamefilter::OnLoad() {
	if (!D2CLIENT_GetPlayerUnit()) {
		createGameBox->Install();
		destoryGameList->Install();
		listRefresh->Install();
	}
	refreshTime = 1500;
	BH::config->ReadInt("GameListRefresh", refreshTime);

	if (refreshTime < 1000) {
		refreshTime = 1500;
	}

	showDiff = &bools["Show Difficulty"];
	*showDiff = true;

	showGs = &bools["Show Gameserver"];
	*showGs = true;

	LoadConfig();
}

void Gamefilter::LoadConfig() {
	BH::config->ReadBoolean("Show Difficulty", *showDiff);
	BH::config->ReadBoolean("Show Gameserver", *showGs);
}

void Gamefilter::OnUnload() {
	createGameBox->Remove();
	destoryGameList->Remove();
	listRefresh->Remove();
}

void Gamefilter::OnGameJoin() {
	createGameBox->Remove();
	destoryGameList->Remove();
	listRefresh->Remove();
}

void Gamefilter::OnGameExit() {
	createGameBox->Install();
	destoryGameList->Install();
	listRefresh->Install();
}

void Gamefilter::OnRealmPacketRecv(BYTE* pPacket, bool* blockPacket) {
	//S>C 0x17 MCP_CHARLIST
	//S>C 0x19 MCP_CHARLIST2
	if (pPacket[0] == 0x19 || pPacket[0] == 0x17) {
		unsigned int nChars = *reinterpret_cast<int*>(pPacket + 3);
		for (Control* pControl = *p_D2WIN_FirstControl; pControl; pControl = pControl->pNext)
		{
			if (pControl->dwType == 4 && pControl->pChildControl && pControl->pChildControl->dwType == 5) {
				pControl->dwSelectEnd = nChars;
				pControl->dwSelectStart = 0;

				pControl->pChildControl->dwScrollPosition = 0;
				pControl->pChildControl->dwScrollEntries = nChars > 8 ? (unsigned int)(ceil((nChars - 8) / 2.0)) : 0;
			}
		}
	}
	if(pPacket[0] == 0x05 && filterBox)
	{		
		wstring wFilter(filterBox->wText);
		string sFilter(wFilter.begin(), wFilter.end());

		GameListEntry* pEntry = new GameListEntry;

		pEntry->wRequestId = *(WORD*)&pPacket[1];
		pEntry->dwIndex = *(DWORD*)&pPacket[3];
		pEntry->bPlayers = pPacket[7];
		pEntry->dwStatus = *(DWORD*)&pPacket[8];
		pEntry->sGameName = reinterpret_cast<CHAR*>(pPacket + 12);
		pEntry->sGameDesc = reinterpret_cast<CHAR*>(pPacket + 13 + pEntry->sGameName.length());

		if(pEntry->bPlayers == 0)
		{
			delete pEntry;
			*blockPacket = true;
			return;
		}

		if(!gameList.empty())
		{
			for(list<GameListEntry*>::iterator ListEntry = gameList.begin(); ListEntry != gameList.end(); ListEntry++)
				if(!(*ListEntry)->sGameName.compare(pEntry->sGameName))
				{
					delete pEntry;
					*blockPacket = true;
					return;
				}
		}

		string sGameName;

		for(unsigned int i = 0; i < sFilter.length(); i++)
			sFilter[i] = ::toupper(sFilter[i]);

		for(unsigned int i = 0; i < pEntry->sGameName.length(); i++)
			sGameName += ::toupper(pEntry->sGameName[i]);

		if(wFilter.empty() || strstr(sGameName.c_str(), sFilter.c_str()))
		{
			ControlText* pText = new ControlText;
			memset(pText, NULL, sizeof(ControlText));

			pText->dwColor = 4;
			pText->wText = new wchar_t[64];
			pText->wText2 = new wchar_t[12];

			memset(pText->wText, NULL, 64);
			memset(pText->wText2, NULL, 12);

			wstring wGameName(pEntry->sGameName.begin(), pEntry->sGameName.end());

			wcscpy_s(pText->wText, 64, wGameName.c_str());
			pText->wText2[0] = pEntry->bPlayers+0x30;

			if((*p_D2MULTI_GameListControl)->pFirstText)
			{
				(*p_D2MULTI_GameListControl)->pLastText->pNext = pText;
				(*p_D2MULTI_GameListControl)->pLastText = pText;
			} else {
				(*p_D2MULTI_GameListControl)->pFirstText = pText;	
				(*p_D2MULTI_GameListControl)->pLastText = pText;
				(*p_D2MULTI_GameListControl)->pSelectedText = pText;
			}

			(*p_D2MULTI_GameListControl)->dwSelectEnd += 1;
			(*p_D2MULTI_GameListControl)->pChildControl->dwScrollEntries = (*p_D2MULTI_GameListControl)->dwSelectEnd > 9 ? (*p_D2MULTI_GameListControl)->dwSelectEnd - 9 : 0;
			filterVector.push_back(pEntry);

			if(::toupper(pEntry->sGameDesc[0]) == 'G' && ::toupper(pEntry->sGameDesc[1]) == 'S')
			{
				switch (pEntry->sGameDesc[2])
				{
				case '1':
					gServerVector.push_back(L"gs1");
					break;
				case '2':
					gServerVector.push_back(L"gs2");
					break;
				case '3':
					gServerVector.push_back(L"gs3");
					break;
				case '4':
					gServerVector.push_back(L"gs4");
					break;
				case '5':
					gServerVector.push_back(L"gs5");
					break;
				case '6':
					gServerVector.push_back(L"gs6");
					break;
				case '7':
					gServerVector.push_back(L"gs7");
					break;
				case '8':
					gServerVector.push_back(L"gs8");
					break;
				case '9':
					gServerVector.push_back(L"gs9");
					break;
				default:
					gServerVector.push_back(L"gs1");
					break;
				}
			}
			else
			{
				gServerVector.push_back(L"gs1");
			}
		}

		gameList.push_back(pEntry);

		return;
	}

	return;
}

BOOL __stdcall Gamefilter::Filterbox_InputHandler(Control* pControl, DWORD dwLength, CHAR* pChar)
{
	wstring wInput(pControl->wText);

	string sFilter(wInput.begin(), wInput.end());

	if(dwLength > 0)
		sFilter+=*pChar;

	for(unsigned int i = 0; i < sFilter.length(); i++)
		sFilter[i] = ::toupper(sFilter[i]);

	Gamefilter::BuildGameList(sFilter);

	return TRUE;
}

BOOL __stdcall Gamefilter::Filterbox_ReturnHandler(wchar_t* wText)
{

	return TRUE;
}

VOID Gamefilter::CreateGamelist(VOID)
{
	if(!gameList.empty())
	{
		for(list<GameListEntry*>::iterator ListEntry = gameList.begin(); ListEntry != gameList.end(); ListEntry++)
			delete (*ListEntry);

		gameList.clear();
		filterVector.clear();
		gServerVector.clear();

	}

	filterBox = D2WIN_CreateEditBox(599, 185, 145, 41, 7, NULL, NULL, Gamefilter::Filterbox_ReturnHandler, NULL, NULL, (ControlPreferences*)p_D2MULTI_EditboxPreferences);
	filterBox->pCellFile = D2CLIENT_LoadUiImage("DATA\\GLOBAL\\UI\\FrontEnd\\textbox2");
	D2WIN_SetEditBoxCallback(filterBox, Gamefilter::Filterbox_InputHandler);
	filterBox->dwMaxLength = 0x12;
	filterBox->Initialize(filterBox);
}

VOID __stdcall Gamefilter::DestroyGamelist(Control* pControl)
{
	if(pControl)
	{
		if(filterBox)
		{
			if(!gameList.empty())
			{
				for(list<GameListEntry*>::iterator ListEntry = gameList.begin(); ListEntry != gameList.end(); ListEntry++)
					delete (*ListEntry);

				gameList.clear();
				filterVector.clear();
				gServerVector.clear();
			}

			D2WIN_DestroyEditBox(filterBox);

			filterBox = NULL;
		}
	}

	D2WIN_DestroyControl(pControl);
}

void Gamefilter::OnOOGDraw() {
	// filterBox is instantiated in the create game box handler, so we can't
	// draw the join game screen until we have it
	if((*p_D2MULTI_GameListControl) && filterBox) {
		wstringstream wFilterStream;
		wstring wFilterString = L"Games: ";
		wstring wFilter = filterBox->wText;

		wFilterStream << (int)(*p_D2MULTI_GameListControl)->dwSelectEnd;
		wFilterString += wFilterStream.str().c_str();
		wFilterString += L"/";
		wFilterStream.str(L"");
		wFilterStream << (int)gameList.size();
		wFilterString += wFilterStream.str().c_str();

		DWORD dwOldSize = D2WIN_SetTextSize(1);
		D2WIN_DrawText(L"Filter", 549, 170, 4, -1);
		D2WIN_DrawText(wFilterString.c_str(), 424, 190, 4, -1);

		if(*showDiff || *showGs) {
			D2WIN_SetTextSize(6);
			DWORD dwListStart = (*p_D2MULTI_GameListControl)->dwSelectStart;
			DWORD dwListEnd = filterVector.size() > 9 ? dwListStart + 9 : filterVector.size();
			for (unsigned int i = dwListStart; i < dwListEnd; i++)
			{
				unsigned int uiYPos = 235 + (i - dwListStart) * 19;
				if(*showDiff) {					
					if((filterVector.at(i)->dwStatus & 0x1000)) {
						D2WIN_DrawText(L"NM", 547, uiYPos, Blue, -1);
					}
					else if ((filterVector.at(i)->dwStatus & 0x2000)) {
						D2WIN_DrawText(L"H", 547, uiYPos, Red, -1);
					}
					else {
						D2WIN_DrawText(L"N", 547, uiYPos, White, -1);
					}
				}
				if(*showGs) {
					D2WIN_DrawText(gServerVector.at(i), 528, uiYPos, Grey, -1);
				}
			}
		}
		D2WIN_SetTextSize(dwOldSize);
	}
}

void Gamefilter::BuildGameList(string sFilter)
{
	if(!gameList.empty() && (*p_D2MULTI_GameListControl))
	{
		filterVector.clear();
		gServerVector.clear();
		ControlText* pText = (*p_D2MULTI_GameListControl)->pFirstText;

		(*p_D2MULTI_GameListControl)->pFirstText = NULL;
		(*p_D2MULTI_GameListControl)->pLastText = NULL;
		(*p_D2MULTI_GameListControl)->pSelectedText = NULL;
		(*p_D2MULTI_GameListControl)->dwSelectEnd = 0;
		(*p_D2MULTI_GameListControl)->dwSelectStart = 0;
		(*p_D2MULTI_GameListControl)->dwMaxLength = 0;

		(*p_D2MULTI_GameListControl)->pChildControl->dwMaxLength = 0;
		(*p_D2MULTI_GameListControl)->pChildControl->dwScrollPosition = 0;
		(*p_D2MULTI_GameListControl)->pChildControl->dwScrollEntries = 0;

		while(pText)
		{
			ControlText* pNext = pText->pNext;

			delete[] pText->wText;
			delete[] pText->wText2;
			delete pText;

			pText = pNext;
		}

		for(list<GameListEntry*>::iterator ListEntry = gameList.begin(); ListEntry != gameList.end(); ListEntry++)
		{
			string sGameName((*ListEntry)->sGameName.c_str());

			for(UINT i = 0; i < sGameName.length(); i++)
				sGameName[i] = ::toupper(sGameName[i]);

			if(strstr(sGameName.c_str(), sFilter.c_str()))
			{

				ControlText* pText = new ControlText;
				memset(pText, NULL, sizeof(ControlText));

				pText->dwColor = 4;
				pText->wText = new wchar_t[64];
				pText->wText2 = new wchar_t[12];

				memset(pText->wText, NULL, 64);
				memset(pText->wText2, NULL, 12);

				wstring wGameName((*ListEntry)->sGameName.begin(), (*ListEntry)->sGameName.end());

				wcscpy_s(pText->wText, 64, wGameName.c_str());
				pText->wText2[0] = (*ListEntry)->bPlayers+0x30;

				if((*p_D2MULTI_GameListControl)->pFirstText)
				{
					(*p_D2MULTI_GameListControl)->pLastText->pNext = pText;
					(*p_D2MULTI_GameListControl)->pLastText = pText;
				} else {
					(*p_D2MULTI_GameListControl)->pFirstText = pText;
					(*p_D2MULTI_GameListControl)->pLastText = pText;
					(*p_D2MULTI_GameListControl)->pSelectedText = pText;
				}
				

				(*p_D2MULTI_GameListControl)->dwSelectEnd += 1;
				(*p_D2MULTI_GameListControl)->pChildControl->dwScrollEntries = (*p_D2MULTI_GameListControl)->dwSelectEnd > 9 ? (*p_D2MULTI_GameListControl)->dwSelectEnd - 9 : 0;
				filterVector.push_back(*ListEntry);

				if (::toupper((*ListEntry)->sGameDesc[0]) == 'G' && ::toupper((*ListEntry)->sGameDesc[1]) == 'S')
				{
					switch ((*ListEntry)->sGameDesc[2])
					{
					case '1':
						gServerVector.push_back(L"gs1");
						break;
					case '2':
						gServerVector.push_back(L"gs2");
						break;
					case '3':
						gServerVector.push_back(L"gs3");
						break;
					case '4':
						gServerVector.push_back(L"gs4");
						break;
					case '5':
						gServerVector.push_back(L"gs5");
						break;
					case '6':
						gServerVector.push_back(L"gs6");
						break;
					case '7':
						gServerVector.push_back(L"gs7");
						break;
					case '8':
						gServerVector.push_back(L"gs8");
						break;
					case '9':
						gServerVector.push_back(L"gs9");
						break;
					default:
						gServerVector.push_back(L"gs1");
						break;
					}
				}
				else
				{
					gServerVector.push_back(L"gs1");
				}

			}
		}
	}
}

VOID __declspec(naked) D2MULTI_CreateGameBox_Interception(VOID)
{
	__asm
	{
		PUSHAD
		CALL Gamefilter::CreateGamelist
		POPAD
		MOV EDX, 0x10
		RETN
	}
}

VOID __declspec(naked) D2MULTI_GameListRefresh_Interception(VOID)
{
	__asm
	{
		CMP EAX, Gamefilter::refreshTime
		RETN
	}
}
