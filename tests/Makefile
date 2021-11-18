.PHONY: test build %

test: build
	@$(MAKE) -f GeneralMakefile test

build:
	@echo Building tests...
	./buildTests.sh $(BUILDDIR)
	@echo

%:
	@$(MAKE) -f GeneralMakefile $@
