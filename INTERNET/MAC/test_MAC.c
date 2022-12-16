#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MAC.h"

uint8_t mac_array[] = {0x55, 0x71, 0x93, 0xbc, 0xae, 0x0e};
MAC_address_t mac;

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("A MAC argument is required");
        return EXIT_FAILURE;
    }
    MAC_error_t error = MAC_fromString(&mac, argv[1]);
    

    switch (error)
    {
    case MAC_ADDRESS_OK:
        printf("Valid MAC\n");
        printf("String from that MAC: %s\n",string_fromMAC(&mac,false));

        MAC_fromArray(&mac,mac_array);
        printf("MAC from array: %s\n",string_fromMAC(&mac,true));
        return EXIT_SUCCESS;
        break;
    case MAC_INVALID_ADDRESS:
        printf("Invalid address");
        break;
    case MAC_INVALID_ARRAY_LENGTH:
        printf("Invalid array length");
        break;
    case MAC_NaN:
        printf("MAC NaN");
        break;
    case MAC_NULL_STRING:
        printf("Null string");
        break;
    case MAC_NULL_TOKEN:
        printf("Null token");
        break;
    case MAC_INVALID_NUMBER:
        printf("Invalid number");
        break;
    default:
        break;
    }
}