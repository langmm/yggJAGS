SUBDIRS = 

jagsmod_LTLIBRARIES = ygg.la

ygg_la_SOURCES = ygg.cc

# TODO: Include yggdrasil library & include paths
ygg_la_CPPFLAGS = -I$(top_srcdir)/src/include

ygg_la_LIBADD = 						\
	$(top_builddir)/src/lib/libjags.la			\
	$(top_builddir)/src/jrmath/libjrmath.la			\
	@LAPACK_LIBS@ @BLAS_LIBS@

ygg_la_LDFLAGS = -module -avoid-version
if WINDOWS
ygg_la_LDFLAGS += -no-undefined
endif

### Test library 

if CANCHECK
check_LTLIBRARIES = libyggtest.la
libyggtest_la_SOURCES = testygg.cc testygg.h
libyggtest_la_CPPFLAGS = -I$(top_srcdir)/src/include
libyggtest_la_CXXFLAGS = $(CPPUNIT_CFLAGS)
libyggtest_la_LDFLAGS = $(CPPUNIT_LIBS)
libyggtest_la_LIBADD = \
	$(top_builddir)/src/lib/libtest.la \
	$(top_builddir)/src/lib/libjags.la \
	$(top_builddir)/src/jrmath/libjrmath.la \
	@LAPACK_LIBS@ @BLAS_LIBS@

if WINDOWS
libyggtest_la_LDFLAGS += -no-undefined
endif

endif
