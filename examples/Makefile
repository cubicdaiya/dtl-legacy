

CPP = g++
STRDIFF_OBJS = strdiff.o common.o
STRDIFF3_OBJS = strdiff3.o common.o
STRDIFF3TEST_OBJS = strdiff3_test.o
UNIDIFF_OBJS = unidiff.o common.o
INTDIFF_OBJS = intdiff.o
INTDIFF3_OBJS = intdiff3.o
PATCH_OBJS = patch.o common.o
FPATCH_OBJS = fpatch.o common.o

.cpp.o:
	$(CPP) -g -c -Wall -O2 -Wextra -Wformat=2 -Wstrict-aliasing=2 -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wfloat-equal -Wpointer-arith -Wswitch-enum $<
#	$(CPP) -c -Wall $<
all : strdiff strdiff3 strdiff3_test unidiff intdiff intdiff3 patch fpatch

strdiff : $(STRDIFF_OBJS)
	$(CPP) -o $@ $(STRDIFF_OBJS)

strdiff3 : $(STRDIFF3_OBJS)
	$(CPP) -o $@ $(STRDIFF3_OBJS)

strdiff3_test : $(STRDIFF3TEST_OBJS)
	$(CPP) -o $@ $(STRDIFF3TEST_OBJS)

unidiff : $(UNIDIFF_OBJS)
	$(CPP) -o $@ $(UNIDIFF_OBJS)

intdiff : $(INTDIFF_OBJS)
	$(CPP) -o $@ $(INTDIFF_OBJS)

intdiff3 : $(INTDIFF3_OBJS)
	$(CPP) -o $@ $(INTDIFF3_OBJS)

patch : $(PATCH_OBJS)
	$(CPP) -o $@ $(PATCH_OBJS)

fpatch : $(FPATCH_OBJS)
	$(CPP) -o $@ $(FPATCH_OBJS)

clean:
	${RM} *.o *~ strdiff strdiff3 strdiff3_test unidiff intdiff intdiff3 patch fpatch

intdiff.o: intdiff.cpp ../dtl.hpp
intdiff3.o: intdiff3.cpp ../dtl.hpp
patch.o: patch.cpp common.cpp ../dtl.hpp
strdiff.o: strdiff.cpp common.cpp ../dtl.hpp
strdiff3.o: strdiff3.cpp common.cpp ../dtl.hpp
strdiff3_test.o: strdiff3_test.cpp ../dtl.hpp
unidiff.o: unidiff.cpp common.cpp ../dtl.hpp
fpatch.o: fpatch.cpp common.cpp ../dtl.hpp
