#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "IPv4.h"

#define IP_TEST 0xC0A80019
IPV4_address_t ip;

uint8_t ip_array[] = {100,101,102,103};

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("IPV4 argument is required");
        return EXIT_FAILURE;
    }
    if( IPV4_fromString(&ip,argv[1]) == IPV4_ADDRESS_OK){
        printf("Valid IP\n");
        uint32_t ip_int = IPV4_int_fromIPv4(&ip);
        printf("IP from string: %s\n",string_fromIPV4(&ip));
    } else{
        printf("Invalid IP ");
        return EXIT_FAILURE;
    }

    if(IPV4_fromArray(&ip,ip_array) == IPV4_ADDRESS_OK){
        printf("Array OK. IP: %s\n",string_fromIPV4(&ip));
    } else{
        printf("Array not OK\n");
    }

    return EXIT_SUCCESS;
}