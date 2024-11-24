#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//Const definition
#define MAX_STRING_QUESTION 256
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
    char question[MAX_STRING_QUESTION];
    char options[3][MAX_STRING_QUESTION];
    int correct_answer;
    struct Question* next;
} Question;

// Function prototypes
Question* createQuestion();
void showMenu(int *choice);
void addQuestion(Question** head);
void playGame(Question* head, Player**);
void freeQuestions(Question* head);

//Player core function prototype definition
Player* createPlayer(int,char*,float);
void insertSortedPlayer(Player**,Player*);
Player* findPlayerByNickname(Player*,char*);
void updatePlayerIfHigherScore(Player**,char*,float);
bool isNicknameInList(Player*,char*);
int getLastId(Player*,int);
void freePlayers(Player*);

//Scoring system function prototype definition
float newScore(int,int);

//Nickname creation function prototype definition
void nicknameCreation(char*);
//Function for Rankings
void PrintPlayers(Player *head);

int main(){
    Question* head = NULL;
    Player *playerHead=NULL;
    int choice;
    while(1){
        showMenu(&choice);
        switch (choice)
        {
            case 1:
                addQuestion(&head);
                break;
            case 2:
                playGame(head,&playerHead);
                break;
            
            case 3:
            PrintPlayers(playerHead);
            break;
            case 4:
                printf("\n============================\n");
                printf("    PROGRAM CREDITS\n");
                printf("============================\n");
                printf("Marathon Quiz Game\n");
                printf("Developed by E13A Group\n\n");
                break;
            case 5:
                printf("Bye bye ...\n");
                freeQuestions(head);
                freePlayers(playerHead);
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
    printf("3. Ranking Players\n");
    printf("4. Show credits\n");
    printf("5. Exit\n");

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
    fgets(newQuestion->question, MAX_STRING_QUESTION, stdin);
    newQuestion->question[strcspn(newQuestion->question, "\n")] = 0;
    
    for (int i = 0; i < 3; i++) {
        printf("Enter option %d: ", i + 1);
        fgets(newQuestion->options[i], MAX_STRING_QUESTION, stdin);
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
void playGame(Question* head,Player** playerHead) {
    int lives = 3;
    int score = 0;
    int questionNumber = 1;
    Question* current = head;
    //Player variables declaration
    Player *newPlayer=NULL;
    int id=0;
    char playerName[MAX_STRING_NICKNAME]="";
    float totalScore=0;
    //End.

    //Player creation
    //ID assignment
    if(*playerHead==NULL){
        id=1000;
    }else{
        id= getLastId(*playerHead,1000);
    }
    //End.
    nicknameCreation(playerName);
    //End.
    
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

        //Mode multipliers for final score (1 - Easy, 2 - Normal, 3 - God)
        totalScore= newScore(score,1);
        
        if (lives == 0) {
            //Player insertion into the list
            newPlayer=createPlayer(id,playerName,totalScore);
            if(!isNicknameInList(*playerHead,playerName)){
                insertSortedPlayer(playerHead,newPlayer);
            }
            updatePlayerIfHigherScore(playerHead,playerName,totalScore);
            //End.
            printf("\nGame Over! You ran out of lives.\n");
        } else if (current == NULL) {
            //Player insertion into the list
            newPlayer=createPlayer(id,playerName,totalScore);
            if(!isNicknameInList(*playerHead,playerName)){
                insertSortedPlayer(playerHead,newPlayer);
            }
            updatePlayerIfHigherScore(playerHead,playerName,totalScore);
            //End.
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

//Empty all the players of the list
void freePlayers(struct Player* head) {
    struct Player* current = head;
    while (current != NULL) {
        struct Player* temp = current;
        current = current->next;
        free(temp);
    }
}

//New Score function
float newScore(int correctAnswers, int difficultySelection){
    //Scoring system
    float score=0;
    switch(difficultySelection){
        case 1:
            score=easyModeMultiplier*correctAnswers;
            break;
        case 2:
            score=normalModeMultiplier*correctAnswers;
            break;
        case 3:
            score=godModeMultiplier*correctAnswers;
            break;
    }
    return score;
    //End.
}

//Nickname creation
void nicknameCreation(char* playerName){
    //Player creation
    //this option for reading the string obligates the user to insert a username with the correct lenght
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

//Check for repeated nicknames
bool isNicknameInList(Player* head, char* nickname){
    Player* reference = findPlayerByNickname(head,nickname);
    if(reference==NULL){
        return false;
    }else{
        return true;
    }
}

//Function to get assign the last id of the newest player
int getLastId(Player* head, int idStart){
    Player *last=head;
    int id=idStart;
    while(last->next!=NULL){
        ++id;
        last=last->next;
    }
    return ++id;
}

//Insert new player in order from the highest score to the lowest
void insertSortedPlayer(Player **head, Player *newPlayer){
    //this is the case where there is only one node or when the new score is the highest
    if(*head==NULL || (*head)->maxScore <= newPlayer->maxScore){
        newPlayer->next=*head;
        if(*head!=NULL){
            (*head)->prev=newPlayer;
        }
        *head=newPlayer;
        return;
    }

    Player *current=*head;
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

        insertSortedPlayer(head,reference);
    }
    
}
//function for rankings
    void PrintPlayers(Player *head){
    	//if the game doesnt have players then, this if action
        if(head==NULL){
            printf("there are not players.\n");
            return;
        }
		//use current like auxiliar
        Player* current = head;
        //int type for count all the player in the list
        int TotalPlayers=0;
		//scroll through the list and count each player
        while(current!=NULL){
            TotalPlayers++;
            current=current->next;
        }
		//int type for declare that in each page there will be 5 players
		int playersPerPage = 5;
		//this int type has a important utility, its function acomodate the total of pages even if there are impar numbers
    	int totalPages = ceil((float)TotalPlayers / playersPerPage);
    	//this int is for indicate our current pages, our "head"
    	int currentPage = 1;

        while(1){
            system("cls");
            
        printf("===============================\n");
        printf("PLAYER RANKINGS (PAGE %d of %d)\n",currentPage,totalPages);
        printf("===============================\n");
		
		/*start is showing us the first player in each page
		 for exmaple: page 1: start with the number 1 
		 and the page 2: start with the number 6*/
        int start = (currentPage - 1) * playersPerPage + 1;
        
		/*end indicates the last player for each page for example
        page 1: end with the number 5
        page 2: end with the number 11
        */
        int end = start + playersPerPage - 1;

        // Move to an initial position from the page
        current = head;
        
        int index = 1;
        while (current != NULL && index < start) {
            current = current->next;
            index++;
        }

        index=start;
		//Shows us the information of each player and its limit is the "end" that was calculated before 
        while (current != NULL && index <= end) {
            printf("%d. [ID: %03d] %s - %.1f pts\n", index, current->id, current->nickname, current->maxScore);
            current = current->next;
            index++;
        }
        
        printf("===============================\n");
        printf("1. Previous Page\n");
        printf("2. Next Page\n");
        printf("3. Exit to Menu\n");
        printf("Option: ");

        int option=0;
        scanf("%d", &option);

        if (option == 1 && currentPage > 1) {
            currentPage--;
        } else if (option == 2 && currentPage < totalPages) {
            currentPage++;
        } else if (option == 3) {
        	 system("cls");
            break;
        } else {
            printf("try again.\n");
        }

        }
        return;
    }
