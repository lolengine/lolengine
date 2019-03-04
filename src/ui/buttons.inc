//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if !defined _BTN
#   error buttons.inc included without the _BTN macro
#endif

_BTN(0, Unknown)

// Mouse buttons (FIXME: rename to LeftClick etc.?)
_BTN(1, Left)
_BTN(2, Right)
_BTN(3, Middle)
_BTN(4, Focus)

// Typical game controller buttons
_BTN(5, A)
_BTN(6, B)
_BTN(7, X)
_BTN(8, Y)
_BTN(9, DpadUp)
_BTN(10, DpadDown)
_BTN(11, DpadLeft)
_BTN(12, DpadRight)
_BTN(13, Start)
_BTN(14, Back)
_BTN(15, LeftThumb)
_BTN(16, RightThumb)
_BTN(17, LeftShoulder)
_BTN(18, RightShoulder)

#undef _BTN

