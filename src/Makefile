# contrib/megasena/Makefile

MODULES = megasena

EXTENSION = megasena
DATA = megasena--1.0.sql megasena--unpackaged--1.0.sql

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/megasena
top_builddir = /usr/src/postgresql-9.3
#top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
