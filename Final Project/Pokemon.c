// Pokemon
// James Zhou and Joseph Kocis
// TODO: Description of Game


//reading file and moving around
//finding pokemon
//battling wild pokemon
//item shop
//multiplayer battling
//gym leader

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXNUMBEROFLEVELS 5

#define MAXSTRINGLENGTH 20
#define MAXNUMBEROFITEMSINBAG 20
#define TIMESUNTILASKTOQUIT 4
#define MAXMOVES 4

#define STAB 1.5
#define CRITICAL 1.5

#define FIRSTUNLOCKEDMAP "Starting Map"

#define ALLMAPSFILE "All Pokemon Maps (DO NOT EDIT)"

//When accessing these files always add the player name after
#define UNLOCKEDMAPSFILE "Unlocked Maps "
#define MYPOKEMON "My Pokemon "
#define MYITEMS "My Items "


//MARK: - Enums

enum nature {
    example, anotherExample
};

enum moveType {
    bug, fight, fire, flying, grass, ground, normal, poison, phsycic, rock, water
};

enum moveName {
    tackle, bite, last
};

enum physicalOrSpecial {
    physical, special
};

enum effectiveness {
    cant, notEffective, notVeryEffective, normalEffectiveness, superEffective
};


//MARK: - Structs

typedef struct item {
    
    
    
} Item;


typedef struct bag {
    int numberOfItems;
    Item items[MAXNUMBEROFITEMSINBAG];
} Bag;



typedef struct player {
    char name[MAXSTRINGLENGTH];
    FILE* unlockedMapsFile;
    FILE* pokemonFile;
    FILE* itemsFile;
} Player;


typedef struct move {
    int damage;
    enum moveType type;
    enum physicalOrSpecial isPhysicalOrSpecial;
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
    
    int numberOfMoves;
    Move moves[MAXMOVES];
    
    
} Pokemon;


//MARK: - Function Headers
int playerHasPlayed
(char playerName[MAXSTRINGLENGTH]);
FILE* getAllMapsFile(void);

char** getAllMaps(FILE* allMapsFile);

void launchGame(void);
void startMainMenu(Player *player, int isNewPlayer);
Player* createPlayer(char playerName[MAXSTRINGLENGTH]);

double getValueOfEffectiveness(enum effectiveness effectiveness);

int levelUp(Pokemon *pokemon);
int evolve(Pokemon *pokemon);
int increaseEV(Pokemon *pokemon, int amount);
int addMove(enum moveName moveToAdd, Move allMoves[MAXMOVES], Pokemon *pokemon);
Move* createAllMoves(void);





int main() {
    launchGame();
    return 0;
}


//closes all files
void endGame() {
    exit(0);
}


//MARK: - Get File Pointers

//returns 0 if the player has never player before, returns 1 if they have
int playerHasPlayed(char playerName[MAXSTRINGLENGTH]) {
    
    FILE *playerFile = fopen(strcat(UNLOCKEDMAPSFILE, playerName), "r");
    
    if (playerFile == NULL) {
        fclose(playerFile);
        return 0;
    } else {
        fclose(playerFile);
        return 1;
    }
    
}



//returns the file with the list of all maps, if the file does not exist it will return NULL
FILE* getAllMapsFile() {
    FILE *allMapsFile = fopen(ALLMAPSFILE, "r+");
    return allMapsFile;
}

//MARK: - Read Files


//returns an array of Strings that are the names of the level files that are on the computer
char** getAllMaps(FILE* allMapsFile) {
    
    //loop through the entire file and add them to our array
        //for each file, only add if the file exists, if not then print an error message
    
    char maps[MAXNUMBEROFLEVELS][MAXSTRINGLENGTH];
    
    int i = 0;
    while (fgets(maps[i], MAXSTRINGLENGTH, allMapsFile) != NULL && i < MAXNUMBEROFLEVELS) {
        //already added the next line of allMapsFile to maps in the above line
        
        //if the file does not actually exist then remove it
        if (fopen(maps[i], "r") == NULL) {
            //print error message
            printf("Error: Level file: %s does not exist. Pokemon will continue running without this level.", maps[i]);
            //remove file from maps
            strcpy(maps[i], NULL);
        } else {
            i++;
        }
    }
    
    //make the remaining elements of maps NULL
    if (i < MAXNUMBEROFLEVELS) {
        for (int j = i; i < MAXNUMBEROFLEVELS; j++) {
            strcpy(maps[i], NULL);
        }
    }
    
    return maps;
}



//MARK: - Player Files

//returns the unlockedMapsFile for the player, creates a new file if one doesn't already exist
FILE* getPlayerUnlockedMapsFile(char* playerName) {
    FILE* playerUnlockedMapsFile = fopen(strcat(UNLOCKEDMAPSFILE, playerName), "r+");
    
    if (playerUnlockedMapsFile == NULL) {
        playerUnlockedMapsFile = fopen(strcat(UNLOCKEDMAPSFILE, playerName), "w+");
    }
    
    return playerUnlockedMapsFile;
}

//returns the pokemonFile for the player, creates a new file if one doesn't already exist
FILE* getPlayerPokemon(char* playerName) {
    FILE* playerPokemonFile = fopen(strcat(MYPOKEMON, playerName), "r+");
    
    if (playerPokemonFile == NULL) {
        playerPokemonFile = fopen(strcat(MYPOKEMON, playerName), "w+");
    }
    
    return playerPokemonFile;
}

//returns the itemsFile for the player, creates a new file if one doesn't already exist
FILE* getPlayerItems(char* playerName) {
    FILE* playerItemsFile = fopen(strcat(MYITEMS, playerName), "r+");
    
    if (playerItemsFile == NULL) {
        playerItemsFile = fopen(strcat(MYITEMS, playerName), "w+");
    }
    
    return playerItemsFile;
}



//MARK: - Startup

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
            endGame();
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
    
    //ask for the name of the user
    if (isNewUser) {
        printf("Creating a new user. What would you like the name of your player to be?\n");
    } else {
        printf("What is the name of the player you'd like to play as?\n");
    }
    
    //read in the name of the player
    char playerName[MAXSTRINGLENGTH];
    scanf("%s", playerName);
    
    
    //if a file exists for this player then start their game
    
    //if they are a new user
    if (isNewUser) {
        
        while (playerHasPlayed(playerName)) {
            
            printf("This user already has save data. Would you still like to play as %s?", playerName);
            char continuePlayResponse[MAXSTRINGLENGTH];
            scanf("%s", continuePlayResponse);
            
            int counter1 = 0;
            while (!(!strcmp(continuePlayResponse, "Yes") || !strcmp(continuePlayResponse, "yes") || !strcmp(continuePlayResponse, "No") || !strcmp(continuePlayResponse, "no"))) {
                
                //if the user decides to quit
                if ((!strcmp(continuePlayResponse, "Quit") || !strcmp(continuePlayResponse, "quit"))) {
                    printf("Ending the game, thanks for playing!\n");
                    endGame();
                    return;
                }
                
                //print appropriate message if the user is stuck
                if (counter < TIMESUNTILASKTOQUIT) {
                    printf("Sorry, please enter yes or no.\nDo you have an existing user you'd like to player as?");
                } else {
                    printf("Sorry, please enter yes, no, or quit.\nDo you have an existing user you'd like to player as?");
                }
                
                //take in new response and increase counter
                scanf("%s", continuePlayResponse);
                counter1++;
            }
            
            //we now know the response is either yes or no
            //if yes
            if (!strcmp(response, "Yes") || !strcmp(response, "yes")) {
                break;
            }
            //otherwise it has to be no, because of the check above
            else {
                //ask for a different user
                printf("What is the name of the player you'd like to play as?\n");
                scanf("%s", playerName);
            }
            
        }
        
        
    }
    //they are a returning user
    else {
        
        while (!playerHasPlayed(playerName)) {
            
            printf("No save data exists for this user. Would you like to create new user %s?", playerName);
            char continuePlayResponse[MAXSTRINGLENGTH];
            scanf("%s", continuePlayResponse);
            
            int counter1 = 0;
            while (!(!strcmp(continuePlayResponse, "Yes") || !strcmp(continuePlayResponse, "yes") || !strcmp(continuePlayResponse, "No") || !strcmp(continuePlayResponse, "no"))) {
                
                //if the user decides to quit
                if ((!strcmp(continuePlayResponse, "Quit") || !strcmp(continuePlayResponse, "quit"))) {
                    printf("Ending the game, thanks for playing!\n");
                    endGame();
                    return;
                }
                
                //print appropriate message if the user is stuck
                if (counter < TIMESUNTILASKTOQUIT) {
                    printf("Sorry, please enter yes or no.\nDo you have an existing user you'd like to player as?");
                } else {
                    printf("Sorry, please enter yes, no, or quit.\nDo you have an existing user you'd like to player as?");
                }
                
                //take in new response and increase counter
                scanf("%s", continuePlayResponse);
                counter1++;
            }
            
            //we now know the response is either yes or no
            //if yes
            if (!strcmp(response, "Yes") || !strcmp(response, "yes")) {
                break;
            }
            //otherwise it has to be no, because of the check above
            else {
                //ask for a different user
                printf("In that case, what is the name of the user you would like to play as?\n");
                scanf("%s", playerName);
            }
            
        }
        
    }
    
    //create new player and player file
    Player* player = createPlayer(playerName);
    
    //start main menu
    startMainMenu(player, isNewUser);
    
    //after the game
    endGame();
    
}


                
//will present the main menu of the game
void startMainMenu(Player *player, int isNewPlayer) {
    
    //new player code
    if (isNewPlayer) {
        printf("Welcome to Pokemon!\n");
    }
    //returning player
    else {
        printf("Welcome back!\n");
    }
    
    
    //find allMapsFile and handle the error of the file not existing
    FILE* allMapsFile = getAllMapsFile();
    if (allMapsFile == NULL) {
        printf("Error: File with file name %s does not exist.\nPokemon will now quit. Goodbye!\n", ALLMAPSFILE);
    }
    
    //get list of all maps
    char** maps = getAllMaps(allMapsFile);
    fclose(allMapsFile);
    
    //get list of all the player's unlocked maps
    char** unlockedMaps = getAllMaps(getPlayerUnlockedMapsFile(player->name));
    
    printf("\nAvailable Maps\n");
    
    //present list of maps saying locked or unlocked
    int i;
    for (i = 0; i < MAXNUMBEROFLEVELS; i++) {
        
        //exit the loop after the last map
        if (maps[i] == NULL) {
            break;
        }
        
        //check to see if map is unlocked
        int mapIsUnlocked = 0;
        //loop through unlocked maps
        for (int j = 0; j < MAXNUMBEROFLEVELS; j++) {
            if (unlockedMaps[j] == NULL) {
                break;
            }
            if (!strcmp(maps[i], unlockedMaps[j])) {
                mapIsUnlocked = 1;
                break;
            }
        }
        
        if (mapIsUnlocked) {
            printf("%d. %s: Unlocked\n", i+1, maps[i]);
        } else {
            printf("%d. %s: Locked\n", i+1, maps[i]);
        }
        
        
    }
    
    
    //allow the player to select a map
    printf("What is the number of the map you would like to play?\n");
    
    
    int selectedMap = 0;
    scanf("%d", &selectedMap);
    selectedMap--;
    
    //make sure the selected map is valid
    while (selectedMap < 0 || selectedMap >= i) {
        printf("That is not a valid map number. Please enter a number between %d and %d.", 1, i);
    }
    
    //present the map
    
    
}





Player* createPlayer(char playerName[MAXSTRINGLENGTH]) {
    
    Player player;
    strcpy(player.name, playerName);
    
    player.unlockedMapsFile = getPlayerUnlockedMapsFile(playerName);
    player.pokemonFile = getPlayerPokemon(playerName);
    player.itemsFile = getPlayerItems(playerName);
    
    //initialize the maps file
    fprintf(player.unlockedMapsFile, FIRSTUNLOCKEDMAP);
    
    return &player;
}

//TODO: fix all of these errors by declaring the variable outside of the function



//MARK: - Battling

//returns the multiplier for the effectiveness given the enum value
double getValueOfEffectiveness(enum effectiveness effectiveness) {
    
    switch (effectiveness) {
        case cant:
            return 0.0;
            break;
        case notEffective:
            return 0.0;
            break;
        case notVeryEffective:
            return 0.5;
            break;
        case normalEffectiveness:
            return 1.0;
            break;
        case superEffective:
            return 2.0;
            break;
        default:
            return 0.0;
            break;
    }
    
}

//levels up the Pokemon that is passed in
//returns 1 if successful and 0 if unsuccessful
int levelUp(Pokemon *pokemon) {
    return 0;
}

//evolves that Pokemon that is passed in
//returns 1 if successful and 0 if unsuccessful
int evolve(Pokemon *pokemon) {
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
enum effectiveness damage(Move move, Pokemon attackingPokemon, Pokemon attackedPokemon) {
    
    //consider using int for index instead of move
    
    /*//check to see if attackingPokemon has move
    //if not then return miss
    int attackingPokemonHasMove = 0;
    for (int i = 0; i < attackingPokemon.numberOfMoves; i++) {
        if (strcmp(attackingPokemon.moves[i].name, move.name) == 0) {
            attackingPokemonHasMove = 1;
            //FIX FIX FIX FIX FIX
        }
    }
    
    if (!attackingPokemonHasMove) {
        return cant;
    }*/
    
    //calculate damage of attackingPokemon using move against attackedPokemon
    
    
    //apply damage
    
    
    return 0;
    
}
