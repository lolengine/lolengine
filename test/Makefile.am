
include $(top_srcdir)/build/autotools/common.am

SUBDIRS = math sandbox xolotl

bench: benchsuite$(EXEEXT)
	./benchsuite$(EXEEXT)

EXTRA_DIST = data/gradient.png

noinst_PROGRAMS = benchsuite $(testsuite) btphystest meshviewer

TESTS = $(testsuite)

# Conditionally built for now because of STLport issues
if !USE_ANDROID
testsuite = testsuite
endif

testsuite_SOURCES = testsuite.cpp \
    unit/vector.cpp unit/matrix.cpp unit/half.cpp unit/trig.cpp \
    unit/build.cpp unit/real.cpp unit/image.cpp unit/quat.cpp unit/cmplx.cpp \
    unit/array.cpp unit/rotation.cpp unit/string.cpp unit/map.cpp \
    unit/color.cpp unit/atomic.cpp unit/interp.cpp
testsuite_CPPFLAGS = $(AM_CPPFLAGS)
testsuite_DEPENDENCIES = @LOL_DEPENDENCIES@

benchsuite_SOURCES = benchsuite.cpp \
    benchmark/vector.cpp benchmark/half.cpp benchmark/trig.cpp \
    benchmark/real.cpp
benchsuite_CPPFLAGS = $(AM_CPPFLAGS)
benchsuite_DEPENDENCIES = @LOL_DEPENDENCIES@

btphystest_SOURCES = BtPhysTest.cpp BtPhysTest.h PhysicObject.h \
    Physics/Src/EasyPhysics.cpp Physics/Include/EasyPhysics.h \
    Physics/Include/LolBtPhysicsIntegration.h Physics/Include/LolPhysics.h \
    Physics/Src/EasyCharacterController.cpp \
    Physics/Include/EasyCharacterController.h \
    Physics/Src/EasyConstraint.cpp \
    Physics/Include/EasyConstraint.h \
    Physics/Src/BulletCharacterController.cpp \
    Physics/Include/BulletCharacterController.h
btphystest_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/src/bullet \
    -DHAVE_PHYS_USE_BULLET
btphystest_DEPENDENCIES = @LOL_DEPENDENCIES@

meshviewer_SOURCES = MeshViewer.cpp \
    shinyMVTexture.lolfx shinyfur.lolfx
meshviewer_CPPFLAGS = $(AM_CPPFLAGS)
meshviewer_DEPENDENCIES = @LOL_DEPENDENCIES@

