/*
   Author:  Randy Jama
   Date: November 2016
   File: Assignment 3
   Compiler Version: Code::Blocks

   ------------------------------------
   COMP 120 - Assignment 3
   Blackjack Game: Player vs Computer
   ------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

//Define constatnts//
#define DECK_SIZE 53
#define HAND_SIZE 13
#define Jack 10
#define Queen 10
#define King 10
#define TRUE 1
#define FALSE 0
#define STARTING_CHERRIES 10


//Function prototypes//
void displayRules();
void InitializeDeck(int[]);
void initializeHand(int[]);
void ShuffleDeck(int[]);
int randomNumber(int);
void displayCard(int, bool);
int drawCard(int[]); //Takes deck, draws card, returns card//
int playerTurn(int[], int[]); //Takes deck and player array, returns score//
int compTurn(int[], int[]); //Takes deck and comp array, returns score//
int cardScore(int); //Takes drawn card integer and returns value between 1-10//
void clean(void); //Allows for a single "clean" entry from user//
void summary_and_winner(int, int); //Calculates and prints who won the whole game//
void roundSummary(int, int, int, int, int[], int[]); //Takes player/comp scores and arrays and cherries, and summarizes the round//
char playAnotherRound(int, int); //Asks user if they wish to play another round//



/*********************************************************************
 * main()
Contains the primary framework of the game.
*********************************************************************/
int main()
{
    //Initialize variables//
    int playerCherries = STARTING_CHERRIES; //Player's Cherries//
    int compCherries = STARTING_CHERRIES; //Computer's Cherries//
    int data[DECK_SIZE]; //Deck array//
    int playerSum; //Player's Score//
    int compSum; //Comp's Score//
    char resume = 'y'; //Resume round/game variable//
    int playerArray[HAND_SIZE];//2 Arrays to hold player and comp cards//
    int compArray[HAND_SIZE];

    srand(time(NULL));//Sets random seed for shuffling deck//

    displayRules();

    //Loop continues until score of either player reaches 0, or player wishes to not continue after a round.//
    /*WHILE LOOP NOT MADE INTO FUNCTION BECAUSE IT KEEPS TRACK OF 2 VARIABLES (PLAYER/COMP CHERRIES). A SINGLE
    FUNCTION CANNOT RETURN MULTIPLE VARIABLES SO THIS LOOP WAS KEPT IN TASK MAIN*/
    while(playerCherries != 0 && compCherries !=0 && resume != 'n')
    {
        //Initialize deck, shuffle deck, and initialize player/comp hands every round//
        InitializeDeck(data);
        ShuffleDeck(data);
        initializeHand(playerArray);
        initializeHand(compArray);

        printf("Here is your first card.\n");
        playerSum = playerTurn(data, playerArray); //Player Turn//
        compSum = compTurn(data, compArray); //Computer Turn//

        roundSummary(playerSum, compSum, playerCherries, compCherries, playerArray, compArray); //Display results of the round//

        //Display who wins this round, and who lost a cherry.//
        if(playerSum > compSum) //Player wins//
        {
            printf("\nYou have won this round, computer has now lost a cherry.\n");
            compCherries--;
        }
        else if(playerSum < compSum) //Computer wins//
        {
            printf("\nComputer won this round, you have now lost a cherry.\n");
            playerCherries--;
        }
        else //Tie//
        {
            printf("\nThis round is a draw. Neither player loses a cherry.\n");
        }

        resume = playAnotherRound(playerCherries, compCherries); //Ask user if they wish to resume playing//
    }

    summary_and_winner(playerCherries, compCherries); //Display who won the game overall//

    return 0;
}



/*********************************************************************
 * InitializeDeck()
 Set slots 0-51 with ascending values 0-51 to represent 52 cards. The
 final 52nd slot of array is to represent the number of drawn cards, and
 is left unchanged in this function.
 PRE: An integer array of size 53.
 POST: None.
 *********************************************************************/
void InitializeDeck(int data[])
{
    int i; //Counter//
    for(i=0; i<DECK_SIZE; i++) //Loop sets ascending values to represent 52 cards from 0-51//
    {
        data[i] = i;
    }
}



/*********************************************************************
 * ShuffleDeck()
 Shuffles the deck of 52 cards randomly, then displays results.
 PRE: An inititalized array representing 52 cards, and a final
 card-counting slot.
 POST: None.
 *********************************************************************/
 /* Must run InitializeDeck function first to label the data array before shuffling*/
void ShuffleDeck(int data[])
{
    int randomIndex; //Stores a randomly generated array slot number//
    int temp; //Temporarily storage during card-swapping process//
    int i; //Counter//
    for(i=0; i<DECK_SIZE-1; i++) /* Shuffle all slots except slot #53, which contains integer 52 */
    {
        randomIndex = randomNumber(DECK_SIZE-i-1);
        temp = data[i];
        data[i] = data[randomIndex];
        data[randomIndex] = temp;
    }

}


/*********************************************************************
 * RandomNumber()
Generates a random number between 0-51, which represents a card from a
deck of 52, and returns that integer value to the function caller.
PRE: Integer representing remaining cards in the deck
POST: None
 *********************************************************************/
int randomNumber(int max)
{
    int i = rand()% max; /* Generates random number between 0-51. */
    return i;
}


/*********************************************************************
 * DisplayCard()
Takes a drawn card from the deck array and calculates/displays the suit
and rank of card.
PRE: integer representing the drawn card, and a boolean expression
representing whether or not a new line should be drawn after printing
card's rank and suit
POST: None.
 *********************************************************************/
 /* Must run InitializeDeck function first to label the data array before shuffling*/
void displayCard(int card, bool newLine)
{
    int suit; //Suit of card//
    int rank; //Rank of card//

    suit = card/13;
    rank = card%13+1;

    //If-else statement for printing rank of valid cards//
    if(rank==0) //rank calculation for -1 (blank space value) is 0//
        printf(" "); //reserve for blank spaces to be printed in the round=summary table//
    else if(rank==1)
        printf("A");
    else if(rank==11)
        printf("J");
    else if(rank==12)
        printf("Q");
    else if(rank==13)
        printf("K");
    else
        printf("%i", rank);

    //If-else statement for printing suit for valid cards//
    if(suit==0)
        if(rank==0) //Nested if-else statement to check if rank is an actual card or a -1 'dummy' slot of a player's array//
            printf(" ");
        else
            printf("S");
    else if(suit==1)
        printf("H");
    else if(suit==2)
        printf("D");
    else
        printf("C");

    if(newLine) //if variable is true, create a new line//
        printf("\n");
}


/*********************************************************************
 * displayRules()
 Prints the rules of the Blackjack game.
 PRE: None.
 POST: None.
 *********************************************************************/
void displayRules()
{
    //Paragraph explaining the rules of the game//
    printf("Written by: Randy Jama \nWelcome to Blackjack!\n");
    printf("Rules:\n");
    printf("Player vs Computer. Each player begins with 10 cherries, or lives, which represent how many more rounds that each\n");
    printf("player has left available. Humans's turn is first. You are given cards that add up to your turn's total score.\n");
    printf("You may end your turn after a card has been drawn, or receive another card. If you score 21 or higher, your\n");
    printf("turn is over. Scoring over 21 means you busted, and will receive a score of 0, and 21 is the highest score\n");
    printf("possible. The computer will then complete their turn after yours, and the round will be summarized in a table,\n");
    printf("showing both scores and the number of cherries you both had going into the round. The loser of the round gets 1\n");
    printf("cherry taken away. You may choose to play another round as long as each player has cherries remaning. If you\n");
    printf("quit at any point, or either player's cherries runout, a winner will be declaired and the game is over.\n");
    printf("==================================================================================================================\n\n");
}




/*********************************************************************
 * drawCard()
 Takes a shuffled deck and draws the right-most "undrawn" card value
 based on the final slot value, and decrements the final slot value.
 Function returns the drawn card.
 PRE: Integer array representing a shuffled deck
 POST: Returns the right-most "unused" card from the array, who's slot
 is represented by the last slot of the integer array.
 *********************************************************************/
int drawCard(int deck[])//takes shuffled deck//
{
    int card = deck[deck[52]-1]; //draws card from the right-most slot that has not been used yet//
    deck[52]--; //Reduces last slot value (counting slot) so that the next time a card is drawn it will not reuse the same slot//
    return card;
}





/*********************************************************************
 * playerTurn()
 Takes deck and player card list arrays, and returns the total score of
 the drawn cards. Player's turn is looped to allow repeated card draws until
 the player either busts or ends their turn.
 PRE: Integer arrays for deck and player's cards
 POST: returns player's score for the round
 *********************************************************************/
int playerTurn(int deck[], int playerArray[])
{
    int i; //Counter//
    int score = 0; //Total score of current turn//
    char hitMe = 'y'; //Stores user's decision to receive another card//

    for(i=0; i < 12 && score < 21 && hitMe == 'y'; i++) //Loop for player to draw cards until they score >=21, or they willingly stop//
    {
        playerArray[i] = drawCard(deck); //draw card as integer value//
        displayCard(playerArray[i], true); //calculates and prints card drawn to user//
        score = score + cardScore(playerArray[i]); //adds rank of card to score//
        printf("SCORE: %i \n", score);
        playerArray[12]++; //adds 1 to the final "counter of lines" slot of playerArray//

        if (score < 21) //Ask to see if player wishes to draw again, if they have not busted//
        {
            //Draw another card loop. Re-prompts user's entry if non-valid entry is given//
            printf("Draw another card? Enter y or n.\n");
            do
            {
                scanf("%c", &hitMe);
                clean(); /* empty the input buffer */
                printf("\n");
                if(hitMe != 'y' && hitMe != 'n')
                {
                    printf("Invalid entry. Please enter y or n: \n");
                }
            }while(hitMe != 'y' && hitMe != 'n');
        }
        else //Player busted//
        {
            hitMe = 'n';
        }
    }

    if (score>21) //Player busted, so they scored nothing//
    {
        score = 0;
    }

    return score; //now that player drew cards and players array is filled, return the final score//
}






/*********************************************************************
 * cardScore()
 Takes drawn card integer, and returns the digit value of the card,
 which is between 1-10. Function is meant to give jack, queen, and king
 cards a value of only 10, despite their ranks being greater than 10.
 PRE: integer representing a "raw" card number drawn from the deck array.
 POST: returns the proper rank of the card drawn.
 *********************************************************************/
int cardScore(int card)
{
    int rank = card%13+1; //Rank of card//
    if (rank > 10)
    {
        rank = 10; //Ensures that the score of the card cannot exceed 10 if face-cards are drawn//
    }
    return rank;
}




/*********************************************************************
 * compTurn()
 Takes deck and comp card list arrays, and returns the score of the
 drawn cards. The turn is over once the computer generates a drawn-card
 score of 15 or greater.
 PRE: Arrays deck and compArray were previously initialized.
 POST: returns computer's score for the round
 *********************************************************************/
int compTurn(int deck[], int compArray[])
{
    int i; //Counter//
    int score = 0; //Stores computer's total score for the turn//
    //Loop for computer to draw cards until score is >=15//
    for(i=0; i < 12 && score <= 15; i++)
    {
        compArray[i] = drawCard(deck); //draw card as integer value//
        score = score + cardScore(compArray[i]); //adds rank of card to score//
        compArray[12]++; //adds 1 to the final "counter of lines" slot of compArray//
    }

    if (score>21) //Computer busted, so it scored nothing//
    {
        score = 0;
    }

    return score;
}





/*********************************************************************
 * clean()
Reads all characters from the input buffer until NEWLINE reached.
Returns nothing.
PRE: none
POST: input stream is empty
*********************************************************************/
void clean()
{
     char dummy; //dummy-variable//
     do
     {
         scanf("%c",&dummy); /*read a single char*/
     } while(dummy != '\n'); /*if it was the NEWLINE we are done*/
}





/*********************************************************************
 * summary_and_winner()
 Takes player and comp cherries, player and comp arrays, then prints out
 a table of cards drawn, and the winner of the entire game.
 PRE: Integers for player's and computer's remaining cherries
 POST: None.
 *********************************************************************/
void summary_and_winner(int playerCherries, int compCherries)
{
    printf("\n==========GAME OVER==========\n");
    printf("Player Cherries: %i\n", playerCherries);
    printf("Comp Cherries: %i\n", compCherries);

    //Check to see who won the game//
    if(playerCherries > compCherries)
    {
        printf("Congratulations! You have defeated the computer.\n");
    }
    else if (playerCherries < compCherries)
    {
        printf("You have lost to the computer.\n");
    }
    else //tie//
    {
        printf("The match has ended in a draw. Nobody wins.\n");
    }
}





/*********************************************************************
 * roundSummary()
Displays final table of round played, showing player and comp card results,
their sums, and the winner.
PRE: 4 integers representing the player's and comp's scores for the round, and
their remaining cherries, and 2 integer arrays representing player's and comp's
drawn cards for the round.
POST: None.
 *********************************************************************/
void roundSummary(int playerSum, int compSum, int playerCherries, int compCherries, int playerArray[], int compArray[])
{
    int i; //Counter//
    int maxCards; //Stores how many rows of cards to print in table//

    printf("==============================================\n");
    printf("\t\tRound Summary");
    printf("\n==============================================\n");
    printf("Player: %i Cherries \tComputer: %i Cherries\n", playerCherries, compCherries);

    //Checks to see which player has drawn more cards, which will be the max number of rows to print//
    if (playerArray[12]>compArray[12])
    {
        maxCards = playerArray[12];
    }
    else
    {
        maxCards = compArray[12];
    }
    for(i=0; i<maxCards; i++)
    {
        /*False statement tells function to not consider the 'nextLine' portion,
        since it is included at the end of this for loop*/
        displayCard(playerArray[i], false);
        printf("\t\t\t");
        displayCard(compArray[i], false);
        printf("\n");
    }
    printf("\nScore: %i\t\tScore: %i", playerSum, compSum);
}





/*********************************************************************
 * initializeHand()
 Sets a player's hand/array to consist of -1 in every slot except the
 final one, which is used to count the number of cards drawn in each
 player's turn.
 PRE: integer array representing the player's or comp's hand.
 POST: None.
 *********************************************************************/
void initializeHand(int data[])
{
    int i; //Counter//
    for(i=0; i<12; i++)
    {
        data[i] = -1; //Setting -1 allows the printing of spaces in the roundSummary table when rank is calculated//
    }
    data[12] = 0;
}




/*********************************************************************
 * playAnotherRound()
 Asks user if they wish to play another round if either player has cherries
 remaining, and returns a char variable representing their decision.
 PRE: 2 integers representing remaining player and comp cherries.
 POST: Returns character variable representing if player wishes to
 play another round.
 *********************************************************************/
 char playAnotherRound(int playerCherries, int compCherries)
 {
    char resume;
    //External if statement is to make sure that the user isn't prompted to play again even when either player has 0 cherries after a round//
    if (playerCherries != 0 && compCherries !=0)
    {
        printf("\nWould you like to play another round? Enter y or n.\n");
        do
        {
            scanf("%c", &resume);
            clean(); /* empty the input buffer */
            printf("\n");
            if(resume != 'y' && resume != 'n') //Re-prompts user's entry if non-valid entry is given.//
            {
                printf("Invalid entry. Please enter y or n: \n");
            }
        }while(resume != 'y' && resume != 'n');
    }
    return resume;
 }


