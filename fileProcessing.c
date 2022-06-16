#include "clinic.h"


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
