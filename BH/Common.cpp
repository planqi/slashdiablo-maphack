/**
*
* Common.cpp
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

#include <string>
#include <vector>
#include <codecvt>
#include <Windows.h>
#include "BH.h"
#include "Common.h"
#include <memory>
using namespace std;

void Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
wstring string_to_wstring(const string& str) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

string wstring_to_string(const wstring& wstr) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

wchar_t* AnsiToUnicode(const char* str)
{
	wchar_t* buf = NULL;
	int len = MultiByteToWideChar(CODE_PAGE, 0, str, -1, buf, 0);
	buf = new wchar_t[len];
	MultiByteToWideChar(CODE_PAGE, 0, str, -1, buf, len);
	return buf;
}

char* UnicodeToAnsi(const wchar_t* str)
{
	char* buf = NULL;
	int len = WideCharToMultiByte(CODE_PAGE, 0, str, -1, buf, 0, "?", NULL);
	buf = new char[len];
	WideCharToMultiByte(CODE_PAGE, 0, str, -1, buf, len, "?", NULL);
	return buf;
}

std::wstring GetColorCode(int ColNo)
{
	wchar_t* pCol = D2LANG_GetLocaleText(3994);
	wostringstream Result;
	Result << pCol << (wchar_t) (ColNo + '0');
	return Result.str();
}

std::string Trim(std::string source) {
	source = source.erase(0, source.find_first_not_of(" "));
	source = source.erase(source.find_last_not_of(" ") + 1);
	source = source.erase(0, source.find_first_not_of("\t"));
	source = source.erase(source.find_last_not_of("\t") + 1);
	return source;
}

bool IsTrue(const char *str) {
	return (_stricmp(str, "1") == 0 || _stricmp(str, "y") == 0 || _stricmp(str, "yes") == 0 || _stricmp(str, "true") == 0);
}

bool StringToBool(std::string str) {
	return IsTrue(str.c_str());
}

int StringToNumber(std::string str) {
	int ret;
	if (!str.find("0x")) {
		from_string<int>(ret, str, std::hex);
	} else {
		from_string<int>(ret, str, std::dec);
	}
	return ret;
}

void PrintText(DWORD Color, char *szText, ...) {
	char szBuffer[152] = {0};
	va_list Args;
	va_start(Args, szText);
	vsprintf_s(szBuffer, 152, szText, Args);
	va_end(Args); 
	wchar_t Buffer[0x130];
	MultiByteToWideChar(CODE_PAGE, 1, szBuffer, 152, Buffer, 304);
	D2CLIENT_PrintGameString(Buffer, Color);	
}

KeyCode pCodes[] = {
	{"None", 0, "Not Set"},
	{"VK_BACK", 0x08, "Backspace"},
	{"VK_TAB", 0x09, "Tab"},
	{"VK_CLEAR", 0x0C, "Clear"},
	{"VK_RETURN", 0x0D, "Enter"},
	{"VK_SHIFT", 0x10, "Shift"},
	{"VK_CONTROL", 0x11, "Control"},
	{"VK_ALT", 0x12, "Alt"},
	{"VK_PAUSE", 0x13, "Pause"},
	{"VK_CAPSLOCK", 0x14, "Caps Lock"},
	{"VK_ESCAPE", 0x1B, "Esc"},
	{"VK_SPACE", 0x20, "Space"},
	{"VK_PAGEUP", 0x21, "Page Up"},
	{"VK_PAGEDN", 0x22, "Page Down"},
	{"VK_END", 0x23, "End"},
	{"VK_HOME", 0x24, "Home"},
	{"VK_LEFT", 0x25, "Left"},
	{"VK_UP", 0x26, "Up"},
	{"VK_RIGHT", 0x27, "Right"},
	{"VK_DOWN", 0x28, "Down"},
	{"VK_SELECT", 0x29, "Select"},
	{"VK_PRINT", 0x2A, "Print"},
	{"VK_EXECUTE", 0x2B, "Execute"},
	{"VK_SNAPSHOT", 0x2C, "Prt Scr"},
	{"VK_INSERT", 0x2D, "Insert"},
	{"VK_DELETE", 0x2E, "Delete"},
	{"VK_HELP", 0x2F, "Help"},
	{"VK_0", 0x30, "0"}, {"VK_1", 0x31, "1"}, {"VK_2", 0x32, "2"}, {"VK_3", 0x33, "3"},
	{"VK_4", 0x34, "4"}, {"VK_5", 0x35, "5"}, {"VK_6", 0x36, "6"}, {"VK_7", 0x37, "7"},
	{"VK_8", 0x38, "8"}, {"VK_9", 0x39, "9"},
	{"VK_A", 0x41, "A"}, {"VK_B", 0x42, "B"}, {"VK_C", 0x43, "C"}, {"VK_D", 0x44, "D"},
	{"VK_E", 0x45, "E"}, {"VK_F", 0x46, "F"}, {"VK_G", 0x47, "G"}, {"VK_H", 0x48, "H"},
	{"VK_I", 0x49, "I"}, {"VK_J", 0x4A, "J"}, {"VK_K", 0x4B, "K"}, {"VK_L", 0x4C, "L"},
	{"VK_M", 0x4D, "M"}, {"VK_N", 0x4E, "N"}, {"VK_O", 0x4F, "O"}, {"VK_P", 0x50, "P"},
	{"VK_Q", 0x51, "Q"}, {"VK_R", 0x52, "R"}, {"VK_S", 0x53, "S"}, {"VK_T", 0x54, "T"},
	{"VK_U", 0x55, "U"}, {"VK_V", 0x56, "V"}, {"VK_W", 0x57, "W"}, {"VK_X", 0x58, "X"},
	{"VK_Y", 0x59, "Y"}, {"VK_Z", 0x5A, "Z"},
	{"VK_NUMPAD0", 0x60, "Numpad 0"}, {"VK_NUMPAD1", 0x61, "Numpad 1"},
	{"VK_NUMPAD2", 0x62, "Numpad 2"}, {"VK_NUMPAD3", 0x63, "Numpad 3"},
	{"VK_NUMPAD4", 0x64, "Numpad 4"}, {"VK_NUMPAD5", 0x65, "Numpad 5"},
	{"VK_NUMPAD6", 0x66, "Numpad 6"}, {"VK_NUMPAD7", 0x67, "Numpad 7"},
	{"VK_NUMPAD8", 0x68, "Numpad 8"}, {"VK_NUMPAD9", 0x69, "Numpad 9"},
	{"VK_NUMPADMULTIPLY", 0x6A, "Numpad *"}, {"VK_NUMPADADD", 0x6B, "Numpad +"},
	{"VK_NUMPADSUBTRACT", 0x6D, "Numpad -"}, {"VK_NUMPADDECIMAL", 0x6E, "Numpad ."}, 
	{"VK_NUMPADDIVIDE", 0x6F, "Numpad /"},
	{"VK_F1", 0x70, "F1"}, {"VK_F2", 0x71, "F2"}, {"VK_F3", 0x72, "F3"}, {"VK_F4", 0x73, "F4"},
	{"VK_F5", 0x74, "F5"}, {"VK_F6", 0x75, "F6"}, {"VK_F7", 0x76, "F7"}, {"VK_F8", 0x77, "F8"},
	{"VK_F9", 0x78, "F9"}, {"VK_F10", 0x79, "F10"}, {"VK_F11", 0x7A, "F11"}, {"VK_F12", 0x7B, "F12"},
	{"VK_F13", 0x7C, "F13"}, {"VK_F14", 0x7D, "F14"}, {"VK_F15", 0x7E, "F15"}, {"VK_F16", 0x7F, "F16"},
	{"VK_NUMLOCK", 0x90, "Numlock"}, {"VK_SCROLL", 0x91, "Scrolllock"}, {"VK_LSHIFT", 0xA0, "Left Shift"},
	{"VK_RSHIFT", 0xA1, "Right Shift"}, {"VK_LCTRL", 0xA2, "Left Ctrl"}, {"VK_RCTRL", 0xA3, "Right Ctrl"},
	{"VK_LMENU", 0xA4, "Left Menu"}, {"VK_RMENU", 0xA5, "Right Menu"}, 
	{"VK_SEMICOLON", 0xBA, ";"}, {"VK_PLUS", 0xBB, "+"}, {"VK_COMMA", 0xBC, ","}, {"VK_MINUS", 0xBD, "-"},
	{"VK_PERIOD", 0xBE, "."}, {"VK_FORWARDSLASH", 0xBD, "/"}, {"VK_TILDE", 0xBF, "~"},
	{"VK_LEFTBRACKET", 0xDB, "["}, {"VK_BACKSLASH", 0xDC, "\\"}, {"VK_RIGHTBRACKET", 0xDD, "]"},
	{"VK_QUOTE", 0xDE, "'"}};

KeyCode GetKeyCode(unsigned int nKey) {
	for (int n = 1; n < (sizeof(pCodes) / sizeof(pCodes[0])); n++)
		if (nKey == pCodes[n].value)
			return pCodes[n];
	return pCodes[0];
}
KeyCode GetKeyCode(const char* name) {
	for (int n = 1; n < (sizeof(pCodes) / sizeof(pCodes[0])); n++)
		if (!_stricmp(name, pCodes[n].name.c_str()))
			return pCodes[n];
	return pCodes[0];
}

ULONGLONG BHGetTickCount(void) {
    static bool first = true;
    static ULONGLONG (*pGetTickCount64)(void);

    if (first) {
        HMODULE hlib = LoadLibraryA("KERNEL32.DLL");
        pGetTickCount64 = (ULONGLONG(*)())GetProcAddress(hlib, "GetTickCount64");
        first = false;
    }
    if (pGetTickCount64) {
        return pGetTickCount64();
	}
    return (ULONGLONG)GetTickCount();
}

// See: http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
std::string string_format(const std::string fmt_str, ...) {
	int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::string str;
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(formatted.get());
}


VOID *memcpy2(void *dest, const void *src, size_t count)
{
	return (char *)memcpy(dest, src, count) + count;
}

HANDLE OpenFileRead(char *filename)
{
	return CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}
BYTE *AllocReadFile(char *filename)
{
	HANDLE hFile = OpenFileRead(filename);
	int filesize = GetFileSize(hFile, 0);
	if (filesize <= 0) return 0;
	BYTE *buf = new BYTE[filesize];
	ReadFile(hFile, buf, filesize);
	CloseHandle(hFile);
	return buf;
}
DWORD ReadFile(HANDLE hFile, void *buf, DWORD len)
//NOTE :- validates len bytes of buf
{
	DWORD numdone = 0;
	ReadFile(hFile, buf, len, &numdone, NULL);
	return numdone;
}

char *GetMyFileNameStrrchr(char *dest, char ch)
{
	GetModuleFileName(BH::instance, dest, MAX_PATH);
	return strrchr(dest, ch) + 1;
}

#define PI 3.1415926535

long CalculateDistance(const POINT& pt1, const POINT& pt2)
{
	return CalculateDistance(pt1.x, pt1.y, pt2.x, pt2.y);
}

long CalculateAngle(const POINT& pt1, const POINT& pt2)
{
	return CalculateAngle(pt1.x, pt1.y, pt2.x, pt2.y);
}

long CalculateDistance(long x1, long y1, long x2, long y2)
{
	return (long)::sqrt((double)((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

BOOL PtInCircle(const POINT& pt, const POINT& ptOrigin, int nRadius)
{
	return CalculateDistance(pt, ptOrigin) < ::abs(nRadius);
}

void NormalizeAngle(int& rAngle)
{
	if (::abs(rAngle) >= 360)
		rAngle %= 360;

	if (rAngle < 0)
		rAngle += 360;
}

void NormalizeRect(RECT& rRect)
{
	NormalizeRect(&rRect);
}

void NormalizeRect(LPRECT lpRect)
{
	if (lpRect == NULL)
		return;

	long temp;
	if (lpRect->left > lpRect->right)
	{
		temp = lpRect->left;
		lpRect->left = lpRect->right;
		lpRect->right = temp;
	}

	if (lpRect->top > lpRect->bottom)
	{
		temp = lpRect->top;
		lpRect->top = lpRect->bottom;
		lpRect->bottom = temp;
	}
}

long CalculateAngle(long x1, long y1, long x2, long y2)
{
	// mathematic stuff, now thanks God I haven't forgot all of them...
	if (x1 == x2 && y1 == y2)
		return 0;

	double fAngle = 0.0;

	if (x1 == x2)
	{
		// vertical special case
		fAngle = y2 > y1 ? 270.0 : 90.0;
	}
	else if (y1 == y2)
	{
		// horizontal special case
		fAngle = x2 > x1 ? 0.0 : 180.0;
	}
	else
	{
		// common case
		fAngle = ::atan(((double)y2 - (double)y1) / ((double)x2 - (double)x1)) * 180.0 / PI;

		// determine the phases (1-4)
		// Phases allocation figure:
		/*
		y
		|
		P2     |    P1
		|
		-----------+----------->x
		|
		P3     |    P4
		|
		*/
		int nPhase = 0;
		if (y2 < y1)
			nPhase = x2 > x1 ? 1 : 2;
		else
			nPhase = x2 > x1 ? 4 : 3;

		// adjust the angle according to phases
		switch (nPhase)
		{
		case 1:
			fAngle = -fAngle;
			break;

		case 2:
			fAngle = 180.0 - fAngle;
			break;

		case 3:
			fAngle = 180.0 - fAngle;
			break;

		case 4:
			fAngle = 360.0 - fAngle;
			break;

		default:
			fAngle = 0.0;
			break;
		}
	}

	return (long)fAngle;
}

POINT CalculatePointOnTrack(const POINT& ptOrigin, int nRadius, int nAngle)
{
	if (nRadius == 0)
		return ptOrigin;

	NormalizeAngle(nAngle);

	POINT pt;
	pt.x = long(double(ptOrigin.x) + ::cos((double)nAngle * PI / 180.0) * (double)nRadius);
	pt.y = long(double(ptOrigin.y) - ::sin((double)nAngle * PI / 180.0) * (double)nRadius);
	return pt;
}

POINT CalculateRandomPosition(const POINT& ptOrigin, int nRadiusMin, int nRadiusMax, int nAngleMin/*=0*/, int nAngleMax/*=360*/)
{
	// Data validation
	nRadiusMin = max(0, nRadiusMin);
	nRadiusMax = max(0, nRadiusMax);

	NormalizeAngle(nAngleMin);
	NormalizeAngle(nAngleMax);

	const int R1 = min(nRadiusMin, nRadiusMax);
	const int R2 = max(nRadiusMin, nRadiusMax);
	const int A1 = min(nAngleMin, nAngleMax);
	const int A2 = max(nAngleMin, nAngleMax);

	const int R = (R1 == R2) ? R1 : (R1 + (::rand() % (R2 - R1))); // Final radius
	const int A = (A1 == A2) ? A1 : (A1 + (::rand() % (A2 - A1))); // Final angle

	return CalculatePointOnTrack(ptOrigin, R, A);
}

char *commaprint(unsigned long n)
{
	int comma = '\0';
	char retbuf[30];
	char *p = &retbuf[sizeof(retbuf) - 1];
	int i = 0;

	if (comma == '\0') {
		struct lconv *lcp = localeconv();
		if (lcp != NULL) {
			if (lcp->thousands_sep != NULL &&
				*lcp->thousands_sep != '\0')
				comma = *lcp->thousands_sep;
			else	comma = ',';
		}
	}

	*p = '\0';

	do {
		if (i % 3 == 0 && i != 0)
			*--p = comma;
		*--p = '0' + n % 10;
		n /= 10;
		i++;
	} while (n != 0);

	return p;
}