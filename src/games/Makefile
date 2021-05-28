# Folders
SUBDIRS = tetris pong

# Rules.
.PHONY: all
all:	$(SUBDIRS) 

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
