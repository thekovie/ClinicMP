#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CLINIC_H
#define CLINIC_H


#define MAX_STRING_SIZE 51
#define MAX_SYMPTOMS 20
#define MAX_IMPRESSION 20

#if defined(_WIN32) // If the system is Windows
    #include <conio.h> // For the pause function
    #include <windows.h> // For the sleep function
#else // If the system is not Windows
    #include <unistd.h> // For the pause function
#endif

typedef char String50[MAX_STRING_SIZE]; // String of 50 characters

/**
    filesExtracted is a struct that contains 2 integers.
    @property {int} symptomBool     1 if 'Symptoms.txt' has been extracted, 0 if it doesn't.
    @property {int} impressionBool  1 if 'Impressions.txt' has been extracted, 0 if it doesn't.
    Pre-condition: symptomBool and impressionBool must be initialized to 0.
 */
typedef struct filesExtracted 
{
    int symptomBool;
    int impressionBool;
} filesExtracted;

/**
    PairSymptom is a struct that contains a String50 question, a String50 symptom, and an int
    overallSymptomsAmt.

    @property {String50} question   The question that the user will be asked.
    @property {String50} symptom    The symptom that the user is experiencing.
    @property {int} overallSymptomsAmt  The amount of symptoms that the user has.
    Pre-condition: question and symptom must be initialized to empty strings.
 */
typedef struct pairSymptom
{
    String50 question;
    String50 symptom;
    int overallSymptomsAmt;
} pairSymptom;


/**
    PairImpression is a struct that contains a string, an integer, an array of integers, and another
    integer.
    
    @property {String50} impression     The name of the impression.
    @property {int} symptomsAmountPerImpression     The amount of symptoms per impression.
    @property {int} symptomsIndexPerImpression      The index of the symptoms per impression.
    @property {int} impressionsAmount   The amount of impressions in the array.
    Pre-condition: impression must be initialized to an empty string.
 */
typedef struct pairImpression
{
    String50 impression; 
    int symptomsAmountPerImpression;
    int symptomsIndexPerImpression[MAX_SYMPTOMS];
    int impressionsAmount;
} pairImpression;


/**
    PatientInformation is a struct that contains a String50, two ints, a char, two ints, an array of
    pairSymptom, and an array of pairImpression.
    
    @property {String50} name   The name of the patient.
    @property {int} patientno   The patient number.
    @property {int} age     age of the patient.
    @property {char} gender     M for male, F for female.
    @property {int} patientSympAmt  The amount of symptoms the patient has.
    @property {int} patientImpAmt   The amount of impressions the patient has.
    @property {pairSymptom} patientSymptoms     This is an array of structs that contain a symptom and a
                                                value.
    @property {pairImpression} patientImpressions   This is an array of pairImpression.
    Pre-condition: name, patientno, age
 */
typedef struct patientInformation
{
    String50 name;
    int patientno;
    int age;
    char gender;
    int patientSympAmt;
    int patientImpAmt;
    pairSymptom patientSymptoms[MAX_SYMPTOMS];
    pairImpression patientImpressions[MAX_IMPRESSION];
} patientInformation;

/*
    Functions
*/
void sleepDelay (int n);
void displayKey ();
char userType();
char doctorMenu();
void writeSymptoms(pairSymptom* masterListSymptom, FILE *fp_symptoms, int N);
void inputSymptoms(pairSymptom* masterListSymptom);
void printImpressions(pairImpression* masterListImpression, FILE *fp_impressions);
void assignSymptoms(pairImpression* masterListImpression, pairSymptom* masterListSymptom, int impressionIndex);
void inputImpression(pairImpression* masterListImpression, pairSymptom* masterListSymptom);
int isImpressionPresent(pairImpression* masterListImpression, char* impression);
void displaySymptoms(String50 impression, pairImpression* masterListImpression,pairSymptom* masterListSymptom);
void readImpressions(pairImpression* masterListImpression, FILE *fp_impressions);
int filesExists();
void readSymptoms(pairSymptom* masterListSymptom, FILE *fp_symptoms);
int ifExtracted(filesExtracted* isFilesExtracted);
void extractList(pairImpression* masterListImpression, pairSymptom* masterListSymptom, filesExtracted* isFilesExtracted);
void modifySymptoms(pairImpression* masterListImpression, pairSymptom* masterListSymptom);
void doctorChoice(char choice, pairSymptom* masterListSymptom, pairImpression* masterListImpression, filesExtracted* isFilesExtracted);
void printPatientInfo(patientInformation *patient);
void getPatientImpression(patientInformation *patient, pairSymptom *masterListSymptom, pairImpression *masterListImpression);
void getPatientSymptoms(patientInformation *patient, pairSymptom *masterListSymptom);
void emptyHistory(patientInformation *patient);
void getPatientInfo (patientInformation *patient, pairSymptom *masterListSymptom, pairImpression *masterListImpression);

#endif