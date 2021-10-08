#include "MAC.h"

/**
 * 
 * @param address
 * @param bytes
 */
void makeMACfromArray(MAC_address_t *address, uint8_t *bytes){
    #ifdef MAC_LOG
    printf("\tMAC from array: ");
    #endif
    for(uint8_t i = 0; i != 6; i++){
        address->MAC_array[i] = bytes[i];
        #ifdef MAC_LOG
        printf("%02X%c",address->MAC_array[i],(i!=5)? ':':' '); 
        #endif
    }
    #ifdef MAC_LOG
    printf("\r\n");
    #endif
}

/**
 * 
 * @param address
 * @param bytes
 */
void makeArrayFromMAC(MAC_address_t *address, uint8_t *bytes){
    #ifdef MAC_LOG
    printf("\tArray from MAC: ");
    #endif
    for(uint8_t i = 0; i != 6; i++){
        bytes[i] = address->MAC_array[i];
        #ifdef MAC_LOG
        printf("%02X%c",bytes[i],(i!=5)? ':':' '); 
        #endif
    }
    #ifdef MAC_LOG
    printf("\r\n");
    #endif
}

/**
 * 
 * @param address
 * @param string
 */
void makeMACfromString(MAC_address_t *address, char *string){
    #ifdef MAC_LOG
    printf("\tMAC from string: ");
    #endif
    char *token = strtok(string,":");
    int16_t index = 0;
    while(token != NULL){
        address->MAC_array[index] = strtoul(token,NULL,16);
        token = strtok(NULL,":");
        #ifdef MAC_LOG
        printf("%02X%c",address->MAC_array[index],(index!=5)? ':':' '); 
        #endif
        index++;
    }
    #ifdef MAC_LOG
    printf("\r\n");
    #endif
}

/**
 * 
 * @param address
 * @return 
 */
char *makeStringFromMAC(MAC_address_t *address){
    static char retString[20];
    sprintf(retString,"%02X:%02X:%02X:%02X:%02X:%02X",address->MAC_bytes.b0,address->MAC_bytes.b1,address->MAC_bytes.b2,address->MAC_bytes.b3,address->MAC_bytes.b4,address->MAC_bytes.b5);
    return retString;
}