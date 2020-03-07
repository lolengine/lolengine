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

// Restore macros that push_macros.h disabled.

#if _MSC_VER
#   pragma pop_macro("min")
#   pragma pop_macro("max")
#   pragma pop_macro("near")
#   pragma pop_macro("far")
#endif
