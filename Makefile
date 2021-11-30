#Encontrei o corpo desse MAKEFILE no https://github.com/TheMACLayer/atmega328p-tutorial/tree/master/Ep1_makefile/ex2
#Alterei para que fosse possivel utilizar no nosso projeto da caixa dagua
# As alterações que tive que fazer foram :
#Criar mais FILENAMES para cada uma das bibliotecas
#Alterar o programmer para arduino 
#e a porta pois no projeto do git está para MAC e estou utilizando nesse momento o UBUNTU 


FILENAME   = app/main
FILENAME2  = app/display
FILENAME3  = app/uart_hal
FILENAME5  = app/memoria
FILENAME6  = app/twimaster
FILENAME7  = app/pcf8574
FILENAME8  = app/lcdpcf8574
PORT       = /dev/ttyACM*
DEVICE     = atmega328p
PROGRAMMER = arduino
BAUD       = 115200
COMPILE    = avr-gcc -Wall -Os -mmcu=$(DEVICE)


all: compile upload clean

compile:
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -c $(FILENAME2).c -o $(FILENAME2).o
	$(COMPILE) -c $(FILENAME3).c -o $(FILENAME3).o
	$(COMPILE) -c $(FILENAME5).c -o $(FILENAME5).o
	$(COMPILE) -c $(FILENAME6).c -o $(FILENAME6).o
	$(COMPILE) -c $(FILENAME7).c -o $(FILENAME7).o
	$(COMPILE) -c $(FILENAME8).c -o $(FILENAME8).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o $(FILENAME2).o $(FILENAME3).o  $(FILENAME5).o $(FILENAME6).o $(FILENAME7).o $(FILENAME8).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex 
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i 
	
	
clean:
	rm $(FILENAME).o
	rm $(FILENAME2).o
	rm $(FILENAME3).o
	rm $(FILENAME5).o
	rm $(FILENAME6).o
	rm $(FILENAME7).o
	rm $(FILENAME8).o
	rm $(FILENAME).elf
	rm $(FILENAME).hex

