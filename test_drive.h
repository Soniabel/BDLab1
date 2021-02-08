#ifndef DB_TEST_DRIVE_H
#define DB_TEST_DRIVE_H
#include "struct.h"
TestDrive GetTestDriveByPOS(int pos);

TestDrive GetTestDriveByKey(int key, const char* name);

void AddDeletedTestDrive(int id);

void findTestDriveAdr(int resourseCount, FILE* resourseZone, TestDrive* record);

int insertTestDrive(Car car, TestDrive td);

int updateTestDrive(TestDrive td);

int deleteTestDrive(Car* car, TestDrive td);

#endif //DB_TEST_DRIVE_H
