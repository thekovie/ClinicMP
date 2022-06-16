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
                        this project p`ossible.        
 */

#include "displayAndMenus.c"
#include "symptomsImpressions.c"
#include "patientProcessing.c"
#include "fileProcessing.c"



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