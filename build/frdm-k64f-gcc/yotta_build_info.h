#ifndef __YOTTA_BUILD_INFO_H__
#define __YOTTA_BUILD_INFO_H__
// yotta build info, #include YOTTA_BUILD_INFO_HEADER to access
#define YOTTA_BUILD_YEAR 2016 // UTC year
#define YOTTA_BUILD_MONTH 2 // UTC month 1-12
#define YOTTA_BUILD_DAY 21 // UTC day 1-31
#define YOTTA_BUILD_HOUR 5 // UTC hour 0-24
#define YOTTA_BUILD_MINUTE 2 // UTC minute 0-59
#define YOTTA_BUILD_SECOND 43 // UTC second 0-61
#define YOTTA_BUILD_UUID 1774e467-4f2c-4442-b62c-f0912c58f174 // unique random UUID for each build
#define YOTTA_BUILD_VCS_ID 223418f1de7ee9ed5d4474cb173d327f3bf20fe8 // git or mercurial hash
#define YOTTA_BUILD_VCS_CLEAN 0 // evaluates true if the version control system was clean, otherwise false
#endif // ndef __YOTTA_BUILD_INFO_H__
