
/*************************************************************
Sarah Sindeband
Due Date: 11/10/21
Course:  C0P3014
Description: Number guessing game with a random number generator static array to store game results.
*************************************************************/
#include <iostream> //standard library for i/o
#include <string> //  string class
#include <ctime> //random number generator
#include <fstream>//output file
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

//const int NUM_GAMES = 40;
//maximum number of games for the array declaration

/*********************************************************
//Following is the declaration of a round of numberGuess
**********************************************************/
class numberGuess
{
public:
	int level = 0;							//game level 1,2, or 3
	int upperValue = 0;						//15, 50, or 150
	int numGuesses = 0;						//4, 6, or 8
	int currentGuess = 0;					//current guess input by the user
	int solution = 0;						//pseudo random generated number
	int small = 0;							//lowest value for feedback
	int large = 0;							//highest value for feedback
	bool above = false;						//current guess above the solution
	bool below = false;						//current guess below the solution
	string name;							//name of current player
	bool wonOrLost = 0;						//true or false for the current round of the game
};


//Declaration / Prototypes

void TestAndDouble(numberGuess*& numGameList, int gameCount, int& size);
//Description: Tests and doubles the size capacity of numGameList when count is the same as the size

void SetUpperRange(numberGuess& currentGame); 
//Description:  Sets the upper value of the current game based on the level

void SetNumGuesses(numberGuess& currentGame);
//Description:  Sets the maximum number of guesses for the current round based on the level

void PlayOneRound(numberGuess& currentGame);
//Description:  Play one round of the number guess game

void GuessInput(numberGuess& currentGame);
//Description:  Displays the range, prompts, and gets the next guess from the user

bool ProcessGuess(numberGuess& currentGame);
//Description: Returns true if the guess matches the solution, or false and lets the user know if the guess
//was above or below the solution

void GenerateSolution(numberGuess& currentGame);
//Description:  Generates the solution number in the correct range

void PrintGameResults(numberGuess numGameList[], int gameCount);
//Description:  Connects to an output file and prints the array results onto the screen and into the output file


int main()
{
	string yesOrNo = "y";
	int gameCount = 0;
	int size = 1;
	numberGuess* numGameList = new numberGuess[size];

	cout << "Do you want to play the guessing game? (y(Y) or n(N)):  ";
	cin >> yesOrNo;

	while (yesOrNo != "n" && yesOrNo != "N")
	{
		TestAndDouble(numGameList, gameCount, size);

		cout << "Enter your first name: ";
		
		cin >> numGameList[gameCount].name;
		//Play one round of the game
		PlayOneRound(numGameList[gameCount]);

		//add one to the gamecount
		gameCount++;

		//Play again?
		cout << "\n--------------------------------------------------\n";
		cout << "Do you want to play the another round? (y(Y) or n(N)):  ";
		cin >> yesOrNo;
	}
	//Print the game results
	PrintGameResults(numGameList, gameCount);
	return  0;
}

void TestAndDouble(numberGuess*& numGameList, int gameCount, int& size)
{
	//Description: Tests and doubles the size capacity of numGameList when count is the same as the size 
	if (gameCount != size)
		return;
	size *= 2; //Multiply size by 2

	//Declare a numberGame pointer and use new to allocate an array with size elements 
	numberGuess* temp = new numberGuess[size];

	//Loop to copy all the numberGames in the gameList over to temp (one by one)
	for (int i = 0; i < gameCount; i++)
	{
		temp[i] = numGameList[i];
	}

	//after copying one by one from gameList to temp delete gameList 
	delete[] numGameList;

	//assign temp to gameList 
	numGameList = temp;
}

void PrintGameResults(numberGuess numGameList[], int gameCount)
//Description:  Connects to an output file and prints the array results onto the screen and into the output file
{
	// declare array for file name
	char filename[16];

	//user should enter filename with .txt extension
	cout << "Enter the filename: ";
	cin >> filename;

	//open the output file
	//add to the file if the user enters the same file name
	ofstream out;
	out.open(filename, ios::app);

	cout << "\n***********************************\n";
	cout << "Name\t" << "Level\t" << "won or lost\n";

	//use a loop to print the array (name, level, WonOrLost) onto the screen
	for (int i = 0; i < gameCount; i++)
	{
		cout << numGameList[i].name << "\t";
		cout << numGameList[i].level << "\t";
		cout << numGameList[i].wonOrLost << "\n";
	}

	out << "\n***********************************\n";
	out << "Name\t" << "Level\t" << "won or lost\n";
	//use a loop to print the array (name, level, WonOrLost) into the file
	for (int i = 0; i < gameCount; i++)
	{
		out << numGameList[i].name << "\t";
		out << numGameList[i].level << "\t";
		out << numGameList[i].wonOrLost << "\n";
	}
	//close the file
	out.close();

}


void SetUpperRange(numberGuess& currentGame)
//Description:  sets the upper value of the current game based on the level
{
	if (currentGame.level == 1) currentGame.upperValue = 15;
	else if (currentGame.level == 2) currentGame.upperValue = 50;
	else if (currentGame.level == 3) currentGame.upperValue = 150;
	else  cout << "That is not a valid level\n";
}

void SetNumGuesses(numberGuess& currentGame)
//Description:  sets the maximum number of guesses for the current round based on the level
{
	if (currentGame.level == 1) currentGame.numGuesses = 4;
	else if (currentGame.level == 2) currentGame.numGuesses = 6;
	else if (currentGame.level == 3) currentGame.numGuesses = 8;
	else  cout << "That is not a valid level\n";
}

void PlayOneRound(numberGuess& currentGame)
//Description:  Play one round of the number guess game
{
	do {
		cout << "************************************************\n";
		cout << "what level(Enter 1,2, or 3)? \n(1) Level 1 - 4 guesses, numbers 1 through 15 \n" <<
			"(2) Level 2 - 6 guesses, numbers 1 through 50\n" <<
			"(3) Level 3 - 8 guesses, numbers 1 through 150:\n";

		cin >> currentGame.level;
	} while (currentGame.level > 3 || currentGame.level < 1);


	//set the upper range based on the level (LevelRange function)
	SetUpperRange(currentGame);

	//generate the random number
	GenerateSolution(currentGame);

	//set the max guesses based on the level (LevelGuesses function)
	SetNumGuesses(currentGame);

	//initalize bool and high/low values
	currentGame.above = 0;
	currentGame.below = 0;
	currentGame.small = 1;
	currentGame.large = currentGame.upperValue;
	currentGame.wonOrLost = 0;

	//cout << "PLAY small: " << currentGame.small << endl;
	//cout << "PLAY large: " << currentGame.large << endl;

	for (int i = 0; i < currentGame.numGuesses; i++)
	{
		cout << "\n-----------------------";
		cout << "\nThis is guess number (" << i + 1 << " of " << currentGame.numGuesses << ")";
		GuessInput(currentGame);

		//reset above and below bool values to false
		currentGame.below = 0;
		currentGame.above = 0;

		if (ProcessGuess(currentGame))
		{
			cout << "\nYou won that round, " << currentGame.name << "!\n";
			i = currentGame.numGuesses;
			currentGame.wonOrLost = true;
		}
	}
	if (!currentGame.wonOrLost)//lost the round
		cout << "\nYou did not win that round, " << currentGame.name << "!\n";
	cout << "\nTHE SOLUTION WAS " << currentGame.solution << endl;

}

void GuessInput(numberGuess& currentGame)
//Description:  Displays the range, prompts, and gets the next guess from the user
{

	if (currentGame.below)
	{
		if (currentGame.small < currentGame.currentGuess)
		{
			currentGame.small = currentGame.currentGuess;
		}
	}
	if (currentGame.above)
	{
		if (currentGame.large > currentGame.currentGuess)
		{
			currentGame.large = currentGame.currentGuess;
		}
	}
	cout << "\nEnter a guess between " << currentGame.small << " and " << currentGame.large << " : ";
	cin >> currentGame.currentGuess;
}

bool ProcessGuess(numberGuess& currentGame)
//Description: Returns true if the guess matches the solution, or false and lets the user know if the guess
//was above or below the solution
{

	if (currentGame.currentGuess == currentGame.solution)
		return true;
	else if (currentGame.currentGuess > currentGame.solution)
	{
		cout << "\nYour guess was too high.";
		cout << "\n-----------------------";
		currentGame.above = true;
		return false;
	}
	else
	{
		cout << "\nYour guess was too low.";
		cout << "\n-----------------------";
		currentGame.below = true;
		return false;
	}

}

void GenerateSolution(numberGuess& currentGame)
//Description:  Generates the solution number in the correct range
{

	srand((unsigned)time(0));
	currentGame.solution = 1 + rand() % currentGame.upperValue;
	//cout << "\nSolution is " << currentGame.solution << endl;
}

