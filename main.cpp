#include "kletz.h"

int main() {

	DWORD pid = GetProcessId("javaw.exe");//Get the minecraft process id
	HANDLE processhandle = OpenProcess(OPENFLAGS, false, pid);//open the minecraft process so we can read and write into it

	std::vector<double>values = { 3 }; //we are looking for the double value 3 as it is the default minecraft reach value
  	double newreach = 4; //this is the new reach value
  
	std::vector<DWORD>locations = kletz::memory::ScanProcessMemory(processhandle, values, true, 0x02000000, 0x06FFFFFF); // vector locations will store all instances of the double value 3
	for (const auto i : locations) { //going trough all the found addresses 
		WriteProcessMemory(processhandle,(LPVOID)(i), &newreach, sizeof(double), 0);//writing the new reach value on every found address
	} 
	CloseHandle(processhandle);//closing handle of opened process
}
