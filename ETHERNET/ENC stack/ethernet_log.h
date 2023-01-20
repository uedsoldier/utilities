/**
 * Macros de nivel de logging
 */
#define	ETH_LOG_NONE		-1	// Logging nothing 
#define	ETH_LOG_EMERG		0	// system is unusable 
#define	ETH_LOG_ALERT		1	// action must be taken immediately 
#define	ETH_LOG_CRIT		2	// critical conditions 
#define	ETH_LOG_ERR			3	// error conditions 
#define	ETH_LOG_WARNING		4	// warning conditions 
#define	ETH_LOG_NOTICE		5	// normal but significant condition 
#define	ETH_LOG_INFO		6	// informational 
#define	ETH_LOG_DEBUG		7	// debug-level messages 
#define	ETH_LOG_DEBUG_V1	8	// debug-verbose-level (v) messages 
#define	ETH_LOG_DEBUG_V2	9	// debug-verbose-level (vv) messages 
#define	ETH_LOG_DEBUG_V3	10	// debug-verbose-level (vvv) messages 

#warning "You can configure LogObject and ACTLOGLEVEL in 'utilities/logging.h'. Note that more verbosity implies more memory usage."
#define ETH_ACTLOGLEVEL     LOG_NONE
//#define ACTLOGLEVEL LOG_WARNING
//#define ACTLOGLEVEL LOG_INFO
//#define ACTLOGLEVEL LOG_DEBUG_V2