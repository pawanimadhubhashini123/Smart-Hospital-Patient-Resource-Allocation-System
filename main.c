#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 100
#define WARDS 4
#define MAX_BEDS 20
#define SPECIALTIES 4

const char *specialtyNames[SPECIALTIES] = {"General Practice (OPD)","Paediatrics","Cardiology","Neurology"};

const double consultationFee[SPECIALTIES] = {1500.00,2500.00,4500.00,5000.00};

const int consultationTime[SPECIALTIES] = {15,20,30,30};

const int dailyPatientCap[SPECIALTIES] = {30,20,12,10};

const char *wardNames[WARDS] = {"General Ward","Paediatric Ward","Surgical Ward","ICU (Intensive Care Unit)"};

const double wardDailyRate[WARDS] = {3000.00,6000.00,12000.00,25000.00};

const int wardCapacity[WARDS] = {20,10,10,5};

int bedOccupancy[WARDS][MAX_BEDS] = {0};

char patientID[MAX_PATIENTS][15];
char patientName[MAX_PATIENTS][50];
int patientAge[MAX_PATIENTS];
int urgencyLevel[MAX_PATIENTS];
int selectedSpecialty[MAX_PATIENTS];
int admitted[MAX_PATIENTS];
int selectedWard[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];
int assignedBed[MAX_PATIENTS];

double baseFee[MAX_PATIENTS];
double surcharge[MAX_PATIENTS];
double wardCost[MAX_PATIENTS];
double grossTotal[MAX_PATIENTS];
double discount[MAX_PATIENTS];
double finalAmount[MAX_PATIENTS];
double waitingTime[MAX_PATIENTS];

int patientCount = 0;
int specialtyQueue[SPECIALTIES] = {0, 0, 0, 0};

int getInteger(char message[], int min, int max);

void registerPatient(void);

int main(void)
{
    int choice;

    do
    {
        printf("\n============================================\n");
        printf(" SMART HOSPITAL RESOURCE ALLOCATION SYSTEM\n");
        printf("============================================\n");
        printf("1. Register Patient\n");
        printf("2. Display Bed Occupancy\n");
        printf("3. Display Patients by Priority\n");
        printf("4. Performance Reports\n");
        printf("5. Exit\n");
        printf("============================================\n");

        choice = getInteger("Enter your choice: ", 1, 5);

        switch (choice)
        {
            case 1:
                registerPatient();
                break;

            case 2:
                break;

            case 3:
                break;

            case 4:
                break;

            case 5:
                printf("\nSystem closed successfully.\n");
                break;
        }

    } while (choice != 5);

    return 0;
}

int getInteger(char message[], int min, int max)
{
    int value;
    int result;

    while (1)
    {
        printf("%s", message);
        result = scanf("%d", &value);

        if (result == 1 && value >= min && value <= max)
        {
            while (getchar() != '\n');
            return value;
        }

        printf("Invalid input. Enter a value from %d to %d.\n", min, max);

        while (getchar() != '\n');
    }
}

