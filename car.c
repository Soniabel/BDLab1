#include "car.h"
#include "struct.h"
#include "index.h"
#include "test_drive.h"


Car GetCarByKEY(int key) {
    Index ind = get_ind_by_key(key);
    FILE* f = fopen("M.f", "rb");
    Car res = { -1, "\0", "\0", -1 };
    fseek(f, ind.pos * C_SIZE, SEEK_SET);
    fread(&res, C_SIZE, 1, f);
    fclose(f);
    return res;
}

void AddDeletedCar(int id)
{
    FILE* resourseZone = fopen("ResourseAdresses.f", "rb");

    int resourseCount;
    //зчитуємо к-сть резервних зон
    fscanf(resourseZone, "%d", &resourseCount);

    int* delIds = malloc(resourseCount * sizeof(int));
    //зчитуємо адреси
    for (int i = 0; i < resourseCount; i++)
    {
        fscanf(resourseZone, "%d", delIds + i);
    }

    fclose(resourseZone);
    //наново виводимо у файл+ додаємо ще один
    resourseZone = fopen("ResourseAdresses.f", "wb");
    fprintf(resourseZone, "%d", resourseCount + 1);

    for (int i = 0; i < resourseCount; i++)
    {
        fprintf(resourseZone, " %d", delIds[i]);
    }

    fprintf(resourseZone, " %d", id);
    free(delIds);
    fclose(resourseZone);
}

void findResourseAdr(int resourseCount, FILE* resourseZone, Car* record)
{
    int* delIds = malloc(resourseCount * sizeof(int));
    //зчитуємо адреси
    for (int i = 0; i < resourseCount; i++)
    {
        fscanf(resourseZone, "%d", delIds + i);
    }

    record->indexAdress = delIds[0];

    fclose(resourseZone);
    //наново записуємо вже без тієї адреси
    fopen("ResourseAdresses.f", "wb");
    fprintf(resourseZone, "%d", resourseCount - 1);

    for (int i = 1; i < resourseCount; i++)
    {
        fprintf(resourseZone, " %d", delIds[i]);
    }

    free(delIds);
    fclose(resourseZone);
}

int insertCar(Car record)
{
    FILE* indexTable = fopen("M.ind", "a+b");
    FILE* database = fopen("M.f", "a+b");
    FILE* resourseZone = fopen("ResourseAdresses.f", "rb");
    Index index;
    int resourseCount;

    fscanf(resourseZone, "%d", &resourseCount);
    //якщо э видалені записи то викликаємо фунц і шукаємо адресу
    if (resourseCount)
    {
        findResourseAdr(resourseCount, resourseZone, &record);

        fclose(indexTable);
        fclose(database);

        indexTable = fopen("M.ind", "r+b");
        database = fopen("M.f", "r+b");

        fseek(indexTable, (record.indexAdress - 1) * I_SIZE, SEEK_SET);
        fread(&index, I_SIZE, 1, indexTable);
        fseek(database, index.pos, SEEK_SET);
    }
    else
    {
        long indexerSize = I_SIZE;

        fseek(indexTable, 0, SEEK_END);

        if (ftell(indexTable))
        {
            fseek(indexTable, -indexerSize, SEEK_END);
            fread(&index, I_SIZE, 1, indexTable);
            record.indexAdress = index.pos + 1;
        }
        else
        {
            record.indexAdress = 1;
        }
    }

    record.first_testDrive = -1;
    //record.slavesCount = 0;

    fwrite(&record, C_SIZE, 1, database);
    index.key = record.model;
    index.pos = record.indexAdress;
    index.physicalExist = 1;


    fseek(indexTable, (record.indexAdress) * I_SIZE, SEEK_SET);
    fwrite(&index, I_SIZE, 1, indexTable);
    fclose(indexTable);
    fclose(database);

    return 1;
}

int updateCar(Car* car)
{
    FILE* indexTable = fopen("M.ind", "r+b");
    FILE* database = fopen("M.f", "r+b");

    Index index;
    int id = car->indexAdress;
    fseek(indexTable, (id)*I_SIZE, SEEK_SET);
    fread(&index, I_SIZE, 1, indexTable);
    fseek(database, index.pos * C_SIZE, SEEK_SET);
    fwrite(car, C_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);

    return 1;
}

int deleteCar(int key)
{
    FILE* indexTable = fopen("M.ind", "r+b");

    if (!is_key(key))
    {
        return 0;
    }

    Car car = GetCarByKEY(key);
    Index index;
    // зчитуємо відп індекс і робимо логічне видалення
    fseek(indexTable, (car.indexAdress) * I_SIZE, SEEK_SET);
    fread(&index, I_SIZE, 1, indexTable);

    index.physicalExist = 0;

    fseek(indexTable, (car.indexAdress) * I_SIZE, SEEK_SET);

    fwrite(&index, I_SIZE, 1, indexTable);
    fclose(indexTable);

    AddDeletedCar(car.indexAdress);


    if (car.first_testDrive)
    {
        FILE* testDrives = fopen("Sl.f", "r+b");
        TestDrive td;

        fseek(testDrives, (car.first_testDrive - 1) * T_SIZE, SEEK_SET);

        do
        {
            fread(&td, T_SIZE, 1, testDrives);
            fclose(testDrives);
            deleteTestDrive(&car,td);
            testDrives = fopen("Sl.f", "r+b");
            fseek(testDrives, (td.next_testDrive - 1) * T_SIZE, SEEK_SET);
        } while (td.next_testDrive != -1);

        fclose(testDrives);
    }
    return 1;
}


