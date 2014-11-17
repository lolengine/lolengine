//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

// To be removed when fully tested
#include <lol/math/simplex_interpolator.h>
	
namespace lol
{

lolunit_declare_fixture(SimplexInterpolatorTest)
{
    void SetUp() {}

    void TearDown() {}

    lolunit_declare_test(Interp2D)
    {
    	simplex_interpolator<2> interpolator;
    }
};

}
