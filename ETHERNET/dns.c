#include "dns.h"

// ctor
void DNS_init(Dns *dns, const IP_address *aDNSServer){
    dns->iDNSServer.ipv4_word = aDNSServer->ipv4_word;
    dns->iRequestId = 0;
}

/** 
 * @brief Convert a numeric IP address string into a four-byte IP address.
 * @param aIPAddrString IP address to convert
 * @param aResult IPAddress structure to store the returned IP address
 * @result 1 if aIPAddrString was successfully converted to an IP address, else error code
*/
int16_t DNS_inet_aton(Dns *dns, const char *aIPAddrString, IP_address *aResult){
     // See if we've been given a valid IP address
    const char* p =aIPAddrString;
    while (*p && ( (*p == '.') || ((*p >= '0') && (*p <= '9')) )) {
        p++;
    }

    if (*p == '\0') {
        // It's looking promising, we haven't found any invalid characters
        p = aIPAddrString;
        int segment = 0;
        int segmentValue = 0;
        while (*p && (segment < 4)) {
            if (*p == '.') {
                // We've reached the end of a segment
                if (segmentValue > 255) {
                    // You can't have IP address segments that don't fit in a byte
                    return 0;
                }
                else {
                    aResult->ipv4_addr_array[segment] = (uint8_t)segmentValue;
                    segment++;
                    segmentValue = 0;
                }
            }
            else {
                // Next digit
                segmentValue = (segmentValue*10)+(*p - '0');
            }
            p++;
        }
        // We've reached the end of address, but there'll still be the last
        // segment to deal with
        if ((segmentValue > 255) || (segment > 3)) {
            // You can't have IP address segments that don't fit in a byte,
            // or more than four segments
            return 0;
        }
        else {
            aResult->ipv4_addr_array[segment] = (uint8_t)segmentValue;
            return 1;
        }
    }
    else {
        return 0;
    }
}

/** Resolve the given hostname to an IP address.
 * @param aHostname Name to be resolved
 * @param aResult IPAddress structure to store the returned IP address
 * @result 1 if aIPAddrString was successfully converted to an IP address, else error code
*/
int16_t DNS_getHostByName(Dns *dns, const char* aHostname, IP_address *aResult){
    uint8_t ret =0;

    // See if it's a numeric IP address
    if (inet_aton(dns, aHostname, aResult)) {
        // It is, our work here is done
        return 1;
    }

    // Check we've got a valid DNS server to use
    if (dns->iDNSServer.ipv4_word == IP_ADDRESS_NONE) {
        return INVALID_SERVER;
    }
	
    // Find a socket to use
    if (iUdp iUdp.begin(1024+(millis() & 0xF)) == 1)
    {
        // Try up to three times
        uint8_t retries = 0;
        while ((retries < 3) && (ret <= 0))
        {
            // Send DNS request
            ret = iUdp.beginPacket(dns->iDNSServer, DNS_PORT);
            if (ret != 0)
            {
                // Now output the request data
                ret = BuildRequest(dns, aHostname);
                if (ret != 0)
                {
                    // And finally send the request
                    ret = iUdp.endPacket();
                    if (ret != 0)
                    {
                        // Now wait for a response
                        int wait_retries = 0;
                        ret = TIMED_OUT;
                        while ((wait_retries < 3) && (ret == TIMED_OUT))
                        {
                            ret = ProcessResponse(dns, 5000, aResult);
                            wait_retries++;
                        }
                    }
                }
            }
            retries++;
        }

        // We're done with the socket now
        iUdp.stop();
    }

    return ret;
}

uint16_t DNS_buildRequest(Dns *dns, const char* aName){
    // Build header
    //                                    1  1  1  1  1  1
    //      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |                      ID                       |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |                    QDCOUNT                    |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |                    ANCOUNT                    |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |                    NSCOUNT                    |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //    |                    ARCOUNT                    |
    //    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // As we only support one request at a time at present, we can simplify
    // some of this header
    dns->iRequestId = millis(); // generate a random ID
    uint16_t twoByteBuffer;

    // FIXME We should also check that there's enough space available to write to, rather
    // FIXME than assume there's enough space (as the code does at present)
    iUdp.write((uint8_t*)&dns->iRequestId, sizeof(dns->iRequestId));

    twoByteBuffer = htons(QUERY_FLAG | OPCODE_STANDARD_QUERY | RECURSION_DESIRED_FLAG);
    iUdp.write((uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));

    twoByteBuffer = htons(1);  // One question record
    iUdp.write((uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));

    twoByteBuffer = 0;  // Zero answer records
    iUdp.write((uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));

    iUdp.write((uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));
    // and zero additional records
    iUdp.write((uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));

    // Build question
    const char* start =aName;
    const char* end =start;
    uint8_t len;
    // Run through the name being requested
    while (*end)
    {
        // Find out how long this section of the name is
        end = start;
        while (*end && (*end != '.') )
        {
            end++;
        }

        if (end-start > 0)
        {
            // Write out the size of this section
            len = end-start;
            iUdp.write(&len, sizeof(len));
            // And then write out the section
            iUdp.write((uint8_t*)start, end-start);
        }
        start = end+1;
    }

    // We've got to the end of the question name, so
    // terminate it with a zero-length section
    len = 0;
    iUdp.write(&len, sizeof(len));
    // Finally the type and class of question
    twoByteBuffer = htons(TYPE_A);
    iUdp.write((uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));

    twoByteBuffer = htons(CLASS_IN);  // Internet class of question
    iUdp.write((uint8_t*)&twoByteBuffer, sizeof(twoByteBuffer));
    // Success!  Everything buffered okay
    return 1;
}
uint16_t DNS_processResponse(Dns *dns, uint16_t aTimeout, IP_address *aAddress){

}

