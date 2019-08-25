/* ************************************************************************************************************************************************************************* */
/* ************************************************************************************************************************************************************************* */
/* This program is a computer version of Monopoly. There are still a number of bugs, but the core functions are present. If I could have some feedback, that would be great! */
/* ************************************************************************************************************************************************************************* */
/* ************************************************************************************************************************************************************************* */

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <cstring>

using namespace std;

void clearscreen(void);

int parkSpaceJackpot = 0;
int numDoubles = 0;
int turnNum = 0;

int numPlayers;



enum ColorType {PURPLE, SKY, PINK, ORANGE, RED, YELLOW, GREEN, BLUE, WHITE};

struct Player {
    string playerName;

    int playerNum;

    /* For the player position */
    int intPosition;
    int * const intPositionPtr = &intPosition;

    /* Wealth */
    int intWealth;
    int *wealthPtr = &intWealth;

    bool inJail; // Is the player in jail?
    bool outJailFree; // Does the player have a get out of jail free card?
    int numTurnInJail;

    bool boolKeepbidding;

    bool boolBankrupt; // Is the player bankrupt?

    Player(string newPlayerName = " ", int newPlayerNum = -1, int newIntWealth =  1500, bool newOutJail = false, int newNumTurnJail = 0){
        *intPositionPtr = 0;

        playerName = newPlayerName;
        playerNum = newPlayerNum;

        *wealthPtr = newIntWealth;
        inJail = false;
        outJailFree = newOutJail;
        numTurnInJail = newNumTurnJail;

        boolKeepbidding = false;
        boolBankrupt = false;

    }
};

Player gamePlayers[8]; // Array of players, sized to the max number of players (8)

struct Property {
    string propertyName;

    int propertyPrice;

     /* For the property position */
    int intPosition;
    //int * const intPositionPtr = intPosition;

    bool ownedProperty; // Is the property owned by a player?
    Player *propertyOwner; // If it is owned, this stores which player
    bool ownAllColor; // Flag if owner owns all of the like colors

    /* Stores the rent values depending on the houses */
    int rentNoHouse;
    int rentOneHouse;
    int rentTwoHouse;
    int rentThreeHouse;
    int rentFourHouse;
    int rentHotel;
    int numHouses; // How many houses the property has

    int buildCost;

    int mortgageValue;
    bool inMortgage;

    ColorType colorGroup;

    /* If the card is a chance, community chest, go, etc, it will have a special function */
    int intPropertyType;


    Property(string newName = " ", int newPosition = 0, int newPropertyType = -1,
             int newMortgageValue = -1, int newPropertyPrice = -1, int newRentNoHouse = -1,
             int newRentOneHouse = -1, int newRentTwoHouse = -1, int newRentThreeHouse = -1,
             int newRentFourHouse = -1, int newRentHotel = -1, int newBuildCost = -1,
             ColorType newColorGroup = WHITE, int newNumHouses = 0){

        propertyName = newName;
        propertyName = newMortgageValue;
        intPropertyType = newPropertyType;

        intPosition = newPosition;


        mortgageValue = newMortgageValue;

        rentNoHouse = newRentNoHouse;
        rentOneHouse = newRentNoHouse;
        rentTwoHouse = newRentTwoHouse;
        rentThreeHouse = newRentThreeHouse;
        rentFourHouse = newRentFourHouse;
        rentHotel = newRentHotel;
        numHouses = newNumHouses;

        buildCost = newBuildCost;

        colorGroup = newColorGroup;
        ownAllColor = false;

        ownedProperty = false;
        inMortgage = false;

    }

};

class GameBoard{

    private:
        Property gameProperties[40]; /* =   {{"Go", 0, 5},
                                        {"Mediterranean Avenue", 1, 0, 30, 60, 2, 10, 30, 90, 160, 250, 50, PURPLE},
                                        {"Community Chest", 2, 4},
                                        {"Baltic Ave.", 3, 0, 30, 60, 4, 20, 60, 180, 320, 450, 50, PURPLE, 60},
                                        {"Income Tax", 4, 6},
                                        {"Reading Railroad", 5, 1, 100, 200},
                                        {"Oriental Ave.", 6, 0, 50, 100, 6, 30, 90, 270, 400, 550, 50, SKY},
                                        {"Chance", 7, 3},
                                        {"Vermont Ave.", 8, 0, 50, 100, 6, 30, 90, 270, 400, 550, 50, SKY},
                                        {"Connecticut Ave.", 9, 0, 60, 120, 8, 40, 100, 300, 450, 600, 50, SKY},
                                        {"Jail", 10, 7},
                                        {"St. Charles Place", 11, 0, 70, 140, 10, 50, 150, 450, 625, 750, 100, PINK},
                                        {"Electric Company", 12, 2, 75, 150},
                                        {"States Ave.", 13, 0, 70, 140, 10, 50, 150, 450, 625, 750, 100, PINK},
                                        {"Virginia Ave.", 14, 0, 80, 160, 12, 60, 180, 500, 700, 900, 100, PINK},
                                        {"Pennsylvania Railroad", 15, 1, 100, 200},
                                        {"St. James Place", 16, 0, 90, 180, 14, 70, 200, 550, 750, 950, 100, ORANGE},
                                        {"Community Chest", 17, 4},
                                        {"Tennessee Ave.", 18, 0, 90, 180, 14, 70, 200, 550, 750, 950, 100, ORANGE},
                                        {"New York Ave.", 19, 0, 100, 200, 16, 80, 220, 600, 800, 1000, 100, ORANGE},
                                        {"Free Parking", 20, 8},
                                        {"Kentucky Ave.", 21, 0, 110, 220, 18, 90, 250, 700, 875, 1050, 150, RED},
                                        {"Chance", 22, 10, 3},
                                        {"Indiana Ave.", 23, 0, 110, 220, 18, 90, 250, 700, 875, 1050, 150, RED},
                                        {"Illinois Ave.", 24, 0, 120, 240, 20, 100, 300, 750, 925, 1100, 150, RED},
                                        {"B.&O. Railroad", 25, 1, 100, 200},
                                        {"Atlantic Ave.", 26, 0, 130, 260, 22, 110, 330, 800, 975, 1150, 150, YELLOW},
                                        {"Ventnor Ave.", 27, 0, 130, 260, 22, 110, 330, 800, 975, 1150, 150, YELLOW},
                                        {"Water Works", 28, 2, 75, 150},
                                        {"Marvin Gardens", 29, 0, 140, 280, 24, 120, 360, 850, 1025, 1200, 150, YELLOW},
                                        {"Go To Jail", 30, 9},
                                        {"Pacific Ave.", 31, 0, 150, 300, 26, 130, 390, 900, 1100, 1275, 200, GREEN },
                                        {"North Carolina Ave.", 32, 0, 150, 300, 26, 130, 390, 900, 1100, 1275, 200, GREEN},
                                        {"Community Chest", 33, 4},
                                        {"Pennsylvania Ave.", 34, 0, 160, 320, 28, 150, 450, 1000, 1200, 1400, 200, GREEN},
                                        {"Short Line", 35, 1, 100, 200},
                                        {"Chance", 36, 3},
                                        {"Park Place", 37, 0, 175, 350, 35, 175, 500, 1100, 1300, 1500, 200, BLUE},
                                        {"Luxury Tax", 38, 10},
                                        {"Boardwalk", 39, 0, 200, 400, 50, 200, 600, 1400, 1700, 2000, 200, BLUE}}; // array of properties for 40 positions
*/
    //Property * const gamePropPtr = &gameProperties[0][0];
        int numProperties;

    public:
        GameBoard(){
            gameProperties[0].propertyName = "Go";
            gameProperties[0].intPosition = 0;
            gameProperties[0].intPropertyType = 5;
            gameProperties[1].propertyName = "Mediterranean Avenue";
            gameProperties[1].intPosition =  1;
            gameProperties[1].intPropertyType = 0;
            gameProperties[1].propertyPrice = 60;
            gameProperties[1].mortgageValue = 30;
            gameProperties[1].rentNoHouse = 2;
            gameProperties[1].rentOneHouse = 10;
            gameProperties[1].rentTwoHouse = 30;
            gameProperties[1].rentThreeHouse = 90;
            gameProperties[1].rentFourHouse = 160;
            gameProperties[1].rentHotel = 250;
            gameProperties[1].buildCost = 50;
            gameProperties[1].colorGroup = PURPLE;
            gameProperties[2].propertyName = "Community Chest";
            gameProperties[2].intPosition =  2;
            gameProperties[2].intPropertyType = 4;
            gameProperties[3].propertyName = "Baltic Ave.";
            gameProperties[3].intPosition = 3;
            gameProperties[3].intPropertyType = 0;
            gameProperties[3].propertyPrice = 30;
            gameProperties[3].mortgageValue = 60;
            gameProperties[3].rentNoHouse = 4;
            gameProperties[3].rentOneHouse = 20;
            gameProperties[3].rentTwoHouse = 60;
            gameProperties[3].rentThreeHouse = 180;
            gameProperties[3].rentFourHouse = 320;
            gameProperties[3].rentHotel = 450;
            gameProperties[3].buildCost = 50;
            gameProperties[3].colorGroup = PURPLE;
            gameProperties[4].propertyName = "Income Tax";
            gameProperties[4].intPosition = 4;
            gameProperties[4].intPropertyType = 6;
            gameProperties[5].propertyName = "Reading Railroad";
            gameProperties[5].intPosition = 5;
            gameProperties[5].intPropertyType = 1;
            gameProperties[5].propertyPrice = 200;
            gameProperties[5].mortgageValue = 100;
            gameProperties[6].propertyName = "Oriental Ave.";
            gameProperties[6].intPosition =  6;
            gameProperties[6].intPropertyType = 0;
            gameProperties[6].propertyPrice = 50;
            gameProperties[6].mortgageValue = 100;
            gameProperties[6].rentNoHouse = 6;
            gameProperties[6].rentOneHouse =  30;
            gameProperties[6].rentTwoHouse = 90;
            gameProperties[6].rentThreeHouse = 270;
            gameProperties[6].rentFourHouse = 400;
            gameProperties[6].rentHotel =  550;
            gameProperties[6].buildCost =  50;
            gameProperties[6].colorGroup = SKY;
            gameProperties[7].propertyName = "Chance";
            gameProperties[7].intPosition = 7;
            gameProperties[7].intPropertyType = 3;
            gameProperties[8].propertyName = "Vermont Ave.";
            gameProperties[8].intPosition = 8;
            gameProperties[8].intPropertyType = 0;
            gameProperties[8].propertyPrice = 100;
            gameProperties[8].mortgageValue = 50;
            gameProperties[8].rentNoHouse = 6;
            gameProperties[8].rentOneHouse = 30;
            gameProperties[8].rentTwoHouse = 90;
            gameProperties[8].rentThreeHouse = 270;
            gameProperties[8].rentFourHouse = 400;
            gameProperties[8].rentHotel = 550;
            gameProperties[8].buildCost = 50;
            gameProperties[8].colorGroup = SKY;
            gameProperties[9].propertyName = "Connecticut Ave.";
            gameProperties[9].intPosition = 9;
            gameProperties[9].intPropertyType = 0;
            gameProperties[9].propertyPrice = 120;
            gameProperties[9].mortgageValue = 60;
            gameProperties[9].rentNoHouse = 8;
            gameProperties[9].rentOneHouse = 40;
            gameProperties[9].rentTwoHouse = 100;
            gameProperties[9].rentThreeHouse = 300;
            gameProperties[9].rentFourHouse = 450;
            gameProperties[9].rentHotel = 600;
            gameProperties[9].buildCost = 50;
            gameProperties[9].colorGroup = SKY;
            gameProperties[10].propertyName = "Jail";
            gameProperties[10].intPosition = 10;
            gameProperties[10].intPropertyType = 7;
            gameProperties[11].propertyName = "St. Charles Place";
            gameProperties[11].intPosition = 11;
            gameProperties[11].intPropertyType = 0;
            gameProperties[11].propertyPrice = 140;
            gameProperties[11].mortgageValue = 70;
            gameProperties[11].rentNoHouse = 10;
            gameProperties[11].rentOneHouse = 50;
            gameProperties[11].rentTwoHouse = 150;
            gameProperties[11].rentThreeHouse = 450;
            gameProperties[11].rentFourHouse = 625;
            gameProperties[11].rentHotel = 750;
            gameProperties[11].buildCost = 100;
            gameProperties[11].colorGroup = PINK;
            gameProperties[12].propertyName = "Electric Company";
            gameProperties[12].intPosition = 12;
            gameProperties[12].intPropertyType = 2;
            gameProperties[12].propertyPrice = 150;
            gameProperties[12].mortgageValue = 75;
            gameProperties[13].propertyName = "States Ave.";
            gameProperties[13].intPosition = 13;
            gameProperties[13].intPropertyType = 0;
            gameProperties[13].propertyPrice = 140;
            gameProperties[13].mortgageValue = 70;
            gameProperties[13].rentNoHouse = 10;
            gameProperties[13].rentOneHouse = 50;
            gameProperties[13].rentTwoHouse = 150;
            gameProperties[13].rentThreeHouse = 450;
            gameProperties[13].rentFourHouse = 625;
            gameProperties[13].rentHotel = 750;
            gameProperties[13].buildCost = 100;
            gameProperties[13].colorGroup = PINK;
            gameProperties[14].propertyName = "Virginia Ave.";
            gameProperties[14].intPosition = 14;
            gameProperties[14].intPropertyType = 0;
            gameProperties[14].propertyPrice = 160;
            gameProperties[14].mortgageValue = 80;
            gameProperties[14].rentNoHouse = 12;
            gameProperties[14].rentOneHouse = 60;
            gameProperties[14].rentTwoHouse = 180;
            gameProperties[14].rentThreeHouse = 500;
            gameProperties[14].rentFourHouse = 700;
            gameProperties[14].rentHotel = 900;
            gameProperties[14].buildCost = 100;
            gameProperties[14].colorGroup = PINK;
            gameProperties[15].propertyName = "Pennsylvania Railroad";
            gameProperties[15].intPosition = 15;
            gameProperties[15].intPropertyType = 1;
            gameProperties[15].propertyPrice = 200;
            gameProperties[15].mortgageValue = 100;
            gameProperties[16].propertyName = "St. James Place";
            gameProperties[16].intPosition = 16;
            gameProperties[16].intPropertyType = 0;
            gameProperties[16].propertyPrice = 180;
            gameProperties[16].mortgageValue = 90;
            gameProperties[16].rentNoHouse = 14;
            gameProperties[16].rentOneHouse = 70;
            gameProperties[16].rentTwoHouse = 200;
            gameProperties[16].rentThreeHouse = 550;
            gameProperties[16].rentFourHouse = 750;
            gameProperties[16].rentHotel = 950;
            gameProperties[16].buildCost = 100;
            gameProperties[16].colorGroup = ORANGE;
            gameProperties[17].propertyName = "Community Chest";
            gameProperties[17].intPosition = 17;
            gameProperties[17].intPropertyType = 4;
            gameProperties[18].propertyName = "Tennessee Ave.";
            gameProperties[18].intPosition = 18;
            gameProperties[18].intPropertyType = 0;
            gameProperties[18].propertyPrice = 180;
            gameProperties[18].mortgageValue = 90;
            gameProperties[18].rentNoHouse = 14;
            gameProperties[18].rentOneHouse = 70;
            gameProperties[18].rentTwoHouse = 200;
            gameProperties[18].rentThreeHouse = 550;
            gameProperties[18].rentFourHouse = 750;
            gameProperties[18].rentHotel = 950;
            gameProperties[18].buildCost = 100;
            gameProperties[18].colorGroup = ORANGE;
            gameProperties[19].propertyName = "New York Ave.";
            gameProperties[19].intPosition = 19;
            gameProperties[19].intPropertyType = 0;
            gameProperties[19].propertyPrice = 180;
            gameProperties[19].mortgageValue = 90;
            gameProperties[19].rentNoHouse = 14;
            gameProperties[19].rentOneHouse = 70;
            gameProperties[19].rentTwoHouse = 200;
            gameProperties[19].rentThreeHouse = 550;
            gameProperties[19].rentFourHouse = 750;
            gameProperties[19].rentHotel = 950;
            gameProperties[19].buildCost = 100;
            gameProperties[19].colorGroup = ORANGE;
            gameProperties[20].propertyName = "Free Parking";
            gameProperties[20].intPosition = 20;
            gameProperties[20].intPropertyType = 8;
            gameProperties[21].propertyName = "Kentucky Ave.";
            gameProperties[21].intPosition = 21;
            gameProperties[21].intPropertyType = 0;
            gameProperties[21].propertyPrice = 220;
            gameProperties[21].mortgageValue = 110;
            gameProperties[21].rentNoHouse = 18;
            gameProperties[21].rentOneHouse = 90;
            gameProperties[21].rentTwoHouse = 250;
            gameProperties[21].rentThreeHouse = 700;
            gameProperties[21].rentFourHouse = 875;
            gameProperties[21].rentHotel = 1050;
            gameProperties[21].buildCost = 150;
            gameProperties[21].colorGroup = RED;
            gameProperties[22].propertyName = "Chance";
            gameProperties[22].intPosition = 22;
            gameProperties[22].intPropertyType = 3;
            gameProperties[23].propertyName = "Indiana Ave.";
            gameProperties[23].intPosition = 23;
            gameProperties[23].intPropertyType = 0;
            gameProperties[23].propertyPrice = 220;
            gameProperties[23].mortgageValue = 110;
            gameProperties[23].rentNoHouse = 18;
            gameProperties[23].rentOneHouse = 90;
            gameProperties[23].rentTwoHouse = 250;
            gameProperties[23].rentThreeHouse = 700;
            gameProperties[23].rentFourHouse = 875;
            gameProperties[23].rentHotel = 1050;
            gameProperties[23].buildCost = 150;
            gameProperties[23].colorGroup = RED;
            gameProperties[24].propertyName = "Illinois Ave.";
            gameProperties[24].intPosition = 24;
            gameProperties[24].intPropertyType = 0;
            gameProperties[24].propertyPrice = 240;
            gameProperties[24].mortgageValue = 120;
            gameProperties[24].rentNoHouse = 20;
            gameProperties[24].rentOneHouse = 100;
            gameProperties[24].rentTwoHouse = 300;
            gameProperties[24].rentThreeHouse = 750;
            gameProperties[24].rentFourHouse = 925;
            gameProperties[24].rentHotel = 1150;
            gameProperties[24].buildCost = 150;
            gameProperties[24].colorGroup = YELLOW;
            gameProperties[25].propertyName = "B. & O. Railroad";
            gameProperties[25].intPosition = 25;
            gameProperties[25].intPropertyType = 1;
            gameProperties[25].propertyPrice = 200;
            gameProperties[25].mortgageValue = 100;
            gameProperties[26].propertyName = "Atlantic Ave.";
            gameProperties[26].intPosition = 26;
            gameProperties[26].intPropertyType = 0;
            gameProperties[26].propertyPrice = 260;
            gameProperties[26].mortgageValue = 130;
            gameProperties[26].rentNoHouse = 22;
            gameProperties[26].rentOneHouse = 110;
            gameProperties[26].rentTwoHouse = 330;
            gameProperties[26].rentThreeHouse = 800;
            gameProperties[26].rentFourHouse = 975;
            gameProperties[26].rentHotel = 1100;
            gameProperties[26].buildCost = 150;
            gameProperties[26].colorGroup = YELLOW;
            gameProperties[27].propertyName = "Ventnor Ave.";
            gameProperties[27].intPosition = 27;
            gameProperties[27].intPropertyType = 0;
            gameProperties[27].propertyPrice = 260;
            gameProperties[27].mortgageValue = 130;
            gameProperties[27].rentNoHouse = 22;
            gameProperties[27].rentOneHouse = 110;
            gameProperties[27].rentTwoHouse = 330;
            gameProperties[27].rentThreeHouse = 800;
            gameProperties[27].rentFourHouse = 975;
            gameProperties[27].rentHotel = 1150;
            gameProperties[27].buildCost = 150;
            gameProperties[27].colorGroup = YELLOW;
            gameProperties[28].propertyName = "Water Works";
            gameProperties[28].intPosition = 28;
            gameProperties[28].intPropertyType = 2;
            gameProperties[28].propertyPrice = 150;
            gameProperties[28].mortgageValue = 75;
            gameProperties[29].propertyName = "Marvin Gardens";
            gameProperties[29].intPosition = 29;
            gameProperties[29].intPropertyType = 0;
            gameProperties[29].propertyPrice = 280;
            gameProperties[29].mortgageValue = 140;
            gameProperties[29].rentNoHouse = 24;
            gameProperties[29].rentOneHouse = 120;
            gameProperties[29].rentTwoHouse = 360;
            gameProperties[29].rentThreeHouse = 850;
            gameProperties[29].rentFourHouse = 1025;
            gameProperties[29].rentHotel = 1200;
            gameProperties[29].buildCost = 150;
            gameProperties[29].colorGroup = YELLOW;
            gameProperties[30].propertyName = "Go to Jail";
            gameProperties[30].intPosition = 30;
            gameProperties[30].intPropertyType = 9;
            gameProperties[31].propertyName = "Pacific Ave.";
            gameProperties[31].intPosition = 31;
            gameProperties[31].intPropertyType = 0;
            gameProperties[31].propertyPrice = 300;
            gameProperties[31].mortgageValue = 150;
            gameProperties[31].rentNoHouse = 26;
            gameProperties[31].rentOneHouse = 130;
            gameProperties[31].rentTwoHouse = 390;
            gameProperties[31].rentThreeHouse = 900;
            gameProperties[31].rentFourHouse = 1100;
            gameProperties[31].rentHotel = 1275;
            gameProperties[31].buildCost = 200;
            gameProperties[31].colorGroup = GREEN;
            gameProperties[32].propertyName = "North Caroline Ave.";
            gameProperties[32].intPosition = 32;
            gameProperties[32].intPropertyType = 0;
            gameProperties[32].propertyPrice = 300;
            gameProperties[32].mortgageValue = 150;
            gameProperties[32].rentNoHouse = 26;
            gameProperties[32].rentOneHouse = 130;
            gameProperties[32].rentTwoHouse = 390;
            gameProperties[32].rentThreeHouse = 900;
            gameProperties[32].rentFourHouse = 1100;
            gameProperties[32].rentHotel = 1275;
            gameProperties[32].buildCost = 200;
            gameProperties[32].colorGroup = GREEN;
            gameProperties[33].propertyName = "Community Chest";
            gameProperties[33].intPosition = 33;
            gameProperties[33].intPropertyType = 4;
            gameProperties[34].propertyName = "Pennsylvania Ave.";
            gameProperties[34].intPosition = 34;
            gameProperties[34].intPropertyType = 0;
            gameProperties[34].propertyPrice = 320;
            gameProperties[34].mortgageValue = 160;
            gameProperties[34].rentNoHouse = 28;
            gameProperties[34].rentOneHouse = 150;
            gameProperties[34].rentTwoHouse = 450;
            gameProperties[34].rentThreeHouse = 1000;
            gameProperties[34].rentFourHouse = 1200;
            gameProperties[34].rentHotel = 1400;
            gameProperties[34].buildCost = 200;
            gameProperties[34].colorGroup = GREEN;
            gameProperties[35].propertyName = "Short Line";
            gameProperties[35].intPosition = 35;
            gameProperties[35].intPropertyType = 1;
            gameProperties[35].propertyPrice = 200;
            gameProperties[35].mortgageValue = 100;
            gameProperties[36].propertyName = "Chance";
            gameProperties[36].intPosition = 36;
            gameProperties[36].intPropertyType = 3;
            gameProperties[37].propertyName = "Park Place";
            gameProperties[37].intPosition = 37;
            gameProperties[37].intPropertyType = 0;
            gameProperties[37].propertyPrice = 350;
            gameProperties[37].mortgageValue = 175;
            gameProperties[37].rentNoHouse = 35;
            gameProperties[37].rentOneHouse = 175;
            gameProperties[37].rentTwoHouse = 500;
            gameProperties[37].rentThreeHouse = 1100;
            gameProperties[37].rentFourHouse = 1300;
            gameProperties[37].rentHotel = 1500;
            gameProperties[37].buildCost = 200;
            gameProperties[37].colorGroup = BLUE;
            gameProperties[38].propertyName = "Luxury Tax";
            gameProperties[38].intPosition = 38;
            gameProperties[38].intPropertyType = 10;
            gameProperties[39].propertyName = "Boardwalk";
            gameProperties[39].intPosition = 39;
            gameProperties[39].intPropertyType = 0;
            gameProperties[39].propertyPrice = 400;
            gameProperties[39].mortgageValue = 200;
            gameProperties[39].rentNoHouse = 50;
            gameProperties[39].rentOneHouse = 200;
            gameProperties[39].rentTwoHouse = 600;
            gameProperties[39].rentThreeHouse = 1400;
            gameProperties[39].rentFourHouse = 1700;
            gameProperties[39].rentHotel = 2000;
            gameProperties[39].buildCost = 200;
            gameProperties[39].colorGroup = BLUE;



        /*  gameProperties[0] = {"Go", 0, 5};
            gameProperties[1] = {"Mediterranean Avenue", 1, 0, 30, 60, 2, 10, 30, 90, 160, 250, 50, PURPLE};
            gameProperties[2] = {"Community Chest", 2, 4};
            gameProperties[3] = {"Baltic Ave.", 3, 0, 30, 60, 4, 20, 60, 180, 320, 450, 50, PURPLE};
            gameProperties[4] = {"Income Tax", 4, 6};
            gameProperties[5] = {"Reading Railroad", 5, 1, 200, 100};
            gameProperties[6] = {"Oriental Ave.", 6, 0, 50, 100, 6, 30, 90, 270, 400, 550, 50, SKY};
            gameProperties[7] = {"Chance", 7, 3};
            gameProperties[8] = {"Vermont Ave.", 8, 0, 50, 100, 6, 30, 90, 270, 400, 550, 50, SKY};
            gameProperties[9] = {"Connecticut Ave.", 9, 0, 60, 120, 8, 40, 100, 300, 450, 600, 50, SKY};
            gameProperties[10]= {"Jail", 10, 7};
            gameProperties[11] = {"St. Charles Place", 11, 0, 70, 140, 10, 50, 150, 450, 625, 750, 100, PINK};
            gameProperties[12] = {"Electric Company", 12, 2, 75, 150};
            gameProperties[13] = {"States Ave.", 13, 0, 70, 140, 10, 50, 150, 450, 625, 750, 100, PINK};
            gameProperties[14] = {"Virginia Ave.", 14, 0, 80, 160, 12, 60, 180, 500, 700, 900, 100, PINK};
            gameProperties[15] = {"Pennsylvania Railroad", 15, 1, 100, 200};
            gameProperties[16] = {"St. James Place", 16, 0, 90, 180, 14, 70, 200, 550, 750, 950, 100, ORANGE};
            gameProperties[17] = {"Community Chest", 17, 4};
            gameProperties[18] = {"Tennessee Ave.", 18, 0, 90, 180, 14, 70, 200, 550, 750, 950, 100, ORANGE};
            gameProperties[19] = {"New York Ave.", 19, 0, 100, 200, 16, 80, 220, 600, 800, 1000, 100, ORANGE};
            gameProperties[20] = {"Free Parking", 20, 8};
            gameProperties[21] = {"Kentucky Ave.", 21, 0, 110, 220, 18, 90, 250, 700, 875, 1050, 150, RED};
            gameProperties[22] = {"Chance", 22, 10, 3};
            gameProperties[23] = {"Indiana Ave.", 23, 0, 110, 220, 18, 90, 250, 700, 875, 1050, 150, RED};
            gameProperties[24] = {"Illinois Ave.", 24, 0, 120, 240, 20, 100, 300, 750, 925, 1100, 150, RED};
            gameProperties[25] = {"B.&O. Railroad", 25, 1, 100, 200};
            gameProperties[26] = {"Atlantic Ave.", 26, 0, 130, 260, 22, 110, 330, 800, 975, 1150, 150, YELLOW};
            gameProperties[27] = {"Ventnor Ave.", 27, 0, 130, 260, 22, 110, 330, 800, 975, 1150, 150, YELLOW};
            gameProperties[28] = {"Water Works", 28, 2, 75, 150};
            gameProperties[29] = {"Marvin Gardens", 29, 0, 140, 280, 24, 120, 360, 850, 1025, 1200, 150, YELLOW};
            gameProperties[30] = {"Go To Jail", 30, 9};
            gameProperties[31] = {"Pacific Ave.", 31, 0, 150, 300, 26, 130, 390, 900, 1100, 1275, 200, GREEN };
            gameProperties[32] = {"North Carolina Ave.", 32, 0, 150, 300, 26, 130, 390, 900, 1100, 1275, 200, GREEN};
            gameProperties[33] = {"Community Chest", 33, 4};
            gameProperties[34] = {"Pennsylvania Ave.", 34, 0, 160, 320, 28, 150, 450, 1000, 1200, 1400, 200, GREEN};
            gameProperties[35] = {"Short Line", 35, 1, 100, 200};
            gameProperties[36] = {"Chance", 36, 3};
            gameProperties[37] = {"Park Place", 37, 0, 175, 350, 35, 175, 500, 1100, 1300, 1500, 200, BLUE};
            gameProperties[38] = {"Luxury Tax", 38, 10};
            gameProperties[39] = {"Boardwalk", 39, 0, 200, 400, 50, 200, 600, 1400, 1700, 2000, 200, BLUE}; */

            numProperties = 40;
        }

        Property getPropInfo(int intPosition){
            return gameProperties[intPosition];
        }

        Property* getPropAdd(int intPosition){
            return (&gameProperties[intPosition]);
        }

        int getNumProperties(){
            return numProperties;
        }

        void setOwnedProp(int intPosition, bool newOwnedProperty){
            gameProperties[intPosition].ownedProperty = newOwnedProperty;
        }

        void setOwner(int intPosition, Player *newOwner){
            gameProperties[intPosition].propertyOwner = newOwner;
        }

        void setOwnAllColor(int intPosition, bool newOwnAllColor){
            gameProperties[intPosition].ownAllColor = newOwnAllColor;
        }

        void setNumHouses(int intPosition, int newNumHouses){
            gameProperties[intPosition].numHouses = newNumHouses;
        }

        void setInMortgage(int intPosition, bool newInMortgage){
            gameProperties[intPosition].inMortgage = newInMortgage;
        }


        void drawBoard(){
            cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|" << endl;
            cout << "|  Free   |Kentucky | Chance  | Indiana |Illinois | B. & O. |Atlantic | Ventnor |  Water  |  Marvin |  Go To  |" << endl;
            cout << "| Parking | Avenue  |    ?    |  Avenue | Avenue  |Railroad | Avenue  |  Avenue |  Works  | Gardens |  Jail   |" << endl;
            cout << "| Jackpot:|         |         |         |         |         |         |         |         |         |         |" << endl;
            cout << "|  $" << setw(4) << parkSpaceJackpot <<  "  |   RED   |         |   RED   |   RED   |         |  YELLOW |  YELLOW | UTILITY | YELLOW  |         |" << endl;
            cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|" << endl;
            cout << "| New York|                                                                                         | Pacific |" << endl;
            cout << "| Avenue  |                                                                                         |  Avenue |" << endl;
            cout << "|         |                                                                                         |         |" << endl;
            cout << "|  ORANGE |                                                                                         |  GREEN  |" << endl;
            cout << "|---------|                                                                                         |---------|" << endl;
            cout << "|Tennessee|                                                                                         |  North  |" << endl;
            cout << "|  Avenue |                                                                                         |Carolina |" << endl;
            cout << "|         |                                                                                         |  Avenue |" << endl;
            cout << "|  ORANGE |                                                                                         |  GREEN  |" << endl;
            cout << "|---------|                                                                                         |---------|" << endl;
            cout << "|Community|                                                                                         |Community|" << endl;
            cout << "|  Chest  |                                                                                         |  Chest  |" << endl;
            cout << "|         |                                                                                         |         |" << endl;
            cout << "|         |                                                                                         |         |" << endl;
            cout << "|---------|                                                                                         |---------|" << endl;
            cout << "|St.James |                                   Monopoly                                              |Pennsylva|" << endl;
            cout << "| Place   |                                                                                         |  -nia   |" << endl;
            cout << "|         |                                                                                         |  Avenue |" << endl;
            cout << "| ORANGE  |                                                                                         |  GREEN  |" << endl;
            cout << "|---------|                                                                                         |---------|" << endl;
            cout << "|Pennsylva|                                                                                         |ShortLine|" << endl;
            cout << "| -nia    |                                                                                         | Railroad|" << endl;
            cout << "|Railroad |                                                                                         |         |" << endl;
            cout << "|         |                                                                                         |         |" << endl;
            cout << "|---------|                                                                                         |---------|" << endl;
            cout << "|Virginia |                                                                                         |  Chance |" << endl;
            cout << "| Avenue  |                                                                                         |    ?    |" << endl;
            cout << "|         |                                                                                         |         |" << endl;
            cout << "|   PINK  |                                                                                         |         |" << endl;
            cout << "|---------|                                                                                         |---------|" << endl;
            cout << "|  States |                                                                                         |  Park   |" << endl;
            cout << "|  Avenue |                                                                                         |  Place  |" << endl;
            cout << "|         |                                                                                         |         |" << endl;
            cout << "|   PINK  |                                                                                         |   BLUE  |" << endl;
            cout << "|---------|                                                                                         |---------|" << endl;
            cout << "|Electric |                                                                                         |  Luxury |" << endl;
            cout << "| Company |                                                                                         |   Tax   |" << endl;
            cout << "|         |                                                                                         |         |" << endl;
            cout << "| UTILITY |                                                                                         |         |" << endl;
            cout << "|---------| Current Turn: " << setw(60) << left << gamePlayers[turnNum].playerName <<         "              |---------|" << endl;
            cout << "|   St.   |       Wealth: $" << setw(59) << left << gamePlayers[turnNum].intWealth <<         "              |Boardwalk|" << endl;
            cout << "| Charles |                                                                                         |         |" << endl;
            cout << "|  Place  |                                                                                         |         |" << endl;
            cout << "|   PINK  |                                                                                         |   BLUE  |" << endl;
            cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|" << endl;
            cout << "|  Jail   |Connectic| Vermont |  Chance | Oriental| Reading | Income  | Baltic  |Community|Mediterra|   Go    |" << endl;
            cout << "|         |   -ut   |  Avenue |    ?    |  Avenue | Railroad|  Tax    | Avenue  |  Chest  |  -nean  |         |" << endl;
            cout << "|         |  Avenue |         |         |         |         |         |         |         | Avenue  | Collect |" << endl;
            cout << "|         |   SKY   |   SKY   |         |   SKY   |         |         |  PURPLE |         |  PURPLE |  $200   |" << endl;
            cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|\n" << endl;




        }
};


int takeTurn(GameBoard *inputBoard, Player *inputPlayer);
int chanceLand(GameBoard *inputBoard, Player *inputPlayer);
int comChestLand(GameBoard *inputBoard, Player *inputPlayer);
int playerInJail(GameBoard *inputBoard, Player *inputPlayer);
void calculateOwnedProps(GameBoard *inputBoard);
void displayOwnedProps();
void displayPlayerStats(GameBoard *inputBoard, Player *inputPlayer);
int buildHouse(GameBoard *inputBoard);
int sellHouse(GameBoard *inputBoard);
int offerTrade(GameBoard *inputBoard);
int tradeWealth(GameBoard *inputBoard, Player *inputPlayer);
int mortgageProp(GameBoard *inputBoard, Player *inputPlayer);
int unmortgageProp(GameBoard *inputBoard, Player *inputPlayer);
int avoidBankruptcy(GameBoard *inputBoard, Player *inputPlayer);
int bankRuptcyProcedure(GameBoard *inputBoard, Player *inputPlayer);
int offerProperties(GameBoard *inputBoard, int whichPlayer, vector<Property*> *inputOfferProp);
int askForProperties(GameBoard *inputBoard, int whichPlayer, vector<Property*> *inputAskProp);
int auctionProperty(GameBoard *inputBoard, Property *inputProperty);
int rollDie();
bool rollDoubles();




vector< vector<Property *> > ownedProperties;


int main()
{
    GameBoard *monopolyBoard = new GameBoard();

    cout << "\nWelcome to Mark's C++ Monopoly Extravaganza!" << endl;
    cout << "\n How many players? ";
    cin >> numPlayers;


    while((numPlayers < 2) || (numPlayers > 8)){
        if(numPlayers < 2){
            cout << "\nThere must be at least 2 players. Please input how many players there are: ";
            cin >> numPlayers;
        }
        if(numPlayers > 8){
            cout << "\nThere can only be 8 players at once. Please input how many players there are: ";
            cin >> numPlayers;
        }

    }

    cin.ignore();
    for(int i = 0; i < numPlayers; i++){
        cout << "\nWhat is Player " << (i + 1) << "'s name? ";
        getline(cin, gamePlayers[i].playerName); // Collects player's name

        vector<Property *> playerProperties; // Creates a vector of property pointers to put into the ownedProperties 2D vector
        ownedProperties.push_back(playerProperties); // Creates a new row in the 2D vector ownedProperties for each player2
        gamePlayers[i].playerNum = i;
    }

    clearscreen();
    cout << "Welcome to Monopoly!\n" << endl;
    (*monopolyBoard).drawBoard();

    bool continuePlaying = true;
    while(continuePlaying) {

        takeTurn(monopolyBoard, &gamePlayers[turnNum]);


        int numRemainPlayers = numPlayers;
        Player *winningPlayer;

        /* This loop checks to see if all but one player is bankrupt, and if so, declares that one player the winner */
        for(int i = 0; i < numPlayers; i++){
            if(gamePlayers[i].boolBankrupt){
                numRemainPlayers--;
            }
            else{
                winningPlayer = &gamePlayers[i];
            }
        }
        /* If there is a winner */
        if(numRemainPlayers == 1){
            cout << "\nWe have a winner! Congratulations " << (*winningPlayer).playerName << "!" << endl;
            continuePlaying = false;
        }
        if(numDoubles == 0){
            if(turnNum + 1 == numPlayers){
                turnNum = 0;
            }
            else{
                    turnNum++;
            }
            while(gamePlayers[turnNum].boolBankrupt){
                if(turnNum + 1 == numPlayers){
                    turnNum = 0;
                }
                else{
                    turnNum++;
                }
            }
        }
        else if(numDoubles == 3){
            cout << "\nYou rolled doubles three times! Go immediately to jail, do not collect go, do not collect $200!" << endl;
            gamePlayers[turnNum].inJail = true;
            gamePlayers[turnNum].intPosition = 10;
            if(turnNum + 1 == numPlayers){
                turnNum = 0;
            }
            else{
                    turnNum++;
            }
            while(gamePlayers[turnNum].boolBankrupt){
                if(turnNum + 1 == numPlayers){
                    turnNum = 0;
                }
                else{
                    turnNum++;
                }
            }
            numDoubles = 0;
        }
        else{
            cout << "\nSince you rolled doubles, you get to take another turn!" << endl;
        }
    }
        return 0;
}

int takeTurn(GameBoard *inputBoard, Player *inputPlayer){

    int diceRoll = -1;
    char chosenOption = 'z';
    int continueTurn = -1;


    //(*inputBoard).drawBoard();
    calculateOwnedProps(inputBoard);

    while(continueTurn == -1){
        displayOwnedProps();

        cout << "It is " << (*inputPlayer).playerName << "'s turn!" << endl;
        displayPlayerStats(inputBoard, inputPlayer); // Display the player's stats
        cout << "What would you like to do?" << "\n     a) Start turn" << "\n     b) Build a house" << "\n     c) Sell a house" << "\n     d) Offer Trade" << "\n     e) Lift a Mortgage";


        while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'c' && chosenOption != 'd' &&  chosenOption != 'e' && chosenOption != 'A'&& chosenOption != 'B' && chosenOption != 'C' && chosenOption != 'D' && chosenOption != 'E') {
            cout << "\n\n Please enter a, b, c, d, or e: ";
            cin >> chosenOption;
        }

        if(chosenOption == 'a' || chosenOption == 'A'){
            continueTurn = 1;
            chosenOption = 'z';
        }

        if(chosenOption == 'b' || chosenOption == 'B'){
            continueTurn = buildHouse(inputBoard);
            chosenOption = 'z';

        }

        if(chosenOption == 'c' || chosenOption == 'C'){
            continueTurn = sellHouse(inputBoard);
            chosenOption = 'z';

        }

         if(chosenOption == 'd' || chosenOption == 'D'){
            continueTurn = offerTrade(inputBoard);
            chosenOption = 'z';

        }

        if(chosenOption == 'e' || chosenOption == 'E'){
            continueTurn = unmortgageProp(inputBoard, inputPlayer);
            chosenOption = 'z';
        }

    }

    chosenOption = '-1';
    clearscreen();
    (*inputBoard).drawBoard();
    displayOwnedProps();

    if((*inputPlayer).inJail){ // If the player is in jail
       diceRoll = playerInJail(inputBoard, inputPlayer);

       if(gamePlayers[turnNum].boolBankrupt){
            return -1;
       }
       else if(diceRoll == -2){ // If the player didn't leave jail, their turn is over
            numDoubles = 0;
            clearscreen();
            (*inputBoard).drawBoard();
            return -1;

       }
       (*inputPlayer).numTurnInJail = 0;

    }

    if(!(*inputPlayer).inJail && diceRoll == -1){ // If the player isn't in jail or needs to roll for the turn after getting out of jail
        diceRoll = rollDie();
        if(rollDoubles() && diceRoll % 2 == 0){
            numDoubles++;
            cout << "\nYou rolled doubles!" << endl;
        }
        else{
            numDoubles = 0;
        }

    }

    cout << "\nYou rolled: " << diceRoll << "!" << endl;

    if((*inputPlayer).intPosition + rollDie() > 39){ // If the player will pass go in the turn, they will collect $200
        (*inputPlayer).intPosition = (((*inputPlayer).intPosition + diceRoll) - 39);
        (*inputPlayer).intWealth += 200;
        cout << "\nYou passed Go, collect $200!" << endl;
    }
    else{
        (*inputPlayer).intPosition += diceRoll;
    }

    displayPlayerStats(inputBoard, inputPlayer);

    /* This switch statement takes different action depending on the property that the player lands on */
    switch((*inputBoard).getPropInfo((*inputPlayer).intPosition).intPropertyType){
        /* If the property is a standard property */
        case 0: if((*inputBoard).getPropInfo((*inputPlayer).intPosition).ownedProperty){ // If the property is owned
                    if((*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                       clearscreen();
                       (*inputBoard).drawBoard();
                       cout << "\nYou own " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << ", kick back and relax!\m" << endl;
                        return -1;
                    }
                    else{ // If another player owns the property, the player will be charged the proper rent
                        cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).playerName)
                            <<  "! Rent is $";
                            switch((*inputBoard).getPropInfo((*inputPlayer).intPosition).numHouses){
                                case 0: if((*inputBoard).getPropInfo((*inputPlayer).intPosition).ownAllColor){ // No houses but all of the color is owned
                                            cout << (2 * (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentNoHouse) << endl;
                                            while((2 * (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentNoHouse) > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (2 * (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentNoHouse);
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += (2 * (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentNoHouse);
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                                        }
                                        else{
                                            cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentNoHouse << endl; // No houses and the whole colorset isn't owned
                                            while((*inputBoard).getPropInfo((*inputPlayer).intPosition).rentNoHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                               }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentNoHouse;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentNoHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                                        }

                                case 1: cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentOneHouse << endl; // If there is one house

                                            while((*inputBoard).getPropInfo((*inputPlayer).intPosition).rentOneHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentOneHouse;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentOneHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 2:     cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentTwoHouse << endl; // If there is two houses
                                            while((*inputBoard).getPropInfo((*inputPlayer).intPosition).rentTwoHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentTwoHouse;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentTwoHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 3:     cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentThreeHouse << endl; // If there is three houses
                                            while((*inputBoard).getPropInfo((*inputPlayer).intPosition).rentThreeHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentThreeHouse;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentThreeHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 4:     cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentFourHouse << endl; // If there are four houses
                                            while((*inputBoard).getPropInfo((*inputPlayer).intPosition).rentFourHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentFourHouse;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentFourHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 5:     cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentHotel << endl; // If there is a hotel/5 houses
                                            while((*inputBoard).getPropInfo((*inputPlayer).intPosition).rentHotel > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentHotel;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += (*inputBoard).getPropInfo((*inputPlayer).intPosition).rentHotel;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                            }
                    }
                }
                else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                    char chosenAvailPropOption = '-1';
                    cout << "\nThis property is available. Would you like to buy?" << endl;
                    cout << "     a) Yes" << endl;
                    cout << "     b) No" << endl;

                    while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                        cout << "\nPlease choose a or b: ";
                        cin >> chosenAvailPropOption;
                    }

                    if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                        if((*inputPlayer).intWealth < (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyPrice){ // If the player can't afford to buy the property
                            cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                            chosenAvailPropOption = 'b';
                        }
                        else{ // If the player can afford to buy the property, they will be able to
                            clearscreen();
                            (*inputBoard).drawBoard();
                            cout << "\n" << (*inputPlayer).playerName << " successfully bought " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << "!" << endl;
                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyPrice;
                            (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                            (*inputBoard).setOwner((*inputPlayer).intPosition, inputPlayer);
                            return -1;
                        }
                    }
                    if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                        cout << "\nThe property will be auctioned." << endl;
                        auctionProperty(inputBoard, (*inputBoard).getPropAdd((*inputPlayer).intPosition));
                        return -1;
                    }
                }


        /* If the property is a railroad */
        case 1: if((*inputBoard).getPropInfo((*inputPlayer).intPosition).ownedProperty){ // If the property is owned
                    if((*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                        clearscreen();
                       (*inputBoard).drawBoard();
                        cout << "\nYou own " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << ", kick back and relax!\n" << endl;
                        return -1;
                    }
                    else{ // If another player owns the property, the player will be charged the proper rent
                        cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).playerName)
                            <<  "! Rent is $";
                             switch((*inputBoard).getPropInfo((*inputPlayer).intPosition).numHouses){
                                case 1: cout << 25 << endl; // If the owner owns one railroad

                                            while(25 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= 25;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 25;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 2:     cout << 50 << endl; // If the owner owns two railroads
                                            while(50 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= 50;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 50;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 3:     cout << 100 << endl; // If the owner owns three railroads
                                            while(100 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= 100;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 100;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 4:     cout << 200 << endl; // If the owner owns four railroads
                                            while(200 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= 200;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 200;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                             }
                    }
                }
                else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                    char chosenAvailPropOption = '-1';
                    cout << "\nThis property is available. Would you like to buy?" << endl;
                    cout << "     a) Yes" << endl;
                    cout << "     b) No" << endl;

                    while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                        cout << "\nPlease choose a or b: ";
                        cin >> chosenAvailPropOption;
                    }

                    if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                        if((*inputPlayer).intWealth < (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyPrice){ // If the player can't afford to buy the property
                            cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                            chosenAvailPropOption = 'b';
                        }
                        else{ // If the player can afford to buy the property, they will be able to
                            clearscreen();
                            (*inputBoard).drawBoard();
                            cout << "\n" << (*inputPlayer).playerName << " successfully bought " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << "!" << endl;
                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyPrice;
                            (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                            (*inputBoard).setOwner((*inputPlayer).intPosition, inputPlayer);
                            return -1;
                        }
                    }
                    if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                        cout << "\nThe property will be auctioned." << endl;
                        auctionProperty(inputBoard, (*inputBoard).getPropAdd((*inputPlayer).intPosition));
                        return -1;
                    }
                }

        /* If the property is a utility */
        case 2:if((*inputBoard).getPropInfo((*inputPlayer).intPosition).ownedProperty){ // If the property is owned
                    if((*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                        clearscreen();
                       (*inputBoard).drawBoard();
                        cout << "\nYou own " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << ", kick back and relax!\n" << endl;
                        return -1;
                    }
                    else{ // If another player owns the property, the player will be charged the proper rent
                        cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).playerName)
                            <<  "! Rent is $";
                             switch((*inputBoard).getPropInfo((*inputPlayer).intPosition).numHouses){
                                case 1: cout << (diceRoll * 4) << endl; // If the owner owns one utility

                                            while((diceRoll * 4) > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (diceRoll * 4);
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += (diceRoll * 4);
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 2:     cout << (diceRoll * 10) << endl; // If the owner owns both utilities
                                            while((diceRoll * 10) > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (diceRoll * 10);
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += (diceRoll * 10);
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                    }
                }
            }
            else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                    char chosenAvailPropOption = '-1';
                    cout << "\nThis property is available. Would you like to buy?" << endl;
                    cout << "     a) Yes" << endl;
                    cout << "     b) No" << endl;

                    while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                        cout << "\nPlease choose a or b: ";
                        cin >> chosenAvailPropOption;
                    }

                    if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                        if((*inputPlayer).intWealth < (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyPrice){ // If the player can't afford to buy the property
                            cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                            chosenAvailPropOption = 'b';
                        }
                        else{ // If the player can afford to buy the property, they will be able to
                            clearscreen();
                            (*inputBoard).drawBoard();
                            cout << "\nYou have successfully bought " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << "!" << endl;
                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyPrice;
                            (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                            (*inputBoard).setOwner((*inputPlayer).intPosition, inputPlayer);
                            return -1;
                        }
                    }
                    if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                        cout << "\nThe property will be auctioned." << endl;
                        auctionProperty(inputBoard, (*inputBoard).getPropAdd((*inputPlayer).intPosition));
                        return -1;
                    }
                }

        case 3: clearscreen();
                (*inputBoard).drawBoard();
                cout << "\nYou landed on Chance!" << endl;
                chanceLand(inputBoard, inputPlayer);
                return -1;

        case 4: clearscreen();
                (*inputBoard).drawBoard();
                cout << "\nYou landed on Community Chest!" << endl;
                comChestLand(inputBoard, inputPlayer);
                return -1;

        case 5: clearscreen();
                (*inputBoard).drawBoard();
                cout << "\n" << (*inputPlayer).playerName << " landed on Go! Collect $200!" << endl;
                (*inputPlayer).intWealth += 200;
                return -1;

        case 6: clearscreen();
                (*inputBoard).drawBoard();
                cout << "\n" << (*inputPlayer).playerName << " is required to pay income tax! It will be: $";
                if(((*inputPlayer).intWealth * .10) < 200){
                    cout << ((*inputPlayer).intWealth * .10) << "." << endl;
                    while((*inputPlayer).intWealth < ((*inputPlayer).intWealth * .10)){
                        int ifBankrupt = 1;
                        cout << "\nYou cannot afford that" << endl;
                        ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                        if(ifBankrupt == -1){
                            return -1;
                        }
                    }
                    (*inputPlayer).intWealth -= ((*inputPlayer).intWealth * .10);
                    parkSpaceJackpot += ((*inputPlayer).intWealth * .10);
                }
                else{
                    cout << "200." << endl;
                    while((*inputPlayer).intWealth < 200){
                        int ifBankrupt = 1;
                        cout << "\nYou cannot afford that" << endl;
                        ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                        if(ifBankrupt == -1){
                            return -1;
                        }
                    }
                    (*inputPlayer).intWealth -= 200;
                    parkSpaceJackpot += 200;
                }
                return -1;

        case 7: clearscreen();
                (*inputBoard).drawBoard();
                cout << "\nYou landed on jail. Just visiting!" << endl;
                return -1;

        case 8: clearscreen();
                (*inputBoard).drawBoard();
                cout << "\nYou landed on Free Parking! You won $" << parkSpaceJackpot << "!" << endl;
                (*inputPlayer).intWealth += parkSpaceJackpot;
                parkSpaceJackpot = 0;
                return -1;

        case 9: clearscreen();
                (*inputBoard).drawBoard();
                cout << "\n" << (*inputPlayer).playerName << " has been found guilty of jaywalking. Go straight to Alkatraz. Do not pass Go. Do not collect $200." << endl;
                (*inputPlayer).intPosition = 10;
                (*inputPlayer).inJail = true;
                return -1;

        case 10: clearscreen();
                (*inputBoard).drawBoard();
                cout << "\n" << (*inputPlayer).playerName <<" is required to pay a Luxury Tax! Pay $75." << endl;
                 while((*inputPlayer).intWealth < 75){
                        int ifBankrupt = 1;
                        cout << "\nYou cannot afford that" << endl;
                        ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                        if(ifBankrupt == -1){
                            return -1;
                        }
                    }
                    (*inputPlayer).intWealth -= 75;
                    parkSpaceJackpot += 75;
                    return -1;

        }


    }

int chanceLand(GameBoard *inputBoard, Player *inputPlayer){
    int whichChanceCard = -1;

    srand(time(0));
    whichChanceCard = ((rand() % 16) + 1);

    switch(whichChanceCard){
        case 1: cout << "\nChance: Get out of jail free! This card may be kept until needed or sold." << endl;
                (*inputPlayer).outJailFree = true;
                return -1;

        case 2:
        case 3: cout << "\nChance: Advance to the nearest Railroad and pay the owner Twice the Rental to which he is otherwise entitled." << endl;
                cout << "If Railroad is unowned, you may buy it from the Bank." << endl;
                if((*inputPlayer).intPosition < 5){
                    (*inputPlayer).intPosition = 5;
                    cout << "\nYou landed on " << (*inputBoard).getPropInfo(5).propertyName << "!" << endl;
                }
                else if ((*inputPlayer).intPosition < 15 && (*inputPlayer).intPosition > 5){
                    (*inputPlayer).intPosition = 15;
                    cout << "\nYou landed on " << (*inputBoard).getPropInfo(15).propertyName << "!" << endl;
                }
                else if ((*inputPlayer).intPosition < 25 && (*inputPlayer).intPosition > 15){
                    (*inputPlayer).intPosition = 25;
                    cout << "\nYou landed on " << (*inputBoard).getPropInfo(25).propertyName << "!" << endl;
                }
                else if ((*inputPlayer).intPosition < 35 && (*inputPlayer).intPosition > 25){
                    (*inputPlayer).intPosition = 35;
                    cout << "\nYou landed on " << (*inputBoard).getPropInfo(35).propertyName << "!" << endl;
                }
                else if((*inputPlayer).intPosition > 35){
                    (*inputPlayer).intPosition = 5;
                    cout << "\nYou passed Go! Collect $200!" << endl;
                    (*inputPlayer).intWealth += 200;
                    cout << "You landed on " << (*inputBoard).getPropInfo(5).propertyName << "!" << endl;
                }

                if((*inputBoard).getPropInfo((*inputPlayer).intPosition).ownedProperty){ // If the property is owned
                    if((*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                        clearscreen();
                       (*inputBoard).drawBoard();
                        cout << "\nYou own " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << ", kick back and relax!\n" << endl;
                        return -1;
                    }
                    else{ // If another player owns the property, the player will be charged the proper rent
                        cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).playerName)
                            <<  "! Rent is $";
                             switch((*inputBoard).getPropInfo((*inputPlayer).intPosition).numHouses){
                                case 1: cout << 50 << endl; // If the owner owns one railroad

                                            while(50 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= 50;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 50;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 2:     cout << 100 << endl; // If the owner owns two railroads
                                            while(100 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= 100;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 100;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 3:     cout << 200 << endl; // If the owner owns three railroads
                                            while(200 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= 200;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 200;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 4:     cout << 400 << endl; // If the owner owns four railroads
                                            while(400 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= 400;
                                            (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 400;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                             }
                    }
                }
                else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                    char chosenAvailPropOption = '-1';
                    cout << "\nThis property is available. Would you like to buy?" << endl;
                    cout << "     a) Yes" << endl;
                    cout << "     b) No" << endl;

                    while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                        cout << "\nPlease choose a or b: ";
                        cin >> chosenAvailPropOption;
                    }

                    if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                        if((*inputPlayer).intWealth < (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyPrice){ // If the player can't afford to buy the property
                            cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                            chosenAvailPropOption = 'b';
                        }
                        else{ // If the player can afford to buy the property, they will be able to
                            clearscreen();
                            (*inputBoard).drawBoard();
                            cout << "\nYou have successfully bought " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << "!" << endl;
                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyPrice;
                            (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                            (*inputBoard).setOwner((*inputPlayer).intPosition, inputPlayer);
                            return -1;
                        }
                    }
                    if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                        cout << "\nThe property will be auctioned." << endl;
                        auctionProperty(inputBoard, (*inputBoard).getPropAdd((*inputPlayer).intPosition));
                        return -1;
                    }
                }

        case 4: {cout << "\nChance: Make General Repairs On All Your Property." << endl;
                cout << "          FOR EACH HOUSE PAY $25.";
                cout << "          FOR EACH HOTEL $100." << endl;

                int numHouses = 0;
                int numHotels = 0;
                int totalCost = 0;

                /* This loop counts the total number of houses and hotels the player has */
                for(int i = 0; i < ownedProperties.at((*inputPlayer).playerNum).size(); i++){
                    if((*ownedProperties.at((*inputPlayer).playerNum).at(i)).numHouses > 0 && (*ownedProperties.at((*inputPlayer).playerNum).at(i)).intPropertyType == 0){
                        if((*ownedProperties.at((*inputPlayer).playerNum).at(i)).numHouses == 5){ // 5 houses == 1 hotel
                                numHotels++;
                        }
                        else{
                            numHouses += (*ownedProperties.at((*inputPlayer).playerNum).at(i)).numHouses;
                        }
                    }
                }

                totalCost = ((numHouses * 25) + (numHotels * 100)); // This totals up the bill, depending on how many houses and hotels the player has

                cout << "\nYou must pay $" << totalCost << "." << endl;

                while(totalCost > (*inputPlayer).intWealth){ // If the player cant afford to pay
                    int ifBankrupt = 1;
                    cout << "\nYou can't afford that!" << endl;
                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                    if(ifBankrupt == -1){
                        return -1;
                    }
                }
                parkSpaceJackpot += totalCost;
                (*inputPlayer).intWealth -= totalCost;}
                return -1;

        case 5: {cout << "\nChance: Your Building and Loan Matures. Collect $150!" << endl;
                (*inputPlayer).intWealth += 150;}
                return -1;

        case 6: cout << "\nChance: Advance to Illinois Ave." << endl;
                if((*inputPlayer).intPosition > 24){ // If the player passes Go on the way to Illinois Ave.
                    cout << "\nYou passed Go, collect $200!" << endl;
                    (*inputPlayer).intWealth += 200;
                }
                (*inputPlayer).intPosition = 24;

                if((*inputBoard).getPropInfo(24).ownedProperty){ // If the property is owned
                    if((*inputBoard).getPropInfo(24).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                       clearscreen();
                       (*inputBoard).drawBoard();
                       cout << "\nYou own " << (*inputBoard).getPropInfo(24).propertyName << ", kick back and relax!\n" << endl;
                        return -1;
                    }
                    else{ // If another player owns the property, the player will be charged the proper rent
                        cout << (*inputBoard).getPropInfo(24).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo(24).propertyOwner).playerName)
                            <<  "! Rent is $";
                            switch((*inputBoard).getPropInfo(24).numHouses){
                                case 0: if((*inputBoard).getPropInfo(24).ownAllColor){ // No houses but all of the color is owned
                                            cout << (2 * (*inputBoard).getPropInfo(24).rentNoHouse) << endl;
                                            while((2 * (*inputBoard).getPropInfo(24).rentNoHouse) > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (2 * (*inputBoard).getPropInfo(24).rentNoHouse);
                                            (*(*inputBoard).getPropInfo(24).propertyOwner).intWealth += (2 * (*inputBoard).getPropInfo(24).rentNoHouse);
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                                        }
                                        else{
                                            cout << (*inputBoard).getPropInfo(24).rentNoHouse << endl; // No houses and the whole colorset isn't owned
                                            while((*inputBoard).getPropInfo(24).rentNoHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                               }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(24).rentNoHouse;
                                            (*(*inputBoard).getPropInfo(24).propertyOwner).intWealth += (*inputBoard).getPropInfo(24).rentNoHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                                        }

                                case 1: cout << (*inputBoard).getPropInfo(24).rentOneHouse << endl; // If there is one house

                                            while((*inputBoard).getPropInfo(24).rentOneHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(24).rentOneHouse;
                                            (*(*inputBoard).getPropInfo(24).propertyOwner).intWealth += (*inputBoard).getPropInfo(24).rentOneHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 2:     cout << (*inputBoard).getPropInfo(24).rentTwoHouse << endl; // If there is two houses
                                            while((*inputBoard).getPropInfo(24).rentTwoHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(24).rentTwoHouse;
                                            (*(*inputBoard).getPropInfo(24).propertyOwner).intWealth += (*inputBoard).getPropInfo(24).rentTwoHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 3:     cout << (*inputBoard).getPropInfo(24).rentThreeHouse << endl; // If there is three houses
                                            while((*inputBoard).getPropInfo(24).rentThreeHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(24).rentThreeHouse;
                                            (*(*inputBoard).getPropInfo(24).propertyOwner).intWealth += (*inputBoard).getPropInfo(24).rentThreeHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 4:     cout << (*inputBoard).getPropInfo(24).rentFourHouse << endl; // If there are four houses
                                            while((*inputBoard).getPropInfo(24).rentFourHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(24).rentFourHouse;
                                            (*(*inputBoard).getPropInfo(24).propertyOwner).intWealth += (*inputBoard).getPropInfo(24).rentFourHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 5:     cout << (*inputBoard).getPropInfo(24).rentHotel << endl; // If there is a hotel/5 houses
                                            while((*inputBoard).getPropInfo(24).rentHotel > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(24).rentHotel;
                                            (*(*inputBoard).getPropInfo(24).propertyOwner).intWealth += (*inputBoard).getPropInfo(24).rentHotel;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                            }
                    }
                }
                else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                    char chosenAvailPropOption = '-1';
                    cout << "\nThis property is available. Would you like to buy?" << endl;
                    cout << "     a) Yes" << endl;
                    cout << "     b) No" << endl;

                    while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                        cout << "\nPlease choose a or b: ";
                        cin >> chosenAvailPropOption;
                    }

                    if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                        if((*inputPlayer).intWealth < (*inputBoard).getPropInfo(24).propertyPrice){ // If the player can't afford to buy the property
                            cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                            chosenAvailPropOption = 'b';
                        }
                        else{ // If the player can afford to buy the property, they will be able to
                            clearscreen();
                            (*inputBoard).drawBoard();
                            cout << "\nYou have successfully bought " << (*inputBoard).getPropInfo(24).propertyName << "!" << endl;
                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(24).propertyPrice;
                            (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                            (*inputBoard).setOwner(24, inputPlayer);
                            return -1;
                        }
                    }
                    if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                        cout << "\nThe property will be auctioned." << endl;
                        auctionProperty(inputBoard, (*inputBoard).getPropAdd(24));
                        return -1;
                    }
                }

        case 7: cout << "\nChance: Pay Poor Tax of $15." << endl;
                while((*inputPlayer).intWealth < 15){
                    int ifBankrupt = 1;
                    cout << "\nYou can't afford to pay $15!" << endl;
                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                    if(ifBankrupt == -1){
                        return -1;
                    }
                }

                parkSpaceJackpot += 15;
                (*inputPlayer).intWealth -= 15;
                return -1;

        case 8: cout << "\nChance: Take a Walk on the Board Walk. Advance Token to Board Walk." << endl;
                (*inputPlayer).intPosition = 39;

                if((*inputBoard).getPropInfo(39).ownedProperty){ // If the property is owned
                    if((*inputBoard).getPropInfo(39).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                       clearscreen();
                       (*inputBoard).drawBoard();
                       cout << "\nYou own " << (*inputBoard).getPropInfo(39).propertyName << ", kick back and relax!\n" << endl;
                        return -1;
                    }
                    else{ // If another player owns the property, the player will be charged the proper rent
                        cout << (*inputBoard).getPropInfo(39).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo(39).propertyOwner).playerName)
                            <<  "! Rent is $";
                            switch((*inputBoard).getPropInfo(39).numHouses){
                                case 0: if((*inputBoard).getPropInfo(39).ownAllColor  == true){ // No houses but all of the color is owned
                                            cout << (2 * (*inputBoard).getPropInfo(39).rentNoHouse) << endl;
                                            while((2 * (*inputBoard).getPropInfo(39).rentNoHouse) > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (2 * (*inputBoard).getPropInfo(39).rentNoHouse);
                                            (*(*inputBoard).getPropInfo(39).propertyOwner).intWealth += (2 * (*inputBoard).getPropInfo(39).rentNoHouse);
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                                        }
                                        else{
                                            cout << (*inputBoard).getPropInfo(39).rentNoHouse << endl; // No houses and the whole colorset isn't owned
                                            while((*inputBoard).getPropInfo(39).rentNoHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                               }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(39).rentNoHouse;
                                            (*(*inputBoard).getPropInfo(39).propertyOwner).intWealth += (*inputBoard).getPropInfo(39).rentNoHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                                        }

                                case 1: cout << (*inputBoard).getPropInfo(39).rentOneHouse << endl; // If there is one house

                                            while((*inputBoard).getPropInfo(39).rentOneHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(39).rentOneHouse;
                                            (*(*inputBoard).getPropInfo(39).propertyOwner).intWealth += (*inputBoard).getPropInfo(39).rentOneHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 2:     cout << (*inputBoard).getPropInfo(39).rentTwoHouse << endl; // If there is two houses
                                            while((*inputBoard).getPropInfo(39).rentTwoHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(39).rentTwoHouse;
                                            (*(*inputBoard).getPropInfo(39).propertyOwner).intWealth += (*inputBoard).getPropInfo(39).rentTwoHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 3:     cout << (*inputBoard).getPropInfo(39).rentThreeHouse << endl; // If there is three houses
                                            while((*inputBoard).getPropInfo(39).rentThreeHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(39).rentThreeHouse;
                                            (*(*inputBoard).getPropInfo(39).propertyOwner).intWealth += (*inputBoard).getPropInfo(39).rentThreeHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 4:     cout << (*inputBoard).getPropInfo(39).rentFourHouse << endl; // If there are four houses
                                            while((*inputBoard).getPropInfo(39).rentFourHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(39).rentFourHouse;
                                            (*(*inputBoard).getPropInfo(39).propertyOwner).intWealth += (*inputBoard).getPropInfo(39).rentFourHouse;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;

                                case 5:     cout << (*inputBoard).getPropInfo(39).rentHotel << endl; // If there is a hotel/5 houses
                                            while((*inputBoard).getPropInfo(39).rentHotel > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                int ifBankrupt = 1;
                                                cout << "\nYou cannot afford rent!" << endl;
                                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                if(ifBankrupt == -1){
                                                    return -1;
                                                }
                                            }

                                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(39).rentHotel;
                                            (*(*inputBoard).getPropInfo(39).propertyOwner).intWealth += (*inputBoard).getPropInfo(39).rentHotel;
                                            clearscreen();
                                            (*inputBoard).drawBoard();
                                            cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                            return -1;
                            }
                    }
                }
                else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                    char chosenAvailPropOption = '-1';
                    cout << "\nThis property is available. Would you like to buy?" << endl;
                    cout << "     a) Yes" << endl;
                    cout << "     b) No" << endl;

                    while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                        cout << "\nPlease choose a or b: ";
                        cin >> chosenAvailPropOption;
                    }

                    if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                        if((*inputPlayer).intWealth < (*inputBoard).getPropInfo(39).propertyPrice){ // If the player can't afford to buy the property
                            cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                            chosenAvailPropOption = 'b';
                        }
                        else{ // If the player can afford to buy the property, they will be able to
                            clearscreen();
                            (*inputBoard).drawBoard();
                            cout << "\nYou have successfully bought " << (*inputBoard).getPropInfo(39).propertyName << "!" << endl;
                            (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(39).propertyPrice;
                            (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                            (*inputBoard).setOwner(39, inputPlayer);
                            return -1;
                        }
                    }
                    if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                        cout << "\nThe property will be auctioned." << endl;
                        auctionProperty(inputBoard, (*inputBoard).getPropAdd(39));
                        return -1;
                    }
                }

        case 9: {cout << "\nChance: You Have Been Elected Chairman of the Board. Pay Each Player $50." << endl;
                int numRemainPlayers = 0;
                int totalCost = 0;

                for(int i = 0; i < numPlayers; i++){
                    if(!gamePlayers[i].boolBankrupt && gamePlayers[i].playerName != (*inputPlayer).playerName){ // bankrupt players and the current player is excluded from the count
                        numRemainPlayers++;
                    }
                }

                totalCost = (50 * numRemainPlayers);
                while((*inputPlayer).intWealth < totalCost){
                    int ifBankrupt = 1;
                    cout << "\nYou cannot afford to pay!" << endl;
                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                    if(ifBankrupt == -1){
                        return -1;
                    }
                }

                (*inputPlayer).intWealth -= totalCost;

                for(int i = 0; i < numPlayers; i++){
                     if(!gamePlayers[i].boolBankrupt && gamePlayers[i].playerName != (*inputPlayer).playerName){ // bankrupt players and the current player is excluded from the count
                        gamePlayers[i].intWealth += 50;
                    }
                }}
                return -1;

        case 10:    {cout << "\nChance: Advance to Go, collect $200!" << endl;
                    (*inputPlayer).intPosition = 0;
                    (*inputPlayer).intWealth += 200;}
                    return -1;

        case 11:    cout << "\nChance: Take a Ride on the Reading. If you pass Go collect $200." << endl;
                    if((*inputPlayer).intPosition > 5){
                        cout << "You passed Go!" << endl;
                        (*inputPlayer).intWealth += 200;
                    }

                    (*inputPlayer).intPosition = 5;

                    if((*inputBoard).getPropInfo((*inputPlayer).intPosition).ownedProperty){ // If the property is owned
                        if((*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                            clearscreen();
                            (*inputBoard).drawBoard();
                            cout << "\nYou own " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << ", kick back and relax!\n" << endl;
                            return -1;
                        }
                        else{ // If another player owns the property, the player will be charged the proper rent
                            cout << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).playerName)
                                <<  "! Rent is $";
                                 switch((*inputBoard).getPropInfo((*inputPlayer).intPosition).numHouses){
                                    case 1: cout << 25 << endl; // If the owner owns one railroad

                                                while(25 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= 25;
                                                (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 25;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 2:     cout << 50 << endl; // If the owner owns two railroads
                                                while(50 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= 50;
                                                (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 50;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 3:     cout << 100 << endl; // If the owner owns three railroads
                                                while(100 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= 100;
                                                (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 100;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 4:     cout << 200 << endl; // If the owner owns four railroads
                                                while(200 > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= 200;
                                                (*(*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner).intWealth += 200;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;
                                 }
                        }
                    }
                    else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                        char chosenAvailPropOption = '-1';
                        cout << "\nThis property is available. Would you like to buy?" << endl;
                        cout << "     a) Yes" << endl;
                        cout << "     b) No" << endl;

                        while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                            cout << "\nPlease choose a or b: ";
                            cin >> chosenAvailPropOption;
                        }

                        if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                            if((*inputPlayer).intWealth < (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyPrice){ // If the player can't afford to buy the property
                                cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                                chosenAvailPropOption = 'b';
                            }
                            else{ // If the player can afford to buy the property, they will be able to
                                clearscreen();
                                (*inputBoard).drawBoard();
                                cout << "\nYou have successfully bought " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << "!" << endl;
                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyPrice;
                                (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                                (*inputBoard).setOwner((*inputPlayer).intPosition, inputPlayer);
                                return -1;
                            }
                        }
                        if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                            cout << "\nThe property will be auctioned." << endl;
                            auctionProperty(inputBoard, (*inputBoard).getPropAdd((*inputPlayer).intPosition));
                            return -1;
                        }
                    }

        case 12:    cout << "\nChance: Go back 3 spaces." << endl;
                    if((*inputPlayer).intPosition == 7){
                        cout << "\nYou are required to pay Income Tax! It will be: $";
                        (*inputPlayer).intPosition = 4;
                        if(((*inputPlayer).intWealth * .10) < 200){
                            cout << ((*inputPlayer).intWealth * .10) << "." << endl;
                            while((*inputPlayer).intWealth < ((*inputPlayer).intWealth * .10)){
                                int ifBankrupt = 1;
                                cout << "\nYou cannot afford that" << endl;
                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                if(ifBankrupt == -1){
                                    return -1;
                                }
                            }
                            (*inputPlayer).intWealth -= ((*inputPlayer).intWealth * .10);
                            parkSpaceJackpot += ((*inputPlayer).intWealth * .10);
                        }
                        else{
                            cout << "200." << endl;
                            while((*inputPlayer).intWealth < 200){
                                int ifBankrupt = 1;
                                cout << "\nYou cannot afford that" << endl;
                                ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                if(ifBankrupt == -1){
                                    return -1;
                                }
                            }
                            (*inputPlayer).intWealth -= 200;
                            parkSpaceJackpot += 200;
                        }
                        return -1;
                    }
                    else if((*inputPlayer).intPosition == 22){
                        (*inputPlayer).intPosition = 19;
                        if((*inputBoard).getPropInfo(19).ownedProperty){ // If the property is owned
                            if((*inputBoard).getPropInfo(19).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                                clearscreen();
                                (*inputBoard).drawBoard();
                                cout << "\nYou own " << (*inputBoard).getPropInfo(24).propertyName << ", kick back and relax!\n" << endl;
                                return -1;
                            }
                            else{ // If another player owns the property, the player will be charged the proper rent
                                cout << (*inputBoard).getPropInfo(19).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo(19).propertyOwner).playerName)
                                    <<  "! Rent is $";
                                switch((*inputBoard).getPropInfo(19).numHouses){
                                    case 0: if((*inputBoard).getPropInfo(19).ownAllColor){ // No houses but all of the color is owned
                                                cout << (2 * (*inputBoard).getPropInfo(19).rentNoHouse) << endl;
                                                while((2 * (*inputBoard).getPropInfo(19).rentNoHouse) > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (2 * (*inputBoard).getPropInfo(19).rentNoHouse);
                                                (*(*inputBoard).getPropInfo(19).propertyOwner).intWealth += (2 * (*inputBoard).getPropInfo(19).rentNoHouse);
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;
                                            }
                                            else{
                                                cout << (*inputBoard).getPropInfo(19).rentNoHouse << endl; // No houses and the whole colorset isn't owned
                                                while((*inputBoard).getPropInfo(19).rentNoHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                   }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(19).rentNoHouse;
                                                (*(*inputBoard).getPropInfo(19).propertyOwner).intWealth += (*inputBoard).getPropInfo(19).rentNoHouse;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;
                                            }

                                    case 1: cout << (*inputBoard).getPropInfo(19).rentOneHouse << endl; // If there is one house

                                                while((*inputBoard).getPropInfo(19).rentOneHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(24).rentOneHouse;
                                                (*(*inputBoard).getPropInfo(19).propertyOwner).intWealth += (*inputBoard).getPropInfo(19).rentOneHouse;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 2:     cout << (*inputBoard).getPropInfo(19).rentTwoHouse << endl; // If there is two houses
                                                while((*inputBoard).getPropInfo(19).rentTwoHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(19).rentTwoHouse;
                                                (*(*inputBoard).getPropInfo(19).propertyOwner).intWealth += (*inputBoard).getPropInfo(19).rentTwoHouse;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 3:     cout << (*inputBoard).getPropInfo(19).rentThreeHouse << endl; // If there is three houses
                                                while((*inputBoard).getPropInfo(19).rentThreeHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(19).rentThreeHouse;
                                                (*(*inputBoard).getPropInfo(19).propertyOwner).intWealth += (*inputBoard).getPropInfo(19).rentThreeHouse;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 4:     cout << (*inputBoard).getPropInfo(19).rentFourHouse << endl; // If there are four houses
                                                while((*inputBoard).getPropInfo(19).rentFourHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(19).rentFourHouse;
                                                (*(*inputBoard).getPropInfo(19).propertyOwner).intWealth += (*inputBoard).getPropInfo(19).rentFourHouse;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 5:     cout << (*inputBoard).getPropInfo(19).rentHotel << endl; // If there is a hotel/5 houses
                                                while((*inputBoard).getPropInfo(19).rentHotel > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(19).rentHotel;
                                                (*(*inputBoard).getPropInfo(19).propertyOwner).intWealth += (*inputBoard).getPropInfo(19).rentHotel;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;
                                }
                            }
                        }
                        else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                            char chosenAvailPropOption = '-1';
                            cout << "\nThis property is available. Would you like to buy?" << endl;
                            cout << "     a) Yes" << endl;
                            cout << "     b) No" << endl;

                            while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                                cout << "\nPlease choose a or b: ";
                                cin >> chosenAvailPropOption;
                            }

                            if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                                if((*inputPlayer).intWealth < (*inputBoard).getPropInfo(19).propertyPrice){ // If the player can't afford to buy the property
                                    cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                                    chosenAvailPropOption = 'b';
                                }
                                else{ // If the player can afford to buy the property, they will be able to
                                    clearscreen();
                                    (*inputBoard).drawBoard();
                                    cout << "\nYou have successfully bought " << (*inputBoard).getPropInfo(19).propertyName << "!" << endl;
                                    (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(19).propertyPrice;
                                    (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                                    (*inputBoard).setOwner(19, inputPlayer);
                                    return -1;
                                }
                            }
                            if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                                cout << "\nThe property will be auctioned." << endl;
                                auctionProperty(inputBoard, (*inputBoard).getPropAdd(19));
                                return -1;
                            }
                        }
                    }
                    else{
                        (*inputPlayer).intPosition == 33;
                        cout << "\nYou landed on Community Chest!" << endl;
                        comChestLand(inputBoard, inputPlayer);
                        return -1;
                    }

        case 13:    cout << "\nChance: Advance token to Nearest Utility." << endl;
                    cout << "          If UNOWNED, you may buy it from the Bank." << endl;
                    cout << "          If OWNED, throw dice and pay owner a total ten times the amount thrown." << endl;
                    if((*inputPlayer).intPosition == 7){
                        (*inputPlayer).intPosition = 12;
                        if((*inputBoard).getPropInfo(12).ownedProperty){ // If the property is owned
                            if((*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                                clearscreen();
                                (*inputBoard).drawBoard();
                                cout << "\nYou own " << (*inputBoard).getPropInfo(12).propertyName << ", kick back and relax!\n" << endl;
                                return -1;
                            }
                            else{ // If another player owns the property, the player will be charged the proper rent
                                cout << (*inputBoard).getPropInfo(12).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo(12).propertyOwner).playerName)
                                    <<  "! Rent is $";

                                int chanceDiceRoll = rollDie();

                                    cout << (chanceDiceRoll * 10) << endl; // If the owner owns both utilities
                                    while((chanceDiceRoll * 10) > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                        int ifBankrupt = 1;
                                        cout << "\nYou cannot afford rent!" << endl;
                                        ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                        if(ifBankrupt == -1){
                                            return -1;
                                        }
                                    }

                                    (*inputPlayer).intWealth -= (chanceDiceRoll * 10);
                                    (*(*inputBoard).getPropInfo(12).propertyOwner).intWealth += (chanceDiceRoll * 10);
                                    clearscreen();
                                    (*inputBoard).drawBoard();
                                    cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                    return -1;

                            }
                        }
                        else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                            char chosenAvailPropOption = '-1';
                            cout << "\nThis property is available. Would you like to buy?" << endl;
                            cout << "     a) Yes" << endl;
                            cout << "     b) No" << endl;

                            while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                                cout << "\nPlease choose a or b: ";
                                cin >> chosenAvailPropOption;
                            }

                            if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                                if((*inputPlayer).intWealth < (*inputBoard).getPropInfo(12).propertyPrice){ // If the player can't afford to buy the property
                                    cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                                    chosenAvailPropOption = 'b';
                                }
                                else{ // If the player can afford to buy the property, they will be able to
                                    clearscreen();
                                    (*inputBoard).drawBoard();
                                    cout << "\nYou have successfully bought " << (*inputBoard).getPropInfo(12).propertyName << "!" << endl;
                                    (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(12).propertyPrice;
                                    (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                                    (*inputBoard).setOwner(12, inputPlayer);
                                    return -1;
                                }
                            }
                            if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                                cout << "\nThe property will be auctioned." << endl;
                                auctionProperty(inputBoard, (*inputBoard).getPropAdd((*inputPlayer).intPosition));
                                return -1;
                            }
                        }
                    }
                    else if((*inputPlayer).intPosition == 22){
                        (*inputPlayer).intPosition = 28;
                        if((*inputBoard).getPropInfo(28).ownedProperty){ // If the property is owned
                            if((*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                                clearscreen();
                                (*inputBoard).drawBoard();
                                cout << "\nYou own " << (*inputBoard).getPropInfo(28).propertyName << ", kick back and relax!\n" << endl;
                                return -1;
                            }
                            else{ // If another player owns the property, the player will be charged the proper rent
                                cout << (*inputBoard).getPropInfo(28).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo(28).propertyOwner).playerName)
                                    <<  "! Rent is $";

                                int chanceDiceRoll = rollDie();

                                    cout << (chanceDiceRoll * 10) << endl; // If the owner owns both utilities
                                    while((chanceDiceRoll * 10) > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                        int ifBankrupt = 1;
                                        cout << "\nYou cannot afford rent!" << endl;
                                        ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                        if(ifBankrupt == -1){
                                            return -1;
                                        }
                                    }

                                    (*inputPlayer).intWealth -= (chanceDiceRoll * 10);
                                    (*(*inputBoard).getPropInfo(28).propertyOwner).intWealth += (chanceDiceRoll * 10);
                                    clearscreen();
                                    (*inputBoard).drawBoard();
                                    cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                    return -1;

                            }
                        }
                        else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                            char chosenAvailPropOption = '-1';
                            cout << "\nThis property is available. Would you like to buy?" << endl;
                            cout << "     a) Yes" << endl;
                            cout << "     b) No" << endl;

                            while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                                cout << "\nPlease choose a or b: ";
                                cin >> chosenAvailPropOption;
                            }

                            if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                                if((*inputPlayer).intWealth < (*inputBoard).getPropInfo(28).propertyPrice){ // If the player can't afford to buy the property
                                    cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                                    chosenAvailPropOption = 'b';
                                }
                                else{ // If the player can afford to buy the property, they will be able to
                                    clearscreen();
                                    (*inputBoard).drawBoard();
                                    cout << "\nYou have successfully bought " << (*inputBoard).getPropInfo(28).propertyName << "!" << endl;
                                    (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(28).propertyPrice;
                                    (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                                    (*inputBoard).setOwner(28, inputPlayer);
                                    return -1;
                                }
                            }
                            if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                                cout << "\nThe property will be auctioned." << endl;
                                auctionProperty(inputBoard, (*inputBoard).getPropAdd((*inputPlayer).intPosition));
                                return -1;
                            }
                        }
                    }
                else{
                    cout << "\nYou passed Go, collect $200!" << endl;
                    (*inputPlayer).intWealth += 200;
                    (*inputPlayer).intPosition = 12;
                    if((*inputBoard).getPropInfo(12).ownedProperty){ // If the property is owned
                            if((*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                                cout << "\nYou own " << (*inputBoard).getPropInfo(12).propertyName << ", kick back and relax!\n" << endl;
                                return -1;
                            }
                            else{ // If another player owns the property, the player will be charged the proper rent
                                cout << (*inputBoard).getPropInfo(12).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo(12).propertyOwner).playerName)
                                    <<  "! Rent is $";

                                int chanceDiceRoll = rollDie();

                                    cout << (chanceDiceRoll * 10) << endl; // If the owner owns both utilities
                                    while((chanceDiceRoll * 10) > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                        int ifBankrupt = 1;
                                        cout << "\nYou cannot afford rent!" << endl;
                                        ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                        if(ifBankrupt == -1){
                                            return -1;
                                        }
                                    }

                                    (*inputPlayer).intWealth -= (chanceDiceRoll * 10);
                                    (*(*inputBoard).getPropInfo(12).propertyOwner).intWealth += (chanceDiceRoll * 10);
                                    clearscreen();
                                    (*inputBoard).drawBoard();
                                    cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                    return -1;

                            }
                    }
                    else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                            char chosenAvailPropOption = '-1';
                            cout << "\nThis property is available. Would you like to buy?" << endl;
                            cout << "     a) Yes" << endl;
                            cout << "     b) No" << endl;

                            while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                                cout << "\nPlease choose a or b: ";
                                cin >> chosenAvailPropOption;
                            }

                            if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                                if((*inputPlayer).intWealth < (*inputBoard).getPropInfo(12).propertyPrice){ // If the player can't afford to buy the property
                                    cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                                    chosenAvailPropOption = 'b';
                                }
                                else{ // If the player can afford to buy the property, they will be able to
                                    clearscreen();
                                    (*inputBoard).drawBoard();
                                    cout << "\nYou have successfully bought " << (*inputBoard).getPropInfo(12).propertyName << "!" << endl;
                                    (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(12).propertyPrice;
                                    (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                                    (*inputBoard).setOwner(12, inputPlayer);
                                    return -1;
                                }
                            }
                            if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                                cout << "\nThe property will be auctioned." << endl;
                                auctionProperty(inputBoard, (*inputBoard).getPropAdd((*inputPlayer).intPosition));
                                return -1;
                            }
                    }
                }

        case 14:    cout << "\nChance: Advance to St. Charles Place." << endl;
                    cout << "          If you pass go, collect $200." << endl;

                    if((*inputPlayer).intPosition > 11){
                        cout << "\nYou passed Go, collect $200!" << endl;
                        (*inputPlayer).intWealth += 200;
                    }

                    (*inputPlayer).intPosition = 11;
                    if((*inputBoard).getPropInfo(11).ownedProperty){ // If the property is owned
                        if((*inputBoard).getPropInfo(11).propertyOwner == inputPlayer){ // if the player owns the  property, they will not be charged rent
                            clearscreen();
                            (*inputBoard).drawBoard();
                            cout << "\nYou own " << (*inputBoard).getPropInfo(11).propertyName << ", kick back and relax!\n" << endl;
                            return -1;
                        }
                        else{ // If another player owns the property, the player will be charged the proper rent
                            cout << (*inputBoard).getPropInfo(11).propertyName << " is owned by " << ((*(*inputBoard).getPropInfo(11).propertyOwner).playerName)
                                <<  "! Rent is $";
                                switch((*inputBoard).getPropInfo(11).numHouses){
                                    case 0: if((*inputBoard).getPropInfo(11).ownAllColor){ // No houses but all of the color is owned
                                                cout << (2 * (*inputBoard).getPropInfo(11).rentNoHouse) << endl;
                                                while((2 * (*inputBoard).getPropInfo(11).rentNoHouse) > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (2 * (*inputBoard).getPropInfo(11).rentNoHouse);
                                                (*(*inputBoard).getPropInfo(11).propertyOwner).intWealth += (2 * (*inputBoard).getPropInfo(11).rentNoHouse);
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;
                                            }
                                            else{
                                                cout << (*inputBoard).getPropInfo(11).rentNoHouse << endl; // No houses and the whole colorset isn't owned
                                                while((*inputBoard).getPropInfo(11).rentNoHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                   }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(11).rentNoHouse;
                                                (*(*inputBoard).getPropInfo(11).propertyOwner).intWealth += (*inputBoard).getPropInfo(11).rentNoHouse;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;
                                            }

                                    case 1: cout << (*inputBoard).getPropInfo(11).rentOneHouse << endl; // If there is one house

                                                while((*inputBoard).getPropInfo(11).rentOneHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(11).rentOneHouse;
                                                (*(*inputBoard).getPropInfo(11).propertyOwner).intWealth += (*inputBoard).getPropInfo(11).rentOneHouse;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 2:     cout << (*inputBoard).getPropInfo(11).rentTwoHouse << endl; // If there is two houses
                                                while((*inputBoard).getPropInfo(11).rentTwoHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(11).rentTwoHouse;
                                                (*(*inputBoard).getPropInfo(11).propertyOwner).intWealth += (*inputBoard).getPropInfo(11).rentTwoHouse;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 3:     cout << (*inputBoard).getPropInfo(11).rentThreeHouse << endl; // If there is three houses
                                                while((*inputBoard).getPropInfo(11).rentThreeHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(11).rentThreeHouse;
                                                (*(*inputBoard).getPropInfo(11).propertyOwner).intWealth += (*inputBoard).getPropInfo(11).rentThreeHouse;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 4:     cout << (*inputBoard).getPropInfo(11).rentFourHouse << endl; // If there are four houses
                                                while((*inputBoard).getPropInfo(11).rentFourHouse > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(11).rentFourHouse;
                                                (*(*inputBoard).getPropInfo(11).propertyOwner).intWealth += (*inputBoard).getPropInfo(11).rentFourHouse;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;

                                    case 5:     cout << (*inputBoard).getPropInfo(11).rentHotel << endl; // If there is a hotel/5 houses
                                                while((*inputBoard).getPropInfo(11).rentHotel > (*inputPlayer).intWealth){ // If the player can't afford to pay rent
                                                    int ifBankrupt = 1;
                                                    cout << "\nYou cannot afford rent!" << endl;
                                                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                                                    if(ifBankrupt == -1){
                                                        return -1;
                                                    }
                                                }

                                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(11).rentHotel;
                                                (*(*inputBoard).getPropInfo(11).propertyOwner).intWealth += (*inputBoard).getPropInfo(11).rentHotel;
                                                clearscreen();
                                                (*inputBoard).drawBoard();
                                                cout << "\n" << (*inputPlayer).playerName << " paid rent!" << endl;
                                                return -1;
                                }
                        }
                    }
                    else{ // If the property was not owned, they player will have the opportunity to buy it or auction it
                        char chosenAvailPropOption = '-1';
                        cout << "\nThis property is available. Would you like to buy?" << endl;
                        cout << "     a) Yes" << endl;
                        cout << "     b) No" << endl;

                        while(chosenAvailPropOption != 'a' && chosenAvailPropOption != 'b' && chosenAvailPropOption != 'A' && chosenAvailPropOption != 'B'){
                            cout << "\nPlease choose a or b: ";
                            cin >> chosenAvailPropOption;
                        }

                        if(chosenAvailPropOption == 'a' || chosenAvailPropOption == 'A'){
                            if((*inputPlayer).intWealth < (*inputBoard).getPropInfo(11).propertyPrice){ // If the player can't afford to buy the property
                                cout << "\nYou can't afford to buy this property! It will be auctioned." << endl;
                                chosenAvailPropOption = 'b';
                            }
                            else{ // If the player can afford to buy the property, they will be able to
                                clearscreen();
                                (*inputBoard).drawBoard();
                                cout << "\nYou have successfully bought " << (*inputBoard).getPropInfo(11).propertyName << "!" << endl;
                                (*inputPlayer).intWealth -= (*inputBoard).getPropInfo(11).propertyPrice;
                                (*inputBoard).setOwnedProp((*inputPlayer).intPosition, true);
                                (*inputBoard).setOwner(24, inputPlayer);
                                return -1;
                            }
                        }
                        if(chosenAvailPropOption == 'b' || chosenAvailPropOption == 'B'){ // If the player chose not to buy, or couldn't buy
                            cout << "\nThe property will be auctioned." << endl;
                            auctionProperty(inputBoard, (*inputBoard).getPropAdd(11));
                            return -1;
                        }
                    }

        case 15:    cout << "\nChance: Go Directly to Jail. Do not Pass go, do not collect $200." << endl;
                    (*inputPlayer).intPosition = 10;
                    (*inputPlayer).inJail = true;
                    (*inputPlayer).numTurnInJail = 0;
                    return -1;

        case 16:    cout << "\nChance: Bank Pays You Dividend of $50!" << endl;
                    (*inputPlayer).intWealth += 50;
                    return -1;
    }

}

int comChestLand(GameBoard *inputBoard, Player *inputPlayer){
    int whichCCCard = -1;

    srand(time(0));
    whichCCCard = ((rand() % 16) + 1);

    switch(whichCCCard){
        case 1: cout << "\nCommunity Chest: Get out of jail free! This card may be kept until needed or sold." << endl;
                (*inputPlayer).outJailFree = true;
                return -1;

        case 2: cout << "\nCommunity Chest: You Have Won Second Prize in a Beauty Contest! Collect $10." << endl;
                (*inputPlayer).intWealth += 10;
                return -1;

        case 3: cout << "\nCommunity Chest: Bank Error in your Favor! Collect $200." << endl;
                (*inputPlayer).intWealth += 200;
                return -1;

        case 4: cout << "\nCommunity Chest: Pay Hospital $100." << endl;
                while((*inputPlayer).intWealth < 100){
                    int ifBankrupt = 1;
                    cout << "\nYou can't afford that!" << endl;
                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                    if(ifBankrupt == -1){
                        return -1;
                    }
                }
                (*inputPlayer).intWealth -= 100;
                parkSpaceJackpot += 100;
                return -1;

        case 5: cout << "\nCommunity Chest: Christmas Fund Matures. Collect $100!" << endl;
                (*inputPlayer).intWealth += 100;
                return -1;

        case 6: cout << "\nCommunity Chest: Doctor's Fee. Pay $50." << endl;
                while((*inputPlayer).intWealth < 50){
                    int ifBankrupt = 1;
                    cout << "\nYou can't afford that!" << endl;
                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                    if(ifBankrupt == -1){
                        return -1;
                    }
                }
                (*inputPlayer).intWealth -= 50;
                return -1;

        case 7: {cout << "\nCommunity Chest: Income Tax Refund! Collect $20." << endl;
                (*inputPlayer).intWealth += 20;}
                return -1;

        case 8: {cout << "\nCommunity Chest: Grand Opera Opening. Collect $50 from Every Player for Opening Night Seats." << endl;
                int numRemainPlayers = 0;
                int totalCost = 0;

                for(int i = 0; i < numPlayers; i++){
                     if(!gamePlayers[i].boolBankrupt && gamePlayers[i].playerName != (*inputPlayer).playerName){ // bankrupt players and the current player is excluded from the count
                        int ifBankrupt = 1;
                        while(gamePlayers[i].intWealth < 50){
                            cout << "\nYou cannot afford to pay!" << endl;
                            ifBankrupt = avoidBankruptcy(inputBoard, &gamePlayers[i]);
                            if(ifBankrupt == -1){
                                gamePlayers[i].intWealth += 50;
                                ifBankrupt = 1;
                            }
                        }
                        if(ifBankrupt == 1){
                            numRemainPlayers++;
                        }
                        gamePlayers[i].intWealth -= 50;
                    }
                }

                totalCost = (numRemainPlayers * 50);
                (*inputPlayer).intPosition += totalCost;}
                return -1;

        case 9: {cout << "\nCommunity Chest: Advance to Go. Collect $200!" << endl;
                (*inputPlayer).intPosition = 0;
                (*inputPlayer).intWealth += 200;}
                return -1;

        case 10:    {cout << "\nCommunity Chest: You are Assessed for Street Repairs." << endl;
                    cout << "                     $40 per house" << endl;
                    cout << "                     $115 per hotel" << endl;

                    int numHouses = 0;
                    int numHotels = 0;
                    int totalCost = 0;

                    /* This loop counts the total number of houses and hotels the player has */
                    for(int i = 0; i < ownedProperties.at((*inputPlayer).playerNum).size(); i++){
                        if((*ownedProperties.at((*inputPlayer).playerNum).at(i)).numHouses > 0 && (*ownedProperties.at((*inputPlayer).playerNum).at(i)).intPropertyType == 0){
                            if((*ownedProperties.at((*inputPlayer).playerNum).at(i)).numHouses == 5){ // 5 houses == 1 hotel
                                    numHotels++;
                            }
                            else{
                                numHouses += (*ownedProperties.at((*inputPlayer).playerNum).at(i)).numHouses;
                            }
                        }
                    }

                    totalCost = ((numHouses * 40) + (numHotels * 115)); // This totals up the bill, depending on how many houses and hotels the player has
                    parkSpaceJackpot += totalCost;

                    cout << "\nYou must pay $" << totalCost << "." << endl;

                    while(totalCost > (*inputPlayer).intWealth){ // If the player cant afford to pay
                        int ifBankrupt = 1;
                        cout << "\nYou can't afford that!" << endl;
                        ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                        if(ifBankrupt == -1){
                            return -1;
                        }
                    }
                    parkSpaceJackpot += totalCost;
                    (*inputPlayer).intWealth -= totalCost;}
                    return -1;

        case 11:    {cout << "\nCommunity Chest: Pay School Tax of $150." << endl;
                    while((*inputPlayer).intWealth < 150){
                        int ifBankrupt = 1;
                        cout << "\nYou can't afford that!" << endl;
                        ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                        if(ifBankrupt == -1){
                            return -1;
                        }
                    }
                    (*inputPlayer).intWealth -= 150;
                    parkSpaceJackpot += 150;}
                    return -1;

        case 12:    {cout << "\nCommunity Chest: Go to Jail, Go Directly to Jail. DO NOT PASS GO, DO NOT COLLECT $200." << endl;
                    (*inputPlayer).intPosition = 10;
                    (*inputPlayer).inJail = true;
                    (*inputPlayer).numTurnInJail = 0;}
                    return -1;

        case 13:    cout << "\nCommunity Chest: Receive for Services $25!" << endl;
                    (*inputPlayer).intWealth += 25;
                    return -1;

        case 14:    cout << "\nCommunity Chest: You Inherit $100!" << endl;
                    (*inputPlayer).intWealth += 100;
                    return -1;

        case 15:    cout << "\nCommunity Chest: Life Insurance Matures. Collect $100!" << endl;
                    (*inputPlayer).intWealth += 100;
                    return -1;

        case 16:    cout << "\nCommunity Chest: From Sale of Stock. You Get $45!" << endl;
                    (*inputPlayer).intWealth += 45;
                    return -1;
    }
}

int playerInJail(GameBoard *inputBoard, Player *inputPlayer){
    char chosenJailOption = '-1';
    int diceRoll;
    bool leaveJail = false;
    if(gamePlayers[turnNum].numTurnInJail < 3){ // If this isn't the player's third turn in jail
        while(gamePlayers[turnNum].outJailFree){
            cout << "\nYou are in Jail! What would you like to do?" << endl;
            cout << "     a) Roll for doubles";
            cout << "     b) Pay $50 bail";
            cout << "     c) Use Get out of Jail Free card";

            while(chosenJailOption != 'a' && chosenJailOption != 'b'&& chosenJailOption != 'c' && chosenJailOption != 'A' && chosenJailOption != 'B' && chosenJailOption != 'C'){
                cout << "\n\nPlease choose a, b, or c: ";
                cin >> chosenJailOption;
            }
            if(chosenJailOption == 'a' || chosenJailOption == 'A'){
                diceRoll = rollDie();
                leaveJail = rollDoubles();
                (*inputPlayer).numTurnInJail++;
                if(leaveJail && (diceRoll % 2 == 0)){ // If the player rolled doubles
                    (*inputPlayer).inJail = false;
                    (*inputPlayer).numTurnInJail = 0;
                    return diceRoll;
                }
                else{
                    return -2;
                }
            }
            if(chosenJailOption == 'b' || chosenJailOption == 'B'){
                /* If the player can't afford to pay $50 */
                if(gamePlayers[turnNum].intWealth < 50){
                    cout << "You can't afford to pay $50!" << endl;
                    leaveJail = false;
                }
                else{
                    clearscreen();
                    (*inputBoard).drawBoard();
                    cout << "\nYou paid $50 to leave jail!" << endl;
                    (*inputPlayer).intWealth -= 50;
                    (*inputPlayer).inJail = false;
                    (*inputPlayer).numTurnInJail = 0;
                    parkSpaceJackpot += 50;
                    return -1;
                }
            }
            if(chosenJailOption == 'c' || chosenJailOption == 'C'){
                clearscreen();
                (*inputBoard).drawBoard();
                cout << "\nYou used your Get out of Jail Free card!" << endl;
                (*inputPlayer).outJailFree = false;
                (*inputPlayer).inJail = false;
                (*inputPlayer).numTurnInJail = 0;
                return -1;
            }
        }
        while(!gamePlayers[turnNum].outJailFree){
            cout << "\n You are in Jail! What would you like to do?" << endl;
            cout << "     a) Roll for doubles";
            cout << "     b) Pay $50 bail";

            while(chosenJailOption != 'a' && chosenJailOption != 'b'&& chosenJailOption != 'A' && chosenJailOption != 'B'){
                cout << "\n\nPlease choose a or b: ";
                cin >> chosenJailOption;
            }
            if(chosenJailOption == 'a' || chosenJailOption == 'A'){
                diceRoll = rollDie();
                leaveJail = rollDoubles();
                (*inputPlayer).numTurnInJail++;
                if(leaveJail && (diceRoll % 2 == 0)){ // If the player rolled doubles
                    (*inputPlayer).inJail = false;
                    (*inputPlayer).numTurnInJail = 0;
                    return diceRoll;
                }
                else{
                        return -2;
                }
            }
            if(chosenJailOption == 'b' || chosenJailOption == 'B'){
                /* If the player can't afford to pay $50 */
                if(gamePlayers[turnNum].intWealth < 50){
                    cout << "You can't afford to pay $50!" << endl;
                    leaveJail = false;
                }
                else{
                    clearscreen();
                    (*inputBoard).drawBoard();
                    cout << "\nYou paid $50 to leave jail!" << endl;
                    (*inputPlayer).intWealth -= 50;
                    (*inputPlayer).inJail = false;
                    (*inputPlayer).numTurnInJail = 0;
                    parkSpaceJackpot += 50;
                    return -1;
                }
            }
        }
    }
    else{ // If the player hasn't left jail in three turns
        while(gamePlayers[turnNum].outJailFree){
            cout << "\n You are in Jail! What would you like to do?" << endl;
            cout << "     a) Roll for doubles";
            cout << "     b) Pay $50 bail";
            cout << "     c) Use Get out of Jail Free card";

            while(chosenJailOption != 'a' && chosenJailOption != 'b'&& chosenJailOption != 'c' && chosenJailOption != 'A' && chosenJailOption != 'B' && chosenJailOption != 'C'){
                cout << "\n\nPlease choose a, b, or c: ";
                cin >> chosenJailOption;
            }
            if(chosenJailOption == 'a' || chosenJailOption == 'A'){
                diceRoll = rollDie();
                leaveJail = rollDoubles();
                if(leaveJail && (diceRoll % 2 == 0)){ // If the player rolled doubles
                    (*inputPlayer).inJail = false;
                    (*inputPlayer).numTurnInJail = 0;
                    return diceRoll;
                }
                else{
                    cout << "\nYou did not roll doubles. Please pay $50." << endl;
                    chosenJailOption = 'b';
                }
            }
            if(chosenJailOption == 'b' || chosenJailOption == 'B'){
                /* If the player can't afford to pay $50 */
                while(gamePlayers[turnNum].intWealth < 50){
                    int ifBankrupt = 1;
                    cout << "\nYou can't afford to pay $50!" << endl;
                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                    if(ifBankrupt == -1){
                        return -1;
                    }
                }

                clearscreen();
                (*inputBoard).drawBoard();
                cout << "\nYou paid $50 to leave jail!" << endl;
                (*inputPlayer).intWealth -= 50;
                (*inputPlayer).inJail = false;
                (*inputPlayer).numTurnInJail = 0;
                return -1;
            }
            if(chosenJailOption == 'c' || chosenJailOption == 'C'){
                clearscreen();
                (*inputBoard).drawBoard();
                cout << "\nYou used your Get out of Jail Free card!" << endl;
                (*inputPlayer).outJailFree = false;
                (*inputPlayer).inJail = false;
                (*inputPlayer).numTurnInJail = 0;
                return -1;
            }
        }
        while(!gamePlayers[turnNum].outJailFree){
            cout << "\n You are in Jail! What would you like to do?" << endl;
            cout << "     a) Roll for doubles";
            cout << "     b) Pay $50 bail";

            while(chosenJailOption != 'a' && chosenJailOption != 'b'&& chosenJailOption != 'A' && chosenJailOption != 'B'){
                cout << "\n\nPlease choose a or b: ";
                cin >> chosenJailOption;
            }
                   if(chosenJailOption == 'a' || chosenJailOption == 'A'){
                diceRoll = rollDie();
                leaveJail = rollDoubles();
                if(leaveJail && (diceRoll % 2 == 0)){ // If the player rolled doubles
                    (*inputPlayer).inJail = false;
                    (*inputPlayer).numTurnInJail = 0;
                    return diceRoll;
                }
                else{
                    cout << "\nYou did not roll doubles. Please pay $50." << endl;
                    chosenJailOption = 'b';
                }
            }
            if(chosenJailOption == 'b' || chosenJailOption == 'B'){
                /* If the player can't afford to pay $50 */
                while(gamePlayers[turnNum].intWealth < 50){
                    int ifBankrupt = 1;
                    cout << "\nYou can't afford to pay $50!" << endl;
                    ifBankrupt = avoidBankruptcy(inputBoard, inputPlayer);
                    if(ifBankrupt == -1){
                        return -1;
                    }
                }

                clearscreen();
                (*inputBoard).drawBoard();
                cout << "\nYou paid $50 to leave jail!" << endl;
                (*inputPlayer).intWealth -= 50;
                (*inputPlayer).inJail = false;
                (*inputPlayer).numTurnInJail = 0;
                parkSpaceJackpot += 50;
                return -1;
            }
        }

    }

}

void calculateOwnedProps(GameBoard *inputBoard){
    /* Loop cycles through each player */
    for (int i = 0; i < numPlayers; i++){
        if(!gamePlayers[i].boolBankrupt){ // if the player is bankrupt, he or she is skipped
            int numOwnedProp = 0;
            ownedProperties.at(i).clear(); // clears any properties already there so it can be updated

            /* Loop cycles through each property on the board */
            for(int i2 = 0; i2 < ((*inputBoard).getNumProperties()); i2++) {

                if((*inputBoard).getPropInfo(i2).ownedProperty){ // Is the property owned?

                    if((*(*inputBoard).getPropInfo(i2).propertyOwner).playerName == gamePlayers[i].playerName){ // If the property is owned, then is it by the player being cycled?
                       // Property *tempOwnedProp;
                       // tempOwnedProp = (*inputBoard).getPropAdd(i2); // Stores the address to the property
                        ownedProperties.at(i).push_back((*inputBoard).getPropAdd(i2)); // Adds the property to the player's vector

                        numOwnedProp++; // Increments the index to add a new member to the vector if there is another property to be added later
                    }
                }
            }
        }
        else{
            ownedProperties.at(i).clear();
        }
    }

    for(int i = 0; i < numPlayers; i++){
        int railRaoadsOwned = 0;
        int ownOtherUtility = 0;
        int numPurpleProps = 0;
        int numSkyProps = 0;
        int numPinkProps = 0;
        int numOrangeProps = 0;
        int numRedProps = 0;
        int numYellowProps = 0;
        int numGreenProps = 0;
        int numBlueProps = 0;

        for(int i2 = 0; i2 < ownedProperties.at(i).size(); i2++){ // This loop calculates how many railroads the player owns
            if(((*ownedProperties.at(i).at(i2)).intPropertyType = 0) && ((*ownedProperties.at(i).at(i2)).colorGroup == PURPLE)){
                numPurpleProps += 1;
            }
            else if(((*ownedProperties.at(i).at(i2)).intPropertyType = 0) && ((*ownedProperties.at(i).at(i2)).colorGroup == SKY)){
                numSkyProps += 1;
            }
            else if(((*ownedProperties.at(i).at(i2)).intPropertyType = 0) && ((*ownedProperties.at(i).at(i2)).colorGroup == PINK)){
                numPinkProps += 1;
            }
            else if(((*ownedProperties.at(i).at(i2)).intPropertyType = 0) && ((*ownedProperties.at(i).at(i2)).colorGroup == ORANGE)){
                numOrangeProps += 1;
            }
            else if(((*ownedProperties.at(i).at(i2)).intPropertyType = 0) && ((*ownedProperties.at(i).at(i2)).colorGroup == RED)){
                numRedProps += 1;
            }
            else if(((*ownedProperties.at(i).at(i2)).intPropertyType = 0) && ((*ownedProperties.at(i).at(i2)).colorGroup == YELLOW)){
                numYellowProps += 1;
            }
            else if(((*ownedProperties.at(i).at(i2)).intPropertyType = 0) && ((*ownedProperties.at(i).at(i2)).colorGroup == GREEN)){
                numGreenProps += 1;
            }
            else if(((*ownedProperties.at(i).at(i2)).intPropertyType = 0) && ((*ownedProperties.at(i).at(i2)).colorGroup == BLUE)){
                numBlueProps += 1;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 1){
                railRaoadsOwned += 1;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 2){ // This loop calculates how many utilities the player owns
                ownOtherUtility += 1;
            }

        }
         for(int i2 = 0; i2 < ownedProperties.at(i).size(); i2++){ // This loop sets the numHouses of the property to the number of the similar types
            if((*ownedProperties.at(i).at(i2)).intPropertyType = 0 && (*ownedProperties.at(i).at(i2)).colorGroup == PURPLE && numPurpleProps == 2){
                (*ownedProperties.at(i).at(i2)).ownAllColor = true;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 0 && (*ownedProperties.at(i).at(i2)).colorGroup == SKY && numSkyProps == 3){
                (*ownedProperties.at(i).at(i2)).ownAllColor = true;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 0 && (*ownedProperties.at(i).at(i2)).colorGroup == PINK && numPinkProps == 3){
                (*ownedProperties.at(i).at(i2)).ownAllColor = true;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 0 && (*ownedProperties.at(i).at(i2)).colorGroup == ORANGE && numOrangeProps == 3){
                (*ownedProperties.at(i).at(i2)).ownAllColor = true;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 0 && (*ownedProperties.at(i).at(i2)).colorGroup == RED && numRedProps == 3){
                (*ownedProperties.at(i).at(i2)).ownAllColor = true;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 0 && (*ownedProperties.at(i).at(i2)).colorGroup == YELLOW && numYellowProps == 3){
                (*ownedProperties.at(i).at(i2)).ownAllColor = true;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 0 && (*ownedProperties.at(i).at(i2)).colorGroup == GREEN && numGreenProps == 3){
                (*ownedProperties.at(i).at(i2)).ownAllColor = true;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 0 && (*ownedProperties.at(i).at(i2)).colorGroup == BLUE && numBlueProps == 2){
                (*ownedProperties.at(i).at(i2)).ownAllColor = true;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 1){
                (*ownedProperties.at(i).at(i2)).numHouses = railRaoadsOwned;
            }
            else if((*ownedProperties.at(i).at(i2)).intPropertyType = 2){
                (*ownedProperties.at(i).at(i2)).numHouses = ownOtherUtility;
            }

        }

    }
}

void displayOwnedProps(){
    cout << "\nHere are the currently owned properties:" << endl;
    /* Loop cycles through each player */
    for(int i = 0; i < numPlayers; i++){
        if(!gamePlayers[i].boolBankrupt){ // if the player is bankrupt, he or she is skipped

            cout << "     " << gamePlayers[i].playerName << ":\n ";
            /* Loop Cycles through each player's properties */
            for(int i2 = 0; i2 < ownedProperties.at(i).size(); i2++){
                cout << "          " << (*(ownedProperties.at(i).at(i2))).propertyName <<  endl; // Prints the property's name
            }
            cout << "\n";
        }
    }
    cout << "\n";
}

void displayPlayerStats(GameBoard *inputBoard, Player *inputPlayer){

    cout << "\nPosition: " << (*inputBoard).getPropInfo((*inputPlayer).intPosition).propertyName << endl; // Prints the position
    cout << "Funds: $" << (*inputPlayer).intWealth << endl; // Prints their wealth

    if((*inputPlayer).outJailFree == true){ // If they have a get out of jail free card
        cout << "You have a get out of jail free card!" << endl;
    }

}

int buildHouse(GameBoard *inputBoard){
     if(ownedProperties.at(turnNum).empty()){ // If the player owns no properties
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYou don't have properties to build on!\n" << endl;
        return -1;
    }

    vector<Property*> buildableProperties;
    char chosenOption = 'z';

    bool noColorFlag = true; // Flags if the player owns no full color sets

    bool purpleFlag = false;
    bool skyFlag = false;
    bool pinkFlag = false;
    bool orangeFlag = false;
    bool redFlag = false;
    bool yellowFlag = false;
    bool greenFlag = false;
    bool blueFlag = false;

    /* This loop adds any properties (of the player's owned properties) that the player can build on, not accounting for expense */
    for(int i = 0; i < ownedProperties.at(turnNum).size(); i++){

        Property *tempOwnedProp;
        tempOwnedProp = ownedProperties.at(turnNum).at(i); // Stores the address to the property

        if((*tempOwnedProp).ownAllColor){
            noColorFlag = false; // If the examined property's color group is owned by the player, then the flag is taken down
            if((*tempOwnedProp).colorGroup == PURPLE && purpleFlag == false){
                purpleFlag = true;
                if((*inputBoard).getPropInfo(1).numHouses == (*inputBoard).getPropInfo(3).numHouses && (*inputBoard).getPropInfo(1).numHouses < 5){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    buildableProperties.push_back((*inputBoard).getPropAdd(1));
                    buildableProperties.push_back((*inputBoard).getPropAdd(3));
                }
                else if((*inputBoard).getPropInfo(1).numHouses < (*inputBoard).getPropInfo(3).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(1));
                }
                else{
                    buildableProperties.push_back((*inputBoard).getPropAdd(3));
                }
            }
            else if((*tempOwnedProp).colorGroup == SKY && skyFlag ==false){
                skyFlag = true;
                if((*inputBoard).getPropInfo(6).numHouses == (*inputBoard).getPropInfo(8).numHouses && (*inputBoard).getPropInfo(6).numHouses == (*inputBoard).getPropInfo(9).numHouses && (*inputBoard).getPropInfo(6).numHouses < 5){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    buildableProperties.push_back((*inputBoard).getPropAdd(6));
                    buildableProperties.push_back((*inputBoard).getPropAdd(8));
                    buildableProperties.push_back((*inputBoard).getPropAdd(9));
                }
                else if((*inputBoard).getPropInfo(6).numHouses < (*inputBoard).getPropInfo(8).numHouses && (*inputBoard).getPropInfo(6).numHouses < (*inputBoard).getPropInfo(9).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(6));
                }
                else if((*inputBoard).getPropInfo(6).numHouses < (*inputBoard).getPropInfo(8).numHouses && (*inputBoard).getPropInfo(6).numHouses == (*inputBoard).getPropInfo(9).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(6));
                    buildableProperties.push_back((*inputBoard).getPropAdd(9));
                }
                else if((*inputBoard).getPropInfo(6).numHouses < (*inputBoard).getPropInfo(9).numHouses && (*inputBoard).getPropInfo(6).numHouses == (*inputBoard).getPropInfo(8).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(6));
                    buildableProperties.push_back((*inputBoard).getPropAdd(8));
                }
                else if((*inputBoard).getPropInfo(8).numHouses < (*inputBoard).getPropInfo(9).numHouses && (*inputBoard).getPropInfo(8).numHouses < (*inputBoard).getPropInfo(6).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(8));
                }
                else if((*inputBoard).getPropInfo(8).numHouses < (*inputBoard).getPropInfo(6).numHouses && (*inputBoard).getPropInfo(8).numHouses == (*inputBoard).getPropInfo(9).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(8));
                    buildableProperties.push_back((*inputBoard).getPropAdd(9));
                }
                else if((*inputBoard).getPropInfo(9).numHouses < (*inputBoard).getPropInfo(6).numHouses && (*inputBoard).getPropInfo(9).numHouses < (*inputBoard).getPropInfo(8).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(9));
                }
            }
            else if((*tempOwnedProp).colorGroup == PINK && pinkFlag == false){
                pinkFlag = true;
                if((*inputBoard).getPropInfo(11).numHouses == (*inputBoard).getPropInfo(13).numHouses && (*inputBoard).getPropInfo(11).numHouses == (*inputBoard).getPropInfo(14).numHouses && (*inputBoard).getPropInfo(11).numHouses < 5){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    buildableProperties.push_back((*inputBoard).getPropAdd(11));
                    buildableProperties.push_back((*inputBoard).getPropAdd(13));
                    buildableProperties.push_back((*inputBoard).getPropAdd(14));
                }
                else if((*inputBoard).getPropInfo(11).numHouses < (*inputBoard).getPropInfo(13).numHouses && (*inputBoard).getPropInfo(11).numHouses < (*inputBoard).getPropInfo(14).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(11));
                }
                else if((*inputBoard).getPropInfo(11).numHouses < (*inputBoard).getPropInfo(13).numHouses && (*inputBoard).getPropInfo(11).numHouses == (*inputBoard).getPropInfo(14).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(11));
                    buildableProperties.push_back((*inputBoard).getPropAdd(14));
                }
                else if((*inputBoard).getPropInfo(11).numHouses < (*inputBoard).getPropInfo(14).numHouses && (*inputBoard).getPropInfo(11).numHouses == (*inputBoard).getPropInfo(13).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(11));
                    buildableProperties.push_back((*inputBoard).getPropAdd(13));
                }
                else if((*inputBoard).getPropInfo(8).numHouses < (*inputBoard).getPropInfo(14).numHouses && (*inputBoard).getPropInfo(13).numHouses < (*inputBoard).getPropInfo(11).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(13));
                }
                else if((*inputBoard).getPropInfo(8).numHouses < (*inputBoard).getPropInfo(11).numHouses && (*inputBoard).getPropInfo(13).numHouses == (*inputBoard).getPropInfo(14).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(13));
                    buildableProperties.push_back((*inputBoard).getPropAdd(14));
                }
                else if((*inputBoard).getPropInfo(9).numHouses < (*inputBoard).getPropInfo(11).numHouses && (*inputBoard).getPropInfo(14).numHouses < (*inputBoard).getPropInfo(13).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(14));
                }
            }
            else if((*tempOwnedProp).colorGroup == ORANGE && orangeFlag == false){
                orangeFlag = true;
                 if((*inputBoard).getPropInfo(16).numHouses == (*inputBoard).getPropInfo(18).numHouses && (*inputBoard).getPropInfo(16).numHouses == (*inputBoard).getPropInfo(19).numHouses && (*inputBoard).getPropInfo(16).numHouses < 5){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    buildableProperties.push_back((*inputBoard).getPropAdd(16));
                    buildableProperties.push_back((*inputBoard).getPropAdd(18));
                    buildableProperties.push_back((*inputBoard).getPropAdd(19));
                }
                else if((*inputBoard).getPropInfo(16).numHouses < (*inputBoard).getPropInfo(18).numHouses && (*inputBoard).getPropInfo(16).numHouses < (*inputBoard).getPropInfo(19).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(16));
                }
                else if((*inputBoard).getPropInfo(16).numHouses < (*inputBoard).getPropInfo(18).numHouses && (*inputBoard).getPropInfo(16).numHouses == (*inputBoard).getPropInfo(19).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(16));
                    buildableProperties.push_back((*inputBoard).getPropAdd(19));
                }
                else if((*inputBoard).getPropInfo(16).numHouses < (*inputBoard).getPropInfo(19).numHouses && (*inputBoard).getPropInfo(16).numHouses == (*inputBoard).getPropInfo(18).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(16));
                    buildableProperties.push_back((*inputBoard).getPropAdd(18));
                }
                else if((*inputBoard).getPropInfo(18).numHouses < (*inputBoard).getPropInfo(19).numHouses && (*inputBoard).getPropInfo(18).numHouses < (*inputBoard).getPropInfo(16).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(18));
                }
                else if((*inputBoard).getPropInfo(18).numHouses < (*inputBoard).getPropInfo(16).numHouses && (*inputBoard).getPropInfo(18).numHouses == (*inputBoard).getPropInfo(19).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(18));
                    buildableProperties.push_back((*inputBoard).getPropAdd(19));
                }
                else if((*inputBoard).getPropInfo(19).numHouses < (*inputBoard).getPropInfo(16).numHouses && (*inputBoard).getPropInfo(19).numHouses < (*inputBoard).getPropInfo(18).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(19));
                }

            }
            else if((*tempOwnedProp).colorGroup == RED && redFlag == false){
                redFlag = true;
                 if((*inputBoard).getPropInfo(21).numHouses == (*inputBoard).getPropInfo(23).numHouses && (*inputBoard).getPropInfo(21).numHouses == (*inputBoard).getPropInfo(24).numHouses && (*inputBoard).getPropInfo(21).numHouses < 5){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    buildableProperties.push_back((*inputBoard).getPropAdd(21));
                    buildableProperties.push_back((*inputBoard).getPropAdd(23));
                    buildableProperties.push_back((*inputBoard).getPropAdd(24));
                }
                else if((*inputBoard).getPropInfo(21).numHouses < (*inputBoard).getPropInfo(23).numHouses && (*inputBoard).getPropInfo(21).numHouses < (*inputBoard).getPropInfo(24).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(21));
                }
                else if((*inputBoard).getPropInfo(21).numHouses < (*inputBoard).getPropInfo(23).numHouses && (*inputBoard).getPropInfo(21).numHouses == (*inputBoard).getPropInfo(24).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(21));
                    buildableProperties.push_back((*inputBoard).getPropAdd(24));
                }
                else if((*inputBoard).getPropInfo(21).numHouses < (*inputBoard).getPropInfo(24).numHouses && (*inputBoard).getPropInfo(21).numHouses == (*inputBoard).getPropInfo(23).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(21));
                    buildableProperties.push_back((*inputBoard).getPropAdd(23));
                }
                else if((*inputBoard).getPropInfo(23).numHouses < (*inputBoard).getPropInfo(24).numHouses && (*inputBoard).getPropInfo(23).numHouses < (*inputBoard).getPropInfo(21).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(23));
                }
                else if((*inputBoard).getPropInfo(23).numHouses < (*inputBoard).getPropInfo(21).numHouses && (*inputBoard).getPropInfo(23).numHouses == (*inputBoard).getPropInfo(24).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(23));
                    buildableProperties.push_back((*inputBoard).getPropAdd(24));
                }
                else if((*inputBoard).getPropInfo(24).numHouses < (*inputBoard).getPropInfo(21).numHouses && (*inputBoard).getPropInfo(24).numHouses < (*inputBoard).getPropInfo(23).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(24));
                }
            }
            else if((*tempOwnedProp).colorGroup == YELLOW && yellowFlag == false){
                yellowFlag = true;
                 if((*inputBoard).getPropInfo(26).numHouses == (*inputBoard).getPropInfo(27).numHouses && (*inputBoard).getPropInfo(26).numHouses == (*inputBoard).getPropInfo(29).numHouses && (*inputBoard).getPropInfo(26).numHouses < 5){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    buildableProperties.push_back((*inputBoard).getPropAdd(26));
                    buildableProperties.push_back((*inputBoard).getPropAdd(27));
                    buildableProperties.push_back((*inputBoard).getPropAdd(29));
                }
                else if((*inputBoard).getPropInfo(26).numHouses < (*inputBoard).getPropInfo(27).numHouses && (*inputBoard).getPropInfo(26).numHouses < (*inputBoard).getPropInfo(29).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(26));
                }
                else if((*inputBoard).getPropInfo(26).numHouses < (*inputBoard).getPropInfo(27).numHouses && (*inputBoard).getPropInfo(26).numHouses == (*inputBoard).getPropInfo(29).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(26));
                    buildableProperties.push_back((*inputBoard).getPropAdd(29));
                }
                else if((*inputBoard).getPropInfo(26).numHouses < (*inputBoard).getPropInfo(29).numHouses && (*inputBoard).getPropInfo(26).numHouses == (*inputBoard).getPropInfo(27).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(26));
                    buildableProperties.push_back((*inputBoard).getPropAdd(27));
                }
                else if((*inputBoard).getPropInfo(27).numHouses < (*inputBoard).getPropInfo(29).numHouses && (*inputBoard).getPropInfo(27).numHouses < (*inputBoard).getPropInfo(26).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(27));
                }
                else if((*inputBoard).getPropInfo(27).numHouses < (*inputBoard).getPropInfo(26).numHouses && (*inputBoard).getPropInfo(27).numHouses == (*inputBoard).getPropInfo(29).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(27));
                    buildableProperties.push_back((*inputBoard).getPropAdd(29));
                }
                else if((*inputBoard).getPropInfo(29).numHouses < (*inputBoard).getPropInfo(26).numHouses && (*inputBoard).getPropInfo(29).numHouses < (*inputBoard).getPropInfo(27).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(29));
                }
            }
            else if((*tempOwnedProp).colorGroup == GREEN && greenFlag == false){
                greenFlag = true;
                 if((*inputBoard).getPropInfo(31).numHouses == (*inputBoard).getPropInfo(32).numHouses && (*inputBoard).getPropInfo(31).numHouses == (*inputBoard).getPropInfo(34).numHouses && (*inputBoard).getPropInfo(31).numHouses < 5){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    buildableProperties.push_back((*inputBoard).getPropAdd(31));
                    buildableProperties.push_back((*inputBoard).getPropAdd(32));
                    buildableProperties.push_back((*inputBoard).getPropAdd(34));
                }
                else if((*inputBoard).getPropInfo(31).numHouses < (*inputBoard).getPropInfo(32).numHouses && (*inputBoard).getPropInfo(31).numHouses < (*inputBoard).getPropInfo(34).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(31));
                }
                else if((*inputBoard).getPropInfo(31).numHouses < (*inputBoard).getPropInfo(32).numHouses && (*inputBoard).getPropInfo(31).numHouses == (*inputBoard).getPropInfo(34).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(31));
                    buildableProperties.push_back((*inputBoard).getPropAdd(34));
                }
                else if((*inputBoard).getPropInfo(31).numHouses < (*inputBoard).getPropInfo(34).numHouses && (*inputBoard).getPropInfo(31).numHouses == (*inputBoard).getPropInfo(32).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(31));
                    buildableProperties.push_back((*inputBoard).getPropAdd(32));
                }
                else if((*inputBoard).getPropInfo(32).numHouses < (*inputBoard).getPropInfo(34).numHouses && (*inputBoard).getPropInfo(32).numHouses < (*inputBoard).getPropInfo(31).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(32));
                }
                else if((*inputBoard).getPropInfo(32).numHouses < (*inputBoard).getPropInfo(31).numHouses && (*inputBoard).getPropInfo(32).numHouses == (*inputBoard).getPropInfo(34).numHouses){
                    buildableProperties.push_back((*inputBoard).getPropAdd(32));
                    buildableProperties.push_back((*inputBoard).getPropAdd(34));
                }
                else if((*inputBoard).getPropInfo(34).numHouses < (*inputBoard).getPropInfo(31).numHouses && (*inputBoard).getPropInfo(9).numHouses < (*inputBoard).getPropInfo(32).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(34));
                }
            }
            else if((*tempOwnedProp).colorGroup == BLUE && blueFlag == false){
                blueFlag = true;
                if((*inputBoard).getPropInfo(37).numHouses == (*inputBoard).getPropInfo(39).numHouses && (*inputBoard).getPropInfo(37).numHouses < 5){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    buildableProperties.push_back((*inputBoard).getPropAdd(37));
                    buildableProperties.push_back((*inputBoard).getPropAdd(39));
                }
                else if((*inputBoard).getPropInfo(37).numHouses < (*inputBoard).getPropInfo(39).numHouses){ // Determines which has the least number of houses, can build on that one
                    buildableProperties.push_back((*inputBoard).getPropAdd(37));
                }
                else{
                    buildableProperties.push_back((*inputBoard).getPropAdd(39));
                }
            }
        }
    }

    if(noColorFlag){ // If the player doesn't own an entire color group
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYou must own all of the colors of a color group in order to build on them!\n" << endl;
        return -1;
    }
    if(buildableProperties.empty()){ // If none of the player's properties are buildable (at this point, it would be because the player already built a hotel (5 houses) on each property
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYou do not have any buildable properties! (You cannot build more than five houses on one property)\n" << endl;
        return -1;
    }

    cout << "What property would you like to build on?" << endl;

    /* Cycles through the properties on buildableProperties, giving letter options for the user to input their choice */
    for(int i = 0; i < buildableProperties.size(); i++){
        Property *tempOwnedProp;
        tempOwnedProp = buildableProperties.at(i); // Stores the address to the property
        cout << "     " << ('a' + i) << ") " << (*tempOwnedProp).propertyName << "  costs $" << (*tempOwnedProp).buildCost << " to build on. (" << (*tempOwnedProp).numHouses << " house(s) already built)" << endl;

        if(i+1 == buildableProperties.size()){ // Adds an option to not build as the last option
            cout << ('a' + i + 1) << ") Return to previous menu" << endl;
        }
    }

    Property *tempOwnedProp; // Creates a temporary property variable for later
    cin.get(chosenOption);

    while(!isalpha(chosenOption)){ // If the player doesn't enter a letter
        cout << "Please enter one of the above letters to choose a property!" << endl;
        cin.get(chosenOption);
    }

    if(chosenOption > buildableProperties.size()){ // If the player decides not to build
        clearscreen();
        (*inputBoard).drawBoard();

            cout << "You chose not to build" << endl;
            return -1;
    }

    if(islower(chosenOption) && chosenOption - 97 <= buildableProperties.size()){ // If the player inputs a lowercase letter && if the letter is one of the options
        tempOwnedProp = buildableProperties.at(chosenOption - 97);
    }
    else if(isupper(chosenOption) && chosenOption - 65 <= buildableProperties.size()){ // If the player inputs an uppercase letter && if the letter is one of the options
        tempOwnedProp = buildableProperties.at(chosenOption - 65);
    }

    Player *tempPropOwner; // Creates a temporary player value for upcoming calculations
    tempPropOwner = (*tempOwnedProp).propertyOwner;

    if((*tempOwnedProp).buildCost <= (*tempPropOwner).intWealth){ // If the player can afford to build at the chosen property
        (*tempOwnedProp).numHouses++;
        (*tempPropOwner).intWealth -= (*tempOwnedProp).buildCost;

        clearscreen();
        (*inputBoard).drawBoard();

        cout << (*tempOwnedProp).propertyName << " now has " << (*tempOwnedProp).numHouses << " houses!" << endl;
        return -1;
        }
    else { // If the player can't afford to build
        clearscreen();
        (*inputBoard).drawBoard();

        cout << "You don't have the funds to build on this property!" << endl;
        return -1;
    }
}

int sellHouse(GameBoard *inputBoard){
    if(ownedProperties.at(turnNum).empty()){ // If the player owns no properties
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYou don't have houses to sell!\n" << endl;
        return -1;
    }
    vector<Property*> removeableProperties;
    char chosenOption = 'z';

    bool noColorFlag = true; // Flags if the player owns no full color sets

    bool purpleFlag = false;
    bool skyFlag = false;
    bool pinkFlag = false;
    bool orangeFlag = false;
    bool redFlag = false;
    bool yellowFlag = false;
    bool greenFlag = false;
    bool blueFlag = false;

    /* This loop adds any properties (of the player's owned properties) that the player can sell houses from */
    for(int i = 0; i < ownedProperties.at(turnNum).size(); i++){

        Property *tempOwnedProp;
        tempOwnedProp = ownedProperties.at(turnNum).at(i); // Stores the address to the property

        if((*tempOwnedProp).ownAllColor){
            noColorFlag = false; // If the examined property's color group is owned by the player, then the flag is taken down
            if((*tempOwnedProp).colorGroup == PURPLE && purpleFlag == false){
                purpleFlag = true;
                if((*inputBoard).getPropInfo(1).numHouses == (*inputBoard).getPropInfo(3).numHouses && (*inputBoard).getPropInfo(1).numHouses > 0){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    removeableProperties.push_back((*inputBoard).getPropAdd(1));
                    removeableProperties.push_back((*inputBoard).getPropAdd(3));
                }
                else if((*inputBoard).getPropInfo(1).numHouses < (*inputBoard).getPropInfo(3).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(3));
                }
                else{
                    removeableProperties.push_back((*inputBoard).getPropAdd(1));
                }
            }
            else if((*tempOwnedProp).colorGroup == SKY && skyFlag ==false){
                skyFlag = true;
                if((*inputBoard).getPropInfo(6).numHouses == (*inputBoard).getPropInfo(8).numHouses && (*inputBoard).getPropInfo(6).numHouses == (*inputBoard).getPropInfo(9).numHouses && (*inputBoard).getPropInfo(6).numHouses > 0){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    removeableProperties.push_back((*inputBoard).getPropAdd(6));
                    removeableProperties.push_back((*inputBoard).getPropAdd(8));
                    removeableProperties.push_back((*inputBoard).getPropAdd(9));
                }
                else if((*inputBoard).getPropInfo(6).numHouses < (*inputBoard).getPropInfo(8).numHouses && (*inputBoard).getPropInfo(6).numHouses < (*inputBoard).getPropInfo(9).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(8));
                    removeableProperties.push_back((*inputBoard).getPropAdd(9));
                }
                else if((*inputBoard).getPropInfo(6).numHouses < (*inputBoard).getPropInfo(8).numHouses && (*inputBoard).getPropInfo(6).numHouses == (*inputBoard).getPropInfo(9).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(8));
                }
                else if((*inputBoard).getPropInfo(6).numHouses < (*inputBoard).getPropInfo(9).numHouses && (*inputBoard).getPropInfo(6).numHouses == (*inputBoard).getPropInfo(8).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(9));
                }
                else if((*inputBoard).getPropInfo(8).numHouses < (*inputBoard).getPropInfo(9).numHouses && (*inputBoard).getPropInfo(8).numHouses < (*inputBoard).getPropInfo(6).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(6));
                    removeableProperties.push_back((*inputBoard).getPropAdd(9));
                }
                else if((*inputBoard).getPropInfo(8).numHouses < (*inputBoard).getPropInfo(6).numHouses && (*inputBoard).getPropInfo(8).numHouses == (*inputBoard).getPropInfo(9).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(6));
                }
                else if((*inputBoard).getPropInfo(9).numHouses < (*inputBoard).getPropInfo(6).numHouses && (*inputBoard).getPropInfo(9).numHouses < (*inputBoard).getPropInfo(8).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(6));
                    removeableProperties.push_back((*inputBoard).getPropAdd(8));
                }
            }
            else if((*tempOwnedProp).colorGroup == PINK && pinkFlag == false){
                pinkFlag = true;
                if((*inputBoard).getPropInfo(11).numHouses == (*inputBoard).getPropInfo(13).numHouses && (*inputBoard).getPropInfo(11).numHouses == (*inputBoard).getPropInfo(14).numHouses && (*inputBoard).getPropInfo(11).numHouses > 0){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    removeableProperties.push_back((*inputBoard).getPropAdd(11));
                    removeableProperties.push_back((*inputBoard).getPropAdd(13));
                    removeableProperties.push_back((*inputBoard).getPropAdd(14));
                }
                else if((*inputBoard).getPropInfo(11).numHouses < (*inputBoard).getPropInfo(13).numHouses && (*inputBoard).getPropInfo(11).numHouses < (*inputBoard).getPropInfo(14).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(13));
                    removeableProperties.push_back((*inputBoard).getPropAdd(14));
                }
                else if((*inputBoard).getPropInfo(11).numHouses < (*inputBoard).getPropInfo(13).numHouses && (*inputBoard).getPropInfo(11).numHouses == (*inputBoard).getPropInfo(14).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(13));
                }
                else if((*inputBoard).getPropInfo(11).numHouses < (*inputBoard).getPropInfo(14).numHouses && (*inputBoard).getPropInfo(11).numHouses == (*inputBoard).getPropInfo(13).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(14));
                }
                else if((*inputBoard).getPropInfo(13).numHouses < (*inputBoard).getPropInfo(14).numHouses && (*inputBoard).getPropInfo(13).numHouses < (*inputBoard).getPropInfo(11).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(11));
                    removeableProperties.push_back((*inputBoard).getPropAdd(14));
                }
                else if((*inputBoard).getPropInfo(13).numHouses < (*inputBoard).getPropInfo(11).numHouses && (*inputBoard).getPropInfo(13).numHouses == (*inputBoard).getPropInfo(14).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(11));
                }
                else if((*inputBoard).getPropInfo(14).numHouses < (*inputBoard).getPropInfo(11).numHouses && (*inputBoard).getPropInfo(14).numHouses < (*inputBoard).getPropInfo(13).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(11));
                    removeableProperties.push_back((*inputBoard).getPropAdd(13));
                }
            }
            else if((*tempOwnedProp).colorGroup == ORANGE && orangeFlag == false){
                orangeFlag = true;
                if((*inputBoard).getPropInfo(16).numHouses == (*inputBoard).getPropInfo(18).numHouses && (*inputBoard).getPropInfo(16).numHouses == (*inputBoard).getPropInfo(19).numHouses && (*inputBoard).getPropInfo(16).numHouses > 0){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    removeableProperties.push_back((*inputBoard).getPropAdd(16));
                    removeableProperties.push_back((*inputBoard).getPropAdd(18));
                    removeableProperties.push_back((*inputBoard).getPropAdd(19));
                }
                else if((*inputBoard).getPropInfo(16).numHouses < (*inputBoard).getPropInfo(18).numHouses && (*inputBoard).getPropInfo(16).numHouses < (*inputBoard).getPropInfo(19).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(18));
                    removeableProperties.push_back((*inputBoard).getPropAdd(19));
                }
                else if((*inputBoard).getPropInfo(16).numHouses < (*inputBoard).getPropInfo(18).numHouses && (*inputBoard).getPropInfo(16).numHouses == (*inputBoard).getPropInfo(19).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(18));
                }
                else if((*inputBoard).getPropInfo(16).numHouses < (*inputBoard).getPropInfo(19).numHouses && (*inputBoard).getPropInfo(16).numHouses == (*inputBoard).getPropInfo(18).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(19));
                }
                else if((*inputBoard).getPropInfo(18).numHouses < (*inputBoard).getPropInfo(19).numHouses && (*inputBoard).getPropInfo(18).numHouses < (*inputBoard).getPropInfo(16).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(16));
                    removeableProperties.push_back((*inputBoard).getPropAdd(19));
                }
                else if((*inputBoard).getPropInfo(18).numHouses < (*inputBoard).getPropInfo(16).numHouses && (*inputBoard).getPropInfo(18).numHouses == (*inputBoard).getPropInfo(19).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(16));
                }
                else if((*inputBoard).getPropInfo(19).numHouses < (*inputBoard).getPropInfo(16).numHouses && (*inputBoard).getPropInfo(19).numHouses < (*inputBoard).getPropInfo(18).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(16));
                    removeableProperties.push_back((*inputBoard).getPropAdd(18));
                }

            }
            else if((*tempOwnedProp).colorGroup == RED && redFlag == false){
                redFlag = true;
                if((*inputBoard).getPropInfo(21).numHouses == (*inputBoard).getPropInfo(23).numHouses && (*inputBoard).getPropInfo(21).numHouses == (*inputBoard).getPropInfo(24).numHouses && (*inputBoard).getPropInfo(21).numHouses > 0){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    removeableProperties.push_back((*inputBoard).getPropAdd(21));
                    removeableProperties.push_back((*inputBoard).getPropAdd(23));
                    removeableProperties.push_back((*inputBoard).getPropAdd(24));
                }
                else if((*inputBoard).getPropInfo(21).numHouses < (*inputBoard).getPropInfo(23).numHouses && (*inputBoard).getPropInfo(21).numHouses < (*inputBoard).getPropInfo(24).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(23));
                    removeableProperties.push_back((*inputBoard).getPropAdd(24));
                }
                else if((*inputBoard).getPropInfo(21).numHouses < (*inputBoard).getPropInfo(23).numHouses && (*inputBoard).getPropInfo(21).numHouses == (*inputBoard).getPropInfo(24).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(23));
                }
                else if((*inputBoard).getPropInfo(21).numHouses < (*inputBoard).getPropInfo(24).numHouses && (*inputBoard).getPropInfo(21).numHouses == (*inputBoard).getPropInfo(23).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(24));
                }
                else if((*inputBoard).getPropInfo(23).numHouses < (*inputBoard).getPropInfo(24).numHouses && (*inputBoard).getPropInfo(23).numHouses < (*inputBoard).getPropInfo(21).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(21));
                    removeableProperties.push_back((*inputBoard).getPropAdd(24));
                }
                else if((*inputBoard).getPropInfo(23).numHouses < (*inputBoard).getPropInfo(21).numHouses && (*inputBoard).getPropInfo(23).numHouses == (*inputBoard).getPropInfo(24).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(21));
                }
                else if((*inputBoard).getPropInfo(24).numHouses < (*inputBoard).getPropInfo(21).numHouses && (*inputBoard).getPropInfo(24).numHouses < (*inputBoard).getPropInfo(23).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(21));
                    removeableProperties.push_back((*inputBoard).getPropAdd(23));
                }
            }
            else if((*tempOwnedProp).colorGroup == YELLOW && yellowFlag == false){
                yellowFlag = true;
                if((*inputBoard).getPropInfo(26).numHouses == (*inputBoard).getPropInfo(27).numHouses && (*inputBoard).getPropInfo(26).numHouses == (*inputBoard).getPropInfo(29).numHouses && (*inputBoard).getPropInfo(26).numHouses > 0){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    removeableProperties.push_back((*inputBoard).getPropAdd(26));
                    removeableProperties.push_back((*inputBoard).getPropAdd(27));
                    removeableProperties.push_back((*inputBoard).getPropAdd(29));
                }
                else if((*inputBoard).getPropInfo(26).numHouses < (*inputBoard).getPropInfo(27).numHouses && (*inputBoard).getPropInfo(26).numHouses < (*inputBoard).getPropInfo(29).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(28));
                    removeableProperties.push_back((*inputBoard).getPropAdd(29));
                }
                else if((*inputBoard).getPropInfo(26).numHouses < (*inputBoard).getPropInfo(27).numHouses && (*inputBoard).getPropInfo(26).numHouses == (*inputBoard).getPropInfo(29).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(27));
                }
                else if((*inputBoard).getPropInfo(26).numHouses < (*inputBoard).getPropInfo(29).numHouses && (*inputBoard).getPropInfo(26).numHouses == (*inputBoard).getPropInfo(27).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(29));
                }
                else if((*inputBoard).getPropInfo(27).numHouses < (*inputBoard).getPropInfo(29).numHouses && (*inputBoard).getPropInfo(27).numHouses < (*inputBoard).getPropInfo(26).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(26));
                    removeableProperties.push_back((*inputBoard).getPropAdd(29));
                }
                else if((*inputBoard).getPropInfo(27).numHouses < (*inputBoard).getPropInfo(26).numHouses && (*inputBoard).getPropInfo(27).numHouses == (*inputBoard).getPropInfo(29).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(26));
                }
                else if((*inputBoard).getPropInfo(29).numHouses < (*inputBoard).getPropInfo(26).numHouses && (*inputBoard).getPropInfo(29).numHouses < (*inputBoard).getPropInfo(27).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(26));
                    removeableProperties.push_back((*inputBoard).getPropAdd(27));
                }
            }
            else if((*tempOwnedProp).colorGroup == GREEN && greenFlag == false){
                greenFlag = true;
                if((*inputBoard).getPropInfo(31).numHouses == (*inputBoard).getPropInfo(32).numHouses && (*inputBoard).getPropInfo(31).numHouses == (*inputBoard).getPropInfo(34).numHouses && (*inputBoard).getPropInfo(31).numHouses > 0){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    removeableProperties.push_back((*inputBoard).getPropAdd(31));
                    removeableProperties.push_back((*inputBoard).getPropAdd(32));
                    removeableProperties.push_back((*inputBoard).getPropAdd(34));
                }
                else if((*inputBoard).getPropInfo(31).numHouses < (*inputBoard).getPropInfo(32).numHouses && (*inputBoard).getPropInfo(31).numHouses < (*inputBoard).getPropInfo(34).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(32));
                    removeableProperties.push_back((*inputBoard).getPropAdd(34));
                }
                else if((*inputBoard).getPropInfo(31).numHouses < (*inputBoard).getPropInfo(32).numHouses && (*inputBoard).getPropInfo(31).numHouses == (*inputBoard).getPropInfo(34).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(32));
                }
                else if((*inputBoard).getPropInfo(31).numHouses < (*inputBoard).getPropInfo(34).numHouses && (*inputBoard).getPropInfo(31).numHouses == (*inputBoard).getPropInfo(32).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(34));
                }
                else if((*inputBoard).getPropInfo(32).numHouses < (*inputBoard).getPropInfo(34).numHouses && (*inputBoard).getPropInfo(32).numHouses < (*inputBoard).getPropInfo(31).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(31));
                    removeableProperties.push_back((*inputBoard).getPropAdd(34));
                }
                else if((*inputBoard).getPropInfo(32).numHouses < (*inputBoard).getPropInfo(31).numHouses && (*inputBoard).getPropInfo(32).numHouses == (*inputBoard).getPropInfo(34).numHouses){
                    removeableProperties.push_back((*inputBoard).getPropAdd(31));
                }
                else if((*inputBoard).getPropInfo(34).numHouses < (*inputBoard).getPropInfo(31).numHouses && (*inputBoard).getPropInfo(34).numHouses < (*inputBoard).getPropInfo(32).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(31));
                    removeableProperties.push_back((*inputBoard).getPropAdd(32));
                }
            }
            else if((*tempOwnedProp).colorGroup == BLUE && blueFlag == false){
                blueFlag = true;
                if((*inputBoard).getPropInfo(37).numHouses == (*inputBoard).getPropInfo(39).numHouses && (*inputBoard).getPropInfo(37).numHouses > 0){ // If the properties have the same amount of houses and have less then 5, the user can build on both
                    removeableProperties.push_back((*inputBoard).getPropAdd(37));
                    removeableProperties.push_back((*inputBoard).getPropAdd(39));
                }
                else if((*inputBoard).getPropInfo(37).numHouses < (*inputBoard).getPropInfo(39).numHouses){ // Determines which has the least number of houses, can build on that one
                    removeableProperties.push_back((*inputBoard).getPropAdd(39));
                }
                else{
                    removeableProperties.push_back((*inputBoard).getPropAdd(37));
                }
            }
        }
    }

    if(noColorFlag){ // If the player doesn't own an entire color group
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYou don't have houses to sell!\n" << endl;
        return -1;
    }

    if(removeableProperties.empty()){ // If none of the player's properties have houses to sell
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYou do not have any houses to sell!\n" << endl;
        return -1;
    }

    cout << "What property would you like to sell from?" << endl;

    /* Cycles through the properties on removeableProperties, giving letter options for the user to input their choice */
    for(int i = 0; i < removeableProperties.size(); i++){
        Property *tempOwnedProp;
        tempOwnedProp = removeableProperties.at(i); // Stores the address to the property
        cout << "     " << ('a' + i) << ") " << (*tempOwnedProp).propertyName << "  pays $" << ((*tempOwnedProp).buildCost / 2) << " to sell. (" << (*tempOwnedProp).numHouses << " house(s) built)" << endl;

        if(i+1 == removeableProperties.size()){ // Adds an option to not build as the last option
            cout << ('a' + i + 1) << ") Return to previous menu." << endl;
        }
    }

    Property *tempOwnedProp; // Creates a temporary property variable for later
    cin.get(chosenOption);

    while(!isalpha(chosenOption)){ // If the player doesn't enter a letter
        cout << "Please enter one of the above letters to choose a property!" << endl;
        cin.get(chosenOption);
    }

    if(chosenOption > removeableProperties.size()){ // If the player decides not to build
        clearscreen();
        (*inputBoard).drawBoard();

            cout << "You chose not to sell." << endl;
            return -1;
    }

    if(islower(chosenOption) && chosenOption - 97 <= removeableProperties.size()){ // If the player inputs a lowercase letter && if the letter is one of the options
        tempOwnedProp = removeableProperties.at(chosenOption - 97);
    }
    else if(isupper(chosenOption)&& chosenOption - 65 <= removeableProperties.size()){ // If the player inputs an uppercase letter && if the letter is one of the options
        tempOwnedProp = removeableProperties.at(chosenOption - 65);
    }

    Player *tempPropOwner; // Creates a temporary player value for upcoming calculations
    tempPropOwner = (*tempOwnedProp).propertyOwner;

    (*tempOwnedProp).numHouses--;
    (*tempPropOwner).intWealth += ((*tempOwnedProp).buildCost / 2);

    clearscreen();
    (*inputBoard).drawBoard();

    cout << (*tempOwnedProp).propertyName << " now has " << (*tempOwnedProp).numHouses << " houses." << endl;
    return -1;



}

int offerTrade(GameBoard *inputBoard){
    int intTradeWealth = 0;
    bool tradeOutJail = false;
    vector<Property*> propsToOffer;
    vector<Property*> propsToAsk;
    Player *playerToTrade; // A pointer to the player that the current player wants to trade with
    char chosenOption = '1';

    cout << "\nWhich player would you like to trade with?\n" << endl;
    /* Cycles through the players, omitting the current player and lists them as menu options for the player to choose */
    for(int i = 0, j = 0; i < numPlayers; i++, j++){
        if(!gamePlayers[i].boolBankrupt){ // if the player is bankrupt, he or she is skipped
            char aChar = ('a' + j); // This char is to increment the option list (i.e. a), b), c), etc.

            if(i == turnNum){ // Omits the current player
                i++;
            }

            cout << "     " << aChar << ") " << gamePlayers[i].playerName << endl; // Prints out a list of the other players
        }
        else{ // If the player is bankrupt, this will keep the lettering from being skipped
            j--;
        }
         /* Adds an option to not trade */
        if(j + 1 == (numPlayers - 1)){
            char bChar = ('a' + j + 1);
            cout << "     " << bChar << ") Return to previous menu." << endl;
        }
    }

    /* If the player doesn't enter a letter */
    while(!isalpha(chosenOption)){
        cout << "\nPlease enter one of the above letters to choose a player: ";
        cin >> chosenOption;
    }

     /* If the player chose not to trade */
    if((chosenOption == (numPlayers + 96) && islower(chosenOption)) || (chosenOption == (numPlayers + 64) && isupper(chosenOption))){ // If the player decides not to trade
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYou chose not to trade.\n" << endl;
        return -1;
    }


    /* This sequence of if statement determines which player the player chose, and stores that player into the above declared pointer variable */
    if(islower(chosenOption) && turnNum <= (chosenOption - 97)){
        playerToTrade = &gamePlayers[chosenOption - 96];
    }
    else if(islower(chosenOption) && turnNum > (chosenOption - 97)){
        playerToTrade = &gamePlayers[chosenOption - 97];
    }
    else if(isupper(chosenOption) && turnNum <= (chosenOption - 64)){
        playerToTrade = &gamePlayers[chosenOption  -64];
    }
    else if(isupper(chosenOption) && turnNum > (chosenOption - 97)){
        playerToTrade = &gamePlayers[chosenOption - 65];
    }
    chosenOption = '1';

    clearscreen();
    (*inputBoard).drawBoard();

    /* This loop continuously gives the player an option to add more to the trade until he doesn't want to add more */
    while(chosenOption != 'e' && chosenOption != 'E'){
        chosenOption =  '1';
        displayOwnedProps();
        displayPlayerStats(inputBoard, &gamePlayers[turnNum]);

        /* Displays Current Trade information */
        if(1==1){
        cout << "\nCurrent Trade:" << endl;
        cout << "     Offering: " << endl;

        cout << "          Properties: ";
        /* Prints out the properties that the player is proposing to offer */
        for(int i  = 0; i < propsToOffer.size(); i++){
            cout << (*propsToOffer[i]).propertyName;
            if((i+1) < propsToOffer.size()){
                cout << ", ";
            }
            else{ // If the current property in iteration is the last in the list
                cout << ". ";
            }
        }

        cout << "\n          Wealth: $";
        if(intTradeWealth > 0){
            cout << intTradeWealth << endl;
        }
        else{
            cout << 0 << endl;
        }
        if(tradeOutJail == true && gamePlayers[turnNum].outJailFree && !(*playerToTrade).outJailFree){
            cout << "          Get out of Jail Free Card" << endl;
        }

        cout << "\n     Asking for: " << endl;
        cout << "          Properties: ";

        /* Prints out the properties that the player is asking for */
        for(int i  = 0; i < propsToAsk.size(); i++){
            cout << (*propsToAsk[i]).propertyName;
            if((i+1) < propsToAsk.size()){
                cout << ", ";
            }
            else{ // If the current property in iteration is the last in the list
                cout << ". ";
            }
        }

        cout << "\n          Wealth: $";
        if(intTradeWealth < 0){
            cout << (0 - intTradeWealth) << endl;
        }
        else{
            cout << 0 << endl;
        }
        if(tradeOutJail == true && !gamePlayers[turnNum].outJailFree && (*playerToTrade).outJailFree){
            cout << "          Get out of Jail Free Card" << endl;
        }

        }

        cout << "\nWhat would you like to trade?" << endl;
        cout << "     a) Wealth" << endl;
        cout << "     b) Offer properties" << endl;
        cout << "     c) Ask for properties" << endl;

        /* If the player has opted to trade the get out of jail free card, then it can be removed from the table */
        if(tradeOutJail){
        cout << "     d) Don't trade get out of jail card" << endl;
        cout << "\nElse:" << endl;
        cout << "     e) Offer trade" << endl;
        cout << "     f) Decide not to trade" << endl;

        while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'c' && chosenOption != 'd' &&
           chosenOption != 'e' && chosenOption != 'f' && chosenOption != 'A'&& chosenOption != 'B' &&
           chosenOption != 'C' && chosenOption != 'D' && chosenOption != 'E' && chosenOption != 'F'){
            cout << "\n\n Please enter a, b, c, d, e, or f: ";
            cin >> chosenOption;
        }

        if(chosenOption == 'd' || chosenOption == 'D'){
            tradeOutJail = false;
            clearscreen();
            (*inputBoard).drawBoard();
        }

        if(chosenOption == 'f' || chosenOption == 'F'){
            clearscreen();
            (*inputBoard).drawBoard();

            cout << "\nYou chose not to trade. \n" << endl;
            return -1;
        }

    }

        /* If the player doesn't have a get out of jail free card, and the other player does, then it adds trade as an option */
        else if(!gamePlayers[turnNum].outJailFree && (*playerToTrade).outJailFree){
        cout << "     d) Ask for " << (*playerToTrade).playerName << "'s get out of jail card" << endl;
        cout << "\nElse:" << endl;
        cout << "     e) Offer trade" << endl;
        cout << "     f) Decide not to trade" << endl;

        while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'c' && chosenOption != 'd' &&
           chosenOption != 'e' && chosenOption != 'f' && chosenOption != 'A'&& chosenOption != 'B' &&
            chosenOption != 'C' && chosenOption != 'D' && chosenOption != 'E' && chosenOption != 'F'){
            cout << "\n\n Please enter a, b, c, d, e, or f: ";
            cin >> chosenOption;
        }

        if(chosenOption == 'd' || chosenOption == 'D'){
            tradeOutJail = true;
            clearscreen();
            (*inputBoard).drawBoard();
        }

        if(chosenOption == 'f' || chosenOption == 'F'){
            clearscreen();
            (*inputBoard).drawBoard();

            cout << "\nYou chose not to trade. \n" << endl;
            return -1;
        }

    }

        /* If the player has a get out of jail free card, and the other player doesn't, then it adds trade as an option */
        else if(gamePlayers[turnNum].outJailFree && !(*playerToTrade).outJailFree){
        cout << "     d) Offer to trade your get out of jail card" << endl;
        cout << "\nElse:" << endl;
        cout << "     e) Offer trade" << endl;
        cout << "     f) Decide not to trade" << endl;

        while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'c' && chosenOption != 'd' &&
           chosenOption != 'e' && chosenOption != 'f' && chosenOption != 'A'&& chosenOption != 'B' &&
           chosenOption != 'C' && chosenOption != 'D' && chosenOption != 'E' && chosenOption != 'F'){
            cout << "\n\n Please enter a, b, c, d, e, or f: ";
            cin >> chosenOption;
        }

        if(chosenOption == 'd' || chosenOption == 'D'){
            tradeOutJail = true;
            clearscreen();
            (*inputBoard).drawBoard();
        }

        if(chosenOption == 'f' || chosenOption == 'F'){
            clearscreen();
            (*inputBoard).drawBoard();

            cout << "\nYou chose not to trade. \n" << endl;
            return -1;
        }
    }

        /* If trading get out of jail cards isn't an option */
        else {
        cout << "\nElse:" << endl;
        cout << "     d) Offer trade" << endl;
        cout << "     e) Decide not to trade" << endl;

        while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'c' && chosenOption != 'd' &&
           chosenOption != 'e' && chosenOption != 'A'&& chosenOption != 'B' &&
           chosenOption != 'C' && chosenOption != 'D' && chosenOption != 'E'){
            cout << "\n\n Please enter a, b, c, d, or e: ";
            cin >> chosenOption;
        }


        if(chosenOption == 'e' || chosenOption == 'E'){
            clearscreen();
            (*inputBoard).drawBoard();

            cout << "\nYou chose not to trade. \n" << endl;
            return -1;
        }

        if(chosenOption == 'd' || chosenOption == 'D'){
            chosenOption++;
        }

    }


        switch(chosenOption){
            case 'a':
            case 'A':   clearscreen();
                        (*inputBoard).drawBoard();
                        intTradeWealth = tradeWealth(inputBoard, &gamePlayers[turnNum]);
                        clearscreen();
                        (*inputBoard).drawBoard();
                        break;
            case 'b':
            case 'B':   if(ownedProperties.at(turnNum).empty()){ // If the player owns no properties
                            clearscreen();
                            (*inputBoard).drawBoard();
                            cout << "\nYou don't have properties to offer!\n" << endl;
                            break;
                        }
                        else{
                            offerProperties(inputBoard, turnNum, &propsToOffer);
                            break;
                        }

            case 'c':
            case 'C':  if(ownedProperties.at((*playerToTrade).playerNum).empty()){
                            clearscreen();
                            (*inputBoard).drawBoard();
                            cout << "\n" << (*playerToTrade).playerName << " doesn't have any properties to trade!\n" << endl;
                            break;
                        }
                        else{
                            askForProperties(inputBoard, (*playerToTrade).playerNum, &propsToAsk);
                            break;
                        }

        }

    }

    chosenOption = '-1';
    clearscreen();
    (*inputBoard).drawBoard();

    /* This is for the other player to accept, reject, or offer a counteroffer to the trade */
    while(chosenOption != 'b' || chosenOption != 'B'){
        chosenOption = '-1';

        cout << "\nIt is " << (*playerToTrade).playerName << "'s turn to accept, reject, or give a counteroffer to " << gamePlayers[turnNum].playerName << "'s offer!\n" << endl;
        displayOwnedProps();
        displayPlayerStats(inputBoard,playerToTrade);

        /* Displays the offered trade */
        if(1==1){
            cout << "\nOffered Trade:" << endl;
            cout << "     Offering: " << endl;

            cout << "          Properties: ";
            /* Prints out the properties that the player is proposing to offer */
            for(int i  = 0; i < propsToOffer.size(); i++){
                cout << (*propsToOffer[i]).propertyName;
                if((i+1) < propsToOffer.size()){
                    cout << ", ";
                }
                else{ // If the current property in iteration is the last in the list
                    cout << ". ";
                }
            }

            cout << "\n          Wealth: $";
            if(intTradeWealth > 0){
                cout << intTradeWealth << endl;
            }
            else{
                cout << 0 << endl;
            }
            if(tradeOutJail == true && gamePlayers[turnNum].outJailFree && !(*playerToTrade).outJailFree){
                cout << "          Get out of Jail Free Card" << endl;
            }

            cout << "\n     Asking for: " << endl;
            cout << "          Properties: ";

            /* Prints out the properties that the player is asking for */
            for(int i  = 0; i < propsToAsk.size(); i++){
                cout << (*propsToAsk[i]).propertyName;
                if((i+1) < propsToAsk.size()){
                    cout << ", ";
                }
                else{ // If the current property in iteration is the last in the list
                    cout << ". ";
                }
            }

            cout << "\n          Wealth: $";
            if(intTradeWealth < 0){
                cout << (0 - intTradeWealth) << endl;
            }
            else{
                cout << 0 << endl;
            }
            if(tradeOutJail == true && !gamePlayers[turnNum].outJailFree && (*playerToTrade).outJailFree){
                cout << "          Get out of Jail Free Card" << endl;
            }

            }

        cout << "\nWhat would you like to do?" << endl;
        cout << "     a) Accept offer" << endl;
        cout << "     b) Reject offer" << endl;
        cout << "     c) Create counteroffer" << endl;

        while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'c' && chosenOption != 'A'
              && chosenOption != 'B' && chosenOption != 'C'){
            cout << "\n\n Please enter a, b, or c: ";
            cin >> chosenOption;
        }

        /* If the player accepts the trade */
        if(chosenOption == 'a' || chosenOption == 'A'){
            if(((*playerToTrade).intWealth - (0 - intTradeWealth)) < 0){ // If the other player asks for more money than the player can give
                clearscreen();
                (*inputBoard).drawBoard();
                cout << "\nYou cannot accept this offer, you do not have enough wealth!" << endl;
                chosenOption = '1';
            }
            else{ // This makes all of the adjustments of the trade
                clearscreen();
                (*inputBoard).drawBoard();
                cout << (*playerToTrade).playerName << " accepted your offer!" << endl;
                for(int i = 0; i < propsToAsk.size(); i++){ // Transfers asked for props
                    (*propsToAsk[i]).propertyOwner = &gamePlayers[turnNum];
                }
                for(int i = 0; i < propsToOffer.size(); i++){ // Transfers offered props
                    (*propsToOffer[i]).propertyOwner = playerToTrade;
                }
                if(tradeOutJail){ // If the get out of jail free card is being traded, this is where the switch happens
                    if((*playerToTrade).outJailFree){
                        (*playerToTrade).outJailFree = false;
                        gamePlayers[turnNum].outJailFree = true;
                    }
                    else{
                        (*playerToTrade).outJailFree = true;
                        gamePlayers[turnNum].outJailFree = false;
                    }
                }

                (*playerToTrade).intWealth += intTradeWealth; // Exchanges any wealth to be traded
                gamePlayers[turnNum].intWealth -= intTradeWealth;

                calculateOwnedProps(inputBoard); // recalculates the properties ownership
                return -1;
            }
        }

        /* If the player rejects the offer */
        if(chosenOption == 'b' || chosenOption == 'B'){
            clearscreen();
            (*inputBoard).drawBoard();

            cout << "\nYou rejected the offer. \n" << endl;
            return -1;
            }

        /* If the player wants to create a counteroffer */
        if(chosenOption == 'c' || chosenOption == 'C'){
            clearscreen();
            (*inputBoard).drawBoard();
            propsToAsk.clear();
            propsToOffer.clear();
            intTradeWealth = 0;
            tradeOutJail = false;

            chosenOption = '-1';
            /* This loop continuously gives the player an option to add more to the counteroffer until he doesn't want to add more */
            while(chosenOption != 'e' && chosenOption != 'E'){
                chosenOption =  '1';
                displayOwnedProps();
                displayPlayerStats(inputBoard, &gamePlayers[turnNum]);

                /* Displays Current CounterOffer information */
                if(1==1){
                    cout << "\nCurrent CounterOffer:" << endl;
                    cout << "     Offering: " << endl;

                    cout << "          Properties: ";
                    /* Prints out the properties that the player is proposing to offer */
                    for(int i  = 0; i < propsToOffer.size(); i++){
                        cout << (*propsToOffer[i]).propertyName;
                        if((i+1) < propsToOffer.size()){
                            cout << ", ";
                        }
                        else{ // If the current property in iteration is the last in the list
                            cout << ". ";
                        }
                    }

                    cout << "\n          Wealth: $";
                    if(intTradeWealth > 0){
                        cout << intTradeWealth << endl;
                    }
                    else{
                        cout << 0 << endl;
                    }
                    if(tradeOutJail == true && gamePlayers[turnNum].outJailFree && !(*playerToTrade).outJailFree){
                        cout << "          Get out of Jail Free Card" << endl;
                    }

                    cout << "\n     Asking for: " << endl;
                    cout << "          Properties: ";

                    /* Prints out the properties that the player is asking for */
                    for(int i  = 0; i < propsToAsk.size(); i++){
                        cout << (*propsToAsk[i]).propertyName;
                        if((i+1) < propsToAsk.size()){
                            cout << ", ";
                        }
                        else{ // If the current property in iteration is the last in the list
                            cout << ". ";
                        }
                    }

                    cout << "\n          Wealth: $";
                    if(intTradeWealth < 0){
                        cout << (0 - intTradeWealth) << endl;
                    }
                    else{
                        cout << 0 << endl;
                    }
                    if(tradeOutJail == true && !gamePlayers[turnNum].outJailFree && (*playerToTrade).outJailFree){
                        cout << "          Get out of Jail Free Card" << endl;
                    }

                }

                cout << "\nWhat would you like to trade as your counteroffer?" << endl;
                cout << "     a) Wealth" << endl;
                cout << "     b) Offer properties" << endl;
                cout << "     c) Ask for properties" << endl;

                /* If the player has opted to trade the get out of jail free card, then it can be removed from the table */
                if(tradeOutJail){
                    cout << "     d) Don't trade get out of jail card" << endl;
                    cout << "\nElse:" << endl;
                    cout << "     e) Offer counteroffer" << endl;
                    cout << "     f) Decide not to make a counteroffer" << endl;

                    while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'c' && chosenOption != 'd' &&
                       chosenOption != 'e' && chosenOption != 'f' && chosenOption != 'A'&& chosenOption != 'B' &&
                       chosenOption != 'C' && chosenOption != 'D' && chosenOption != 'E' && chosenOption != 'F'){
                        cout << "\n\n Please enter a, b, c, d, e, or f: ";
                        cin >> chosenOption;
                    }

                    if(chosenOption == 'd' || chosenOption == 'D'){
                        tradeOutJail = false;
                        clearscreen();
                        (*inputBoard).drawBoard();
                    }

                    if(chosenOption == 'f' || chosenOption == 'F'){
                        clearscreen();
                        (*inputBoard).drawBoard();

                        cout << "\nYou chose not to make a counteroffer. \n" << endl;
                        return -1;
                    }

                }

                /* If the player doesn't have a get out of jail free card, and the other player does, then it adds trade as an option */
                else if(gamePlayers[turnNum].outJailFree && !(*playerToTrade).outJailFree){
                    cout << "     d) Ask for " << gamePlayers[turnNum].playerName << "'s get out of jail card" << endl;
                    cout << "\nElse:" << endl;
                    cout << "     e) Offer counteroffer" << endl;
                    cout << "     f) Decide not to make a counteroffer" << endl;

                    while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'c' && chosenOption != 'd' &&
                       chosenOption != 'e' && chosenOption != 'f' && chosenOption != 'A'&& chosenOption != 'B' &&
                        chosenOption != 'C' && chosenOption != 'D' && chosenOption != 'E' && chosenOption != 'F'){
                        cout << "\n\n Please enter a, b, c, d, e, or f: ";
                        cin >> chosenOption;
                    }

                    if(chosenOption == 'd' || chosenOption == 'D'){
                        tradeOutJail = true;
                        clearscreen();
                        (*inputBoard).drawBoard();
                    }

                    if(chosenOption == 'f' || chosenOption == 'F'){
                        clearscreen();
                        (*inputBoard).drawBoard();

                        cout << "\nYou chose not to make a counteroffer. \n" << endl;
                        return -1;
                    }

                }

                /* If the player has a get out of jail free card, and the other player doesn't, then it adds trade as an option */
                else if(!gamePlayers[turnNum].outJailFree && (*playerToTrade).outJailFree){
                    cout << "     d) Offer to trade your get out of jail card" << endl;
                    cout << "\nElse:" << endl;
                    cout << "     e) Offer counteroffer" << endl;
                    cout << "     f) Decide not to make a counteroffer" << endl;

                    while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'c' && chosenOption != 'd' &&
                       chosenOption != 'e' && chosenOption != 'f' && chosenOption != 'A'&& chosenOption != 'B' &&
                       chosenOption != 'C' && chosenOption != 'D' && chosenOption != 'E' && chosenOption != 'F'){
                        cout << "\n\n Please enter a, b, c, d, e, or f: ";
                        cin >> chosenOption;
                    }

                    if(chosenOption == 'd' || chosenOption == 'D'){
                        tradeOutJail = true;
                        clearscreen();
                        (*inputBoard).drawBoard();
                    }

                    if(chosenOption == 'f' || chosenOption == 'F'){
                        clearscreen();
                        (*inputBoard).drawBoard();

                        cout << "\nYou chose not to make a counteroffer. \n" << endl;
                        return -1;
                    }
                }

                /* If trading get out of jail cards isn't an option */
                else {
                    cout << "\nElse:" << endl;
                    cout << "     d) Offer counteroffer" << endl;
                    cout << "     e) Decide not make a counteroffer" << endl;

                    while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'c' && chosenOption != 'd' &&
                       chosenOption != 'e' && chosenOption != 'A'&& chosenOption != 'B' &&
                       chosenOption != 'C' && chosenOption != 'D' && chosenOption != 'E'){
                        cout << "\n\n Please enter a, b, c, d, or e: ";
                        cin >> chosenOption;
                    }


                    if(chosenOption == 'e' || chosenOption == 'E'){
                        clearscreen();
                        (*inputBoard).drawBoard();

                        cout << "\nYou chose not to make a counteroffer. \n" << endl;
                        return -1;
                    }

                    if(chosenOption == 'd' || chosenOption == 'D'){
                        chosenOption++;
                    }

                }


                switch(chosenOption){
                    case 'a':
                    case 'A':   clearscreen();
                                (*inputBoard).drawBoard();
                                intTradeWealth = tradeWealth(inputBoard, playerToTrade);
                                clearscreen();
                                (*inputBoard).drawBoard();
                                break;
                    case 'b':
                    case 'B':   if(ownedProperties.at((*playerToTrade).playerNum).empty()){ // If the player owns no properties
                                    clearscreen();
                                    (*inputBoard).drawBoard();
                                    cout << "\nYou don't have properties to offer!\n" << endl;
                                    break;
                                }
                                else{
                                    offerProperties(inputBoard, (*playerToTrade).playerNum, &propsToOffer);
                                    break;
                                }

                    case 'c':
                    case 'C':  if(ownedProperties.at(turnNum).empty()){
                                    clearscreen();
                                    (*inputBoard).drawBoard();
                                    cout << "\n" << gamePlayers[turnNum].playerName << " doesn't have any properties to trade!\n" << endl;
                                    break;
                                }
                                else{
                                    askForProperties(inputBoard, turnNum, &propsToAsk);
                                    break;
                                }

                }

            }

            /* This is for the  player to accept or reject a counteroffer */
            chosenOption = '-1';
            clearscreen();
            (*inputBoard).drawBoard();
            cout << "\nIt is " << gamePlayers[turnNum].playerName << "'s turn to accept or reject " << (*playerToTrade).playerName << "'s counteroffer!\n" << endl;
            displayOwnedProps();
            displayPlayerStats(inputBoard,playerToTrade);

            /* Displays the counteroffer */
            if(1==1){
                cout << "\nOffered Counteroffer:" << endl;
                cout << "     Offering: " << endl;

                cout << "          Properties: ";
                /* Prints out the properties that the player is proposing to offer */
                for(int i  = 0; i < propsToOffer.size(); i++){
                    cout << (*propsToOffer[i]).propertyName;
                    if((i+1) < propsToOffer.size()){
                        cout << ", ";
                    }
                    else{ // If the current property in iteration is the last in the list
                        cout << ". ";
                    }
                }

                cout << "\n          Wealth: $";
                if(intTradeWealth > 0){
                    cout << intTradeWealth << endl;
                }
                else{
                    cout << 0 << endl;
                }
                if(tradeOutJail == true && gamePlayers[turnNum].outJailFree && !(*playerToTrade).outJailFree){
                    cout << "          Get out of Jail Free Card" << endl;
                }

                cout << "\n     Asking for: " << endl;
                cout << "          Properties: ";

                /* Prints out the properties that the player is asking for */
                for(int i  = 0; i < propsToAsk.size(); i++){
                    cout << (*propsToAsk[i]).propertyName;
                    if((i+1) < propsToAsk.size()){
                        cout << ", ";
                    }
                    else{ // If the current property in iteration is the last in the list
                        cout << ". ";
                    }
                }

                cout << "\n          Wealth: $";
                if(intTradeWealth < 0){
                    cout << (0 - intTradeWealth) << endl;
                }
                else{
                    cout << 0 << endl;
                }
                if(tradeOutJail == true && !gamePlayers[turnNum].outJailFree && (*playerToTrade).outJailFree){
                    cout << "          Get out of Jail Free Card" << endl;
                }

            }

            cout << "\nWhat would you like to do?" << endl;
            cout << "     a) Accept counteroffer" << endl;
            cout << "     b) Reject counteroffer" << endl;

            while(chosenOption != 'a' && chosenOption != 'b' && chosenOption != 'A' && chosenOption != 'B'){
                cout << "\n\n Please enter a or b: ";
                cin >> chosenOption;
            }

            /* If the player accepts the counteroffer */
            if(chosenOption == 'a' || chosenOption == 'A'){
                if((gamePlayers[turnNum].intWealth - (0 - intTradeWealth)) < 0){ // If the other player asks for more money than the player can give
                    clearscreen();
                    (*inputBoard).drawBoard();
                    cout << "\nYou cannot accept this counteroffer, you do not have enough wealth!" << endl;
                    chosenOption = '1';
                }
                else{ // This makes all of the adjustments of the trade
                    clearscreen();
                    (*inputBoard).drawBoard();
                    cout << gamePlayers[turnNum].playerName << " accepted the counteroffer!" << endl;
                    for(int i = 0; i < propsToAsk.size(); i++){ // Transfers asked for props
                        (*propsToAsk[i]).propertyOwner = playerToTrade;
                    }
                    for(int i = 0; i < propsToOffer.size(); i++){ // Transfers offered props
                        (*propsToOffer[i]).propertyOwner = &gamePlayers[turnNum];
                    }
                    if(tradeOutJail){ // If the get out of jail free card is being traded, this is where the switch happens
                        if((*playerToTrade).outJailFree){
                            (*playerToTrade).outJailFree = false;
                            gamePlayers[turnNum].outJailFree = true;
                        }
                        else{
                            (*playerToTrade).outJailFree = true;
                            gamePlayers[turnNum].outJailFree = false;
                        }
                    }

                    (*playerToTrade).intWealth -= intTradeWealth; // Exchanges any wealth to be traded
                    gamePlayers[turnNum].intWealth += intTradeWealth;

                    calculateOwnedProps(inputBoard); // recalculates the properties ownership
                    return -1;
                }
            }

            /* If the player rejects the counteroffer */
            if(chosenOption == 'b' || chosenOption == 'B'){
                clearscreen();
                (*inputBoard).drawBoard();

                cout << "\nYou rejected the counteroffer. \n" << endl;
                return -1;
            }

        }

    }

}

int offerProperties(GameBoard *inputBoard, int whichPlayer, vector<Property*> *inputOfferProp){
    char offerPropChoice = '1';
    vector<Property*> potentOfferProps;

    /* This cycles through the properties that the player owns */
    for(int i = 0; i < ownedProperties.at(whichPlayer).size(); i++){
        Property *tempOwnedProp;
        tempOwnedProp = ownedProperties.at(whichPlayer).at(i); // Stores the address to the property

        /* If the player has already chosen to offer a property, then it will not appear as an option to be offered again */
        for(int i2 = 0; (*inputOfferProp).size(); i2++){
            if((*tempOwnedProp).propertyName != (*(*inputOfferProp)[i2]).propertyName){
            potentOfferProps.push_back(tempOwnedProp);
            }
        }
    }

    cout << "\nWhat property would you like to offer?" << endl;

    /* Lists the properties that a player can offer */
    for(int i = 0; i < potentOfferProps.size(); i++){

        cout << "     " << ('a' + i) << ") " << (*potentOfferProps.at(i)).propertyName << endl;

        if(i+1 == potentOfferProps.size()){ // Adds an option to not trade properties as the last option
            cout << ('a' + i + 1) << ") Return to previous menu" << endl;
        }
    }

    Property *tempOfferedProp; // Creates a temporary property variable for later
    cin >> offerPropChoice;

    while(!isalpha(offerPropChoice)){ // If the player doesn't enter a letter
        cout << "Please enter one of the above letters to choose a property!" << endl;
        cin >> offerPropChoice;
    }

    if(offerPropChoice > potentOfferProps.size()){ // If the player decides not to trade properties
            cout << "You chose not to offer any properties" << endl;
            return -1;
    }

    if(islower(offerPropChoice) && offerPropChoice - 97 <= potentOfferProps.size()){ // If the player inputs a lowercase letter && if the letter is one of the options
        tempOfferedProp = potentOfferProps.at(offerPropChoice - 97);
    }
    else if(isupper(offerPropChoice) && offerPropChoice - 65 <= potentOfferProps.size()){ // If the player inputs an uppercase letter && if the letter is one of the options
        tempOfferedProp = potentOfferProps.at(offerPropChoice - 65);
    }

    (*inputOfferProp).push_back(tempOfferedProp);
    return -1;

}

int askForProperties(GameBoard *inputBoard, int whichPlayer, vector<Property*> *inputAskProp){
    char askPropChoice = '1';
    vector<Property*> potentAskProps;

    /* This cycles through the properties that the other player owns */
    for(int i = 0; i < ownedProperties.at(whichPlayer).size(); i++){
        Property *tempOwnedProp;
        tempOwnedProp = ownedProperties.at(whichPlayer).at(i); // Stores the address to the property

        /* If the player has already chosen to ask for a property, then it will not appear as an option to be asked for again */
        for(int i2 = 0; (*inputAskProp).size(); i2++){
            if((*tempOwnedProp).propertyName != (*(*inputAskProp)[i2]).propertyName){
            potentAskProps.push_back(tempOwnedProp);
            }
        }
    }

    cout << "\nWhat property would you like to ask for?" << endl;

    /* Lists the properties that a player can ask for */
    for(int i = 0; i < potentAskProps.size(); i++){

        cout << "     " << ('a' + i) << ") " << (*potentAskProps.at(i)).propertyName << endl;

        if(i+1 == potentAskProps.size()){ // Adds an option to not trade properties as the last option
            cout << ('a' + i + 1) << ") Return to previous menu" << endl;
        }
    }

    Property *tempAskedProp; // Creates a temporary property variable for later
    cin >> askPropChoice;

    while(!isalpha(askPropChoice)){ // If the player doesn't enter a letter
        cout << "Please enter one of the above letters to choose a property!" << endl;
        cin >> askPropChoice;
    }

    if(askPropChoice > potentAskProps.size()){ // If the player decides not to trade properties
            cout << "You chose not to ask for any properties" << endl;
            return -1;
    }

    if(islower(askPropChoice) && askPropChoice - 97 <= potentAskProps.size()){ // If the player inputs a lowercase letter && if the letter is one of the options
        tempAskedProp = potentAskProps.at(askPropChoice - 97);
    }
    else if(isupper(askPropChoice) && askPropChoice - 65 <= potentAskProps.size()){ // If the player inputs an uppercase letter && if the letter is one of the options
        tempAskedProp = potentAskProps.at(askPropChoice - 65);
    }

    (*inputAskProp).push_back(tempAskedProp);
    return -1;

}

/* If the player asks for money, returns a negative number */
int tradeWealth(GameBoard *inputBoard, Player *inputPlayer){
    char choseWealthTrade = '1';
    int intTradeWealth;

    cout << "\nHow would you like to trade with wealth?" << endl;
    cout << "     a) Ask for wealth" << endl;
    cout << "     b) Offer wealth" << endl;
    cout << "     c) Do not trade with wealth\n" << endl;


    while(choseWealthTrade != 'a' && choseWealthTrade != 'b' &&  choseWealthTrade != 'c' && choseWealthTrade != 'A' && choseWealthTrade != 'B' && choseWealthTrade != 'C'){
        cout << " Please choose a, b, or c: ";
        cin >> choseWealthTrade;
    }


    /* If the player chooses to ask for money */
    if(choseWealthTrade == 'a' || choseWealthTrade == 'A'){
        cout << "\n\n How much wealth do you want to ask for? $";
        cin >> intTradeWealth;

        while(intTradeWealth < 0){
            cout << "\n\n Please input a positive number: $";
            cin >> intTradeWealth;
        }
    }

    /* If the player chooses to offer money */
    else if(choseWealthTrade == 'b' ||  choseWealthTrade == 'B') {
        cout << "\n\n How much wealth do you want to offer? $";
        cin >> intTradeWealth;

        /* This loop is responsible for getting the player to input a number that is positive and less than their total wealth */
        while(intTradeWealth < 0 || intTradeWealth > (*inputPlayer).intWealth){
            if(intTradeWealth < 0){
                cout << "\n\nPlease input a positive number: $";
                cin >> intTradeWealth;
            }

            if(intTradeWealth > ((*inputPlayer).intWealth)){
                cout << "\n\nYou don't have that much wealth! Please offer an amount you can afford: $";
                cin >> intTradeWealth;
            }
        }
    }

    switch(choseWealthTrade){
        case 'a':
        case 'A': return (0 - intTradeWealth);

        case 'b':
        case 'B': return intTradeWealth;

        default: cout << "\nYou did not want to trade wealth.\n";
                return 0;
    }

}

int mortgageProp(GameBoard *inputBoard, Player *inputPlayer){
    if(ownedProperties.at(turnNum).empty()){ // If the player owns no properties
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYou don't have properties to mortgage!\n" << endl;
        return -1;
    }


    Property *tempMortProp;

    char chosenMortgageOption = '-1';
    cout << "\nWhat property would you like to mortgage?" << endl;
     /* This cycles through the properties that the player owns */
    for(int i = 0; i < ownedProperties.at((*inputPlayer).playerNum).size(); i++){
       if ((*ownedProperties.at((*inputPlayer).playerNum).at(i)).inMortgage){ // If the property is already in mortgage, then it won't be added
        i++;
       }
       else if((*ownedProperties.at((*inputPlayer).playerNum).at(i)).numHouses > 0){ // If the property has houses, it won't be added
        i++;
       }

       cout << "     " << ('a' + i) << ") " << (*ownedProperties.at((*inputPlayer).playerNum).at(i)).propertyName << " can be mortgaged for $" << (*ownedProperties.at((*inputPlayer).playerNum).at(i)).mortgageValue << endl;

        if(i+1 == ownedProperties.at((*inputPlayer).playerNum).size()){ // Adds an option to not build as the last option
            cout << ('a' + i + 1) << ") Return to previous menu" << endl;
        }
    }

    cin.get(chosenMortgageOption);

    while(!isalpha(chosenMortgageOption)){ // If the player doesn't enter a letter
        cout << "Please enter one of the above letters to choose a property!" << endl;
        cin.get(chosenMortgageOption);
    }

    if(chosenMortgageOption > ownedProperties.at((*inputPlayer).playerNum).size()){ // If the player decides not to build
        clearscreen();
        (*inputBoard).drawBoard();

            cout << "You chose not to mortgage any properties" << endl;
            return -1;
    }

    if(islower(chosenMortgageOption) && chosenMortgageOption - 97 <= ownedProperties.at((*inputPlayer).playerNum).size()){ // If the player inputs a lowercase letter && if the letter is one of the options
        tempMortProp = ownedProperties.at((*inputPlayer).playerNum).at(chosenMortgageOption - 97);
    }
    else if(isupper(chosenMortgageOption) && chosenMortgageOption - 65 <= ownedProperties.at((*inputPlayer).playerNum).size()){ // If the player inputs an uppercase letter && if the letter is one of the options
        tempMortProp = ownedProperties.at((*inputPlayer).playerNum).at(chosenMortgageOption - 65);
    }

    (*inputPlayer).intWealth += (*tempMortProp).mortgageValue;
    (*tempMortProp).inMortgage = true;

    clearscreen();
    (*inputBoard).drawBoard();
    cout << "/nYou Mortgaged " << (*tempMortProp).propertyName << " for  $" << (*tempMortProp).mortgageValue << endl;
    return -1;

}

int unmortgageProp(GameBoard *inputBoard, Player *inputPlayer){
    if(ownedProperties.at(turnNum).empty()){ // If the player owns no properties
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYou don't have properties to lift a mortgage from!\n" << endl;
        return -1;
    }

    Property *tempMortProp;

    char chosenMortgageOption = '-1';
    cout << "\nWhat property would you like to lift a mortgage from?" << endl;
     /* This cycles through the properties that the player owns */
    for(int i = 0; i < ownedProperties.at((*inputPlayer).playerNum).size(); i++){
        if (!(*ownedProperties.at((*inputPlayer).playerNum).at(i)).inMortgage){ // If the property is not in mortgage, then it won't be added
            i++;
        }

        if(!i == ownedProperties.at((*inputPlayer).playerNum).size()){
            cout << "     " << ('a' + i) << ") You can lift " << (*ownedProperties.at((*inputPlayer).playerNum).at(i)).propertyName << "'s mortgage for $" << ((*ownedProperties.at((*inputPlayer).playerNum).at(i)).mortgageValue + (0.10 * (*ownedProperties.at((*inputPlayer).playerNum).at(i)).mortgageValue)) << endl;
        }


        if(i == ownedProperties.at((*inputPlayer).playerNum).size()){ // Adds an option to not build as the last option
            cout << "     a) Return to previous menu" << endl;
        }
        else if(i + 1 == ownedProperties.at((*inputPlayer).playerNum).size()){
            cout << "     " << ('a' + i + 1) << ") Return to previous menu" << endl;
        }
    }

    cin.get(chosenMortgageOption);

    while(!isalpha(chosenMortgageOption)){ // If the player doesn't enter a letter
        cout << "\nPlease enter one of the above letters to choose a property:" << endl;
        cin.get(chosenMortgageOption);
    }

    if(chosenMortgageOption > ownedProperties.at((*inputPlayer).playerNum).size()){ // If the player decides not to build
        clearscreen();
        (*inputBoard).drawBoard();

            cout << "You chose not to lift any mortgages." << endl;
            return -1;
    }

    if(islower(chosenMortgageOption) && chosenMortgageOption - 97 <= ownedProperties.at((*inputPlayer).playerNum).size()){ // If the player inputs a lowercase letter && if the letter is one of the options
        tempMortProp = ownedProperties.at((*inputPlayer).playerNum).at(chosenMortgageOption - 97);
    }
    else if(isupper(chosenMortgageOption) && chosenMortgageOption - 65 <= ownedProperties.at((*inputPlayer).playerNum).size()){ // If the player inputs an uppercase letter && if the letter is one of the options
        tempMortProp = ownedProperties.at((*inputPlayer).playerNum).at(chosenMortgageOption - 65);
    }

    if((*inputPlayer).intWealth < (*tempMortProp).mortgageValue + (0.10 * (*tempMortProp).mortgageValue)){
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYou can't afford to lift this mortgage!" << endl;
        return -1;
    }
    else {
        (*inputPlayer).intWealth -= ((*tempMortProp).mortgageValue + (0.10 * (*tempMortProp).mortgageValue));
        (*tempMortProp).inMortgage = false;
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "/nYou lifted  " << (*tempMortProp).propertyName << "'s mortgage for  $" << ((*tempMortProp).mortgageValue + (0.10 * (*tempMortProp).mortgageValue)) << endl;
        return -1;
    }
}

int avoidBankruptcy(GameBoard *inputBoard, Player *inputPlayer){
    int chosenBankruptOption = '-1';
    cout << "\nWhat would you like to do?" << endl;
    cout << "     a) Mortgage Properties" << endl;
    cout << "     b) Sell Houses" << endl;
    cout << "     c) Offer trade" << endl;
    cout << "     d) Declare Bankruptcy" << endl;

    while(chosenBankruptOption != 'a' && chosenBankruptOption != 'b'&& chosenBankruptOption != 'c' && chosenBankruptOption != 'd'&& chosenBankruptOption != 'A' && chosenBankruptOption != 'B' && chosenBankruptOption != 'C' && chosenBankruptOption != 'D'){
        cout << "\nPlease choose a, b, c, or d: ";
        cin >> chosenBankruptOption;
    }

    /* If the player decides to mortgage properties */
    if(chosenBankruptOption == 'a' || chosenBankruptOption == 'A'){
        mortgageProp(inputBoard, inputPlayer);
        return 1;
    }

    /* If the player decides to sell a house */
    if(chosenBankruptOption == 'b' || chosenBankruptOption == 'B'){
        sellHouse(inputBoard);
        return 1;
    }

    /* If the player decides to trade */
    if(chosenBankruptOption == 'c' || chosenBankruptOption == 'C'){
        offerTrade(inputBoard);
        return 1;
    }

    /* If the player declares bankruptcy */
    if(chosenBankruptOption == 'd' || chosenBankruptOption == 'D'){
        clearscreen();
        (*inputBoard).drawBoard();
        cout << gamePlayers[turnNum].playerName << " declared bankruptcy." << endl;
        gamePlayers[turnNum].boolBankrupt = true;
        bankRuptcyProcedure(inputBoard, inputPlayer);
        return -1;
    }
}

int bankRuptcyProcedure(GameBoard *inputBoard, Player *inputPlayer){
    Player *playerLostTo;
    char bankruptChosenOption = '1';
    cout << "\nAre you going bankrupt to a player or the bank?" << endl;
    cout << "     a) Bank " << endl;
    cout << "     b) Player" << endl;

    while(bankruptChosenOption != 'a' && bankruptChosenOption != 'b' && bankruptChosenOption != 'A' && bankruptChosenOption != 'B'){
        cout << "\nPlease enter a or b: ";
        cin >> bankruptChosenOption;
    }

    if(bankruptChosenOption == 'a' || bankruptChosenOption == 'A'){
        clearscreen();
        (*inputBoard).drawBoard();
        cout << "\nYour properties will be auctioned to the highest bidder:" << endl;
        for(int i = 0; i < ownedProperties.at((*inputPlayer).playerNum).size(); i++){
            (*ownedProperties.at((*inputPlayer).playerNum).at(i)).numHouses = 0;
            auctionProperty(inputBoard, ownedProperties.at((*inputPlayer).playerNum).at(i));
        }
        clearscreen();
        (*inputBoard).drawBoard();
        return -1;

    }
    if(bankruptChosenOption == 'b' || bankruptChosenOption == 'B'){ // If the player is bankrupted by another player, then the other player gets all of the player's assets
        bankruptChosenOption = '-1';
        cout << "\nWhich player?\n" << endl;
        /* Cycles through the players, omitting the current player and lists them as menu options for the player to choose */
        for(int i = 0, j = 0; i < numPlayers; i++, j++){
            if(!gamePlayers[i].boolBankrupt){ // if the player is bankrupt, he or she is skipped
                char aChar = ('a' + j); // This char is to increment the option list (i.e. a), b), c), etc.

                if(i == turnNum){ // Omits the current player
                    i++;
                }

                cout << "     " << aChar << ") " << gamePlayers[i].playerName << endl; // Prints out a list of the other players
            }
            else{ // If the player is bankrupt, this will keep the lettering from being skipped
                j--;
            }

        }

        /* If the player doesn't enter a letter */
        while(!isalpha(bankruptChosenOption)){
            cout << "\nPlease enter one of the above letters to choose a player: ";
            cin >> bankruptChosenOption;
        }

        /* This sequence of if statement determines which player the player chose, and stores that player into the above declared pointer variable */
        if(islower(bankruptChosenOption) && turnNum <= (bankruptChosenOption - 97)){
            playerLostTo = &gamePlayers[bankruptChosenOption - 96];
        }
        else if(islower(bankruptChosenOption) && turnNum > (bankruptChosenOption - 97)){
            playerLostTo = &gamePlayers[bankruptChosenOption - 97];
        }
        else if(isupper(bankruptChosenOption) && turnNum <= (bankruptChosenOption - 64)){
            playerLostTo = &gamePlayers[bankruptChosenOption  -64];
        }
        else if(isupper(bankruptChosenOption) && turnNum > (bankruptChosenOption - 97)){
            playerLostTo = &gamePlayers[bankruptChosenOption - 65];
        }

        for(int i = 0; i < ownedProperties.at((*inputPlayer).playerNum).size(); i++){
            (*ownedProperties.at((*inputPlayer).playerNum).at(i)).numHouses = 0;
            (*ownedProperties.at((*inputPlayer).playerNum).at(i)).propertyOwner = playerLostTo;
        }

        (*playerLostTo).intWealth += (*inputPlayer).intWealth;

        if((*inputPlayer).outJailFree){
            (*playerLostTo).outJailFree = true;
        }
        clearscreen();
        (*inputBoard).drawBoard();
        return -1;
    }


}

int auctionProperty(GameBoard *inputBoard, Property *inputProperty){
    char chosenAuctionOption = '-1';
    int currentOffer = 0;
    int highestOffer = 0;
    Player *biddingPlayer;
    bool mainKeepBidding = false;
    bool firstRound = true;

    cout << (*inputProperty).propertyName << " is up for auction!" << endl;
    cout << "Bidding is starting at $5." << endl;

    do{ // If the players keep bidding, the auction will keep offering the chance for the bidding players to continue
        mainKeepBidding = false;
        clearscreen();
        (*inputBoard).drawBoard();

        cout << (*inputProperty).propertyName << " is up for auction!" << endl;
        if(firstRound){
            cout << "Bidding is starting at $5." << endl;
        }

        for(int i = 0; i < numPlayers; i++){ // This loop cycles through the players if anyone wants to bid
            cout << "\nCurrent Bid: $" << highestOffer << endl;
            if(!gamePlayers[i].boolBankrupt && (gamePlayers[i].boolKeepbidding || firstRound)){
                cout << gamePlayers[i].playerName << "'s turn: bid on property? " << endl;
                cout << "     a) Yes" << endl;
                cout << "     b) No " << endl;

                while(chosenAuctionOption != 'a' && chosenAuctionOption != 'b' && chosenAuctionOption != 'A' && chosenAuctionOption != 'B'){
                    cout << "\nPlease enter a or b: ";
                    cin >> chosenAuctionOption;
                }


                if(chosenAuctionOption == 'a' || chosenAuctionOption == 'A'){
                    cout << "\nHow much would you like to bid?  $";
                    cin >> currentOffer;

                    while(gamePlayers[i].intWealth < currentOffer){
                        cout << "You cannot afford to bid that! Please input an affordable amount: $";
                        cin >> currentOffer;
                    }
                    if(currentOffer > highestOffer){ // If the player offers the highest bid
                        highestOffer = currentOffer;
                        biddingPlayer = &gamePlayers[i];
                    }
                    gamePlayers[i].boolKeepbidding = true;
                    mainKeepBidding = true;
                }
                if(chosenAuctionOption == 'b' || chosenAuctionOption == 'B'){
                    gamePlayers[i].boolKeepbidding = false;
                }
            }
    chosenAuctionOption = '-1';
    }
    firstRound = false;

    } while(mainKeepBidding);

    clearscreen();
    (*inputBoard).drawBoard();
    if(highestOffer != 0){
        cout << (*biddingPlayer).playerName << " won the auction for $" << highestOffer << "!" << endl;
        (*inputProperty).ownedProperty = true;
        (*inputProperty).propertyOwner = biddingPlayer;
        (*biddingPlayer).intWealth -= highestOffer;
        return -1;
    }
    else{
        cout << "No one bid on the property." << endl;
        (*inputProperty).ownedProperty = false;
        return -1;
    }

}

int rollDie(){
    srand(time(0));
    return ((rand() % 11) + 2);
}

bool rollDoubles(){
    int roll;
    srand(time(0));
    roll = ((rand() % 36));
    if(roll >= 7){
        return false;
    }
    else{
        return true;
    }
}

void clearscreen(void)
{
    system("cls");
}



