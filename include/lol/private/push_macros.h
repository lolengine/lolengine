//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

// Undefine some intrusive macros from the Windows headers but allow
// to restore them afterwards if that’s what the user really wants.

#if _MSC_VER
#   pragma push_macro("min")
#   pragma push_macro("max")
#   pragma push_macro("near")
#   pragma push_macro("far")
#   undef min
#   undef max
#   undef near
#   undef far
#endif
