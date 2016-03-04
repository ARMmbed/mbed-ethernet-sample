#ifndef __YOTTA_BUILD_INFO_H__
#define __YOTTA_BUILD_INFO_H__
// yotta build info, #include YOTTA_BUILD_INFO_HEADER to access
#define YOTTA_BUILD_YEAR 2016 // UTC year
#define YOTTA_BUILD_MONTH 3 // UTC month 1-12
#define YOTTA_BUILD_DAY 4 // UTC day 1-31
#define YOTTA_BUILD_HOUR 23 // UTC hour 0-24
#define YOTTA_BUILD_MINUTE 24 // UTC minute 0-59
#define YOTTA_BUILD_SECOND 20 // UTC second 0-61
#define YOTTA_BUILD_UUID 978b8761-37fb-41c6-a19c-d2ceef70fbbd // unique random UUID for each build
#define YOTTA_BUILD_VCS_ID 2104a30dae9a8fc68e0fd595b4b8d2df310fbbe7 // git or mercurial hash
#define YOTTA_BUILD_VCS_CLEAN 0 // evaluates true if the version control system was clean, otherwise false
#define YOTTA_BUILD_VCS_DESCRIPTION 2104a30 // git describe or mercurial equivalent
#endif // ndef __YOTTA_BUILD_INFO_H__
