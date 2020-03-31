#include "ethernet.h"
#include "icmpv4.h"
#include "ip.h"
#include "utils.h"

void icmpv4_incoming(struct sk_buff *skb) 
{
    struct iphdr *iphdr = ip_hdr(skb);
    struct icmp_v4 *icmp = (struct icmp_v4 *) iphdr->data;

    //TODO: Check csum

    switch (icmp->type) {
    case ICMP_V4_ECHO:
        printf("ICMP V4 Echo request recieved!\n");
        return;
    case ICMP_V4_DST_UNREACHABLE:
        print_err("ICMPv4 received 'dst unreachable' code %d, "
                  "check your routes and firewall rules\n", icmp->code);
        goto drop_pkt;
    default:
        print_err("ICMPv4 did not match supported types\n");
        goto drop_pkt;
    }

drop_pkt:
    free_skb(skb);
    return;
}

// TODO: ICMPV4 REPLY