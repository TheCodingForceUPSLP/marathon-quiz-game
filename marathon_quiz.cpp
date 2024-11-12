#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Const definition
#define MAX_STRING 256
#define MAX_STRING_NICKNAME 30
//Scoring criteria const definition
#define easyModeMultiplier 0.5
#define normalModeMultiplier 1
#define godModeMultiplier 2

//Player struct definition
typedef struct Player{
	int id;
	char nickname[MAX_STRING_NICKNAME];
	float maxScore;
	struct Player* next;
	struct Player* prev;
}Player;

typedef struct Question {
    char question[MAX_STRING];
    char options[3][MAX_STRING];
    int correct_answer;
    struct Question* next;
} Question;

// Function prototypes
Question* createQuestion();
void showMenu(int *choice);
void addQuestion(Question** head);
void playGame(Question* head);
void freeQuestions(Question* head);

//Player core function definition
Player* createPlayer(int, char*, float);
void insertSortedPlayer(Player**, Player*, bool&);
Player* findPlayerByNickname(Player*, char*);
void updatePlayerIfHigherScore(Player**,char*,float);

int main(){
    Question* head = NULL;
    int choice;
    while(1){
        showMenu(&choice);
        switch (choice)
        {
            case 1:
                addQuestion(&head);
                break;
            case 2:
                playGame(head);
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

    //Player creation section start... Will work when implemented, right now code is unreachable

    //Variable definition for general functioning
    Player *playerHead=NULL;
    Player *newPlayer=NULL;
    bool inserted=true;
    int id=1000;
    //End.

    //Scoring system
    float score=0;
    switch(1){
        case 1:
            score=easyModeMultiplier*1;
        break;
        case 2:
            score=normalModeMultiplier*1;
        break;
        case 3:
            score=godModeMultiplier*1;
        break;
    }
    //End.

    //Nickname reading options, all of them work, just depends on what you like the most :)
    char playerName[MAX_STRING_NICKNAME]="";

    //this option for reading the string obligates the user to insert a username with the correct length
    bool correctLength=true;
    do{
        fflush(stdin);
        correctLength=true;
        printf("Enter your nickname: ");
        scanf(" %[^\n]",playerName);
        if(strlen(playerName)>MAX_STRING_NICKNAME){
            printf("\nName is too long\n");
            correctLength=false;
        }
    }while(!correctLength);
    //End.

    //this option limits the length of the string in spite of how long the input is
    //printf("Enter your nickname: ");
    //fgets(playerName,MAX_STRING_NICKNAME,stdin);
    //strtok(playerName, "\n");
    //End.

    //simple way to read the string, no limiters
    //scanf(" %[^\n]",playerName);
    //End.

    //End.

    //Player creation
    newPlayer=createPlayer(id,playerName,score);
    insertSortedPlayer(&playerHead,newPlayer,inserted);
    if(inserted){
        ++id;
    }
    updatePlayerIfHigherScore(&playerHead,playerName,score);
    //End.
    
    //Memmory freeing
    free(playerHead);
    //End.
    return 0;
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
void playGame(Question* head) {
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

//Functions for the main process for player creation

//Node creation
Player* createPlayer(int id, char *nickname, float score){
    Player *newPlayer=(Player*)malloc(sizeof(Player));
    newPlayer->id=id;
    newPlayer->maxScore=score;
    strcpy(newPlayer->nickname,nickname);
    newPlayer->next=NULL;
    newPlayer->prev=NULL;
    return newPlayer;
}

//Insert new player in order from the highest score to the lowest
void insertSortedPlayer(Player **head, Player *newPlayer, bool &inserted){
    //Check for repeated nicknames
    Player *current=*head;
    if(current!=NULL){
        do{
            if(strcmp(newPlayer->nickname,current->nickname)==0){
                //Condition test
                //printf("\nNickname already exists\n");
                //system("pause");
                //End.
                inserted=false;
                return;
            }
            current=current->next;
        }while(current!=NULL);
    }
    //End.
    inserted=true;
    //this is the case where there is only one node or when the new score is the highest
    if(*head==NULL || (*head)->maxScore <= newPlayer->maxScore){
        newPlayer->next=*head;
        if(*head!=NULL){
            (*head)->prev=newPlayer;
        }
        *head=newPlayer;
        return;
    }

    current = *head;
    //case for the first insertion after the main node
    if(current->next==NULL){
        current->next = newPlayer;
        newPlayer->prev=current;
        return;
    }

    //cicle that runs until the next value is smaller than the value being compared
    while(current->next!=NULL && current->next->maxScore > newPlayer->maxScore){
        current=current->next;
    }

    //insertion of the relocated node
    newPlayer->next=current->next;
    newPlayer->prev=current;
    if(current->next!=NULL){
        current->next->prev=newPlayer;
    }
    current->next=newPlayer;
}

//Function to search for players by using the nickname as a reference
Player* findPlayerByNickname(Player *head, char *nickname){
    Player* current=head;
    while(current!=NULL){
        if(strcmp(current->nickname,nickname)==0){
            return current;
        }
        current=current->next;
    }
    return NULL;
}

//This function will go into effect when the new score is higher and the player needs to be rearranged
void updatePlayerIfHigherScore(Player **head, char *nickname, float newScore){
    //reference adress for locating the data needed
    Player* reference= findPlayerByNickname(*head,nickname);
    //in case the player doesn't exist
    if(reference==NULL){
        printf("\n Player doesn't exist");
        return;
    }
    //initial condition for the sorting algorithm, the new score must be higher than the previous score, if not, do nothing
    if(newScore>reference->maxScore){
        //the new score is stored
        reference->maxScore=newScore;

        //pointers for sorting the nodes
        Player* current=*head;
        Player* prevPlayer=reference->prev;
        Player* nextPlayer=reference->next;
        //End of pointers

        //this indicates that the node is the first one of the list, no sorting must be made
        if(prevPlayer==NULL){
            return;
        }

        //pointer redirectioning for the list no to get cropped out
        prevPlayer->next=nextPlayer;
        if(nextPlayer!=NULL){
            nextPlayer->prev=prevPlayer;
        }

        bool inserted=true;

        insertSortedPlayer(head,reference,inserted);
    }
    return;
}