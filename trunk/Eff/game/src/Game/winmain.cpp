/******************************************************************************
engine app

******************************************************************************/


#include "pch.hpp"

// Standard Library Headers.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <winnt.h>

#include <crtdbg.h>
#include <string.h>
#include "App.h"
//#include "rendersystem/release.h"
//
//	Application Specific Definitions:
//	Some pre-processing and debug defines for the application.
//s
static const char* APP_NAME = "App";

static const int MIN_WINDOW_WIDTH = 100;
static const int MIN_WINDOW_HEIGHT = 100;

#if defined( _DEBUG )
const char * APP_TITLE = "Aurora Client Debug Version";
#elif defined( _HYBRID )
#if defined( _EVALUATION )
    const char * APP_TITLE = "Aurora Client Evaluation Version";
#else
    const char * APP_TITLE = "Aurora Hybrid Version";
#endif
#else
const char * APP_TITLE = "Aurora";
#endif

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxerr9.lib")

static bool g_bActive = false;
static bool g_bAppStarted = false;
static HWND g_hWnd = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


bool g_bAppQuit = false;

bool processOutstandingMessages()
{
	MSG msg;

	while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		if( msg.message == WM_QUIT )
		{
			g_bAppQuit = true;
			return false;
		}

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return true;
}




/**
 *	This function processes the command line.
 *
 *	@return True if successful, otherwise false.
 */
int parseCommandLine( LPSTR str )
{
	const int MAX_ARGS = 20;
	const char seps[]   = " \t";
	char * argv[ MAX_ARGS ];
	int argc = 0;

	argv[ argc ] = strtok( str, seps );

	while (argv[ argc ] != NULL)
	{
		if (++argc >= MAX_ARGS)
		{
			ERROR_MSG( "winmain::parseCommandLine: Too many arguments!!\n" );
			return FALSE;
		}

		argv[ argc ] = strtok( NULL, seps );
	}

	//for (int i = 0; i < argc - 1; i++)
	//{
	//	if (strcmp( "--config", argv[i] ) == 0 ||
	//		strcmp( "-c", argv[i] ) == 0)
	//	{
	//		// TODO:PM Handle filenames with spaces

	//		if (argv[ i+1 ][0] != '"')
	//		{
	//			configFilename = std::string( argv[ i+1 ] );
	//		}
	//		else
	//		{
	//			CRITICAL_MSG( "winmain::parseCommandLine: "
	//				"config filename cannot have spaces\n" );

	//			return FALSE;
	//		}
	//	}
	//}

	//return BWResource::init( argc, argv )	? TRUE : FALSE;
	return TRUE;
}

//-----------------------------------------------------------------------------
// WinMain()
// Desc: Application entry point.
//-----------------------------------------------------------------------------
int PASCAL WinMain( HINSTANCE hInst, HINSTANCE, LPSTR str, int nCmdShow )
{
	if (!parseCommandLine( str ))
	{
		return FALSE;
	}

	int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flags |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF;
	_CrtSetDbgFlag(flags);
	//_CrtSetBreakAlloc(9907);

    // Set up the window class
	HCURSOR cursor = LoadCursor( hInst, MAKEINTRESOURCE(0) );
    WNDCLASS wc = { CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInst, NULL,
                    // LoadIcon( hInst, MAKEINTRESOURCE(IDI_BIGWORLD_ICON)),
					cursor, NULL, NULL, APP_NAME };
    if( !RegisterClass( &wc ) )
        return FALSE;

	const int borderWidth = GetSystemMetrics(SM_CXFRAME);
	const int borderHeight = GetSystemMetrics(SM_CYFRAME);
	const int titleHeight = GetSystemMetrics(SM_CYCAPTION);

	
    // Create the main window
    HWND hWnd = CreateWindow(
		APP_NAME,
		APP_TITLE,
		//(WS_OVERLAPPEDWINDOW)&(~WS_MAXIMIZEBOX)&(~WS_MINIMIZEBOX),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1024 + borderWidth*2,
		768 + titleHeight,
		NULL,
		NULL,
		hInst,
		0 );

	g_hWnd = hWnd;

    // Initialize and start a new game
    ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	MSG msg;

	while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
    }

	timeBeginPeriod(1);
	
	// App scope
	try
	{
		// Initialize the application.
		//App app;
		//BWResource::init( 0, 0 );

		//if( !App::instance()->init( hInst, hWnd ) )
		//{
		//	DestroyWindow( hWnd );
		//	return FALSE;
		//}

		g_bAppStarted = true;

		int count = 0;
		// Standard game loop.
		while( !g_bAppQuit )
		{
			MSG msg;

			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				count++;
				// Check for a quit message
				if( msg.message == WM_QUIT )
					break;

				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				//// Play the game (check user input 
				//// and update the window)
				//if (!App::instance()->updateFrame(g_bActive))
				//{
				//	//App::instance()->release();
				//	//::PostQuitMessage(0);
				//	break;
				//}
			}
		}
		//App::instance()->release();
	}
	catch (std::runtime_error &)
	{		
		//releaseSingleness();
		DestroyWindow( hWnd );
		return FALSE;
	}
	timeEndPeriod(1);
	//releaseSingleness();
    return TRUE;
}

//-----------------------------------------------------------------------------
// Name: WndProc()
// Desc: Window procedure for the game window
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
    HDC hdc;
    
	switch (msg) {

	case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;
	case WM_SYSKEYDOWN:
        // disable ALT key
        if (wParam == VK_MENU) 
            return 0;
        break;

	//case WM_SETCURSOR:
	//	// This message should only be generated when 
	//	// running with inputDevices/exclusive == false.
	//	if (g_bActive && g_bAppStarted && MouseCursor::instance().isActive())
	//	{
	//		MouseCursor::instance().refresh();
	//	}
	//	return TRUE;
	//	break;

	case WM_ACTIVATE:
		g_bActive = ( WA_INACTIVE != wParam ); // this line caused the app to crash when alt-tabing in
											   // fullscreen mode - svein

		if (wParam == WA_INACTIVE)
		{
			//onFocusChange( false );
			App::instance()->handleSetFocus( false );
		}
		else
		{
			//onFocusChange( true );
			App::instance()->handleSetFocus( true );
		}
		break;

	//case WM_ENTERMENULOOP:
	//	//g_bActive = FALSE;
	//	break;

	//case WM_EXITMENULOOP:
	//	//if (( GetActiveWindow() == hWnd ) && ( FALSE == IsIconic( hWnd ) ))
	//	//	g_bActive = TRUE;
	//	break;

	//case WM_GETMINMAXINFO:
	//	((MINMAXINFO*)lParam)->ptMinTrackSize.x = MIN_WINDOW_WIDTH;
	//	((MINMAXINFO*)lParam)->ptMinTrackSize.y = MIN_WINDOW_HEIGHT;
	//	break;

	//case WM_PAINT:
	//	if (!g_bAppStarted)
	//	{
	//		RECT rect;
	//		GetClientRect( hWnd, &rect );
	//		FillRect( GetDC( hWnd ), &rect, (HBRUSH)( COLOR_WINDOW+1) );
	//	}
	//	break;

	//case WM_MOVE:
	//	if (g_bActive && g_bAppStarted)
	//	{
	//		App::instance().moveWindow((int16)LOWORD(lParam), (int16)HIWORD(lParam));
	//	}
	//	break;

	//case WM_SIZE:
	//	g_bActive = !( (SIZE_MAXHIDE == wParam) || (SIZE_MINIMIZED == wParam) || ( 0 == lParam ));

	//	if (g_bActive && g_bAppStarted)
	//	{
	//		App::instance().resizeWindow();
	//	}
	//	break;

	case WM_SYSCOMMAND:
		if ((wParam & 0xFFF0) == SC_CLOSE)
		{
			PostQuitMessage(0);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

// winmain.cpp
