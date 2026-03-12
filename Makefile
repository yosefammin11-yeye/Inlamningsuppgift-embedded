CC      := avr-gcc
LD      := avr-ld
OBJCOPY := avr-objcopy
OBJISP  := avrdude
# Med avr-size kan vi få ett hum om hur stor plats vårat program tar i minnet
# Under kompilering ger det ut en rapport som ser ut ungefär så här:
#    text    data     bss     dec     hex filename
#    2524      42      18    2584     a18 program.elf
# där 'text' är hur stor plats själva programmet tar i flash-minnet,
# 'data' är hur stor plats initialiserad data tar i RAM-minnet,
# 'bss' är hur stor plats icke-initialiserad data och noll-initialiserad data tar i RAM-minnet
# 'dec' och 'hex' är den sammanlagda storleken uttryckt i decimalt och hexadecimalt
# (i exemplet: 2524 + 42 + 18 = 2584 = 0xA18)
# och 'filename' är namnet på filen avr-size undersökt
AVRSIZE := avr-size
PORT    := /dev/ttyUSB0
# För att kunna flasha måste detta kopplas till den port arduinon är inkopplad till
# På Linux kan man hitta det genom att köra kommandot <sudo dmesg | grep tty>
# På Windows får man nog kolla i device managern/enhetshanteraren, och det ser ut så här för COM3 som ett exempel: PORT := \\\\.\\COM3

MCU := atmega328p

CFLAGS  := -Wall -Wextra  -Wundef -pedantic \
		-Os -std=gnu99 -DF_CPU=16000000UL -mmcu=${MCU}
LDFLAGS := -mmcu=$(MCU)

BIN := program.hex
SOURCES := main.c lcd.c
OBJS := $(SOURCES:.c=.o)

# .PHONY är en tag som säger till att dessa taggar inte är associerade med
# filer, utan ska alltid köras när de anropas.
# Detta behövs eftersom Makefile targets normalt bara körs om det skett en
# förändring i minst en av filerna den beror på.
# Makefiles struktur är ju <target: prerequisites> där vi kan ta som exempel
# <%.hex: %.elf> som säger att nu ska vi bygga *.hex för alla *.elf-filer vi hittar
.PHONY: clean fresh all isp

all: $(BIN)

%.o:%.c
	$(COMPILE.c) -MD -o $@ $<

%.elf: $(OBJS)
	$(CC) -Wl,-Map=$(@:.elf=.map) $(LDFLAGS) -o $@ $^
	$(AVRSIZE) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .fuse -R .lock -R .user_signatures -R .comment $< $@

# Med detta kan vi flasha vårat program till en inkopplad Arduino genom att skriva <make isp> istället för bara <make>
isp: ${BIN}
	@sudo $(OBJISP) -F -V -c arduino -p ${MCU} -P ${PORT} -U flash:w:$<

clean:
	@rm -f $(OBJS) *.map *.P *.d

fresh: clean
	@rm -rv $(BIN)
