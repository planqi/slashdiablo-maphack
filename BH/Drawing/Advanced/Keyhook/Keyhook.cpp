#include "Keyhook.h"
#include "../../../D2Ptrs.h"
#include "../../../Common.h"

using namespace std;
using namespace Drawing;

/* Basic Hook Initializer
 *		Used for just drawing basics.
 */
Keyhook::Keyhook(HookVisibility visibility, unsigned int x, unsigned int y, unsigned int* key, std::string hotkeyName) :
Hook(visibility, x, y) {
	//Correctly format the string from the given arguments.
	timeout = 0;
	SetKey(key);
	SetName(hotkeyName);
}

/* Group Hook Initializer
 *		Used in conjuction with other basic hooks to create an advanced hook.
 */
Keyhook::Keyhook(HookGroup *group, unsigned int x, unsigned int y, unsigned int* key, std::string hotkeyName) :
Hook(group, x, y) {
	//Correctly format the string from the given arguments.
	timeout = 0;
	SetKey(key);
	SetName(hotkeyName);
}

bool Keyhook::OnLeftClick(bool up, unsigned int x, unsigned int y) {
	if (InRange(x,y)) {
		if (up) {
			if (!timeout)
				timeout = GetTickCount();
			else
				timeout = 0;
		}
		return true;
	}
	return false;
}

void Keyhook::OnDraw() {
	KeyCode keyCode = GetKeyCode(GetKey());
	string prefix = "";
	bool IsInRange = InRange(*p_D2CLIENT_MouseX, *p_D2CLIENT_MouseY);
	if (name.length() > 0) {
		if(IsInRange)
			prefix = name + "ÿc7 ";
		else
			prefix = name + "ÿc4 ";
	}

	string text = prefix + keyCode.literalName;
	if (timeout) {
		unsigned int time = (unsigned int)(3 - floor((double)(GetTickCount() - timeout) / 1000));
		if (time <= 0)
			timeout = 0;
		char num[100];
		_itoa_s(time, num, 100, 10);
		text = prefix + string(num) + " secs";
	}
	DWORD size = D2WIN_SetTextSize(0);
	wchar_t* keyText = AnsiToUnicode(text.c_str());
	D2WIN_DrawText(keyText, GetX(), GetY() + 10, IsInRange?7:4, 0);
	delete[] keyText;
	D2WIN_SetTextSize(size);
}

bool Keyhook::OnKey(bool up, BYTE kkey, LPARAM lParam) {
	if (timeout) {
		Lock();
		if (up) {
			if (kkey == VK_ESCAPE)
				kkey = 0;
			*key = (unsigned int)kkey;
			timeout = 0;
		}
		Unlock();
		return true;
	}
	return false;
}

unsigned int Keyhook::GetXSize() {
	KeyCode keyCode = GetKeyCode(GetKey());
	string prefix = "";
	if (name.length() > 0)
		prefix = name + ":ÿc4 ";
	string text = prefix + keyCode.literalName;
	DWORD width, fileNo;
	wchar_t* wString = AnsiToUnicode(text.c_str());
	DWORD oldFont = D2WIN_SetTextSize(0);
	D2WIN_GetTextWidthFileNo(wString, &width, &fileNo);
	D2WIN_SetTextSize(oldFont);
	delete[] wString;
	return width; 
}

unsigned int Keyhook::GetYSize() {
	return 10;
}
