# Folders
SUBDIRS = Idp.Gpx.Partnerize Idp.Gpx.Silhouette Idp.Gpx.Snatch

# Rules.
.PHONY: all
all:	$(SUBDIRS)
	cp $(BUILD_DIR)/Idp.Gpx.Partnerize $(BIN_DIR)/part
	cp $(BUILD_DIR)/Idp.Gpx.Silhouette $(BIN_DIR)/silh
	cp $(BUILD_DIR)/Idp.Gpx.Snatch $(BIN_DIR)/snatch

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	dotnet publish $@ -c Release -o $(BUILD_DIR) \
		/p:PublishSingleFile=true -r linux-x64 \
		/p:DebugType=None /p:DebugSymbols=false