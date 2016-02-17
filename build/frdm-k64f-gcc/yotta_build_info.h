#ifndef __YOTTA_BUILD_INFO_H__
#define __YOTTA_BUILD_INFO_H__
// yotta build info, #include YOTTA_BUILD_INFO_HEADER to access
#define YOTTA_BUILD_YEAR 2016 // UTC year
#define YOTTA_BUILD_MONTH 2 // UTC month 1-12
#define YOTTA_BUILD_DAY 17 // UTC day 1-31
#define YOTTA_BUILD_HOUR 23 // UTC hour 0-24
#define YOTTA_BUILD_MINUTE 4 // UTC minute 0-59
#define YOTTA_BUILD_SECOND 36 // UTC second 0-61
#define YOTTA_BUILD_UUID 5cdbab7e-6008-4e7a-ae04-b212a9265f7f // unique random UUID for each build
#define YOTTA_BUILD_VCS_ID e839646f0630d604b61ce0d4210d01a83fe6788c // git or mercurial hash
#define YOTTA_BUILD_VCS_CLEAN 0 // evaluates true if the version control system was clean, otherwise false
#endif // ndef __YOTTA_BUILD_INFO_H__
