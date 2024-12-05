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
//array of category names
const char* categoryNames[] = {
    "Mexican History",
    "General History",
    "Geography",
    "Sports, Movies, Books, Art",
    "Science and Biology",
    "Spanish, Philosophy and Religion"
};

//Player struct definition
typedef struct Player{
	int id;
	char nickname[MAX_STRING_NICKNAME];
	float maxScore;
	struct Player* next;
	struct Player* prev;
}Player;

typedef struct Question {
	int id;
	int category;
	char question[MAX_STRING_QUESTION];
	char options[3][MAX_STRING_QUESTION];
	int correct_answer;
	int wrongCount;
	struct Question* next;
} Question;

typedef struct PlayedRound{
	int difficulty; 
	int playerID;
	int points;
	struct PlayedRound* next;
}PlayedRound;

// Definition of the doubly linked list node structure
typedef struct WrongAnswer {
    int IdQuestion;               // Stores the question number, like 1, 2, 3...x
    char Question[MAX_STRING_QUESTION];           // Stores the wrong question from the main question node
    char wrongAnswer[MAX_STRING_QUESTION];        // Stores the wrong answer (A, B, C, D, and the answer)
    char correctAnswer[MAX_STRING_QUESTION];      // Stores the correct answer from the main question node
    struct WrongAnswer* next;     // Points to the next node
    struct WrongAnswer* prev;     // Points to the previous node
} WrongAnswer;

// Function prototypes
Question* createQuestion(int questionId);
Question* searchQuestion(Question *questionHead, int questionId);
int getLastQuestionId(Question* questionHead, int idStart);
void deleteQuestionById(Question **questionHead, int questionId);
void modifyQuestionById(Question* questionHead, int id, int category);
void loadQuestionsFromFile(Question** questionHead);
void saveQuestionsToFile(Question* questionHead);
void showMenu(int *choice);

PlayedRound* createPlayedRound(int difficulty, int playerID , int points);
void insertPlayedRound(PlayedRound **head, int difficulty, int playerID , int points);
void addQuestion(Question** questionHead);
void playGame(Question* questionHead, Player**, PlayedRound**, WrongAnswer**);
void freeQuestions(Question* questionHead);

//Wrong Answers core function prototype definition
void InsertWrongAnswer(WrongAnswer** wrongAnswerHead, int id, Question* questionHead, int wrong, int correct);
void freeListWrongAnswers(WrongAnswer* wrongAnswerHead);
void updateWrongCount(Question* questionHead, int questionId);
int getWrongQuestionCount(WrongAnswer* wrongAnswerHead);
void calculateErrorPercentage(Question* questionHead, int totalQuestions, float *topErrorPercentages, int *topErrorQuestionIds);
void displayTop5FailedQuestions(Question* questionHead, float *topErrorPercentages, int *topErrorQuestionIds);
void displayBottom5Scores(Player* playerHead, Player* currentPlayer);
void saveWrongAnswersToFile(WrongAnswer* wrongAnswerHead);
void loadWrongAnswersFromFile(WrongAnswer** wrongAnswerHead);

//Player core function prototype definition
Player* findPlayerByNickname(Player*,char*);
Player* createPlayer(int,char*,float);
void insertSortedPlayer(Player**,Player*);
void updatePlayerIfHigherScore(Player**,char*,float);
void freePlayers(Player*);
void deletePlayer(Player** head);
void changeName(Player* head);
bool isNicknameInList(Player*,char*);
int getLastId(Player*,int);

//Functions to handle players file
void savePlayersToFile(Player* playerHead);
void loadPlayersFromFile(Player** playerHead);

//Scoring system function prototype definition
float newScore(int,int);

//Nickname creation function prototype definition
void nicknameCreation(char*);

//Funciton for display quetions page
void displayQuestionsInPages(Question* questionHead);

//Function for display the rankings
void printPlayers(Player *head);
//Category functions
void categoryMenu(int *category);
void displayQuestionsByCategory(Question* head, int category);

int main(){
    WrongAnswer* wrongAnswerHead = NULL;
    Question* questionHead = NULL;
    Player *playerHead=NULL;
    PlayedRound* playedRoundHead = NULL;
    
    int choice;
    int questionId = 0;
    int category= 0;

    int totalWrongQuestions;
    float topErrorPercentages[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    int topErrorQuestionIds[5] = {-1, -1, -1, -1, -1};

    loadPlayersFromFile(&playerHead);
    loadQuestionsFromFile(&questionHead);
    loadWrongAnswersFromFile(&wrongAnswerHead);
    while(1){
        showMenu(&choice);
        switch (choice)
        {
            case 1:
                addQuestion(&questionHead);
                saveQuestionsToFile(questionHead);
                break;
            case 2:
                playGame(questionHead,&playerHead, &playedRoundHead, &wrongAnswerHead);
                savePlayersToFile(playerHead);
                saveQuestionsToFile(questionHead);
                saveWrongAnswersToFile(wrongAnswerHead);
                break;
            case 3:
            	printPlayers(playerHead);
            	break;	
            case 4:
                changeName(playerHead);
                break;
            case 5:
                deletePlayer(&playerHead);        
                break;
            case 6: 
		        printf("\nEnter the id of the question to modify: ");
            	scanf("%d",&questionId);
		        modifyQuestionById(questionHead, questionId, category);
		        saveQuestionsToFile(questionHead);
            	break;
        
            case 7:
            	printf("\nEnter the id of the question to delete: ");
            	scanf("%d",&questionId);
            	deleteQuestionById(&questionHead, questionId);
            	saveQuestionsToFile(questionHead);
            	break;
            case 8:
            	categoryMenu(&category);
            	displayQuestionsByCategory(questionHead, category);
            	break;
            case 9:
            	displayQuestionsInPages(questionHead);  
				break;          	
            case 10:
                printf("\n============================\n");
                printf(" Top 5 Hardest Questions \n");
                printf("============================\n");
                totalWrongQuestions = getWrongQuestionCount(wrongAnswerHead);
                calculateErrorPercentage(questionHead, totalWrongQuestions, 
                                            topErrorPercentages, topErrorQuestionIds); 
                displayTop5FailedQuestions(questionHead,topErrorPercentages, topErrorQuestionIds);
                break;
            case 11:
                printf("\n============================\n");
                printf("    PROGRAM CREDITS\n");
                printf("============================\n");
                printf("Marathon Quiz Game\n");
                printf("Developed by E13A Group\n\n");
                break;
            case 12:
                printf("Bye bye ...\n");
                freeQuestions(questionHead);
                freePlayers(playerHead);
                freeListWrongAnswers(wrongAnswerHead);
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
    printf("\n============================\n");
    printf("    MARATHON QUIZ GAME\n");
    printf("============================\n");
    printf("1. Register new question\n");
    printf("2. Play game\n");
    printf("3. Display ranking players\n");   
    printf("4. Rename player\n");
    printf("5. Delete player\n");
    printf("6. Modify question by Id\n");
    printf("7. Delete question\n");
    printf("8. Display questions by category\n");
    printf("9. Display questions by page\n");
    printf("10. Display Top 5 Failed Questions\n");
    printf("11. Show credits\n");
    printf("12. Exit\n");

    printf("Select an option: ");
    scanf("%d", choice);
    getchar();
}
/*
    Display the category selection menu
*/
void categoryMenu(int *category){
    printf("\n1. Mexican History\n");
    printf("2. General History\n");
    printf("3. Geography\n");
    printf("4. Sports, Movies, Books, Art\n");
    printf("5. Science and Biology\n");
    printf("6. Spanish, Philosophy and Religion\n");
    
    do{
        printf("Select a category (1-6): ");
        scanf("%d", category); 
		while(getchar() != '\n');
    }while(*category < 1 || *category > 6); 
}
/*
Create a question in memory.
*/
Question* createQuestion(int questionId) {
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
    
    int category;
    categoryMenu(&category);
    newQuestion->category=category;
    
    newQuestion->wrongCount=0;
    newQuestion->id = questionId;
    newQuestion->next = NULL;
    return newQuestion;
}

/*
Search for specific question by id
*/
Question* searchQuestion(Question *questionHead, int questionId){
    if (questionHead == NULL) return NULL;

    Question *aux = questionHead;

    while (aux != NULL) {
        if (aux->id == questionId) return aux;
        aux = aux->next;
    }

    printf("ID %d entered is not found\n", questionId);
    return NULL;
}

/*
Delete specific question by id
*/
void deleteQuestionById(Question **questionHead, int questionId) {
    if (*questionHead == NULL) return;

    Question *current = *questionHead;
    Question *previous = NULL;

    // Searches for the node with the specified ID
    current = searchQuestion(*questionHead, questionId);
    
    // If the ID is not found, terminate
    if (current == NULL){
    	printf("\nThe question with the entered id was not found\n");
    	return;
	}
    
    current = *questionHead;
    
    while (current->id != questionId) {
        previous = current;
        current = current->next;
    }

    // If the node to be deleted is the first node to be deleted
    if (previous == NULL) {
        *questionHead = current->next;
    } else {
        previous->next = current->next;
    }

    // Frees the memory of the deleted node
    free(current);
    
    printf("\nThe question was correctly deleted");
}

// Modify question by ID
void modifyQuestionById(Question* questionHead, int id, int category) {
    if (questionHead == NULL) {
        printf("The question list is empty.\n");
        return;
    }

    // Search question by ID
    Question* current = searchQuestion(questionHead, id);
    if (current == NULL) {
        printf("Question with ID %d not found.\n", id);
        return;
    }
    printf("\nModifying question with ID %d...\n", id);

    // Modify
    printf("Enter new question: ");
    getchar();
    fgets(current->question, MAX_STRING_QUESTION, stdin);
    current->question[strcspn(current->question, "\n")] = 0; 

    // Refresh options
    for (int i = 0; i < 3; i++) {
        printf("Enter option %d: ", i + 1);
        fgets(current->options[i], MAX_STRING_QUESTION, stdin);
        current->options[i][strcspn(current->options[i], "\n")] = 0; 
    }

    // check correct answer
    do {
        printf("Enter correct answer (1-3): ");
        scanf("%d", &current->correct_answer);
        getchar(); // clean buffer
    } while (current->correct_answer < 1 || current->correct_answer > 3);

    // Refresh category
    categoryMenu(&category);
    current->category=category;
    printf("Question modified successfully.\n");
}

/*
Function to get assign the last id of the newest question
*/
int getLastQuestionId(Question* questionHead, int idStart){
	if(questionHead == NULL) return idStart;
	Question *last=questionHead;
    int id=idStart;
    while(last->next!=NULL){
        ++id;
        last=last->next;
    }
    return ++id;
}

/*
Add a question into the simple linked list
*/
void addQuestion(Question** questionHead) {
	int questionId = getLastQuestionId(*questionHead, 1);
    Question* newQuestion = createQuestion(questionId);
    
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
Save questions to a .txt file
*/
void saveQuestionsToFile(Question* questionHead) {
    FILE* file = fopen("questions.txt", "w");
    if (file == NULL) {
        printf("Error: File not found.\n");
        return;
    }

    Question* current = questionHead;
    while (current != NULL) {
        fprintf(file, "%d|%s|%s|%s|%s|%d|%d|%d\n",
                current->id,
                current->question,
                current->options[0],
                current->options[1],
                current->options[2],
                current->correct_answer,
                current->category,
				current->wrongCount);
                
        current = current->next;
    }
    fclose(file);
}

/*
Load questions from a .txt file
*/
void loadQuestionsFromFile(Question** questionHead) {
    FILE* file = fopen("questions.txt", "r");
    if (file == NULL) {
        printf("The file 'questions.txt' does not exist. A new one will be created when questions are saved.\n");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        Question* newQuestion = (Question*)malloc(sizeof(Question));
        if (newQuestion == NULL) {
            printf("Error: Could not allocate memory while loading questions.\n");
            fclose(file);
            return;
        }

        // Parse the line from the file
        if (sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%d",
                &newQuestion->id,
                newQuestion->question,
                newQuestion->options[0],
                newQuestion->options[1],
                newQuestion->options[2],
                &newQuestion->correct_answer,
                &newQuestion->category,
				&newQuestion->wrongCount) != 8) {
            printf("Warning: Invalid format in line: %s", line);
            free(newQuestion);
            continue;
        }

        // Add the question to the list
        newQuestion->next = *questionHead;
        *questionHead = newQuestion;
    }

    fclose(file);
}

/*
Add Update Wrong Count by Question Id
*/
void updateWrongCount(Question* questionHead,int questionId){
	Question *auxQuestion = searchQuestion(questionHead, questionId);
    if (auxQuestion == NULL) {
        return;
    }
	auxQuestion->wrongCount += 1;
}

/*
Display specific question with options
*/
void displayQuestion(Question* q, int* questionNumber) {
    printf("\nCategory:  %s\n", categoryNames[(q->category)-1]);
    printf("\nQuestion %d: %s\n", *questionNumber, q->question);
    for (int i = 0; i < 3; i++) {
        printf("%d. %s\n", i + 1, q->options[i]);
    }
}
/*
Display question by specific category
*/
void displayQuestionsByCategory(Question* head, int category){
	Question * current= head;
	int found=0;
	while (current != NULL){
		if(current->category==category){
		    printf("\n-> %s\n", current->question);
		    found=1;
		}
		current= current->next;
	}
	if(found!=1) printf("\nThere are no questions in this category\n" );
	printf("\n");
}

/*
Display questions page
*/
void displayQuestionsInPages(Question* questionHead) {
    if (questionHead == NULL) {
        printf("No questions to display.\n");
        return;
    }

    int currentPage = 1;
    int questionsPerPage = 5;
    Question* current = questionHead;
    
    while (1) {
        int totalQuestions = 0;
        Question* temp = current;

        // Calculate total number of questions
        while (temp != NULL) {
            totalQuestions++;
            temp = temp->next;
        }

        // Calculate total pages
        int totalPages = ceil((float)totalQuestions / questionsPerPage);


        if (currentPage > totalPages) {
            currentPage = totalPages;
        }

        // Display questions for the current page
        system("cls");
        printf("===============================\n");
        printf("QUESTION LIST (PAGE %d of %d)\n", currentPage, totalPages);
        printf("===============================\n");

        int start = (currentPage - 1) * questionsPerPage + 1;
        int end = start + questionsPerPage - 1;
        int index = 1;

        
        temp = questionHead;
        int displayCount = 0;

        // Traverse to the starting question for the current page
        while (temp != NULL && index < start) {
            temp = temp->next;
            index++;
        }

        // Show questions on current page
        while (temp != NULL && displayCount < questionsPerPage) {
            printf("Question ID: %d:\n", temp->id);
            printf("Category: %s\n", categoryNames[temp->category - 1]);
            printf("Question: %s\n", temp->question);
            for (int i = 0; i < 3; i++) {
                printf("Option %d: %s\n", i + 1, temp->options[i]);
            }
            printf("Correct Answer: Option %d: %s\n", temp->correct_answer, temp->options[temp->correct_answer - 1]);
            printf("Wrong Answers count: %d\n", temp->wrongCount);
            printf("\n");

            temp = temp->next;
            displayCount++;
        }
		
		// Options menu for page or exit
        printf("===============================\n");
        printf("1. Previous Page\n");
        printf("2. Next Page\n");
        printf("3. Exit to Menu\n");
        printf("Option: ");
        int option = 0;
        scanf("%d", &option);

        if (option == 1) { // Previous page
            if (currentPage > 1) {
                currentPage--;
            } else {
                printf("Already on the first page.\n");
            }
        } else if (option == 2) { // Next page
            if (currentPage < totalPages) {
                currentPage++;
            } else {
                printf("Already on the last page.\n");
            }
        } else if (option == 3) { // Exit
            break;
        } else {
            printf("Invalid option. Please try again.\n");
        }
    }
}

/*
Logic for marathon game
*/
void playGame(Question* questionHead,Player** playerHead, PlayedRound **playerRound, WrongAnswer** wrongAnswerHead) {
    int lives = 3;
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
                                            //id 1 to x... questions  get 1 to 3    the correct answer
            InsertWrongAnswer(wrongAnswerHead, questionNumber, current, answer, current->correct_answer);
            updateWrongCount(questionHead,current->id);
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

            // Function to display the bottom 5 scores
            displayBottom5Scores(*playerHead, newPlayer);
        } else if (current == NULL) {
            //Player insertion into the list
            newPlayer=createPlayer(id,playerName,totalScore);
            if(!isNicknameInList(*playerHead,playerName)){
                insertSortedPlayer(playerHead,newPlayer);
            }
            updatePlayerIfHigherScore(playerHead,playerName,totalScore);
            //End.
            printf("\nCongratulations! You completed all questions!\n");
            
            // Function to display the bottom 5 scores
            displayBottom5Scores(*playerHead, newPlayer);
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


//Empty all the players of the list
void freePlayers(struct Player* playerHead) {
    struct Player* current = playerHead;
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
bool isNicknameInList(Player* playerHead, char* nickname){
    Player* reference = findPlayerByNickname(playerHead,nickname);
    if(reference==NULL){
        return false;
    }else{
        return true;
    }
}

//Function to get assign the last id of the newest player
int getLastId(Player* playerHead, int idStart){
    Player *last=playerHead;
    int id=idStart;
    while(last->next!=NULL){
        ++id;
        last=last->next;
    }
    return ++id;
}

//Insert new player in order from the highest score to the lowest
void insertSortedPlayer(Player **playerHead, Player *newPlayer){
    //this is the case where there is only one node or when the new score is the highest
    if(*playerHead==NULL || (*playerHead)->maxScore <= newPlayer->maxScore){
        newPlayer->next=*playerHead;
        if(*playerHead!=NULL){
            (*playerHead)->prev=newPlayer;
        }
        *playerHead=newPlayer;
        return;
    }

    Player *current=*playerHead;
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
Player* findPlayerByNickname(Player *playerHead, char *nickname){
    Player* current=playerHead;
    while(current!=NULL){
        if(strcmp(current->nickname,nickname)==0){
            return current;
        }
        current=current->next;
    }
    return NULL;
}

//This function will go into effect when the new score is higher and the player needs to be rearranged
void updatePlayerIfHigherScore(Player **playerHead, char *nickname, float newScore){
    //reference adress for locating the data needed
    Player* reference= findPlayerByNickname(*playerHead,nickname);
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

        insertSortedPlayer(playerHead,reference);
    }
    return;
}

void deletePlayer(Player** head){
    int playerId;

    if (*head == NULL) {
        printf("There's no player to delete\n");
        return;
    }

    Player* current = *head;

    // Prompt user for the ID of the player to delete
    printf("What's the id of the poor soul you want to delete?\n");
    scanf("%i", &playerId);

    while (current != NULL) {
        if (current->id == playerId) {
            
            if (current == *head) {
                *head = current->next;
                if (*head != NULL) {
                    (*head)->prev = NULL;
                }
            } else {
                if (current->prev != NULL) {
                    current->prev->next = current->next;
                }
                if (current->next != NULL) {
                    current->next->prev = current->prev;
                }
            }
            
            free(current);
            printf("The player %i was successfully deleted. RIP.\n", playerId);
            return;
        }
        current = current->next;
    }

    // If no player with the given ID was found
    printf("No player with ID %i found.\n", playerId);
}

void changeName(Player* head) {
    int playerId;
    char nickname[MAX_STRING_NICKNAME];
   //checks if theres already players
    if (head == NULL) {
        printf("There's no player to rename\n");
        return;
    }

    Player* current = head;
	//catchs the id 
    printf("Gimme the id of the one who wants a fresh new name\n ");
    scanf("%d", &playerId);

    //looks for the id in the list until its empty or finds the name
    while (current != NULL && current->id != playerId) {
        current = current->next;
    }

    //if the id doesnt exist exits
    if (current == NULL) {
        printf("There's no such a player with the ID: %d\n", playerId);
        return;
    }
    //if the id exist now it asks you the name
    nicknameCreation(nickname);
 	//this parts lets copy an especific number of characters and lets space for the NULL character
    strcpy( current->nickname,nickname);

    printf("The person with the ID %d will be known as %s now\n", playerId, current->nickname);
}

//function for rankings
void printPlayers(Player *head){
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

/*
Save the players to a file with | as the separator.
*/
void savePlayersToFile(Player* playerHead) {
    FILE* file = fopen("players.txt", "w");
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }

    Player* current = playerHead;
    while (current != NULL) {
        // Write player data in the format: id|nickname|maxScore
        fprintf(file, "%d|%s|%.2f\n", current->id, current->nickname, current->maxScore);
        current = current->next;
    }

    fclose(file);
    printf("Players saved successfully to 'players.txt'.\n");
}

/*
Load the players from a file with | as the separator.
*/
void loadPlayersFromFile(Player** playerHead) {
    FILE* file = fopen("players.txt", "r");
    if (file == NULL) {
        perror("Failed to open file for reading");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char nickname[MAX_STRING_NICKNAME];
        float maxScore;

        // Parse the line using | as the delimiter
        if (sscanf(line, "%d|%[^|]|%f", &id, nickname, &maxScore) == 3) {
            Player* newPlayer = createPlayer(id, nickname, maxScore);
            insertSortedPlayer(playerHead, newPlayer);
        } else {
            printf("Invalid format in line: %s", line);
        }
    }

    fclose(file);
    printf("Players loaded successfully from 'players.txt'.\n");

}

void InsertWrongAnswer(WrongAnswer** wrongAnswerHead, int id, Question* questionHead, int wrong, int correct) {
    // Search for the question node using the given ID
    Question* questionNode = searchQuestion(questionHead, id);
    if (questionNode == NULL) {
        printf("Question with ID %d not found.\n", id);
        return;
    }

    // Allocate memory for the new wrongAnswer node
    WrongAnswer* newNode = (WrongAnswer*)malloc(sizeof(WrongAnswer));
    if (!newNode) { // Check if memory allocation was successful
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }


    // Assign values to the new node
    newNode->IdQuestion = id;
    strcpy(newNode->Question, questionNode->question); // Copy the question text

    // Validate the index of the wrong answer
    if (wrong < 1 || wrong > 3) {
        printf("Invalid wrong answer index: %d. Must be between 1 and 3.\n", wrong);
        free(newNode);
        return;
    }
    // Copy the wrong answer option
    strcpy(newNode->wrongAnswer, questionNode->options[wrong - 1]);

    // Validate the index of the correct answer
    if (correct < 1 || correct > 3) {
        printf("Invalid correct answer index: %d. Must be between 1 and 3.\n", correct);
        free(newNode);
        return;
    }
    // Copy the correct answer option
    strcpy(newNode->correctAnswer, questionNode->options[correct - 1]);

    // Set the pointers for the new node
    newNode->next = NULL;
    newNode->prev = NULL;

    // Insert the new node at the end of the list
    if (*wrongAnswerHead == NULL) { // If the list is empty
        *wrongAnswerHead = newNode;
    } else {
        WrongAnswer* temp = *wrongAnswerHead;
        // Traverse to the last node in the list
        while (temp->next) {
            temp = temp->next;
        }
        // Link the new node at the end
        temp->next = newNode;
        newNode->prev = temp;
    }
}

void freeListWrongAnswers(WrongAnswer* wrongAnswerHead) {
    WrongAnswer* temp;
    while (wrongAnswerHead) {
        temp = wrongAnswerHead;
        wrongAnswerHead = wrongAnswerHead->next;
        free(temp);  // Free the memory of the node
    }
}

int getWrongQuestionCount(WrongAnswer* wrongAnswerHead){
    int totalQuestions = 0;
    WrongAnswer* tempWrongQuestion = wrongAnswerHead;

    while (tempWrongQuestion) {
        totalQuestions++;
        tempWrongQuestion = tempWrongQuestion->next;
    }

    return totalQuestions;
}

void calculateErrorPercentage(Question* questionHead, int totalQuestions, float *topErrorPercentages, int *topErrorQuestionIds){
    Question* tempQuestion = questionHead;
    float currentErrorPercentage;
    int currentIdQuestion;
    while (tempQuestion) {
        currentErrorPercentage = ((float)tempQuestion->wrongCount / (float)totalQuestions);
        currentIdQuestion = tempQuestion->id;
        //check if this percentage belongs to top 5
        for(int i = 0; i < 5; i++){
            if(currentErrorPercentage > topErrorPercentages[i]) {
                //shift lower percentages down
                for(int j = 4; j > i; j--){
                    topErrorPercentages[j] = topErrorPercentages[j - 1];
                    topErrorQuestionIds[j] = topErrorQuestionIds[j - 1];
                }
                //insert the new top percentage
                topErrorPercentages[i] = currentErrorPercentage;
                topErrorQuestionIds[i] = currentIdQuestion;
                break;
            }
        }
        tempQuestion = tempQuestion->next;
    }
}

void displayTop5FailedQuestions(Question* questionHead, float *topErrorPercentages, int *topErrorQuestionIds){
    for(int i=0; i < 5; i++){
        if(topErrorPercentages[i] > 0){
            Question *tempQuestion = questionHead;
            while(tempQuestion){
                if(tempQuestion->id == topErrorQuestionIds[i]){
                    printf("Question ID: %d\n", tempQuestion->id);
                    printf("Question: %s\n", tempQuestion->question);
                    printf("Error Percentage: %.2f%%\n", topErrorPercentages[i]*100);
                    printf("----------------------------\n");
                    break;
                }
                tempQuestion = tempQuestion->next;
            }
        }
    }
}

/*
Save the WrongAnswers to a file with | as the separator.
*/
void saveWrongAnswersToFile(WrongAnswer* wrongAnswerHead) {
    FILE* file = fopen("wrong_answers.txt", "w");
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }

    WrongAnswer* current = wrongAnswerHead;
    while (current != NULL) {
        // Write wrong answer data in the format: id|question|wrongAnswer|correctAnswer
        fprintf(file, "%d|%s|%s|%s\n",
                current->IdQuestion,
                current->Question,
                current->wrongAnswer,
                current->correctAnswer);
        current = current->next;
    }

    fclose(file);
    printf("Wrong answers saved successfully to 'wrong_answers.txt'.\n");
}

/*
Load the WrongAnswers from a file with | as the separator.
*/
void loadWrongAnswersFromFile(WrongAnswer** wrongAnswerHead) {
    FILE* file = fopen("wrong_answers.txt", "r");
    if (file == NULL) {
        perror("Failed to open file for reading");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char question[MAX_STRING_QUESTION];
        char wrongAnswer[MAX_STRING_QUESTION];
        char correctAnswer[MAX_STRING_QUESTION];

        // Parse the line using | as the delimiter
        if (sscanf(line, "%d|%[^|]|%[^|]|%[^\n]",
                   &id, question, wrongAnswer, correctAnswer) == 4) {
            // Create a new WrongAnswer node
            WrongAnswer* newNode = (WrongAnswer*)malloc(sizeof(WrongAnswer));
            if (newNode == NULL) {
                printf("Error: Could not allocate memory while loading wrong answers.\n");
                fclose(file);
                return;
            }

            // Assign values to the new node
            newNode->IdQuestion = id;
            strcpy(newNode->Question, question);
            strcpy(newNode->wrongAnswer, wrongAnswer);
            strcpy(newNode->correctAnswer, correctAnswer);
            newNode->next = NULL;
            newNode->prev = NULL;

            // Insert the new node at the end of the list
            if (*wrongAnswerHead == NULL) {
                *wrongAnswerHead = newNode;
            } else {
                WrongAnswer* temp = *wrongAnswerHead;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = newNode;
                newNode->prev = temp;
            }
        } else {
            printf("Invalid format in line: %s", line);
        }
    }

    fclose(file);
    printf("Wrong answers loaded successfully from 'wrong_answers.txt'.\n");
}

void displayBottom5Scores(Player* playerHead, Player* currentPlayer) {
    Player* temp = playerHead;
    int playerCount = 0;
 
    // Count the number of players
    while (temp != NULL) {
        playerCount++;
        temp = temp->next;
    }
 
    // If there are 5 or fewer players, display all scores
    if (playerCount <= 5) {
        printf("\n==============================\n");
        printf("Keep practicing! \nHere are some scores to beat:\n\n");
 
        printf("Bottom Scores:\n");
        temp = playerHead;
        for (int i = 0; i < playerCount; i++) {
            if (temp == currentPlayer) {
                printf("%d. YOU: %s - %.0f pts\n", i + 1, temp->nickname, temp->maxScore);
            } else {
                printf("%d. %s - %.0f pts\n", i + 1, temp->nickname, temp->maxScore);
            }
            temp = temp->next;
        }
        printf("\nDon't give up! Try again!\n");
        printf("==============================\n");
        return;
    }
 
    // If there are more than 5 players, collect players into an array
    Player* players[10000];  // Temporarily, for the dynamic list
    temp = playerHead;
    int index = 0;
    while (temp != NULL) {
        players[index++] = temp;
        temp = temp->next;
    }

    // Sort players by score
    for (int i = 0; i < index - 1; i++) {
        for (int j = i + 1; j < index; j++) {
            if (players[i]->maxScore > players[j]->maxScore) {
                Player* tempPlayer = players[i];
                players[i] = players[j];
                players[j] = tempPlayer;
            }
        }
    }

    // Check if the current player is among the bottom 5 scores
    bool isInBottom5 = false;
    for (int i = 0; i < 5; i++) {
        if (players[i] == currentPlayer) {
            isInBottom5 = true;
            break;
        }
    }
 
    // If the current player is not in the bottom 5, do nothing
    if (!isInBottom5) {
        return;
    }
 
    // Display the bottom 5 scores
    printf("\n==============================\n");
    printf("Keep practicing! \nHere are some scores to beat:\n\n");
 
    printf("Bottom 5 Scores:\n");
 
    // Show the bottom 5 scores, highlighting the current player as "YOU"
    for (int i = 0; i < 5; i++) {
        if (players[i] == currentPlayer) {
            printf("%d. YOU: %s  - %.0f pts\n", 5 - i, players[i]->nickname, players[i]->maxScore);
        } else {
            printf("%d. %s - %.0f pts\n", 5 - i, players[i]->nickname, players[i]->maxScore);
        }
    }
 
    // Motivational message
    printf("\nDon't give up! Try again!\n");
    printf("==============================\n");
}
