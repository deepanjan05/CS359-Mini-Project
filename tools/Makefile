CPPFLAGS = -I ../include -Wall -Werror

all: libcs359

debug: CFLAGS+= -DDEBUG_API
debug: libcs359

libcs359: libcs359.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -fPIC -shared -o libcs359.so libcs359.c -ldl

.PHONY:
clean:
	rm libcs359.so*
