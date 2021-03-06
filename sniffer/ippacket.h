#ifndef _IPPACKET_H
#define _IPPACKET_H

#include <stdbool.h>
#include "ipaddress.h"

typedef struct ippacket_ ippacket;

typedef enum ipprotocol_ {
    ipp_icmp, ipp_igmp, ipp_udp, ipp_tcp, ipp_other, ipp_none
} ipprotocol;

struct ippacket_ {
    bool owned;
    unsigned char *p_data;
    unsigned int p_len;
    void (*print_ippacket)(ippacket *self);
    unsigned int(*version)(ippacket *self);
    unsigned int(*ip_header_length)(ippacket *self);
    unsigned int(*ihl)(ippacket *self);
    unsigned int(*tos)(ippacket *self);
    unsigned int(*total_length)(ippacket *self);
    unsigned int(*fragment_id)(ippacket *self);
    unsigned int(*fragment_flags)(ippacket *self);
    unsigned int(*fragment_pos)(ippacket *self);
    unsigned int(*protocol_id)(ippacket *self);
    unsigned int(*protocol)(ippacket *self);
    unsigned int(*ttl)(ippacket *self);
    unsigned int(*checksum)(ippacket *self);
    ipaddress* (*destination_ip)(ippacket *self);
    ipaddress* (*source_ip)(ippacket *self);
    unsigned int(*count_options)(ippacket *self);
    bool (*option_header)(ippacket *self, unsigned int idx, unsigned int *optclass, unsigned int *optnumber, unsigned int *optlen);
};

ippacket* new_ippacket(bool owned, unsigned char *p_data, unsigned int p_len);

#endif
