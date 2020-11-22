// Pokemon
// James Zhou and Joseph Kocis
// 12/4/18
// A representation of a Pokemon game. The user can sign in and catch Pokemon by battling their own Pokemon against wild Pokemon. The player can also battle against a gym leader in map 2.


#include "Pokemon.h"


//seeds the random number generator and calls the launchGame function
int main() {
    srand((int) time(0));
    launchGame();
    return 0;
}


//closes all files
void endGame() {
    exit(0);
}

//generates a random number within a range
int getRandomNumber(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}


//MARK: - General Dealing With Files

//checks to see if the player has player the game before
int playerHasPlayed(char playerName[MAXSTRINGLENGTH]) {
    
    //create the file name of the unlocked maps file for the playerName
    char fileName[strlen(UNLOCKEDMAPSFILE) + strlen(playerName) + strlen(".txt")];
    strcpy(fileName, UNLOCKEDMAPSFILE);
    strcat(fileName, playerName);
    strcat(fileName, ".txt");
    
    FILE* playerFile = fopen(fileName, "r");
    
    //if the file exists, then we'll assume player has played the game before
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

//MARK: - Dealing with Reading Files

//removes the \n character from a string if \n is the final character
void removeFinalNewLineCharacter(char string[MAXSTRINGLENGTH]) {
    //if the final character is \n
    if (!strcmp(&string[strlen(string) - 1], "\n")) {
        //remove the final character
        string[strlen(string) - 1] = '\0';
    }
}

//returns an array of Strings that are the names of the level files that existon the computer
//Parameters: allMapsFile: a file containing the list of all maps, maps: an array of strings that the information will be stored in to, maxNumberOfLevels: the greatest number of levels the game can support, maxStringLength: the longest a string can be that the game will support
int getAllMaps(FILE* allMapsFile, char maps[MAXNUMBEROFLEVELS][MAXSTRINGLENGTH], const int maxNumberOfLevels, const int maxStringLength) {
    
    //loop through the entire file and add them to our array
        //for each file, only add if the file exists, if not then print an error message
    
    
    //add the next line of allMapsFile to maps
    int i = 0;
    while (fgets(maps[i], maxStringLength, allMapsFile) != NULL && i < maxNumberOfLevels) {
        //already added the next line of allMapsFile to maps in the above line
        
        //remove the new line character if one exists
        removeFinalNewLineCharacter(maps[i]);
        
        
        //if the file does not actually exist then we want to remove it from maps
        
        //create the file name of this map
        char fileName[MAXSTRINGLENGTH];
        strcpy(fileName, maps[i]);
        strcat(fileName, ".txt");
        
        //if the file does not exist, then remove it
        if (fopen(fileName, "r") == NULL) {
            //print error message
            printf("Error: Level file: %s does not exist. Pokemon will continue running without this level.", maps[i]);
            //remove file from maps
            strcpy(maps[i], NOMAPFILE);
        }
        //move on to the next spot in maps only if the file exists
        else {
            i++;
        }
    }
    
    //make the remaining elements of maps NOMAPFILE
    if (i < maxNumberOfLevels) {
        for (int j = i; j < maxNumberOfLevels; j++) {
            strcpy(maps[j], NOMAPFILE);
        }
    }
    
    //the operation was successful
    return 1;
}

//fills mapsContents with the contents of mapsFile
int readMap(FILE* mapsFile, char mapsContents[MAXLINESPERMAP][MAXLINESPERMAP + 1]) {
    
    //loop through the entire file and add each line to our array
    int i = 0;
    while (fgets(mapsContents[i], MAXLINESPERMAP + 1, mapsFile) != NULL && i < MAXLINESPERMAP) {
        //already added the next line of mapsFile to mapsContents in the above line
        
        //remove the new line character if one exists and increment i
        removeFinalNewLineCharacter(mapsContents[i]);
        i++;
    }
    
    //make the remaining elements of maps ENDOFMAP
    if (i < MAXLINESPERMAP) {
        for (int j = i; j < MAXLINESPERMAP; j++) {
            strcpy(mapsContents[j], ENDOFMAP);
        }
    }
    
    //the operation was successful
    return 1;
}

//fills dual arrays of the pokemon names and numbers
//Parameters: pokemonFileName: the file name for a file that contains the list of all of a player's pokemon names and numbers, numberOfPokemon: the amount of pokemon in the file, pokemonNames: the array where pokemon names will be stored, pokemonNumbers: the array where pokemon numbers will be stored
void getPokemonNamesAndNumbers(char pokemonFileName[MAXSTRINGLENGTH], int numberOfPokemon, char pokemonNames[numberOfPokemon][MAXSTRINGLENGTH], int pokemonNumbers[numberOfPokemon]) {
    
    //open the file
    FILE* pokemonFile = fopen(pokemonFileName, "r");
    
    //loop through each two line and add the information to the relevant arrays
    for (int i = 0; i < numberOfPokemon; i++) {
        fscanf(pokemonFile, "%s", pokemonNames[i]);
        fscanf(pokemonFile, "%d", &pokemonNumbers[i]);
    }
}

//checks to see if a file exists for a pokemon and number
int fileExistsFor(Player* player, char pokemonName[MAXSTRINGLENGTH], int pokemonNumber) {
    
    //create the file name from the pokemon name and number
    char pokemonFileName[MAXSTRINGLENGTH];
    strcpy(pokemonFileName, "");
    strcat(pokemonFileName, player->name);
    strcat(pokemonFileName, " ");
    strcat(pokemonFileName, pokemonName);
    strcat(pokemonFileName, " ");
    
    char pokemonNumberAsString[MAXSTRINGLENGTH];
    strcpy(pokemonNumberAsString, "");
    sprintf(pokemonNumberAsString, "%d", pokemonNumber);
    
    strcat(pokemonFileName, pokemonNumberAsString);
    strcat(pokemonFileName, ".txt");
    
    //open the file
    FILE* pokemonFile = fopen(pokemonFileName, "r+");
    
    //check results, close the file, and return appropriate information
    if (pokemonFile == NULL) {
        fclose(pokemonFile);
        return 0;
    } else {
        fclose(pokemonFile);
        return 1;
    }
    
}


//Precondition: already check to make sure the file for this pokemon alreaady exists
//will construct a file name and store the pokemon information given the pokemon name and number
//Parameters: pokemon: the pokemon for the information to be stored in, player: the player who owns this pokemon, pokemonName: the name of the pokemon, pokemonNumber: the number of the pokemon
int getPokemonInformation(Pokemon* pokemon, Player* player, char pokemonName[MAXSTRINGLENGTH], int pokemonNumber) {
    
    //create the file name for the pokemon
    char pokemonFileName[MAXSTRINGLENGTH];
    strcpy(pokemonFileName, "");
    strcat(pokemonFileName, player->name);
    strcat(pokemonFileName, " ");
    strcat(pokemonFileName, pokemonName);
    strcat(pokemonFileName, " ");
    
    char pokemonNumberAsString[MAXSTRINGLENGTH];
    strcpy(pokemonNumberAsString, "");
    sprintf(pokemonNumberAsString, "%d", pokemonNumber);
    
    strcat(pokemonFileName, pokemonNumberAsString);
    strcat(pokemonFileName, ".txt");
    
    //open the file
    FILE* pokemonFile = fopen(pokemonFileName, "r+");
    
    //read the information and return if successful or unsuccessful
    return readPokemonInformation(pokemonFile, pokemon, pokemonName, pokemonNumber);
}


//Saves all the information from pokemonFile to pokemon
//returns 0 if there was an error reading the information and 1 if the operation was successful
int readPokemonInformation(FILE* pokemonFile, Pokemon* pokemon, char name[MAXSTRINGLENGTH], int fileNumber) {
    
    //copy the pokemon name and file number to the pokemon
    strcpy(pokemon->name, name);
    pokemon->fileNumber = fileNumber;
    
    //a string that represents the title of the information currently being read
    char informationTitle[MAXSTRINGLENGTH];
    
    //the following lines check to see if the title of the information is correct
    //if it's not: then we return 0 and don't read any more information
    //if it is: then we assume the information directly following the title is correct
    
    //Base HP
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseHP")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseHP);
    
    //Base Attack
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseAttack")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseAttack);
    
    //Defense
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseDefense")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseDefense);
    
    //Special Attack
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseSpecialAttack")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseSpecialAttack);
    
    //Special Defense
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseSpecialDefense")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseSpecialDefense);
    
    //Speed
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseSpeed")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseSpeed);
    
    //IV HP
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "IVHP")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->ivHP);
    
    //IV Attack
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "IVAttack")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->ivAttack);
    
    //IV Defense
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "IVDefense")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->ivDefense);
    
    //IV Special Attack
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "IVSpecialAttack")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->ivSpecialAttack);
    
    //IV Special Defense
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "IVSpecialDefense")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->ivSpecialDefense);
    
    //IV Speed
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "IVSpeed")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->ivSpeed);
    
    //EV HP
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "EVHP")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->evHP);
    
    //EV Attack
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "EVAttack")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->evAttack);
    
    //EV Defense
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "EVDefense")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->evDefense);
    
    //EV Special Attack
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "EVSpecialAttack")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->evSpecialAttack);
    
    //EV Special Defense
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "EVSpecialDefense")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->evSpecialDefense);
    
    //EV Speed
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "EVSpeed")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->evSpeed);
    
    //Nature
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "Nature")) {
        return 0;
    }
    char natureAsString[MAXSTRINGLENGTH];
    fscanf(pokemonFile, "%s", natureAsString);
    pokemon->nature = convertNatureStringToNature(natureAsString);
    
    //XP
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "XP")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->XP);
    
    //Level
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "Level")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->level);
    
    //Next Evolution
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "NextEvolution")) {
        return 0;
    }
    char pokemonNextEvolutionString[MAXSTRINGLENGTH];
    fscanf(pokemonFile, "%s", pokemonNextEvolutionString);
    pokemon->nextEvolution = convertPokemonStringToNextEvolution(pokemonNextEvolutionString);
    
    //Type1
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "Type1")) {
        return 0;
    }
    char pokemonType1String[MAXSTRINGLENGTH];
    fscanf(pokemonFile, "%s", pokemonType1String);
    pokemon->type1 = convertMoveTypeStringToMoveType(pokemonType1String);
    
    //Type2
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "Type2")) {
        return 0;
    }
    char pokemonType2String[MAXSTRINGLENGTH];
    fscanf(pokemonFile, "%s", pokemonType2String);
    pokemon->type2 = convertMoveTypeStringToMoveType(pokemonType2String);
    
    //Catch Rate
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "CatchRate")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->catchRate);
    
    //Level to Evolve
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "LeveltoEvolve")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->levelToEvolve);
    
    //EV Type Given
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "EVTypeGiven")) {
        return 0;
    }
    char pokemonEVTypeGivenString[MAXSTRINGLENGTH];
    fscanf(pokemonFile, "%s", pokemonEVTypeGivenString);
    pokemon->evTypeGiven = convertStatStringToStat(pokemonEVTypeGivenString);
    
    //EV Amount Given
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "EVAmountGiven")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->evAmountGiven);
    
    //Hidden EV Attack
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "HiddenEVAttack")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->hiddenEvAttack);
    
    //Hidden EV Defense
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "HiddenEVDefense")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->hiddenEvDefense);
    
    //Hidden EV Special Attack
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "HiddenEVSpecialAttack")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->hiddenEvSpecialAttack);
    
    //Hidden EV Special Defense
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "HiddenEVSpecialDefense")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->hiddenEvSpecialDefense);
           
    //Hidden EV Speed
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "HiddenEVSpeed")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->hiddenEvSpeed);
    
    //HP
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "HP")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->HP);
    
    //File Number
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "FileNumber")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->fileNumber);
    
    //Number of Moves
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "NumberofMoves")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->numberOfMoves);
    
    //for each move
    for (int i = 0; i < pokemon->numberOfMoves; i++) {
        
        //Move Name
        fscanf(pokemonFile, "%s", informationTitle);
        if (checkPokemonFileInformation(pokemon, informationTitle, "MoveName")) {
            return 0;
        }
        char moveNameString[MAXSTRINGLENGTH];
        fscanf(pokemonFile, "%s", moveNameString);
        pokemon->moves[i].name = convertMoveNameStringToMoveName(moveNameString);
        
        fillMoveInformation(&pokemon->moves[i]);
        
    }
    
    return 1;
}

//returns 1 if there is an error in the information in the Pokemon file, and 0 if there is no error
int checkPokemonFileInformation(Pokemon* pokemon, char informationTitle[MAXSTRINGLENGTH], char correctTitle[MAXSTRINGLENGTH]) {
    
    //if the informationTitle and correctTitle are not the same, then print an error and return 1
    if (strcmp(informationTitle, correctTitle) != 0) {
        printf("Error: The file for pokemon %s %d does not have the field %s in the correct location. This pokemon will not be available in game.\n", pokemon->name, pokemon->fileNumber, correctTitle);
        return 1;
    } else {
        return 0;
    }
    
}

//precondition: move already has a name
//fills move with the relevant information given the name
int fillMoveInformation(Move* move) {
    
    //copy the move name
    char moveName[MAXSTRINGLENGTH];
    convertMoveNameToString(move->name, moveName);
    
    //access the file with the move information
    char moveFileName[MAXSTRINGLENGTH];
    strcpy(moveFileName, moveName);
    strcat(moveFileName, ".txt");
    FILE* moveFile = fopen(moveFileName, "r+");
    
    //if the file does not exist, print and error and return 0
    if (moveFile == NULL) {
        printf("Error: The file for move %s does not exist. Pokemon will continue to run.\n", moveName);
        return 0;
    }
    
    //access the information from the file
    char currentLine[MAXSTRINGLENGTH];
    
    //the following lines read the informationTitle: check to see if the title is correct, and then assume the following information is correct as well
    
    //power
    fscanf(moveFile, "%s", currentLine);
    if (strcmp(currentLine, "Power") != 0) {
        printf("Error: The file for move %s has incorrect labeling for %s. Pokemon will continue to run.\n", moveName, "Power");
        return 0;
    }
    fscanf(moveFile, "%d", &move->damage);
    
    //type
    fscanf(moveFile, "%s", currentLine);
    if (strcmp(currentLine, "Type") != 0) {
        printf("Error: The file for move %s has incorrect labeling for %s. Pokemon will continue to run.\n", moveName, "Type");
        return 0;
    }
    char typeString[MAXSTRINGLENGTH];
    fscanf(moveFile, "%s", typeString);
    move->type = convertMoveTypeStringToMoveType(typeString);
    
    //physical or special
    fscanf(moveFile, "%s", currentLine);
    if (strcmp(currentLine, "PhysicalorSpecial") != 0) {
        printf("Error: The file for move %s has incorrect labeling for %s. Pokemon will continue to run.\n", moveName, "PhyscialorSpecial");
        return 0;
    }
    char physicalOrSpecialString[MAXSTRINGLENGTH];
    fscanf(moveFile, "%s", physicalOrSpecialString);
    move->isPhysicalOrSpecial = convertPhysicalOrSpecialStringToPhysicalOrSpecial(physicalOrSpecialString);
    
    //accuracy
    fscanf(moveFile, "%s", currentLine);
    if (strcmp(currentLine, "Accuracy") != 0) {
        printf("Error: The file for move %s has incorrect labeling for %s. Pokemon will continue to run.\n", moveName, "Accuracy");
        return 0;
    }
    fscanf(moveFile, "%d", &move->accuracy);
    
    //the operation was successful
    return 1;
}


//MARK: - Write to Files

//adds the string to file with fileName and reopens the file
void writeToFile(FILE* file, char fileName[MAXSTRINGLENGTH], char string[MAXSTRINGLENGTH]) {
    fprintf(file, "%s\n", string);
    fclose(file);
    fopen(fileName, "a+");
}

//clears the pokemonFile and writes all the inormation in pokemon to the pokemon's file
void writePokemonInformation(FILE* pokemonFile, char pokemonFileName[MAXSTRINGLENGTH], Pokemon* pokemon) {
    
    //clear the file
    pokemonFile = fopen(pokemonFileName, "w+");
    
    
    //write the information
    writeToFile(pokemonFile, pokemonFileName, "BaseHP");
    char pokemonHP[MAXSTRINGLENGTH];
    strcpy(pokemonHP, "");
    sprintf(pokemonHP, "%d", pokemon->baseHP);
    writeToFile(pokemonFile, pokemonFileName, pokemonHP);
    
    writeToFile(pokemonFile, pokemonFileName, "BaseAttack");
    char pokemonAttack[MAXSTRINGLENGTH];
    strcpy(pokemonAttack, "");
    sprintf(pokemonAttack, "%d", pokemon->baseAttack);
    writeToFile(pokemonFile, pokemonFileName, pokemonAttack);
    
    writeToFile(pokemonFile, pokemonFileName, "BaseDefense");
    char pokemonDefense[MAXSTRINGLENGTH];
    strcpy(pokemonDefense, "");
    sprintf(pokemonDefense, "%d", pokemon->baseDefense);
    writeToFile(pokemonFile, pokemonFileName, pokemonDefense);
    
    writeToFile(pokemonFile, pokemonFileName, "BaseSpecialAttack");
    char pokemonSpecialAttack[MAXSTRINGLENGTH];
    strcpy(pokemonSpecialAttack, "");
    sprintf(pokemonSpecialAttack, "%d", pokemon->baseSpecialAttack);
    writeToFile(pokemonFile, pokemonFileName, pokemonSpecialAttack);
    
    writeToFile(pokemonFile, pokemonFileName, "BaseSpecialDefense");
    char pokemonSpecialDefense[MAXSTRINGLENGTH];
    strcpy(pokemonSpecialDefense, "");
    sprintf(pokemonSpecialDefense, "%d", pokemon->baseSpecialDefense);
    writeToFile(pokemonFile, pokemonFileName, pokemonSpecialDefense);
    
    writeToFile(pokemonFile, pokemonFileName, "BaseSpeed");
    char pokemonSpeed[MAXSTRINGLENGTH];
    strcpy(pokemonSpeed, "");
    sprintf(pokemonSpeed, "%d", pokemon->baseSpeed);
    writeToFile(pokemonFile, pokemonFileName, pokemonSpeed);
    
    writeToFile(pokemonFile, pokemonFileName, "IVHP");
    char pokemonIVHP[MAXSTRINGLENGTH];
    strcpy(pokemonIVHP, "");
    sprintf(pokemonIVHP, "%d", pokemon->ivHP);
    writeToFile(pokemonFile, pokemonFileName, pokemonIVHP);
    
    writeToFile(pokemonFile, pokemonFileName, "IVAttack");
    char pokemonIVAttack[MAXSTRINGLENGTH];
    strcpy(pokemonIVAttack, "");
    sprintf(pokemonIVAttack, "%d", pokemon->ivAttack);
    writeToFile(pokemonFile, pokemonFileName, pokemonIVAttack);
    
    writeToFile(pokemonFile, pokemonFileName, "IVDefense");
    char pokemonIVDefense[MAXSTRINGLENGTH];
    strcpy(pokemonIVDefense, "");
    sprintf(pokemonIVDefense, "%d", pokemon->ivDefense);
    writeToFile(pokemonFile, pokemonFileName, pokemonIVDefense);
    
    writeToFile(pokemonFile, pokemonFileName, "IVSpecialAttack");
    char pokemonIVSpecialAttack[MAXSTRINGLENGTH];
    strcpy(pokemonIVSpecialAttack, "");
    sprintf(pokemonIVSpecialAttack, "%d", pokemon->ivSpecialAttack);
    writeToFile(pokemonFile, pokemonFileName, pokemonIVSpecialAttack);
    
    writeToFile(pokemonFile, pokemonFileName, "IVSpecialDefense");
    char pokemonIVSpecialDefense[MAXSTRINGLENGTH];
    strcpy(pokemonIVSpecialDefense, "");
    sprintf(pokemonIVSpecialDefense, "%d", pokemon->ivSpecialDefense);
    writeToFile(pokemonFile, pokemonFileName, pokemonIVSpecialDefense);
    
    writeToFile(pokemonFile, pokemonFileName, "IVSpeed");
    char pokemonIVSpeed[MAXSTRINGLENGTH];
    strcpy(pokemonIVSpeed, "");
    sprintf(pokemonIVSpeed, "%d", pokemon->ivSpeed);
    writeToFile(pokemonFile, pokemonFileName, pokemonIVSpeed);
    
    writeToFile(pokemonFile, pokemonFileName, "EVHP");
    char pokemonEVHP[MAXSTRINGLENGTH];
    strcpy(pokemonEVHP, "");
    sprintf(pokemonEVHP, "%d", pokemon->evHP);
    writeToFile(pokemonFile, pokemonFileName, pokemonEVHP);
    
    writeToFile(pokemonFile, pokemonFileName, "EVAttack");
    char pokemonEVAttack[MAXSTRINGLENGTH];
    strcpy(pokemonEVAttack, "");
    sprintf(pokemonEVAttack, "%d", pokemon->evAttack);
    writeToFile(pokemonFile, pokemonFileName, pokemonEVAttack);
    
    writeToFile(pokemonFile, pokemonFileName, "EVDefense");
    char pokemonEVDefense[MAXSTRINGLENGTH];
    strcpy(pokemonEVDefense, "");
    sprintf(pokemonEVDefense, "%d", pokemon->evDefense);
    writeToFile(pokemonFile, pokemonFileName, pokemonEVDefense);
    
    writeToFile(pokemonFile, pokemonFileName, "EVSpecialAttack");
    char pokemonEVSpecialAttack[MAXSTRINGLENGTH];
    strcpy(pokemonEVSpecialAttack, "");
    sprintf(pokemonEVSpecialAttack, "%d", pokemon->evSpecialAttack);
    writeToFile(pokemonFile, pokemonFileName, pokemonEVSpecialAttack);
    
    writeToFile(pokemonFile, pokemonFileName, "EVSpecialDefense");
    char pokemonEVSpecialDefense[MAXSTRINGLENGTH];
    strcpy(pokemonEVSpecialDefense, "");
    sprintf(pokemonEVSpecialDefense, "%d", pokemon->evSpecialDefense);
    writeToFile(pokemonFile, pokemonFileName, pokemonEVSpecialDefense);
    
    writeToFile(pokemonFile, pokemonFileName, "EVSpeed");
    char pokemonEVSpeed[MAXSTRINGLENGTH];
    strcpy(pokemonEVSpeed, "");
    sprintf(pokemonEVSpeed, "%d", pokemon->evSpeed);
    writeToFile(pokemonFile, pokemonFileName, pokemonEVSpeed);
    
    writeToFile(pokemonFile, pokemonFileName, "Nature");
    char natureAsString[MAXSTRINGLENGTH];
    convertNatureToString(pokemon->nature, natureAsString);
    writeToFile(pokemonFile, pokemonFileName, natureAsString);
    
    writeToFile(pokemonFile, pokemonFileName, "XP");
    char pokemonXP[MAXSTRINGLENGTH];
    strcpy(pokemonXP, "");
    sprintf(pokemonXP, "%d", pokemon->XP);
    writeToFile(pokemonFile, pokemonFileName, pokemonXP);
    
    writeToFile(pokemonFile, pokemonFileName, "Level");
    char pokemonLevelString[MAXSTRINGLENGTH];
    strcpy(pokemonLevelString, "");
    sprintf(pokemonLevelString, "%d", pokemon->level);
    writeToFile(pokemonFile, pokemonFileName, pokemonLevelString);
    
    writeToFile(pokemonFile, pokemonFileName, "NextEvolution");
    char pokemonNextEvolution[MAXSTRINGLENGTH];
    strcpy(pokemonNextEvolution, "");
    convertPokemonNextEvolutionToString(pokemon->nextEvolution, pokemonNextEvolution);
    writeToFile(pokemonFile, pokemonFileName, pokemonNextEvolution);
    
    writeToFile(pokemonFile, pokemonFileName, "Type1");
    char pokemonType1[MAXSTRINGLENGTH];
    strcpy(pokemonType1, "");
    convertMoveTypeToString(pokemon->type1, pokemonType1);
    writeToFile(pokemonFile, pokemonFileName, pokemonType1);
    
    writeToFile(pokemonFile, pokemonFileName, "Type2");
    char pokemonType2[MAXSTRINGLENGTH];
    strcpy(pokemonType2, "");
    convertMoveTypeToString(pokemon->type2, pokemonType2);
    writeToFile(pokemonFile, pokemonFileName, pokemonType2);
    
    writeToFile(pokemonFile, pokemonFileName, "CatchRate");
    char pokemonCatchRate[MAXSTRINGLENGTH];
    strcpy(pokemonCatchRate, "");
    sprintf(pokemonCatchRate, "%d", pokemon->catchRate);
    writeToFile(pokemonFile, pokemonFileName, pokemonCatchRate);
    
    writeToFile(pokemonFile, pokemonFileName, "LeveltoEvolve");
    char pokemonLevelToEvolve[MAXSTRINGLENGTH];
    strcpy(pokemonLevelToEvolve, "");
    sprintf(pokemonLevelToEvolve, "%d", pokemon->levelToEvolve);
    writeToFile(pokemonFile, pokemonFileName, pokemonLevelToEvolve);
    
    writeToFile(pokemonFile, pokemonFileName, "EVTypeGiven");
    char pokemonEVTypeGiven[MAXSTRINGLENGTH];
    convertStatToString(pokemon->evTypeGiven, pokemonEVTypeGiven);
    writeToFile(pokemonFile, pokemonFileName, pokemonEVTypeGiven);
    
    writeToFile(pokemonFile, pokemonFileName, "EVAmountGiven");
    char pokemonEVAmountGiven[MAXSTRINGLENGTH];
    strcpy(pokemonEVAmountGiven, "");
    sprintf(pokemonEVAmountGiven, "%d", pokemon->evAmountGiven);
    writeToFile(pokemonFile, pokemonFileName, pokemonEVAmountGiven);
    
    writeToFile(pokemonFile, pokemonFileName, "HiddenEVAttack");
    char hiddenEvAttack[MAXSTRINGLENGTH];
    strcpy(hiddenEvAttack, "");
    sprintf(hiddenEvAttack, "%d", pokemon->hiddenEvAttack);
    writeToFile(pokemonFile, pokemonFileName, hiddenEvAttack);
    
    writeToFile(pokemonFile, pokemonFileName, "HiddenEVDefense");
    char hiddenEvDefense[MAXSTRINGLENGTH];
    strcpy(hiddenEvDefense, "");
    sprintf(hiddenEvDefense, "%d", pokemon->hiddenEvDefense);
    writeToFile(pokemonFile, pokemonFileName, hiddenEvDefense);
    
    writeToFile(pokemonFile, pokemonFileName, "HiddenEVSpecialAttack");
    char hiddenEvSpecialAttack[MAXSTRINGLENGTH];
    strcpy(hiddenEvSpecialAttack, "");
    sprintf(hiddenEvSpecialAttack, "%d", pokemon->hiddenEvSpecialAttack);
    writeToFile(pokemonFile, pokemonFileName, hiddenEvSpecialAttack);
    
    writeToFile(pokemonFile, pokemonFileName, "HiddenEVSpecialDefense");
    char hiddenEvSpecialDefense[MAXSTRINGLENGTH];
    strcpy(hiddenEvSpecialDefense, "");
    sprintf(hiddenEvSpecialDefense, "%d", pokemon->hiddenEvSpecialDefense);
    writeToFile(pokemonFile, pokemonFileName, hiddenEvSpecialDefense);
    
    writeToFile(pokemonFile, pokemonFileName, "HiddenEVSpeed");
    char hiddenEvSpeed[MAXSTRINGLENGTH];
    strcpy(hiddenEvSpeed, "");
    sprintf(hiddenEvSpeed, "%d", pokemon->hiddenEvSpeed);
    writeToFile(pokemonFile, pokemonFileName, hiddenEvSpeed);
    
    writeToFile(pokemonFile, pokemonFileName, "HP");
    char realHP[MAXSTRINGLENGTH];
    strcpy(realHP, "");
    sprintf(realHP, "%d", pokemon->HP);
    writeToFile(pokemonFile, pokemonFileName, realHP);
    
    writeToFile(pokemonFile, pokemonFileName, "FileNumber");
    char pokemonNumberAsString[MAXSTRINGLENGTH];
    strcpy(pokemonNumberAsString, "");
    sprintf(pokemonNumberAsString, "%d", pokemon->fileNumber);
    writeToFile(pokemonFile, pokemonFileName, pokemonNumberAsString);
    
    writeToFile(pokemonFile, pokemonFileName, "NumberofMoves");
    char numberOfMovesAsString[MAXSTRINGLENGTH];
    strcpy(numberOfMovesAsString, "");
    sprintf(numberOfMovesAsString, "%d", pokemon->numberOfMoves);
    writeToFile(pokemonFile, pokemonFileName, numberOfMovesAsString);
    
    //for each move, write the information
    for (int i = 0; i < pokemon->numberOfMoves; i++) {
        
        writeToFile(pokemonFile, pokemonFileName, "MoveName");
        char moveNameAsString[MAXSTRINGLENGTH];
        convertMoveNameToString(pokemon->moves[i].name, moveNameAsString);
        writeToFile(pokemonFile, pokemonFileName, moveNameAsString);
        
    }
    
    
}


//MARK: - Player Files

//returns the unlockedMapsFile for the player, creates a new file if one doesn't already exist
FILE* getPlayerUnlockedMapsFile(char* playerName) {
    
    //create the file name
    char fileName[strlen(UNLOCKEDMAPSFILE) + strlen(playerName) + strlen(".txt")];
    strcpy(fileName, UNLOCKEDMAPSFILE);
    strcat(fileName, playerName);
    strcat(fileName, ".txt");
    
    //open the file
    FILE* playerUnlockedMapsFile = fopen(fileName, "r+");
    
    //create a new file if the file didn't exist
    if (playerUnlockedMapsFile == NULL) {
        playerUnlockedMapsFile = fopen(fileName, "w+");
    }
    
    //return the file pointer
    return playerUnlockedMapsFile;
}

//returns the pokemonFile for the player, creates a new file if one doesn't already exist
FILE* getPlayerPokemon(char* playerName) {
    
    //create the file name
    char fileName[strlen(MYPOKEMON) + strlen(playerName) + strlen(".txt")];
    strcpy(fileName, MYPOKEMON);
    strcat(fileName, playerName);
    strcat(fileName, ".txt");
    
    //open the file
    FILE* playerPokemonFile = fopen(fileName, "r+");
    
    //create a new file if the file didn't exist
    if (playerPokemonFile == NULL) {
        playerPokemonFile = fopen(fileName, "w+");
    }
    
    //return the file pointer
    return playerPokemonFile;
}


//MARK: - Pokemon Creation Files

//get the numberOfPokemon in the game
int getNumberOfPokemon() {
    //open the all pokemon file
    FILE* allPokemonFile = fopen(ALLPOKEMONFILE, "r+");
    
    //if the file doesn't exist
    if (allPokemonFile == NULL) {
        printf("Error: %s file does not exist. Pokemon will continue to run but will have 0 pokemon in the game.\n", ALLPOKEMONFILE);
        return 0;
    }
    
    //first read in the numberOfPokemon and return
    int numberOfPokemon = 0;
    fscanf(allPokemonFile, "%d", &numberOfPokemon);
    return numberOfPokemon;
}

//get list of all the Pokemon in the game and stores them in allPokemon
//Pre-condition: allPokemonFile exists
void getListOfAllPokemon(char allPokemon[getNumberOfPokemon()][MAXSTRINGLENGTH]) {
    
    //open the allPokemonFile
    FILE* allPokemonFile = fopen(ALLPOKEMONFILE, "r+");
    
    //read in all the Pokemon names
    for (int i = 0; i < getNumberOfPokemon(); i++) {
        fscanf(allPokemonFile, "%s", allPokemon[i]);
    }
    
}


//checks to see if the passed in Pokemon is in the All Pokemon file
int doesPokemonExist(char pokemonName[MAXSTRINGLENGTH]) {
    
    //create a list of all the Pokemon in the game
    char allPokemon[getNumberOfPokemon()][MAXSTRINGLENGTH];
    getListOfAllPokemon(allPokemon);
    
    //loops through and compares the names
    for (int i = 0; i < getNumberOfPokemon(); i++) {
        if (!strcmp(pokemonName, allPokemon[i])) {
            return 1;
        }
    }
    //if it hasn't returned by now, the pokemon does not exist in allPokemonFile
    return 0;
    
}


//MARK: - Startup

//creates a Player pointer to be used for the rest of the game, starts a new user or continues an existing user
//presents the initial menu for starting the game
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
            printf("Sorry, please enter yes or no.\nDo you have an existing user you'd like to player as?\n");
        } else {
            printf("Sorry, please enter yes, no, or quit.\nDo you have an existing user you'd like to player as?\n");
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
        
        //handle if the player has already player but they said they were a new user
        while (playerHasPlayed(playerName)) {
            
            //ask if they still want to play
            printf("This user already has save data. Would you still like to play as %s?\n", playerName);
            char continuePlayResponse[MAXSTRINGLENGTH];
            scanf("%s", continuePlayResponse);
            
            //handle incorrect responses
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
                    printf("Sorry, please enter yes or no.\nDo you have an existing user you'd like to player as?\n");
                } else {
                    printf("Sorry, please enter yes, no, or quit.\nDo you have an existing user you'd like to player as?\n");
                }
                
                //take in new response and increase counter
                scanf("%s", continuePlayResponse);
                counter1++;
                
            }
            
            //we now know the response is either yes or no
            //if yes
            if (!strcmp(continuePlayResponse, "Yes") || !strcmp(continuePlayResponse, "yes")) {
                isNewUser = 0;
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
        
        //handle if they haven't played but they said they are a returning user
        while (!playerHasPlayed(playerName)) {
            
            //ask if they want to create a new user
            printf("No save data exists for this user. Would you like to create new user %s?\n", playerName);
            char continuePlayResponse[MAXSTRINGLENGTH];
            scanf("%s", continuePlayResponse);
            
            //handle incorrect responses
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
                    printf("Sorry, please enter yes or no.\nDo you have an existing user you'd like to player as?\n");
                } else {
                    printf("Sorry, please enter yes, no, or quit.\nDo you have an existing user you'd like to player as?\n");
                }
                
                //take in new response and increase counter
                scanf("%s", continuePlayResponse);
                counter1++;
            }
            
            //we now know the response is either yes or no
            //if yes
            if (!strcmp(continuePlayResponse, "Yes") || !strcmp(continuePlayResponse, "yes")) {
                isNewUser = 1;
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
    Player player;
    createPlayer(playerName, &player);
    
    //start main menu
    while (startMainMenu(&player, isNewUser)) {
        isNewUser = 0;
    }
    
    
    //after the game
    endGame();
    
}


                
//presents the main menu of the game
int startMainMenu(Player *player, int isNewPlayer) {
    
    //new player
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
        endGame();
        return 0;
    }
    
    //get list of all maps
    char maps[MAXNUMBEROFLEVELS][MAXSTRINGLENGTH];
    getAllMaps(allMapsFile, maps, MAXNUMBEROFLEVELS, MAXSTRINGLENGTH);
    fclose(allMapsFile);
    
    //get list of all the player's unlocked maps
    char unlockedMaps[MAXNUMBEROFLEVELS][MAXSTRINGLENGTH];
    getAllMaps(getPlayerUnlockedMapsFile(player->name), unlockedMaps, MAXNUMBEROFLEVELS, MAXSTRINGLENGTH);
    
    printf("\nAvailable Maps\n");
    
    //present list of maps saying locked or unlocked
    int mapNumber = 0;
    for (int i = 0; i < MAXNUMBEROFLEVELS; i++) {
        
        //don't run the code if there is not map file
        if (!strcmp(maps[i], NOMAPFILE)) {
            continue;
        }
        mapNumber++;
        
        //check to see if map is unlocked
        int mapIsUnlocked = isMapUnlocked(maps[i], unlockedMaps);
        
        //print the appropriate message if the map is locked or unlocked
        if (mapIsUnlocked) {
            printf("%d. %s: Unlocked\n", mapNumber, maps[i]);
        } else {
            printf("%d. %s: Locked\n", mapNumber, maps[i]);
        }
        
        
    }
    
    
    //allow the player to select a map
    printf("What is the number of the map you would like to play?\n");
    
    
    //stores the value of the selected map and decreases it because arrays are 0 indexed
    int selectedMap = 0;
    scanf("%d", &selectedMap);
    selectedMap--;
    
    //make sure the selected map is valid and unlocked
    while (selectedMap < 0 || selectedMap >= mapNumber || !isMapUnlocked(maps[selectedMap], unlockedMaps)) {
        
        //if the number is not valid
        if (selectedMap < 0 || selectedMap >= mapNumber) {
            printf("That is not a valid map number. Please enter a number between %d and %d.\n", 1, mapNumber);
        }
        //if the map is not unlocked
        else if (!isMapUnlocked(maps[selectedMap], unlockedMaps)) {
            printf("That map is not unlocked, please enter the number of the unlocked map you'd like to play.\n");
        }
        //Assign the new input
        scanf("%d", &selectedMap);
        selectedMap--;
        
    }
    
    //store if the player wants to choose another map
    int firstGameResult;
    
    //start the game
    if (isNewPlayer) {
        //choose a starter Pokemon is they are a new player
        Pokemon starterPokemon;
        chooseStarterPokemon(player, &starterPokemon);
        //start map
        firstGameResult = startMap(player, isNewPlayer, maps[selectedMap]);
    } else {
        //start map
        firstGameResult = startMap(player, isNewPlayer, maps[selectedMap]);
    }
    
    //if the player wants to choose another map to play
    return firstGameResult;
    
}

//fill player with relevant information given playerName
//returns based on whether or not it was successful
int createPlayer(char playerName[MAXSTRINGLENGTH], Player* player) {
    
    //Player player;
    strcpy(player->name, playerName);
    
    //fill the number of pokemon as 0 for now
    //add the unlockedMapsFile and pokemonFile
    player->unlockedMapsFile = getPlayerUnlockedMapsFile(playerName);
    player->pokemonFile = getPlayerPokemon(playerName);
    player->numberOfPokemon = 0;
    
    //add the unlockedMapsFileName
    char unlockedMapsFileName[strlen(UNLOCKEDMAPSFILE) + strlen(playerName) + strlen(".txt")];
    strcpy(unlockedMapsFileName, UNLOCKEDMAPSFILE);
    strcat(unlockedMapsFileName, playerName);
    strcat(unlockedMapsFileName, ".txt");
    strcpy(player->unlockedMapsFileName, unlockedMapsFileName);
    
    //add the pokemonFileName
    char pokemonFileName[strlen(MYPOKEMON) + strlen(playerName) + strlen(".txt")];
    strcpy(pokemonFileName, MYPOKEMON);
    strcat(pokemonFileName, playerName);
    strcat(pokemonFileName, ".txt");
    strcpy(player->pokemonFileName, pokemonFileName);
    
    
    //get the number of Pokemon
    FILE* pokemonFile = fopen(player->pokemonFileName, "r+");
    char pokemonName[MAXSTRINGLENGTH];
    //check if there's a line
    while (fgets(pokemonName, MAXSTRINGLENGTH, pokemonFile) != NULL) {
        //increase number of Pokemon
        player->numberOfPokemon++;
        //clear the number
        fgets(pokemonName, MAXSTRINGLENGTH, pokemonFile);
    }
    
    //initialize the maps file
    writeToFile(player->unlockedMapsFile, player->unlockedMapsFileName, FIRSTUNLOCKEDMAP);
    
    //the operation was successful
    return 1;
}



//MARK: - Pokemon

//presents the menus and handles choosing a starter Pokemon
//Parameters: the player playing the game, a pokemon to store the informatino of the starterPokemon
void chooseStarterPokemon(Player* player, Pokemon* starterPokemon) {
    
    //ask which Pokemon they want to start with and store the response
    printf("Which starter Pokemon would you like: Charmander, Bulbasaur, or Squirtle?\n");
    char response[MAXSTRINGLENGTH];
    scanf("%s", response);
    
    //handle invalid input
    while (!(!strcmp(response, "Charmander") || !strcmp(response, "charmander") || !strcmp(response, "Bulbasaur") || !strcmp(response, "bulbasaur") || !strcmp(response, "Squirtle") || !strcmp(response, "squirtle"))) {
        printf("That is not a valid response. Which starter Pokemon would you like: Charmander, Bulbasaur, or Squirtle?\n");
        scanf("%s", response);
    }
    
    
    //create the Pokemon and add it to the player
    if (!strcmp(response, "Charmander") || !strcmp(response, "charmander")) {
        createPokemon(starterPokemon, charmander);
    } else if (!strcmp(response, "Bulbasaur") || !strcmp(response, "bulbasaur")) {
        createPokemon(starterPokemon, bulbasaur);
    } else if (!strcmp(response, "Squirtle") || !strcmp(response, "squirtle")) {
        createPokemon(starterPokemon, squirtle);
    }
    addPokemonToPlayer(player, starterPokemon);
}

//adds a pokemon to the files of a player
void addPokemonToPlayer(Player* player, Pokemon* pokemon) {
    
    //figure out the number: start with 1
    int number = 1;
    
    //loop through all the pokemon in the player's pokemonFile
    for (int i = 0; i < player->numberOfPokemon; i++) {
        //read the name and number from the file
        char pokemonName[MAXSTRINGLENGTH];
        int pokemonNumber;
        
        fscanf(player->pokemonFile, "%s", pokemonName);
        fscanf(player->pokemonFile, "%d", &pokemonNumber);
        
        //if this is the Pokemon, then increase the number
        if (!strcmp(pokemonName, pokemon->name)) {
            number++;
        }
    }
    
    //convert the number to a string
    char numberAsString[MAXSTRINGLENGTH];
    strcpy(numberAsString, "");
    sprintf(numberAsString, "%d", number);
    
    
    //create Pokemon file name
    char pokemonFileName[MAXSTRINGLENGTH];
    strcpy(pokemonFileName, player->name);
    strcat(pokemonFileName, " ");
    strcat(pokemonFileName, pokemon->name);
    strcat(pokemonFileName, " ");
    strcat(pokemonFileName, numberAsString);
    strcat(pokemonFileName, ".txt");
    
    //open the pokemon file
    FILE* pokemonFile = fopen(pokemonFileName, "r+");
    
    //check to make sure that the file does not exist, if it does then increase the number and recreate the file
    while (pokemonFile != NULL) {
        
        //increase the number
        number++;
        
        //convert the number to a string
        char numberAsString[MAXSTRINGLENGTH];
        strcpy(numberAsString, "");
        sprintf(numberAsString, "%d", number);
        
        
        //create Pokemon file
        strcpy(pokemonFileName, player->name);
        strcat(pokemonFileName, " ");
        strcat(pokemonFileName, pokemon->name);
        strcat(pokemonFileName, " ");
        strcat(pokemonFileName, numberAsString);
        strcat(pokemonFileName, ".txt");
        
        pokemonFile = fopen(pokemonFileName, "r+");
        
        
    }
    
    
    //create the file
    pokemonFile = fopen(pokemonFileName, "w+");
    
    //save the file number
    pokemon->fileNumber = number;
    
    //fill the file
    writePokemonInformation(pokemonFile, pokemonFileName, pokemon);
    
    
    //add the pokemon to the list of pokemon for the player
    writeToFile(player->pokemonFile, player->pokemonFileName, pokemon->name);
    writeToFile(player->pokemonFile, player->pokemonFileName, numberAsString);
    player->numberOfPokemon = player->numberOfPokemon + 1;
    
}


//MARK: - Creating Pokemon

//stores the information for a new pokemon in parameter pokemon given the pokemonName
int createPokemon(Pokemon* pokemon, enum pokemonName pokemonName) {
    
    //create the file name where the information of this pokemonName is stored
    char pokemonFileName[MAXSTRINGLENGTH];
    convertPokemonNextEvolutionToString(pokemonName, pokemonFileName);
    strcat(pokemonFileName, ".txt");
    
    //open the file and check for errors
    FILE* pokemonFile = fopen(pokemonFileName, "r+");
    if (pokemonFile == NULL) {
        printf("Error: %s file does not exist. Pokemon will continue to run.\n", pokemonFileName);
    }
    
    //copy the name of the Pokemon
    char pokemonNameString[MAXSTRINGLENGTH];
    convertPokemonNextEvolutionToString(pokemonName, pokemonNameString);
    strcpy(pokemon->name, pokemonNameString);
    
    //stores the title of the information currently being read
    char informationTitle[MAXSTRINGLENGTH];
    
    //read the information
    //this is different from the readPokemon function because the information is slightly different because this file only stores generic starting information for all pokemon with pokemonName. readPokemon reads information about a specific instance of a pokemon
    
    //Base HP
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseHP")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseHP);
    
    //Base Attack
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseAttack")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseAttack);
    
    //Defense
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseDefense")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseDefense);
    
    //Special Attack
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseSpecialAttack")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseSpecialAttack);
    
    //Special Defense
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseSpecialDefense")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseSpecialDefense);
    
    //Speed
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "BaseSpeed")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->baseSpeed);
    
    //EV Type Given
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "EVTypeGiven")) {
        return 0;
    }
    char pokemonEVTypeGivenString[MAXSTRINGLENGTH];
    fscanf(pokemonFile, "%s", pokemonEVTypeGivenString);
    pokemon->evTypeGiven = convertStatStringToStat(pokemonEVTypeGivenString);
    
    //EV Amount Given
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "EVAmountGiven")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->evAmountGiven);
    
    //Type1
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "Type1")) {
        return 0;
    }
    char pokemonType1String[MAXSTRINGLENGTH];
    fscanf(pokemonFile, "%s", pokemonType1String);
    pokemon->type1 = convertMoveTypeStringToMoveType(pokemonType1String);
    
    //Type2
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "Type2")) {
        return 0;
    }
    char pokemonType2String[MAXSTRINGLENGTH];
    fscanf(pokemonFile, "%s", pokemonType2String);
    pokemon->type2 = convertMoveTypeStringToMoveType(pokemonType2String);
    
    //Level to Evolve
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "LevelToEvolve")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->levelToEvolve);
    
    //Next Evolution
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "NextEvolution")) {
        return 0;
    }
    char pokemonNextEvolutionString[MAXSTRINGLENGTH];
    fscanf(pokemonFile, "%s", pokemonNextEvolutionString);
    pokemon->nextEvolution = convertPokemonStringToNextEvolution(pokemonNextEvolutionString);
    
    //Catch Rate
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "CatchRate")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->catchRate);
    
    //Number of Moves
    fscanf(pokemonFile, "%s", informationTitle);
    if (checkPokemonFileInformation(pokemon, informationTitle, "NumberOfMoves")) {
        return 0;
    }
    fscanf(pokemonFile, "%d", &pokemon->numberOfMoves);
    
    //for each move
    for (int i = 0; i < pokemon->numberOfMoves; i++) {
        
        //Move Name
        char moveNameString[MAXSTRINGLENGTH];
        fscanf(pokemonFile, "%s", moveNameString);
        pokemon->moves[i].name = convertMoveNameStringToMoveName(moveNameString);
        
    }
    
    //generate random numbers for the ivs
    pokemon->ivHP = getRandomNumber(0, 31);
    pokemon->ivAttack = getRandomNumber(0, 31);
    pokemon->ivDefense = getRandomNumber(0, 31);
    pokemon->ivSpecialAttack = getRandomNumber(0, 31);
    pokemon->ivSpecialDefense = getRandomNumber(0, 31);
    pokemon->ivSpeed = getRandomNumber(0, 31);
    
    //the evs and hidden evs start at 0
    pokemon->evHP = 0;
    pokemon->evAttack = 0;
    pokemon->evDefense = 0;
    pokemon->evSpecialAttack = 0;
    pokemon->evSpecialDefense = 0;
    pokemon->evSpeed = 0;
    
    pokemon->hiddenEvAttack = 0;
    pokemon->hiddenEvDefense = 0;
    pokemon->hiddenEvSpecialAttack = 0;
    pokemon->hiddenEvSpecialDefense = 0;
    pokemon->hiddenEvSpeed = 0;
    
    //create a random nature
    pokemon->nature = getRandomNumber(0, lastNature - 1);
    
    //start with 0 XP
    pokemon->XP = 0;
    
    //create the starting level based on what level they evolve at
    if (pokemon->levelToEvolve == 4) {
        pokemon->level = 1;
    } else if (pokemon->levelToEvolve == 8) {
        pokemon->level = 5;
    } else if (pokemon->levelToEvolve == -1) {
        pokemon->level = 9;
    }
    
    //give the pokemon max hp
    pokemon->HP = getMaxHP(*pokemon);
    
    
    //fill out the moves information
    for (int i = 0; i < pokemon->numberOfMoves; i++) {
        
        //get the move name
        enum moveName moveName = pokemon->moves[i].name;
        
        //convert to a string
        char moveNameString[MAXSTRINGLENGTH];
        convertMoveNameToString(moveName, moveNameString);
        
        //get the move file
        char moveFileName[MAXSTRINGLENGTH];
        strcpy(moveFileName, moveNameString);
        strcat(moveFileName, ".txt");
        FILE* moveFile = fopen(moveFileName, "r+");
        
        //if the move file doesn't exist
        if (moveFile == NULL) {
            printf("Error: The file for move %s does not exist. Pokemon will continue to run.\n", moveNameString);
        }
        
        //get the information
        char currentLine[MAXSTRINGLENGTH];
        
        //power
        fscanf(moveFile, "%s", currentLine);
        if (strcmp(currentLine, "Power") != 0) {
            printf("Error: The file for move %s has incorrect labeling for %s. Pokemon will continue to run.\n", moveNameString, "Power");
            return 0;
        }
        fscanf(moveFile, "%d", &pokemon->moves[i].damage);
        
        //type
        fscanf(moveFile, "%s", currentLine);
        if (strcmp(currentLine, "Type") != 0) {
            printf("Error: The file for move %s has incorrect labeling for %s. Pokemon will continue to run.\n", moveNameString, "Type");
            return 0;
        }
        char typeString[MAXSTRINGLENGTH];
        fscanf(moveFile, "%s", typeString);
        pokemon->moves[i].type = convertMoveTypeStringToMoveType(typeString);
        
        //physical or special
        fscanf(moveFile, "%s", currentLine);
        if (strcmp(currentLine, "PhysicalorSpecial") != 0) {
            printf("Error: The file for move %s has incorrect labeling for %s. Pokemon will continue to run.\n", moveNameString, "PhyscialorSpecial");
            return 0;
        }
        char physicalOrSpecialString[MAXSTRINGLENGTH];
        fscanf(moveFile, "%s", physicalOrSpecialString);
        pokemon->moves[i].isPhysicalOrSpecial = convertPhysicalOrSpecialStringToPhysicalOrSpecial(physicalOrSpecialString);
        
        //accuracy
        fscanf(moveFile, "%s", currentLine);
        if (strcmp(currentLine, "Accuracy") != 0) {
            printf("Error: The file for move %s has incorrect labeling for %s. Pokemon will continue to run.\n", moveNameString, "Accuracy");
            return 0;
        }
        fscanf(moveFile, "%d", &pokemon->moves[i].accuracy);
        
        
    }
    
    //give a file number of 0 for now
    //this will change once the pokemon is assigned to a player
    pokemon->fileNumber = 0;
    
    //the operation was successful
    return 1;
    
}


//MARK: - Dealing with Maps


//preconditions are that unlockedMaps has length MAXNUMBEROFLEVELS and those strings have length MAXSTRINGLENGTH
//returns 1 if the map is unlocked, and 0 if the map is not unlocked
int isMapUnlocked(char map[MAXSTRINGLENGTH], char unlockedMaps[MAXNUMBEROFLEVELS][MAXSTRINGLENGTH]) {
    
    //check to see if map is unlocked
    //loop through unlocked maps
    for (int i = 0; i < MAXNUMBEROFLEVELS; i++) {
        
        //we have reached the end of map, and the map is locked
        if (!strcmp(unlockedMaps[i], NOMAPFILE)) {
            return 0;
        }
        
        //the map is unlocked
        if (!strcmp(map, unlockedMaps[i])) {
            return 1;
        }
    }
    return 0;
    
}

//Precondition: mapFileName is the name of a map file that exists
//Returns 1 if a new map has to be presented again, returns 0 if the game will now end
int startMap(Player* player, int isNewPlayer, char mapFileName[MAXSTRINGLENGTH]) {
    
    //add .txt to the file name
    char mapFileNameTxt[MAXSTRINGLENGTH];
    strcpy(mapFileNameTxt, mapFileName);
    strcat(mapFileNameTxt, ".txt");
    
    //open the map file
    FILE* mapFile = fopen(mapFileNameTxt, "r+");
    
    //quit the game if the map file does not exist
    if (mapFile == NULL) {
        printf("Error: File with file name %s does not exist.\nPokemon will now quit. Goodbye!\n", mapFileName);
        endGame();
        return 0;
    }
    
    //get an array from the map file
    char map[MAXLINESPERMAP][MAXLINESPERMAP + 1];
    readMap(mapFile, map);
    
    
    //figure out height of map
    int mapHeight = 0;
    for (int row = 0; row < MAXLINESPERMAP; row++) {
        if (strcmp(map[row], ENDOFMAP) != 0) {
            mapHeight++;
        } else {
            break;
        }
    }
    
    
    //place the player at an available spot
    int playerPosition[2];
    findAvailableSpot(map, mapHeight, playerPosition);
    int playerRow = playerPosition[0];
    int playerColumn = playerPosition[1];
    
    //check for errors
    if (playerRow == -1 || playerColumn == -1) {
        endGame();
        return 0;
    }
    
    //loop until the player quits
    int hasQuit = 0;
    while (!hasQuit) {
        
        //present the map
        printMapAndKeyAndPlayer(map, mapHeight, playerRow, playerColumn);
        
        
        //ask for input
        printf("Please enter a direction (W, A, S, or D) and a number for distance to move your player, enter quit to quit and save the game, enter new to save and go to a new map, or enter pokemon to see your pokemon.\n");
        char input[MAXSTRINGLENGTH];
        scanf("%s", input);
        
        //handle invalid input
        while (!( !strcmp(input, "W") || !strcmp(input, "w") || !strcmp(input, "S") || !strcmp(input, "s") || !strcmp(input, "A") ||  !strcmp(input, "a") || !strcmp(input, "D") || !strcmp(input, "d") || !strcmp(input, "Quit") || !strcmp(input, "quit") || !strcmp(input, "New") || !strcmp(input, "new") || !strcmp(input, "Pokemon") || !strcmp(input, "pokemon"))) {
            printf("Sorry, please enter a direction (W, A, S, or D) and a number for distance to move your player, enter quit to quit and save the game, enter new to save and go to a new map, or enter pokemon to see your pokemon.\n");
            scanf("%s", input);
        }
        
        //if they want to quit
        if (!strcmp(input, "Quit") || !strcmp(input, "quit")) {
            printf("Quitting and saving the game. Thanks for playing!\n");
            hasQuit = 1;
            return 0;
        }
        
        //if they want to change maps
        if (!strcmp(input, "New") || !strcmp(input, "new")) {
            hasQuit = 1;
            return 1;
        }
        
        //if they want to see their pokemon
        if (!strcmp(input, "Pokemon") || !strcmp(input, "pokemon")) {
            presentPokemon(player);
            continue;
        }
        
        
        //they want to move
        int dx = 0;
        int dy = 0;
        
        //take in the distance and assign to dx or dy based on the direction
        if (!strcmp(input, "W") || !strcmp(input, "w")) {
            scanf("%d", &dy);
            dy *= -1;
        } else if (!strcmp(input, "S") || !strcmp(input, "s")) {
            scanf("%d", &dy);
        } else if (!strcmp(input, "A") || !strcmp(input, "a")) {
            scanf("%d", &dx);
            dx *= -1;
        } else if (!strcmp(input, "D") || !strcmp(input, "d")) {
            scanf("%d", &dx);
        }
        
        //move the player each spot
        if (dx != 0) {
            
            //for each move of the player
            for (int i = 0; i < abs(dx); i++) {
                
                //check to see if the player can move there
                int nextColumn = playerColumn + (dx/abs(dx));
                if (!canStandOnSpot(map, playerRow, nextColumn)) {
                    break;
                } else {
                    playerColumn = nextColumn;
                }
                
                //check to see if the spot is actionable and perform the action
                int pokemonFound = checkAndPerformAction(map, playerRow, playerColumn, player);
                
                //break if breakable action
                if (isActionBreakable(map[playerRow][playerColumn]) || pokemonFound) {
                    //check to see if not long grass, if so then move back one
                    if (map[playerRow][playerColumn] != LONGGRASS) {
                        playerColumn -= (dx/abs(dx));
                    }
                    break;
                }
                
            }
            
        }
        if (dy != 0) {
            
            //for each move of the player
            for (int i = 0; i < abs(dy); i++) {
                
                //check to see if the player can move there
                int nextRow = playerRow + (dy/abs(dy));
                if (!canStandOnSpot(map, nextRow, playerColumn)) {
                    break;
                } else {
                    playerRow = nextRow;
                }
                
                //check to see if the spot is actionable and perform the action
                int pokemonFound = checkAndPerformAction(map, playerRow, playerColumn, player);
                
                //break if breakable action
                if (isActionBreakable(map[playerRow][playerColumn]) || pokemonFound) {
                    //check to see if not long grass, if so then move back one
                    if (map[playerRow][playerColumn] != LONGGRASS) {
                        playerRow -= (dy/abs(dy));
                    }
                    break;
                }
                
            }
            
        }
        
        
    }
    
    
    
    //end the game
    return 0;
}

//presents a list of pokemon the player has
void presentPokemon(Player* player) {
    
    //gets the names and numbers of all the player's Pokemon
    char pokemonNames[player->numberOfPokemon][MAXSTRINGLENGTH];
    int pokemonNumbers[player->numberOfPokemon];
    getPokemonNamesAndNumbers(player->pokemonFileName, player->numberOfPokemon, pokemonNames, pokemonNumbers);
    
    //print all the Pokemon and their numbers
    printf("Here is a list of all of your Pokemon.\n");
    for (int i = 0; i < player->numberOfPokemon; i++) {
        printf("%s %d\n", pokemonNames[i], pokemonNumbers[i]);
    }
    
    //ask if the player wants stat information
    printf("Would you like the stats information about any of your Pokemon?\n");
    char answer[MAXSTRINGLENGTH];
    scanf("%s", answer);
    
    //loop until the player does not want any more information
    while (!(!strcmp(answer, "No") || !strcmp(answer, "no"))) {
        
        //handle invalid input
        while (!(!strcmp(answer, "Yes") || !strcmp(answer, "yes") || !strcmp(answer, "No") || !strcmp(answer, "no"))) {
            printf("Please enter either yes or no. Would you like the stats information about any of your Pokemon?\n");
            scanf("%s", answer);
        }
        
        //the user entered no
        if (!strcmp(answer, "No") || !strcmp(answer, "no")) {
            break;
        }
        
        //the user entered yes
        
        //ask for which Pokemon the user wants information about
        char pokemonName[MAXSTRINGLENGTH];
        int pokemonNumber;
        printf("Which Pokemon would you like stats information about?\n");
        scanf("%s", pokemonName);
        scanf("%d", &pokemonNumber);
        
        //handle invalid input
        while (!elementsAreInArrays(player, pokemonName, pokemonNumber, pokemonNames, pokemonNumbers)) {
            printf("Please enter the name and number for the Pokemon you would like information about.\n");
            scanf("%s", pokemonName);
            scanf("%d", &pokemonNumber);
        }
        
        //create the Pokemon's stats
        Pokemon printPokemon;
        getPokemonInformation(&printPokemon, player, pokemonName, pokemonNumber);
        
        //print the Pokemon
        printf("Name: %s %d\n", printPokemon.name, printPokemon.fileNumber);
        printf("HP: %d\n", printPokemon.HP);
        printf("Base HP: %d\n", printPokemon.baseHP);
        printf("Base Attack: %d\n", printPokemon.baseAttack);
        printf("Base Defense: %d\n", printPokemon.baseDefense);
        printf("Base Special Attack: %d\n", printPokemon.baseSpecialAttack);
        printf("Base Special Defense: %d\n", printPokemon.baseSpecialDefense);
        printf("Base Speed: %d\n", printPokemon.baseSpeed);
        printf("IV HP: %d\n", printPokemon.ivHP);
        printf("IV Attack: %d\n", printPokemon.ivAttack);
        printf("IV Defense: %d\n", printPokemon.ivDefense);
        printf("IV Special Attack: %d\n", printPokemon.ivSpecialAttack);
        printf("IV Special Defense: %d\n", printPokemon.ivSpecialDefense);
        printf("IV Speed: %d\n", printPokemon.ivSpeed);
        printf("EV HP: %d\n", printPokemon.evHP);
        printf("EV Attack: %d\n", printPokemon.evAttack);
        printf("EV Defense: %d\n", printPokemon.evDefense);
        printf("EV Special Attack: %d\n", printPokemon.evSpecialAttack);
        printf("EV Special Defense: %d\n", printPokemon.evSpecialDefense);
        printf("EV Speed: %d\n", printPokemon.evSpeed);
        char natureAsString[MAXSTRINGLENGTH];
        convertNatureToString(printPokemon.nature, natureAsString);
        printf("Nature: %s\n", natureAsString);
        printf("XP: %d\n", printPokemon.XP);
        printf("Level: %d\n", printPokemon.level);
        if (printPokemon.type2 == noType) {
            char type[MAXSTRINGLENGTH];
            convertMoveTypeToString(printPokemon.type1, type);
            printf("Type: %s\n", type);
        } else {
            char type1[MAXSTRINGLENGTH];
            convertMoveTypeToString(printPokemon.type1, type1);
            printf("Type 1: %s\n", type1);
            char type2[MAXSTRINGLENGTH];
            convertMoveTypeToString(printPokemon.type2, type2);
            printf("Type 2: %s\n", type2);
        }
        char evGivenString[MAXSTRINGLENGTH];
        convertStatToString(printPokemon.evTypeGiven, evGivenString);
        printf("EV Given: %s %d\n", evGivenString, printPokemon.evAmountGiven);
        printf("\nMove Stats:\n");
        for (int i = 0; i < printPokemon.numberOfMoves; i++) {
            char moveNameString[MAXSTRINGLENGTH];
            convertMoveNameToString(printPokemon.moves[i].name, moveNameString);
            printf("Move Name: %s\n", moveNameString);
            printf("Move Power: %d\n", printPokemon.moves[i].damage);
            char moveTypeString[MAXSTRINGLENGTH];
            convertMoveTypeToString(printPokemon.moves[i].type, moveTypeString);
            printf("Move Type: %s\n", moveTypeString);
            char podString[MAXSTRINGLENGTH];
            convertPhysicalOrSpecialToString(printPokemon.moves[i].isPhysicalOrSpecial, podString);
            printf("Physical or Special: %s\n", podString);
            printf("Accuracy: %d\n\n", printPokemon.moves[i].accuracy);
        }
        
        //ask again
        printf("Would you the like the stats information about any more of your Pokemon?\n");
        scanf("%s", answer);
    }
    
}

//check to see if the element is in the array
int elementsAreInArrays(Player* player, char element[MAXSTRINGLENGTH], int number, char array[player->numberOfPokemon][MAXSTRINGLENGTH], int intArray[player->numberOfPokemon]) {
    for (int i = 0; i < player->numberOfPokemon; i++) {
        if (!strcmp(element, array[i]) && number == intArray[i]) {
            return 1;
        }
    }
    return 0;
}


//check for an action and perform the action
//returns whether or not a pokemon was found in long grass
int checkAndPerformAction(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int playerRow, int playerColumn, Player* player) {
    
    //performs the appropriate action based on objects in the map
    switch (map[playerRow][playerColumn]) {
        case LONGGRASS:
            return checkLongGrass(player);
        case SHOPDOOR:
            showShop(player);
            return 0;
        case GYMLEADER:
            startGymLeaderBattle(player);
            return 0;
        default:
            return 0;
    }
    
}

//checks to see if the player can move to the spot
int canStandOnSpot(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int playerRow, int playerColumn) {
    
    //make sure the indexes are valid
    if (playerRow < 0 || playerRow >= MAXLINESPERMAP) {
        return 0;
    }
    if (playerColumn < 0 || playerColumn >= strlen(map[playerRow])) {
        return 0;
    }
    
    //get the character of the position
    char position = map[playerRow][playerColumn];
    
    //return appropriate results for each possible position
    switch (position) {
        case BUSH:
            return 0;
        case PATH:
            return 1;
        case SHORTGRASS:
            return 1;
        case LONGGRASS:
            return 1;
        case WATER:
            return 0;
        case FENCE:
            return 0;
        case ROCK:
            return 0;
        case HOUSE:
            return 0;
        case MOUNTAIN:
            return 0;
        case SHOPDOOR:
            return 1;
        case PLAYER:
            return 0;
        case GYMLEADER:
            return 1;
        default:
            return 0;
    }
    
}

//check to see if the action is breakable (if the player can't move through the object, for example a boundary)
int isActionBreakable(char action) {
    switch (action) {
        case SHOPDOOR:
            return 1;
        case GYMLEADER:
            return 1;
    }
    return 0;
}


//prints the map key, map, and player
void printMapAndKeyAndPlayer(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int mapHeight, int playerRow, int playerColumn) {
    printMapKey();
    printf("\n");
    printMapWithPlayer(map, mapHeight, playerRow, playerColumn);
    printf("\n");
}

//prints the given map, with a player at the given coordinate
void printMapWithPlayer(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int mapHeight, int playerRow, int playerColumn) {
    
    
    //loop through map row and column
    for (int row = 0; row < mapHeight; row++) {
        
        for (int column = 0; column < strlen(map[row]); column++) {
            
            //print the player if it's the player's spot
            if (row == playerRow && column == playerColumn) {
                char playerEmoji[MAXEMOJILENGTH];
                convertCharacterToEmoji(PLAYER, playerEmoji);
                printf("%s", playerEmoji);
            }
            //else print from the map
            else {
                //convert the character to an emoji
                char emoji[MAXEMOJILENGTH];
                convertCharacterToEmoji(map[row][column], emoji);
                printf("%s", emoji);
            }
            
        }
        printf("\n");
    }
    
}

//converts the given character to an emoji and stores it in the passed in string emoji
void convertCharacterToEmoji(char character, char emoji[MAXEMOJILENGTH]) {
    
    //stores the character as a string
    char characterAsString[2];
    strcpy(characterAsString, "");
    sprintf(characterAsString, "%c", character);
    
    //copys the appropriate emoji to the emoji string
    switch (character) {
        case BUSH:
            strcpy(emoji, "🌳");
            break;
        case PATH:
            strcpy(emoji, "⬜️");
            break;
        case SHORTGRASS:
            strcpy(emoji, "🌱");
            break;
        case LONGGRASS:
            strcpy(emoji, "🌿");
            break;
        case WATER:
            strcpy(emoji, "🌊");
            break;
        case FENCE:
            strcpy(emoji, "🚪");
            break;
        case ROCK:
            strcpy(emoji, "🗿");
            break;
        case SHOPDOOR:
            strcpy(emoji, "🏣");
            break;
        case PLAYER:
            strcpy(emoji, "🙂");
            break;
        case GYMLEADER:
            strcpy(emoji, "😎");
            break;
        case HOUSE:
            strcpy(emoji, "🏠");
            break;
        case MOUNTAIN:
            strcpy(emoji, "⛰");
            break;
        //defualt just print the character
        default:
            strcpy(emoji, characterAsString);
            break;
    }
    
}

//prints out the key to the emoji map
void printMapKey() {
    printf("\nThis is the key to the map.\nBushes: 🌳\nPathways: ⬜️\nShort Grass: 🌱\nLong Grass: 🌿\nWater: 🌊\nFrence: 🚪\nRock: 🗿\nHouse: 🏠\nShop Door: 🏣\nBattle Door: 🏢\nPlayer: 🙂\nGym Leader: 😎\nMountain: ⛰\n");
}

//finds the spot that a player can be placed on that is closest to the center of the map
void findAvailableSpot(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int mapHeight, int availableSpot[2]) {
    
    //start at the center
    int checkRow = mapHeight / 2;
    int checkColumn = (int) (strlen(map[checkRow]) - 1) / 2;
    
    //check spots in this row
    for (int i = 0; i < (int) strlen(map[checkRow]) / 2; i++) {
        
        //move right
        int rightCheckColumn = checkColumn + i;
        if (isPositionInMap(map, mapHeight, checkRow, rightCheckColumn)) {
            if (characterIsAvailableToPlacePlayerOn(map[checkRow][rightCheckColumn])) {
                checkColumn = rightCheckColumn;
                availableSpot[0] = checkRow;
                availableSpot[1] = checkColumn;
                return;
            }
        }
        
        //move left
        int leftCheckColumn = checkColumn - i;
        if (isPositionInMap(map, mapHeight, checkRow, leftCheckColumn)) {
            if (characterIsAvailableToPlacePlayerOn(map[checkRow][leftCheckColumn])) {
                checkColumn = leftCheckColumn;
                availableSpot[0] = checkRow;
                availableSpot[1] = checkColumn;
                return;
            }
        }
        
    }
    
    
    //try the above and below rows
    for (int i = 0; i < mapHeight / 2; i++) {
        
        //move up
        int upCheckRow = checkRow + i;
        int upCheckColumn = (int) (strlen(map[upCheckRow]) - 1) / 2;
        for (int j = 0; j < (int) strlen(map[upCheckRow]) / 2; j++) {
            
            //move right
            int rightCheckColumn = upCheckColumn + j;
            if (isPositionInMap(map, mapHeight, upCheckRow, rightCheckColumn)) {
                if (characterIsAvailableToPlacePlayerOn(map[upCheckRow][rightCheckColumn])) {
                    availableSpot[0] = upCheckRow;
                    availableSpot[1] = rightCheckColumn;
                    return;
                }
            }
            
            //move left
            int leftCheckColumn = upCheckColumn - j;
            if (isPositionInMap(map, mapHeight, upCheckRow, leftCheckColumn)) {
                if (characterIsAvailableToPlacePlayerOn(map[upCheckRow][leftCheckColumn])) {
                    availableSpot[0] = upCheckRow;
                    availableSpot[1] = leftCheckColumn;
                    return;
                }
            }
            
        }
        
        
        //move down
        int downCheckRow = checkRow - i;
        int downCheckColumn = (int) (strlen(map[downCheckRow]) - 1) / 2;
        for (int j = 0; j < (int) strlen(map[downCheckRow]) / 2; j++) {
            
            //move right
            int rightCheckColumn = downCheckColumn + j;
            if (isPositionInMap(map, mapHeight, downCheckRow, rightCheckColumn)) {
                if (characterIsAvailableToPlacePlayerOn(map[downCheckRow][rightCheckColumn])) {
                    availableSpot[0] = downCheckRow;
                    availableSpot[1] = rightCheckColumn;
                    return;
                }
            }
            
            //move left
            int leftCheckColumn = downCheckColumn - j;
            if (isPositionInMap(map, mapHeight, downCheckRow, leftCheckColumn)) {
                if (characterIsAvailableToPlacePlayerOn(map[downCheckRow][leftCheckColumn])) {
                    availableSpot[0] = downCheckRow;
                    availableSpot[1] = leftCheckColumn;
                    return;
                }
            }
            
        }
        
        
        //no spot on the map is available for the player
        printf("Error: This map does not appear to have a single spot for the player to stand. Pokemon will not quit. Thanks for playing!\n");
        availableSpot[0] = -1;
        availableSpot[1] = -1;
        
    }
    
    
}

//checks if the given row and column values are valid values in the map
int isPositionInMap(char map[MAXLINESPERMAP][MAXLINESPERMAP + 1], int mapHeight, int row, int column) {
    
    //if the position is valid
    if (row >= 0 && row < mapHeight) {
        if (column >= 0 && column < (int) (strlen(map[row]))) {
            return 1;
        }
    }
    //the position is not valid
    return 0;
    
}


//returns 0 if a player cannot be placed on top of the character, returns 1 if the player can be placed on top of the character
int characterIsAvailableToPlacePlayerOn(char character) {
    switch (character) {
        case BUSH:
            return 0;
        case PATH:
            return 1;
        case SHORTGRASS:
            return 1;
        case LONGGRASS:
            return 1;
        case WATER:
            return 0;
        case FENCE:
            return 0;
        case ROCK:
            return 0;
        case SHOPDOOR:
            return 0;
        case PLAYER:
            return 0;
        case GYMLEADER:
            return 0;
        case HOUSE:
            return 0;
        case MOUNTAIN:
            return 0;
        default:
            return 0;
    }
}

//unlocks the next map for player
int unlockedNewMaps(Player* player) {
    
    //unlocked a new map after every three caught pokemon
    
    //get all maps
    char allMaps[MAXNUMBEROFLEVELS][MAXSTRINGLENGTH];
    FILE* allMapsFile = fopen(ALLMAPSFILE, "r+");
    getAllMaps(allMapsFile, allMaps, MAXNUMBEROFLEVELS, MAXSTRINGLENGTH);
    
    //get unlocked maps
    char unlockedMaps[MAXNUMBEROFLEVELS][MAXSTRINGLENGTH];
    FILE* playerUnlockedMapsFile = fopen(player->unlockedMapsFileName, "r+");
    getAllMaps(playerUnlockedMapsFile, unlockedMaps, MAXNUMBEROFLEVELS, MAXSTRINGLENGTH);
    
    //get the index of the next unlockable map
    int indexOfMapToUnlocked = (player->numberOfPokemon / 3);
    
    //if the index is out of bounds
    if (indexOfMapToUnlocked >= MAXNUMBEROFLEVELS) {
        return 0;
    }
    
    //check if the map is already unlocked
    if (!strcmp(allMaps[indexOfMapToUnlocked], unlockedMaps[indexOfMapToUnlocked])) {
        return 0;
    }
    
    //the map needs to be unlocked
    writeToFile(playerUnlockedMapsFile, player->unlockedMapsFileName, allMaps[indexOfMapToUnlocked]);
    printf("Congratulations! You have unlocked the next map: %s!\n", allMaps[indexOfMapToUnlocked]);
    return 1;
    
}

//MARK: - Map Actions

//returns whether or not a pokemon was found
//checks the long grass for wild pokemon and will commence a battle if one is found
int checkLongGrass(Player* player) {
    
    //determine if there is a pokemon
    int chanceOfFindingWildPokemon = (3.0/18.0) * 100.0;
    int randomNumberToFindWildPokemon = getRandomNumber(1, 100);
    
    //returns 0 if no pokemon is found
    if (randomNumberToFindWildPokemon > chanceOfFindingWildPokemon) {
        return 0;
    }
    
    //generates the wild pokemon
    int randomNumberToGeneratePokemon = getRandomNumber(0, none - 1);
    
    //create the wild pokemon
    Pokemon wildPokemon;
    createPokemon(&wildPokemon, randomNumberToGeneratePokemon);
    
    //print message
    printf("A wild level %d %s has appeared!\n", wildPokemon.level, wildPokemon.name);
    
    
    //ask if the user wants to flee
    printf("Would you like to battle the wild %s??\n", wildPokemon.name);
    
    //create list of Pokemon
    char pokemonNames[player->numberOfPokemon][MAXSTRINGLENGTH];
    int pokemonNumbers[player->numberOfPokemon];
    getPokemonNamesAndNumbers(player->pokemonFileName, player->numberOfPokemon, pokemonNames, pokemonNumbers);
    
    //get response
    char response[MAXSTRINGLENGTH];
    scanf("%s", response);
    
    int numberOfAvailablePokemonLeft = player->numberOfPokemon;
    
    //loop until the player says no or they have no Pokemo left to battle with
    while (!(!strcmp(response, "No")|| !strcmp(response, "no")) && numberOfAvailablePokemonLeft > 0) {
        
        //handle incorrect input
        while (!(!strcmp(response, "Yes") || !strcmp(response, "yes") || !strcmp(response, "No") || !strcmp(response, "no"))) {
            printf("Please enter yes or no. Would you like to battle the wild %s??\n", wildPokemon.name);
            scanf("%s", response);
        }
        
        //if no, then leave the battle
        if (!strcmp(response, "No") || !strcmp(response, "no")) {
            return 1;
        }
        
        //choose Pokemon
        printf("Enter the number of the Pokemon you would like to use in battle.\n\n");
        for (int i = 0; i < numberOfAvailablePokemonLeft; i++) {
            printf("%d. %s %d\n", i + 1, pokemonNames[i], pokemonNumbers[i]);
        }
        int pokemonIndex;
        scanf("%d", &pokemonIndex);
        
        //handle incorrect user input
        while (pokemonIndex < 1 || pokemonIndex >= numberOfAvailablePokemonLeft + 1) {
            printf("That is not a valid number. Please enter the number of the Pokemon you would like to use in battle.\n\n");
            for (int i = 0; i < numberOfAvailablePokemonLeft; i++) {
                printf("%d. %s %d\n", i + 1, pokemonNames[i], pokemonNumbers[i]);
            }
            scanf("%d", &pokemonIndex);
        }
        pokemonIndex--;
        
        
        //create player Pokemon
        Pokemon playerPokemon;
        char playerPokemonFileName[MAXSTRINGLENGTH];
        strcpy(playerPokemonFileName, player->name);
        strcat(playerPokemonFileName, " ");
        strcat(playerPokemonFileName, pokemonNames[pokemonIndex]);
        strcat(playerPokemonFileName, " ");
        
        char pokemonNumberString[MAXSTRINGLENGTH];
        strcpy(pokemonNumberString, "");
        sprintf(pokemonNumberString, "%d", pokemonNumbers[pokemonIndex]);
        
        strcat(playerPokemonFileName, pokemonNumberString);
        strcat(playerPokemonFileName, ".txt");
        
        //open the pokemonFile
        FILE* playerPokemonFile = fopen(playerPokemonFileName, "r+");
        if (playerPokemonFile == NULL) {
            printf("Error: The file with filename %s does not exist.\n", playerPokemonFileName);
            return 0;
        }
        
        //read the information about the pokemon
        readPokemonInformation(playerPokemonFile, &playerPokemon, pokemonNames[pokemonIndex], pokemonNumbers[pokemonIndex]);
        
        //if the pokemon has no HP
        while (playerPokemon.HP <= 0) {
            printf("This pokemon has no HP, please select a different Pokemon.\n");
            //choose Pokemon
            printf("Enter the number of the Pokemon you would like to use in battle, or enter -1 to quit.\n\n");
            for (int i = 0; i < numberOfAvailablePokemonLeft; i++) {
                printf("%d. %s %d\n", i + 1, pokemonNames[i], pokemonNumbers[i]);
            }
            int pokemonIndex;
            scanf("%d", &pokemonIndex);
            if (pokemonIndex == -1) {
                return 1;
            }
            
            //handle incorrect user input
            while (pokemonIndex < 1 || pokemonIndex >= numberOfAvailablePokemonLeft + 1) {
                
                printf("That is not a valid number. Please enter the number of the Pokemon you would like to use in battle.\n\n");
                for (int i = 0; i < numberOfAvailablePokemonLeft; i++) {
                    printf("%d. %s %d\n", i + 1, pokemonNames[i], pokemonNumbers[i]);
                }
                scanf("%d", &pokemonIndex);
            }
            pokemonIndex--;
            
            
            //create player Pokemon
            char playerPokemonFileName[MAXSTRINGLENGTH];
            strcpy(playerPokemonFileName, player->name);
            strcat(playerPokemonFileName, " ");
            strcat(playerPokemonFileName, pokemonNames[pokemonIndex]);
            strcat(playerPokemonFileName, " ");
            
            char pokemonNumberString[MAXSTRINGLENGTH];
            strcpy(pokemonNumberString, "");
            sprintf(pokemonNumberString, "%d", pokemonNumbers[pokemonIndex]);
            
            strcat(playerPokemonFileName, pokemonNumberString);
            strcat(playerPokemonFileName, ".txt");
            
            //get pokemon file
            FILE* playerPokemonFile = fopen(playerPokemonFileName, "r+");
            if (playerPokemonFile == NULL) {
                printf("Error: The file with filename %s does not exist.\n", playerPokemonFileName);
                return 0;
            }
            
            //read the pokemon's information
            readPokemonInformation(playerPokemonFile, &playerPokemon, pokemonNames[pokemonIndex], pokemonNumbers[pokemonIndex]);

        }
        
        //battle
        int battleResult = startBattle(&playerPokemon, &wildPokemon, wild, player->name, "The wild", player);
        
        //save the result of the battle to files
        writePokemonInformation(playerPokemonFile, playerPokemonFileName, &playerPokemon);
        
        //remove already used Pokemon from list if lost, break if won
        if (!battleResult) {
            //move final value to Pokemon index
            char tempString[MAXSTRINGLENGTH];
            strcpy(tempString, pokemonNames[numberOfAvailablePokemonLeft - 1]);
            strcpy(pokemonNames[pokemonIndex], tempString);
            pokemonNumbers[pokemonIndex] = pokemonNumbers[numberOfAvailablePokemonLeft - 1];
            //decrease number of available Pokemon left
            numberOfAvailablePokemonLeft--;
        } else {
            break;
        }
        
        
        //ask if they want to continue
        if (numberOfAvailablePokemonLeft > 0) {
            printf("Would you like to use another Pokemon to battle the wild %s?\n", wildPokemon.name);
            scanf("%s", response);
        }
        
    }
    
    return 1;
}

//starts a gym leader battle
void startGymLeaderBattle(Player* player) {
    
    //greet the player
    printf("Hi, my name is Gym Leader Gary! Would you like to battle me?\n");
    char willBattle[MAXSTRINGLENGTH];
    scanf("%s", willBattle);
    
    //filter out bad input
    while (!(!strcmp(willBattle, "Yes") || !strcmp(willBattle, "yes") || !strcmp(willBattle, "No") || !strcmp(willBattle, "no"))) {
        printf("Please enter yes or no. Would you like Gym Leader Gary?\n");
        scanf("%s", willBattle);
    }
    
    //return if the answer is no
    if (!strcmp(willBattle, "No") || !strcmp(willBattle, "no")) {
        return;
    }
    
    //they want to battle
    
    //get pokemon names and numbers
    char pokemonNames[player->numberOfPokemon][MAXSTRINGLENGTH];
    int pokemonNumbers[player->numberOfPokemon];
    getPokemonNamesAndNumbers(player->pokemonFileName, player->numberOfPokemon, pokemonNames, pokemonNumbers);
    int numberOfAvailablePokemonLeft = player->numberOfPokemon;
    
    
    //create gym leader pokemon array
    Pokemon charizardPokemon;
    createPokemon(&charizardPokemon, charizard);
    Pokemon blastoisePokemon;
    createPokemon(&blastoisePokemon, blastoise);
    Pokemon venusaurPoksemon;
    createPokemon(&venusaurPoksemon, venusaur);
    Pokemon gymLeaderPokemon[3];
    gymLeaderPokemon[0] = charizardPokemon;
    gymLeaderPokemon[1] = blastoisePokemon;
    gymLeaderPokemon[2] = venusaurPoksemon;
    int currentBattle = 0;
    
    //loop until the player says no
    while (!(!strcmp(willBattle, "No")|| !strcmp(willBattle, "no")) && numberOfAvailablePokemonLeft > 0 && currentBattle < 3) {
        
        //choose Pokemon
        printf("Enter the number of the Pokemon you would like to use in battle.\n\n");
        for (int i = 0; i < numberOfAvailablePokemonLeft; i++) {
            printf("%d. %s %d\n", i + 1, pokemonNames[i], pokemonNumbers[i]);
        }
        int pokemonIndex;
        scanf("%d", &pokemonIndex);
        
        //handle incorrect user input
        while (pokemonIndex < 1 || pokemonIndex >= numberOfAvailablePokemonLeft + 1) {
            printf("That is not a valid number. Please enter the number of the Pokemon you would like to use in battle.\n\n");
            for (int i = 0; i < numberOfAvailablePokemonLeft; i++) {
                printf("%d. %s %d\n", i + 1, pokemonNames[i], pokemonNumbers[i]);
            }
            scanf("%d", &pokemonIndex);
        }
        pokemonIndex--;
        
        //create player Pokemon
        Pokemon playerPokemon;
        char playerPokemonFileName[MAXSTRINGLENGTH];
        strcpy(playerPokemonFileName, player->name);
        strcat(playerPokemonFileName, " ");
        strcat(playerPokemonFileName, pokemonNames[pokemonIndex]);
        strcat(playerPokemonFileName, " ");
        
        char pokemonNumberString[MAXSTRINGLENGTH];
        strcpy(pokemonNumberString, "");
        sprintf(pokemonNumberString, "%d", pokemonNumbers[pokemonIndex]);
        
        strcat(playerPokemonFileName, pokemonNumberString);
        strcat(playerPokemonFileName, ".txt");
        
        //get the pokemon file
        FILE* playerPokemonFile = fopen(playerPokemonFileName, "r+");
        if (playerPokemonFile == NULL) {
            printf("Error: The file with filename %s does not exist.\n", playerPokemonFileName);
            return;
        }
        
        //read the pokemon's information
        readPokemonInformation(playerPokemonFile, &playerPokemon, pokemonNames[pokemonIndex], pokemonNumbers[pokemonIndex]);
        
        //if the pokemon has no HP
        while (playerPokemon.HP <= 0) {
            printf("This pokemon has no HP, please select a different Pokemon.\n");
            //choose Pokemon
            printf("Enter the number of the Pokemon you would like to use in battle, or enter -1 to quit.\n\n");
            for (int i = 0; i < numberOfAvailablePokemonLeft; i++) {
                printf("%d. %s %d\n", i + 1, pokemonNames[i], pokemonNumbers[i]);
            }
            int pokemonIndex;
            scanf("%d", &pokemonIndex);
            if (pokemonIndex == -1) {
                return;
            }
            
            //handle incorrect user input
            while (pokemonIndex < 1 || pokemonIndex >= numberOfAvailablePokemonLeft + 1) {
                
                printf("That is not a valid number. Please enter the number of the Pokemon you would like to use in battle.\n\n");
                for (int i = 0; i < numberOfAvailablePokemonLeft; i++) {
                    printf("%d. %s %d\n", i + 1, pokemonNames[i], pokemonNumbers[i]);
                }
                scanf("%d", &pokemonIndex);
            }
            pokemonIndex--;
            
            
            //create player Pokemon
            char playerPokemonFileName[MAXSTRINGLENGTH];
            strcpy(playerPokemonFileName, player->name);
            strcat(playerPokemonFileName, " ");
            strcat(playerPokemonFileName, pokemonNames[pokemonIndex]);
            strcat(playerPokemonFileName, " ");
            
            char pokemonNumberString[MAXSTRINGLENGTH];
            strcpy(pokemonNumberString, "");
            sprintf(pokemonNumberString, "%d", pokemonNumbers[pokemonIndex]);
            
            strcat(playerPokemonFileName, pokemonNumberString);
            strcat(playerPokemonFileName, ".txt");
            
            //open the pokemon file
            FILE* playerPokemonFile = fopen(playerPokemonFileName, "r+");
            if (playerPokemonFile == NULL) {
                printf("Error: The file with filename %s does not exist.\n", playerPokemonFileName);
                return;
            }
            
            //read the pokemon's information
            readPokemonInformation(playerPokemonFile, &playerPokemon, pokemonNames[pokemonIndex], pokemonNumbers[pokemonIndex]);
            
        }
        
        //battle
        int battleResult = startBattle(&playerPokemon, &gymLeaderPokemon[currentBattle], gymLeader, player->name, "Gym Leader Gary", player);
        if (battleResult) {
            currentBattle++;
            continue;
        }
        
        //save the result of the battle
        writePokemonInformation(playerPokemonFile, playerPokemonFileName, &playerPokemon);
        
        //remove already used Pokemon from list if lost, break if won
        if (!battleResult) {
            //move final value to Pokemon index
            char tempString[MAXSTRINGLENGTH];
            strcpy(tempString, pokemonNames[numberOfAvailablePokemonLeft - 1]);
            strcpy(pokemonNames[pokemonIndex], tempString);
            pokemonNumbers[pokemonIndex] = pokemonNumbers[numberOfAvailablePokemonLeft - 1];
            //decrease number of available Pokemon left
            numberOfAvailablePokemonLeft--;
        }
        
        //ask if they want to continue
        if (numberOfAvailablePokemonLeft > 0) {
            printf("Would you like to use another Pokemon to battle Gym Leader Gary?\n");
            scanf("%s", willBattle);
        }
    }
    
    //if they defeated all of the gym leader's pokemon, they have won
    if (currentBattle >= 4) {
        printf("Congratulations! You have defeated Gym Leader Gary!\n");
    }
    //otherwise they ran away or lost
    else {
        printf("You were defeated by Gym Leader Gary. You should go heal your Pokemon.\n");
    }
    
    
    
}

//shows the PokeStop
void showShop(Player* player) {
    
    //greets the player
    printf("Welcome to the PokeStop!\nWould you like to heal all your Pokemon?\n");
    char response[MAXSTRINGLENGTH];
    scanf("%s", response);
    
    //handle incorrect input
    while (!(!strcmp(response, "Yes") || !strcmp(response, "yes") || !strcmp(response, "No") || !strcmp(response, "no"))) {
        printf("Please enter yes or no. Would you like to heal all your Pokemon?\n");
        scanf("%s", response);
    }
    
    //if yes then heal all pokemon
    if (!strcmp(response, "Yes") || !strcmp(response, "yes")) {
        
        //get list of all Pokemon names and numbers
        char pokemonNames[player->numberOfPokemon][MAXSTRINGLENGTH];
        int pokemonNumbers[player->numberOfPokemon];
        getPokemonNamesAndNumbers(player->pokemonFileName, player->numberOfPokemon, pokemonNames, pokemonNumbers);
        
        //loop through all of them
        for (int i = 0; i < player->numberOfPokemon; i++) {
            
            //create a new pokemon with the appropriate information and max hp
            Pokemon pokemon;
            getPokemonInformation(&pokemon, player, pokemonNames[i], pokemonNumbers[i]);
            pokemon.HP = getMaxHP(pokemon);
            
            //access the Pokemon file
            char pokemonFileName[MAXSTRINGLENGTH];
            strcpy(pokemonFileName, player->name);
            strcat(pokemonFileName, " ");
            strcat(pokemonFileName, pokemon.name);
            strcat(pokemonFileName, " ");
            
            char pokemonFileNumberString[MAXSTRINGLENGTH];
            strcpy(pokemonFileNumberString, "");
            sprintf(pokemonFileNumberString, "%d", pokemon.fileNumber);
            strcat(pokemonFileName, pokemonFileNumberString);
            strcat(pokemonFileName, ".txt");
            
            //open the file
            FILE* pokemonFile = fopen(pokemonFileName, "r+");
            
            //save the new information to the file
            writePokemonInformation(pokemonFile, pokemonFileName, &pokemon);
        }
        
    }
    
}


//MARK: - Battling

//runs one battle between two pokemon, changes based off the mode
//returns 1 if player 1 wins the battle or the battle ends, returns 0 otherwise
int startBattle(Pokemon* pokemon1, Pokemon* pokemon2, enum battleMode battleMode, const char player1Name[MAXSTRINGLENGTH], const char player2Name[MAXSTRINGLENGTH], Player* player1) {
    
    //create title for each user battling
    char player1Title[MAXSTRINGLENGTH];
    char player2Title[MAXSTRINGLENGTH];
    strcpy(player1Title, player1Name);
    strcpy(player2Title, player2Name);
    
    //add appropriate endings to the ends of title
    switch (battleMode) {
        case wild:
            strcat(player1Title, "'s");
            break;
        case gymLeader:
            strcat(player1Title, "'s");
            strcat(player2Title, "'s");
            break;
    }
    
    //determine who goes first
    int pokemon1GoesFirst = (getSpeed(*pokemon1) > getSpeed(*pokemon2));
    int firstMoveHasOccured = 0;
    
    //check for lose
    if (pokemon2->HP <= 0) {
        printf("%s %s has fainted!\n", player2Title, pokemon2->name);
    }
    
    //loop until a player has negative HP
    while (pokemon1->HP > 0 && pokemon2->HP > 0) {
        
        //if the first move hasn't happened yet and player 1 goes first
        if ((!firstMoveHasOccured && pokemon1GoesFirst) || firstMoveHasOccured) {
            
            //print name player's pokemon, level, type and hp, and opponenets pokemon, level, type, and hp
            printf("\n%s %s\n", player1Title, pokemon1->name);
            printf("HP: %d\n", pokemon1->HP);
            printf("Level: %d\n", pokemon1->level);
            char pokemon1TypeString[MAXSTRINGLENGTH];
            convertMoveTypeToString(pokemon1->type1, pokemon1TypeString);
            printf("Type: %s\n", pokemon1TypeString);
            if (pokemon1->type2 != noType) {
                char pokemon1Type2String[MAXSTRINGLENGTH];
                convertMoveTypeToString(pokemon1->type2, pokemon1Type2String);
                printf("Type: %s\n\n", pokemon1Type2String);
            } else {
                printf("\n");
            }
            
            //print the same information for player 2
            printf("%s %s\n", player2Title, pokemon2->name);
            printf("HP: %d\n", pokemon2->HP);
            printf("Level: %d\n", pokemon2->level);
            char pokemon2TypeString[MAXSTRINGLENGTH];
            convertMoveTypeToString(pokemon2->type1, pokemon2TypeString);
            printf("Type: %s\n", pokemon2TypeString);
            if (pokemon2->type2 != noType) {
                char pokemon2Type2String[MAXSTRINGLENGTH];
                convertMoveTypeToString(pokemon2->type2, pokemon2Type2String);
                printf("Type: %s\n\n", pokemon2Type2String);
            } else {
                printf("\n");
            }
            
            //store the option chosen in battle
            int optionChosen;
            
            //if wild, the user can catch
            if (battleMode == wild) {
                
                //present attack, flee, and catch
                printf("Available Options:\n1. Attack\n2. Catch\n3. Flee\nWhich option would you like to chose?\n");
                scanf("%d", &optionChosen);
                
                //filter out bad inputs
                while (optionChosen < 1 || optionChosen > 3) {
                    printf("Please enter a valid number. The available options are:\n1. Attack\n2. Catch\n3. Flee\nWhich option would you like to chose?\n");
                    scanf("%d", &optionChosen);
                }
                
            }
            //otherwise the user cannot catch
            else {
                
                //present attack and flee
                printf("Available Options:\n1. Attack\n2. Flee\nWhich option would you like to chose?\n");
                scanf("%d", &optionChosen);
                
                //filter out bad inputs
                while (optionChosen < 1 || optionChosen > 2) {
                    printf("Please enter a valid number. The available options are:\n1. Attack\n2. Catch\n3. Flee\nWhich option would you like to chose?\n");
                    scanf("%d", &optionChosen);
                }
                
                //switch to flee
                if (optionChosen == 2) {
                    optionChosen = 3;
                }
                
            }
            
            //the user ran away
            if (optionChosen == 3) {
                return 1;
            }
            //the user wants to catch
            else if (optionChosen == 2) {
                
                //generate catchNumber and catchValue
                int randomCatchNumber = getRandomNumber(0, 255);
                int catchValue = ((3*getMaxHP(*pokemon2) - 2*pokemon2->HP) * pokemon2->catchRate * 1.0)/(3*getMaxHP(*pokemon2));
                
                //the catch did not work
                if (randomCatchNumber > catchValue) {
                    printf("Sorry, you didn't catch %s %s.\n", player2Title, pokemon2->name);
                }
                //the catch did work
                else {
                    //add the pokemon to the player
                    printf("Congratuglations, you caught %s %s!!\n", player2Title, pokemon2->name);
                    addPokemonToPlayer(player1, pokemon2);
                    
                    //check to see if the player has unlocked any new maps
                    unlockedNewMaps(player1);
                    
                    break;
                }
                
                
                
            }
            //the user wants to attack
            else if (optionChosen == 1) {
                
                //attack
                printf("Here are the moves you can choose from:\n");
                
                //print all the moves
                for (int i = 0; i < pokemon1->numberOfMoves; i++) {
                    
                    //convert move names and type to strings
                    char moveTypeString[MAXSTRINGLENGTH];
                    convertMoveTypeToString(pokemon1->moves[i].type, moveTypeString);
                    
                    char moveNameString[MAXSTRINGLENGTH];
                    convertMoveNameToString(pokemon1->moves[i].name, moveNameString);
                    
                    printf("%d.%s: %s Type\n", i + 1, moveNameString, moveTypeString);
                }
                //ask which move they would like to use and store it. decrease the index because arrays are 0 based indexed
                printf("Which move would you like to use?\n");
                int moveIndex;
                scanf("%d", &moveIndex);
                moveIndex--;
                
                //filter out bad inputs
                while (moveIndex < 0 || moveIndex >= pokemon1->numberOfMoves) {
                    printf("Please enter a valid move number.\n");
                    scanf("%d", &moveIndex);
                    moveIndex--;
                }
                
                //apply the damage and print effectiveness message
                enum effectiveness effectiveness = damage(moveIndex, pokemon1, pokemon2);
                switch (effectiveness) {
                    case notEffective:
                        printf("The move was not effective.\n");
                        break;
                    case notVeryEffective:
                        printf("The move was not very effective.\n");
                        break;
                    case normalEffectiveness:
                        printf("The move was effective.\n");
                        break;
                    case superEffective:
                        printf("The move was super effective!\n");
                        break;
                }
                
            }
            
        }
        
        //print name player's pokemon, level, type, and hp, and opponenets pokemon, level, type, and hp
        printf("\n%s %s\n", player1Title, pokemon1->name);
        printf("HP: %d\n", pokemon1->HP);
        printf("Level: %d\n", pokemon1->level);
        char pokemon1TypeString[MAXSTRINGLENGTH];
        convertMoveTypeToString(pokemon1->type1, pokemon1TypeString);
        printf("Type: %s\n", pokemon1TypeString);
        if (pokemon1->type2 != noType) {
            char pokemon1Type2String[MAXSTRINGLENGTH];
            convertMoveTypeToString(pokemon1->type2, pokemon1Type2String);
            printf("Type: %s\n\n", pokemon1Type2String);
        } else {
            printf("\n");
        }
        
        //print the same information for player 2
        printf("%s %s\n", player2Title, pokemon2->name);
        printf("HP: %d\n", pokemon2->HP);
        printf("Level: %d\n", pokemon2->level);
        char pokemon2TypeString[MAXSTRINGLENGTH];
        convertMoveTypeToString(pokemon2->type1, pokemon2TypeString);
        printf("Type: %s\n", pokemon2TypeString);
        if (pokemon2->type2 != noType) {
            char pokemon2Type2String[MAXSTRINGLENGTH];
            convertMoveTypeToString(pokemon2->type2, pokemon2Type2String);
            printf("Type: %s\n\n", pokemon2Type2String);
        } else {
            printf("\n");
        }
        
        //check for lose
        if (pokemon2->HP <= 0) {
            printf("%s %s has fainted!\n", player2Title, pokemon2->name);
            break;
        }
        
        
        //the enemy can only flee if it is wild
        if (battleMode == wild) {
            //enemy goes
            int willEnemyFlee = getRandomNumber(0, 100);
            if (willEnemyFlee > 97) {
                printf("%s has run away!\n", pokemon2->name);
                return 1;
            }
        }
        
        //wild pokemon and gymleaders choose random moves
        if (battleMode == wild || battleMode == gymLeader) {
            
            //choose random move
            int moveIndex = getRandomNumber(0, pokemon2->numberOfMoves-1);
            
            //apply the damage and store effectiveness
            enum effectiveness effectiveness = damage(moveIndex, pokemon2, pokemon1);
            
            //convert move name to string
            char moveName[MAXSTRINGLENGTH];
            convertMoveNameToString(pokemon2->moves[moveIndex].name, moveName);
            
            //print what move was used
            printf("%s %s used %s against %s %s.\n", player2Title, pokemon2->name, moveName, player1Title, pokemon1->name);
            
            //print effectiveness message
            switch (effectiveness) {
                case notEffective:
                    printf("The move was not effective.\n");
                    break;
                case notVeryEffective:
                    printf("The move was not very effective.\n");
                    break;
                case normalEffectiveness:
                    printf("The move was effective.\n");
                    break;
                case superEffective:
                    printf("The move was super effective!\n");
                    break;
            }
            
        }
        
        
        //check for loses
        if (pokemon1->HP <= 0) {
            pokemon1->HP = 0;
            printf("%s %s was defeated by %s %s.\n", player1Title, pokemon1->name, player2Title, pokemon2->name);
            return 0;
        }
        
        //the first move has now occured
        firstMoveHasOccured = 1;
    }
    
    
    //player 1 has won the battle if the function hasn't returned
    
    //give ev from pokemon2 to pokemon1
    switch (pokemon2->evTypeGiven) {
        case attack:
            pokemon1->hiddenEvAttack += pokemon2->evAmountGiven;
            break;
        case defense:
            pokemon1->hiddenEvDefense += pokemon2->evAmountGiven;
            break;
        case specialAttack:
            pokemon1->hiddenEvSpecialAttack += pokemon2->evAmountGiven;
            break;
        case specialDefense:
            pokemon1->hiddenEvSpecialDefense += pokemon2->evAmountGiven;
            break;
        case speed:
            pokemon1->hiddenEvSpeed += pokemon2->evAmountGiven;
            break;
    }
    
    //increase XP based on difference in levels
    int differenceInLevels = abs(pokemon1->level - pokemon2->level);
    if (differenceInLevels == 0) {
        differenceInLevels = 1;
    }
    pokemon1->XP += 4 * differenceInLevels;
    
    
    //check for level up
    if (pokemon1->XP >= 20) {
        levelUp(pokemon1);
    }
    
    
    return 1;
}


//returns the max HP value for the Pokemon
int getMaxHP(Pokemon pokemon) {
    return (((2 * pokemon.baseHP) + pokemon.ivAttack + ((pokemon.evAttack / 4) * pokemon.level)) / 100) + pokemon.level + 10;
}

//returns the attack stat for the Pokemon
double getAttack(Pokemon pokemon) {
    return (((2 * pokemon.baseAttack + pokemon.ivAttack + (pokemon.evAttack / 4) * pokemon.level) / 100) + 5) * getNatureMultiplyer(attack, pokemon.nature);
}

//returns the defense stat for the Pokemon
double getDefense(Pokemon pokemon) {
    return (((2 * pokemon.baseDefense + pokemon.ivDefense + (pokemon.evDefense / 4) * pokemon.level) / 100) + 5) * getNatureMultiplyer(defense, pokemon.nature);
}

//returns the special attack stat for the Pokemon
double getSpecialAttack(Pokemon pokemon) {
    return (((2 * pokemon.baseSpecialAttack + pokemon.ivSpecialAttack + (pokemon.evSpecialAttack / 4) * pokemon.level) / 100) + 5) * getNatureMultiplyer(specialAttack, pokemon.nature);
}

//returns the special defense stat for the Pokemon
double getSpecialDefense(Pokemon pokemon) {
    return (((2 * pokemon.baseSpecialDefense + pokemon.ivSpecialDefense + (pokemon.evSpecialDefense / 4) * pokemon.level) / 100) + 5) * getNatureMultiplyer(specialDefense, pokemon.nature);
}

//returns the speed stat for the Pokemon
double getSpeed(Pokemon pokemon) {
    return (((2 * pokemon.baseSpeed + pokemon.ivSpeed + (pokemon.evSpeed / 4) * pokemon.level) / 100) + 5) * getNatureMultiplyer(speed, pokemon.nature);
}

//returns the multiplier for the effectiveness given the enum value
double getValueOfEffectiveness(enum effectiveness effectiveness) {
    
    switch (effectiveness) {
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
    }
    
}

//levels up the Pokemon that is passed in
//returns 1 if successful and 0 if unsuccessful
int levelUp(Pokemon *pokemon) {
    //increase the level
    pokemon->level++;
    
    //make hidden ev's visible ev's
    pokemon->evAttack = pokemon->hiddenEvAttack;
    pokemon->evDefense = pokemon->hiddenEvDefense;
    pokemon->evSpecialAttack = pokemon->hiddenEvSpecialAttack;
    pokemon->evSpecialDefense = pokemon->hiddenEvSpecialDefense;
    pokemon->evSpeed = pokemon->hiddenEvSpeed;
    return 0;
}


//returns the damage given the move and both Pokemon
//does not apply any damage or make any changes to anything
enum effectiveness damage(int moveIndex, Pokemon* attackingPokemon, Pokemon* attackedPokemon) {
    
    //make sure moveIndex is in range
    if (moveIndex < 0 || moveIndex >= attackingPokemon->numberOfMoves) {
        return notEffective;
    }
    
    //use accuracy to see if the move is successful
    int accuracyRandomNumber = getRandomNumber(0, 100);
    if (accuracyRandomNumber > attackingPokemon->moves[moveIndex].accuracy) {
        return notEffective;
    }
    
    //calculate critical
    int criticalValue = 1;
    int criticalRate = 6;
    int randomCriticalNumber = getRandomNumber(0, 100);
    if (randomCriticalNumber < criticalRate) {
        criticalValue = CRITICAL;
    }
    
    //calculate stab
    int stab = 1;
    if (attackingPokemon->moves[moveIndex].type == attackedPokemon->type1 || attackingPokemon->moves[moveIndex].type == attackedPokemon->type2 || attackingPokemon->moves[moveIndex].type == attackingPokemon->type1 || attackingPokemon->moves[moveIndex].type == attackingPokemon->type2) {
        stab = STAB;
    }
    
    //calculate damage
    double damage;
    
    //if the move is physical
    if (attackingPokemon->moves[moveIndex].isPhysicalOrSpecial == physical) {
        
        //calculate damage
        damage = ((((2 * attackingPokemon->level)/5) + 2) * attackingPokemon->moves[moveIndex].damage * (( getAttack(*attackingPokemon) / getDefense(*attackedPokemon) )/50) + 2) * criticalValue * stab * getTypeMultipler(attackingPokemon->moves[moveIndex].type, attackedPokemon->type1, attackedPokemon->type2);
    }
    //if the move is special
    else if (attackingPokemon->moves[moveIndex].isPhysicalOrSpecial == special) {
        
        //calculate damage
        damage = ((((2 * attackingPokemon->level)/5) + 2) * attackingPokemon->moves[moveIndex].damage * (( getSpecialAttack(*attackingPokemon) / getSpecialDefense(*attackedPokemon) )/50) + 2) * criticalValue * stab * getTypeMultipler(attackingPokemon->moves[moveIndex].type, attackedPokemon->type1, attackedPokemon->type2);
    }
    //the move is not effective
    else {
        return notEffective;
    }
    
    
    //apply damage
    attackedPokemon->HP -= damage;
    
    //return effectiveness based on the typeMultiplier
    double typeMultiplier = getTypeMultipler(attackingPokemon->moves[moveIndex].type, attackedPokemon->type1, attackedPokemon->type2);
    if (typeMultiplier <= 0) {
        return notEffective;
    } else if (typeMultiplier < 1) {
        return notVeryEffective;
    } else if (typeMultiplier == 1) {
        return normalEffectiveness;
    } else if (typeMultiplier > 1) {
        return superEffective;
    }
    
    //won't ever reach here because the above if statements handle all cases
    return notEffective;
}

//gets the type multiplyer for attacking in battles
double getTypeMultipler(enum moveType moveType, enum moveType pokemonType1, enum moveType pokemonType2) {
    
    //start at 1.0
    double typeMultiplyer = 1.0;
    
    //based on the moveType, multipy typeMultiplyer by different values
    switch (moveType) {
        case normal:
            if (pokemonType1 == rock || pokemonType2 == rock) {
                typeMultiplyer *= 0.5;
            }
            break;
        case fight:
            if (pokemonType1 == normal || pokemonType2 == normal) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == flying || pokemonType2 == flying) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == poison || pokemonType2 == poison) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == rock || pokemonType2 == rock) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == bug || pokemonType2 == bug) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == phsycic || pokemonType2 == phsycic) {
                typeMultiplyer *= 0.5;
            }
            break;
        case flying:
            if (pokemonType1 == fight || pokemonType2 == fight) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == rock || pokemonType2 == rock) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == bug || pokemonType2 == bug) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == grass || pokemonType2 == grass) {
                typeMultiplyer *= 2.0;
            }
            break;
        case poison:
            if (pokemonType1 == poison || pokemonType2 == poison) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == ground || pokemonType2 == ground) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == rock || pokemonType2 == rock) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == grass || pokemonType2 == grass) {
                typeMultiplyer *= 2.0;
            }
            break;
        case ground:
            if (pokemonType1 == flying || pokemonType2 == flying) {
                typeMultiplyer *= 0.0;
            }
            if (pokemonType1 == poison || pokemonType2 == poison) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == rock || pokemonType2 == rock) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == bug || pokemonType2 == bug) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == grass || pokemonType2 == grass) {
                typeMultiplyer *= 0.5;
            }
            break;
        case rock:
            if (pokemonType1 == fight || pokemonType2 == fight) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == flying || pokemonType2 == flying) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == ground || pokemonType2 == ground) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == bug || pokemonType2 == bug) {
                typeMultiplyer *= 2.0;
            }
            break;
        case bug:
            if (pokemonType1 == fight || pokemonType2 == fight) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == flying || pokemonType2 == flying) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == poison || pokemonType2 == poison) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == fire || pokemonType2 == fire) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == grass || pokemonType2 == grass) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == phsycic || pokemonType2 == phsycic) {
                typeMultiplyer *= 2.0;
            }
            break;
        case fire:
            if (pokemonType1 == rock || pokemonType2 == rock) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == bug || pokemonType2 == bug) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == water || pokemonType2 == water) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == grass || pokemonType2 == grass) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == fire || pokemonType2 == fire) {
                typeMultiplyer *= 0.5;
            }
            break;
        case water:
            if (pokemonType1 == ground || pokemonType2 == ground) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == rock || pokemonType2 == rock) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == fire || pokemonType2 == fire) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == water || pokemonType2 == water) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == grass || pokemonType2 == grass) {
                typeMultiplyer *= 0.5;
            }
            break;
        case grass:
            if (pokemonType1 == flying || pokemonType2 == flying) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == poison || pokemonType2 == poison) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == ground || pokemonType2 == ground) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == rock || pokemonType2 == rock) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == bug || pokemonType2 == bug) {
                typeMultiplyer *= 0.5;
            }
            if (pokemonType1 == water || pokemonType2 == water) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == grass || pokemonType2 == grass) {
                typeMultiplyer *= 0.5;
            }
            break;
        case phsycic:
            if (pokemonType1 == fight || pokemonType2 == fight) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == poison || pokemonType2 == poison) {
                typeMultiplyer *= 2.0;
            }
            if (pokemonType1 == phsycic || pokemonType2 == phsycic) {
                typeMultiplyer *= 0.5;
            }
            break;
        case noType:
            break;
    }
    
    return typeMultiplyer;
    
}

//natures increase and decrease different stats
//returns the multiplyer for a stat based on the nature
double getNatureMultiplyer(enum stats stat, enum nature nature) {
    
    //for every nature check every stat and return the appropriate value
    switch (nature) {
        case hardy:
            return 1.0;
            break;
        case lonely:
            switch (stat) {
                case attack:
                    return 1.1;
                    break;
                case defense:
                    return 0.9;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case brave:
            switch (stat) {
                case attack:
                    return 1.1;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 0.9;
                    break;
            }
            break;
        case adamant:
            switch (stat) {
                case attack:
                    return 1.1;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 0.9;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case naughty:
            switch (stat) {
                case attack:
                    return 1.1;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 0.9;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case bold:
            switch (stat) {
                case attack:
                    return 0.9;
                    break;
                case defense:
                    return 1.1;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case docile:
            return 1.0;
            break;
        case relaxed:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 1.1;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 0.9;
                    break;
            }
            break;
        case impish:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 1.1;
                    break;
                case specialAttack:
                    return 0.9;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case lax:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 1.1;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 0.9;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case timid:
            switch (stat) {
                case attack:
                    return 0.9;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 1.1;
                    break;
            }
            break;
        case hasty:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 0.9;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 1.1;
                    break;
            }
            break;
        case serious:
            return 1.0;
            break;
        case jolly:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 0.9;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 1.1;
                    break;
            }
            break;
        case naive:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 0.9;
                    break;
                case speed:
                    return 1.1;
                    break;
            }
            break;
        case modest:
            switch (stat) {
                case attack:
                    return 0.9;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 1.1;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case mild:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 0.9;
                    break;
                case specialAttack:
                    return 1.1;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case quiet:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 1.1;
                    break;
                case specialDefense:
                    return 1.0;
                    break;
                case speed:
                    return 0.9;
                    break;
            }
            break;
        case bashful:
            return 1.0;
            break;
        case rash:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 1.1;
                    break;
                case specialDefense:
                    return 0.9;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case calm:
            switch (stat) {
                case attack:
                    return 0.9;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 1.1;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case gentle:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 0.9;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 1.1;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case sassy:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 1.0;
                    break;
                case specialDefense:
                    return 1.1;
                    break;
                case speed:
                    return 0.9;
                    break;
            }
            break;
        case careful:
            switch (stat) {
                case attack:
                    return 1.0;
                    break;
                case defense:
                    return 1.0;
                    break;
                case specialAttack:
                    return 0.9;
                    break;
                case specialDefense:
                    return 1.1;
                    break;
                case speed:
                    return 1.0;
                    break;
            }
            break;
        case lastNature:
            return 1.0;
            break;
    }
    return 1.0;
    
}



//MARK: - Converting Enums and Strings

//converts a physicalOrSpecial value to a string
void convertPhysicalOrSpecialToString(enum physicalOrSpecial physicalOrSpecial, char physicalOrSpecialAsString[MAXSTRINGLENGTH]) {
    
    switch (physicalOrSpecial) {
        case physical:
            strcpy(physicalOrSpecialAsString, "Physical");
            break;
        case special:
            strcpy(physicalOrSpecialAsString, "Special");
            break;
    }
    
}

//converts a physicalOrSpecial string to an enum
enum physicalOrSpecial convertPhysicalOrSpecialStringToPhysicalOrSpecial(char physicalOrSpecialString[MAXSTRINGLENGTH]) {
    
    if (!strcmp(physicalOrSpecialString, "Physical")) {
        return physical;
    } else if (!strcmp(physicalOrSpecialString, "Special")) {
        return special;
    } else {
        printf("Error: %s is not a valid value of physical or special. Pokemon will continue to run using physical.\n", physicalOrSpecialString);
        return physical;
    }
    
}

//converts a moveType to a string
void convertMoveTypeToString(enum moveType moveType, char moveTypeAsString[MAXSTRINGLENGTH]) {
    
    switch (moveType) {
        case bug:
            strcpy(moveTypeAsString, "Bug");
            break;
        case fight:
            strcpy(moveTypeAsString, "Fight");
            break;
        case fire:
            strcpy(moveTypeAsString, "Fire");
            break;
        case flying:
            strcpy(moveTypeAsString, "Flying");
            break;
        case grass:
            strcpy(moveTypeAsString, "Grass");
            break;
        case ground:
            strcpy(moveTypeAsString, "Ground");
            break;
        case normal:
            strcpy(moveTypeAsString, "Normal");
            break;
        case poison:
            strcpy(moveTypeAsString, "Poison");
            break;
        case phsycic:
            strcpy(moveTypeAsString, "Phsycic");
            break;
        case rock:
            strcpy(moveTypeAsString, "Rock");
            break;
        case water:
            strcpy(moveTypeAsString, "Water");
            break;
        case noType:
            strcpy(moveTypeAsString, "noType");
            break;
    }
    
}

//converts a moveType string to an enum
enum moveType convertMoveTypeStringToMoveType(char moveTypeAsString[MAXSTRINGLENGTH]) {
    
    if (!strcmp(moveTypeAsString, "Bug")) {
        return bug;
    } else if (!strcmp(moveTypeAsString, "Fight")) {
        return fight;
    } else if (!strcmp(moveTypeAsString, "Fire")) {
        return fire;
    } else if (!strcmp(moveTypeAsString, "Flying")) {
        return flying;
    } else if (!strcmp(moveTypeAsString, "Grass")) {
        return grass;
    } else if (!strcmp(moveTypeAsString, "Ground")) {
        return ground;
    } else if (!strcmp(moveTypeAsString, "Normal")) {
        return normal;
    } else if (!strcmp(moveTypeAsString, "Poison")) {
        return poison;
    } else if (!strcmp(moveTypeAsString, "Phsycic")) {
        return phsycic;
    } else if (!strcmp(moveTypeAsString, "Rock")) {
        return rock;
    } else if (!strcmp(moveTypeAsString, "Water")) {
        return water;
    } else if (!strcmp(moveTypeAsString, "noType")) {
        return noType;
    } else {
        printf("Error: %s is not a valid move type. Pokemon will continue to run using a normal type instead of a %s type.\n", moveTypeAsString, moveTypeAsString);
        return normal;
    }
    
}

//converts a moveName to a string
void convertMoveNameToString(enum moveName moveName, char moveNameAsString[MAXSTRINGLENGTH]) {
    
    switch (moveName) {
        case tackle:
            strcpy(moveNameAsString, "Tackle");
            break;
        case confusion:
            strcpy(moveNameAsString, "Confusion");
            break;
        case gust:
            strcpy(moveNameAsString, "Gust");
            break;
        case silverWind:
            strcpy(moveNameAsString, "SilverWind");
            break;
        case pursuit:
            strcpy(moveNameAsString, "Pursuit");
            break;
        case hyperFang:
            strcpy(moveNameAsString, "HyperFang");
            break;
        case ironTail:
            strcpy(moveNameAsString, "IronTail");
            break;
        case wingAttack:
            strcpy(moveNameAsString, "WingAttack");
            break;
        case steelWing:
            strcpy(moveNameAsString, "SteelWing");
            break;
        case airSlash:
            strcpy(moveNameAsString, "AirSlash");
            break;
        case flamethrower:
            strcpy(moveNameAsString, "Flamethrower");
            break;
        case slash:
            strcpy(moveNameAsString, "Slash");
            break;
        case fireBlast:
            strcpy(moveNameAsString, "FireBlast");
            break;
        case razorLeaf:
            strcpy(moveNameAsString, "RazorLeaf");
            break;
        case sludgeBomb:
            strcpy(moveNameAsString, "SludgeBomb");
            break;
        case powerWhip:
            strcpy(moveNameAsString, "PowerWhip");
            break;
        case engeryBall:
            strcpy(moveNameAsString, "EnergyBall");
            break;
        case surf:
            strcpy(moveNameAsString, "Surf");
            break;
        case iceBeam:
            strcpy(moveNameAsString, "IceBeam");
            break;
        case hydroPump:
            strcpy(moveNameAsString, "HydroPump");
            break;
        case strength:
            strcpy(moveNameAsString, "Strength");
            break;
    }
    
}

//converts a moveName string to an enum
enum moveName convertMoveNameStringToMoveName(char moveNameAsString[MAXSTRINGLENGTH]) {
    
    if (!strcmp(moveNameAsString, "Tackle")) {
        return tackle;
    } else if (!strcmp(moveNameAsString, "Confusion")) {
        return confusion;
    } else if (!strcmp(moveNameAsString, "Gust")) {
        return gust;
    } else if (!strcmp(moveNameAsString, "SilverWind")) {
        return silverWind;
    } else if (!strcmp(moveNameAsString, "Pursuit")) {
        return pursuit;
    } else if (!strcmp(moveNameAsString, "HyperFang")) {
        return hyperFang;
    } else if (!strcmp(moveNameAsString, "IronTail")) {
        return ironTail;
    } else if (!strcmp(moveNameAsString, "WingAttack")) {
        return wingAttack;
    } else if (!strcmp(moveNameAsString, "SteelWing")) {
        return steelWing;
    } else if (!strcmp(moveNameAsString, "AirSlash")) {
        return airSlash;
    } else if (!strcmp(moveNameAsString, "Flamethrower")) {
        return flamethrower;
    } else if (!strcmp(moveNameAsString, "Slash")) {
        return slash;
    } else if (!strcmp(moveNameAsString, "FireBlast")) {
        return fireBlast;
    } else if (!strcmp(moveNameAsString, "RazorLeaf")) {
        return razorLeaf;
    } else if (!strcmp(moveNameAsString, "SludgeBomb")) {
        return sludgeBomb;
    } else if (!strcmp(moveNameAsString, "PowerWhip")) {
        return powerWhip;
    } else if (!strcmp(moveNameAsString, "EnergyBall")) {
        return engeryBall;
    } else if (!strcmp(moveNameAsString, "Surf")) {
        return surf;
    } else if (!strcmp(moveNameAsString, "IceBeam")) {
        return iceBeam;
    } else if (!strcmp(moveNameAsString, "HydroPump")) {
        return hydroPump;
    } else if (!strcmp(moveNameAsString, "Strength")) {
        return strength;
    } else {
        printf("Error: %s is not a valid move. Pokemon will continue to run using tackle instead of %s.\n", moveNameAsString, moveNameAsString);
        return tackle;
    }
    
}

//converts a nature to a string
void convertNatureToString(enum nature nature, char natureAsString[MAXSTRINGLENGTH]) {
    switch (nature) {
        case hardy:
            strcpy(natureAsString, "hardy");
            break;
        case lonely:
            strcpy(natureAsString, "lonely");
            break;
        case brave:
            strcpy(natureAsString, "brave");
            break;
        case adamant:
            strcpy(natureAsString, "adament");
            break;
        case naughty:
            strcpy(natureAsString, "naughty");
            break;
        case bold:
            strcpy(natureAsString, "bold");
            break;
        case docile:
            strcpy(natureAsString, "docile");
            break;
        case relaxed:
            strcpy(natureAsString, "relaxed");
            break;
        case impish:
            strcpy(natureAsString, "impish");
            break;
        case lax:
            strcpy(natureAsString, "lax");
            break;
        case timid:
            strcpy(natureAsString, "timid");
            break;
        case hasty:
            strcpy(natureAsString, "hasty");
            break;
        case serious:
            strcpy(natureAsString, "serious");
            break;
        case jolly:
            strcpy(natureAsString, "jolly");
            break;
        case naive:
            strcpy(natureAsString, "naive");
            break;
        case modest:
            strcpy(natureAsString, "modest");
            break;
        case mild:
            strcpy(natureAsString, "mild");
            break;
        case quiet:
            strcpy(natureAsString, "quiet");
            break;
        case bashful:
            strcpy(natureAsString, "bashful");
            break;
        case rash:
            strcpy(natureAsString, "rash");
            break;
        case calm:
            strcpy(natureAsString, "calm");
            break;
        case gentle:
            strcpy(natureAsString, "gentle");
            break;
        case sassy:
            strcpy(natureAsString, "sassy");
            break;
        case careful:
            strcpy(natureAsString, "careful");
            break;
        case lastNature:
            strcpy(natureAsString, "lastNature");
    }
}

//converts a nature string to an enum
enum nature convertNatureStringToNature(char natureAsString[MAXSTRINGLENGTH]) {
    
    if (!strcmp(natureAsString, "hardy")) {
        return hardy;
    } else if (!strcmp(natureAsString, "lonely")) {
        return lonely;
    } else if (!strcmp(natureAsString, "brave")) {
        return brave;
    } else if (!strcmp(natureAsString, "adament")) {
        return adamant;
    } else if (!strcmp(natureAsString, "naughty")) {
        return naughty;
    } else if (!strcmp(natureAsString, "bold")) {
        return bold;
    } else if (!strcmp(natureAsString, "docile")) {
        return docile;
    } else if (!strcmp(natureAsString, "relaxed")) {
        return relaxed;
    } else if (!strcmp(natureAsString, "impish")) {
        return impish;
    } else if (!strcmp(natureAsString, "lax")) {
        return lax;
    } else if (!strcmp(natureAsString, "timid")) {
        return timid;
    } else if (!strcmp(natureAsString, "hasty")) {
        return hasty;
    } else if (!strcmp(natureAsString, "serious")) {
        return serious;
    } else if (!strcmp(natureAsString, "jolly")) {
        return jolly;
    } else if (!strcmp(natureAsString, "naive")) {
        return naive;
    } else if (!strcmp(natureAsString, "modest")) {
        return modest;
    } else if (!strcmp(natureAsString, "mild")) {
        return mild;
    } else if (!strcmp(natureAsString, "quiet")) {
        return quiet;
    } else if (!strcmp(natureAsString, "bashful")) {
        return bashful;
    } else if (!strcmp(natureAsString, "rash")) {
        return rash;
    } else if (!strcmp(natureAsString, "calm")) {
        return calm;
    } else if (!strcmp(natureAsString, "gentle")) {
        return gentle;
    } else if (!strcmp(natureAsString, "sassy")) {
        return sassy;
    } else if (!strcmp(natureAsString, "careful")) {
        return careful;
    } else if (!strcmp(natureAsString, "lastNature")) {
        return lastNature;
    } else {
        printf("Error: %s is not a valid nature. Pokemon will continue to run using the nature relaxed.\n", natureAsString);
        return relaxed;
    }
    
}

//converts a pokemon name to a string
void convertPokemonNextEvolutionToString(enum pokemonName nextEvolution, char nextEvolutionName[MAXSTRINGLENGTH]) {
    
    switch (nextEvolution) {
        case caterpie:
            strcpy(nextEvolutionName, "Caterpie");
            break;
        case metapod:
            strcpy(nextEvolutionName, "Metapod");
            break;
        case butterfree:
            strcpy(nextEvolutionName, "Butterfree");
            break;
        case rattata:
            strcpy(nextEvolutionName, "Rattata");
            break;
        case raticate:
            strcpy(nextEvolutionName, "Raticate");
            break;
        case pidgey:
            strcpy(nextEvolutionName, "Pidgey");
            break;
        case pidgeotto:
            strcpy(nextEvolutionName, "Pidgeotto");
            break;
        case pidgeot:
            strcpy(nextEvolutionName, "Pidgeot");
            break;
        case charmander:
            strcpy(nextEvolutionName, "Charmander");
            break;
        case charmeleon:
            strcpy(nextEvolutionName, "Charmeleon");
            break;
        case charizard:
            strcpy(nextEvolutionName, "Charizard");
            break;
        case bulbasaur:
            strcpy(nextEvolutionName, "Bulbasaur");
            break;
        case ivysaur:
            strcpy(nextEvolutionName, "Ivysaur");
            break;
        case venusaur:
            strcpy(nextEvolutionName, "Venusaur");
            break;
        case squirtle:
            strcpy(nextEvolutionName, "Squirtle");
            break;
        case wartortle:
            strcpy(nextEvolutionName, "Wartortle");
            break;
        case blastoise:
            strcpy(nextEvolutionName, "Blastoise");
            break;
        case none:
            strcpy(nextEvolutionName, "None");
    }
    
}

//converts a pokemon name string to an enum
enum pokemonName convertPokemonStringToNextEvolution(char pokemonString[MAXSTRINGLENGTH]) {
    
    if (!strcmp(pokemonString, "Caterpie")) {
        return caterpie;
    } else if (!strcmp(pokemonString, "Metapod")) {
        return metapod;
    } else if (!strcmp(pokemonString, "Butterfree")) {
        return butterfree;
    } else if (!strcmp(pokemonString, "Rattata")) {
        return rattata;
    } else if (!strcmp(pokemonString, "Raticate")) {
        return raticate;
    } else if (!strcmp(pokemonString, "Pidgey")) {
        return pidgey;
    } else if (!strcmp(pokemonString, "Pidgeotto")) {
        return pidgeotto;
    } else if (!strcmp(pokemonString, "Pidgeot")) {
        return pidgeot;
    } else if (!strcmp(pokemonString, "Charmander")) {
        return charmander;
    } else if (!strcmp(pokemonString, "Charmeleon")) {
        return charmeleon;
    } else if (!strcmp(pokemonString, "Charizard")) {
        return charizard;
    } else if (!strcmp(pokemonString, "Bulbasaur")) {
        return bulbasaur;
    } else if (!strcmp(pokemonString, "Ivysaur")) {
        return ivysaur;
    } else if (!strcmp(pokemonString, "Venusaur")) {
        return venusaur;
    } else if (!strcmp(pokemonString, "Squirtle")) {
        return squirtle;
    } else if (!strcmp(pokemonString, "Wartortle")) {
        return wartortle;
    } else if (!strcmp(pokemonString, "Blastoise")) {
        return blastoise;
    } else if (!strcmp(pokemonString, "None")) {
        return none;
    } else {
        printf("Error: There is no pokemon with name %s. Pokemon will continue to run using the name Caterpie instead.\n", pokemonString);
        return caterpie;
    }
    
}

//converts a stat to a string
void convertStatToString(enum stats stat, char statAsString[MAXSTRINGLENGTH]) {
    
    switch (stat) {
        case attack:
            strcpy(statAsString, "Attack");
            break;
        case defense:
            strcpy(statAsString, "Defense");
            break;
        case specialAttack:
            strcpy(statAsString, "SpecialAttack");
            break;
        case specialDefense:
            strcpy(statAsString, "SpecialDefense");
            break;
        case speed:
            strcpy(statAsString, "Speed");
            break;
    }
    
}


//converts a stat string to an enum
enum stats convertStatStringToStat(char statAsString[MAXSTRINGLENGTH]) {
    
    if (!strcmp(statAsString, "Attack")) {
        return attack;
    } else if (!strcmp(statAsString, "Defense")) {
        return defense;
    } else if (!strcmp(statAsString, "SpecialAttack")) {
        return specialAttack;
    } else if (!strcmp(statAsString, "SpecialDefense")) {
        return specialDefense;
    } else if (!strcmp(statAsString, "Speed")) {
        return speed;
    } else {
        printf("Error: There is no stat with the name %s. Using the stat attack instead.\n", statAsString);
        return attack;
    }
    
}
