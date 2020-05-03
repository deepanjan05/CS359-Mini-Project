CPPFLAGS = -I include -Wall -Werror -pthread

src = $(wildcard src/*.c)
obj = $(patsubst src/%.c, build/%.o, $(src))
headers = $(wildcard include/*.h)

prereq:
	sudo apt-get install libcap-dev

cs359: $(obj)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(obj) -o cs359
	@echo
	@echo "cs359 needs CAP_NET_ADMIN:"
	sudo setcap cap_setpcap,cap_net_admin=ep cs359

build/%.o: src/%.c ${headers}
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

debug: CFLAGS+= -DDEBUG_SOCKET -DDEBUG_TCP -g -fsanitize=thread
debug: cs359

apps: $(apps)
	$(MAKE) -C tools

all: prereq cs359 apps

clean:
	rm build/*.o cs359
