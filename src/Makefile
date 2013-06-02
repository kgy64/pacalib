# Makefile for my GLES-based library for PangoCairo: PaCaLib
#

NAME                 =  PaCaLib
VERS_MAJOR           =  0
VERS_MINOR           =  1

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

BASE_LIBRARIES       =  pangocairo

export SRCDIR        =  .

.PHONY: all
all: _everything

include $(SCRIPTDIR)/makelib

export LFLAGS       +=  $(MY_PROJECT_LIBS)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

.PHONY: clean
clean:
	@echo " - Cleaning $(NAME)..."
	@rm -f $(BINDIR)/* $(OBJECTS) $(Y_CPPFILES) $(Y_HEADERS) $(DEPENDS)

