#ifndef LIBCS359_H_
#define LIBCS359_H_

#include <poll.h>
#include <dlfcn.h>
#include "list.h"
#include "utils.h"

#ifdef DEBUG_API
#define lvl_dbg(msg, ...)                                               \
    do {                                                                \
        print_debug("cs359 ttid %lu "msg, pthread_self(), ##__VA_ARGS__); \
    } while (0)
#define lvl_sock_dbg(msg, sock, ...)                                        \
    do {                                                                \
        lvl_dbg("lvlfd %d fd %d: "msg, sock->lvlfd, sock->fd, ##__VA_ARGS__); \
    } while (0)
#else
#define lvl_sock_dbg(msg, sock, ...)
#define lvl_dbg(msg, ...)
#endif

struct cs359_sock {
    struct list_head list;
    int lvlfd; /* For CS359 IPC */
    int fd;
};

static inline struct cs359_sock *cs359_alloc() {
    struct cs359_sock *sock = malloc(sizeof(struct cs359_sock));
    memset(sock, 0, sizeof(struct cs359_sock));

    return sock;
};

static inline void cs359_free(struct cs359_sock *sock) {
    free(sock);
}

#endif
