# Makefile for my PangoCairo on OpenGL Library: PaCaLib
#

.PHONY: all
all:
	@(cd src && $(MAKE) -s)

.PHONY: clean
clean:
	(cd src && $(MAKE) -s clean)
	rm -rf doc

.PHONY: doc
doc:
	@doxygen Doxyfile

