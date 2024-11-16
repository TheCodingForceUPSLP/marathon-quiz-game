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

typedef struct playedRound{
	int difficulty; 
	int playerID;
	int points;
	struct playedRound* next;
}playedRound;

// Function prototypes
Question* createQuestion();
void showMenu(int *choice);
void addQuestion(Question** head);
void playGame(Question* head, playedRound **playerRound);
void freeQuestions(Question* head);
playedRound* createPlayedRound(int difficulty, int playerID , int points);
void insertPlayedRound(playedRound **head, playedRound* newRound);

int main(){
    Question* head = NULL;
    playedRound* playedRoundHead = NULL;
    int choice;
    while(1){
        showMenu(&choice);
        switch (choice)
        {
            case 1:
                addQuestion(&head);
                break;
            case 2:
                playGame(head, &playedRoundHead);
                break;
            case 3:
                printf("\n============================\n");
                printf("    PROGRAM CREDITS\n");
                printf("============================\n");
                printf("Marathon Quiz Game\n");
                printf("Developed by E13A Group\n\n");
                break;
            case 4:
                printf("Bye bye ...\n");
                freeQuestions(head);
                return 0;
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
    }
}

/*
Display the Main Menu.
*/
void showMenu(int *choice){
    ("\n============================\n");
    printf("    MARATHON QUIZ GAME\n");
    printf("============================\n");
    printf("1. Register new question\n");
    printf("2. Play game\n");
    printf("3. Show credits\n");
    printf("4. Exit\n");

    printf("Select an option: ");
    scanf("%d", choice);
    getchar();
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

/*
Display specific question with options
*/
void displayQuestion(Question* q, int* questionNumber) {
    printf("\nQuestion %d: %s\n", *questionNumber, q->question);
    for (int i = 0; i < 3; i++) {
        printf("%d. %s\n", i + 1, q->options[i]);
    }
}

/*
Logic for marathon game
*/
void playGame(Question* head, playedRound **playerRound) {
    int lives = 3;
    int score = 0;
    int questionNumber = 1;
    Question* current = head;
    
    printf("\nGame started! You have %d lives.\n", lives);
    
    while (current != NULL && lives > 0) {
        displayQuestion(current, &questionNumber);
        
        int answer;
        printf("Your answer (1-3): ");
        scanf("%d", &answer);
        
        if (answer == current->correct_answer) {
            printf("Correct!\n");
            score++;
        } else {
            lives--;
            printf("Wrong! Lives remaining: %d\n", lives);
        }
        
        current = current->next;
        questionNumber++;
        
        if (lives == 0) {
            printf("\nGame Over! You ran out of lives.\n");
        } else if (current == NULL) {
            printf("\nCongratulations! You completed all questions!\n");
        }
    }
    
    printf("Final score: %d\n", score);
    /*
    NOTE:
    The userID is hardcoded 
    please modify the call
    */
    insertPlayedRound(playerRound, 1, score);
}

/*
Empty the questions linked list.
*/
void freeQuestions(struct Question* head) {
    struct Question* current = head;
    while (current != NULL) {
        struct Question* temp = current;
        current = current->next;
        free(temp);
    }
}

/*
Create the player round.
*/
playedRound* createPlayedRound(int difficulty, int playerID, int points){
    playedRound *newRound = (playedRound*)malloc(sizeof(playedRound));
	if (newRound == NULL) {
        printf("ERROR\n");
        return NULL;
    }
	newRound->difficulty = difficulty;
	newRound->playerID = playerID; 
	newRound->points = points;
	newRound->next = NULL; 
	
	return newRound;
}

/*
Insert player round sorted by difficulty (1,2,3) and the points (ascending order).
*/
void insertPlayedRound(playedRound **head, playedRound* newRound){
	playedRound *newRound = createPlayedRound(difficulty, playerID, points);  
	//Go through the list and find the position to insert the node
	if(*head ==NULL || (*head)->newRound->difficulty > newRound->difficulty 
	    							&& (*head)->newRound->points < newRound->points){
		newRound->next = *head;
		*head = newRound;
		return; 
	}
	playedRound *current = *head;
	while(current->next !=NULL && current->next->difficulty < newRound->difficulty 
									&& current->next->points >= newRound->points){
		current = current->next;
	}
	newRound->next = current->next;
	current->next = newRound; 
	
}

	

