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

typedef struct PlayedRound{
	int difficulty; 
	int playerID;
	int points;
	struct PlayedRound* next;
}PlayedRound;

// Function prototypes
Question* createQuestion();
void showMenu(int *choice);

PlayedRound* createPlayedRound(int difficulty, int playerID , int points);
void insertPlayedRound(PlayedRound **head, int difficulty, int playerID , int points);
void loadPlayedRoundsFromFile(PlayedRound** playedRoundHead);
void savePlayedRoundsToFile(PlayedRound* playedRoundHead);
void freePlayedRounds(PlayedRound* playedRoundHead);
void displayPlayedRounds(PlayedRound* playedRoundHead);
  
void addQuestion(Question** questionHead);
void playGame(Question* questionHead, Player**, PlayedRound**);
void freeQuestions(Question* questionHead);

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

int main(){
    Question* questionHead = NULL;
    Player *playerHead=NULL;
    PlayedRound* playedRoundHead = NULL;
    int choice;
    loadPlayedRoundsFromFile(&playedRoundHead);
    while(1){
        showMenu(&choice);
        switch (choice)
        {
            case 1:
                addQuestion(&questionHead);
                break;
            case 2:
                playGame(questionHead,&playerHead, &playedRoundHead);
                savePlayedRoundsToFile(playedRoundHead); // Save after playing a game
                break;
            case 3:
            	displayPlayedRounds(playedRoundHead);
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
                freeQuestions(questionHead);
                freePlayers(playerHead);
                freePlayedRounds(playedRoundHead);
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
    printf("3. Show played rounds\n");
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
void addQuestion(Question** questionHead) {
    Question* newQuestion = createQuestion();
    
    if (*questionHead == NULL) {
        *questionHead = newQuestion;
    } else {
        Question* current = *questionHead;
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
void playGame(Question* questionHead,Player** playerHead, PlayedRound **playerRound) {
    int lives;
    int score = 0;
    int questionNumber = 1;
    Question* current = questionHead;
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
    nicknameCreation(playerName);
    //End.

    // Select difficulty
    int difficulty;
    printf("\n============================\n");
    printf("   SELECT THE DIFFICULTY MODE:\n");
    printf("1. Easy Mode (5 lives)\n");
    printf("2. Normal Mode (3 lives)\n");
    printf("3. God Mode (1 life)\n");
    printf("============================\n");
    
    do {
        printf("Select an option: ");
        scanf("%d", &difficulty);
        getchar(); // Clear the newline character
        if (difficulty < 1 || difficulty > 3) {
            printf("Invalid option! Please select a number between 1 and 3.\n");
        }
    } while (difficulty < 1 || difficulty > 3);

    // Assign lives based on the difficulty
    switch (difficulty) {
        case 1:
            lives = 5;
            break;
        case 2:
            lives = 3;
            break;
        case 3:
            lives = 1;
            break;
    }
    
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
    /*
    NOTE:
    The difficulty and userID are hardcoded 
    please modify the call
    */
    insertPlayedRound(playerRound, 1, 1, score);
}

/*
Empty the questions linked list.
*/
void freeQuestions(struct Question* questionHead) {
    struct Question* current = questionHead;
    while (current != NULL) {
        struct Question* temp = current;
        current = current->next;
        free(temp);
    }
}

/*
Create the player round.
*/
PlayedRound* createPlayedRound(int difficulty, int playerID, int points){
    PlayedRound *newRound = (PlayedRound*)malloc(sizeof(PlayedRound));
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
void insertPlayedRound(PlayedRound **head, int difficulty, int playerID, int points){
	PlayedRound *newRound = createPlayedRound(difficulty, playerID, points);  
	//Go through the list and find the position to insert the node
	if(*head ==NULL || (*head)->difficulty > newRound->difficulty 
	    							&& (*head)->points < newRound->points){
		newRound->next = *head;
		*head = newRound;
		return; 
	}
	PlayedRound *current = *head;
	while(current->next !=NULL && current->next->difficulty < newRound->difficulty 
									&& current->next->points >= newRound->points){
		current = current->next;
	}
	newRound->next = current->next;
	current->next = newRound; 
	
}

//Load the records for played rounds 
void loadPlayedRoundsFromFile(PlayedRound** playedRoundHead) {
    FILE* file = fopen("playedRounds.txt", "r");
    if (file == NULL) {
        printf("Warning: File not found or could not be opened.\n");
        return;
    }
    int difficulty, playerID, points;
    while (fscanf(file, "%d|%d|%d\n", &difficulty, &playerID, &points) == 3) {
        insertPlayedRound(playedRoundHead, difficulty, playerID, points);
    }
    fclose(file);
    printf("Played rounds have been loaded successfully.\n");
}

//Save the records for played rounds 
void savePlayedRoundsToFile(PlayedRound* playedRoundHead){
    FILE* file = fopen("playedRounds.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open file for saving played rounds.\n");
        return;
    }
    PlayedRound* current = playedRoundHead;
    while (current != NULL) {
        fprintf(file, "%d|%d|%d\n", current->difficulty, current->playerID, current->points);
        current = current->next;
    }
    fclose(file);
    printf("Played rounds have been saved successfully.\n");
}

//Empty all the players rounds
void freePlayedRounds(PlayedRound* playedRoundHead){
    PlayedRound* current = playedRoundHead;
    while (current != NULL) {
        PlayedRound* temp = current;
        current = current->next;
        free(temp);
    }
}

/*
Display Played Rounds
*/
void displayPlayedRounds(PlayedRound* playedRoundHead) {
    if (playedRoundHead == NULL) {
        printf("\nNo rounds played yet.\n");
        return;
    }

    int selectedMode;
    do {
        printf("\nSelect game mode to view:\n");
        printf("1. Easy Mode\n2. Normal Mode\n3. God Mode\n");
        printf("Enter your choice (1-3): ");
        scanf("%d", &selectedMode);
    } while (selectedMode < 1 || selectedMode > 3);

    // Filter and count rounds for the selected mode
    PlayedRound* current = playedRoundHead;
    PlayedRound* filteredRounds[1000]; // Temporary array for filtered rounds
    int count = 0;

    while (current != NULL) {
        if (current->difficulty == selectedMode) {
            filteredRounds[count++] = current;
        }
        current = current->next;
    }

    if (count == 0) {
        printf("\nNo rounds found for the selected mode.\n");
        return;
    }

    // Sort the filtered rounds by score (descending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (filteredRounds[j]->points < filteredRounds[j + 1]->points) {
                PlayedRound* temp = filteredRounds[j];
                filteredRounds[j] = filteredRounds[j + 1];
                filteredRounds[j + 1] = temp;
            }
        }
    }

    // Pagination system
    int roundsPerPage = 5;
    int totalPages = ceil((float)count / roundsPerPage);
    int currentPage = 1;

    while (1) {
        system("cls");
        printf("\n==============================\n");
        printf("PLAYED ROUNDS - %s (PAGE %d of %d)\n",
               selectedMode == 1 ? "Easy Mode" : selectedMode == 2 ? "Normal Mode" : "God Mode",
               currentPage, totalPages);
        printf("==============================\n");

        int start = (currentPage - 1) * roundsPerPage;
        int end = start + roundsPerPage;

        for (int i = start; i < end && i < count; i++) {
            printf("%d. Player ID: %d | Points: %d\n", i + 1, filteredRounds[i]->playerID, filteredRounds[i]->points);
        }

        printf("==============================\n");
        printf("1. Previous Page\n2. Next Page\n3. Exit\n");
        printf("Enter your choice: ");
        
        int option;
        scanf("%d", &option);

        if (option == 1 && currentPage > 1) {
            currentPage--;
        } else if (option == 2 && currentPage < totalPages) {
            currentPage++;
        } else if (option == 3) {
            system("cls");
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
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
    return;
}
