

CPP = g++
STRDIFF_OBJS      = strdiff.o common.o
STRDIFF3_OBJS     = strdiff3.o common.o
STRDIFF3TEST_OBJS = strdiff3_test.o
UNIDIFF_OBJS      = unidiff.o common.o
UNISTRDIFF_OBJS   = unistrdiff.o common.o
INTDIFF_OBJS      = intdiff.o
INTDIFF3_OBJS     = intdiff3.o
PATCH_OBJS        = patch.o common.o
FPATCH_OBJS       = fpatch.o common.o
ALL_OBJS          = strdiff strdiff3 strdiff3_test unidiff unistrdiff intdiff intdiff3 patch fpatch


.cpp.o:
	$(CPP) -c -O2 -Wall $<
all : $(ALL_OBJS)

strdiff : $(STRDIFF_OBJS)
	$(CPP) -O2 -o $@ $(STRDIFF_OBJS)

strdiff3 : $(STRDIFF3_OBJS)
	$(CPP) -O2 -o $@ $(STRDIFF3_OBJS)

strdiff3_test : $(STRDIFF3TEST_OBJS)
	$(CPP) -O2 -o $@ $(STRDIFF3TEST_OBJS)

unidiff : $(UNIDIFF_OBJS)
	$(CPP) -O2 -o $@ $(UNIDIFF_OBJS)

unistrdiff : $(UNISTRDIFF_OBJS)
	$(CPP) -O2 -o $@ $(UNISTRDIFF_OBJS)

intdiff : $(INTDIFF_OBJS)
	$(CPP) -O2 -o $@ $(INTDIFF_OBJS)

intdiff3 : $(INTDIFF3_OBJS)
	$(CPP) -O2 -o $@ $(INTDIFF3_OBJS)

patch : $(PATCH_OBJS)
	$(CPP) -O2 -o $@ $(PATCH_OBJS)

fpatch : $(FPATCH_OBJS)
	$(CPP) -O2 -o $@ $(FPATCH_OBJS)

clean:
	${RM} *.o *~ $(ALL_OBJS)

intdiff.o: intdiff.cpp ../dtl.hpp
intdiff3.o: intdiff3.cpp ../dtl.hpp
patch.o: patch.cpp common.cpp ../dtl.hpp
strdiff.o: strdiff.cpp common.cpp ../dtl.hpp
strdiff3.o: strdiff3.cpp common.cpp ../dtl.hpp
strdiff3_test.o: strdiff3_test.cpp ../dtl.hpp
unidiff.o: unidiff.cpp common.cpp ../dtl.hpp
unistrdiff.o: unistrdiff.cpp common.cpp ../dtl.hpp
fpatch.o: fpatch.cpp common.cpp ../dtl.hpp
