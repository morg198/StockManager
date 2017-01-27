#pragma once
#include <iostream>
using std::cout;
using std::cin;

/**********************************************************************
*	Purpose: Checks for invalid input with optional(default values given)
*			choices to define what is valids
*
*	Entry: when user wants to check if input was valid
*			a bool to see if there is a min number input can be
*			a bool to see if there is a max number input can be
*			a min number
*			a max number
*			the users input
*
*	Exit: true if input was valid, else false
**********************************************************************/
bool ValidInput(int input = 0, bool useMin = false, bool useMax = false, int min = 0, int max = 0)
{
	bool valid = true;			//Variable to hold the validity of the input
	if (cin.fail())																			//If cin is in fail state
	{
		valid = false;																		//Valid is false
	}
	if ((useMin == true && input < min) || (useMax && input > max))							//If input is out of range of user restrictions
	{
		valid = false;																		//Valid is false
	}
	return valid;																			//Return value of valid
}

/**********************************************************************
*	Purpose: Clears the cin buffer and fail state
*
*	Entry: when user wants to clear cin
*
*	Exit: nothing
**********************************************************************/
void ClearCin()
{
	cin.clear();																		//Clears the cin state
	cin.ignore(cin.rdbuf()->in_avail());												//Clears the cin buffer
}

/**********************************************************************
*	Purpose: Shows user that they entered an invalid input and waits
*			for confirmation
*
*	Entry: when user needs to show that there was invalid input
*			takes bool by reference that would be used for an input loop
*
*	Exit: message to show invalid input
**********************************************************************/
void DisplayInvalid(bool &valid)
{
	valid = false;										//sets valid bool to false
	ClearCin();											//Clears cin
	system("cls");										//Clears the screen
	cout << "Invalid Input, try again\n\n\n\n";			//Shows user input was invalid
	system("pause");									//Waits for user to hit enter
	system("cls");										//Clears the screen
}

/**********************************************************************
*	Purpose: Shows user that they entered an invalid input and waits
*			for confirmation
*
*	Entry: when user needs to show that there was invalid input
*
*	Exit: message to show invalid input
**********************************************************************/
void DisplayInvalidNoVar()
{	
	ClearCin();											//Clears cin
	system("cls");										//Clears the screen
	cout << "Invalid Input, try again\n\n\n\n";			//Shows user input was invalid
	system("pause");									//Waits for user to hit enter
	system("cls");										//Clears the screen
}

/**********************************************************************
*	Purpose: Checks if there was invalid input and sets a bool that 
*			is a input loop bool to either true or false
*
*	Entry: when user needs to check for valid input in a loop
*
*	Exit: none
**********************************************************************/
void CheckForInvalidInput(bool &valid)
{
	if (!ValidInput())				//If input was invalid
	{
		DisplayInvalid(valid);		//Show user input was invalid and set bool to false
	}
	else							//If input was valid
	{
		valid = true;				//set input bool to to true
	}
}

/**********************************************************************
*	Purpose: Checks if there was invalid input and sets a bool that
*			is a input loop bool to either true or false with
*			advanced options to define what is valid
*
*	Entry: when user needs to check for valid input in a loop
*
*	Exit: none
**********************************************************************/
void CheckForAdvancedInvalidInput(bool &valid, int input = 0, bool useMin = false, bool useMax = false, int min = 0, int max = 0)
{
	if (!ValidInput(input, useMin, useMax, min, max))			//If input was invalid
	{
		DisplayInvalid(valid);									//Show user input was invalid and set bool to false
	}
	else														//If input was valid
	{
		valid = true;											//set input bool to to true
	}
}
