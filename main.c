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

int main(void)
{
    return 0;
}
