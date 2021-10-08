#include "IPv4.h"

/**
 * 
 * @param address
 * @param bytes
 */
void makeIPfromArray(IPV4_address_t *address, uint8_t *bytes){
    #if IPV4_LOG
    printf("IPv4 from array: ");
    #endif
    for(uint8_t i = 0; i != 4; i++){
        address->ipv4_addr_array[i] = bytes[i];
        #if IPV4_LOG
        printf("%u%c",address->ipv4_addr_array[i],(i!=3)? '.':' '); 
        #endif
    }
    #if IPV4_LOG
    printf("\r\n");
    #endif
    
}

/**
 * 
 * @param address
 * @param bytes
 */
void makeArrayFromIP(IPV4_address_t *address, uint8_t *bytes){
    #if IPV4_LOG
    printf("Array from IPv4: ");
    #endif
    for(uint8_t i = 0; i != 4; i++){
        bytes[i] = address->ipv4_addr_array[i];
        #if IPV4_LOG
        printf("%u%c",bytes[i],(i!=3)? '.':' '); 
        #endif
    }
    #if IPV4_LOG
    printf("\r\n");
    #endif
}

/**
 * 
 * @param address
 * @param string
 */
void makeIPfromString(IPV4_address_t *address, char *string){
    char _str[40];
    memset(_str,0,sizeof(_str));
    strcpy(_str,string);
    #if IPV4_LOG
    printf("IPv4 from string %s\r\n",string);
    #endif
    char *token = strtok(_str,".");
    uint8_t index = 3;
    while(token != NULL){
        address->ipv4_addr_array[index] = atoi(token);
        token = strtok(NULL,".");
        #if IPV4_LOG
        printf("%u%c",address->ipv4_addr_array[index],(index!=0)? '.':' '); 
        #endif
        index--;
    }
    #if IPV4_LOG
    printf("\r\n");
    #endif
}

/**
 * 
 * @param address
 * @return 
 */
char *makeStringFromIP(IPV4_address_t *address){
    static char retString[20];
    #if IPV4_LOG
    printf("Make IPv4 string from IPv4: ");
    #endif
    sprintf(retString,"%u.%u.%u.%u",address->ipv4_bytes.b3,address->ipv4_bytes.b2,address->ipv4_bytes.b1,address->ipv4_bytes.b0);
    #if IPV4_LOG
    printf("%s\r\n",retString);
    #endif
    return retString;
}

/**
 * 
 * @param ip_int
 * @return 
 */
char *makeStringFromInt(uint32_t ip_int){
    static char retString[20];
    #if IPV4_LOG
    printf("Make IPv4 string from int: ");
    #endif
    sprintf(retString,"%u.%u.%u.%u",(uint8_t)((ip_int & 0xFF000000UL)>>24),(uint8_t)((ip_int & 0xFF0000UL)>>16),(uint8_t)((ip_int & 0xFF00UL)>>8),(uint8_t)((ip_int) & 0xFFUL));
    #if IPV4_LOG
    printf("%s\r\n",retString);
    #endif
    return retString;
}

/**
 * 
 * @param mask
 * @return 
 */
bool IPV4_validMask(IPV4_address_t *subnetmask){
    uint32_t mask = subnetmask->ipv4_word;
    if (mask == 0) return true;
    if (mask & (~mask >> 1)) {
        return true;
    } else {
        return false;
    }
}


/**
 * 
 * @param address
 * @return 
 */
uint32_t makeIPintFromIP(IPV4_address_t *address){
    return address->ipv4_word;
}