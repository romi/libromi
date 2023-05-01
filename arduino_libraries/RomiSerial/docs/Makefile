
LIB_SRC=../Console.cpp \
	../CRC8.cpp \
	../EnvelopeParser.cpp \
	../MessageParser.cpp \
	../Printer.cpp \
	../Reader.cpp \
	../RomiSerialClient.cpp \
	../RomiSerial.cpp \
	../RomiSerialUtil.cpp \
	../RSerial.cpp  \
	../rtime.cpp

all:
	g++ -g -O0 analogread.cpp $(LIB_SRC) -I ../../RomiSerial -o analogread_app
	g++ -g -O0 blink.cpp $(LIB_SRC) -I ../../RomiSerial -o blink_app
