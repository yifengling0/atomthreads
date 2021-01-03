#include "kassert.h"
#include "atom.h"
#include "syslog.h"

void assert_failed(const char* file, int line, const char *msg)
{
    CRITICAL_STORE;

    CRITICAL_START();
    
    Syslog(LOG_EMERG, "assert failed %s,%d,%s", file, line, msg);

    while(1);

    CRITICAL_END();
}