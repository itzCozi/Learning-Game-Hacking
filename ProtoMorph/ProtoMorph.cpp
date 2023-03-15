// This is where the Assault Cube internal/external cheat will be written
#include <Windows.h>
#include <iostream>
#include <string>
#include <iostream>
#include <TlHelp32.h>
using namespace std;

class Variables {
  const string version = "1.0.0";

  struct vec3 {
    float x, y, z;
  }
};

class Functions {
  DWORD GetProcId(const char *procName) {
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE) {
      PROCESSENTRY32 procEntry;
      procEntry.dwSize = sizeof(procEntry);

      if (Process32First(hSnap, &procEntry)) {
        do {
          if (!_stricmp(procEntry.szExeFile, procName)) {
            procId = procEntry.th32ProcessID;
            break;
          }
        } while (Process32Next(hSnap, &procEntry));
      }
    }
    CloseHandle(hSnap);
    return procId;
  }
};


int main() { 
  std::cout << "Hello World!\n"; 

}