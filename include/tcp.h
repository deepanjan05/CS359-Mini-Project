#ifndef TCP_H_
#define TCP_H_
#include "syshead.h"
#include "ip.h"
#include "utils.h"

#define TCP_HDR_LEN sizeof(struct tcphdr)
#define TCP_DOFFSET sizeof(struct tcphdr) / 4

#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PSH 0x08
#define TCP_ACK 0x10

#define TCP_URG 0x20
#define TCP_ECN 0x40
#define TCP_WIN 0x80

#define TCP_SYN_BACKOFF 500
#define TCP_CONN_RETRIES 3

#define TCP_OPT_NOOP 1
#define TCP_OPTLEN_MSS 4
#define TCP_OPT_MSS 2
#define TCP_OPT_SACK_OK 4
#define TCP_OPT_SACK 5
#define TCP_OPTLEN_SACK 2
#define TCP_OPT_TS 8

#define TCP_2MSL 60000
#define TCP_USER_TIMEOUT 180000

#define tcp_hlen(tcp) (tcp->hl << 2)

#ifdef DEBUG_TCP
extern const char *tcp_dbg_states[];

#else
#endif

struct tcphdr {
    uint16_t sport;
    uint16_t dport;
    uint32_t seq;
    uint32_t ack_seq;
    uint8_t rsvd : 4;
    uint8_t hl : 4;
    uint8_t fin : 1,
            syn : 1,
            rst : 1,
            psh : 1,
            ack : 1,
            urg : 1,
            ece : 1,
            cwr : 1;
    uint16_t win;
    uint16_t csum;
    uint16_t urp;
    uint8_t data[];
} __attribute__((packed));

struct tcp_options {
    uint16_t options;
    uint16_t mss;
    uint8_t sack;
};

struct tcp_opt_mss {
    uint8_t kind;
    uint8_t len;
    uint16_t mss;
} __attribute__((packed));

struct tcpiphdr {
    uint32_t saddr;
    uint32_t daddr;
    uint8_t zero;
    uint8_t proto;
    uint16_t tlen;
} __attribute__((packed));

enum tcp_states {
    TCP_LISTEN, /* represents waiting for a connection request from any remote
                   TCP and port. */
    TCP_SYN_SENT, /* represents waiting for a matching connection request
                     after having sent a connection request. */
    TCP_SYN_RECEIVED, /* represents waiting for a confirming connection
                         request acknowledgment after having both received and sent a
                         connection request. */
    TCP_ESTABLISHED, /* represents an open connection, data received can be
                        delivered to the user.  The normal state for the data transfer phase
                        of the connection. */
    TCP_FIN_WAIT_1, /* represents waiting for a connection termination request
                       from the remote TCP, or an acknowledgment of the connection
                       termination request previously sent. */
    TCP_FIN_WAIT_2, /* represents waiting for a connection termination request
                       from the remote TCP. */
    TCP_CLOSE, /* represents no connection state at all. */
    TCP_CLOSE_WAIT, /* represents waiting for a connection termination request
                       from the local user. */
    TCP_CLOSING, /* represents waiting for a connection termination request
                    acknowledgment from the remote TCP. */
    TCP_LAST_ACK, /* represents waiting for an acknowledgment of the
                     connection termination request previously sent to the remote TCP
                     (which includes an acknowledgment of its connection termination
                     request). */
    TCP_TIME_WAIT, /* represents waiting for enough time to pass to be sure
                      the remote TCP received the acknowledgment of its connection
                      termination request. */
};

struct tcb {
    uint32_t snd_una; /* oldest unacknowledged sequence number */
    uint32_t snd_nxt; /* next sequence number to be sent */
    uint32_t snd_wnd;
    uint32_t snd_up;
    uint32_t snd_wl1;
    uint32_t snd_wl2;
    uint32_t iss;
    uint32_t rcv_nxt; /* next sequence number expected on an incoming segments, and
                         is the left or lower edge of the receive window */
    uint32_t rcv_wnd;
    uint32_t rcv_up;
    uint32_t irs;
};

static inline struct tcphdr *tcp_hdr(const struct sk_buff *skb)
{
    return (struct tcphdr *)(skb->head + ETH_HDR_LEN + IP_HDR_LEN);
}

void tcp_init();
void tcp_select_initial_window(uint32_t *rcv_wnd);

int generate_iss();

#endif
