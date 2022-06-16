/**
    Description:       This is a console-based program that can generate a list of clinical impressions 
                      along with other patient information in a summary in the form of history of present illness (HPI).
    
    Programmed by:     John Kovie L. Niño
                       Reign Elaiza D. Larraquel
    
    Last modified:     Jun 16, 2022
    Version:           1.0
    Acknowledgements:  - I thank https://unix.stackexchange.com/questions/293940/how-can-i-make-press-any-key-to-continue
                        and https://stackoverflow.com/questions/5725296/difference-between-sh-and-bash
                        for the help in implementing the pause code in some operating systems and other libraries for making
                        this project possible.        
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) // If the system is Windows
    #include <conio.h> // For the pause function
    #include <windows.h> // For the sleep function
#else // If the system is not Windows
    #include <unistd.h> // For the pause function
#endif


#define MAX_STRING_SIZE 51
#define MAX_SYMPTOMS 20
#define MAX_IMPRESSION 20

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


/**
    The function `sleepDelay` is a cross-platform function that will delay the execution of the program
    for `n` seconds

    @param n    The number of seconds to sleep.
    Pre-condition: n must be a positive integer.
 */
void 
sleepDelay (int n) 
{
    #if defined(_WIN32)
        Sleep(1000*n);
    #else
        sleep(1*n);
    #endif
}


/**
    It waits for the user to press a key.
    Pre-condition: None.
 */
void 
displayKey () 
{
    printf("\n\nPress any key to continue...\n");
    #if defined(_WIN32) && !defined(UNIX) // If the system is Windows
        getch();
    #else // If the system is not Windows
        system("sh -c \"read -n 1 -sr\""); // Reads a single character from the standard input
    #endif
}


/**
    It asks the user to enter a choice of either D, P, or E. If the user enters anything other than D,
    P, or E, the user is asked to try again.
  
    @return The user's choice.
    Pre-condition: None.
 */
char 
userType() 
{
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


/**
    It's a function that displays a menu for the doctor to use, and returns the doctor's choice.

    @return The choice of the user.
    Pre-condition: None.
 */
char 
doctorMenu() 
{
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


/**
    This function is used to write the symptoms and impressions to the file.

    @param masterList   is the list of symptoms and impressions.
    @param fp_symptoms  is  the file that will contain the symptoms.
    @param N    is the number of symptoms.
*/
void 
writeSymptoms(pairSymptom* masterListSymptom, 
              FILE *fp_symptoms, 
              int N) 
{
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


/**
    It asks the user for the number of symptoms they want to consider, then asks for the name of each
    symptom and its corresponding question.

    @param masterListSymptom    a pointer to a struct that contains the symptom and the question.
 */
void 
inputSymptoms(pairSymptom* masterListSymptom) 
{
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
        printf("\n\n");
        do {
            printf("Symptom # %d:\n", counter + 1); // Display the number of the symptom
            printf("What is the symptom?  ");
            scanf(" %[^\n]", masterListSymptom[counter].symptom);
            if (strlen(masterListSymptom[counter].symptom) > MAX_STRING_SIZE) {
                printf("\n\nYour input went over the string lentgth. Please try again.\n\n");
                sleepDelay(1);
            }
        } while (strlen(masterListSymptom[counter].symptom) > MAX_STRING_SIZE);

        
        do {
            printf("How do you want to ask the symptom?\n");
            scanf(" %[^\n]", masterListSymptom[counter].question);

            if (strlen(masterListSymptom[counter].question) > MAX_STRING_SIZE) {
                printf("\n\nYour input went over the string lentgth. Please try again.\n\n");
                sleepDelay(1);
            }
        } while (strlen(masterListSymptom[counter].question) > MAX_STRING_SIZE);

        // add question mark at the end if there is no question mark
        if (masterListSymptom[counter].question[strlen(masterListSymptom[counter].question) - 1] != '?') {
            strcat(masterListSymptom[counter].question, "?");
        }

        strcat(masterListSymptom[counter].question, " [Y/N]"); // add a [Y/N] to the question
    }

    masterListSymptom->overallSymptomsAmt = noSymptoms; // set the number of symptoms in the master list
    writeSymptoms(masterListSymptom, fp_symptoms, noSymptoms); // write the symptoms to the file
    
}


/**
    This function is used to write the impressions and its corresponding symptoms to the file.

    @param masterListImpression   is the list of impressions and symptoms.
    @param masterListSymptom    is the list of symptoms and questions.
    @param fp_impression    is the file that will contain the impressions and their corresponding symptoms.
*/
void 
printImpressions (pairImpression* masterListImpression, 
                  FILE *fp_impressions) 
{
    int counter;
    int counter2;

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


/**
    It asks the user for the number of symptoms present in the case, then asks the user to input the
    corresponding number of each symptom

    @param masterListImpression     is a pointer to a struct that contains the impression and the symptoms
                                    that are associated with it.
    @param masterListSymptom     is a pointer to a struct that contains the list of symptoms
    @param impressionIndex   the index of the current impression
 */
void 
assignSymptoms (pairImpression* masterListImpression, 
                pairSymptom* masterListSymptom, 
                int impressionIndex) 
{
    int presentSymptoms;
    int counter2;
    int symptomIndex;

    printf("\nBelow is a list of symptoms.\n");
    printf(" NO.\tSYMPTOM NAME\n");
    for (counter2 = 0; counter2 < masterListSymptom->overallSymptomsAmt; counter2++) {
        printf(" [%d]\t%s\n", counter2 + 1, masterListSymptom[counter2].symptom);
    }
    
    // asks for the number of symptoms that are present in the case
    // while the number of symptoms is not valid
    do {
        printf("\nHow many symptoms are present in the case? ");
        scanf("%d", &presentSymptoms);
        
        if (presentSymptoms <= 0 || presentSymptoms > masterListSymptom->overallSymptomsAmt) {
            printf("\n\nInvalid number of symptoms. Please try again.\n\n"); // If the number of symptoms is invalid
            sleepDelay(1);
        }
    } while (presentSymptoms <= 0 || presentSymptoms > masterListSymptom->overallSymptomsAmt);

    // assigns no. of symptoms to the current impression
    masterListImpression[impressionIndex].symptomsAmountPerImpression = presentSymptoms;

    // assigns the symptoms to the current impression
    printf("Enter the corresponding number of each symptom:\n");

    for (counter2 = 0; counter2 < presentSymptoms; counter2++) {
        // check if the symptom is valid
        do {
            printf("Symptom # %d: ", counter2 + 1);
            scanf("%d", &symptomIndex);
            if (symptomIndex <= 0 || symptomIndex > masterListSymptom->overallSymptomsAmt) {
                printf("\n\nInvalid symptom. Please try again.\n\n");
                sleepDelay(1);
            }
        } while (symptomIndex <= 0 || symptomIndex > masterListSymptom->overallSymptomsAmt);
        
        symptomIndex -= 1; // decrement the index by 1 to match the index of the symptom in the master list
        masterListImpression[impressionIndex].symptomsIndexPerImpression[counter2] = symptomIndex; // assign the index of the symptom to the current impression
    }
}


/**
    This function is used to gather information from the Doctor user, the name of each impression and the 
    symptoms that are under it.

    @param masterListImpression     The list of impressions
    @param masterListSymptom        The list of symptoms and questions
*/
void 
inputImpression (pairImpression* masterListImpression, 
                 pairSymptom* masterListSymptom) 
{
    int noImpressions = 0;
    int counter1;
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
        do {
            printf("\nImpression # %d:\n", counter1 + 1); // Display the number of the impression
            printf("What is the illness?  ");
            scanf(" %[^\n]", masterListImpression[counter1].impression);

            if (strlen(masterListImpression[counter1].impression) > MAX_STRING_SIZE) {
                printf("\n\nInvalid impression. Please try again.\n\n");
                sleepDelay(1);
            }
        } while (strlen(masterListImpression[counter1].impression) > MAX_STRING_SIZE);
    
        assignSymptoms(masterListImpression, masterListSymptom, counter1);
    }

    masterListImpression->impressionsAmount = noImpressions; // set the number of impressions in the master list
    printImpressions(masterListImpression, fp_impressions); // write the impressions to the file
}


/**
    It checks if a given impression is present in the master list of impressions.

    @param masterListImpression     is a pointer to a struct that contains a char* and an int.
    @param impression   the impression to be checked.

    @return an integer.
 */
int 
isImpressionPresent (pairImpression* masterListImpression, 
                    char* impression) 
{
    int i;
    for (i = 0; i < masterListImpression->impressionsAmount; i++) {
        if (strcmp(masterListImpression[i].impression, impression) == 0) {
            return 1;
        }
    }
    return 0;
}


/**
    It takes in a list of impressions and a list of symptoms, and then asks the user for an impression.
    It then searches the list of impressions for the user's input, and if it finds it, it prints out the
    symptoms associated with that impression.

    @param masterListImpression     This is a pointer to a struct that contains the impression and the 
                                    symptoms associated with it.
    @param masterListSymptom    a pointer to a struct that contains a list of symptoms.
 */
void 
displaySymptoms(String50 impression, 
                pairImpression* masterListImpression,
                pairSymptom* masterListSymptom)
{ 
    int counter;
    int counter2;

    for (counter = 0; counter < masterListImpression->impressionsAmount; counter++) {
        if (strcmp(impression, masterListImpression[counter].impression) == 0) {
            printf("\nThe symptoms for %s are:\n", impression);
            for (counter2 = 0; counter2 < masterListImpression[counter].symptomsAmountPerImpression; counter2++) {
                printf(" %s\n", masterListSymptom[counter2].symptom);
            }
        }
    }
}


/**
    It reads the impressions file and stores the data in a struct.

    @param masterListImpression     an array of structs that holds the impressions.
    @param fp_impressions   the file pointer to the file that contains the impressions.
 */
void 
readImpressions(pairImpression* masterListImpression, 
                FILE *fp_impressions) 
{
    int impressionCount; 
    int index;
    int i;
    int j;
    int k;
    char line[60];
    
    fgets(line, MAX_STRING_SIZE, fp_impressions);
    sscanf(line, "%d", &impressionCount); // get the number of impressions

    masterListImpression->impressionsAmount = impressionCount;

    for (i = 1; i <= impressionCount * 3; i++) {
        fgets(line, MAX_STRING_SIZE, fp_impressions);
        line[strlen(line) - 1] = '\0'; // remove the newline character

        switch (i % 3) {
            case 1: { //index
                sscanf(line, "%d", &index);
            }
                break;
            case 0: { //symptoms
                String50 symptomNum = "";
                int sympNum = 0; // temporary variable to hold the symptom number
                int sympIndex = 0; //  index of symptomNum
                int impIndex = 0; // the index of the impression in the master list

                for (j = 0; j < strlen(line); j++) {

                    // if the character is a space, then we have reached the end of the symptom number
                    if (line[j] != ' ') {
                        symptomNum[sympIndex] = line[j];
                        sympIndex++;
                    }

    
                    else if ((line[j] == ' ' && sympIndex > 0) || j == strlen(line) - 1) { 
                        sympNum = atoi(symptomNum); // convert the string to an integer
                        masterListImpression[index - 1].symptomsIndexPerImpression[impIndex] = sympNum - 1; // subtract 1 to match the index of the symptom in the master list
                        
                        int length = strlen(symptomNum);

                        // clear the symptomNum
                        for (k = 0; k < length; k++) {
                            symptomNum[k] = '\0';
                        }
                        impIndex++; // increment the index of the symptom in the impression
                        sympIndex = 0; // reset the index of the symptomNum
                    }
                }
                masterListImpression[index - 1].symptomsAmountPerImpression = impIndex; // set the number of symptoms per impression
                
            }
                break;
            case 2: { //diagnosis
                strcpy(masterListImpression[index - 1].impression, line);
            }
                break;
        }
    }

}


/**
    It checks if the files exist and if they are empty.
 
    @return the value 1 or 0 depending on whether the files exist and are empty.
 */
int 
filesExists() 
{
    FILE *fp_impressions;
    FILE *fp_symptoms;

    fp_impressions = fopen("Impressions.txt", "r");
    fp_symptoms = fopen("Symptoms.txt", "r");

    int size = 0;
    int size2 = 0;

    if (fp_impressions == NULL || fp_symptoms == NULL) {
        printf("\n\nThe file/s does not exist.\n\n");
        return 0;
    }

    while (fgetc(fp_impressions) != EOF) {
        size++;
    }

    while (fgetc(fp_symptoms) != EOF) {
        size2++;
    }

    if (size == 0 || size2 == 0) {
        printf("\nImpressions or Symptoms textfile is empty.\n\n");
        return 0;
    }

    fclose(fp_impressions);
    fclose(fp_symptoms);
    return 1;
}


/**
    The function reads the number of symptoms from the file, then reads the symptom and the question for
    each symptom.
 
    @param masterListSymptom    a pointer to a struct that contains the symptom and the question.
    @param fp_symptoms  the file pointer to the file that contains the symptoms.
 */
void 
readSymptoms (pairSymptom* masterListSymptom, 
              FILE *fp_symptoms) 
{
    int counter;
    int dumpIndex;

    fscanf(fp_symptoms, "%d", &masterListSymptom->overallSymptomsAmt); // read the number of symptoms from the file

    for (counter = 0; counter < masterListSymptom->overallSymptomsAmt; counter++) {
        fscanf(fp_symptoms, "%d", &dumpIndex);
        fscanf(fp_symptoms, " %[^\n]", masterListSymptom[counter].symptom); // read the symptom
        fscanf(fp_symptoms, " %[^\n]", masterListSymptom[counter].question); // read the question
    }

}


/**
    The function checks if both Impressions and Symptoms has been extracted
 
    @param isFilesExtracted     a struct that contains two bools, one for each file.
   
    @return a 1 or 0.
 */
int 
ifExtracted (filesExtracted* isFilesExtracted) 
{
    if (isFilesExtracted->impressionBool == 1 && isFilesExtracted->symptomBool == 1) {
        return 1;
    }
    return 0;
}


/**
    It reads the symptoms and impressions from the files and stores them in the linked lists.
  
    @param masterListImpression     a struct that contains the impressions.
    @param masterListSymptom    a linked list of symptoms.
    @param isFilesExtracted     a struct that contains two bools, one for symptoms and one for impressions.
 */
void 
extractList (pairImpression* masterListImpression, 
             pairSymptom* masterListSymptom, 
             filesExtracted* isFilesExtracted) 
{
    FILE *fp_impressions;
    FILE *fp_symptoms;

    fp_impressions = fopen("Impressions.txt", "r");
    fp_symptoms = fopen("Symptoms.txt", "r");

    if (filesExists()) {
        readSymptoms(masterListSymptom, fp_symptoms); // read the symptoms from the file
        readImpressions(masterListImpression, fp_impressions); // read the impressions from the file

        printf("\nSymptoms and Impressions has been extracted from the files.\n");
        isFilesExtracted->impressionBool = 1;
        isFilesExtracted->symptomBool = 1;

    }

    fclose(fp_symptoms);
    fclose(fp_impressions);
    displayKey();
}


/**
    It asks the user for an impression, then displays the symptoms of that impression, theIn asks the
    user to modify the symptoms of that impression.
   
    @param masterListImpression     a struct that contains the impression and the amount of symptoms it has.
    @param masterListSymptom    a struct that contains the symptoms
 */
void 
modifySymptoms (pairImpression* masterListImpression, 
                pairSymptom* masterListSymptom) 
{
    int impressionIndex;
    String50 impression;
    FILE *fp_impressions;
    int counter;

    printf("\n\nList of Impressions:\n");
    for (counter = 0; counter < masterListImpression->impressionsAmount; counter++) {
        printf("- %s\n",masterListImpression[counter].impression);
    }
    printf("\n");

    do {
        printf("What is the impression?  ");
        scanf(" %[^\n]", impression);

        if (!isImpressionPresent(masterListImpression, impression)) {
            printf("\n\nImpression not found. Take note that this is case-sensitive.\nPlease try again.\n\n");
            sleepDelay(1);
        }
    } while (!isImpressionPresent(masterListImpression, impression));

    sleepDelay(1);
    displaySymptoms(impression, masterListImpression, masterListSymptom);

    sleepDelay(1);
    printf("\nYou can modify the symptoms of %s.\n", impression);

    for (counter = 0; counter < masterListImpression->impressionsAmount; counter++) {
        if (strcmp(impression, masterListImpression[counter].impression) == 0) {
            impressionIndex = counter;
        }
    }

    fp_impressions = fopen("Impressions.txt", "w");

    sleepDelay(1);
    assignSymptoms(masterListImpression, masterListSymptom, impressionIndex); 
    printImpressions(masterListImpression, fp_impressions);

    sleepDelay(1);
    printf("\n\nSymptoms for %s have been modified.\n", impression);

    displaySymptoms(impression, masterListImpression, masterListSymptom);

    sleepDelay(1);
    fclose(fp_impressions);
    displayKey();

}


/**
    It's a function that takes in a character, two structs, and a struct pointer. It then checks the
    character and does a switch case.
   
    @param choice   the choice of the user.
    @param masterListSymptom    a pointer to a struct that contains the symptoms and the amount of
                                symptoms.
    @param masterListImpression     a struct that contains the impression and the amount of impressions.
    @param isFilesExtracted     a struct that contains the boolean values of whether the files have been
                                extracted or not.
 */
void 
doctorChoice (char choice, 
              pairSymptom* masterListSymptom, 
              pairImpression* masterListImpression, 
              filesExtracted* isFilesExtracted) 
{
    String50 impression;
    int counter;

    system("clear || cls");

    switch(choice) {
        case 'C':
        case 'c':
            inputSymptoms(masterListSymptom);
            isFilesExtracted->symptomBool = 1;
            system("clear || cls");
            inputImpression(masterListImpression, masterListSymptom);
            isFilesExtracted->impressionBool = 1;
            break;
        case 'D':
        case 'd':
            if (ifExtracted(isFilesExtracted)) {
                printf("\n\nList of Impressions:\n");
                for (counter = 0; counter < masterListImpression->impressionsAmount; counter++) {
                    printf("- %s\n",masterListImpression[counter].impression);
                }
                printf("\n");

                do {
                printf("What is the impression?  ");
                scanf(" %[^\n]", impression);

                    if (!isImpressionPresent(masterListImpression, impression)) {
                        printf("\n\nImpression not found. Take note that this is case-sensitive.\nPlease try again.\n\n");
                        sleepDelay(1);
                    }
                } while (!isImpressionPresent(masterListImpression, impression));
                
                sleepDelay(1);
                displaySymptoms(impression, masterListImpression, masterListSymptom);
            }
            else {
                printf("\n[ERROR!] You cannot enter this option yet. Load the database first.\n\n");
            }
            
            displayKey();

            break;

        case 'M':
        case 'm':
            if (ifExtracted(isFilesExtracted)) {
                modifySymptoms(masterListImpression, masterListSymptom);
            }
            else {
                printf("\n[ERROR!] You cannot enter this option yet. Load the database first.\n\n");
                displayKey();
            }
            break;
        case 'U':
        case 'u':
            extractList(masterListImpression, masterListSymptom, isFilesExtracted);
            break;

       
    }
    system("clear || cls");
}


/**
    It prints the patient's information to a file.
   
    @param patient  a pointer to a patientInformation struct.
 */
void 
printPatientInfo (patientInformation *patient) 
{
    FILE *fp_patient;
    String50 filename;
    int counter = 0;
    int counter2 = 0;


    // Set filename to patient's ID no.
    sprintf(filename, "%d.txt", patient->patientno);

    // Open file
    fp_patient = fopen(filename, "w");


    fprintf(fp_patient, "History of Patient Illness\n\n");
    printf("\nHistory of Patient Illness\n\n");

    // Print patient's name, ID no., and age
    fprintf(fp_patient, "%s, patient # %d is a %d-year old ", patient->name, patient->patientno, patient->age);
    printf("%s, patient # %d is a %d-year old ", patient->name, patient->patientno, patient->age);

    // Print patient's gender
    if (patient->gender == 'M' || patient->gender == 'm') {
        fprintf(fp_patient, "male.");
        printf("male.");
    }
        
    if (patient->gender == 'F' || patient->gender == 'f') {
        fprintf(fp_patient, "female.");
        printf("female.");
    }

    // Print if patient has symptoms
    if ((patient->gender == 'M' || patient->gender == 'm') && patient->patientSympAmt > 0) {
        fprintf(fp_patient, " He has ");
        printf(" He has ");
    }
        
    if (patient->gender == 'F' || patient->gender == 'f' && patient->patientSympAmt > 0) {
        fprintf(fp_patient, " She has ");
        printf(" She has ");
    }
    
    // Print patient's symptoms
    if(patient->patientSympAmt == 1){ // If the patient is experiencing only 1 symptom
        fprintf(fp_patient, "%s.", patient->patientSymptoms[counter].symptom);
        printf("%s.", patient->patientSymptoms[counter].symptom);
    }
    else if(patient->patientSympAmt == 2){ // If the patient is experiencing 2 symptoms
        fprintf(fp_patient, "%s and %s.", patient->patientSymptoms[counter].symptom, patient->patientSymptoms[counter + 1].symptom);
        printf("%s and %s.", patient->patientSymptoms[counter].symptom, patient->patientSymptoms[counter + 1].symptom);
    }
    else if (patient->patientSympAmt > 2) { // If the patient is experiencing more than 2 symptoms
        for (counter = 0; counter < patient->patientSympAmt; counter++) { 
            if (counter < (patient->patientSympAmt - 1)) {
                fprintf(fp_patient, "%s, ", patient->patientSymptoms[counter].symptom);
                printf("%s, ", patient->patientSymptoms[counter].symptom);
            }
            else if (counter == (patient->patientSympAmt - 1)) {
                fprintf(fp_patient, "and %s.", patient->patientSymptoms[counter].symptom);
                printf("and %s.", patient->patientSymptoms[counter].symptom);
            }
        }
    }

    //Checks if there is an impression
    if (patient->patientImpAmt == 0) {
        fprintf(fp_patient, " There are no impressions to be formed based on the symptoms present or observed.");
        printf(" There are no impressions to be formed based on the symptoms present or observed.");
    }
    else if (patient->patientImpAmt == 1) {
        fprintf(fp_patient, " The impression is ");
        printf(" The impression is ");
    }    
    else {
        fprintf(fp_patient, " Impressions are ");
        printf(" Impressions are ");
    }

    // Print patient's impressions
    if (patient->patientImpAmt == 1) { // If the patient is experiencing only 1 impression
        fprintf(fp_patient, "%s.", patient->patientImpressions[counter2].impression);
        printf("%s.", patient->patientImpressions[counter2].impression);
    }
    else if (patient->patientImpAmt == 2) { // If the patient is experiencing 2 impressions
        fprintf(fp_patient, "%s and %s.", patient->patientImpressions[counter2].impression, patient->patientImpressions[counter2 + 1].impression);
        printf("%s and %s.", patient->patientImpressions[counter2].impression, patient->patientImpressions[counter2 + 1].impression);
    }
    else if (patient->patientImpAmt > 2) { // If the patient is experiencing more than 2 impressions
        for (counter2 = 0; counter2 < patient->patientImpAmt; counter2++) { 
            if (counter2 < (patient->patientImpAmt - 1)) {
                fprintf(fp_patient, "%s, ", patient->patientImpressions[counter2].impression);
                printf("%s, ", patient->patientImpressions[counter2].impression);
            }
            else if (counter2 == (patient->patientImpAmt - 1)) {
                fprintf(fp_patient, "and %s.", patient->patientImpressions[counter2].impression);
                printf("and %s.", patient->patientImpressions[counter2].impression);
            }
        }
    }

    printf("\n\nGenerating HPI text file for Patient No. %d...\n", patient->patientno);
    sleepDelay(2);
    fclose(fp_patient);  

    printf("History of Patient Illness text file for Patient No. %d has been generated.\nKindly open %d.txt to check your HPI.\n\n", patient->patientno, patient->patientno);
    displayKey();
}


/**
    It compares the symptoms of the patient to the symptoms of the master list of impressions. If the
    symptoms match, the impression is added to the patient's impression list.
   
    @param patient  a struct that contains the patient's symptoms and impressions.
    @param masterListSymptom    a struct that contains the symptom and its index.
    @param masterListImpression     a struct that contains the impression and the symptoms that are
                                    associated with it.
 */
void 
getPatientImpression (patientInformation *patient, 
                      pairSymptom *masterListSymptom, 
                      pairImpression *masterListImpression) 
{
    int counter1;
    int counter2;
    int counter3;
    int matchSymptom = 0;
    int impressionIndex = 0;

    for (counter1 = 0; counter1 < masterListImpression->impressionsAmount; counter1++) { // For each impression in the master list
        for (counter2 = 0; counter2 < masterListImpression[counter1].symptomsAmountPerImpression; counter2++) { // For each symptom of the impression in the master list
            for (counter3 = 0; counter3 < patient->patientSympAmt; counter3++) { // For each symptom of the patient
                if (strcmp(masterListSymptom[masterListImpression[counter1].symptomsIndexPerImpression[counter2]].symptom, patient->patientSymptoms[counter3].symptom) == 0){
                    matchSymptom += 1; // The amount of symptoms that match the impression
                }
            }

            if (matchSymptom == masterListImpression[counter1].symptomsAmountPerImpression){ // If the patient has all the symptoms of the impression
                strcpy(patient->patientImpressions[impressionIndex].impression, masterListImpression[counter1].impression);
                impressionIndex += 1;
            }         
        
        }
        matchSymptom = 0;

    }

    patient->patientImpAmt = impressionIndex;
}


/**
    The function asks the user to input a Y or N for each symptom in the master list of symptoms. If the
    user inputs a Y, the symptom is added to the patient's list of symptoms.
   
    @param patient  a struct that holds the patient's information.
    @param masterListSymptom    This is a pointer to a struct that contains a list of symptoms.
 */
void 
getPatientSymptoms (patientInformation *patient,
                    pairSymptom *masterListSymptom)
{
    char answer;
    int counter;
    int symptomIndex = 0;

    for(counter = 0; counter < masterListSymptom->overallSymptomsAmt; counter++) {
        do {
            printf("%s  ", masterListSymptom[counter].question);
            scanf(" %c", &answer);

            if (answer == 'Y' || answer == 'y') {
                strcpy(patient->patientSymptoms[symptomIndex].symptom, masterListSymptom[counter].symptom);
                symptomIndex += 1;
            }

            if (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n') {
                printf("Invalid input. Please enter Y or N.\n\n");
            }
        } while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');
    }

    patient->patientSympAmt = symptomIndex;
}


/**
    It takes a pointer to a patientInformation struct, and then it loops through the patientSymptoms and
    patientImpressions arrays, and sets each element to an empty string.
    
    @param patient  a pointer to a patientInformation struct.
 */
void 
emptyHistory (patientInformation *patient) 
{
    int symp;
    int imp;

    for(symp = 0; symp < patient->patientSympAmt; symp++) {
        strcpy(patient->patientSymptoms[symp].symptom, "");
    }
    for(imp = 0; imp < patient->patientImpAmt; imp++) {
        strcpy(patient->patientImpressions[imp].impression, "");
    }

    patient->patientSympAmt = 0;
    patient->patientImpAmt = 0;
}


/**
    It asks the user for their name, age, gender, and then asks them a series of questions about their
    symptoms and impressions.
   
    @param patient  a struct that contains the patient's information.
    @param masterListSymptom    a linked list of symptoms.
    @param masterListImpression     a linked list of impressions.
 */
void 
getPatientInfo (patientInformation *patient, 
                pairSymptom *masterListSymptom, 
                pairImpression *masterListImpression)
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

        if (!(patient->gender == 'M' || patient->gender == 'm' || patient->gender == 'F' || patient->gender == 'f')) {
            printf("Wrong input, try again.\n\n");
        }
    } while (!(patient->gender == 'M' || patient->gender == 'm' || patient->gender == 'F' || patient->gender == 'f'));
    
    emptyHistory(patient);

    // ask questions to patient
    getPatientSymptoms(patient, masterListSymptom);

    // get the impression for the patient 
    getPatientImpression(patient, masterListSymptom, masterListImpression);

    // write text file
    printPatientInfo(patient);

}


int 
main() 
{
    pairSymptom masterListSymptom[MAX_SYMPTOMS];
    pairImpression masterListImpression[MAX_IMPRESSION];
    filesExtracted isFilesExtracted;
    patientInformation patient;
    char docChoice;

    char cMenuChoice;
    
    do {
        //system("clear || cls");
        cMenuChoice = userType();
        switch (cMenuChoice) {
            case 'D':
            case 'd':
                do {
                    system("clear || cls");
                    docChoice = doctorMenu();
                    doctorChoice(docChoice, masterListSymptom, masterListImpression, &isFilesExtracted);
                } while(docChoice != 'E' && docChoice != 'e' &&  docChoice != 'U' && docChoice != 'u');
                break;
            case 'P':
            case 'p':
                if (ifExtracted(&isFilesExtracted)) {
                    getPatientInfo(&patient, masterListSymptom, masterListImpression);
                }
                else {
                    printf("\n[ERROR!] You cannot enter this option yet. Load the database first.\n\n");
                    displayKey();
                }
                break;
        }
    } while (cMenuChoice != 'E' && cMenuChoice != 'e');
    

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