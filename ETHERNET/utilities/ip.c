/**/
#include <string.h>
#include "../../INTERNET/IPV4/IPv4.h"
#include "ip.h"
#include "ipopt.h"

#if IP_CONF_IPV6
// TODO
#endif



/**
 * Definición de variables
*/
#if UIP_FIXEDADDR > 0
const uip_ipaddr_t uip_hostaddr = {HTONS((UIP_IPADDR0 << 8) | UIP_IPADDR1), HTONS((UIP_IPADDR2 << 8) | UIP_IPADDR3)};
const uip_ipaddr_t uip_draddr =
  {HTONS((UIP_DRIPADDR0 << 8) | UIP_DRIPADDR1),
   HTONS((UIP_DRIPADDR2 << 8) | UIP_DRIPADDR3)};
const uip_ipaddr_t uip_netmask =
  {HTONS((UIP_NETMASK0 << 8) | UIP_NETMASK1),
   HTONS((UIP_NETMASK2 << 8) | UIP_NETMASK3)};
#else
uip_ipaddr_t uip_hostaddr, uip_draddr, uip_netmask;
#endif /* UIP_FIXEDADDR */