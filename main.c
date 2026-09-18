#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
void displayPatientBill(int index);
void displayPatientsByPriority(void);
void displayReports(void);

void savePatientRecord(int index);

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
                displayPatientsByPriority();
                break;

            case 4:
                displayReports();
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
            int valid = 1;
            for (size_t i = 0; i < strlen(name); i++)
            {
                if (!isalpha((unsigned char)name[i]) && !isspace((unsigned char)name[i]))
                {
                    valid = 0;
                    break;
                }
            }

            if (valid)
            {
                return;
            }
            else
            {
                printf("Invalid input! Name should only contain letters and spaces.\n");
            }
        }
        else
        {
            printf("Patient name cannot be empty.\n");
        }
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

    printf("\nYes(1) - Admitted to Ward\n");
    printf("No(0) - Outpatient (OPD)\n");

    admitted[patientCount] = getInteger("Is the patient admitted to a ward? ", 0, 1);

    selectedWard[patientCount] = -1;
    assignedBed[patientCount] = -1;
    daysAdmitted[patientCount] = 0;

    if (admitted[patientCount] == 1)
    {
        displayWards();

        ward = getInteger("Select Ward ID: ", 1, 4);

        ward--;

        bed = findAvailableBed(ward);

        if (bed == -1)
        {
            printf("\nSelected ward is full.\n");
            printf("Patient registration cancelled.\n");

            specialtyQueue[specialty - 1]--;
            return;
        }

        selectedWard[patientCount] = ward;
        assignedBed[patientCount] = bed;

        daysAdmitted[patientCount] = getInteger("Enter Days Admitted (1-365): ", 1, 365);

        bedOccupancy[ward][bed] = 1;
    }

    baseFee[patientCount] = consultationFee[selectedSpecialty[patientCount]];

    surcharge[patientCount] = calculateSurcharge(baseFee[patientCount],urgencyLevel[patientCount]);

    if (admitted[patientCount] == 1)
    {
        wardCost[patientCount] = calculateWardCost(selectedWard[patientCount],daysAdmitted[patientCount]);
    }
    else
    {
        wardCost[patientCount] = 0.0;
    }

    grossTotal[patientCount] = baseFee[patientCount] + surcharge[patientCount] + wardCost[patientCount];

    discount[patientCount] = calculateDiscount(grossTotal[patientCount],patientAge[patientCount]);

    finalAmount[patientCount] = grossTotal[patientCount] - discount[patientCount];

    printf("\nPatient registered successfully.\n");

    displayPatientBill(patientCount);
    savePatientRecord(patientCount);

    patientCount++;
}

void displayPatientBill(int index)
{
    printf("\n==========================================================\n");
    printf("          SMART HOSPITAL ADMISSION & BILL\n");
    printf("==========================================================\n");

    printf("Patient ID            : %s\n", patientID[index]);
    printf("Patient Name          : %s\n", patientName[index]);
    printf("Age                   : %d Years",patientAge[index]);

    if (patientAge[index] < 5 || patientAge[index] > 65)
    {
        printf(" (15%% Subsidy Eligible)");
    }

    printf("\n");

    printf("Specialty             : %s\n", specialtyNames[selectedSpecialty[index]]);

    if (admitted[index] == 1)
    {
        printf("Assigned Ward         : %s (Bed #%02d)\n", wardNames[selectedWard[index]], assignedBed[index] + 1);
    }
    else
    {
        printf("Assigned Ward         : Outpatient (OPD)\n");
    }

    printf("Urgency Level         : Level %d ", urgencyLevel[index]);

    if (urgencyLevel[index] == 1)
        printf("(Normal)\n");
    else if (urgencyLevel[index] == 2)
        printf("(Urgent)\n");
    else
        printf("(Critical)\n");

    printf("---------------------------------------------------------\n");

    printf("Base Consultation Fee : LKR %.2f\n", baseFee[index]);

    if (urgencyLevel[index] == 1)
    {
        printf("Emergency Surcharge   : LKR %.2f (0%%)\n", surcharge[index]);
    }
    else if (urgencyLevel[index] == 2)
    {
        printf("Emergency Surcharge   : LKR %.2f (20%%)\n", surcharge[index]);
    }
    else
    {
        printf("Emergency Surcharge   : LKR %.2f (50%%)\n", surcharge[index]);
    }

    printf("Ward Stay Cost        : LKR %.2f\n",wardCost[index]);
    printf("---------------------------------------------------------\n");
    printf("Gross Total Bill      : LKR %.2f\n",grossTotal[index]);
    printf("Age Subsidy Discount  : LKR - %.2f\n", discount[index]);
    printf("---------------------------------------------------------\n");
    printf("Final Payable Amount  : LKR %.2f\n", finalAmount[index]);
    printf("Estimated Waiting Time: %.2f mins", waitingTime[index]);

    if (waitingTime[index] == 0)
    {
        printf(" (Immediate Attention)");
    }

    printf("\n");
    printf("=========================================================\n");
}

void displayPatientsByPriority(void)
{
    int order[MAX_PATIENTS];
    int i, j, temp;

    if (patientCount == 0)
    {
        printf("\nNo patients registered.\n");
        return;
    }

    for (i = 0; i < patientCount; i++)
    {
        order[i] = i;
    }


    for (i = 0; i < patientCount - 1; i++)
    {
        int highest = i;

        for (j = i + 1; j < patientCount; j++)
        {
            if (urgencyLevel[order[j]] > urgencyLevel[order[highest]])
            {
                highest = j;
            }
        }

        temp = order[i];
        order[i] = order[highest];
        order[highest] = temp;
    }

    printf("\n=====================================================================\n");
    printf("              PATIENT PRIORITY LIST\n");
    printf("=====================================================================\n");
    printf("  %-5s %-10s %-20s %-15s %-22s\n","Rank", "ID", "Name", "Urgency level","Final Amount");

    for (i = 0; i < patientCount; i++)
    {
        int index = order[i];

        printf("  %-5d %-10s %-20s Level %-8d  LKR %.2f\n", i + 1, patientID[index], patientName[index], urgencyLevel[index],finalAmount[index]);
    }
}

void displayReports(void)
{
    int i, ward, bed;
    int normal = 0;
    int urgent = 0;
    int critical = 0;

    int occupied;
    double totalRevenue = 0.0;
    double totalDiscount = 0.0;
    double highestBill = 0.0;
    int highestPatient = -1;

    if (patientCount == 0)
    {
        printf("\nNo patient data available.\n");
        return;
    }

    for (i = 0; i < patientCount; i++)
    {
        if (urgencyLevel[i] == 1)
            normal++;
        else if (urgencyLevel[i] == 2)
            urgent++;
        else
            critical++;

        totalRevenue += finalAmount[i];
        totalDiscount += discount[i];

        if (finalAmount[i] > highestBill)
        {
            highestBill = finalAmount[i];
            highestPatient = i;
        }
    }

    printf("\n====================================================\n");
    printf("             PERFORMANCE REPORT & ANALYTICS \n");
    printf("====================================================\n");

    printf("Patient count by urgency level\n");
    printf("Leval 1 (Normal)   : %d\n", normal);
    printf("Leval 2 (Urgent)   : %d\n", urgent);
    printf("Leval 3 (Critical) : %d\n", critical);
    printf("Total Registered   : %d\n", patientCount);

    printf("\nRevenue & Discount Summary\n");
    printf("Total Revenue   : LKR %.2f\n", totalRevenue);
    printf("Total Discounts : LKR %.2f\n", totalDiscount);

    printf("\nBed Occupancy Percentage per ward\n");

    for (ward = 0; ward < WARDS; ward++)
    {
        occupied = 0;

        for (bed = 0; bed < wardCapacity[ward]; bed++)
        {
            if (bedOccupancy[ward][bed] == 1)
            {
                occupied++;
            }
        }

        printf("%-24s : %.2f%%\n", wardNames[ward],(occupied * 100.0) / wardCapacity[ward]);
    }

    if (highestPatient != -1)
    {
        printf("\nHighestPaying Patient\n");
        printf("Patient Name : %s\n",patientName[highestPatient]);
        printf("Final Bill   : LKR %.2f\n",finalAmount[highestPatient]);
    }

    printf("====================================================\n");
}

void savePatientRecord(int index)
{
    FILE *file;

    file = fopen("patient_records.txt", "a");

    if (file == NULL)
    {
        return;
    }

    fprintf(file,
            "%s | %s | Age: %d | Urgency: %d | Final Bill: %.2f\n",
            patientID[index],
            patientName[index],
            patientAge[index],
            urgencyLevel[index],
            finalAmount[index]);

    fclose(file);
}
