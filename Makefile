#Encontrei o corpo desse MAKEFILE no https://github.com/TheMACLayer/atmega328p-tutorial/tree/master/Ep1_makefile/ex2
#Alterei para que fosse possivel utilizar no nosso projeto da caixa dagua
# As alterações que tive que fazer foram :
#Criar mais FILENAMES para cada uma das bibliotecas
#Alterar o programmer para arduino 
#e a porta pois no projeto do git está para MAC e estou utilizando nesse momento o UBUNTU 


FILENAME   = app/main
FILENAME2  = app/display
FILENAME3  = app/ultrassom
FILENAME4  = app/bluetooh
PORT       = /dev/ttyACM*
DEVICE     = atmega328p
PROGRAMMER = arduino
BAUD       = 115200
COMPILE    = avr-gcc -Wall -Os -mmcu=$(DEVICE)


default: compile upload clean

compile:
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -c $(FILENAME2).c -o $(FILENAME2).o
	$(COMPILE) -c $(FILENAME3).c -o $(FILENAME3).o
	$(COMPILE) -c $(FILENAME4).c -o $(FILENAME4).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o $(FILENAME2).o $(FILENAME3).o $(FILENAME4).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex 
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

upload:
	avrdude  -F -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i 
	
#-e -Ulock:w:0xff:m -Uefuse:w:0xfd:m -Uhfuse:w:0xde:m

clean:
	rm $(FILENAME).o
	rm $(FILENAME2).o
	rm $(FILENAME3).o
	rm $(FILENAME4).o
	rm $(FILENAME).elf
	rm $(FILENAME).hex