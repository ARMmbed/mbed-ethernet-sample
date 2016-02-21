#ifndef __YOTTA_BUILD_INFO_H__
#define __YOTTA_BUILD_INFO_H__
// yotta build info, #include YOTTA_BUILD_INFO_HEADER to access
#define YOTTA_BUILD_YEAR 2016 // UTC year
#define YOTTA_BUILD_MONTH 2 // UTC month 1-12
#define YOTTA_BUILD_DAY 21 // UTC day 1-31
#define YOTTA_BUILD_HOUR 16 // UTC hour 0-24
#define YOTTA_BUILD_MINUTE 43 // UTC minute 0-59
#define YOTTA_BUILD_SECOND 25 // UTC second 0-61
#define YOTTA_BUILD_UUID b74242a8-0f15-4492-99ab-49a6071b47ee // unique random UUID for each build
#define YOTTA_BUILD_VCS_ID 66abd87aa29232257aaaadcea1884560e9421966 // git or mercurial hash
#define YOTTA_BUILD_VCS_CLEAN 0 // evaluates true if the version control system was clean, otherwise false
#endif // ndef __YOTTA_BUILD_INFO_H__
