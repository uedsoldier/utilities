#include <stdio.h>
#include <stdint.h>
#include "IPv6.h"

IPV6_address_t ip;

const char *ipv6 = "f6c9:c787:d12f:5a33:62c3:089a:70c4:a7c7";

int main(){

    IPV6_fromString(&ip,ipv6);
    printf("IPv6 address: %s\n",string_fromIPV6(&ip));

    return 0;
}