PACKAGE_TARNAME = papi
prefix = /home/frandiaz99/tesis_papi
exec_prefix = ${prefix}
datarootdir = ${prefix}/share
datadir = ${datarootdir}/${PACKAGE_TARNAME}
DATADIR = $(DESTDIR)$(datadir)
INCLUDE = -I. -I${prefix}/include
CC	= gcc
F77	= gfortran
CC_R	= $(CC) -pthread
CFLAGS	= -g -O2 -Wno-error -Wno-discarded-qualifiers~  -DPAPI_NUM_COMP=3 -O1
ENABLE_FORTRAN = yes
