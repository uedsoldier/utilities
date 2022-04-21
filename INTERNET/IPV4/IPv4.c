#include "IPv4.h"

/**
 * @brief 
 * 
 * @param address 
 * @param bytes 
 * @return IPV4_error_t 
 */
IPV4_error_t IPV4_fromArray(IPV4_address_t *address, uint8_t *bytes){
    #if IPV4_LOG
    printf("IPv4 from array: ");
    #endif
    uint8_t j = 0;
    for(int8_t i = IPV4_BYTE_COUNT-1; i != -1; i--){
        address->ipv4_addr_array[j++] = bytes[i];
        #if IPV4_LOG
        printf("%u%c",address->ipv4_addr_array[i],(i!=IPV4_BYTE_COUNT-1)? IPV4_STRING_SEPARATOR:' '); 
        #endif
    }
    #if IPV4_LOG
    printf("\n");
    #endif
    return IPV4_ADDRESS_OK;
    
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
        printf("%x%c",bytes[i],(i!=IPV4_BYTE_COUNT-1)? IPV4_STRING_SEPARATOR:' '); 
        #endif
    }
    #if IPV4_LOG
    printf("\n");
    #endif
}

/**
 * @brief 
 * 
 * @param address 
 * @param string 
 * @return IPV4_error_t 
 */
IPV4_error_t IPV4_fromString(IPV4_address_t *address, const char *string){
    char _str[IPV4_MAX_SIZE];                              // Arreglo de almacenamiento de datos
    memset(_str,0,sizeof(_str));                    
    memcpy(_str,string,strlen(string));
    if(_str == NULL){
        return IPV4_NULL_STRING;
    }
    char *ptr;
    uint8_t index = 3, dots = 0;
    uint16_t num;
    ptr = strtok(_str,".");
    if(ptr == NULL){
        return IPV4_NULL_TOKEN;
    }
    while(ptr != NULL){
        if(!string_validate_int(ptr)){
            return IPV4_NaN;
        }
        num = atoi(ptr);
        if(num >= 0 && num <= 255){
            ptr = strtok(NULL,".");
            if(ptr != NULL){
                dots++;
            } 
            address->ipv4_addr_array[index] = (uint8_t)num;
            index --;
            
        } else{
            return IPV4_INVALID_NUMBER;
        }
    }
    if(dots != 3){
        return IPV4_INVALID_ADDRESS;
    }
    return IPV4_ADDRESS_OK;
}

/**
 * 
 * @param address
 * @return 
 */
char *string_fromIPV4(IPV4_address_t *address){
    static char retString[IPV4_MAX_SIZE];
    sprintf(retString,"%u%c%u%c%u%c%u",address->ipv4_bytes.b3,IPV4_STRING_SEPARATOR,address->ipv4_bytes.b2,IPV4_STRING_SEPARATOR,address->ipv4_bytes.b1,IPV4_STRING_SEPARATOR,address->ipv4_bytes.b0);
    return retString;
}

/**
 * 
 * @param ip_int
 * @return 
 */
char *IPV4_string_fromInt(uint32_t ip_int){
    static char retString[IPV4_MAX_SIZE];
    sprintf(retString,"%u%c%u%c%u%c%u",(uint8_t)((ip_int & 0xFF000000UL)>>24),IPV4_STRING_SEPARATOR,(uint8_t)((ip_int & 0xFF0000UL)>>16),IPV4_STRING_SEPARATOR,(uint8_t)((ip_int & 0xFF00UL)>>8),IPV4_STRING_SEPARATOR,(uint8_t)((ip_int) & 0xFFUL),IPV4_STRING_SEPARATOR);
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