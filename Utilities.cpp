
#pragma once
#define _CRT_SECURE_NO_WARNINGS


#include "Utilities.h"
#include <fstream>
#include <memory>
#include <Psapi.h>
#include "xor.h"
std::string console_title;
bool FileLog = false;
std::ofstream logFile;

void Utilities::OpenConsole(std::string Title)
{
	AllocConsole();
	FILE *conin, *conout;

	freopen_s(&conin, XorStr("conin$"), XorStr("r"), stdin);
	freopen_s(&conout, XorStr("conout$"), XorStr("w"), stderr);
	freopen_s(&conout, XorStr("conout$"), XorStr("w"), stdout);

	if (Title.length() > 0) {
		SetConsoleTitleA(Title.c_str());
		console_title = Title;
	}
}


void Utilities::CloseConsole()
{
	FreeConsole();
	HWND hWnd = FindWindowA(NULL, console_title.c_str());
	if (hWnd) {
		DestroyWindow(hWnd);
		hWnd = NULL;
	}
}

void Utilities::Log(const char* fmt, ...)
{
	if (!fmt)
		return;
	if (strlen(fmt) < 2)
		return;

	va_list va_alist;
	char logBuf[256] = { 0 };

	va_start(va_alist, fmt);
	_vsnprintf(logBuf + strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	if (logBuf[0] != XorStr('\0')) {
		SetConsoleColor(FOREGROUND_INTENSE_GREEN);
		printf(XorStr("[%s]"), GetTimeString().c_str());
		SetConsoleColor(FOREGROUND_WHITE);
		printf(XorStr(": %s\n"), logBuf);
	}

	if (FileLog) {
		logFile << logBuf << std::endl;
	}
}

std::string Utilities::GetTimeString()
{
	time_t current_time;
	struct tm* time_info;
	static char timeString[10];

	time(&current_time);
	time_info = localtime(&current_time);

	strftime(timeString, sizeof(timeString), XorStr("%I:%M%p"), time_info);
	return timeString;
}

void Utilities::SetConsoleColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Utilities::EnableLogFile(std::string filename)
{
	logFile.open(filename.c_str());
	if (logFile.is_open())
		FileLog = true;
}

DWORD Utilities::Memory::WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle) {
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data) {
		if (*szMask == XorStr('x') && *Mask != *Data) {
			return false;
		}
	}
	return (*szMask) == 0;
}

uint64_t Utilities::Memory::FindPatternIDA(const char* szModule, const char* szSignature)
{

#define INRANGE(x, a, b) (x >= a && x <= b)
#define getBits(x) (INRANGE((x & (~0x20)), XorStr('A'), XorStr('F')) ? ((x & (~0x20)) - XorStr('A') + 0xa) : (INRANGE(x, XorStr('0'), XorStr('9')) ? x - XorStr('0') : 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
	DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
	DWORD endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = szSignature;
	DWORD firstMatch = 0;
	for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
		if (!*pat)
			return firstMatch;
		if (*(PBYTE)pat == XorStr('\?') || *(BYTE*)pCur == getByte(pat)) {
			if (!firstMatch)
				firstMatch = pCur;
			if (!pat[2])
				return firstMatch;
			if (*(PWORD)pat == XorStr('\?\?') || *(PBYTE)pat != XorStr('\?'))
				pat += 3;
			else
				pat += 2; //one ?
		}
		else {
			pat = szSignature;
			firstMatch = 0;
		}
	}
	return NULL;
}

DWORD Utilities::Memory::FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1) {
		if (bCompare((BYTE*)(Address + c), Mask, szMask)) {
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

DWORD Utilities::Memory::FindTextPattern(std::string moduleName, char* string)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	int len = strlen(string);
	char* szMask = new char[len + 1];
	for (int i = 0; i < len; i++) {
		szMask[i] = XorStr('x');
	}
	szMask[len] = XorStr('\0');

	for (DWORD c = 0; c < Length; c += 1) {
		if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask)) {
			return (DWORD)(Address + c);
		}
	}
	return 0;
}


bool Utilities::Memory::VMTManager::Initialise(DWORD* InstancePointer)
{
	Instance = InstancePointer;
	OriginalTable = (DWORD*)*InstancePointer;
	int VMTSize = MethodCount(InstancePointer);
	size_t TableBytes = VMTSize * 4;

	CustomTable = (DWORD*)malloc(TableBytes + 8);
	if (!CustomTable)
		return false;
	memcpy((void*)CustomTable, (void*)OriginalTable, VMTSize * 4);

	*InstancePointer = (DWORD)CustomTable;

	initComplete = true;
	return true;
}

int Utilities::Memory::VMTManager::MethodCount(DWORD* InstancePointer)
{
	DWORD* VMT = (DWORD*)*InstancePointer;
	int Index = 0;
	int Amount = 0;
	while (!IsBadCodePtr((FARPROC)VMT[Index])) {
		if (!IsBadCodePtr((FARPROC)VMT[Index])) {
			Amount++;
			Index++;
		}
	}

	return Amount;
}

DWORD Utilities::Memory::VMTManager::HookMethod(DWORD NewFunction, int Index)
{
	if (initComplete) {
		CustomTable[Index] = NewFunction;
		return OriginalTable[Index];
	}
	else
		return NULL;
}

void Utilities::Memory::VMTManager::UnhookMethod(int Index)
{
	if (initComplete)
		CustomTable[Index] = OriginalTable[Index];
	return;
}

void Utilities::Memory::VMTManager::RestoreOriginal()
{
	if (initComplete) {
		*Instance = (DWORD)OriginalTable;
	}
	return;
}

void Utilities::Memory::VMTManager::RestoreCustom()
{
	if (initComplete) {
		*Instance = (DWORD)CustomTable;
	}
	return;
}

DWORD Utilities::Memory::VMTManager::GetOriginalFunction(int Index)
{
	return OriginalTable[Index];
}























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































