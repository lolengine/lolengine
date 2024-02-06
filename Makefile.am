
include $(top_srcdir)/build/autotools/common.am

lol_files := $(shell find $(srcdir)/lol/include -maxdepth 2 -type f) \
             $(filter %.h %.ipp %.hpp, $(shell find $(srcdir)/lol/include -type f))

# Put "people" at the end because they're more likely to break builds.
SUBDIRS = build src tools doc
DIST_SUBDIRS = $(SUBDIRS) binaries

EXTRA_DIST += $(lol_files) bootstrap COPYING TODO \
              .gitignore .gitattributes .gitmodules \
              .travis.yml lolengine.sln build.config
AUTOMAKE_OPTIONS = dist-bzip2
ACLOCAL_AMFLAGS = -I build/autotools/m4

test: lolcheck
check: lolcheck

