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
