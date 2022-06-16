#include "clinic.h"

#if defined(_WIN32) // If the system is Windows
    #include <conio.h> // For the pause function
    #include <windows.h> // For the sleep function
#else // If the system is not Windows
    #include <unistd.h> // For the pause function
#endif

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