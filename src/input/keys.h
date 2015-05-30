﻿//
//  Lol Engine
//
//  Copyright © 2010—2013 Benjamin Litzelmann
//            © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

/* A list of typical keyboard scancodes, along with a user-friendly
 * name and, if possible, a string representation. */

#if !defined _SC
#   error scancode.h included without the _SC macro
#endif

_SC(0, "", Unknown)

/* Usage page 0x07 */

_SC(3,  "", Whatever) // FIXME: if this isn’t here, we can’t bind key A; WTF?
_SC(4,  "A", A)
_SC(5,  "B", B)
_SC(6,  "C", C)
_SC(7,  "D", D)
_SC(8,  "E", E)
_SC(9,  "F", F)
_SC(10, "G", G)
_SC(11, "H", H)
_SC(12, "I", I)
_SC(13, "J", J)
_SC(14, "K", K)
_SC(15, "L", L)
_SC(16, "M", M)
_SC(17, "N", N)
_SC(18, "O", O)
_SC(19, "P", P)
_SC(20, "Q", Q)
_SC(21, "R", R)
_SC(22, "S", S)
_SC(23, "T", T)
_SC(24, "U", U)
_SC(25, "V", V)
_SC(26, "W", W)
_SC(27, "X", X)
_SC(28, "Y", Y)
_SC(29, "Z", Z)

_SC(30, "1", 1)
_SC(31, "2", 2)
_SC(32, "3", 3)
_SC(33, "4", 4)
_SC(34, "5", 5)
_SC(35, "6", 6)
_SC(36, "7", 7)
_SC(37, "8", 8)
_SC(38, "9", 9)
_SC(39, "0", 0)

_SC(40, "\n", Return)
_SC(41, "\x1b", Escape)
_SC(42, "\b", Backspace)
_SC(43, "\t", Tab)
_SC(44, " ", Space)

_SC(45, "-", Minus)
_SC(46, "=", Equals)
_SC(47, "<", LeftBracket)
_SC(48, ">", RightBracket)
_SC(49, "\\", Backslash)

_SC(50, "", NonUSHash)
_SC(51, ";", Semicolon)
_SC(52, "\'", Apostrophe)
_SC(53, "`", Grave)
_SC(54, ",", Comma)
_SC(55, ".", Period)
_SC(56, "/", Slash)

_SC(57, "", CapsLock)

_SC(58, "", F1)
_SC(59, "", F2)
_SC(60, "", F3)
_SC(61, "", F4)
_SC(62, "", F5)
_SC(63, "", F6)
_SC(64, "", F7)
_SC(65, "", F8)
_SC(66, "", F9)
_SC(67, "", F10)
_SC(68, "", F11)
_SC(69, "", F12)

_SC(70, "", PrintScreen)
_SC(71, "", ScrollLock)
_SC(72, "", Pause)
_SC(73, "", Insert)
_SC(74, "", Home)
_SC(75, "", PageUp)
_SC(76, "", Delete)
_SC(77, "", End)
_SC(78, "", PageDown)
_SC(79, "", Right)
_SC(80, "", Left)
_SC(81, "", Down)
_SC(82, "", Up)

_SC(83, "", NumLockClear)
_SC(84, "/", KP_Divide)
_SC(85, "*", KP_Multiply)
_SC(86, "-", KP_Minus)
_SC(87, "+", KP_Plus)
_SC(88, "", KP_Enter)
_SC(89, "1", KP_1)
_SC(90, "2", KP_2)
_SC(91, "3", KP_3)
_SC(92, "4", KP_4)
_SC(93, "5", KP_5)
_SC(94, "6", KP_6)
_SC(95, "7", KP_7)
_SC(96, "8", KP_8)
_SC(97, "9", KP_9)
_SC(98, "0", KP_0)
_SC(99, ".", KP_Period)

_SC(100, "", NonUSBackslash)
_SC(101, "", Application)
_SC(102, "^", Power)
_SC(103, "=", KP_Equals)
_SC(104, "", F13)
_SC(105, "", F14)
_SC(106, "", F15)
_SC(107, "", F16)
_SC(108, "", F17)
_SC(109, "", F18)
_SC(110, "", F19)
_SC(111, "", F20)
_SC(112, "", F21)
_SC(113, "", F22)
_SC(114, "", F23)
_SC(115, "", F24)
_SC(116, "", Execute)
_SC(117, "", Help)
_SC(118, "", Menu)
_SC(119, "", Select)
_SC(120, "", Stop)
_SC(121, "", Again)
_SC(122, "", Undo)
_SC(123, "", Cut)
_SC(124, "", Copy)
_SC(125, "", Paste)
_SC(126, "", Find)
_SC(127, "", Mute)
_SC(128, "", VolumeUp)
_SC(129, "", VolumeDOwn)
_SC(133, "", KP_Comma)
_SC(134, "", KP_EqualsAS400)

_SC(135, "", International1)
_SC(136, "", International2)
_SC(137, "", International3)
_SC(138, "", International4)
_SC(139, "", International5)
_SC(140, "", International6)
_SC(141, "", International7)
_SC(142, "", International8)
_SC(143, "", International9)
_SC(144, "", Lang1)
_SC(145, "", Lang2)
_SC(146, "", Lang3)
_SC(147, "", Lang4)
_SC(148, "", Lang5)
_SC(149, "", Lang6)
_SC(150, "", Lang7)
_SC(151, "", Lang8)
_SC(152, "", Lang9)

_SC(153, "", AltErase)
_SC(154, "", SysReq)
_SC(155, "", Cancel)
_SC(156, "", Clear)
_SC(157, "", Prior)
_SC(158, "", Return2)
_SC(159, "", Separator)
_SC(160, "", Out)
_SC(161, "", Oper)
_SC(162, "", ClearAgain)
_SC(163, "", CrSel)
_SC(164, "", ExSel)

_SC(176, "", KP_00)
_SC(177, "", KP_000)
_SC(178, "", ThousandsSeparator)
_SC(179, "", DecimalSeparator)
_SC(180, "", CurrencyUnit)
_SC(181, "", CurrencySubunit)
_SC(182, "(", KP_LeftParen)
_SC(183, ")", KP_RightParen)
_SC(184, "{", KP_LeftBrace)
_SC(185, "}", KP_RightBrace)
_SC(186, "\t", KP_Tab)
_SC(187, "", KP_Backspace)
_SC(188, "A", KP_A)
_SC(189, "B", KP_B)
_SC(190, "C", KP_C)
_SC(191, "D", KP_D)
_SC(192, "E", KP_E)
_SC(193, "F", KP_F)
_SC(194, "", KP_Xor)
_SC(195, "^", KP_Power)
_SC(196, "%", KP_Percent)
_SC(197, "<", KP_Less)
_SC(198, ">", KP_Greater)
_SC(199, "&", KP_Ampersand)
_SC(200, "&&", KP_DblAmpersand)
_SC(201, "|", KP_VerticalBar)
_SC(202, "||", KP_DblVerticalBar)
_SC(203, ":", KP_Colon)
_SC(204, "#", KP_Hash)
_SC(205, " ", KP_Space)
_SC(206, "@", KP_At)
_SC(207, "!", KP_Exclam)
_SC(208, "", KP_MemStore)
_SC(209, "", KP_MemRecall)
_SC(210, "", KP_MemClear)
_SC(211, "", KP_MemAdd)
_SC(212, "", KP_MemSubtract)
_SC(213, "", KP_MemMultiply)
_SC(214, "", KP_MemDivide)
_SC(215, "", KP_PlusMinus)
_SC(216, "", KP_Clear)
_SC(217, "", KP_ClearEntry)
_SC(218, "", KP_Binary)
_SC(219, "", KP_Octal)
_SC(220, "", KP_Decimal)
_SC(221, "", KP_Hexadecimal)

_SC(224, "", LCtrl)
_SC(225, "", LShift)
_SC(226, "", LAlt)
_SC(227, "", LGui)
_SC(228, "", RCtrl)
_SC(229, "", RShift)
_SC(230, "", RAlt)
_SC(231, "", RGui)

_SC(257, "", Mode)

/* Usage page 0x0c */

_SC(258, "", AudioNext)
_SC(259, "", AudioPrev)
_SC(260, "", AudioStop)
_SC(261, "", AudioPlay)
_SC(262, "", AudioMute)
_SC(263, "", MediaSelect)
_SC(264, "", WWW)
_SC(265, "", Mail)
_SC(266, "", Calculator)
_SC(267, "", Computer)
_SC(268, "", AC_Search)
_SC(269, "", AC_Home)
_SC(270, "", AC_Back)
_SC(271, "", AC_Forward)
_SC(272, "", AC_Stop)
_SC(273, "", AC_Refresh)
_SC(274, "", AC_Bookmarks)

/* Extra SDL scancodes */

_SC(275, "", BrightnessDown)
_SC(276, "", BrightnessUp)
_SC(277, "", DisplaySwitch)
_SC(278, "", KbdIllumToggle)
_SC(279, "", KbdIllumDown)
_SC(280, "", KbdIllumUp)
_SC(281, "", Eject)
_SC(282, "", Sleep)

_SC(283, "", App1)
_SC(284, "", App2)

//Custom Lock management
_SC(285, "", CapsLockStatus)
_SC(286, "", ScrollLockStatus)
_SC(287, "", NumLockClearStatus)

#undef _SC

