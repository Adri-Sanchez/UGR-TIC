escritorPort=/dev/ttyACM0
lectorPort=/dev/ttyACM0
INCLUDES=-I./include -I./lib
INCLUDEDIR=./include
LIBDIR=./lib
OBJDIR=./obj
CPPDIR=./src
BINDIR=./bin
NAME=receptorHuffman

all: escritorPc escritorArduino sendArduino 
escritorArduino:
	avr-gcc -Os -mmcu=atmega328p -c -o $(OBJDIR)/$(NAME).o $(CPPDIR)/$(NAME).cpp $(INCLUDES)
	avr-gcc -mmcu=atmega328p $(OBJDIR)/$(NAME).o $(INCLUDES) -lm -lArduTIC -L$(LIBDIR) -o $(OBJDIR)/$(NAME).bin
	avr-objcopy -O ihex -R .eeprom $(OBJDIR)/$(NAME).bin $(BINDIR)/$(NAME).hex


sendArduino:
	avrdude -F -V -c arduino -p ATMEGA328P -P $(escritorPort) - b 115200 -U flash:w:$(BINDIR)/$(NAME).hex
escritorPc:
	g++ -o $(BINDIR)/receptorPC $(CPPDIR)/receptorPC.cpp $(LIBDIR)/PCSerialUSB.cpp  $(INCLUDES) -std=c++11