#include "clinic.h"


#if defined(_WIN32) // If the system is Windows
    #include <conio.h> // For the pause function
    #include <windows.h> // For the sleep function
#else // If the system is not Windows
    #include <unistd.h> // For the pause function
#endif

/**
    This function is used to write the symptoms and impressions to the file.

    @param masterList   is the list of symptoms and impressions.
    @param fp_symptoms  is  the file that will contain the symptoms.
    @param N    is the number of symptoms.

    Pre-condition: masterListSymptom is not empty.
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

    Pre-condition: masterListSymptom is empty.
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

    Pre-condition: masterListImpression is not empty.
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

    Pre-condition: impressionIndex is an integer and masterListSymptom is not empty.
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

    Pre-condition: masterListImpression is empty.
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

        assignSymptoms(masterListImpression, masterListSymptom, counter1); // assign the symptoms to the current impression
    }

    masterListImpression->impressionsAmount = noImpressions; // set the number of impressions in the master list
    printImpressions(masterListImpression, fp_impressions); // write the impressions to the file
}


/**
    It checks if a given impression is present in the master list of impressions.

    @param masterListImpression     is a pointer to a struct that contains a char* and an int.
    @param impression   the impression to be checked.

    @return an integer.

    Pre-condition: masterListImpression is not empty.
 */
int 
isImpressionPresent (pairImpression* masterListImpression, 
                    char* impression) 
{
    int i;
    for (i = 0; i < masterListImpression->impressionsAmount; i++) {
        if (strcmp(masterListImpression[i].impression, impression) == 0) { // if the impression is present
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

    Pre-condition: masterListImpression and masterListSymptom is not empty.
 */
void 
displaySymptoms(String50 impression, 
                pairImpression* masterListImpression,
                pairSymptom* masterListSymptom)
{ 
    int counter;
    int counter2;

    for (counter = 0; counter < masterListImpression->impressionsAmount; counter++) {
        if (strcmp(impression, masterListImpression[counter].impression) == 0) { // if the impression is present
            printf("\nThe symptoms for %s are:\n", impression);
            for (counter2 = 0; counter2 < masterListImpression[counter].symptomsAmountPerImpression; counter2++) { // for each symptom
                printf(" %s\n", masterListSymptom[masterListImpression[counter].symptomsIndexPerImpression[counter2]].symptom); // print the symptom
            }
        }
    }
}



/**
    It asks the user for an impression, then displays the symptoms of that impression, theIn asks the
    user to modify the symptoms of that impression.
   
    @param masterListImpression     a struct that contains the impression and the amount of symptoms it has.
    @param masterListSymptom    a struct that contains the symptoms.

    Pre-condition: masterListImpression is not empty.
 */
void 
modifySymptoms (pairImpression* masterListImpression, 
                pairSymptom* masterListSymptom) 
{
    int impressionIndex;
    String50 impression;
    FILE *fp_impressions;
    int counter;

    // Prints all impressions
    printf("\n\nList of Impressions:\n");
    for (counter = 0; counter < masterListImpression->impressionsAmount; counter++) {
        printf("- %s\n",masterListImpression[counter].impression);
    }
    printf("\n");

    // Ask for the impression that the user wants to modify
    do {
        printf("What is the impression?  ");
        scanf(" %[^\n]", impression);

        if (!isImpressionPresent(masterListImpression, impression)) {
            printf("\n\nImpression not found. Take note that this is case-sensitive.\nPlease try again.\n\n");
            sleepDelay(1);
        }
    } while (!isImpressionPresent(masterListImpression, impression));

    sleepDelay(1);
    displaySymptoms(impression, masterListImpression, masterListSymptom); // display the symptoms of the given impression

    sleepDelay(1);
    printf("\nYou can modify the symptoms of %s.\n", impression);

    // Get the index of the impression
    for (counter = 0; counter < masterListImpression->impressionsAmount; counter++) {
        if (strcmp(impression, masterListImpression[counter].impression) == 0) {
            impressionIndex = counter;
        }
    }

    fp_impressions = fopen("Impressions.txt", "w");

    sleepDelay(1);
    assignSymptoms(masterListImpression, masterListSymptom, impressionIndex); // assign the symptoms to the current impression
    printImpressions(masterListImpression, fp_impressions); // write the impressions to the file

    sleepDelay(1);
    printf("\n\nSymptoms for %s have been modified.\n", impression);

    displaySymptoms(impression, masterListImpression, masterListSymptom); // display the new symptoms of the given impression

    sleepDelay(1);
    fclose(fp_impressions);
    displayKey();

}