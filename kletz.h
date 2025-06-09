#include <windows.h>
#include <vector>
#include <algorithm>

namespace kletz::memory {
	template<typename T> [[nodiscard]] std::vector<uintptr_t> ScanMemory(HANDLE hProc, const T* values, size_t count, uintptr_t min = 0, uintptr_t max = 0) noexcept {
		std::vector<uintptr_t> matches;
		MEMORY_BASIC_INFORMATION mbi;
		
		for (uintptr_t addr = min; !max || addr < max; addr = mbi.RegionSize ? addr + mbi.RegionSize : max) {
			if (!VirtualQueryEx(hProc, (LPCVOID)addr, &mbi, sizeof(mbi)) break;
			if (mbi.State != MEM_COMMIT || mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) continue;

			const auto size = max ? std::min(mbi.RegionSize, max - addr) : mbi.RegionSize;
			std::vector<T> buffer(size / sizeof(T));
			
			if (ReadProcessMemory(hProc, (LPCVOID)addr, buffer.data(), size, nullptr)) {
				for (size_t i = 0; i < buffer.size(); ++i) {
					if (std::find(values, values + count, buffer[i]) != values + count) {
						matches.emplace_back(addr + i * sizeof(T));
					}
				}
			}
		}
		return matches;
	}
}
