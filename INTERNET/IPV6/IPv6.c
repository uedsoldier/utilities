#include "IPv6.h"

/**
 * 
 * @param address
 * @param bytes
 */
void IPV6_fromArray(IPV6_address_t *address, uint16_t *words){
    #if IPV6_LOG
    printf("IPv6 from array: ");
    #endif
    for(uint8_t i = 0; i != IPV6_WORD_COUNT; i++){
        address->ipv6_addr_array[i] = words[i];
        #if IPV6_LOG
        printf("%x%c",address->ipv6_addr_array[i],(i!=IPV6_WORD_COUNT-1)? IPV6_STRING_SEPARATOR:' '); 
        #endif
    }
    #if IPV6_LOG
    printf("\n");
    #endif
    
}

/**
 * 
 * @param address
 * @param bytes
 */
void array_fromIPV6(IPV6_address_t *address, uint16_t *words){
    #if IPV6_LOG
    printf("Array from IPv6: ");
    #endif
    for(uint8_t i = 0; i != IPV6_WORD_COUNT; i++){
        words[i] = address->ipv6_addr_array[i];
        #if IPV6_LOG
        printf("%x%c",words[i],(i!=IPV6_WORD_COUNT-1)? IPV6_STRING_SEPARATOR:' '); 
        #endif
    }
    #if IPV6_LOG
    printf("\n");
    #endif
}

/**
 * 
 * @param address
 * @param string
 */
void IPV6_fromString(IPV6_address_t *address, const char *string){
    char _str[40];
    memset(_str,0,sizeof(_str));
    strcpy(_str,string);
    #if IPV6_LOG
    printf("IPv6 from string %s\n",string);
    #endif
    char *token = strtok(_str,":");
    uint8_t index = IPV6_WORD_COUNT-1;
    while(token != NULL){
        address->ipv6_addr_array[index] = strtol(token,NULL,16);
        token = strtok(NULL,":");
        #if IPV6_LOG
        printf("%x%c",address->ipv6_addr_array[index],(index!=0)? IPV6_STRING_SEPARATOR:' '); 
        #endif
        index--;
    }
    #if IPV6_LOG
    printf("\n");
    #endif
}

/**
 * 
 * @param address
 * @return 
 */
char *string_fromIPV6(IPV6_address_t *address){
    static char retString[40];
    #if IPV6_LOG
    printf("Make IPv6 string from IPv6: ");
    #endif
    sprintf(retString,"%x%c%x%c%x%c%x%c%x%c%x%c%x%c%x",address->ipv6_words.w7,IPV6_STRING_SEPARATOR,address->ipv6_words.w6,IPV6_STRING_SEPARATOR,address->ipv6_words.w5,IPV6_STRING_SEPARATOR,address->ipv6_words.w4,IPV6_STRING_SEPARATOR,address->ipv6_words.w3,IPV6_STRING_SEPARATOR,address->ipv6_words.w2,IPV6_STRING_SEPARATOR,address->ipv6_words.w1,IPV6_STRING_SEPARATOR,address->ipv6_words.w0);
    #if IPV6_LOG
    printf("%s\n",retString);
    #endif
    return retString;
}

/**
 * 
 * @param ip_int
 * @return 
 */
char *IPV6_string_fromInt(uint32_t ip_int){
    static char retString[40];
    #if IPV6_LOG
    printf("Make IPv6 string from int: ");
    #endif
    sprintf(retString,"%x%c%x%c%x%c%x",(uint8_t)((ip_int & 0xFF000000UL)>>24),(uint8_t)((ip_int & 0xFF0000UL)>>16),(uint8_t)((ip_int & 0xFF00UL)>>8),(uint8_t)((ip_int) & 0xFFUL));
    #if IPV6_LOG
    printf("%s\n",retString);
    #endif
    return retString;
}

/**
 * 
 * @param mask
 * @return 
 */
bool IPV6_validMask(IPV6_address_t *subnetmask){
    uint32_t mask = subnetmask->ipv6_word;
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
uint32_t IPV6_int_fromIPV6(IPV6_address_t *address){
    return address->ipv6_word;
}

/**
 * @param *src
 * @param *dst
 */
void IPv6_copy(IPV6_address_t *dest, IPV6_address_t *src ){
    memcpy(dest,src,sizeof(IPV6_address_t));
}

/**
 * @param *a1
 * @param *a2
 * @return 
 */
bool IPv4_compare(IPV6_address_t *a1, IPV6_address_t *a2 ){
    return (memcmp(a1,a2,sizeof(IPV6_address_t)) == 0)? true:false;
}