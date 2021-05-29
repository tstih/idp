# Folders
SUBDIRS = stdlib gpx

# Rules.
.PHONY: all
all:	$(SUBDIRS) 

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
