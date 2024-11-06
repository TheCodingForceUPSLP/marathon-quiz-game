#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STRING 256

typedef struct Question {
    char question[MAX_STRING];
    char options[3][MAX_STRING];
    int correct_answer;
    struct Question* next;
} Question;

// Function prototypes
Question* createQuestion();
void addQuestion(Question** head);
void playGame(Question* head);
void freeQuestions(Question* head);

int main(){
    int choice;
    while(1){
        printf("\n============================\n");
        printf("    MARATHON QUIZ GAME\n");
        printf("============================\n");
        printf("1. Exit\n");

        printf("Select an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                printf("Bye bye ...\n");
                return 0;
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
    }
    
}