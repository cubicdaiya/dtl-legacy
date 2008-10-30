

CPP = g++
STRDIFF_OBJS = strdiff.o
UNIDIFF_OBJS = unidiff.o common.o
INTDIFF_OBJS = intdiff.o
PATCH_OBJS = patch.o
FPATCH_OBJS = fpatch.o common.o

.cpp.o:
	$(CPP) -c -Wall -O2 -Wextra -Wformat=2 -Wstrict-aliasing=2 -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wfloat-equal -Wpointer-arith -Wswitch-enum $<
#	$(CPP) -c -Wall $<
all : strdiff unidiff intdiff patch fpatch

strdiff : $(STRDIFF_OBJS)
	$(CPP) -o $@ $(STRDIFF_OBJS)

unidiff : $(UNIDIFF_OBJS)
	$(CPP) -o $@ $(UNIDIFF_OBJS)

intdiff : $(INTDIFF_OBJS)
	$(CPP) -o $@ $(INTDIFF_OBJS)

patch : $(PATCH_OBJS)
	$(CPP) -o $@ $(PATCH_OBJS)

fpatch : $(FPATCH_OBJS)
	$(CPP) -o $@ $(FPATCH_OBJS)

clean:
	${RM} *.o *~ strdiff unidiff intdiff patch fpatch

intdiff.o: intdiff.cpp ../dtl.hpp
patch.o: patch.cpp ../dtl.hpp
strdiff.o: strdiff.cpp ../dtl.hpp
unidiff.o: unidiff.cpp common.cpp ../dtl.hpp
fpatch.o: fpatch.cpp common.cpp ../dtl.hpp
