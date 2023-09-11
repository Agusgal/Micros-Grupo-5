/***************************************************************************/ /**
  @file     data_base.h
  @brief    Application functions
  @author   Grupo 2
 ******************************************************************************/

#ifndef DATA_BASE_H_
#define DATA_BASE_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define ID_ARRAY_SIZE 8
#define PIN_ARRAY_SIZE 5
#define MAX_NUM_USERS 20
#define MAX_CARD_NUMBER 19
#define DEFAULT_CARD_CARACTER -1
#define NO_INPUT_CHAR -1
#define BACKSPACE -2
#define MAX_NUM_ATTEMPTS 3


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum
{
    VALIDATE_SUCCESSFULL,
    DATABASE_FULL,
    ID_ALREADY_EXISTS,
    CARD_NUMBER_EXISTS,
    ID_WRONG_FORMAT,
    PIN_WRONG_FORMAT,
    CARD_NUMBER_WRONG_FORMAT,
    STORE_SUCCESSFULL,
    DELETE_SUCCESSFULL,
    ID_NOT_FOUND,
    WRONG_PIN_VERIFICATION,
    PIN_CHANGED_SUCCESFULLY
} Status;
typedef enum
{
    USER,
    ADMIN
} hierarchy_t;

typedef struct
{
    int8_t userID[ID_ARRAY_SIZE];
    int8_t userPIN[PIN_ARRAY_SIZE];
    int8_t cardNumber[MAX_CARD_NUMBER];
    hierarchy_t typeOfUser;
    uint8_t Attempts;
} user_t;

typedef struct
{
    user_t userList[MAX_NUM_USERS];
    uint8_t lastItem;
} dataBase_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief This fuction takes care of setting dataBase.lastItem to -1 and the array blockedUsersIndexes with default value, 
 * further it creates 3 users examples and stores them in the database
 */
void initializeDataBase(void);

/**
 * @brief This function is in charge of verifying if the id that is passed as a parameter is stored in the database.
 * @param userID, the id that is going to be checked 
 * @return true if the userID is stored in the database or false if is not.
 */
bool verifyID(int8_t userID[]);

/**
 * @brief This function is in charge of verifying if the pin that is passed as a parameter is stored in the 
 * database and if it corresponds to the id passed 
 * @param userPIN, the pin that is going to be checked 
 * @return true if the userPIN is stored in the database and is the pin of the id passed, or false if is not.
 */
bool verifyPIN(int8_t userPIN[]);

/**
 * @brief This function returns true is the current user is locked or false if is not
 * @return true if the user is bloqued or false if is not.
 */
bool isCurrentUserBlocked(void);

/**
 * @brief This function returns the number of attempts of the current user
 * @return the number of attempts made by the current user
 */
uint8_t AttemptsOnCurrent(void);

/**
 * @brief This is used in a time callback to unblock (pop) the fist user added to the BlockedUsers array
 */
void UnblockUser(void);

/**
 * @brief This function returns true is the current user is Admin or false if is not
 * @return true if the user is Admin or false if is not.
 */
bool IsAdmin(void);

/**
 * @brief This function takes care of verify the data passed as a parameter and if the validation was fine
 * then stores the data passed on a new user.
 * @param userID, the id of the new user that is going to be stored
 * @param userPIN, the pin of the new user that is going to be stored
 * @param cardNumber, the cardNumber of the new user that is going to be stored
 * @param numCharactersCardNumber, the numbers of characters of the card number of the new user that is going to be stored
 * @param typeOfUser, the type of the new user
 * @return a variable with Status type, wich could be "STORE_SUCCESSFULL", "DATABASE_FULL", "PIN_WRONG_FORMAT" or "CARD_NUMBER_EXISTS", 
 */
Status checkAddUser(int8_t userID[], int8_t userPIN[], int8_t cardNumber[], uint8_t numCharactersCardNumber, hierarchy_t typeOfUser);

/**
 * @brief This function takes care of verify the data passed as a parameter to store the data on a new user
 * @param userID, the id of the new user that is going to be stored
 * @param userPIN, the pin of the new user that is going to be stored
 * @param cardNumber, the cardNumber of the new user that is going to be stored
 * @param numCharactersCardNumber, the numbers of characters of the card number of the new user that is going to be stored
 * @param typeOfUser, the type of the new user
 * @return a variable with Status type, wich could be "VALIDATE_SUCCESSFULL", "DATABASE_FULL", "PIN_WRONG_FORMAT" or "CARD_NUMBER_EXISTS", 
 */
Status validateAll(int8_t userID[], int8_t userPIN[], int8_t cardNumber[], uint8_t numCharactersCardNumber, hierarchy_t typeOfUser);

/**
 * @brief Check Id Format and availability
 * @param userID, the id that will be validated
 * @return a variable with Status type, wich could be "VALIDATE_SUCCESSFULL", "ID_ALREADY_EXISTS" or "ID_WRONG_FORMAT" 
 */
Status validateNewId(int8_t userID[]);

/**
 * @brief this function Changes the pin of the current user
 * @param the new pin
 * @return a variable with Status type, wich could be "PIN_CHANGED_SUCCESFULLY" or "PIN_WRONG_FORMAT" 
 */
Status changePin(int8_t* userNewPin);

/**
 * @brief This function is in charge of verifying if the Card number that is passed as a parameter is stored in the database.
 * @param cardNumber, the cardNumber that is going to be checked 
 * @param numCharactersCardNumber, number of characters of the card number passed
 * @return true if the cardNumber is stored in the database or false if is not.
 */
bool verifyCardNumber(int8_t cardNumber[], uint8_t numCharactersCardNumber);

/**
 * @brief this function removes an user from the database
 * @param the userID to be removed
 * @return a variable with Status type, wich could be "DELETE_SUCCESSFULL" or "ID_NOT_FOUND" 
 */
Status removeUser(int8_t userID[]);

/**
 * @brief this function returns the id of a user with the cardNumber that is passed as a parameter
 * @param cardNumber to find the user id
 * @return the user id or NULL if the cardNumber is not on the database
 */
int8_t * getIdByCardNumber(int8_t cardNumber[]);

/**
 * @brief Checks if the "pin" array format matches the format of a pin. The array must be complete (length equal to PIN_ARRAY_SIZE) and all elements must be numbers from 0 to 9.4
 * @param userPIN, The pin array.
 * @return A bool indicating if the format is valid or not.
 */
bool checkPinArrayFormat(int8_t userPIN[]);


#endif /* DATABASE_H_ */
