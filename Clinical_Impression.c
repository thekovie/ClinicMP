/**
 * Description:       This is a console-based program that can generate a list of clinical impressions 
 *                    along with other patient information in a summary in the form of history of present illness (HPI).
 * 
 * Programmed by:     John Kovie L. Niño
 *                    Reign Elaiza D. Larraquel
 * 
 * Last modified:     May 22, 2022
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


#define MAX_STRING_SIZE 50
#define MAX_SYMPTOMS 20
#define MAX_IMPRESSION 20


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

        if (!(choice == 'D' || choice == 'P' || choice == 'E')) {
            printf("\n\nInvalid choice. Please try again.\n\n");
            sleepDelay(1);
        }
    } while (!(choice == 'D' || choice == 'P' || choice == 'E'));

    return choice;
}

void inputSymptoms() {
    int N = 0;
    int counter;

    //I1: Ask for the number of symptoms (let’s say this is N) 
    do {
        printf("How many symptoms do you want to consider? ");
        scanf("%d", &N);
        
        if (N == 0 || N > MAX_SYMPTOMS) {
            printf("\n\nInvalid number of symptoms. Please try again.\n\n");
            sleepDelay(1);
        }
    } while (N == 0 || N > MAX_SYMPTOMS);

    //I2: Enter the name of each symptom and its corresponding question
    for(counter = 0; counter < N; counter++){
        printf("Symptom # %d:\n", counter + 1);
        printf("What is the symptom?  ");
        //Place scanf here for symptom

        printf("How do you want to ask the symptom?\n");
        //Place scanf here for symptom question
    }
}

void inputImpression() {
    int I = 0;
    int presentSymptoms;
    int counter1, counter2;

    //I3: Ask for the number of impressions
    do {
        printf("How many impressions do you want to enter? ");
        scanf("%d", &I);
        
        if (I == 0 || I > MAX_IMPRESSION) {
            printf("\n\nInvalid number of impressions. Please try again.\n\n");
            sleepDelay(1);
        }
    } while (I == 0 || I > MAX_IMPRESSION);

    //I4: For I times, ask for each impression and list all the symptom names entered in I2
    for(counter1 = 0; counter1 < I; counter1++){
        printf("Impression # %d:\n", counter1 + 1);
        printf("What is the illness?  ");
        //Place scanf here for illness

        printf("\nBelow is a list of symptoms.\n");
        //Place list of symptoms here

        //printf("How many of the symptoms above are present in a %s case?  ", /*add string array*/);
        //scanf("%d", &presentSymptoms);

        printf("Enter the corresponding number of each symptom:\n");

        for(counter2 = 0; counter2 < presentSymptoms; counter2++){
            //Place scanf here for symptom
        }
    }
}

void displaySymptoms(){ //change function type to the correct type, this should return a string

    printf("What is the impression?  ");
    //Place scanf here for impression

    //printf("\nSymptoms of %s are:\n", /*add string array*/);
}

void modifySymptoms(){
    displaySymptoms(); //this should return a string
    
    //this could also be connected with inputImpression
}

void doctorChoice(char choice){
    switch(choice){
        case 'C':
            inputSymptoms();
            inputImpression();
            break;
        case 'D':
            displaySymptoms();
            break;
        case 'M':
            modifySymptoms();
            break;
    }
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

        if (!(choice == 'C' || choice == 'U' || choice == 'D' || choice == 'M' || choice == 'E')) {
            printf("\n\nInvalid choice. Please try again.\n\n");
            sleepDelay(1);
        }
    } while (!(choice == 'C' || choice == 'U' || choice == 'D' || choice == 'M' || choice == 'E'));
    
    return choice;
}

int main() {

    
    

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