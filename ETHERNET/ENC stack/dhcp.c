#include "dhcp.h"
#include "utilities/util.h"

int16_t beginWithDHCP(Dhcp_t *dhcp,MAC_address_t *mac, uint32_t timeout, uint32_t responseTimeout) {
    if(responseTimeout == NULL) {
        responseTimeout = 4000;
    }
    if(timeout == NULL) {
        timeout = 60000;
    }
    dhcp->_LeaseTime = 0;
    dhcp->_T1=0;
    dhcp->_T2=0;
    dhcp->_lastCheck=0;
    dhcp->_timeout = timeout;
    dhcp->_responseTimeout = responseTimeout;

    // zero out MacAddr
    memset(&(dhcp->_MacAddr), 0, sizeof(dhcp->_MacAddr)); 
    reset_lease(dhcp);
    memcpy(&(dhcp->_MacAddr), mac, sizeof(mac) );
    dhcp->_state = STATE_DHCP_START;
    return request_lease(&dhcp);
}

void reset_lease(Dhcp_t *dhcp){
  memset(&(dhcp->_LocalIp), 0, 4);
  memset(&(dhcp->_SubnetMask), 0, 4);
  memset(&(dhcp->_GatewayIp), 0, 4);
  memset(&(dhcp->_DhcpServerIp), 0, 4);
  memset(&(dhcp->_DnsServerIp), 0, 4);
}

//return:0 on error, 1 if request is sent and response is received
int16_t request_lease(Dhcp_t *dhcp){
    
    uint8_t messageType = 0;

    // Pick an initial transaction ID
    dhcp->_TransactionId = random(1UL, 2000UL);
    dhcp->_InitialTransactionId = dhcp->_TransactionId;

    UdpSocket.stop();
    if (UdpSocket.begin(DHCP_CLIENT_PORT) == 0)
    {
      // Couldn't get a socket
      return 0;
    }
    
    presend(dhcp);
    
    uint16_t result = 0;
    
    unsigned long startTime = millis();
    
    while(dhcp->_state != STATE_DHCP_LEASED) {
        if(dhcp->_state == STATE_DHCP_START) {
            dhcp->_TransactionId++;
            
            send_MESSAGE(DHCP_DISCOVER, ((millis() - startTime) / 1000));
            dhcp->_state = STATE_DHCP_DISCOVER;
        }
        else if(dhcp->_state == STATE_DHCP_REREQUEST){
            dhcp->_TransactionId++;
            send_MESSAGE(DHCP_REQUEST, ((millis() - startTime)/1000));
            dhcp->_state = STATE_DHCP_REQUEST;
        }
        else if(dhcp->_state == STATE_DHCP_DISCOVER) {
            uint32_t respId;
            messageType = parseDHCPResponse(dhcp,dhcp->_responseTimeout, respId);
            if(messageType == DHCP_OFFER) {
                // We'll use the transaction ID that the offer came with,
                // rather than the one we were up to
                dhcp->_TransactionId = respId;
                send_MESSAGE(DHCP_REQUEST, ((millis() - startTime) / 1000));
                dhcp->_state = STATE_DHCP_REQUEST;
            }
        }
        else if(dhcp->_state == STATE_DHCP_REQUEST) {
            uint32_t respId;
            messageType = parseDHCPResponse(dhcp,dhcp->_responseTimeout, respId);
            if(messageType == DHCP_ACK) {
                dhcp->_state = STATE_DHCP_LEASED;
                result = 1;
                //use default lease time if we didn't get it
                if(dhcp->_LeaseTime == 0){
                    dhcp->_LeaseTime = DEFAULT_LEASE;
                }
                //calculate T1 & T2 if we didn't get it
                if(dhcp->_T1 == 0){
                    //T1 should be 50% of LeaseTime
                    dhcp->_T1 = dhcp->_LeaseTime >> 1;
                }
                if(dhcp->_T2 == 0){
                    //T2 should be 87.5% (7/8ths) of LeaseTime
                    dhcp->_T2 = dhcp->_T1 << 1;
                }
                dhcp->_renewInSec = dhcp->_T1;
                dhcp->_rebindInSec = dhcp->_T2;
            }
            else if(messageType == DHCP_NAK)
                dhcp->_state = STATE_DHCP_START;
        }
        
        if(messageType == 0xFF) {
            messageType = 0;
            dhcp->_state = STATE_DHCP_START;
        }
        
        if(result != 1 && ((millis() - startTime) > dhcp->_timeout))
            break;
    }
    
    // We're done with the socket now
    UdpSocket.stop();
    dhcp->_TransactionId++;

    return result;
}

void presend_DHCP(Dhcp_t *dhcp) {
}

void send_DHCP_MESSAGE(Dhcp_t *dhcp,uint8_t messageType, uint16_t secondsElapsed) {
    uint8_t buffer[32];
    memset(buffer, 0, 32);
    IP_address dest_addr; // Broadcast address
    dest_addr.ipv4_word = 0xFFFFFFFF; 

    if (-1 == UdpSocket.beginPacket(dest_addr, DHCP_SERVER_PORT)) {
        // FIXME Need to return errors
        return;
    }

    buffer[0] = DHCP_BOOTREQUEST;   // op
    buffer[1] = DHCP_HTYPE10MB;     // htype
    buffer[2] = DHCP_HLENETHERNET;  // hlen
    buffer[3] = DHCP_HOPS;          // hops

    // xid
    uint32_t xid = htonl(dhcp->_TransactionId);
    memcpy(buffer + 4, &(xid), 4);

    // 8, 9 - seconds elapsed
    buffer[8] = ((secondsElapsed & 0xFF00) >> 8);
    buffer[9] = (secondsElapsed & 0x00FF);

    // flags - the only one flag DHCP uses is
    // to request the server response as broadcast, not unicast
//    unsigned short flags = htons(DHCP_FLAGSBROADCAST);
//    memcpy(buffer + 10, &(flags), 2);

    // ciaddr: already zeroed
    // yiaddr: already zeroed
    // siaddr: already zeroed
    // giaddr: already zeroed

    //put data in W5100 transmit buffer
    UdpSocket.write(buffer, 28);

    memset(buffer, 0, 32); // clear local buffer

    memcpy(buffer, &(dhcp->_MacAddr), 6); // chaddr

    //put data in W5100 transmit buffer
    UdpSocket.write(buffer, 16);

    memset(buffer, 0, 32); // clear local buffer

    // leave zeroed out for sname && file
    // put in W5100 transmit buffer x 6 (192 bytes)
  
    for(uint8_t i = 0; i != 6; i++) {
        UdpSocket.write(buffer, 32);
    }
  
    // OPT - Magic Cookie
    buffer[0] = (uint8_t)((MAGIC_COOKIE >> 24) & 0xFF);
    buffer[1] = (uint8_t)((MAGIC_COOKIE >> 16) & 0xFF);
    buffer[2] = (uint8_t)((MAGIC_COOKIE >> 8) & 0xFF);
    buffer[3] = (uint8_t)(MAGIC_COOKIE & 0xFF);

    // OPT - message type
    buffer[4] = dhcpMessageType;
    buffer[5] = 0x01;
    buffer[6] = messageType; //DHCP_REQUEST;

    // OPT - client identifier
    buffer[7] = dhcpClientIdentifier;
    buffer[8] = 0x07;
    buffer[9] = 0x01;
    memcpy(buffer + 10, &(dhcp->_MacAddr), 6);

    // OPT - host name
    buffer[16] = hostName;
    buffer[17] = strlen(HOST_NAME) + 6; // length of hostname + last 3 bytes of mac address
    strcpy((char*)&(buffer[18]), HOST_NAME);

    printByte(dhcp,(char*)&(buffer[24]), dhcp->_MacAddr.MAC_array[3]);
    printByte(dhcp,(char*)&(buffer[26]), dhcp->_MacAddr.MAC_array[4]);
    printByte(dhcp,(char*)&(buffer[28]), dhcp->_MacAddr.MAC_array[5]);

    //put data in W5100 transmit buffer
    UdpSocket.write(buffer, 30);

    if(messageType == DHCP_REQUEST)
    {
        buffer[0] = dhcpRequestedIPaddr;
        buffer[1] = 0x04;
        buffer[2] = dhcp->_LocalIp.ipv4_addr_array[0];
        buffer[3] = dhcp->_LocalIp.ipv4_addr_array[1];
        buffer[4] = dhcp->_LocalIp.ipv4_addr_array[2];
        buffer[5] = dhcp->_LocalIp.ipv4_addr_array[3];

        buffer[6] = dhcpServerIdentifier;
        buffer[7] = 0x04;
        buffer[8] = dhcp->_DhcpServerIp.ipv4_addr_array[0];
        buffer[9] = dhcp->_DhcpServerIp.ipv4_addr_array[1];
        buffer[10] = dhcp->_DhcpServerIp.ipv4_addr_array[2];
        buffer[11] = dhcp->_DhcpServerIp.ipv4_addr_array[3];

        //put data in W5100 transmit buffer
        UdpSocket.write(buffer, 12);
    }
    
    buffer[0] = dhcpParamRequest;
    buffer[1] = 0x06;
    buffer[2] = subnetMask;
    buffer[3] = routersOnSubnet;
    buffer[4] = dns;
    buffer[5] = domainName;
    buffer[6] = dhcpT1value;
    buffer[7] = dhcpT2value;
    buffer[8] = endOption;
    
    //put data in W5100 transmit buffer
    UdpSocket.write(buffer, 9);

    UdpSocket.endPacket();
}

uint8_t parseDHCPResponse(Dhcp_t *dhcp, uint32_t responseTimeout, uint32_t *transactionId)
{
    uint8_t type = 0;
    uint8_t opt_len = 0;
     
    unsigned long startTime = millis();

    while(UdpSocket.parsePacket() <= 0)
    {
        if((millis() - startTime) > responseTimeout)
        {
            return 255;
        }
        delay(50);
    }
    // start reading in the packet
    RIP_MSG_FIXED fixedMsg;
    UdpSocket.read((uint8_t*)&fixedMsg, sizeof(RIP_MSG_FIXED));
  
    if(fixedMsg.op == DHCP_BOOTREPLY && UdpSocket.remotePort() == DHCP_SERVER_PORT) {
        *transactionId = ntohl(fixedMsg.xid);
        if(memcmp(fixedMsg.chaddr, &(dhcp->_MacAddr), 6) != 0 || (transactionId < dhcp->_InitialTransactionId) || (*transactionId > dhcp->_TransactionId))
        {
            // Need to read the rest of the packet here regardless
            UdpSocket.discardReceived();
            return 0;
        }

        memcpy(&(dhcp->_LocalIp), fixedMsg.yiaddr, 4);

        // Skip to the option part
        // Doing this a byte at a time so we don't have to put a big buffer
        // on the stack (as we don't have lots of memory lying around)
        for (int i =0; i != (240 - (uint8_t)sizeof(RIP_MSG_FIXED)); i++) {
            UdpSocket.read(); // we don't care about the returned byte
        }

        while (UdpSocket.available() > 0) {
            switch (UdpSocket.read()) {
                case endOption :
                    break;
                    
                case padOption :
                    break;
                
                case dhcpMessageType :
                    opt_len = UdpSocket.read();
                    type = UdpSocket.read();
                    break;
                
                case subnetMask :
                    opt_len = UdpSocket.read();
                    UdpSocket.read(dhcp->_SubnetMask, 4);
                    break;
                
                case routersOnSubnet :
                    opt_len = UdpSocket.read();
                    UdpSocket.read(dhcp->_GatewayIp, 4);
                    for (int i = 0; i < opt_len-4; i++)
                    {
                        UdpSocket.read();
                    }
                    break;
                
                case dns :
                    opt_len = UdpSocket.read();
                    UdpSocket.read(dhcp->_DnsServerIp, 4);
                    for (int i = 0; i < opt_len-4; i++)
                    {
                        UdpSocket.read();
                    }
                    break;
                
                case dhcpServerIdentifier :
                    opt_len = UdpSocket.read();
                    if( dhcp->_DhcpServerIp.ipv4_addr_array[0] == 0 ||
                        dhcp->_DhcpServerIp.ipv4_word == UdpSocket.remoteIP() )
                    {
                        UdpSocket.read(dhcp->_DhcpServerIp, sizeof(dhcp->_DhcpServerIp));
                    }
                    else{
                        // Skip over the rest of this option
                        while (opt_len--){
                            UdpSocket.read();
                        }
                    }
                    break;

                case dhcpT1value : 
                    opt_len = UdpSocket.read();
                    UdpSocket.read((uint8_t*)&dhcp->_T1, sizeof(dhcp->_T1));
                    dhcp->_T1 = ntohl(dhcp->_T1);
                    break;

                case dhcpT2value : 
                    opt_len = UdpSocket.read();
                    UdpSocket.read((uint8_t*)&dhcp->_T2, sizeof(dhcp->_T2));
                    dhcp->_T2 = ntohl(dhcp->_T2);
                    break;

                case dhcpIPaddrLeaseTime :
                    opt_len = UdpSocket.read();
                    UdpSocket.read((uint8_t*)&dhcp->_LeaseTime, sizeof(dhcp->_LeaseTime));
                    dhcp->_LeaseTime = ntohl(dhcp->_LeaseTime);
                    dhcp->_renewInSec = dhcp->_LeaseTime;
                    break;

                default :
                    opt_len = UdpSocket.read();
                    // Skip over the rest of this option
                    while (opt_len--) {
                        UdpSocket.read();
                    }
                    break;
            }
        }
    }

    // Need to skip to end of the packet regardless here
    UdpSocket.discardReceived();

    return type;
}


/*
    returns:
    0/DHCP_CHECK_NONE: nothing happened
    1/DHCP_CHECK_RENEW_FAIL: renew failed
    2/DHCP_CHECK_RENEW_OK: renew success
    3/DHCP_CHECK_REBIND_FAIL: rebind fail
    4/DHCP_CHECK_REBIND_OK: rebind success
*/
int16_t checkLease(Dhcp_t *dhcp){
    //this uses a signed / unsigned trick to deal with millis overflow
    unsigned long now = millis();
    signed long snow = (long)now;
    int rc=DHCP_CHECK_NONE;
    if (dhcp->_lastCheck != 0){
        signed long factor;
        //calc how many ms past the timeout we are
        factor = snow - (long)dhcp->_secTimeout;
        //if on or passed the timeout, reduce the counters
        if ( factor >= 0 ){
            //next timeout should be now plus 1000 ms minus parts of second in factor
            dhcp->_secTimeout = snow + 1000 - factor % 1000;
            //how many seconds late are we, minimum 1
            factor = factor / 1000 +1;
            
            //reduce the counters by that mouch
            //if we can assume that the cycle time (factor) is fairly constant
            //and if the remainder is less than cycle time * 2 
            //do it early instead of late
            if(dhcp->_renewInSec < factor*2 )
                dhcp->_renewInSec = 0;
            else
                dhcp->_renewInSec -= factor;
            
            if(dhcp->_rebindInSec < factor*2 )
            dhcp->_rebindInSec = 0;
            else
                dhcp->_rebindInSec -= factor;
        }

        //if we have a lease but should renew, do it
        if (dhcp->_state == STATE_DHCP_LEASED && dhcp->_renewInSec <=0){
            dhcp->_state = STATE_DHCP_REREQUEST;
            rc = 1 + request_lease(dhcp);
        }

        //if we have a lease or is renewing but should bind, do it
        if( (dhcp->_state == STATE_DHCP_LEASED || dhcp->_state == STATE_DHCP_START) && dhcp->_rebindInSec <=0){
            //this should basically restart completely
            dhcp->_state = STATE_DHCP_START;
            reset_lease(dhcp);
            rc = 3 + request_lease(dhcp);
        }
    }
    else{
        dhcp->_secTimeout = snow + 1000;
    }

    dhcp->_lastCheck = now;
    return rc;
}

IP_address getLocalIp(Dhcp_t *dhcp) {
    return dhcp->_LocalIp;
}

IP_address getSubnetMask(Dhcp_t *dhcp) {
    return dhcp->_SubnetMask;
}

IP_address getGatewayIp(Dhcp_t *dhcp) {
    return dhcp->_GatewayIp;
}

IP_address getDhcpServerIp(Dhcp_t *dhcp) {
    return dhcp->_DhcpServerIp;
}

IP_address getDnsServerIp(Dhcp_t *dhcp) {
    return dhcp->_DnsServerIp;
}

void printByte(Dhcp_t *dhcp,char *buf, uint8_t n) {
    char *str = &buf[1];
    buf[0]='0';
    do {
    unsigned long m = n;
    n /= 16;
    char c = m - 16 * n;
    *str-- = c < 10 ? c + '0' : c + 'A' - 10;
    } while(n);
}