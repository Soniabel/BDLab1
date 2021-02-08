#include "test_drive.h"
#include "struct.h"
#include "car.h"

TestDrive GetTestDriveByPOS(int pos) {
    FILE* f = fopen("Sl.f", "rb");
    TestDrive res;
    int test = T_SIZE;
    fseek(f, (pos - 1) * T_SIZE, SEEK_SET);
    fread(&res, T_SIZE, 1, f);
    fclose(f);
    return res;
}

TestDrive GetTestDriveByKey(int key, const char* name) {
    Car car = GetCarByKEY(key);
    TestDrive cur = GetTestDriveByPOS(car.first_testDrive);
    int flag = 1;
    while (strcmp(name, cur.time) != 0) {
        if (cur.next_testDrive > 0)
            cur = GetTestDriveByPOS(cur.next_testDrive);
        else {
            flag = 0;
            break;
        }
    }
    if (!flag)

        cur.time[0] = "\0";
    return cur;
}

void AddDeletedTestDrive(int id)
{
    FILE* resourseZone = fopen("TestDriveResourse.f", "rb");

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
    resourseZone = fopen("TestDriveResourse.f", "wb");
    fprintf(resourseZone, "%d", resourseCount + 1);

    for (int i = 0; i < resourseCount; i++)
    {
        fprintf(resourseZone, " %d", delIds[i]);
    }

    fprintf(resourseZone, " %d", id);
    free(delIds);
    fclose(resourseZone);
}

void findTestDriveAdr(int resourseCount, FILE* resourseZone, TestDrive* record)
{
    int* delIds = malloc(resourseCount * sizeof(int));
    //зчитуємо адреси
    for (int i = 0; i < resourseCount; i++)
    {
        fscanf(resourseZone, "%d", delIds + i);
    }

    record->next_testDrive = delIds[0];
    record->selfAdress = delIds[0];

    fclose(resourseZone);
    //наново записуємо вже без тієї адреси
    fopen("TestDriveResourse.f", "wb");
    fprintf(resourseZone, "%d", resourseCount - 1);

    for (int i = 1; i < resourseCount; i++)
    {
        fprintf(resourseZone, " %d", delIds[i]);
    }

    free(delIds);
    fclose(resourseZone);
}

int insertTestDrive(Car car, TestDrive td)
{
    td.physicalExist = 1;

    FILE* database = fopen("Sl.f", "a+b");
    FILE* testDriveZone = fopen("TestDriveResourse.f", "rb");

    int garbageCount;

    fscanf(testDriveZone, "%d", &garbageCount);

    if (garbageCount)
    {
        findTestDriveAdr(garbageCount, testDriveZone, &td);
        fclose(database);
        database = fopen("Sl.f", "r+b");
        fseek(database, (td.selfAdress - 1) * T_SIZE, SEEK_SET);
    }
    else
    {
        fseek(database, 0, SEEK_END);
        int dbSize = ftell(database);
        td.selfAdress = dbSize / T_SIZE + 1;
    }


    // Замовлень ще немає, пишемо адресу першої
    if (car.first_testDrive == -1)
    {
        car.first_testDrive = td.selfAdress;
        td.next_testDrive = -1;
    }
        // Замовлення вже є, оновлюємо адресу першої
    else
    {
        TestDrive previous;
        int temp = car.first_testDrive;
        car.first_testDrive = td.selfAdress;
        td.next_testDrive = temp;
    }
    fwrite(&td, T_SIZE, 1, database);

    fclose(database);
    updateCar(&car);

    return 1;
}

int updateTestDrive(TestDrive td)
{
    FILE* database = fopen("Sl.f", "r+b");

    fseek(database, (td.selfAdress - 1) * T_SIZE, SEEK_SET);
    fwrite(&td, T_SIZE, 1, database);
    fclose(database);

    return 1;
}

int deleteTestDrive(Car*car, TestDrive td)
{
    FILE* database = fopen("Sl.f", "r+b");
    TestDrive previous;

    fseek(database, (car->first_testDrive - 1) * T_SIZE, SEEK_SET);
    //шукаємо попередників
    do
    {
        fread(&previous, T_SIZE, 1, database);
        fseek(database, (previous.next_testDrive - 1) * T_SIZE, SEEK_SET);
    } while (previous.next_testDrive != td.selfAdress && td.selfAdress != car->first_testDrive);
    //якщо їх немає то дивимося чи є наслідники в тест-драйві
    if (td.selfAdress == car->first_testDrive)
    {
        if (td.selfAdress == td.next_testDrive)
        {
            car->first_testDrive = -1;
        }
        else
        {
            car->first_testDrive = td.next_testDrive;
        }
    }
    else
    { //якщо є попередники але у тест-драйва немає наслідників, то адресу встановлюємо на себе
        if (td.selfAdress == td.next_testDrive)
        {
            previous.next_testDrive = previous.selfAdress;
        }
        else
        { //в іншому випадку встановлюємо на наступники
            previous.next_testDrive = td.next_testDrive;
        }

        fseek(database, previous.selfAdress * T_SIZE, SEEK_SET);
        fwrite(&previous, T_SIZE, 1, database);
    }

    AddDeletedTestDrive(td.selfAdress);

    td.physicalExist = 0;
    fseek(database, (td.selfAdress - 1) * T_SIZE, SEEK_SET);
    fwrite(&td, T_SIZE, 1, database);
    fclose(database);
    updateCar(&car);

}
