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

void displaySpecialties(void);
void displayWards(void);
void displayBeds(void);

int getInteger(char message[], int min, int max);
void getName(char name[]);

int findAvailableBed(int ward);

double calculateSurcharge(double fee, int urgency);
double calculateWardCost(int ward, int days);
double calculateDiscount(double gross, int age);

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
                displayBeds();
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

void getName(char name[])
{
    while (1)
    {
        printf("Enter Patient Name: ");
        fgets(name, 50, stdin);

        name[strcspn(name, "\n")] = '\0';

        if (strlen(name) > 0)
        {
          return;
        }
        printf("Patient name cannot be empty.\n");
    }
}

void displaySpecialties(void)
{
    int i;

    printf("\n------------------- DOCTOR SPECIALTIES & FEES ------------------------\n");
    printf("%-4s %-25s %-15s %-10s %-10s\n","ID", "Specialty", "Base Fee", "Time", "Daily Cap");
    for (i = 0; i < SPECIALTIES; i++)
    {
        printf("%-3d  %-25s LKR%8.2f %6d mins %5d\n", i + 1, specialtyNames[i], consultationFee[i], consultationTime[i], dailyPatientCap[i]);
    }
}

void displayWards(void)
{
    int i;

    printf("\n-------------------- HOSPITAL WARD INFORMATION --------------------\n");
    printf(" %-4s %-28s %-18s %-10s\n","ID", "Ward Name", "Daily Bed Rate", "Bed Capacity");

    for (i = 0; i < WARDS; i++)
    {
        printf(" %-4d %-28s LKR%11.2f %6d \n",i + 1, wardNames[i], wardDailyRate[i], wardCapacity[i]);
    }
}

int findAvailableBed(int ward)
{
    int bed;

    for (bed = 0; bed < wardCapacity[ward]; bed++)
    {
        if (bedOccupancy[ward][bed] == 0)
        {
            return bed;
        }
    }

    return -1;
}

void displayBeds(void)
{
    int ward;
    int bed;
    int available;

    printf("\n==============================================================\n");
    printf("                       BED OCCUPANCY MAP\n");
    printf("==============================================================\n");

    for (ward = 0; ward < WARDS; ward++)
    {
        available = 0;

        for (bed = 0; bed < wardCapacity[ward]; bed++)
        {
            if (bedOccupancy[ward][bed] == 0)
            {
                available++;
            }
        }

        printf("\n%s (Capacity: %d | Available: %d)\n", wardNames[ward], wardCapacity[ward], available);

        for (bed = 0; bed < wardCapacity[ward]; bed++)
        {
            printf("[%d-%02d]",bedOccupancy[ward][bed], bed + 1);

            if ((bed + 1) % 10 == 0)
            {
                printf("\n");
            }
        }

        if (wardCapacity[ward] % 10 != 0)
        {
            printf("\n");
        }
    }

    printf("\n==============================================================\n");
    printf("         [0] = Available   [1] = Occupied\n");
    printf("==============================================================\n");
}

double calculateSurcharge(double fee, int urgency)
{
    if (urgency == 1)
    {
        return 0.0;
    }
    else if (urgency == 2)
    {
        return fee * 0.20;
    }
    else
    {
        return fee * 0.50;
    }
}

double calculateWardCost(int ward, int days)
{
    if (days == 0)
    {
        return 0.0;
    }

    return wardDailyRate[ward] * days;
}

double calculateDiscount(double gross, int age)
{
    if (age < 5 || age > 65)
    {
        return gross * 0.15;
    }

    return 0.0;
}

void registerPatient(void)
{
    int specialty;
    int ward;
    int bed;

    if (patientCount >= MAX_PATIENTS)
    {
        printf("\nPatient registration limit reached.\n");
        return;
    }

    printf("\n=================================================\n");
    printf("          PATIENT REGISTRATION\n");
    printf("=================================================\n");

    sprintf(patientID[patientCount], "PAT-%04d", 1001 + patientCount);

    printf("Patient ID: %s\n", patientID[patientCount]);
    getName(patientName[patientCount]);

    patientAge[patientCount] = getInteger("Enter Patient Age (0-150): ", 0, 150);

    printf("\n1 = Normal\n");
    printf("2 = Urgent\n");
    printf("3 = Critical\n");
    urgencyLevel[patientCount] = getInteger("Enter Emergency Level: ", 1, 3);

    displaySpecialties();

    specialty = getInteger("Select Specialty ID: ", 1, 4);

    if (specialtyQueue[specialty - 1] >= dailyPatientCap[specialty - 1])
    {
        printf("\nSorry. Daily patient capacity for this specialty is full.\n");
        return;
    }

    selectedSpecialty[patientCount] = specialty - 1;

    waitingTime[patientCount] = specialtyQueue[specialty - 1] * consultationTime[specialty - 1];

    specialtyQueue[specialty - 1]++;

    patientCount++;
}
