
include $(top_srcdir)/build/autotools/common.am

noinst_PROGRAMS = $(testsuite)

TESTS = $(testsuite)

# Conditionally built for now because of STLport issues
if !USE_ANDROID
testsuite = test-base test-math test-sys test-image test-entity
endif

test_base_SOURCES = test-common.cpp \
    base/avl_tree.cpp base/array.cpp base/enum.cpp base/map.cpp \
    base/string.cpp base/types.cpp
test_base_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/tools/lolunit
test_base_DEPENDENCIES = @LOL_DEPS@

test_math_SOURCES = test-common.cpp \
    math/array2d.cpp math/array3d.cpp math/arraynd.cpp math/box.cpp \
    math/cmplx.cpp math/half.cpp math/interp.cpp math/matrix.cpp \
    math/quat.cpp math/rand.cpp math/real.cpp math/rotation.cpp \
    math/trig.cpp math/vector.cpp math/simplex_interpolator.cpp \
    math/polynomial.cpp
test_math_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/tools/lolunit
test_math_DEPENDENCIES = @LOL_DEPS@

test_sys_SOURCES = test-common.cpp \
    sys/thread.cpp
test_sys_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/tools/lolunit
test_sys_DEPENDENCIES = @LOL_DEPS@

test_image_SOURCES = test-common.cpp \
    image/color.cpp image/image.cpp
test_image_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/tools/lolunit
test_image_DEPENDENCIES = @LOL_DEPS@

test_entity_SOURCES = test-common.cpp \
    entity/camera.cpp
test_entity_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/tools/lolunit
test_entity_DEPENDENCIES = @LOL_DEPS@

noinst_DATA = data/gradient.png

