# Folders
SUBDIRS = stdlib gpx hal

# Rules.
.PHONY: all
all:	$(SUBDIRS) 

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
