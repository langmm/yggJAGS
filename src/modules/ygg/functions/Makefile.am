noinst_LTLIBRARIES = libyggfunc.la

libyggfunc_la_CPPFLAGS = -I$(top_srcdir)/src/include \
	@YGGINTERFACE_INCL@

libyggfunc_la_SOURCES = YggScalar.cc

noinst_HEADERS = YggScalar.h

## Test library 

if CANCHECK
check_LTLIBRARIES = libyggfunctest.la
libyggfunctest_la_SOURCES = testyggfunc.cc testyggfunc.h
libyggfunctest_la_CPPFLAGS = -I$(top_srcdir)/src/include
libyggfunctest_la_CXXFLAGS = $(CPPUNIT_CFLAGS)
endif
