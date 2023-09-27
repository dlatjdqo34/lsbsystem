APP_DIR=embedded
MODULES_DIR=drivers/tutorial/01.hello_module \
			drivers/tutorial/02.export

SUBDIRS=$(APP_DIR) $(MODULES_DIR)

RPI_SERVER_IP?=root@192.168.20.60
RPI_DIR=/root
RPI_PATH?=$(RPI_SERVER_IP):$(RPI_DIR)

SCP_FILES = $(APP_DIR)/lsbsystem \
			drivers/tutorial/01.hello_module/hello_module.ko \
			drivers/tutorial/02.export/export_module1.ko \
			drivers/tutorial/02.export/export_module2.ko \

.PHONY: all clean 
all clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done

.PHONY: app
app:
	$(MAKE) -C $(APP_DIR)

.PHONY: modules
modules:
	@for dir in $(MODULES_DIR); do \
		$(MAKE) -C $$dir; \
	done

.PHONY: run
run:
	$(MAKE) -C $(APP_DIR) $@ 

.PHONY: scp
scp:
	scp $(SCP_FILES) $(RPI_PATH)