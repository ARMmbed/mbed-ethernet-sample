#ifndef __YOTTA_BUILD_INFO_H__
#define __YOTTA_BUILD_INFO_H__
// yotta build info, #include YOTTA_BUILD_INFO_HEADER to access
#define YOTTA_BUILD_YEAR 2016 // UTC year
#define YOTTA_BUILD_MONTH 2 // UTC month 1-12
#define YOTTA_BUILD_DAY 21 // UTC day 1-31
#define YOTTA_BUILD_HOUR 16 // UTC hour 0-24
#define YOTTA_BUILD_MINUTE 0 // UTC minute 0-59
#define YOTTA_BUILD_SECOND 2 // UTC second 0-61
#define YOTTA_BUILD_UUID d30c0516-cdba-42a0-a8d0-4e5e8bb10663 // unique random UUID for each build
#define YOTTA_BUILD_VCS_ID 6762a03c17c1e591e28fdfd10aadb53e10020258 // git or mercurial hash
#define YOTTA_BUILD_VCS_CLEAN 0 // evaluates true if the version control system was clean, otherwise false
#endif // ndef __YOTTA_BUILD_INFO_H__
