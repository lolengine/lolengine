//
//  Lol Engine
//
//  Copyright © 2010–2025 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The QOA decoder
// ———————————————
//

#include <lol/audio/stream>

namespace lol::audio
{

std::shared_ptr<stream<int16_t>> make_qoa_decoder(uint8_t const *data, size_t size);

} // namespace lol
