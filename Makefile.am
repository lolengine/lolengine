
include $(top_srcdir)/build/autotools/common.am

# Put "people" at the end because they're more likely to break builds.
SUBDIRS = build src tools doc
DIST_SUBDIRS = $(SUBDIRS) binaries

EXTRA_DIST += bootstrap COPYING TODO \
              .gitignore .gitattributes .gitmodules \
              .travis.yml lol.sln build.config
AUTOMAKE_OPTIONS = dist-bzip2
ACLOCAL_AMFLAGS = -I build/autotools/m4

test: lolcheck
check: lolcheck

