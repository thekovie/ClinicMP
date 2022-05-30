/**
 * Description:       This is a console-based program that can generate a list of clinical impressions 
 *                    along with other patient information in a summary in the form of history of present illness (HPI).
 * 
 * Programmed by:     John Kovie L. Niño
 *                    Reign Elaiza D. Larraquel
 * 
 * Last modified:     May 30, 2022
 * Version:           1.0-alpha
 * Acknowledgements:  - I thank https://unix.stackexchange.com/questions/293940/how-can-i-make-press-any-key-to-continue
 *                      and https://stackoverflow.com/questions/5725296/difference-between-sh-and-bash
 *                      for the help in implementing the pause code in some operating systems and other libraries for making
 *                      this project possible.         
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if defined(_WIN32) // If the system is Windows
    #include <conio.h> // For the pause function
    #include <windows.h> // For the sleep function
#else // If the system is not Windows
    #include <unistd.h> // For the pause function
#endif


#define MAX_STRING_SIZE 51
#define MAX_SYMPTOMS 20
#define MAX_IMPRESSION 20

typedef char String50[MAX_STRING_SIZE];

typedef struct pairSymptom
{
    String50 question;
    String50 symptom;
    int overallSymptomsAmt;
} pairSymptom;

typedef struct pairImpression
{
    pairSymptom symptoms[MAX_SYMPTOMS];
    String50 impression;
    int symptomsAmountPerImpression;
    int impressionsAmount;
} pairImpression;

typedef struct patientInformation
{
    String50 name;
    int patientno;
    int age;
    char gender;
} patientInformation;



/**
 * If the program is running on Windows, then call the Windows Sleep function. Otherwise, call the Unix
 * sleep function
 */
void sleepDelay (int n) {
    #if defined(_WIN32)
        Sleep(1000*n);
    #else
        sleep(1*n);
    #endif
}


/**
 * It waits for the user to press a key
 */
void displayKey () {
    printf("\n\nPress any key to continue...\n");
    #if defined(_WIN32) && !defined(UNIX) // If the system is Windows
        getch();
    #else // If the system is not Windows
        system("sh -c \"read -n 1 -sr\""); // Reads a single character from the standard input
    #endif
}

char userType() {
    char choice;

    do {
        printf("%s\n\n%s\n%s\n%s\n\n%s",
        "MENU #1 User Type",
        "D for Doctor",
        "P for Patient",
        "E for Exit",
        "Enter your choice: ");
        
        scanf(" %c", &choice);

        if (!(choice == 'D' || choice == 'd' || choice == 'P' || choice == 'p' || choice == 'E' || choice == 'e')) {
            printf("\n\nInvalid choice. Please try again.\n\n");
            sleepDelay(1);
        }
    } while (!(choice == 'D' || choice == 'd' || choice == 'P' || choice == 'p' || choice == 'E' || choice == 'e'));

    return choice;
}

char doctorMenu() {
    char choice;

    do {
        printf("%s\n\n%s\n%s\n%s\n%s\n%s\n\n%s",
            "MENU #2 Doctor",
            "C to Create a new list of symptoms and impressions",
            "U to Use the existing list of symptoms and impressions",
            "D to Display Symptoms",
            "M to Modify Symptoms",
            "E to Exit",
            "Enter your choice: ");

        scanf(" %c", &choice);

        if (!(choice == 'C' || choice == 'c' || choice == 'U' || choice == 'u' || choice == 'D' || choice == 'd' || choice == 'M' || choice == 'm' || choice == 'E' || choice == 'e')) {
            printf("\n\nInvalid choice. Please try again.\n\n");
            sleepDelay(1);
        }
    } while (!(choice == 'C' || choice == 'c' || choice == 'U' || choice == 'u' || choice == 'D' || choice == 'd' || choice == 'M' || choice == 'm' || choice == 'E' || choice == 'e'));
    
    return choice;
}

void writeSymptoms(pairSymptom* masterListSymptom, FILE *fp_symptoms, int N) {
    fp_symptoms = fopen("Symptoms.txt", "w");
    int counter;

    fprintf(fp_symptoms, "%d\n", N); // write the number of symptoms to the file
    for (counter = 0; counter < N; counter++) {
        fprintf(fp_symptoms, "%d\n", counter+1); // write the index of the symptom
        fprintf(fp_symptoms, "%s\n", masterListSymptom[counter].symptom); // write the symptom to the file
        fprintf(fp_symptoms, "%s\n", masterListSymptom[counter].question); // write the question to the file
    }

    fclose(fp_symptoms);
}

void inputSymptoms(pairSymptom* masterListSymptom, FILE *fp_symptoms, pairImpression* masterListImpression) {
    int counter;
    int noSymptoms;

    //I1: Ask for the number of symptoms (let’s say this is N) 
    do {
        printf("How many symptoms do you want to consider? ");
        scanf("%d", &noSymptoms);
        
        if (noSymptoms == 0 || noSymptoms > MAX_SYMPTOMS) {
            printf("\n\nInvalid number of symptoms. Please try again.\n\n");
            sleepDelay(1);
        }
    } while (noSymptoms == 0 || noSymptoms > MAX_SYMPTOMS);


    //I2: Enter the name of each symptom and its corresponding question
    for (counter = 0; counter < noSymptoms; counter++) {

        printf("Symptom # %d:\n", counter + 1); // Display the number of the symptom
        printf("What is the symptom?  ");
        scanf(" %[^\n]", masterListSymptom[counter].symptom); //Place scanf here for symptom
        
        printf("How do you want to ask the symptom?\n");
        scanf(" %[^\n]", masterListSymptom[counter].question); //Place scanf here for symptom question
        strcat(masterListSymptom[counter].question, " [Y/N]"); // add a [Y/N] to the question   
    }

    masterListSymptom->overallSymptomsAmt = noSymptoms;
    writeSymptoms(masterListSymptom, fp_symptoms, noSymptoms);
}


int findSymptomIndex(pairSymptom* masterListSymptom, pairImpression* masterListImpression, int symptomIndex) {
    int counter;

    for (counter = 0; counter < masterListImpression->symptomsAmountPerImpression; counter++) {
        if (strcmp(masterListSymptom[symptomIndex].symptom, masterListImpression->symptoms[counter].symptom) == 0) {
            return 1;
        }
    }

    return -1;
}


void printImpressions(pairImpression* masterListImpression, pairSymptom* masterListSymptom, FILE *fp_impressions) {
    int counter, counter2;
    fp_impressions = fopen("Impressions.txt", "w");

    fprintf(fp_impressions, "%d\n", masterListImpression->impressionsAmount); // write the number of impressions to the file

    for (counter = 0; counter < masterListImpression->impressionsAmount; counter++) {
        fprintf(fp_impressions, "%d\n", counter+1);
        fprintf(fp_impressions, "%s\n", masterListImpression[counter].impression);

        // print all index of symptoms under the impression
        for (counter2 = 0; counter2 < masterListImpression[counter].symptomsAmountPerImpression; counter2++) {
            if (findSymptomIndex(masterListSymptom, masterListImpression, counter) != -1) {
                fprintf(fp_impressions, "%d ", counter2+1); // index of the symptom per impression
            }
        }
        fprintf(fp_impressions, "\n");
    }

    fclose(fp_impressions);
} 

void inputImpression(pairImpression* masterListImpression, pairSymptom* masterListSymptom, FILE *fp_impressions) {
    int noImpressions = 0;
    int presentSymptoms;
    int counter1;
    int counter2;
    int symptomIndex;


    //I3: Ask for the number of impressions
    do {
        printf("How many impressions do you want to enter? ");
        scanf("%d", &noImpressions);
        
        if (noImpressions == 0 || noImpressions > MAX_IMPRESSION) {
            printf("\n\nInvalid number of impressions. Please try again.\n\n");
            sleepDelay(1);
        }
    } while (noImpressions == 0 || noImpressions > MAX_IMPRESSION);

    //I4: For I times, ask for each impression and list all the symptom names entered in I2
    for (counter1 = 0; counter1 < noImpressions; counter1++) {
        printf("Impression # %d:\n", counter1 + 1);
        printf("What is the illness?  ");
        scanf(" %[^\n]", masterListImpression[counter1].impression); //Place scanf here for illness
    
    
        printf("\nBelow is a list of symptoms.\n");
        //Print all listed in masterlist symptoms
        for (counter2 = 0; counter2 < masterListSymptom->overallSymptomsAmt; counter2++) {
            printf("%d. %s\n", counter2 + 1, masterListSymptom[counter2].symptom);
        }
        

        printf("How many of the symptoms above are present in a %s case?  ", masterListImpression[counter1].impression);
        scanf("%d", &presentSymptoms);
        
        // assigns no of symptoms to the current impression
        masterListImpression[counter1].symptomsAmountPerImpression = presentSymptoms;

        printf("Enter the corresponding number of each symptom:\n");

        for(counter2 = 0; counter2 < presentSymptoms; counter2++){
            scanf("%d", &symptomIndex); //Place scanf here for symptom
            symptomIndex -= 1;

            masterListImpression[counter1].symptoms[counter2] = masterListSymptom[symptomIndex];

        }
    }

    masterListImpression->impressionsAmount = noImpressions;
    printImpressions(masterListImpression, masterListSymptom, fp_impressions);
}

void displaySymptoms(FILE *fp_symptoms){ //change function type to the correct type, this should return a string

    printf("What is the impression?  ");
    //Place scanf here for impression

    //printf("\nSymptoms of %s are:\n", /*add string array*/);
}

void modifySymptoms(){
    //displaySymptoms(); //this should return a string
    
    //this could also be connected with inputImpression
}

void doctorChoice(char choice, pairSymptom* masterListSymptom, pairImpression* masterListImpression, FILE *fp_symptoms, FILE *fp_impressions) {
    switch(choice) {
        case 'C':
        case 'c':
            inputSymptoms(masterListSymptom, fp_symptoms, masterListImpression);
            inputImpression(masterListImpression, masterListSymptom, fp_impressions);
            break;
        case 'D':
        case 'd':
            displaySymptoms(fp_symptoms);
            break;
        case 'M':
        case 'm':
            modifySymptoms();
            break;
        case 'U':
        case 'u':
            // function to use existing list of symptoms and impressions
            break;
    }
}

void getPatientInfo (patientInformation *patient)
{
    printf("What is your name? ");
    scanf(" %[^\n]", patient->name);

    printf("What is your patient number? ");
    scanf("%d", &patient->patientno);

    printf("Enter your age: ");
    scanf("%d", &patient->age);

    printf("Gender (M/F): ");
    scanf("%c", &patient->gender);

}

void getSymptoms (pairSymptom *masterListSymptom, FILE *fp_symptoms)
{
    int question;
    int answer;
    int counter;
    fp_symptoms = fopen("Symptoms.txt", "r");

    fscanf(fp_symptoms, "%d", &question);

    for(counter = 0; counter < question; counter++)
    {
        printf("%s", masterListSymptom[counter].question);
        scanf("%d", &answer);
        
    }


    fclose(fp_symptoms);
}


int main() {
    pairSymptom masterListSymptom[MAX_SYMPTOMS];
    pairImpression masterListImpression[MAX_IMPRESSION];
    patientInformation patient;
    
    FILE *fp_symptoms;
    FILE *fp_impressions;

    

    char cMenuChoice = userType();
    
    while (cMenuChoice != 'E' && cMenuChoice != 'e') {
        switch (cMenuChoice) {
            case 'D':
            case 'd':
                doctorChoice(doctorMenu(), masterListSymptom, masterListImpression, fp_symptoms, fp_impressions);
                break;
            case 'P':
            case 'p':
                getPatientInfo(&patient);
                break;
        }

        cMenuChoice = userType();
    }
    
    

    return 0;
}

/*************************************************************************************************
    This is to certify that this project is our own work, based on our personal efforts in studying and
    applying the concepts learned. We have constructed the functions and their respective algorithms
    and corresponding code by ourselves. The program was run, tested, and debugged by our own efforts. We
    further certify that we have not copied in part or whole or otherwise plagiarized the work of other
    students and/or persons.
    
    John Kovie L. Niño, DLSU ID# 12109975
    Reign Elaiza D. Larraquel, DLSU ID# 12119768
*************************************************************************************************/