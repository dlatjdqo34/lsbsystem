SUBDIRS = embedded


.PHONY: all clean

all clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
