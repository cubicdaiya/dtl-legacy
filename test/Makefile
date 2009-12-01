# Makefile for test programs

CPP        = g++
CPPFLAGS   = -c -O2 -Wall
OBJS_FLAGS = -O2 -o
LIBS       = -lgtest
OBJS       = dtl_test.o

DEPENDENCY_FILE = Makefile.depend

.cpp.o:
	$(CPP) $(CPPFLAGS) $< $(LIBS)

dtl_test : $(OBJS)
	$(CPP) $(OBJS_FLAGS) $@ $(OBJS) $(LIBS)

test : 
	@./dtl_test

clean :
	${RM} *.o *~ dtl_test

depend:
	@echo "generating dependency file"
	@$(CPP) -MM *.cpp > $(DEPENDENCY_FILE)

include $(DEPENDENCY_FILE)
