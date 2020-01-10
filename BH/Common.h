/**
*
* Common.h
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

#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "D2Ptrs.h"

void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);

std::wstring string_to_wstring(const std::string& str);
std::string wstring_to_string(const std::wstring& wstr);

wchar_t* AnsiToUnicode(const char* str);
char* UnicodeToAnsi(const wchar_t* str);
std::wstring GetColorCode(int ColNo);
template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

template< class type> std::string to_string( const type & value)
{ std::stringstream ss; ss << value; return ss.str(); }

bool IsTrue(const char *str);
bool StringToBool(std::string str);
int StringToNumber(std::string str);

std::string Trim(std::string source);

void PrintText(DWORD Color, char *szText, ...);

struct KeyCode {
	std::string name;
	unsigned int value;
	std::string literalName;
};

KeyCode GetKeyCode(unsigned int nKey);
KeyCode GetKeyCode(const char* name);
ULONGLONG BHGetTickCount(void);

std::string string_format(const std::string fmt_str, ...);


VOID *memcpy2(void *dest, const void *src, size_t count);
HANDLE OpenFileRead(char *filename);
BYTE *AllocReadFile(char *filename);
DWORD ReadFile(HANDLE hFile, void *buf, DWORD len);
char *GetMyFileNameStrrchr(char *dest, char ch);

long CalculateDistance(const POINT& pt1, const POINT& pt2);
long CalculateAngle(const POINT& pt1, const POINT& pt2);
long CalculateDistance(long x1, long y1, long x2, long y2);
BOOL PtInCircle(const POINT& pt, const POINT& ptOrigin, int nRadius);
void NormalizeAngle(int& rAngle);
void NormalizeRect(RECT& rRect);
void NormalizeRect(LPRECT lpRect);
long CalculateAngle(long x1, long y1, long x2, long y2);
POINT CalculatePointOnTrack(const POINT& ptOrigin, int nRadius, int nAngle);
POINT CalculateRandomPosition(const POINT& ptOrigin, int nRadiusMin, int nRadiusMax, int nAngleMin/*=0*/, int nAngleMax/*=360*/);
char *commaprint(unsigned long n);
