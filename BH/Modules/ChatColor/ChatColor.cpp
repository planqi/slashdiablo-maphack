#include "ChatColor.h"
#include "../../BH.h"
#include "../../D2Ptrs.h"
#include "../../D2Stubs.h"

void ChatColor::Init() {
	
}

void Print(DWORD color, char* format, ...) {
	va_list vaArgs;
	va_start(vaArgs, format);
	int len = _vscprintf(format, vaArgs) + 1;
	char* str = new char[len];
	vsprintf_s(str, len, format, vaArgs);
	va_end(vaArgs);

	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CODE_PAGE, 0, str, -1, wstr, len);
	D2CLIENT_PrintGameString(wstr, color);
	delete[] wstr;

	delete[] str;
}

void ChatColor::OnGameJoin(const string& name, const string& pass, int diff) {
	inGame = true;
}

void ChatColor::OnGameExit() {
	inGame = false;
}

void ChatColor::OnLoad() {
	LoadConfig();
}

void ChatColor::LoadConfig() {
	whisperColors.clear();

	BH::config->ReadAssoc("Whisper Color", whisperColors);
}

void ChatColor::OnChatPacketRecv(BYTE* packet, bool* block) {
	if (packet[1] == 0x0F && inGame) {
		unsigned int event_id = *(unsigned short int*)&packet[4];

		if (event_id == 4) {
			const char* from = (const char*)&packet[28];
			unsigned int fromLen = strlen(from);

			const char* message = (const char*)&packet[28 + fromLen + 1];
			unsigned int messageLen = strlen(message);

			bool replace = false;
			int color = 0;
			if (whisperColors.find(from) != whisperColors.end()) {
				replace = true;
				color = whisperColors[from];
			}

			if (replace) {
				*block = true;

				Print(color, "%s | %s", from, message);
			}
		}
	}
}
