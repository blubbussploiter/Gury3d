/** 
 @file prompt.h
 
 @maintainer Morgan McGuire, matrix@graphics3d.com
 @cite   Windows GUI code by Max McGuire

 @created 2001-08-26
 @edited  2003-09-13
 */

#ifndef G3D_PROMPT_H
#define G3D_PROMPT_H

#include "platform.h"
#include <string>

namespace G3D {

/**
  Prints a prompt to stdout and waits for user input.  The return value is
  the number of the user's choice (the first is 0, if there are no
  choices, returns 0). 
 
  @param useGui Under Win32, use a GUI, not stdout prompt.
  @param windowTitle The title for the prompt window
  @param promptx The text string to prompt the user with
  @param choice  An array of strings that are the choices the user may make
  @param numChoices The length of choice.

  @cite Windows dialog interface by Max McGuire, mmcguire@ironlore.com
  @cite Font setting code by Kurt Miller, kurt@flipcode.com
  @deprecated
 */
int G3D_DEPRECATED prompt(
    const char*     windowTitle,
    const char*     promptx,
    const char**    choice,
    int             numChoices,
    bool            useGui);

/**
  Prints a prompt and waits for user input.  The return value is
  the number of the user's choice (the first is 0, if there are no
  choices, returns 0).
  <P>Uses GUI under Win32, stdout prompt otherwise.
  @deprecated
 */
inline int G3D_DEPRECATED prompt(
    const char*     windowTitle,
    const char*     promptx,
    const char**    choice,
    int             numChoices) {

    return prompt(windowTitle, promptx, choice, numChoices, true);
}


/**
 Displays a GUI prompt with "Ok" as the only choice.
 @deprecated 
 <B>Note:</B> SDL 1.2.6 interferes with this and causes it to return
 immediately.  If you use SDL, use SDL 1.2.5, which does not have that
 problem.
 */
void G3D_DEPRECATED msgBox(
    const std::string& message,
    const std::string& title="Message");


}; // namespace

#endif

