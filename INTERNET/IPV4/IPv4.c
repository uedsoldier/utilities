#include "IPv4.h"

/**
 * 
 * @param address
 * @param bytes
 */
void IPV4_fromArray(IPV4_address_t *address, uint8_t *bytes){
    #if IPV4_LOG
    printf("IPv4 from array: ");
    #endif
    for(uint8_t i = 0; i != IPV4_BYTE_COUNT; i++){
        address->ipv4_addr_array[i] = bytes[i];
        #if IPV4_LOG
        printf("%u%c",address->ipv4_addr_array[i],(i!=3)? IPV4_STRING_SEPARATOR:'\0'); 
        #endif
    }
    #if IPV4_LOG
    printf("\n");
    #endif
    
}

/**
 * 
 * @param address
 * @param bytes
 */
void array_fromIPV4(IPV4_address_t *address, uint8_t *bytes){
    #if IPV4_LOG
    printf("Array from IPv4: ");
    #endif
    for(uint8_t i = 0; i != IPV4_BYTE_COUNT; i++){
        bytes[i] = address->ipv4_addr_array[i];
        #if IPV4_LOG
        printf("%u%c",bytes[i],(i!=3)? IPV4_STRING_SEPARATOR:'\0'); 
        #endif
    }
    #if IPV4_LOG
    printf("\n");
    #endif
}

/**
 * 
 * @param address
 * @param string
 */
void IPV4_fromString(IPV4_address_t *address, const char *string){
    char _str[20];
    memset(_str,0,sizeof(_str));
    strcpy(_str,string);
    #if IPV4_LOG
    printf("IPv4 from string %s\n",string);
    #endif
    char *token = strtok(_str,".");
    uint8_t index = 3;
    while(token != NULL){
        address->ipv4_addr_array[index] = atoi(token);
        token = strtok(NULL,".");
        #if IPV4_LOG
        printf("%u%c",address->ipv4_addr_array[index],(index!=0)? IPV4_STRING_SEPARATOR:'\0'); 
        #endif
        index--;
    }
    #if IPV4_LOG
    printf("\n");
    #endif
}

/**
 * 
 * @param address
 * @return 
 */
char *string_fromIPV4(IPV4_address_t *address){
    static char retString[20];
    #if IPV4_LOG
    printf("Make IPv4 string from IPv4: ");
    #endif
    sprintf(retString,"%u%c%u%c%u%c%u",address->ipv4_bytes.b3,IPV4_STRING_SEPARATOR,address->ipv4_bytes.b2,IPV4_STRING_SEPARATOR,address->ipv4_bytes.b1,IPV4_STRING_SEPARATOR,address->ipv4_bytes.b0);
    #if IPV4_LOG
    printf("%s\n",retString);
    #endif
    return retString;
}

/**
 * 
 * @param ip_int
 * @return 
 */
char *IPV4_string_fromInt(uint32_t ip_int){
    static char retString[20];
    #if IPV4_LOG
    printf("Make IPv4 string from int: ");
    #endif
    sprintf(retString,"%u%c%u%c%u%c%u",(uint8_t)((ip_int & 0xFF000000UL)>>24),IPV4_STRING_SEPARATOR,(uint8_t)((ip_int & 0xFF0000UL)>>16),IPV4_STRING_SEPARATOR,(uint8_t)((ip_int & 0xFF00UL)>>8),IPV4_STRING_SEPARATOR,(uint8_t)((ip_int) & 0xFFUL),IPV4_STRING_SEPARATOR);
    #if IPV4_LOG
    printf("%s\n",retString);
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
uint32_t IPV4_int_fromIPv4(IPV4_address_t *address){
    return address->ipv4_word;
}

/**
 * @param *src
 * @param *dst
 */
void IPV4_copy(IPV4_address_t *dest, IPV4_address_t *src ){
    dest->ipv4_word = src->ipv4_word;
}

/**
 * @param *a1
 * @param *a2
 * @return 
 */
bool IPV4_compare(IPV4_address_t *a1, IPV4_address_t *a2 ){
    return a1->ipv4_word == a2->ipv4_word;
}