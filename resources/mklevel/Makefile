CC		:=	/usr/bin/g++
CFLAGS	?=	-std=c++17 -Wall -O3
LD		:=	/usr/bin/ld
OUT		:=	mklevel

LIBS	:= -lpugixml -lcsv
OBJS	:=	mklevel.o

all: $(OUT)

clean:
	@rm -rf *.o $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

mklevel.o: mklevel.cpp

