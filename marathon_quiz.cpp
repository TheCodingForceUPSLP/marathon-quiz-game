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
    Question* head = NULL;
    int choice;
    while(1){
        printf("\n============================\n");
        printf("    MARATHON QUIZ GAME\n");
        printf("============================\n");
        printf("1. Register new question\n");
        printf("2. Exit\n");

        printf("Select an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                addQuestion(&head);
                break;
            case 2:
                printf("Bye bye ...\n");
                return 0;
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
    }
}

/*
Create a question in memory.
*/
Question* createQuestion() {
    Question* newQuestion = (Question*)malloc(sizeof(Question));
    if (newQuestion == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    
    printf("\nEnter the question: ");
    fgets(newQuestion->question, MAX_STRING, stdin);
    newQuestion->question[strcspn(newQuestion->question, "\n")] = 0;
    
    for (int i = 0; i < 3; i++) {
        printf("Enter option %d: ", i + 1);
        fgets(newQuestion->options[i], MAX_STRING, stdin);
        newQuestion->options[i][strcspn(newQuestion->options[i], "\n")] = 0;
    }
    
    do {
        printf("Enter the correct answer (1, 2, or 3): ");
        scanf("%d", &newQuestion->correct_answer);
        getchar();
    } while (newQuestion->correct_answer < 1 || newQuestion->correct_answer > 3);
    
    newQuestion->next = NULL;
    return newQuestion;
}

/*
Add a question into the simple linked list
*/
void addQuestion(Question** head) {
    Question* newQuestion = createQuestion();
    
    if (*head == NULL) {
        *head = newQuestion;
    } else {
        Question* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newQuestion;
    }
    printf("\nQuestion added successfully!\n");
}