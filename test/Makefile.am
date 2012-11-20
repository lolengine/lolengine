
include $(top_srcdir)/build/autotools/common.am

SUBDIRS = math sandbox xolotl

AM_CPPFLAGS = -I$(top_srcdir)/src

bench: benchsuite$(EXEEXT)
	./benchsuite$(EXEEXT)

EXTRA_DIST = data/gradient.png

noinst_PROGRAMS = benchsuite $(testsuite) btphystest

TESTS = $(testsuite)

# Conditionally built for now because of STLport issues
if !USE_ANDROID
testsuite = testsuite
endif

testsuite_SOURCES = testsuite.cpp \
    unit/vector.cpp unit/matrix.cpp unit/half.cpp unit/trig.cpp \
    unit/build.cpp unit/real.cpp unit/image.cpp unit/quat.cpp unit/cmplx.cpp \
    unit/array.cpp unit/rotation.cpp unit/string.cpp
testsuite_CPPFLAGS = @LOL_CFLAGS@
testsuite_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@
testsuite_DEPENDENCIES = $(top_builddir)/src/liblol.a

benchsuite_SOURCES = benchsuite.cpp \
    benchmark/vector.cpp benchmark/half.cpp benchmark/trig.cpp \
    benchmark/real.cpp
benchsuite_CPPFLAGS = @LOL_CFLAGS@
benchsuite_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@
benchsuite_DEPENDENCIES = $(top_builddir)/src/liblol.a

btphystest_SOURCES = BtPhysTest.cpp BtPhysTest.h PhysicObject.h \
    Physics/Src/EasyPhysics.cpp Physics/Include/EasyPhysics.h \
    Physics/Include/LolBtPhysicsIntegration.h Physics/Include/LolPhysics.h \
    Physics/Src/EasyCharacterController.cpp \
    Physics/Include/EasyCharacterController.h \
    Physics/Src/EasyConstraint.cpp \
    Physics/Include/EasyConstraint.h \
    Physics/Src/BulletCharacterController.cpp \
    Physics/Include/BulletCharacterController.h
btphystest_CPPFLAGS = @LOL_CFLAGS@ -I$(top_srcdir)/src/bullet \
    -DHAVE_PHYS_USE_BULLET
btphystest_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@
btphystest_DEPENDENCIES = $(top_builddir)/src/liblol.a

