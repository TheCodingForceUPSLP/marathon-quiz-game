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
