CXX = gcc
CFLAGS = -Wall -g -std=c99
LIBS    = -lbcm2835 -lm -lcurl

CORE = Test Example_Lights Example_Door
OBJS = CloudTools.o PiTools.o TFT_Printer.o TFT.o SPI.o SensorsInterface.o MPL3115A2.o i2c.o APDS9300.o CAP1203.o FXOS8700CQ.o MCP79410.o led.o Utilities.o
FILES = Makefile CloudTools.h CloudTools.c PiTools.h PiTools.c TFT_Printer.h TFT_Printer.c TFT.h TFT.c Font.h SPI.h SPI.c MPL3115A2.h MPL3115A2.c APDS9300.c APDS9300.h CAP1203.c CAP1203.h FXOS8700CQ.c FXOS8700CQ.h MCP79410.c MCP79410.h led.c led.h SensorsInterface.h SensorsInterface.c i2c.c i2c.h Utilities.c Utilities.h

all: $(CORE)

Test: Test.o $(OBJS) Test.c $(FILES)
	$(CXX) $(CFLAGS) -o Test Test.c $(OBJS) $(LIBS)
	
Example_Lights: Example_Lights.o $(OBJS) Example_Lights.c $(FILES)
	$(CXX) $(CFLAGS) -o Example_Lights Example_Lights.c $(OBJS) $(LIBS)
	
Example_Door: Example_Door.o $(OBJS) Example_Door.c $(FILES)
	$(CXX) $(CFLAGS) -o Example_Door Example_Door.c $(OBJS) $(LIBS)

clean:
	rm -f $(CORE)
	rm -f *.o

%.o: %.c  $(FILES)
	$(CXX) -c $(CFLAGS) $< -o $@


