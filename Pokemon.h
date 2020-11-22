//
//  Pokemon.h
//  Pokemon
//
//  Created by Jody Kocis on 11/23/18.
//  Header file for Pokemon.c
//

#ifndef Pokemon_h
#define Pokemon_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define LEVELOFNOMOREEVOLUTION -1
#define LEVELOFFIRSTEVOLUTION 4
#define LEVELOFSECONDEVOLUTION 8

#define MAXNUMBEROFLEVELS 5
#define MAXLINESPERMAP 100

#define MAXEMOJILENGTH 10
#define MAXSTRINGLENGTH 50 + 1
#define MAXNUMBEROFITEMSINBAG 20
#define TIMESUNTILASKTOQUIT 4
#define MAXMOVES 4

#define STAB 1.5
#define CRITICAL 1.5

#define FIRSTUNLOCKEDMAP "Starting Map"
#define NOMAPFILE "No Map File"

#define ALLMAPSFILE "All Pokemon Maps (DO NOT EDIT).txt"
#define ALLPOKEMONFILE "All Pokemon.txt"

#define ENDOFMAP "End of Map"

//When accessing these files always add the player name after
#define UNLOCKEDMAPSFILE "Unlocked Maps "
#define MYPOKEMON "My Pokemon "
#define MYITEMS "My Items "


//Map Key
#define BUSH '*'
#define PATH 'P'
#define SHORTGRASS '_'
#define LONGGRASS '^'
#define WATER '~'
#define FENCE '['
#define ROCK 'o'
#define HOUSE 'H'
#define MOUNTAIN 'M'

#define SHOPDOOR '$'

#define PLAYER '8'
#define GYMLEADER '0'


//MARK: - Enums


//Used to determine if the user is fighting a wild pokemon or a gymLeader. Is an enum because we wanted to make it easy to add future support for a feature we originally intended to add which was multiplayer battling
enum battleMode {
    wild, gymLeader
};

//The natures a Pokemon can have
enum nature {
    hardy, lonely, brave, adamant, naughty, bold, docile, relaxed, impish, lax, timid, hasty, serious, jolly, naive, modest, mild, quiet, bashful, rash, calm, gentle, sassy, careful, lastNature
};

//The types that a move a can be
enum moveType {
    bug, fight, fire, flying, grass, ground, normal, poison, phsycic, rock, water, noType
};

//The name of every move the game supports
enum moveName {
    tackle, confusion, gust, silverWind, pursuit, hyperFang, ironTail, wingAttack, steelWing, airSlash, flamethrower, slash, fireBlast, razorLeaf, sludgeBomb, powerWhip, engeryBall, surf, iceBeam, hydroPump, strength
};

//Determines if a move is physical or special
enum physicalOrSpecial {
    physical, special
};

//The effectiveness values a move can have on a Pokemon
enum effectiveness {
    notEffective, notVeryEffective, normalEffectiveness, superEffective
};

//The different stats value a Pokemon has
enum stats {
    attack, defense, specialAttack, specialDefense, speed
};

//The name of every Pokemon the game supports
enum pokemonName {
    caterpie, metapod, butterfree, rattata, raticate, pidgey, pidgeotto, pidgeot, charmander, charmeleon, charizard, bulbasaur, ivysaur, venusaur, squirtle, wartortle, blastoise, none
};


//MARK: - Structs


//Represents the player currently playing the game
typedef struct player {
    char name[MAXSTRINGLENGTH];
    FILE* unlockedMapsFile;
    FILE* pokemonFile;
    
    char unlockedMapsFileName[MAXSTRINGLENGTH];
    char pokemonFileName[MAXSTRINGLENGTH];
    
    int numberOfPokemon;
} Player;


//Represents a move a Pokemon can have
typedef struct move {
    int damage;
    enum moveType type;
    enum physicalOrSpecial isPhysicalOrSpecial;
    int accuracy;
    enum moveName name;
} Move;


//Represent a Pokemon in game
typedef struct pokemon {
    
    char name[MAXSTRINGLENGTH];
    int baseHP;
    int baseAttack;
    int baseDefense;
    int baseSpecialAttack;
    int baseSpecialDefense;
    int baseSpeed;
    int ivHP;
    int ivAttack;
    int ivDefense;
    int ivSpecialAttack;
    int ivSpecialDefense;
    int ivSpeed;
    int evHP;
    int evAttack;
    int evDefense;
    int evSpecialAttack;
    int evSpecialDefense;
    int evSpeed;
    int hiddenEvAttack;
    int hiddenEvDefense;
    int hiddenEvSpecialAttack;
    int hiddenEvSpecialDefense;
    int hiddenEvSpeed;
    int HP;
    enum nature nature;
    int XP;
    int level;
    enum pokemonName nextEvolution;
    enum moveType type1;
    enum moveType type2;
    int catchRate;
    int levelToEvolve;
    enum stats evTypeGiven;
    int evAmountGiven;
    
    int fileNumber;
    
    int numberOfMoves;
    Move moves[MAXMOVES];
    
} Pokemon;


//MARK: - Function Headers


void endGame(void);
int getRandomNumber(int lower, int upper);

//Dealing with files
int playerHasPlayed(char playerName[MAXSTRINGLENGTH]);
FILE* getAllMapsFile(void);

void removeFinalNewLineCharacter(char string[MAXSTRINGLENGTH]);

//Dealing with maps
int getAllMaps(FILE* allMapsFile, char maps[MAXNUMBEROFLEVELS][MAXSTRINGLENGTH], const int maxNumberOfLevels, const int maxStringLength);
int readMap(FILE* mapsFile, char maps[MAXLINESPERMAP][MAXLINESPERMAP + 1]);

//Reading pokemon information
void getPokemonNamesAndNumbers(char pokemonFileName[MAXSTRINGLENGTH], int numberOfPokemon, char pokemonNames[numberOfPokemon][MAXSTRINGLENGTH], int pokemonNumbers[numberOfPokemon]);
int fileExistsFor(Player* player, char pokemonName[MAXSTRINGLENGTH], int pokemonNumber);
int getPokemonInformation(Pokemon* pokemon, Player* player, char pokemonName[MAXSTRINGLENGTH], int pokemonNumber);
int readPokemonInformation(FILE* pokemonFile, Pokemon* pokemon, char name[MAXSTRINGLENGTH], int fileNumber);
int checkPokemonFileInformation(Pokemon* pokemon, char informationTitle[MAXSTRINGLENGTH], char correctTitle[MAXSTRINGLENGTH]);
int fillMoveInformation(Move* move);


void writeToFile(FILE* file, char fileName[MAXSTRINGLENGTH], char string[MAXSTRINGLENGTH]);

//Getting player files
FILE* getPlayerUnlockedMapsFile(char* playerName);
FILE* getPlayerPokemon(char* playerName);

//Getting player Pokemon
int getNumberOfPokemon(void);
void getListOfAllPokemon(char allPokemon[getNumberOfPokemon()][MAXSTRINGLENGTH]);
int doesPokemonExist(char pokemonName[MAXSTRINGLENGTH]);

//Startup
void launchGame(void);
int startMainMenu(Player *player, int isNewPlayer);
int createPlayer(char playerName[MAXSTRINGLENGTH], Player* player);

//Creating Pokemon and writing Pokemon informatnio
void chooseStarterPokemon(Player* player, Pokemon* starterPokemon);
void addPokemonToPlayer(Player* player, Pokemon* pokemon);
void writePokemonInformation(FILE* pokemonFile, char pokemonFileName[MAXSTRINGLENGTH], Pokemon* pokemon);
int createPokemon(Pokemon* pokemon, enum pokemonName pokemonName);

//Presenting visual aspects of the game
int startMap(Player* player, int isNewPlayer, char mapFileName[MAXSTRINGLENGTH]);
void presentPokemon(Player* player);
int elementsAreInArrays(Player* player, char element[MAXSTRINGLENGTH], int number, char array[player->numberOfPokemon][MAXSTRINGLENGTH], int intArray[player->numberOfPokemon]);
int checkAndPerformAction(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int playerRow, int playerColumn, Player* player);
int canStandOnSpot(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int playerRow, int playerColumn);
int isActionBreakable(char action);
void printMapAndKeyAndPlayer(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int mapHeight, int playerRow, int playerColumn);
void printMapWithPlayer(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int mapHeight, int playerRow, int playerColumn);
void convertCharacterToEmoji(char character, char emoji[MAXEMOJILENGTH]);
void printMapKey(void);

//Dealing with maps
void findAvailableSpot(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int mapHeight, int availableSpot[2]);
int isPositionInMap(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int mapHeight, int row, int column);
int characterIsAvailableToPlacePlayerOn(char character);
int unlockedNewMaps(Player* player);
int isMapUnlocked(char map[MAXSTRINGLENGTH], char unlockedMaps[MAXNUMBEROFLEVELS][MAXSTRINGLENGTH]);

//Simulating spots on the map
int checkLongGrass(Player* player);
void startGymLeaderBattle(Player* player);
void showShop(Player* player);

//Battling
int startBattle(Pokemon* pokemon1, Pokemon* pokemon2, enum battleMode battleMode, const char player1Name[MAXSTRINGLENGTH], const char player2Name[MAXSTRINGLENGTH], Player* player1);
int getMaxHP(Pokemon pokemon);
double getAttack(Pokemon pokemon);
double getDefense(Pokemon pokemon);
double getSpecialAttack(Pokemon pokemon);
double getSpecialDefense(Pokemon pokemon);
double getSpeed(Pokemon pokemon);
enum effectiveness damage(int moveIndex, Pokemon* attackingPokemon, Pokemon* attackedPokemon);
double getTypeMultipler(enum moveType moveType, enum moveType pokemonType1, enum moveType pokemonType2);
double getValueOfEffectiveness(enum effectiveness effectiveness);
int levelUp(Pokemon *pokemon);
double getNatureMultiplyer(enum stats stat, enum nature);


//Converting enums to string and strings to enums
void convertPhysicalOrSpecialToString(enum physicalOrSpecial physicalOrSpecial, char physicalOrSpecialAsString[MAXSTRINGLENGTH]);
enum physicalOrSpecial convertPhysicalOrSpecialStringToPhysicalOrSpecial(char physicalOrSpecialString[MAXSTRINGLENGTH]);
void convertMoveTypeToString(enum moveType moveType, char moveTypeAsString[MAXSTRINGLENGTH]);
enum moveType convertMoveTypeStringToMoveType(char moveTypeAsString[MAXSTRINGLENGTH]);
void convertMoveNameToString(enum moveName moveName, char moveNameAsString[MAXSTRINGLENGTH]);
enum moveName convertMoveNameStringToMoveName(char moveNameAsString[MAXSTRINGLENGTH]);
void convertNatureToString(enum nature nature, char natureAsString[MAXSTRINGLENGTH]);
enum nature convertNatureStringToNature(char natureAsString[MAXSTRINGLENGTH]);
void convertPokemonNextEvolutionToString(enum pokemonName nextEvolution, char nextEvolutionName[MAXSTRINGLENGTH]);
enum pokemonName convertPokemonStringToNextEvolution(char pokemonString[MAXSTRINGLENGTH]);
void convertStatToString(enum stats stat, char statAsString[MAXSTRINGLENGTH]);
enum stats convertStatStringToStat(char statAsString[MAXSTRINGLENGTH]);



#endif /* Pokemon_h */
