# Makefile for my PangoCairo on OpenGL Library: PaCaLib
#

.PHONY: all
all:

.PHONY: clean
clean:
	(cd src && $(MAKE) -s clean)
	rm -rf doc

