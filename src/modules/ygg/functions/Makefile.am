noinst_LTLIBRARIES = libyggfunc.la

libyggfunc_la_CPPFLAGS = -I$(top_srcdir)/src/include \
	$(YGGINTERFACE_CFLAGS)

libyggfunc_la_SOURCES = YggScalar.cc YggVector.cc YggArray.cc

noinst_HEADERS = YggScalar.h YggVector.h YggArray.h

## Test library 

if CANCHECK
check_LTLIBRARIES = libyggfunctest.la
libyggfunctest_la_SOURCES = testyggfunc.cc testyggfunc.h
libyggfunctest_la_CPPFLAGS = -I$(top_srcdir)/src/include $(YGGINTERFACE_CFLAGS)
libyggfunctest_la_CXXFLAGS = $(CPPUNIT_CFLAGS) $(YGGINTERFACE_CFLAGS)
endif
