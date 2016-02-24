#ifndef __YOTTA_BUILD_INFO_H__
#define __YOTTA_BUILD_INFO_H__
// yotta build info, #include YOTTA_BUILD_INFO_HEADER to access
#define YOTTA_BUILD_YEAR 2016 // UTC year
#define YOTTA_BUILD_MONTH 2 // UTC month 1-12
#define YOTTA_BUILD_DAY 23 // UTC day 1-31
#define YOTTA_BUILD_HOUR 23 // UTC hour 0-24
#define YOTTA_BUILD_MINUTE 44 // UTC minute 0-59
#define YOTTA_BUILD_SECOND 44 // UTC second 0-61
#define YOTTA_BUILD_UUID c20fa958-ba31-4f89-bee9-5fd048b2701b // unique random UUID for each build
#define YOTTA_BUILD_VCS_ID 5593070cbc165c17d34cf5d091113bc39f2222fa // git or mercurial hash
#define YOTTA_BUILD_VCS_CLEAN 1 // evaluates true if the version control system was clean, otherwise false
#endif // ndef __YOTTA_BUILD_INFO_H__
