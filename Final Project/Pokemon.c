// Pokemon
// James Zhou and Joseph Kocis


#include <stdio.h>
#include <string.h>

#define MAXSTRINGLENGTH 20
#define MAXNUMBEROFITEMSINBAG 20
#define TIMESUNTILASKTOQUIT 4
#define MAXMOVES 20


enum nature {
    example, anotherExample
};

enum moveType {
    texample, tanotherExample
};

enum moveName {
    tackle, bite, last
};

typedef struct item {
    
    
    
} Item;


typedef struct bag {
    int numberOfItems;
    Item items[MAXNUMBEROFITEMSINBAG];
} Bag;



typedef struct player {
    char name[MAXSTRINGLENGTH];
    FILE *readDataFile;
    FILE *writeDataFile;
} Player;


typedef struct move {
    int damage;
    enum moveType type;
    int accuracy;
    enum moveName name;
} Move;


typedef struct pokemon {
    
    int HP;
    int attack;
    int defense;
    int specialAttack;
    int specialDefense;
    int speed;
    int base;
    int iv;
    int ev;
    enum nature nature;
    
    Move moves[MAXMOVES];
    
} Pokemon;












//function headers
void launchGame(void);
void startMainMenu(Player *player);
Player* createPlayer(char playerName[MAXSTRINGLENGTH]);

int levelUp(Pokemon *pokemon);
int increaseEV(Pokemon *pokemon, int amount);
int addMove(enum moveName moveToAdd, Move allMoves[MAXMOVES], Pokemon *pokemon);
Move* createAllMoves();


int main() {
    launchGame();
    return 0;
}

//creates a Player pointer to be used for the rest of the game, starts a new user or continues an existing user
void launchGame() {
    
    //welcome
    printf("Welcome to Pokemon!\n\n");
    
    //find out who's playing
    printf("Do you have an existing user you'd like to play as?\n");
    
    //get the response
    char response[MAXSTRINGLENGTH];
    scanf("%s", response);
    
    //if the response is not valid, keep trying until it is valid
    int counter = 0;
    while (!(!strcmp(response, "Yes") || !strcmp(response, "yes") || !strcmp(response, "No") || !strcmp(response, "no"))) {
        
        //if the user decides to quit
        if ((!strcmp(response, "Quit") || !strcmp(response, "quit"))) {
            printf("Ending the game, thanks for playing!\n");
            return;
        }
        
        //print appropriate message if the user is stuck
        if (counter < TIMESUNTILASKTOQUIT) {
            printf("Sorry, please enter yes or no.\nDo you have an existing user you'd like to player as?");
        } else {
            printf("Sorry, please enter yes, no, or quit.\nDo you have an existing user you'd like to player as?");
        }
        
        //take in new response and increase counter
        scanf("%s", response);
        counter++;
    }
    
    int isNewUser = 0;
    
    //it is a new user
    if (!strcmp(response, "No") || !strcmp(response, "no")) {
        isNewUser = 1;
    }
    
    //if a file exists for this player then start their game
    
    
}

void startMainMenu(Player *player) {
    
    
    
}


Player* createPlayer(char playerName[MAXSTRINGLENGTH]) {
    
    // TODO: - add error checking for if there is no existing file for this playerName
    
    Player player;
    strcpy(player.name, playerName);
    player.readDataFile = fopen(playerName, "r");
    player.writeDataFile = fopen(playerName, "w");
    return &player;
}


//levels up the Pokemon that is passed in
//returns 1 if successful and 0 if unsuccessful
int levelUp(Pokemon *pokemon) {
    return 0;
}

//increases the ev on the passed in Pokemon by the amount passed in
//returns 1 if successful and 0 if unsuccessful
int increaseEV(Pokemon *pokemon, int amount) {
    return 0;
}

//adds moveToAdd to pokemon
//returns 1 if sussessful and 0 if unsuccessful
int addMove(enum moveName moveToAdd, Move allMoves[MAXMOVES], Pokemon *pokemon) {
    
    //loops through allMoves except for last to find the first and hopefully only occurance (return 0 if not the only occurance) of moveToAdd
    //adds the move to moves in the Pokemon
    return 0;
    
}

//returns an array of all possible moves
Move* createAllMoves() {
    
    for (int i = 0; i < last; i++) {
        
    }
    Move example[MAXMOVES];
    return example;
}

//returns the damage given the move and both Pokemon
//does not apply any damage or make any changes to anything
int getDamage(Move move, Pokemon attackingPokemon, Pokemon attackedPokemon) {
    
    //calculate damage given the damage, accuracy, and type of the move, and both Pokemon
    return 0;
    
}
