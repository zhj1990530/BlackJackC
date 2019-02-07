//  CS222  Project 1 ///// G01112757
//
//  Name:  Huajun Zhang (Ryan)
// 
//  Simple BlackJack 10/11/2017


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// function prototypes  
char getachar(void); 		// get a character in raw mode	 
int dealing(void);          // dealing a card
char *convert_card(int);    // convert num to string
void play(int*, int, int*); // play game
int fix_card(int);          //fix the jkq;
void end(int, int*);       //end  
void begin(int*, int*);     //begin

//main function
int main(void){
    int bankroll=1000;  //declare variable bankroll
    int bet;            //declare variable bet
    int gamerecord[5]={0,0,0,0,0};  //declare variable array gamerecord, gamerecord[0] is win, gamerecord[1] is lose, gamerecord[2] is push,gamerecord[3] is bust,gamerecord[3] is blackjack
    char input;         //declare input using for flush input
    
    srandom((unsigned int)(time(NULL)));     // "Seed" the random number generator with the current time 
    /*"The random number generator needs to be seeded with the current time at the beginning of the main program"*/
    
    begin(&bankroll, &gamerecord[0]);//load record and bankroll in txt file
    
    do{        
        printf("Please place bet 10-1000 within your bank roll of %d: ", bankroll);
    
        scanf(" %d", &bet);
    
        printf("you bet %d on this game. Good luck!\n", bet);
        
        while((input = getchar()) != '\n' && input != EOF);
        //flush all input in buffer
        
        play(&bankroll,bet,&gamerecord[0]);
        
        if(bankroll<=0)
            break;
        
        printf("Press y to continue playing:\n");
        
                
    }while( getachar() == 'y');
    
    end(bankroll, &gamerecord[0]);//report game record
  
	return 0;
    
    //printf("success");
    
}

void play(int *bankroll, int bet, int *gamerecord){
    
    char input;         //declare input using for flush input
    
    int dealer_hand[5]={0,0,0,0,0}; //declare array dealer's hand
    int player_hand[5]={0,0,0,0,0}; //declare array player's hand
    int dealer_card_num=2;          //declare number of card dealer have
    int player_card_num=2;          //declare number of card player
    int player_point=0;             //declare point of player
    int dealer_point=0;             //declare point of dealer
    
    char card[]="null";             //declare card store dealing card string
    int num;                        //declare card store dealing number
    int i,j;
    /*draw card first round*/
    dealer_hand[0] = dealing();
    dealer_hand[1] = dealing();
    player_hand[0] = dealing();
    player_hand[1] = dealing();
    
    /*print hands*/
    printf("Dealer's hole card face value:\n%s\n",convert_card(dealer_hand[0]));
    
    printf("Player has:\n%s\n%s\n",convert_card(player_hand[0]),convert_card(player_hand[1]));
    
    /*player round*************************************************/
    //deal with A
    player_point= fix_card(player_hand[0])+fix_card(player_hand[1]);
    if(player_hand[0]==1||player_hand[1]==1){
        player_point+=10;
    }
    
    for(i=2;i<5;i++){
        printf("press space key to hit or any other key to stand:\n");
                
        if(getachar()==' '){
            //dealing a new card
            player_hand[i]=dealing();
            
            //deal with A can be either 11 or 1
            if(player_hand[i]==1){
                if(player_point>10){
                    player_point=player_point+1;// count 1 in order to not bust
                }else{
                    player_point=player_point+11;// keep hitting
                }
            }else{
                // get total point
                player_point=fix_card(player_hand[i])+player_point;
            }
            
            printf("dealing %s\n",convert_card(player_hand[i]));
            
            if(player_point>21){
                printf("you busted, lose\n");
                gamerecord[1]++;    //lose record
                gamerecord[3]++;    //bust record
                *bankroll=*bankroll-bet; //lose money
                return;
            }
            
            printf("player's point is: %d\n",player_point);
            
            //card num increase
            player_card_num++;
        }else
            break;
        
    }
    
    //print score
    if(player_point==21){
        
        printf("player got Black jack!!\n");
        gamerecord[4]++;
        
    }else if(player_point>21){
        
        printf("player busted !!\n"); //not reach here
        
    }else{
        
        printf("player's point is: %d\n",player_point);
        
    }
    //print hands
    for(j = 0; j < player_card_num; j++) {
        printf("%s\n", convert_card(player_hand[j]));
    }
    /*player round end*********************************************/

    
    /*dealer round*************************************************/
    //dealer's point first 2 card
    
    dealer_point= fix_card(dealer_hand[0])+fix_card(dealer_hand[1]);
    if(dealer_hand[0]==1||dealer_hand[1]==1){
        dealer_point+=10;
    }    
    
    /*dealer draw cards up to 5 will stop when point greater than 16 */
    for(i=2;i<5;i++){
        if(dealer_point < 17 ){
            
            //dealing a new card
            dealer_hand[i]=dealing();
            
            //deal with A can be either 11 or 1
            if(dealer_hand[i]==1){
                if(dealer_point>10){
                    dealer_point=dealer_point+1;// count 1 in order to not bust
                }else{
                    dealer_point=dealer_point+11;// keep hitting
                }
            }else{
                // get total point
                dealer_point=fix_card(dealer_hand[i])+dealer_point;
            }
            //card num increase
            dealer_card_num++;
        }else{
            break;
        }
    }
    
    //print score
    if(dealer_point==21){
        printf("dealer got Black jack!!\n");
        
    }else if(dealer_point>21){
        printf("dealer busted, player win !!\n");
        gamerecord[0]++;        //record win
        *bankroll=*bankroll+bet;//win money
        return;
    }else{
        printf("dealer's point is: %d\n",dealer_point);
    }
    //print hands
    for(j = 0; j < dealer_card_num; j++) {
        printf("%s\n", convert_card(dealer_hand[j]));
    }
    
    /*dealer round end*********************************************/
    
    /*judge *******************************************************/
    if(dealer_point>player_point){
        printf("dealer win!!!\n");
        gamerecord[1]++;        //record lose
        *bankroll=*bankroll-bet;//lose money

    }else if(dealer_point==player_point){
        printf("push!!\n");
        gamerecord[2]++;        //record push
        
    }else{
        printf("player win!!!\n");
        gamerecord[0]++;        //record win
        *bankroll=*bankroll+bet;//win money

    }
    return;
    
}

void end(int bankroll, int *gamerecord){
    FILE *inp;         /* pointer to input file */
    //print record
    int i;
    
    inp = fopen("proj1.txt", "w");
        //open file
    if(bankroll>0){
        
        fprintf(inp, "%d\n", bankroll);
        //writing bankroll
    
        for(i=0;i<5;i++){
            fprintf(inp, "%d\n", gamerecord[i]);
        }//writing record
        
        printf("your current bankroll:%d\n",bankroll);
    fclose(inp);
        
    }else{
        fprintf(inp, "%d\n", 1000);
    //writing bankroll
    
        for(i=0;i<5;i++){
            fprintf(inp, "%d\n", 0);
        }//writing record
        
        printf("Game over!\n");
        
    }
    printf("win: %d\nlose: %d\npush: %d\nbust %d\nbj: %d\n",gamerecord[0],gamerecord[1],gamerecord[2],gamerecord[3],gamerecord[4]);
    //print current bankroll
    //printf("your current bankroll:%d\n",bankroll);
    fclose(inp);
    //close file
}
void begin(int *bankroll, int *gamerecord){
    FILE *inp;         /* pointer to input file */
    inp = fopen("proj1.txt", "r");
    int i;
    //open file
    fscanf(inp, "%d", bankroll);
    //read bankroll
    for(i=0;i<5;i++){
        fscanf(inp, "%d", &gamerecord[i]);
    }//read record
    
    fclose(inp);
    //close file
}

//fix jqk
int fix_card(int num){
    if(num>=10)
        return 10;
    else
        return num;
}
//dealing a random card
int dealing(void){    
    
    return (int)(1+random() % 13);// return a random number from 1 to 13
    
}
//read one input/keypress
char getachar(void) {	 // get a character in raw mode
    char c; 
	fflush(stdin); 	// clear any input (keys) in the stdin pipe 
	system("stty raw  -echo");
	scanf("%c", &c);
    system("stty cooked echo");
 	return (c); 	
}

//convert number to string
char *convert_card(int num){
    char buffer[]="null";
	switch (num)
	{
		case 1:
            {
            return "A";
            }
		case 2:
            {
            return "2";
            }
		case 3:
            {
            return "3";
            }
		case 4:
            {
            return "4";
            }
		case 5:
            {
            return "5";
            }
		case 6:
            {
            return "6";
            }
		case 7:
            {
            return "7";
            }
		case 8:
            {
            return "8";
            }
		case 9:
            {
            return "9";
            }
        case 10:
            {
            return "10";
            }
		case 11:
            {
            return "J";
            }
		case 12:
            {
			return "Q";
            }
		case 13:
            {
			return "K";
            }
        default:
            return "XXXX"; //won't reach here
	}
}