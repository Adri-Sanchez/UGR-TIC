escritorPORT=/dev/ttyACM0
receptorPORT=/dev/ttyACM0
INCLUDES=-I./include -I./lib
INCLUDEDIR=./include
LIBDIR=./lib
OBJDIR=./obj
CPPDIR=./src
BINDIR=./bin
ARDUNAME=emisorArdu
PCNAME=emisorPC

# ------------------------------------------------
# All
# ------------------------------------------------
all: programaArduino programaPC send
	
# ------------------------------------------------
# Programa para Arduino
# ------------------------------------------------
programaArduino:
	avr-gcc -Os -mmcu=atmega328p -c -o $(OBJDIR)/$(ARDUNAME).o $(CPPDIR)/$(ARDUNAME).cpp $(INCLUDES)
	avr-gcc -Os -mmcu=atmega328p $(OBJDIR)/$(ARDUNAME).o $(INCLUDES) -lm -lArduTIC -L$(LIBDIR) -o $(OBJDIR)/$(ARDUNAME).bin
	avr-objcopy -O ihex -R .eeprom $(OBJDIR)/$(ARDUNAME).bin $(BINDIR)/$(ARDUNAME).hex

send:
	avrdude -F -V -c arduino -p ATMEGA328P -P $(escritorPORT) -b 115200 -U flash:w:$(BINDIR)/$(ARDUNAME).hex

# ------------------------------------------------
# Programa en PC
# ------------------------------------------------
programaPC:
	g++ -o$(BINDIR)/$(PCNAME) $(CPPDIR)/$(PCNAME).cpp $(LIBDIR)/PCSerialUSB.cpp $(INCLUDES) -std=c++11 -w

clean:
	rm -f -r *~
	rm -f -r obj/*
	rm -f -r src/*~

cleanall: clean
	rm -f -r bin/*

