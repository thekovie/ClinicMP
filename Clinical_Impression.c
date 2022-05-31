/**
 * Description:       This is a console-based program that can generate a list of clinical impressions 
 *                    along with other patient information in a summary in the form of history of present illness (HPI).
 * 
 * Programmed by:     John Kovie L. Niño
 *                    Reign Elaiza D. Larraquel
 * 
 * Last modified:     May 31, 2022
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
    int symptomsAmountPerImpression; // The amount of symptoms per impression
    int symptomsIndexPerImpression[MAX_SYMPTOMS]; // The index of the symptoms per impression
    int impressionsAmount;
} pairImpression;

typedef struct patientInformation
{
    String50 name;
    int patientno;
    int age;
    char gender;
} patientInformation;



/*
    If the program is running on Windows, then call the Windows Sleep function. Otherwise, call the Unix
    sleep function
*/
void sleepDelay (int n) {
    #if defined(_WIN32)
        Sleep(1000*n);
    #else
        sleep(1*n);
    #endif
}

/*
    It waits for the user to press a key
*/
void displayKey () {
    printf("\n\nPress any key to continue...\n");
    #if defined(_WIN32) && !defined(UNIX) // If the system is Windows
        getch();
    #else // If the system is not Windows
        system("sh -c \"read -n 1 -sr\""); // Reads a single character from the standard input
    #endif
}

/*
    Asks whether the user is a Doctor or a Patient
*/
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

/*
    Displays the menu for the Doctor user
*/
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

/*
    This function is used to write the symptoms and impressions to the file.

    @param masterList   is the list of symptoms and impressions
    @param fp_symptoms  is  the file that will contain the symptoms
    @param N            is the number of symptoms
*/
void writeSymptoms(pairSymptom* masterListSymptom, FILE *fp_symptoms, int N) {
    int counter;
    fp_symptoms = fopen("Symptoms.txt", "w");

    fprintf(fp_symptoms, "%d\n", N); // write the number of symptoms to the file
    for (counter = 0; counter < N; counter++) {
        fprintf(fp_symptoms, "%d\n", counter+1); // write the index of the symptom
        fprintf(fp_symptoms, "%s\n", masterListSymptom[counter].symptom); // write the symptom to the file
        fprintf(fp_symptoms, "%s\n", masterListSymptom[counter].question); // write the question to the file
    }

    fclose(fp_symptoms);
}

/*
    Gathers information from the Doctor user, the name of each symptom and its corresponding question.

    @param masterListSymptom    The list of symptoms and questions
    @param fp_symptoms          The file pointer to the file containing the list of symptoms and questions
    @param masterListImpression The list of impressions
*/
void inputSymptoms(pairSymptom* masterListSymptom, pairImpression* masterListImpression) {
    FILE *fp_symptoms;
    int counter;
    int noSymptoms;

    fp_symptoms = fopen("Symptoms.txt", "w");

    //I1: Asks for the number of symptoms that will be entered 
    do {
        printf("How many symptoms do you want to consider? ");
        scanf("%d", &noSymptoms);
        
        if (noSymptoms <= 0 || noSymptoms > MAX_SYMPTOMS) {
            printf("\n\nInvalid number of symptoms. Please try again.\n\n"); // If the number of symptoms is invalid
            sleepDelay(1);
        }
    } while (noSymptoms <= 0 || noSymptoms > MAX_SYMPTOMS);


    //I2: Enter the name of each symptom and its corresponding question
    for (counter = 0; counter < noSymptoms; counter++) {

        printf("Symptom # %d:\n", counter + 1); // Display the number of the symptom
        printf("What is the symptom?  ");
        scanf(" %[^\n]", masterListSymptom[counter].symptom); 
        
        printf("How do you want to ask the symptom?\n");
        scanf(" %[^\n]", masterListSymptom[counter].question); 
        strcat(masterListSymptom[counter].question, " [Y/N]"); // add a [Y/N] to the question   
    }

    masterListSymptom->overallSymptomsAmt = noSymptoms; // set the number of symptoms in the master list
    writeSymptoms(masterListSymptom, fp_symptoms, noSymptoms); // write the symptoms to the file
}


/*
    This function is used to write the impressions and its corresponding symptoms to the file.

    @param masterListImpression   is the list of impressions and symptoms
    @param masterListSymptom      is the list of symptoms and questions
    @param fp_impression          is the file that will contain the impressions and their corresponding symptoms
*/
void printImpressions(pairImpression* masterListImpression, pairSymptom* masterListSymptom, FILE *fp_impressions) {
    int counter, counter2;
    fp_impressions = fopen("Impressions.txt", "w");

    fprintf(fp_impressions, "%d\n", masterListImpression->impressionsAmount); // write the number of impressions to the file

    for (counter = 0; counter < masterListImpression->impressionsAmount; counter++) {
        fprintf(fp_impressions, "%d\n", counter+1); // write the index of the impression
        fprintf(fp_impressions, "%s\n", masterListImpression[counter].impression);

        // print all index of symptoms under the impression
        for (counter2 = 0; counter2 < masterListImpression[counter].symptomsAmountPerImpression; counter2++) {
            fprintf(fp_impressions, "%d ", masterListImpression[counter].symptomsIndexPerImpression[counter2]+1);
        }
        fprintf(fp_impressions, "\n");
    }

    fclose(fp_impressions);
}

/*
    This function is used to gather information from the Doctor user, the name of each impression and the symptoms that are under it.

    @param masterListImpression The list of impressions
    @param masterListSymptom    The list of symptoms and questions
    @param fp_impression        The file pointer to the file containing the list of impressions and its symptoms
*/
void inputImpression(pairImpression* masterListImpression, pairSymptom* masterListSymptom) {
    int noImpressions = 0;
    int presentSymptoms;
    int counter1;
    int counter2;
    int symptomIndex;
    FILE *fp_impressions;

    fp_impressions = fopen("Impressions.txt", "w");


    //I3: Ask for the number of impressions that will be entered
    do {
        printf("How many impressions do you want to enter? ");
        scanf("%d", &noImpressions);
        
        if (noImpressions <= 0 || noImpressions > MAX_IMPRESSION) {
            printf("\n\nInvalid number of impressions. Please try again.\n\n"); // If the number of impressions is invalid
            sleepDelay(1);
        }
    } while (noImpressions <= 0 || noImpressions > MAX_IMPRESSION);

    //I4: Enter each impression and list all the symptom names entered in I2
    for (counter1 = 0; counter1 < noImpressions; counter1++) {
        printf("Impression # %d:\n", counter1 + 1); // Display the number of the impression
        printf("What is the illness?  ");
        scanf(" %[^\n]", masterListImpression[counter1].impression); 
    
    
        //Print all listed in masterlist symptoms
        printf("\nBelow is a list of symptoms.\n");
        for (counter2 = 0; counter2 < masterListSymptom->overallSymptomsAmt; counter2++) {
            printf("%d. %s\n", counter2 + 1, masterListSymptom[counter2].symptom);
        }
        
        // asks for the number of symptoms that are present in the case 
        printf("How many of the symptoms above are present in a %s case?  ", masterListImpression[counter1].impression);
        scanf("%d", &presentSymptoms); 
        
        // assigns no. of symptoms to the current impression
        masterListImpression[counter1].symptomsAmountPerImpression = presentSymptoms;

        // assigns the symptoms to the current impression
        printf("Enter the corresponding number of each symptom:\n");

        for(counter2 = 0; counter2 < presentSymptoms; counter2++){
            scanf("%d", &symptomIndex); // get the index of the symptom
            symptomIndex -= 1; // decrement the index by 1 to match the index of the symptom in the master list

            masterListImpression[counter1].symptoms[counter2] = masterListSymptom[symptomIndex]; // assign the symptom to the current impression
            masterListImpression[counter1].symptomsIndexPerImpression[counter2] = symptomIndex; // assign the index of the symptom to the current impression
        }
    }

    masterListImpression->impressionsAmount = noImpressions; // set the number of impressions in the master list
    printImpressions(masterListImpression, masterListSymptom, fp_impressions); // write the impressions to the file
}

void displaySymptoms(pairImpression* masterListImpression, pairSymptom* masterListSymptomm) { 
    String50 impression;
    int counter, counter2;

    printf("What is the impression?  ");
    scanf(" %[^\n]", impression);

    for (counter = 0; counter < masterListImpression->impressionsAmount; counter++) {
        if (strcmp(impression, masterListImpression[counter].impression) == 0) {
            printf("\nThe symptoms for %s are:\n", impression);
            for (counter2 = 0; counter2 < masterListImpression[counter].symptomsAmountPerImpression; counter2++) {
                printf(" %s\n", counter2 + 1, masterListImpression[counter].symptoms[counter2].symptom);
            }
        }
    }
    
}


void extractList(pairImpression* masterListImpression, pairSymptom* masterListSymptom) {
    FILE *fp_impressions;
    FILE *fp_symptoms;

    fp_impressions = fopen("Impressions.txt", "r");
    fp_symptoms = fopen("Symptoms.txt", "r");

    if (fp_impressions != NULL && fp_symptoms != NULL) {
        readImpressions(masterListImpression, fp_impressions); // read the impressions from the file
        readSymptoms(masterListSymptom, fp_symptoms); // read the symptoms from the file

        printf("\nSymptoms and Impressions has been extracted from the files.\n");
    }
    else {
        printf("\n\nError: File not found.\n\n");
        sleepDelay(1);
    }

    fclose(fp_impressions);
    fclose(fp_symptoms);
}

void readImpressions(pairImpression* masterListImpression, FILE *fp_impressions) {
    
}

void readSymptoms(pairSymptom* masterListSymptom, FILE *fp_symptoms) {

}

void modifySymptoms(pairImpression* masterListImpression, pairSymptom* masterListSymptom){
    displaySymptoms(masterListImpression, masterListSymptom); //this should return a string
    
    //printf("You can modify the symptoms of %s", );
    //this could also be connected with inputImpression
}

void doctorChoice(char choice, pairSymptom* masterListSymptom, pairImpression* masterListImpression) {
    switch(choice) {
        case 'C':
        case 'c':
            inputSymptoms(masterListSymptom, masterListImpression);
            inputImpression(masterListImpression, masterListSymptom);
            break;
        case 'D':
        case 'd':
            displaySymptoms(masterListImpression, masterListSymptom);
            break;
        case 'M':
        case 'm':
            modifySymptoms(masterListImpression, masterListSymptom);
            break;
        case 'U':
        case 'u':
            extractList(masterListImpression, masterListSymptom);
            break;
    }
}

void printPatientInfo(patientInformation *patient) {
    FILE *fp_patient;
    String50 filename;


    // Set filename to patient's ID no.
    sprintf(filename, "%d.txt", patient->patientno);

    // Open file
    fp_patient = fopen(filename, "w");


    fprintf(fp_patient, "History of Patient Illness\n\n");
    fprintf(fp_patient, "%s, is a %d-year old ", patient->name, patient->age);
    if (patient->gender == 'M' || patient->gender == 'm')
        fprintf(fp_patient, "male. He has ");
    if (patient->gender == 'F' || patient->gender == 'f')
        fprintf(fp_patient, "female. She has ");
    // print symptoms and impressions.

    fclose(fp_patient);
}

void getPatientInfo (patientInformation *patient)
{
    printf("What is your name? ");
    scanf(" %[^\n]", patient->name);
    
    printf("What is your patient number? ");
    scanf("%d", &patient->patientno);

    printf("Enter your age: ");
    scanf("%d", &patient->age);
    do {
        printf("Gender (M/F): ");
        scanf(" %c", &patient->gender);

        if (!(patient->gender == 'M' || patient->gender == 'm' || patient->gender == 'F' || patient->gender == 'f'))
            printf("Wrong input, try again.\n\n");
    } while (!(patient->gender == 'M' || patient->gender == 'm' || patient->gender == 'F' || patient->gender == 'f'));
    

    // ask questions to patient

    // write text file
    printPatientInfo(patient);

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

    char cMenuChoice = userType();
    
    while (cMenuChoice != 'E' && cMenuChoice != 'e') {
        switch (cMenuChoice) {
            case 'D':
            case 'd':
                doctorChoice(doctorMenu(), masterListSymptom, masterListImpression);
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