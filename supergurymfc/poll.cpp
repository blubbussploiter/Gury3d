#include "application.h"

static unsigned int _sdlKeys[SDLK_LAST];

static void makeKeyEvent(int vkCode, int lParam, GEvent& e) {

    // If true, we're looking at the right hand version of
    // Fix VK_SHIFT, VK_CONTROL, VK_MENU
    bool extended = (lParam >> 24) & 0x01;

    // Check for normal letter event
    if ((vkCode >= 'A') && (vkCode <= 'Z')) {

        // Make key codes lower case canonically
        e.key.keysym.sym = (SDLKey)(vkCode - 'A' + 'a');

    }
    else if (vkCode == VK_SHIFT) {

        e.key.keysym.sym = (SDLKey)(extended ? SDLK_RSHIFT : SDLK_LSHIFT);

    }
    else if (vkCode == VK_CONTROL) {

        e.key.keysym.sym = (SDLKey)(extended ? SDLK_RCTRL : SDLK_LCTRL);

    }
    else if (vkCode == VK_MENU) {

        e.key.keysym.sym = (SDLKey)(extended ? SDLK_RALT : SDLK_LALT);

    }
    else {

        e.key.keysym.sym = (SDLKey)_sdlKeys[iClamp(vkCode, 0, SDLK_LAST)];

    }

    e.key.keysym.scancode = MapVirtualKey(vkCode, 0);
    //(lParam >> 16) & 0x7F;

    static BYTE lpKeyState[256];
    GetKeyboardState(lpKeyState);

    int mod = 0;
    if (lpKeyState[VK_LSHIFT] & 0x80) {
        mod = mod | KMOD_LSHIFT;
    }

    if (lpKeyState[VK_RSHIFT] & 0x80) {
        mod = mod | KMOD_RSHIFT;
    }

    if (lpKeyState[VK_LCONTROL] & 0x80) {
        mod = mod | KMOD_LCTRL;
    }

    if (lpKeyState[VK_RCONTROL] & 0x80) {
        mod = mod | KMOD_RCTRL;
    }

    if (lpKeyState[VK_LMENU] & 0x80) {
        mod = mod | KMOD_LALT;
    }

    if (lpKeyState[VK_RMENU] & 0x80) {
        mod = mod | KMOD_RALT;
    }
    e.key.keysym.mod = (SDLMod)mod;

    ToUnicode(vkCode, e.key.keysym.scancode, lpKeyState, (LPWSTR)&e.key.keysym.unicode, 1, 0);
}


/**
 Configures a mouse up/down event
 */
static void mouseButton(bool down, int keyEvent, DWORD flags, GEvent& e) {
    (void)flags;
    // TODO: process flags
    if (down) {
        e.key.type = SDL_KEYDOWN;
        e.key.state = SDL_PRESSED;
    }
    else {
        e.key.type = SDL_KEYUP;
        e.key.state = SDL_RELEASED;
    }

    e.key.keysym.unicode = ' ';
    e.key.keysym.sym = (SDLKey)keyEvent;

    e.key.keysym.scancode = 0;

    static BYTE lpKeyState[256];
    GetKeyboardState(lpKeyState);

    int mod = 0;
    if (lpKeyState[VK_LSHIFT] & 0x80) {
        mod = mod | KMOD_LSHIFT;
    }

    if (lpKeyState[VK_RSHIFT] & 0x80) {
        mod = mod | KMOD_RSHIFT;
    }

    if (lpKeyState[VK_LCONTROL] & 0x80) {
        mod = mod | KMOD_LCTRL;
    }

    if (lpKeyState[VK_RCONTROL] & 0x80) {
        mod = mod | KMOD_RCTRL;
    }

    if (lpKeyState[VK_LMENU] & 0x80) {
        mod = mod | KMOD_LALT;
    }

    if (lpKeyState[VK_RMENU] & 0x80) {
        mod = mod | KMOD_RALT;
    }
    e.key.keysym.mod = (SDLMod)mod;
}

bool RBX::Experimental::Application::pollEvent(GEvent& e)
{
	if (msg.message != WM_QUIT)
	{
		if (msg.hwnd == parent) {
			switch (msg.message) {
			    case WM_KEYDOWN:
			    case WM_SYSKEYDOWN:
                {

                    e.key.type = SDL_KEYDOWN;
                    e.key.state = SDL_PRESSED;

                    // Fix invalid repeat key flag
                    if (justReceivedFocus) {
                        justReceivedFocus = false;
                        msg.lParam &= ~(0x40000000);
                    }

                    // Need the repeat messages to find LSHIFT and RSHIFT
                    if (!((msg.lParam >> 30) & 0x01)) {
                        // This is not an autorepeat message
                        makeKeyEvent(msg.wParam, msg.lParam, e);
                        _keyboardButtons[msg.wParam] = true;
                        return true;
                    }
                    break;

                }
			    case WM_KEYUP:
			    case WM_SYSKEYUP:
                {
                    e.key.type = SDL_KEYUP;
                    e.key.state = SDL_RELEASED;

                    makeKeyEvent(msg.wParam, msg.lParam, e);
                    _keyboardButtons[msg.wParam] = false;
                    return true;
                }
			    case WM_LBUTTONDOWN:
                {
                    mouseButton(true, SDL_LEFT_MOUSE_KEY, msg.wParam, e);
                    _mouseButtons[0] = true;
                    return true;
                }
			    case WM_MBUTTONDOWN:
                {
                    mouseButton(true, SDL_MIDDLE_MOUSE_KEY, msg.wParam, e);
                    _mouseButtons[1] = true;
                    return true;
                }
			    case WM_RBUTTONDOWN:
                {
                    mouseButton(true, SDL_RIGHT_MOUSE_KEY, msg.wParam, e);
                    _mouseButtons[2] = true;
                    return true;
                }
			    case WM_LBUTTONUP:
                {
                    mouseButton(false, SDL_LEFT_MOUSE_KEY, msg.wParam, e);
                    _mouseButtons[0] = false;
                    return true;
                }
			    case WM_MBUTTONUP:
                {
                    mouseButton(false, SDL_MIDDLE_MOUSE_KEY, msg.wParam, e);
                    _mouseButtons[1] = false;
                    return true;
                }
			    case WM_RBUTTONUP:
                {
                    mouseButton(false, SDL_RIGHT_MOUSE_KEY, msg.wParam, e);
                    _mouseButtons[2] = false;
                    return true;
                }
			} // switch
		} // if
	}

	return false;
}