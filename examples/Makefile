

CPP = g++
STRDIFF_OBJS      = strdiff.o common.o
STRDIFF3_OBJS     = strdiff3.o common.o
STRDIFF3TEST_OBJS = strdiff3_test.o
UNIDIFF_OBJS      = unidiff.o common.o
UNISTRDIFF_OBJS   = unistrdiff.o common.o
INTDIFF_OBJS      = intdiff.o
UINTDIFF_OBJS     = uintdiff.o
INTDIFF3_OBJS     = intdiff3.o
PATCH_OBJS        = patch.o common.o
FPATCH_OBJS       = fpatch.o common.o
ALL_OBJS          = strdiff strdiff3 strdiff3_test unidiff unistrdiff intdiff uintdiff intdiff3 patch fpatch

DEPEND_FILE       = Makefile.depend

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

uintdiff : $(UINTDIFF_OBJS)
	$(CPP) -O2 -o $@ $(UINTDIFF_OBJS)

intdiff3 : $(INTDIFF3_OBJS)
	$(CPP) -O2 -o $@ $(INTDIFF3_OBJS)

patch : $(PATCH_OBJS)
	$(CPP) -O2 -o $@ $(PATCH_OBJS)

fpatch : $(FPATCH_OBJS)
	$(CPP) -O2 -o $@ $(FPATCH_OBJS)

clean:
	${RM} *.o *~ $(ALL_OBJS)

depend:
	@echo "generating dependency file"
	@$(CPP) -MM *.cpp > $(DEPEND_FILE)

include $(DEPEND_FILE)
