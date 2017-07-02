// clinicSymulator.cpp: Okre�la punkt wej�cia dla aplikacji konsoli.
//

#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <ncurses.h>
#include "Patient.h"
#include "Reception.h"
#include "Doctor.h"
#include "patientThreadFunction.h"
#include "receptionThreadFunction.h"


using namespace std;

const int Row = 24;
const int Col = 80;

void patientGenerator(Reception *reception, char viewArray[Row][Col]);
void viewArrayGenerator(char viewArray[Row][Col], const int numberOfDoctors);
void print(char viewArray[Row][Col], atomic<bool> &end);
void doctorThreadFunction(shared_ptr<Doctor> doctor, char viewArray[Row][Col], atomic<bool> &end);

int main(int argc, char** argv)
{

	if(argc != 2)
	{
		cout << "Bledna liczba argumentow!\n";
		return -1;
	}
	int numberOfDoctors = atoi(argv[1]);
	if(numberOfDoctors < 1 || numberOfDoctors > 4)
	{
		cout << "Argument musi byc liczba! od 1 do 4\n";
		return -2;
	}


	initscr();
	curs_set(0);
	char viewArray[Row][Col];
	viewArrayGenerator(viewArray, numberOfDoctors);

	atomic<bool> end {false};
	thread printThread(print, viewArray, ref(end));

	Reception *reception = new Reception();
	vector<shared_ptr<Doctor>> doctors;
	for (int i = 0; i < numberOfDoctors; i++)
	{
		shared_ptr<Doctor> doctor = shared_ptr<Doctor>(new Doctor(i));
		doctors.push_back(doctor);
	}

	receptionThreadFunction receptrionFunction(reception, doctors);
	thread receptionThread(receptrionFunction, ref(end));

	thread patientGeneratorThread(patientGenerator, ref(reception), viewArray);

	vector<thread> threadsDoctors;
	for(auto it = doctors.begin(); it != doctors.end(); it++ )
	{
		thread doctorThread(doctorThreadFunction,*it, viewArray, ref(end));
		threadsDoctors.push_back(move(doctorThread));
	}
	for(auto &doctorThread : threadsDoctors)
	{
		doctorThread.join();
	}
	patientGeneratorThread.join();
	printThread.join();
	receptionThread.join();
	delete reception;
	endwin();
	return 0;
}

void viewArrayGenerator(char viewArray[Row][Col], const int numberOfDoctors)
{
	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Col; j++)
			viewArray[i][j] = ' ';

	viewArray[1][9] = 'R';

	for(int i = 0, row = 6; i < numberOfDoctors; i++,row+=5)
	{
		viewArray[row][9] = 'D';
	}

	for (int i = 15; i < Col; i += 5)
	{
		viewArray[1][i] = '|';
		if (i < 65)
		{
			for(int j =0, row = 6; j < numberOfDoctors; j++, row+=5)
			{
				viewArray[row][i] = '|';
			}
		}
	}
}


void print(char viewArray[Row][Col], atomic<bool> &end)
{
	while (!end)
	{
		clear();
		for (int i = 0; i < Row; i++)
			for(int j = 0; j < Col; j++)
				mvaddch(i,j,viewArray[i][j]);
		refresh();
		this_thread::sleep_for(chrono::milliseconds(20));
	}
}


void patientGenerator(Reception *reception, char viewArray[Row][Col])
{
	vector<thread> threads;
	for (int numberOfPatients = 1; numberOfPatients <= 100; numberOfPatients++)
	{
		while(numberOfPatients - reception->getNumberOfPatientDone() >12)
		{
			this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		patientThreadFunction patientFunction(numberOfPatients, ref(reception), viewArray);
		thread patientThread(patientFunction);
		threads.push_back(move(patientThread));

		random_device randomDevice;
		mt19937 generator(randomDevice());
		uniform_int_distribution<> range(0, 4);
		int seconds = range(generator);
		this_thread::sleep_for(std::chrono::seconds(seconds));
	}
	for (auto &t : threads)
	{
		t.join();
	}
}



void doctorThreadFunction(shared_ptr<Doctor> doctor, char viewArray[Row][Col], atomic<bool> &end)
{
	string doctorName = doctor->showName();
	int row;
	if (doctorName == "D0")
		row = 6;
	if (doctorName == "D1")
		row = 11;
	if (doctorName == "D2")
		row = 16;
	if (doctorName == "D3")
		row = 21;

	while (!end)
	{
		viewArray[row][62] = 'E';
		viewArray[row][63] = 'N';
		viewArray[row][64] = 'D';
		viewArray[row][65] = ' ';
		viewArray[row][66] = 'V';
		viewArray[row][67] = 'I';
		viewArray[row][68] = 'S';
		viewArray[row][69] = 'I';
		viewArray[row][70] = 'T';
		viewArray[row][71] = ' ';
		viewArray[row][72] = 'F';
		viewArray[row][73] = 'O';
		viewArray[row][74] = 'R';
		viewArray[row][75] = ':';
		viewArray[row][76] = ' ';
		viewArray[row][77] = doctor->getTimeToEndVisit()/10 + 48;
		viewArray[row][78] = doctor->getTimeToEndVisit()%10 + 48;
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}
