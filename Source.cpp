#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <vld.h>


#include <iostream>
#include <cstring>
#include <conio.h>
#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include "../InvalidInputFunctions.h"

using namespace std;


#pragma region Declarations
//Struct that contains information about a stock
struct Stock;

//This function runs the core of the program by looping the menu and deleting the allocated memory before it exits
void Run(bool &run);

//This function displays the main menu to the user then gets the users input and calls the appropriate functions
void DisplayMenu(bool &run, Stock**& stocks, int &numStocks);

//This function gets a stock from the user then allocates memory to keep that stock
void GetStock(Stock**& allStocks, int &numStocks);

//This function will display the users current stocks, or say that they do not have any
void DisplayStocks(Stock**& stocks, int numStocks);

//This function will sort the stocks in alphabetic order
void SortStocks(Stock**& stocks, int first, int last);

//This function will take two stocks and swap their m_Values
void SwapStocks(Stock*& original, Stock*& newStock);

//This function will return true and set an index if a stock is found with the given m_pSymbol, otherwise it returns false
bool Search(Stock**& scope, const int len, char* find, int &index);

//This function is used to edit the m_Value of a stock
void EditStock(Stock**& stocks, int numStocks);

//This function will get the index of a stock and return -1 if the stock was not found or will return the index 
int GetStockIndex(Stock**& stocks, int numStocks);

//This function will delete a stock from an array of stock pointers
void DeleteStock(Stock**& stocks, int &numStocks);
#pragma endregion

int main()
{
	bool run = true;			//Variable used to determine if program should run
	do
	{
		Run(run);				//Runs the program
	} while (run);				//Loops program if run is true

	return 0;
}

#pragma region Definitions
/***************************************************
* Purpose: Holds the contents of a stock defined as
*			having a m_pSymbol, m_pName, and m_Value where
*			the m_pSymbol and m_pName are char*'s
*	
* Constructors: Set the m_pSymbol and m_pName to nullptr
*				and m_Value to 0
*
* Destructors: Delete the allocated memory for the
*				m_pSymbol and m_pName
***************************************************/
struct Stock
{
	char* m_pSymbol;		//This will hold the stock's m_pSymbol which will be dynamically allocated
	char* m_pName;			//This will hold the stock's m_pName which will be dynamically allocated
	double m_Value;		//This holds the m_Value of the stock

	/************************************
	* Purpose: Inititializes pointers to
	*		nullptr
	************************************/
	Stock::Stock() : m_pSymbol(nullptr), m_pName(nullptr), m_Value(0)
	{
	};

	/***********************************
	* Purpose: Deletes the allocated
	*		memory for m_pSymbol and m_pName
	***********************************/
	Stock::~Stock()
	{
		delete[] m_pSymbol;
		delete[] m_pName;
	}
};

/*************************************************************
* Purpose: Will take two stocks and swap their m_Values, used 
*		for sorting
*************************************************************/
void SwapStocks(Stock*& original, Stock*& newStock)
{
	char* temp = nullptr;								//A temporary char* to hold a m_pSymbol
	char* tempName = nullptr;							//A temporary char* to hold a m_pName
	int tempVal = -1;									//A temporary int that holds a m_Value

	delete[] temp;									//Deletes temp for safety purposes
	delete[] tempName;								//Deletes tempNames ^^^
	temp = new char[strlen(original->m_pSymbol) + 1];				//Sets temp to a new char[] that is the length of the original m_pSymbol + 1(for '\0')
	strcpy(temp, original->m_pSymbol);								//Copies the contents of the original m_pSymbol into temp
	tempName = new char[strlen(original->m_pName) + 1];			//Sets tempm_pName to a new char[] that is the length of the original m_pName + 1(for '\0')
	strcpy(tempName, original->m_pName);							//Copies the contents of the original m_pName into tempm_pName
	tempVal = original->m_Value;									//Sets the tempm_Value to the original m_Value

	delete original;								//Delete the original stock freeing up the memory
	original = new Stock;							//Allocate new memory for the original stock

	original->m_pSymbol = new char[strlen(newStock->m_pSymbol) + 1];	//Sets the original stocks m_pSymbol to a new char[] that is the length of the new stocks m_pSymbol + 1(for '\0')
	strcpy(original->m_pSymbol, newStock->m_pSymbol);					//Copies the contents of the newStock's m_pSymbol to the original stock's m_pSymbol
	original->m_pName = new char[strlen(newStock->m_pName) + 1];		//Sets the original stocks m_pName to a new char[] that is the length of the new stocks m_pName + 1(for '\0')
	strcpy(original->m_pName, newStock->m_pName);						//Copies the contents of the new stocks m_pName into the original stocks m_pName
	original->m_Value = newStock->m_Value;							//Sets the m_Value of the original stock to the m_Value of the new stock


	delete newStock;								//Delete the new stock freeing up the memory ( now that the original stock is equal to it )
	newStock = new Stock;							//Allocate new memory for the newStock (which we will be setting to the original's(temps) m_Values

	newStock->m_pSymbol = new char[strlen(temp) + 1];				//Allocates memory for the new stock's m_pSymbol equal to enough to hold the length of temp + 1(for '\0')
	strcpy(newStock->m_pSymbol, temp);								//Copies the contents of temp(what the original m_pSymbol was) into the new stock's m_pSymbol
	newStock->m_pName = new char[strlen(tempName) + 1];			//Allocates memory for the new stock's m_pName to hold the length of tempm_pName + 1(for '\0')
	strcpy(newStock->m_pName, tempName);							//Copies the contents of tempm_pName(what the original m_pName was) into the new stock's m_pName
	newStock->m_Value = tempVal;									//Sets the new stocks m_Value to tempm_Value(the original m_Value)

	delete[] temp;									//Deallocate the memory for temp
	delete[] tempName;								//Deallocate the memory for tempm_pName
}

/**********************************************************************
* Purpose: Sorts stocks to be in a alphabetic order given the pointer
*	array of stocks, the index to start at and the index to end atS
**********************************************************************/
void SortStocks(Stock**& stocks, int first, int last)
{
	int pivot;					//Variable to store the pivot point
	int j;						//Variable j which will represent a higher index in general
	int i;						//Variavle i which will represent the lower index in general
	
	if (first < last)						//If first is less than last(then the start index is lower than the end index so proceed)
	{	
		pivot = first;				//Set pivot equal to the m_Value of first
		i = first;					//Set i = to the m_Value of first
		j = last;					//Set j = to the m_Value of last

		while (i < j)																				//While i < j( used to loop and determine when to swap
		{
			while (strcmp(stocks[i]->m_pSymbol, stocks[pivot]->m_pSymbol) <= 0 && i < last)						//If the m_pSymbol of stocks[i] <= the m_pSymbol of stocks[pivot] then the m_pSymbols are either equal or i comes first so add 1 to i as long as i < last
				i++;
			while (strcmp(stocks[j]->m_pSymbol, stocks[pivot]->m_pSymbol) > 0)									//If the m_pSymbol of stocks[j] > stocks[pivot] then stocks[j] comes after i so subtract 1 from j
				j--;
			
			if (i < j)
			{
				SwapStocks(stocks[i], stocks[j]);												//If i is still less than j than the two stocks need to be swapped so swap them
			}
		}

		if(stocks[pivot]->m_pSymbol != stocks[j]->m_pSymbol)				//As long as sticks[pivot] is not equal to stocks[j] then swap, the check is there to prevent issues with memory
			SwapStocks(stocks[pivot], stocks[j]);
		SortStocks(stocks, first, j - 1);							//Call this function with the original first m_Value but a new last m_Value
		SortStocks(stocks, j + 1, last);							//Call this function with a new first m_Value but the original last m_Value
	}
	
}

/**************************************************************
* Purpose: Runs the main program by storing the stocks and the
*			number of stocks. Loops the menu of options for 
*			the user and before exiting deallocates the memory
*			that was used by stocks
**************************************************************/
void Run(bool &run)
{
	int numStocks = 0;								//Variable to hold the number of stocks initialized to 0
	Stock** stocks = new Stock*[numStocks];			//Allocates memory for an array of Stock pointers

	do
	{
		DisplayMenu(run, stocks, numStocks);		//Displays the menu to the user and loops
	} while (run);
	
	for (int i = 0; i < numStocks; i++)
	{
		delete stocks[i];							//Deallocates memory for each stock
	}
	delete stocks;									//Deallocates memory for the stocks pointer
	
}

/**********************************************************************
* Purpose: Displays a menu of options to the user and gets their input
*		then calls the appropriate functions based on what the user
*		has chosen or reprompts the user if they entered an invalid 
*		choice
**********************************************************************/
void DisplayMenu(bool &run, Stock**& stocks, int &numStocks)
{
	char choice = ' ';					//Variable to hold the user's choice
	bool valid = true;					//Variable used to determine if input was valid

	do
	{
		system("cls");							//Clears the screen then displays the menu to the user
		cout << "---Menu---\n\n"
			<< "1) Add Stock\n"
			<< "2) Display Stocks\n"
			<< "3) Edit Stock\n"
			<< "4) Delete Stock\n"
			<< "5) Exit\n\n\n"
			<< "Enter Choice: ";				//Prompts user for choice
		ClearCin();	
		cin >> choice;							//Gets the user's choice

		switch (choice)											//Switches on the m_Value of choice
		{
		case '1':
			valid = true;									
			GetStock(stocks, numStocks);
			DisplayStocks(stocks, numStocks);							//If user selected to add a new stock then call the getstock function afterwards display the new list of the user's stocks
			break;
		case '2':
			valid = true;
			DisplayStocks(stocks, numStocks);							//If user chose to display stocks then display the stocks to the user
			break;
		case '3':
			valid = true;
			EditStock(stocks, numStocks);
			DisplayStocks(stocks, numStocks);							//If the user chose to edit a stock then call the editstock function then display the stocks to the user
			break;
		case '4':
			valid = true;
			DeleteStock(stocks, numStocks);
			DisplayStocks(stocks, numStocks);							//If the user chose to delete a stock then call the delete stock function, then display the new modified list of stocks to the user
			break;
		case '5':
			valid = true;
			run = false;												//If the user chose to exit then set run to false to start closing the program
			break;
		default:														
			DisplayInvalid(valid);										//If the user's input was invalid display a message saying that and set valid to false to reprompt user
		}
	} while (!valid);
}

/***************************************************************************
* Purpose: Gets a new stock from the user and puts it into the stocks
*			pointer array
***************************************************************************/
void GetStock(Stock**& allStocks, int &numStocks)
{
	char buffer[256];				//A variable to hold the buffer for the new m_pSymbol
	char nameBuffer[256];			//A variable to hold the buffer for the new m_pName
	double value = -1;				//A variable to hold the m_Value for the new m_Value
	bool valid = true;				//A variable to determine if input was valid

	do
	{

#pragma region NASDAQ
	system("cls");
	cout << "Enter the NASDAQ of the stock: ";
	ClearCin();
	cin >> buffer;										//Prompts user for the stock's m_pSymbol and gets it putting it into buffer
	for (char& c : buffer)
		c = toupper(c);									//Makes the m_pSymbol uppercase
#pragma endregion 

#pragma region Name
	system("cls");
	cout << "Enter the name of the stock: ";
	ClearCin();									//Prompts user for the stock's m_pName, gets it, and puts it into m_pNamebuffer
	cin.getline(nameBuffer, 256);
	for (char& c : nameBuffer)
		c = toupper(c);									//Makes the m_pName uppercase
#pragma endregion

#pragma region values
	system("cls");
	cout << "Enter the value of the stock: ";
	ClearCin();
	cin >> value;
	CheckForAdvancedInvalidInput(valid, value);			//Prompts user for the stock's m_Value, gets it, stores it in m_Value, then makes sure it was a valid input
#pragma endregion
	} while (!valid);

#pragma region Update Stocks
	Stock** tempStocks = new Stock*[numStocks]();									//Allocates memory for a temporary Stock** that holds a Stock for the number of stocks
	for (int i = 0; i < numStocks; i++)
	{
		tempStocks[i] = new Stock();														//Allocates memory for a Stock
		tempStocks[i]->m_pSymbol = new char[strlen(allStocks[i]->m_pSymbol) + 1];					//Allocates memory for the new Stock's m_pSymbol
		tempStocks[i]->m_pName = new char[strlen(allStocks[i]->m_pName) + 1];						//Allocates memory for the new Stock's m_pName
		tempStocks[i]->m_Value = allStocks[i]->m_Value;											//Sets the new Stock's m_Value
		strcpy(tempStocks[i]->m_pSymbol, allStocks[i]->m_pSymbol);								//Copies the contents of allStocks m_pSymbol into tempStocks m_pSymbol
		strcpy(tempStocks[i]->m_pName, allStocks[i]->m_pName);									//Copies the contents of allStocks m_pName into allStocks m_pName
		delete allStocks[i];																//Deallocates the memory used by the current stock in allstocks
	}
	delete[] allStocks;																		//Deallocates the memory used by allstocks

	allStocks = new Stock*[numStocks + 1]();										//Allocates memory for the new list of stocks that will hold 1 more stock than before
	for (int i = 0; i < numStocks; i++)
	{																						//Loops through the old number of stocks giving back the stocks m_Value to the new stock** that holds one more
		allStocks[i] = new Stock();															//Allocates memory for a stock
		allStocks[i]->m_pSymbol = new char[strlen(tempStocks[i]->m_pSymbol) + 1];					//Allocates memory for the stock's m_pSymbol
		allStocks[i]->m_pName = new char[strlen(tempStocks[i]->m_pName) + 1];						//Allocates memory for the stock's m_pName
		allStocks[i]->m_Value = tempStocks[i]->m_Value;											//Sets the stock's m_Value to the tempStocks m_Valuee
		strcpy(allStocks[i]->m_pSymbol, tempStocks[i]->m_pSymbol);								//Copies the contents of tempStocks m_pSymbol into allStocks m_pSymbol
		strcpy(allStocks[i]->m_pName, tempStocks[i]->m_pName);									//Copies the contents of tempStocks m_pName into allStocks m_pName
		delete tempStocks[i];																//Deallocate the memory of the tempStock
	}
	delete[] tempStocks;																	//Deallocate the memory of tempStocks

	allStocks[numStocks] = new Stock;														//Allocates memory for a new stock
	allStocks[numStocks]->m_pSymbol = new char[strlen(buffer) + 1];							//Allocates memory for the new stock's m_pSymbol
	allStocks[numStocks]->m_pName = new char[strlen(nameBuffer) + 1];							//Allocates memory for the new stock's m_pName
	allStocks[numStocks]->m_Value = value;													//Sets the new stocks m_Value to the user defined m_Value
	strcpy(allStocks[numStocks]->m_pName, nameBuffer);											//Copies the contents of the m_pNameBuffer into the new stock's m_pName
	strcpy(allStocks[numStocks]->m_pSymbol, buffer);											//Copies the contents of the m_pSymbol buffer into the new stock's m_pSymbol
#pragma endregion

	numStocks++;																		//Adds one to the number of stocks
}

/***************************************************
* Purpose: Displays the user's stocks to the user
***************************************************/
void DisplayStocks(Stock**& stocks, int numStocks)
{

	if (numStocks > 1)
		SortStocks(stocks, 0, numStocks - 1);							//If the number of stocks is greater than 1 then sort them in alphabetic order
	system("cls");
	cout << "---Stocks---\n\n\n";								//Displays the screens header to the user

	if (numStocks > 0)
	{
		for (int i = 0; i < numStocks; i++)
		{
			cout << "NASDAQ: " << stocks[i]->m_pSymbol << "\nName: " << stocks[i]->m_pName << "\nValue: " << stocks[i]->m_Value << "\n\n";		//If there is at leas one stock display each stocks m_pSymbol, m_pName, and m_Value in that order
		}
	}
	else
	{
		cout << "You do not currently have any stocks\n\n";																				//If there are no stocks, display that to the user
	}
	cout << "\n\n";
	system("pause");																													//Waits for user input
}

/*******************************************************************
* Purpose: Searches for a stock with a given m_pSymbol, if it is found
*			set the index that it was found at and return true
*			if it wasn't found return false
*******************************************************************/
bool Search(Stock**& scope, const int len, char* find, int &index)
{
	int first, last, middle;										//Variables used to search

	first = 0;														//Sets the first index to 0
	last = len - 1;													//Sets the last index to the length( the number of stocks ) - 1 (since it's an index)
	middle = (first + last) / 2;									//Sets middle to the midpoint of first and last

	while (first <= last)												//Loops aslong as first is <= to last
	{
		if (strcmp(scope[middle]->m_pSymbol, find) < 0)
			first = middle + 1;														//If the middle scopes m_pSymbol is closer to a alphabetically than find( then find's m_Value has to be higher than the current middle so set the first index to the middle + 1
		else if (strcmp(scope[middle]->m_pSymbol, find) == 0)							
		{
			index = middle;
			delete[] find;
			return true;															//If the middle scopes m_pSymbol is equal then set index to middle, deallocate the memory of fine, and return true
		}
		else																	
			last = middle - 1;														//Otherwise set last index = to middle - 1( since the search will have to be on that half )

		middle = (first + last) / 2;												//Refind the midpoint
	}
	if (first > last)												
	{
		delete[] find;
		return false;										//If first is greater than last then the desired stock was not found so deallocate the memory for find and return false
	}
		
}

/*******************************************************************
* Purpose: Gets the index of a stock by first asking the user
*			what stock they wish to search for
*******************************************************************/
int GetStockIndex(Stock**& stocks, int numStocks)
{
	int index = -2;									//Initializes index to -2
	char desiredStock[256];							//A buffer for the desired stock
	system("cls");
	cout << "Enter the NASDAQ of the stock: ";
	cin >> desiredStock;										//Prompts the user for the stock's m_pSymbol and gets it
	for (char& c : desiredStock)
		c = toupper(c);											//Makes the m_pSymbol uppercase				

	
	char* dStockPointer = new char[strlen(desiredStock) + 1];			//Allocates memory for the stocks m_pSymbol
	strcpy(dStockPointer, desiredStock);								//Copies the contents of the desiredStock buffer into dStockPointer

	if (Search(stocks, numStocks, dStockPointer, index))						//Searches for the desired stock and sets the index if found
	{
	}
	else
	{
		index = -1;																//If stock was not found then set index to -1
	}
	return index;																//Return the index
}

/*********************************
* Purpose: Edits a stocks m_Value
*********************************/
void EditStock(Stock**& stocks, int numStocks)
{
	int index = GetStockIndex(stocks, numStocks);												//Gets an index for the stock

	system("cls");
	if (index >= 0)																						//If the index is >= 0 then the stock exists so proceed
	{
		system("cls");
		cout << "Stock found!\n\n\n\nPress any key to continue to edit stock...";
		ClearCin();
		_getch();																						//Display to the user that the stock was found and wait for their conformation

		double value;															//Variable to hold the new m_Value
		bool valid;																//Variable to determine valid input

		do
		{
			system("cls");
			cout << "Enter the new value of the stock: ";
			ClearCin();
			cin >> value;													
			CheckForAdvancedInvalidInput(valid, value);							//Prompts user to enter the new m_Value and gets it from the user, then makes sure it was valid

			if (valid)
			{
				stocks[index]->m_Value = value;									//If the input was valid then set the desired stock's m_Value to the desired m_Value
			}

		} while (!valid);
	}
	else
	{
		cout << "Stock not found!\n\n\n\n";
		system("pause");														//If the stocks were not found then display that to the user and wait for user input
	}
}

/*************************************************************
* Purpose: Deletes a stock from allStocks and reallocates 
*			memory that will be less now that one stock was
*			deleted
*************************************************************/
void DeleteStock(Stock**& stocks, int &numStocks)
{
	int index = GetStockIndex(stocks, numStocks);

	system("cls");
	if (index >= 0)																						//If stock was found
	{	
		system("cls");
		cout << "Stock found!\n\n\n\nPress any key to delete the stock...";
		ClearCin();
		_getch();																						//Display that the stock was found and wait for input

		Stock** tempStocks = new Stock*[numStocks-1]();												//Allocates memory for a temporary stock that will hold 1 less stock than the current stocks

		int offset = 0;																		//Number used as an offset for i when filling the tempstocks
		for (int i = 0; i < numStocks; i++)
		{
			if (i != index)																		//As long as i is not the index of the stock to delete
			{														
				tempStocks[i - offset] = new Stock;																//Allocate memory for a new stock
				tempStocks[i - offset]->m_pSymbol = new char[strlen(stocks[i]->m_pSymbol) + 1];						//Allocate memory for the stocks m_pSymbol			
				tempStocks[i - offset]->m_pName = new char[strlen(stocks[i]->m_pName) + 1];							//Allocate memory for the stocks m_pName
				strcpy(tempStocks[i - offset]->m_pSymbol, stocks[i]->m_pSymbol);										//Copies contents of stocks[i]'s m_pSymbol to tempStock[i]'s m_pSymbol
				strcpy(tempStocks[i - offset]->m_pName, stocks[i]->m_pName);											//Copies contents of stocks[i]'s m_pName to tempStocks[i]'s m_pName
				tempStocks[i - offset]->m_Value = stocks[i]->m_Value;												//Sets the tempStocks[i]'s m_Value to stocks[i]'s m_Value
			}	
			else
			{
				//if(i!=0)
					offset++;
			}

			delete stocks[i];																			//Deallocates memory for stocks[i]
		}
		delete stocks;																					//Deallocates memory for stocks

		stocks = new Stock*[numStocks - 1]();																//Allocates memory for a new Stock* that is 1 less than before
		for (int i = 0; i < numStocks - 1; i++)															//Loops through the stocks
		{
			stocks[i] = new Stock();																		//Allocates memory for a new stock
			stocks[i]->m_pSymbol = new char[strlen(tempStocks[i]->m_pSymbol) + 1];							//Allocates memory for the stock's m_pSymbol
			stocks[i]->m_pName = new char[strlen(tempStocks[i]->m_pName) + 1];								//Allocates memory for the stock's m_pName
			strcpy(stocks[i]->m_pSymbol, tempStocks[i]->m_pSymbol);											//Copies contents of tempStocks m_pSymbol into allStocks m_pSymbol
			strcpy(stocks[i]->m_pName, tempStocks[i]->m_pName);												//Copies contents of tempStocks m_pName into allstocks m_pName
			stocks[i]->m_Value = tempStocks[i]->m_Value;													//Sets the m_Value of the stock to tempstocks m_pName
		}
		
		for (int i = 0; i < numStocks - 1; i++)
			delete tempStocks[i];
		delete tempStocks;
		numStocks--;															//Subtracts 1 from the number of stocks
	}
	else
	{
		cout << "Stock not found!\n\n\n\n";
		system("pause");														//If stock was not found display that to the user and wait for input
	}
}
#pragma endregion