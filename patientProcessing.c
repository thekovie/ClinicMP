#include "clinic.h"






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
        
    if (patient->gender == 'F' || (patient->gender == 'f' && patient->patientSympAmt > 0)) {
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

