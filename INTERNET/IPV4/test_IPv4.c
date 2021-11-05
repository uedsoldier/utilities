#include <stdio.h>
#include <stdint.h>
#include "IPv4.h"

#define IP_TEST 0xC0A80019
IPV4_address_t ip;
int main(){

    IPv4_fromString(&ip,"192.168.0.1");
    uint32_t ip_int = IPv4_int_fromIPv4(&ip);

    printf("0x%X\n",ip_int);

    string_fromIPv4(&ip);
    
    IPv4_string_fromInt(0xC0A80019);

    return 0;
}