#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "IPv6.h"

IPV6_address_t ip;

const char *ipv6 = "f6c9:c787:d12f:0:0:089a:70c4:a7c7";
uint16_t ip_array[] = {0xAABB, 0x0000, 0x9336, 0xbc78, 0xae00, 0x0ef5, 0x1550, 0x74fa };

int main(int argc, char *argv[]){
    if (argc != 2)
    {
        printf("A IPv6 argument is required");
        return EXIT_FAILURE;
    }

    IPV6_error_t error = IPV6_fromString(&ip,argv[1]);
    switch (error)
    {
    case IPV6_ADDRESS_OK:
        printf("MAC OK\n");
        printf("String from that MAC: %s\n",string_fromIPV6(&ip,false));

        IPV6_fromArray(&ip,ip_array);
        printf("IPv6 from array: %s\n",string_fromIPV6(&ip,true));
        printf("\nCaso 2:\n");
        error = IPV6_fromString(&ip,ipv6);
        printf("IPv6 address: %s\n",string_fromIPV6(&ip,true));
        return EXIT_SUCCESS;
        break;
    case IPV6_INVALID_ADDRESS:
        printf("Invalid address");
        break;
    case IPV6_INVALID_ARRAY_LENGTH:
        printf("Invalid array length");
        break;
    case IPV6_NaN:
        printf("MAC NaN");
        break;
    case IPV6_NULL_STRING:
        printf("Null string");
        break;
    case IPV6_NULL_TOKEN:
        printf("Null token");
        break;
    case IPV6_INVALID_NUMBER:
        printf("Invalid number");
        break;
    default:
        break;
    }
    

    return 0;
}