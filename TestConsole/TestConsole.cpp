#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>

void ParseCmdlineW(const wchar_t* cmdLine,
	std::vector<wchar_t*>& vecArgs, std::wstring& holder)
{
	holder = cmdLine;
	wchar_t* firstAlphanumeric = nullptr;
	bool withinQuote = false;
	for (int i = 0; i < holder.size(); ++i)
	{
		wchar_t ch = holder[i];
		if (ch == L'\"' && withinQuote == false)
		{
			withinQuote = true;
			continue;
		}

		if (withinQuote == false)
		{
			if (ch != L' ' && ch != L'\t')
			{
				if (!firstAlphanumeric)
					firstAlphanumeric = &holder[i];
			}
			else
			{
				if (firstAlphanumeric)
				{
					holder[i] = L'\0';
					vecArgs.push_back(firstAlphanumeric);
					firstAlphanumeric = nullptr;
				}
			}
		}
		else // withinQuote == true
		{
			if (ch != L'\"')
			{
				if (!firstAlphanumeric)
					firstAlphanumeric = &holder[i];
			}
			else if (ch == L'\"')
			{
				if (firstAlphanumeric)
				{
					holder[i] = L'\0';
					vecArgs.push_back(firstAlphanumeric);
					firstAlphanumeric = nullptr;
				}
				withinQuote = false;
			}
		}
	}
	if (firstAlphanumeric)
	{
		vecArgs.push_back(firstAlphanumeric);
		firstAlphanumeric = nullptr;
	}
}


void ParseCmdlineA(const char* cmdLine,
	std::vector<char*>& vecArgs, std::string& holder)
{
	holder = cmdLine;
	char* firstAlphanumeric = nullptr;
	bool withinQuote = false;
	for (int i = 0; i < holder.size(); ++i)
	{
		char ch = holder[i];
		if (ch == '\"' && withinQuote == false)
		{
			withinQuote = true;
			continue;
		}

		if (withinQuote == false)
		{
			if (ch != ' ' && ch != '\t')
			{
				if (!firstAlphanumeric)
					firstAlphanumeric = &holder[i];
			}
			else
			{
				if (firstAlphanumeric)
				{
					holder[i] = '\0';
					vecArgs.push_back(firstAlphanumeric);
					firstAlphanumeric = nullptr;
				}
			}
		}
		else // withinQuote == true
		{
			if (ch != '\"')
			{
				if (!firstAlphanumeric)
					firstAlphanumeric = &holder[i];
			}
			else if (ch == '\"')
			{
				if (firstAlphanumeric)
				{
					holder[i] = '\0';
					vecArgs.push_back(firstAlphanumeric);
					firstAlphanumeric = nullptr;
				}
				withinQuote = false;
			}
		}
	}
	if (firstAlphanumeric)
	{
		vecArgs.push_back(firstAlphanumeric);
		firstAlphanumeric = nullptr;
	}
}

#ifdef UNICODE
#define ParseCmdlineT ParseCmdlineW
#else
#define ParseCmdlineT ParseCmdlineA
#endif

int main(int argc, char* argv[])
{
	// print out the cmd args in debugger
	for (int i = 0; i < argc; ++i)
	{
		::OutputDebugStringA(argv[i]);
		::OutputDebugStringA("\n");
	}

	return 0;
}

// Uncomment the lines below if you do not want to include <windows.h>
// to get WinMain but I can't do it here because this project calls the two
// functions, GetModuleFileNameA and OutputDebugStringA from <windows.h>
//#define HINSTANCE void*
//#define LPSTR const char*
//int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

int __stdcall WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
{
	std::vector<char*> vecArgs;
	std::string holder;

	char executablePath[MAX_PATH];
	DWORD res = ::GetModuleFileNameA(
		NULL,
		executablePath,
		MAX_PATH
	);

	if (res != 0)
	{
		vecArgs.push_back(executablePath);
		ParseCmdlineA(lpCmdLine, vecArgs, holder);
	}

	return main((int)vecArgs.size(), vecArgs.data());
}
