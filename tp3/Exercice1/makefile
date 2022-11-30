#----- Fichier genere automatiquement sous Darwin ------
#----- Wed Nov 30 18:37:50 CET 2022

#-----
INCDIR=.
CCFLAGS=-W -Wall -Werror  -pedantic -I$(INCDIR) -I.
LDFLAGS= 
DEPOPT=-M
CCC=gcc
#-----

#-------------  Appli  --------------

TARGET=pipe pipeWc

FILESpipe=pipe.c


LDFLAGSpipe=$(LDFLAGS) 

FILESpipeWc=pipeWc.c


LDFLAGSpipeWc=$(LDFLAGS) 

OBJECTSpipe=$(FILESpipe:.c=.o)

OBJECTSpipeWc=$(FILESpipeWc:.c=.o)

#-------------  Appli  --------------

all: $(TARGET)
#-------------  Appli  --------------

pipe : $(OBJECTSpipe)
	$(CCC) $(OBJECTSpipe) \
		-o pipe $(LDFLAGSpipe)
pipeWc : $(OBJECTSpipeWc)
	$(CCC) $(OBJECTSpipeWc) \
		-o pipeWc $(LDFLAGSpipeWc)
#------------------------------------

.c.o :
	$(CCC) $(CCFLAGS) -c $< \
		-o $*.o
 
dep :
	@echo "======== Mise a jour des dependances : .depend ========"
	@rm -f .depend
	@for i in *.c ; do \
	echo $$i ; \
	$(CCC) $(DEPOPT) $(CCFLAGS) $$i > .tmpdepend ; \
	OBJNAME=`echo $$i | sed -e s%\\\.c%.o% ` ; \
	cat .tmpdepend | \
	sed -e s%`basename $$i .c`\\\.o%$$OBJNAME%         >> .depend ; \
	echo  >> .depend ; \
	done
	@rm -f .tmpdepend
 
CLEANING=rm -f *.o core a.out $(TARGET) *.exe .depend
CONSEIL=echo Penser a faire : make -f makefile dep
 
clear :
	@$(CLEANING)
	@echo
	@$(CONSEIL)
	@echo
clean :
	$(CLEANING)
	@echo
	@$(CONSEIL)
	@echo

sinclude .depend

