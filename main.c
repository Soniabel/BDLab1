#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "car.h"
#include "test_drive.h"
#define I_SIZE  sizeof(Index)
#define C_SIZE  sizeof(Car)
#define T_SIZE  sizeof(TestDrive)

Car readCar()
{
    Car car = { -1,"","",-1 };
    int x;
    printf("Enter name: ");
    scanf("%d", &x);
    strcpy(car.model, x);
    char color[4];
    color[0] = '\0';
    printf("Enter color: ");
    scanf("%s", color);
    strcpy(car.color, color);

    return car;

}

TestDrive readTestDrive()
{
    TestDrive td = { -1,-1,-1,"","" };
    char name[10];
    name[0] = '\0';
    printf("Enter time ( work time: 9am-18pm ): ");
    scanf("%s", name);
    strcpy(td.time, name);
    char fullName[20];

    fullName[0] = '\0';

    printf("Enter customer full name: ");
    scanf("%s", fullName);

    strcpy(td.customerFullName, fullName);
    return td;

}

void printCar(Car c)
{
    printf("Car\'s model: %s\n", c.model);
    printf("Car\'s color: %s\n", c.color);
}

void printTestDrive(TestDrive td, Car c)
{
    printf("Car\'s model: %s\n", c.model);
    printf("Car\'s color: %s\n", c.color);
    printf("TestDrive time: %s\n", td.time);
    printf("Customer full name: %s\n", td.customerFullName);
    printf("-------------------\n");

}

void printAll()
{
    FILE* database = fopen("M.f", "r+b");
    FILE* testDrives = fopen("Sl.f", "r+b");
    Car car;
    int index = 0;
    do {
        fseek(database, index * C_SIZE, SEEK_SET);
        fread(&car, C_SIZE, 1, database);
        if (feof(database) != 0)
            break;
        TestDrive td;
        fseek(testDrives, (car.first_testDrive - 1) * T_SIZE, SEEK_SET);
        do
        {
            fread(&td, T_SIZE, 1, testDrives);
            printTestDrive(td, car);
            fseek(testDrives, (td.next_testDrive - 1) * T_SIZE, SEEK_SET);
        } while (td.next_testDrive != -1);
        ++index;
    } while (feof(database) == 0);
    fclose(database);
    fclose(testDrives);
}

void files_init() {
    FILE* c_ind, * c_fl, * p_fl;
    c_ind = fopen("M.ind", "wb");
    Index ind = { 0, 1 ,1 };
    fwrite(&ind, I_SIZE, 1, c_ind);
    ind = (Index){ 1,2,1 };
    fwrite(&ind, I_SIZE, 1, c_ind);
    ind = (Index){ 2,3,1 };
    fwrite(&ind, I_SIZE, 1, c_ind);
    ind = (Index){ 3,4,1 };
    fwrite(&ind, I_SIZE, 1, c_ind);

    fclose(c_ind);
    c_fl = fopen("M.f", "wb");
    Car cr = { 0, "Infinity QX50","white",1 };
    fwrite(&cr, C_SIZE, 1, c_fl);
    cr = (Car){ 1, "Infinity Q50","grey",4 };
    fwrite(&cr, C_SIZE, 1, c_fl);
    cr = (Car){ 2, "Infinity QX80","black",5 };
    fwrite(&cr, C_SIZE, 1, c_fl);
    cr = (Car){ 3, "Infinity Q60","red",7 };
    fwrite(&cr, C_SIZE, 1, c_fl);

    fclose(c_fl);

    p_fl = fopen("Sl.f", "wb");
    TestDrive info = { 2,1,1,"9am","denys" };
    fwrite(&info, T_SIZE, 1, p_fl);
    info = (TestDrive){ 3,1,2,"11am","marko" };
    fwrite(&info, T_SIZE, 1, p_fl);
    info = (TestDrive){ -1,1,3, "2pm","olena" };
    fwrite(&info, T_SIZE, 1, p_fl);
    info = (TestDrive){ -1,1,4,"4pm","tanya" };
    fwrite(&info, T_SIZE, 1, p_fl);
    info = (TestDrive){ 6,1,5,"6pm","vasyl" };
    fwrite(&info, T_SIZE, 1, p_fl);
    info = (TestDrive){ -1,1,6, "12am","nikita" };
    fwrite(&info, T_SIZE, 1, p_fl);
    info = (TestDrive){ -1,1,7, "1pm","fedor" };
    fwrite(&info, T_SIZE, 1, p_fl);

    fclose(p_fl);
}

int main()
{
    Car car;
    TestDrive td;
    files_init();
    while (1)
    {
        int choice;
        int id;
        char name[10];
        name[0] = '\0';

        printf("Choose option:\n1 - Insert Car  2 - Get Car 3 - Update Car 4 - Delete Car\n5 - Insert TestDrive 6 - Get TestDrive 7 - Update TestDrive 8 - Delete TestDrive\n9 - Exit\n10 - PrintALL\n");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                car = readCar();
                insertCar(car);
                break;

            case 2:
                printf("Enter ID: ");
                scanf("%d", &id);
                car = GetCarByKEY(id);
                printCar(car);
                break;

            case 3:
                printf("Enter ID: ");
                scanf("%d", &id);
                car = GetCarByKEY(id);
                printf("Enter color: ");
                char color[4];
                color[0] = '\0';
                scanf("%s", color);
                strcpy(car.color, color);
                updateCar(&car);
                printf("Updated successfully\n");
                break;

            case 4:
                printf("Enter ID: ");
                scanf("%d", &id);
                deleteCar(id);
                printf("Deleted successfully\n");
                break;

            case 5:
                printf("Enter car\'s ID: ");
                scanf("%d", &id);
                car = GetCarByKEY(id);

                td = readTestDrive();
                insertTestDrive(car, td);
                printf("Inserted successfully");
                break;

            case 6:
                printf("Enter car\'s ID: ");
                scanf("%d", &id);
                car = GetCarByKEY(id);
                printf("Enter time: ");
                scanf("%s", name);
                td = GetTestDriveByKey(car.model, name);
                printTestDrive(td, car);
                break;

            case 7:
                printf("Enter testDrive\'s ID: ");
                scanf("%d", &id);
                car = GetCarByKEY(id);
                printf("Enter time: ");
                scanf("%s", name);
                td = GetTestDriveByKey(car.model, name);
                char form[20];
                form[0] = '\0';
                printf("Enter customer full name: ");
                scanf("%s", form);
                strcpy(td.customerFullName, form);
                updateTestDrive(td);
                printf("Updated successfully\n");

                break;

            case 8:
                printf("Enter testDrives\'s ID: ");
                scanf("%d", &id);
                car = GetCarByKEY(id);
                printf("Enter time: ");
                scanf("%s", &name);
                td = GetTestDriveByKey(car.model, name);
                deleteTestDrive(&car, td);
                printf("Deleted successfully\n");

                break;

            case 9:
                return 0;
                break;

            case 10:
                printAll();
                break;
            default:
                printf("Invalid input, please try again\n");
        }

        printf("---------\n");
    }

    return 0;
}