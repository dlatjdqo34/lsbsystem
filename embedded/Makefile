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
LDFLAGS=-lm -lrt -Wl,--no-as-needed -ldl -lseccomp

_OBJS = main.o \
		$(SYSTEM)/system_server.o \
		$(UI)/input.o \
		$(UI)/ui.o \
		$(UTILS)/mq.o \
		$(UTILS)/utils.o \
		$(WEB)/web_server.o 

OBJS = $(patsubst %,$(OUTDIR)/%, $(_OBJS))

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) -o $@ $(OBJS)
	@cp $@ ../

$(OUTDIR)/%.o: %.c
	$(shell mkdir -p $(OUTSUBDIRS))
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) -c -o $@ $<

.PHONY: clean
clean: 
	rm -rf $(OUTDIR) $(TARGET) ../$(TARGET)


