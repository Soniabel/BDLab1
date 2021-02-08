#ifndef DB_STRUCT_H
#define DB_STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int indexAdress;
    char model[4];
    char color[4];
    int first_testDrive;
}Car;

typedef struct {
    int next_testDrive;
    int physicalExist;
    int selfAdress;
    char time[10];
    char customerFullName[20];
}TestDrive;

typedef struct
{
    int pos;
    int key;
    int physicalExist;

}Index;

#define I_SIZE  sizeof(Index)
#define C_SIZE  sizeof(Car)
#define T_SIZE  sizeof(TestDrive)

#endif //DB_STRUCT_H
