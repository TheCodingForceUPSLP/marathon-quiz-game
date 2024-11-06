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