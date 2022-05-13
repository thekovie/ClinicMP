/**
 * Description:       This is a console-based similar program that can generate a list of clinical impressions 
 *                    along with other patient information in a summary in the form of history of present illness (HPI).
 * 
 * Programmed by:     John Kovie L. Niño
 *                    Reign Elaiza D. Larraquel
 * 
 * Last modified:     May 13, 2022
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


/**
 * If the program is running on Windows, then call the Windows Sleep function. Otherwise, call the Unix
 * sleep function
 */
void sleepDelay () {
    #if defined(_WIN32)
        Sleep(1000);
    #else
        sleep(1);
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

char userType(char choice) {
    do {
        printf("%s\n\n%s\n%s\n%s\n\n%s",
        "MENU #1 User Type",
        "D for Doctor",
        "P for Patient",
        "E for Exit",
        "Enter your choice: ");
        
        scanf(" %c", &choice);

        if (!(choice == 'D' && choice == 'P' && choice == 'E')) {
            printf("\n\nInvalid choice. Please try again.\n\n");
            sleepDelay();
        }
    } while (!(choice == 'D' && choice == 'P' && choice == 'E'));

    return choice;
}

char doctorMenu(char choice) {
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

        if (!(choice == 'C' && choice == 'U' && choice == 'D' && choice == 'M' && choice == 'E')) {
            printf("\n\nInvalid choice. Please try again.\n\n");
            sleepDelay();
        }
    } while (!(choice == 'C' && choice == 'U' && choice == 'D' && choice == 'M' && choice == 'E'));
    
    return choice;
}

int main() {
    

    return 0;
}
