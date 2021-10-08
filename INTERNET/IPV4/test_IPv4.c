#include <stdio.h>
#include <stdint.h>
#include "IPv4.h"

#define IP_TEST 0xC0A80019
IPV4_address_t ip;
int main(){

    makeIPfromString(&ip,"192.168.0.1");
    uint32_t ip_int = makeIPintFromIP(&ip);

    printf("0x%X\n",ip_int);

    makeStringFromIP(&ip);
    
    makeStringFromInt(0xC0A80019);

    return 0;
}