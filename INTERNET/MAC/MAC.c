#include "MAC.h"

/**
 * 
 * @param address
 * @param bytes
 */
MAC_error_t MAC_fromArray(MAC_address_t *address, uint8_t *bytes){
    uint8_t j = 0;
    for(int8_t i = MAC_BYTE_COUNT-1; i != -1; i--){
        address->MAC_array[j++] = bytes[i];
        
    }
    return MAC_ADDRESS_OK;
}

/**
 * 
 * @param address
 * @param bytes
 */
void array_fromMAC(MAC_address_t *address, uint8_t *bytes){
    #ifdef MAC_LOG
    printf("\tArray from MAC: ");
    #endif
    for(uint8_t i = 0; i != MAC_BYTE_COUNT; i++){
        bytes[i] = address->MAC_array[i];
        #ifdef MAC_LOG
        printf("%02X%c",bytes[i],(i!=MAC_BYTE_COUNT-1)? ':':' '); 
        #endif
    }
    #ifdef MAC_LOG
    printf("\r\n");
    #endif
}

/**
 * @brief 
 * 
 * @param address 
 * @param string 
 * @return true 
 * @return false 
 */
MAC_error_t MAC_fromString(MAC_address_t *address, char *string){
    char _str[MAC_MAX_SIZE];                    // Arreglo de almacenamiento de datos
    memset(_str,0,sizeof(_str));            
    memcpy(_str,string,strlen(string));
    if(_str == NULL){
        return MAC_NULL_STRING;
    }
    char *ptr;
    uint8_t index = MAC_BYTE_COUNT-1, colons = 0;
    uint16_t num;
    ptr = strtok(_str,":");
    if(ptr == NULL){
        return MAC_NULL_TOKEN;
    }
    while(ptr != NULL){
        if(!string_validate_hex(ptr)){
            return MAC_NaN;
        }
        num = strtol(ptr,NULL,16);     //atoi(ptr);
        if(num >= 0 && num <= 255){
            ptr = strtok(NULL,":");
            if(ptr != NULL){
                colons++;
            } 
            address->MAC_array[index] = (uint8_t)num;
            index --;
            
        } else{
            return MAC_INVALID_NUMBER;
        }
    }
    if(colons != MAC_BYTE_COUNT-1){
        return MAC_INVALID_ADDRESS;
    }
    return MAC_ADDRESS_OK;
}

/**
 * 
 * @param address
 * @return 
 */
char *string_fromMAC(MAC_address_t *address, bool upper) {
    static char retString[MAC_MAX_SIZE];
    sprintf(retString,upper? "%02X:%02X:%02X:%02X:%02X:%02X":"%02x:%02x:%02x:%02x:%02x:%02x",address->MAC_bytes.b5,address->MAC_bytes.b4,address->MAC_bytes.b3,address->MAC_bytes.b2,address->MAC_bytes.b1,address->MAC_bytes.b0);
    return retString;
}