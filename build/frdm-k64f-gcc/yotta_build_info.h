#ifndef __YOTTA_BUILD_INFO_H__
#define __YOTTA_BUILD_INFO_H__
// yotta build info, #include YOTTA_BUILD_INFO_HEADER to access
#define YOTTA_BUILD_YEAR 2016 // UTC year
#define YOTTA_BUILD_MONTH 2 // UTC month 1-12
#define YOTTA_BUILD_DAY 18 // UTC day 1-31
#define YOTTA_BUILD_HOUR 22 // UTC hour 0-24
#define YOTTA_BUILD_MINUTE 8 // UTC minute 0-59
#define YOTTA_BUILD_SECOND 48 // UTC second 0-61
#define YOTTA_BUILD_UUID c83e9bf6-a684-45b9-a22e-86fa0777ec3d // unique random UUID for each build
#define YOTTA_BUILD_VCS_ID 2dc90c5f7f0625695246af90aa34ccaa1dbd91d2 // git or mercurial hash
#define YOTTA_BUILD_VCS_CLEAN 0 // evaluates true if the version control system was clean, otherwise false
#endif // ndef __YOTTA_BUILD_INFO_H__
