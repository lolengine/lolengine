
include $(top_srcdir)/build/autotools/common.am

# FIXME: can't include "Lol (vs2015).sln" because of the spaces

EXTRA_DIST += lol-build \
              check-source.sh run-bitten.sh \
              build-linux build-mingw build-mingw64 \
              build-nacl32 build-nacl64 build-android build-html \
              \
              msbuild/lolfx.targets \
              msbuild/lolfx.xml \
              msbuild/lolfx.props \
              msbuild/lol.config.props \
              msbuild/lol.vars.props \
              msbuild/lol.rules.props \
              \
              autotools/common.am

TESTS = check-source.sh

