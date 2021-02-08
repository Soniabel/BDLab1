#ifndef DB_CAR_H
#define DB_CAR_H
#include "struct.h"
Car GetCarByKEY(int key);

void AddDeletedCar(int id);

void findResourseAdr(int resourseCount, FILE* resourseZone, Car* record);

int insertCar(Car record);

int updateCar(Car* car);

int deleteCar(int key);




#endif //DB_CAR_H
