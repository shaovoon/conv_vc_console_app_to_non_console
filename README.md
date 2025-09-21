# C++: Convert Console Project to Non-Console

This tip is written in response to the [Q&A question](https://www.codeproject.com/Questions/5376312/How-do-I-create-a-Cplusplus-console-program-that-d) posted by [Eduardo Quintana](https://www.codeproject.com/script/Membership/View.aspx?mid=122721) from Brazil. I encountered this same problem many years back when I downloaded an OpenGL tutorial that launched a console window before opening an OpenGL window. Having a console is extremely useful to log the messages to screen for troubleshooting but as a final product, only the OpenGL window should be shown to the user. In this tip, I&#39;ll show you how to convert a Visual C++ Console project not to show the console screen.

Firstly, to open __Project Properties__ dialog, right-click on the project in the Solution Explorer and select __Properties__ at the bottom of the pop-up menu or you can type __Alt+Enter__ key. Select the __Linker__->__System__ on the tree on the left panel. Then, change the __Configuration__ dropdown to __All Configurations__ and __Platform__ dropdown to __All Platforms__. Finally, change the Subsystem from `Console (/SUBSYSTEM:CONSOLE)` to `Windows (/SUBSYSTEM:WINDOWS)` as shown on the screenshot.

* __Configuration__: All Configurations
* __Platform__: All Platforms
* __Subsystem__: Windows (_/SUBSYSTEM:WINDOWS_)

![winsubsystem.png](/images/winsubsystem.png)

Then, you can rebuild the project, but you will encounter this linker error.

```
error LNK2019: unresolved external symbol WinMain referenced in function 
               "int __cdecl invoke_main(void)" (?invoke_main@@YAHXZ)
```

The solution is to add `WinMain` function and include _Windows.h_. Console program starts from `main` while Windows program begins from `WinMain`. You can call your original `main` from `WinMain`.

```Cpp
#include <Windows.h>

// Your original main function
int main()
{
    // your original code here
    return 0;
}

int __stdcall WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    // You can use OutputDebugStringA to log your
    // debug message to your Visual Studio debugger
    // or Sysinternals DebugView
    OutputDebugStringA("Hello World!\n");

    return main(); // call your main() here
}
```

If you do not wish to include the heavy duty _Window.h_ to bring in the `WinMain`, you can declare `WinMain` as such. I cannot do it here as I need the `OutputDebugStringA` from that header.

```Cpp
#define HINSTANCE void*
#define LPSTR const char*
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, int nShowCmd);
```

Rebuild your project. It should run without a console window. If you need to forward `lpCmdLine` to your `main`, you can use `ParseCmdlineA` for ASCII or `ParseCmdlineW` for Unicode version. Both take care of command argument specified in quotes. Unfortunately, `lpCmdLine` does not contain the path of executable which I have to separately retrieve using `GetModuleFileNameA`. This feature is suggested by Richard Chambers to make the solution complete.

```Cpp
void ParseCmdlineW(const wchar_t* cmdLine,
    std::vector<wchar_t*>& vecArgs, std::wstring& holder)
{
    // implementation not shown
}

void ParseCmdlineA(const char* cmdLine,
    std::vector<char*>& vecArgs, std::string& holder)
{
    // implementation not shown
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
        OutputDebugStringA(argv[i]);
        OutputDebugStringA("\n");
    }

    return 0;
}

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
    DWORD res = GetModuleFileNameA(
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
```

## Alternate Solutions: Hide the Console or Make a Window Service

In the article comment section, Stacy Dudovitz gives two solutions: either move the console window offscreen and hide it or make your application a Windows service. Marius Bancila has written an excellent Windows service article: [Interact with Windows Services in C++](https://www.codeproject.com/Articles/1098263/Interact-with-Windows-Services-in-Cplusplus); You can use it as a guide to write your service.

```Cpp
#include <Windows.h>

// suggested by Stacy Dudovitz
int main()
{
    // move the window offscreen, then hide it
    HWND hwnd = ::GetConsoleWindow();
    ::SetWindowPos(hwnd, NULL, 5000, 5000, 0, 0, 0);
    ::ShowWindow(hwnd, SW_HIDE);

    // application code here 
    //     .....

    return 0;
}
```

## History

* 19<sup>th</sup> February, 2024: Added solution if user does not want to include _Windows.h_ for `WinMain`
* 7<sup>th</sup> February, 2024: Added alternate solutions</a> by Stacy Dudovitz. Added `HideConsole` project download
* 30<sup>th</sup> January, 2024: First release




