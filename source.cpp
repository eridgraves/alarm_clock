// AlarmClock v 1.1
//********************************************************************
//
// Eric Graves
//
// Purpose: Implement an alarm clock in c++
//
// Input: Integers
//
// Output: multiple formatted strings and integers
//
// Known Bugs: Maximum Delay is just over 69 years
//
//********************************************************************

//#define NOMINMAX    // Avoid collision of max() in std and windows.h

#include <ctime>    // Used for local_time() and tm struct
#include <chrono>   // Clock functions
#include <thread>   // Sleep_for and sleep_until
#include <iostream>
#include <iomanip>
#include <fstream>
//#include "Windows.h"
#include <string>

using namespace std::this_thread;
using namespace std::chrono;
using namespace std;


class alarm { // NO DESTRUCTOR

public:
	//void set();         // Gets inout for alarm                                      <====================== CHANGE THIS NAME
	void alarm_set();     // Sets an alarm
	bool error_check();   // Error checks the set alarm
	void display(int);    // Displays the time
	long calc_delay();    // Calculates the delay
	void delay(long);     // Sleep_for(long delay)
	void display_time();
	void new_delay(long); // Sleep_for(long delay)
	int get_hrs();        // Return alarm time
	int get_mins();
	void sleep(int);      // Int because sleep is a short time, not more than a few minutes
	bool sleep_flag = false; // Default value?
	void write_log();     // Write time to log
	void read_log();
  bool log_state;       // True = first run, false = second run
	long delays[3];       // Array holding delay (calculated when set) for each alarm
	void error();         // Displays error
	bool a1_flag;
	bool a2_flag;
	bool a3_flag;
	int times[2][3];      // Holds each alarm time | hours | mins |

	// A constructor!
	alarm() {
		a1_flag = false;
		a2_flag = false;
		a3_flag = false;
		log_state = true;
		for (int x = 0; x <= 2; x++) {
			delays[x] = -99;
		}
		for (int x = 0; x <= 1; x++) {     // 2 columns
			for (int y = 0; y <= 2; y++) { // 2 Rows
				times[x][y] = -99;
			}
		}
	}

private:
	int hrs;            // Alarm Time Variables
	int mins;
	//int seconds;
};

int main() {

	// TODO: When the user presses the set alarm button, change the value of alarm_set


	// Declare Time Structures
	struct tm *tmp;
	time_t s;

	// Declare an alarm
	alarm a;
	bool alarm_set = true;
	//a.read_log();

	// Allow alarms to be set at startup
	int input = 0;
	cout << "\t\t   Alarm Clock v1.1\n\nPlease enter how many alarms you would like to set (max 3): ";
	do {
		while (!(cin >> input)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please enter a number between 0 and 3: ";
		}
	} while (!((input <= 3) && (input >= 0)));

	// If there are zero alarms to be set, don't set any alarms
	if (input == 0) {
		alarm_set = false;
	}
	else {
		while(input > 0) {
			// ====================================================================================
			// Sets the alarm
			// Runs one time when an alarm is set by the user
			if (alarm_set) { // Pretty sure this if is redundant
				a.alarm_set();
				// Write start time to log
				a.log_state = true;
				a.write_log();

				// Save the delay in the appropriate array index
				//  |Array delays   |
				// ------------------
				// 0| Alarm 1 delay |
				// 1| Alarm 2 delay |
				// 2| Alarm 3 delay |
				for (int i = 0; i < 3; i++) {
					if (a.delays[i] == -99) {
						a.delays[i] = a.calc_delay();
						if (i == 0) {
							a.a1_flag = true;
						}
						else if (i == 1) {
							a.a2_flag = true;
						}
						else {
							a.a3_flag = true;
						}

						break;
					}
					if (i == 2) {
						a.error();
					}
					continue;
				}

			}
			input-= 1;
		}
		// Clear Alarm set flag
		alarm_set = false;
	}
	system("clear");

	// The loop for the clock
	// Runs every second
	// Displays the current time
	// Checks if alarms are set, updates them, and displays them
	while (1) {

		// ====================================================================================
		// Sets the alarm
		// Runs one time when an alarm is set by the user
		if (alarm_set) {
			a.alarm_set();
			// Write start time to log   TODO: Move to seperate alarm member function
			a.log_state = true;
			a.write_log();

			// Save the delay in the appropriate array index
			//  |Array delays   |
			// ------------------
			// 0| Alarm 1 delay |
			// 1| Alarm 2 delay |
			// 2| Alarm 3 delay |
			for (int i = 0; i < 3; i++) {
				if (a.delays[i] == -99) {
					a.delays[i] = a.calc_delay();
					if (i == 0) {
						a.a1_flag = true;
					}
					else if (i == 1) {
						a.a2_flag = true;
					}
					else {
						a.a3_flag = true;
					}

					break;
				}
				if (i == 2) {
					a.error();
				}
				continue;
			}
			// Clear Alarm set flag
			alarm_set = false;
		}

		// ====================================================================================
		// Handles the clock functions
		// Update the current GMT and convert it to local time
		s = time(NULL);
		tmp = localtime(&s);

		// Display current local time hh:mm:ss
		printf("%02d:%02d:%02d\n" /*Format allows for the leading zeros to be printed in the time*/, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

		// Delay for 1 second
		sleep_for(seconds(1));

		// Clear the screen (to update the displayed time)
		system("clear");

		// ====================================================================================
		// CHECK IF ALARM BUTTON IS PRESSED AND SET FLAG

		// ====================================================================================
		// This logic checks each alarm to see if it is set, updates the set ones, and displays them as needed
		if (a.a1_flag) {
			if (a.delays[0] > 0) {
				a.delays[0] -= 1;
			}
			else if(a.delays[0] <= 0){
				//a.write_log();
				a.display(1);

			}
			//else {
			//	cout << "Error in alarm displaying main";
			//}
		}
		if (a.a2_flag) {
			if (a.delays[1] > 0) {
				a.delays[1] -= 1;
			}
			else if (a.delays[1] <= 0) {
				//a.write_log();
				a.display(2);

			}
			//else {
				//cout << "Error in alarm displaying main";
			//}
		}
		if (a.a3_flag) {
			if (a.delays[2] > 0) {
				a.delays[2] -= 1;
			}
			else if (a.delays[2] <= 0) {
				//a.write_log();
				a.display(3);

			}
			//else {
			//	cout << "Error in alarm displaying main";
			//}
		}
		// ====================================================================================

	}
	system("pause");
	return 0;

}

/*
void alarm::set() {
cout << "Please enter a time for the alarm to be set (in 24 hour time, format hh:mm): "; //ss): ";
cin >> hrs;
cin.ignore();       // ignore the :
cin >> mins;
//cin.ignore();
//cin >> seconds;
}
*/
void alarm::alarm_set() {
	// Have the user enter the desired alarm time
	do {
		cout << "Please enter a time for the alarm to be set (in 24 hour time, format hh:mm): "; //ss): ";
		cin >> hrs;
		cin.ignore();       // ignore the :
		cin >> mins;
		//cin.ignore();
		//cin >> seconds;
	} while (error_check()); // Error check the input until there is a valid input

	// Notify that the alarm was set
	cout << "An alarm was set for " << hrs << ":" << mins << endl;

}

void alarm::error() {
	cout << "All Alarms have been set." << endl;
}

bool alarm::error_check() {
	if (((hrs > 23) || (hrs < 0)) || ((mins > 59) || (mins < 0))) {
		cout << "The time you entered is invalid. Please try again\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return true;
	}
	return false;
}

// Wanted to have input while alarm sounds, but that requires multi-threading
void alarm::display(int alarm) {

	// Save the current time point
	// Get the current time -- should be system_clock::time_point type
	auto dis_time = system_clock::to_time_t(system_clock::now());

	// ALert the user of the alarm
	cout << "Alarm " << alarm << " is going off!\nThe current time is ";
	display_time();
	//cout << setw(2) << setfill('0') << hrs << ":" << setw(2) << setfill('0') << mins << endl;
	cout << "Press 1 to start the sleep timer, or any other key to stop the alarm: ";

	// Play Sound
	//MessageBeep(0xffffffff);
	int x = rand() % 5;
	for (; x < 5; x++) {
		//Beep(750, 300);
		cout << "BEEP\n";
		sleep_for(seconds(1));
	}

	// Clear any junk that the user has input
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	// See if user wants to activate the sleep timer by pressing '1'
	int input;
	cin >> input;
	if (input == 1) {
		sleep(10);    // Ten minute delay
		display(alarm);
	}

	//Calculate time spent in the display
	int delay_time = system_clock::to_time_t(system_clock::now()) - dis_time;

	system("clear");
	switch (alarm) {
	case 1:
		delays[0] -= delay_time; // Subtract the time spent in the display
		a1_flag = false;
		break;
	case 2:
		delays[1] -= delay_time; // Subtract the time spent in the display
		a2_flag = false;
		break;
	case 3:
		delays[2] -= delay_time; // Subtract the time spent in the display
		a3_flag = false;
		break;
	default:
		cout << "Error in display\n";
	}
}

int alarm::get_hrs() {
	return hrs;
}

int alarm::get_mins() {
	return mins;
}

void alarm::delay(long delay) {

	sleep_for(seconds(delay));
}

void alarm::display_time() {
	system("clear");
	auto now = system_clock::now(); // Get the current time -- should be system_clock::time_point type
	auto converted_now = system_clock::to_time_t(now);
	cout << put_time(localtime(&converted_now), "%X") << endl;
}

void alarm::new_delay(long delay) { // Delay is seconds until alarm time

	// Line up delay with minutes
	if (!(delay % 60 == 0)) {
		sleep_for(seconds(delay % 60));  // sleep for < 60 seconds
		delay -= delay % 60;
	}
	display_time();
	int n = delay / 60;

	for (int x = 0; x < n; x++) {
		sleep_for(seconds(60));
		display_time();
	}
}

long alarm::calc_delay() {  // Should this be in minutes?

	long min_delay = 0;
	long hr_delay = 0;
	time_t rawtime; // Time in seconds since the epoch
	time(&rawtime); // Convert to readable time
	struct tm *tm_struct = localtime(&rawtime); // Convert to local time and get the struct pointer
	int current_hour = tm_struct->tm_hour; // Get the current hours time (0-23)
	int current_min = tm_struct->tm_min; //

										 // Fix for ignoring seconds
	int current_sec = tm_struct->tm_sec; // Subtract this value from the delay

										 // Delay until current_min = mins
										 //====================MINUTES DELAY=====================//
	if (current_min > mins/*Time > Alarm*/) { // Where Time is current time (in 24hrs) and Alarm is the alarm time (in 24hrs)
											  /*delay = 24 - Time + Alarm*/
											  //cout << "alarm mins > current mins\n";
		min_delay = 60 - current_min + mins;
	}
	else if (current_min <= mins/*Alarm > Time*/) {
		/*delay = ALarm - Time*/
		//cout << "alarm mins < current mins\n";
		min_delay = mins - current_min;
	}
	else {
		// Error
		cout << "ERROR IN alarm::delay() MINUTE DELAY\n";
	}

	//sleep_for(seconds(60 * min_delay - current_sec));
	//======================================================//

	// Update hour time
	time(&rawtime); // Convert to readable time
	tm_struct = localtime(&rawtime); // Convert to local time and get the struct pointer
	current_hour = tm_struct->tm_hour; // Get the current hours time (0-23)

									   // Calculate hour delay
									   //=======================HOUR DELAY======================//
	if (current_hour > hrs /*Time > Alarm*/) { // Where Time is current time (in 24hrs) and Alarm is the alarm time (in 24hrs)
											   /*delay = 24 - Time + Alarm*/
											   //cout << "hrs > hour\n";
		hr_delay = 24 - current_hour + hrs;
	}
	else if (current_hour <= hrs/*Alarm > Time*/) {
		/*delay = ALarm - Time*/
		//cout << "hrs < hour\n";
		hr_delay = hrs - current_hour;
	}
	else {
		// Error
		cout << "ERROR IN alarm::delay() HOUR DELAY\n";
	}

	//sleep_for(seconds(3600 * hr_delay));
	//======================================================//

	return (hr_delay * 3600 + min_delay * 60 - current_sec);

	/*// THIS IS A TIMER
	if (system_clock::now() < seconds(hrs * 3600 + mins * 60)) {
	sleep_until(system_clock::now() - seconds(hrs * 3600 + mins * 60)); // The real delay function
	}
	else if (system_clock::now() > seconds(hrs * 3600 + mins * 60)) {
	sleep_until(system_clock::now() - seconds(hrs * 3600 + mins * 60)); // The real delay function
	}
	else {
	return; // idk why you set an alarm for right now. Or there is an error
	}*/
}

void alarm::sleep(int delay) {

	// Declare time structures -- Save original time to make up for lost time during input delays? -- Not in this function
	struct tm *tmp;
	time_t s;

	system("clear");

	for (int x = delay; x > 0; x--) {

		// Update time structures
		s = time(NULL);
		tmp = localtime(&s);

		// Display current local time hh:mm:ss
		printf("%02d:%02d:%02d\nSleep Timer Active\n" /*Format allows for the leading zeros to be printed in the time*/, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
		sleep_for(seconds(1));
		system("clear");
	}
}

void alarm::write_log() {

	//if (log_state) {
	//cout << "updating log...";
	ofstream log;
	log.open("alarm_log.txt", ios::out | ios::app); // Dont erase the existing log items
	auto now = system_clock::now(); // Get the current time -- should be of type system_clock::time_point
	auto converted_now = system_clock::to_time_t(now);
	log << setw(2) << setfill('0') << "Alarm set at " << put_time(localtime(&converted_now), "%Y-%m-%d %X") << " and will go off at " << hrs << ":" << setw(2) << setfill('0') << mins << endl;
	//cout << " done\n";
	log_state = false;
	log.close();
	//}
	/*else {
	log_state = true;
	auto now = system_clock::now();
	auto converted_now = system_clock::to_time_t(now);
	ofstream log;
	log.open("alarm_log.txt", ios::out | ios::app); // Dont erase the existing log items
	log << ". The alarm went off at " << put_time(localtime(&converted_now), "%X") << endl;
	log.close();
	}*/
}

void alarm::read_log() {
	string s;
	std::getline(std::cin, s);
	cout << "The last " << s << endl;

}
