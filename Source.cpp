// Edward Zempel
// CS 1119 Intro to Programming
// Project 11, Final Project
#include <iostream>
#include <iomanip>
#include <cstdlib> // for exception handling
#include <regex> // for regular expressions
#include <string>
#include <sstream> // contains stringstream
#include <fstream> // file stream manipulation
using namespace std;

void EmployeeNumberPrompt(string& employeeNumberVariable, string fieldTitle);
bool isNumberOnly(string& input);

void namePrompt(string& textVariable, string firstOrLast);
bool isTextOnly(string& input);
bool isTooLong(string& nameString);

void doublePrompt(double& textVariable, string variableName);
bool isDouble(string& input);
bool isGreaterThanZero(double& doubleToCheck);

void display(const string&);
void display(const double& outputdouble);

bool isTxtFile(string& fileNameToTest);
bool isCsvFile(string& fileNameToTest);

bool promptToContinue();
bool isOvertime(const double& hoursWorked);

int main() {
	cout << "Welcome to Ed Zempel's final project!" << endl;

	string dataFileName{ "" };
	bool fileNameLoop{ 1 }; // control variable for data file prompt loop
	// data file prompt loop
	while (fileNameLoop) { 
		string filenameInput{ "" };
		cout << "Enter path for txt file (exp: data.txt, c:\\data\\data.txt)." << endl;
		cout << "Enter 0 for default 'data.txt': ";
		cin >> filenameInput;
		if (filenameInput == "0") {
			filenameInput = "data.txt";
		}
		if (isTxtFile(filenameInput)) {
			dataFileName = filenameInput;
			// display(filenameInput); // for code debugging
			fileNameLoop = 0;
		}
		else {
			cout << "Invalid entry. Look at example pattern." << endl;
		}

	}

	ofstream outDataFile;
	// open data file
	try {
		outDataFile.open( dataFileName, ios::out );
		//outDataFile.open( "data.txt", ios::app ); // for debugging
	}
	catch (const ifstream::failure&){
		cerr << dataFileName << "Could not open file." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	string employeeNumber{ "0" };
	string firstName{ "" };
	string lastName{ "" };
	double hoursWorked{ 0 };
	double payRate{ 0 };

	bool enterMoreEmployees{ 1 };
	while (enterMoreEmployees) {
		// prompt for employee information
		EmployeeNumberPrompt(employeeNumber, "employee number");
		namePrompt(firstName, "first");
		namePrompt(lastName, "last");
		doublePrompt(hoursWorked, "hours worked");
		doublePrompt(payRate, "pay rate");

		// write employee information to data file
		outDataFile << employeeNumber << " " << firstName << " " << lastName << " " << hoursWorked << " " << payRate << endl;

		// check input values for debugging
		display(employeeNumber);
		display(firstName);
		display(lastName);
		display(hoursWorked);
		display(payRate);

		// prompt to enter more employees
		enterMoreEmployees = promptToContinue();
	}
	
	//close dataTxt file
	outDataFile.close();

	string csvFileName{ "" };
	bool csvFileNameLoop{ 1 }; // control variable for csv file prompt loop
	// csv file prompt loop
	while (csvFileNameLoop) {
		string filenameInput{ "" };
		cout << "Enter path for csv file (exp: data.csv, c:\\data\\data.csv)." << endl;
		cout << "Enter 0 for default 'data.csv': ";
		cin >> filenameInput;
		if (filenameInput == "0") {
			filenameInput = "data.csv";
		}
		if (isCsvFile(filenameInput)) {
			csvFileName = filenameInput;
			// display(filenameInput); // for code debugging
			csvFileNameLoop = 0;
		}
		else {
			cout << "Invalid entry. Look at example pattern." << endl;
		}

	}

	ofstream outCsvFile;
	// open data file
	try {
		outCsvFile.open(csvFileName, ios::out);
		//outCsvFile.open("data.csv", ios::out); // for debugging
	}
	catch (const ifstream::failure&) {
		cerr << csvFileName << " could not open file." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	ifstream inDataFile;
	try {
		inDataFile.open(dataFileName, ios::in);
		//inDataFile.open("fakefile.txt", ios::in); // test try catch
		//inDataFile.open("data.txt", ios::in); // for debugging
	}
	catch (const ifstream::failure& e) {
		cerr << dataFileName << " could not open file. Also, " << e.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	// display headers
	cout << setw(15) << "Employee Number" << setw(18) << "First Name" <<
		setw(18) << "Last Name" << setw(15) << "Regular Hours" <<
		setw(16) << "Overtime Hours" << setw(15) << "Pay Rate" <<
		setw(19) << "Gross Pay" << endl;
	while (inDataFile >> employeeNumber >> firstName >> lastName >> hoursWorked >> payRate)
	{
		int overtimeStatus = isOvertime(hoursWorked);
		double grossPay{ 0 };
		double overtimeHours{ 0 };
		switch (overtimeStatus)
		{
		case 1: // if overtime
			 grossPay = (payRate * 40) + ((hoursWorked - 40) * (1.5*payRate));
			 overtimeHours = hoursWorked - 40;
			 hoursWorked = 40;
			break;
		case 0: // if not overtime
			 grossPay = payRate * hoursWorked;
			 overtimeHours = 0;
			break;
		default:
			break;
		}
		// write employee information to csv file
		outCsvFile << employeeNumber << "," << firstName.at(0) << "," << lastName << "," << setprecision(2) << fixed << hoursWorked
			<< "," << overtimeHours << "," << payRate << "," << grossPay << endl;

		// output employee information to screen
		cout << setw(15) << employeeNumber << setw(18) << firstName <<
			setw(18) << lastName << setw(15) << setprecision(2) << fixed << hoursWorked <<
			setw(16) << overtimeHours << setw(10) << "$" << payRate <<
			setw(11) << "$" << grossPay << endl;
	}

	// close all open files
	inDataFile.close();
	outCsvFile.close();

	//cout << setw(25) << "Employee number" << setw(18) << "First Name" << setw(18) << "Last Name" << setw(10) << "" << endl;
	system("pause");
}

void display(const string& outputString) {
	cout << outputString << endl;
}

void display(const double& outputdouble) {
	cout << outputdouble << endl;
}
// prompt for new value for the textVariable and validate for text only
void namePrompt(string& textVariable, string firstOrLast) {
	string input;
	cout << "Enter " << firstOrLast << " name: ";
	cin >> input;
	if (isTextOnly(input)) {
		textVariable = input;
		if (isTooLong(input)) { // check if over 15 characters
			cout << "Invalid entry. Name must be under 15 characters." << endl;
			namePrompt(textVariable, firstOrLast);
		}
	}
	else {
		while (!isTextOnly(input)) {
			cout << "Invaid entry. Only alphabetic characters allowed." << endl;
			// namePrompt(stringToChange);
			cout << "Please re-enter " << firstOrLast << " name using only A-Z or a-z characters. Enter 0 for default \"Sample\": ";
			cin >> input;
			if (input == "0") {
				input = "Sample";
				break;
			}
			if (isTooLong(input)) { // check if over 15 characters
				cout << "Invalid entry. Name must be under 15 characters." << endl;
				namePrompt(textVariable, firstOrLast);
			}
		}
		textVariable = input;
	}
}
bool isTextOnly(string& input) {
	regex textPattern("[a-z]+", regex_constants::icase); // matches any number of charters  from  a to z and ignores the case
	bool isTextMatch = regex_match(input, textPattern);
	return isTextMatch;
}

void EmployeeNumberPrompt(string& employeeNumberVariable, string fieldTitle) {
	string input;
	cout << "Enter " << fieldTitle << ": ";
	cin >> input;
	if (isNumberOnly(input)) {
		employeeNumberVariable = input;
	}
	else {
		while (!isNumberOnly(input)) {
			cout << "Invaid entry. Only positive numbers allowed." << endl;
			// namePrompt(stringToChange);
			cout << "Please re-enter " << fieldTitle << " using characters  0-9. Enter 0 for default \"100\": ";
			cin >> input;
			if (input == "0") {
				input = "100";
				break;
			}
		}
		employeeNumberVariable = input;
	}
}
bool isNumberOnly(string& input) {
	regex numberPattern("[0-9]+"); // accepts any number of numbers 0 through 9
	bool isNumberMatch = regex_match(input, numberPattern);
	return isNumberMatch;
}

void doublePrompt(double& doubleVariable, string variableName) {
	string input;
	cout << "Enter " << variableName << " : ";
	cin >> input;
	if (isDouble(input)) {
		stringstream convert(input);
		if (!(convert >> doubleVariable)) {
			doubleVariable = 0;
			cout << "Error 1 in doublePrompt";
		}
		if (!isGreaterThanZero(doubleVariable)) {
			cout << "Invalid entry. Please enter a number above 0 such as 10.5 or 10." << endl;
			doublePrompt(doubleVariable, variableName);
		}
		
	}
	else {
		while (!isDouble(input)) {
			cout << "Invaid entry. Only positive numbers allowed." << endl;
			// namePrompt(stringToChange);
			cout << "Please re-enter positive " << variableName << " using characters  0-9: ";
			cin >> input;
		}
		stringstream convert(input);
		if (!(convert >> doubleVariable)) {
			doubleVariable = 0;
			cout << "Error 2 in doublePrmpt";
		}
		if (!isGreaterThanZero(doubleVariable)) {
			cout << "Invalid entry. Please enter a number above 0 such as 10.5 or 10." << endl;
			doublePrompt(doubleVariable, variableName);
		}
	}
}
bool isDouble(string& input) {
	regex doublePattern("\\.?[0-9]+\\.?[0-9]*");
	bool isDoubleMatch = regex_match(input, doublePattern);
	return isDoubleMatch;
}
bool isGreaterThanZero(double& doubleToCheck) {
	if (doubleToCheck > 0) {
		return 1;
	}
	else {
		return 0;
	}
}
// check if file is txt
bool isTxtFile(string& fileNameToTest) {
	regex txtFilePattern("[a-z]?:?\\\\?([a-z0-9]*\\\\?)*[a-z0-9]+\\.txt");
	// regex txtFilePattern("[a-z]?:?\\?[a-z0-9]+\\.txt");
	bool isTxtFile = regex_match(fileNameToTest, txtFilePattern);
	return isTxtFile;
}
// check if file is csv
bool isCsvFile(string& fileNameToTest) {
	regex CsvFilePattern("[a-z]?:?\\\\?([a-z0-9]*\\\\?)*[a-z0-9]+\\.csv");
	bool isCsvFile = regex_match(fileNameToTest, CsvFilePattern);
	return isCsvFile;
}

bool promptToContinue() {
	string input{ "" };
	cout << "Enter 1 to enter more employees or 0 to stop: ";
	cin >> input;
	if (input == "0") {
		return 0;
	}
	else {
		return 1;
	}
}

bool isOvertime(const double& hoursWorked) {
	if (hoursWorked > 40) {
		return 1;
	}
	else {
		return 0;
	}
}

bool isTooLong(string& nameString) {
	if (nameString.length() > 15) {
		return 1;
	}
	else {
		return 0;
	}
}
