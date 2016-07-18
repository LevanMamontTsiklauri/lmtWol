CC=gcc
CFLAGS=-G


wol:
	$(CC) -o wol lmtWol.c

wolReceiver:
	$(CC) -o wolReceiver wolReceiver.c

all: wol wolReceiver

clean:
	rm -rf wol wolReceiver
	