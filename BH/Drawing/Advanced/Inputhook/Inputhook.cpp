#include "Inputhook.h"
#include "../../../D2Ptrs.h"
#include "../../Basic/Framehook/Framehook.h"
#include "../../../Common.h"

using namespace std;
using namespace Drawing;

Inputhook::Inputhook(HookVisibility visibility, unsigned int x, unsigned int y, unsigned int xSize, std::string formatString, ...) :
 Hook(visibility, x, y) {
	SetXSize(xSize);
	SetFont(0);
	SetActive(false);
	SetCursorState(true);
	ResetCursorTick();
	ResetSelection();
	textPos = 0;
	char buffer[4096];
	va_list arg;
	va_start(arg, formatString);
	vsprintf_s(buffer, 4096, formatString.c_str(), arg);
	va_end(arg);
	text = buffer;
	SetCursorPosition(text.length());
}

Inputhook::Inputhook(HookGroup* group, unsigned int x, unsigned int y, unsigned int xSize, std::string formatString, ...) :
 Hook(group, x, y) {
	SetXSize(xSize);
	SetFont(0);
	SetActive(false);
	SetCursorState(true);
	ResetCursorTick();
	ResetSelection();
	textPos = 0;
	char buffer[4096];
	va_list arg;
	va_start(arg, formatString);
	vsprintf_s(buffer, 4096, formatString.c_str(), arg);
	va_end(arg);
	text = buffer;
	SetCursorPosition(text.length());
 }

 void Inputhook::SetText(string newText, ...) {
	char buffer[4096];
	va_list arg;
	va_start(arg, newText);
	vsprintf_s(buffer, 4096, newText.c_str(), arg);
	va_end(arg);
	text = buffer;
 }

 void Inputhook::SetFont(unsigned int newFont) {
	if (newFont >=  0 && newFont < 16) {
		Lock();
		font = newFont;
		Unlock();
	}
 }

 void Inputhook::SetXSize(unsigned int newXSize) {
	// if (newXSize > 0 && (Hook::GetScreenWidth() - GetX()) < newXSize) {
		 Lock();
		 xSize = newXSize;
		 Unlock();
	// }
 }

 void Inputhook::CursorTick() {
	  if (cursorTick % 30 == 0) { 
		  ResetCursorTick(); 
		  ToggleCursor(); 
	  }
	  cursorTick++;
 }

 void Inputhook::SetCursorPosition(unsigned int newPosition) {
	 if (newPosition >= 0 && newPosition <= text.length()) {
		Lock();
		cursorPos = newPosition;
		Unlock();
	 }
 }

 void Inputhook::SetSelectionPosition(unsigned int pos) {
	 if (pos < text.length()) {
		 Lock();
		 selectPos = pos;
		 Unlock();
	 }
 }

 void Inputhook::SetSelectionLength(unsigned int length) {
	 if (length <= text.length()) {
		 Lock();
		 selectLength = length;
		 Unlock();
	 }
 }

void Inputhook::IncreaseCursorPosition(unsigned int len) { 
	Lock();
	 SetCursorPosition(cursorPos + len); 
	 if ((textPos + GetCharacterLimit()) < cursorPos)
		 textPos = cursorPos - GetCharacterLimit();
	 Unlock();
};

void Inputhook::DecreaseCursorPosition(unsigned int len) { 
	Lock();
	SetCursorPosition(cursorPos - len); 
	 if ((cursorPos - textPos) == -1 && textPos > 0)
		 textPos -= len;
	Unlock();
}; 

unsigned int Inputhook::GetCharacterLimit() {
	return (GetXSize() / Texthook::GetTextSize("A", GetFont()).x);
}

 void Inputhook::OnDraw() {
	 Lock();
	 //Font height
	 unsigned int height[] = {10,11,18,24,10,13,7,13,10,12,8,8,7,12};
	 
	 //Current text width
	 POINT textSize = Texthook::GetTextSize(GetText().substr(textPos, GetCursorPosition() - textPos), GetFont());

	 //Draw the outline box!
	 RECT pRect  = {GetX(), GetY(), GetX() + GetXSize(), GetY() + height[GetFont()] + 4};
	 D2GFX_DrawRectangle(GetX(), GetY(), GetX() + GetXSize(), GetY() + height[GetFont()] + 4, 0, BTFull);
	 Framehook::DrawRectStub(&pRect);
	 string drawnText = text;

	 //Draw the text in!
	 int len = drawnText.length() - textPos;
	 if (len > (int)GetCharacterLimit())
		len = GetCharacterLimit();
	drawnText = drawnText.substr(textPos, len);

	 
	 if (IsSelected()) {
		 drawnText.insert(GetSelectionPosition() + GetSelectionLength(), "ÿc0");
		 drawnText.insert(GetSelectionPosition(), "ÿc9");
	 }

	
	 DWORD oldFont = D2WIN_SetTextSize(GetFont());
	 wchar_t* wText = AnsiToUnicode(drawnText.c_str());
	 D2WIN_DrawText(wText, GetX() + 3, GetY() + 3 + height[GetFont()], 0, 0);
	 delete[] wText;
	 D2WIN_SetTextSize(oldFont);

	 //Draw the cursor!
	 CursorTick();
	 if (ShowCursor() && IsActive())
		 D2GFX_DrawLine(GetX() + textSize.x + 2, GetY() + 3, GetX() + textSize.x + 2, GetY() + textSize.y, 255, 0);

	 Unlock();
 }


 bool Inputhook::OnKey(bool up, BYTE key, LPARAM lParam) {
	 if (!IsActive())
		 return false;
	 Lock();
	 bool ctrlState = ((GetKeyState(VK_LCONTROL) & 0x80) || (GetKeyState(VK_RCONTROL) & 0x80));
	 bool shiftState = ((GetKeyState(VK_LSHIFT) & 0x80) || (GetKeyState(VK_RSHIFT) & 0x80));
	 switch(key) {
		case VK_BACK:
			if (!up)
				Backspace();
		break;
		case VK_DELETE:
			if (!up && text.length() != GetCursorPosition()) {
				Erase(GetCursorPosition(), 1);
			}
		break;
		case VK_ESCAPE:
			if (up)
				SetActive(false);
		break;
		case VK_LEFT:
			if (!up && GetCursorPosition() != 0) {
				if (shiftState) {
					if (IsSelected()) {
						if (GetSelectionPosition() == GetCursorPosition()) {
							SetSelectionPosition(GetSelectionPosition() - 1);
							SetSelectionLength(GetSelectionLength() + 1);
						} else {
							SetSelectionLength(GetSelectionLength() - 1);
						}
					} else {
						SetSelectionPosition(GetCursorPosition() - 1);
						SetSelectionLength(1);
					}
				}
				DecreaseCursorPosition(1);
			}
		break;
		case VK_RIGHT:
			if (!up && GetCursorPosition() != text.length()) {
				if (shiftState) {
					if (IsSelected()) {
						if (GetCursorPosition() == (GetSelectionPosition() + GetSelectionLength())) {
							SetSelectionLength(GetSelectionLength() + 1);
						} else {
							SetSelectionPosition(GetSelectionPosition() + 1);
							SetSelectionLength(GetSelectionLength() - 1);
						}
					} else {
						SetSelectionPosition(GetCursorPosition());
						SetSelectionLength(1);
					}
				}
				IncreaseCursorPosition(1);
			}
		break;
		default:
			if (up)
				return true;

			if (ctrlState) {
				//Select All
				if (key == 0x41) {
					SetSelectionPosition(0);
					SetSelectionLength(text.length());
				}
				OpenClipboard(NULL);
				//Paste
				if (key == 0x56) {
					HANDLE pHandle = GetClipboardData(CF_TEXT);
					if (!pHandle)
						return true;
					InputText((char*)GlobalLock(pHandle));
				}
				//Copy & Cut
				if (key == 0x43 || key == 0x58) {
					if (!IsSelected() || text.length() == 0)
						return true;
					
					Lock();
					string mText = text.substr(GetSelectionPosition(), GetSelectionLength());
			
					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (mText.size() + 1) * sizeof(CHAR)); 
					char* szStr = (char*)GlobalLock(hGlobal);
					memcpy(szStr, mText.c_str(), mText.size() * sizeof(CHAR));
					GlobalUnlock(hGlobal);
					EmptyClipboard();
					SetClipboardData(CF_TEXT, hGlobal);

					if (key == 0x58) {
						Erase(GetSelectionPosition(), GetSelectionLength());

						ResetSelection();
					}
					Unlock();
				}
				CloseClipboard();
				return true;
			}

			BYTE layout[256];
			WORD out[2];
			CHAR szChar[10];
			GetKeyboardState(layout);
			if (ToAscii(key, (lParam & 0xFF0000), layout, out, 0) == 0)
				return false;
			sprintf_s(szChar, sizeof(szChar), "%c", out[0]);

			InputText(szChar);
		break;
	 }
	 return true;
 }

 bool Inputhook::OnLeftClick(bool up, unsigned int x, unsigned int y) {
	 if (InRange(x, y)) {
		 if (up)
			 SetActive(true);
		 if (GetLeftClickHandler())
			 GetLeftClickHandler()(up, this, GetLeftClickVoid());
		 return true;
	 } else {
		 SetActive(false);
	 }
	 return false;
 }

 bool Inputhook::OnRightClick(bool up, unsigned int x, unsigned int y) {
		if (!InRange(x, y))
			if (GetLeftClickHandler())
				return GetRightClickHandler()(up, this, GetRightClickVoid());
		return false;
 }

 void Inputhook::InputText(string newText) {
	 Lock();

	 //If we have text selected, replace the text with the new text
	 if (IsSelected()) {
		 Replace(GetSelectionPosition(), GetSelectionLength(), newText);
		 ResetSelection();
	 //Otherwise just add the text at the cursor position.
	 } else {
		 text.insert(GetCursorPosition(), newText);
		 IncreaseCursorPosition(newText.length());
	 }

	 Unlock();
 }

 void Inputhook::Backspace() {
	 if (GetCursorPosition() == 0)
		 return;
	 Lock();
	 if (IsSelected()) {
		 Erase(GetSelectionPosition(), GetSelectionLength());
		 ResetSelection();
	 } else {
		 text.erase(GetCursorPosition() - 1, 1);
		 DecreaseCursorPosition(1);
		 if (textPos > 0)
			textPos -= 1;
	 }
	 Unlock();
 }

void Inputhook::Replace(unsigned int pos, unsigned int len, std::string str) {
	if ((unsigned int)pos + len > text.length())
		return;
	Lock();
	text.replace(pos, len, str);
	SetCursorPosition(pos + str.length());
	Unlock();
}

void Inputhook::Erase(unsigned int pos, unsigned int len) {
	if ((unsigned int)pos + len > text.length())
		return;
	Lock();
	text.erase(pos,len);
	SetCursorPosition(pos);
	Unlock();
}