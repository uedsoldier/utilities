#include "IPv6.h"

/**
 * @brief 
 * 
 * @param address 
 * @param words 
 * @return IPV6_error_t 
 */
IPV6_error_t IPV6_fromArray(IPV6_address_t *address, uint16_t *words){
    uint8_t j = 0;
    for(int8_t i = IPV6_WORD_COUNT-1; i != -1; i--){
        address->ipv6_addr_array[j++] = words[i];
        
    }
    return IPV6_ADDRESS_OK;
    
}

/**
 * 
 * @param address
 * @param bytes
 */
void array_fromIPV6(IPV6_address_t *address, uint16_t *words){
    for(uint8_t i = 0; i != IPV6_WORD_COUNT; i++){
        words[i] = address->ipv6_addr_array[i];
    }
}

/**
 * @brief 
 * 
 * @param address 
 * @param string 
 * @return IPV6_error_t 
 */
IPV6_error_t IPV6_fromString(IPV6_address_t *address, const char *string){
    char _str[IPV6_MAX_SIZE];                    // Arreglo de almacenamiento de datos
    memset(_str,0,sizeof(_str));            
    memcpy(_str,string,strlen(string));
    if(_str == NULL){
        return IPV6_NULL_STRING;
    }
    char *ptr;
    uint8_t index = IPV6_WORD_COUNT-1, colons = 0;
    uint16_t num;
    ptr = strtok(_str,":");
    if(ptr == NULL){
        return IPV6_NULL_TOKEN;
    }
    while(ptr != NULL){
        if(!string_validate_hex(ptr)){
            return IPV6_NaN;
        }
        num = strtoul(ptr,NULL,16);     
        if(num >= 0x00 && num <= 0xFFFF){
            ptr = strtok(NULL,":");
            if(ptr != NULL){
                colons++;
            } 
            address->ipv6_addr_array[index] = (uint16_t)num;
            index --;
            
        } else{
            return IPV6_INVALID_NUMBER;
        }
    }
    if(colons != IPV6_WORD_COUNT-1){
        return IPV6_INVALID_ADDRESS;
    }
    return IPV6_ADDRESS_OK;
}

/**
 * @brief 
 * 
 * @param address 
 * @param upper 
 * @return char* 
 */
char *string_fromIPV6(IPV6_address_t *address, bool upper){
    static char retString[IPV6_MAX_SIZE];
    sprintf(retString, upper? "%X%c%X%c%X%c%X%c%X%c%X%c%X%c%X":"%x%c%x%c%x%c%x%c%x%c%x%c%x%c%x",address->ipv6_words.w7,IPV6_STRING_SEPARATOR,address->ipv6_words.w6,IPV6_STRING_SEPARATOR,address->ipv6_words.w5,IPV6_STRING_SEPARATOR,address->ipv6_words.w4,IPV6_STRING_SEPARATOR,address->ipv6_words.w3,IPV6_STRING_SEPARATOR,address->ipv6_words.w2,IPV6_STRING_SEPARATOR,address->ipv6_words.w1,IPV6_STRING_SEPARATOR,address->ipv6_words.w0);
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