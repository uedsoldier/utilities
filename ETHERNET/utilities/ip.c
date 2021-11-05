/**/

#include "ip.h"

#if IP_CONF_IPV6
// TODO
#endif



/**
 * Definición de variables
*/
#if UIP_FIXEDADDR > 0
const IP_address_t ip_hostaddr = {HTONS((UIP_IPADDR0 << 8) | UIP_IPADDR1), HTONS((UIP_IPADDR2 << 8) | UIP_IPADDR3)};
const IP_address_t ip_draddr =
  {HTONS((UIP_DRIPADDR0 << 8) | UIP_DRIPADDR1),
   HTONS((UIP_DRIPADDR2 << 8) | UIP_DRIPADDR3)};
const IP_address_t ip_netmask =
  {HTONS((UIP_NETMASK0 << 8) | UIP_NETMASK1),
   HTONS((UIP_NETMASK2 << 8) | UIP_NETMASK3)};
#else
IP_address_t ip_hostaddr, ip_draddr, ip_netmask;
#endif /* UIP_FIXEDADDR */