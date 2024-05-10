#include <Windows.h>

// suggested by Stacy Dudovitz
int main()
{
	// move the window offscreen, then hide it - no annoying flash
	HWND hwnd = ::GetConsoleWindow();
	::SetWindowPos(hwnd, NULL, 5000, 5000, 0, 0, 0);
	::ShowWindow(hwnd, SW_HIDE);
	// sleep for 30 seconds for testing
	::Sleep(30000);

	// application code here 
	//     .....

	return 0;
}