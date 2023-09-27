TARGET=lsbsystem

SYSTEM=system
UI=ui
WEB=web
UTILS=utils

DIRS =$(SYSTEM) $(UI) $(WEB) $(UTILS)
OUTDIR = obj
OUTSUBDIRS =$(patsubst %,$(OUTDIR)/%, $(DIRS))

INCLUDE =$(patsubst %,-I%, $(DIRS))

CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lm -lrt -Wl,--no-as-needed -ldl -lseccomp -lpthread

_OBJS = main.o \
		$(SYSTEM)/system_server.o \
		$(UI)/input.o \
		$(UI)/ui.o \
		$(UTILS)/lsb_mq.o \
		$(UTILS)/lsb_timer.o \
		$(UTILS)/utils.o \
		$(WEB)/web_server.o 

OBJS = $(patsubst %,$(OUTDIR)/%, $(_OBJS))

RPI_SERVER_IP?=root@192.168.20.60
RPI_DIR=/root
RPI_PATH?=$(RPI_SERVER_IP):$(RPI_DIR)

.PHONY: all
all default: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) -o $@ $(OBJS)

$(OUTDIR)/%.o: %.c
	$(shell mkdir -p $(OUTSUBDIRS))
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) -c -o $@ $<

.PHONY: run scp clean
run:
	@./$(TARGET)

scp:
	scp lsbsystem $(RPI_PATH)

clean: 
	rm -rf $(OUTDIR) $(TARGET)


