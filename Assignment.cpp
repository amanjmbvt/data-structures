#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct LifeGuard
{
	long starttime;
	long endtime;
	long totaltime;
	long overlaptime;
	struct LifeGuard* next;
};

long memberscount = 0, shiftstart=1000000000, shiftend=0;
LifeGuard* firstnode, *mostoverlapped;

LifeGuard* createnode(long starttime, long endtime, long totaltime, long overlaptime)
{
	struct LifeGuard* temp, * s;
	temp = new(struct LifeGuard);
	if (temp == NULL)
	{
		cout << "Memory not allocated " << endl;
		return 0;
	}
	else
	{
		temp->starttime = starttime;
		temp->endtime = endtime;
		temp->totaltime = totaltime;
		temp->overlaptime = overlaptime;
		temp->next = NULL;
		return temp;
	}
}

void insert_last(long starttime, long endtime, long totaltime, long overlaptime)
{
	struct LifeGuard* temp, * s;
	temp = createnode(starttime, endtime, totaltime, overlaptime);
	s = firstnode;
	while (s->next != NULL)
	{
		s = s->next;
	}
	temp->next = NULL;
	s->next = temp;
}

string* splitstring(string str)
{
	string* splitarray = new string[2];
	string word = "";
	for (auto x : str)
	{
		if (x == ' ')
		{
			splitarray[0] = word;
			word = "";
		}
		else
		{
			word = word + x;
		}
	}
	splitarray[1] = word;
	
	return splitarray;
}

void createnodes(string file_name)
{
	cout << "Started reading";
	std::ifstream file("inputfiles/" + file_name);
	std::string to;
	int flag = 0;
	cout << "Completed reading";
	while (std::getline(file, to)) {
		if (flag == 0)
		{
			memberscount = stol(to, nullptr, 10);		
			flag = 1;
		}
		else 
		{
			string* temparr = splitstring(to);
			long starttime = stol(temparr[0], nullptr, 10);
			long endtime = stol(temparr[1], nullptr, 10);
			// cout << "Creating node:--" << starttime << " " << endtime << "\n";
			if(firstnode == NULL)
				firstnode = createnode(starttime, endtime, endtime - starttime, 0);
			else 
				insert_last(starttime, endtime, endtime - starttime, 0);
		}

		// cout << to << endl;
	//	flag++;
	}
	cout << "Completed creating";
}

void calculateoverlaptime()
{
	LifeGuard* currentguard = firstnode, * nodeloop;
	mostoverlapped = NULL;
	for (int i=0; currentguard != NULL; currentguard = currentguard->next, i++)
	{
		nodeloop = firstnode;
		if (shiftstart > currentguard->starttime)
			shiftstart = currentguard->starttime;
		
		if (shiftend < currentguard->endtime)
			shiftend = currentguard->endtime;
		
		for (int j=0;nodeloop != NULL; nodeloop = nodeloop->next, j++)
		{
			if (i == j) continue;
			if (currentguard->starttime >= nodeloop->starttime && currentguard->starttime <= nodeloop->endtime)
			{
				currentguard->overlaptime += nodeloop->endtime - currentguard->starttime;
			}

			if (currentguard->endtime >= nodeloop->starttime && currentguard->endtime <= nodeloop->endtime)
			{
				currentguard->overlaptime += currentguard->endtime - nodeloop->starttime;
			}			
		}

		//	Checking the record of most overlapped guard
		if (mostoverlapped == NULL)
			mostoverlapped = currentguard;
		else {
			long mosttime = mostoverlapped->totaltime - mostoverlapped->overlaptime;
			long currtime = currentguard->totaltime - currentguard->overlaptime;
			if(mosttime > currtime)
				mostoverlapped = currentguard;
		}
		cout << currentguard->starttime << "   " << currentguard->endtime << "   " << currentguard->totaltime << "   " << currentguard->overlaptime << "\n";
	}
	cout << shiftstart << " " << shiftend << "\n";

	cout << " -------------------------------mostoverlapped-------------------\n";
	cout << mostoverlapped->starttime << "   " << mostoverlapped->endtime << "   " << mostoverlapped->totaltime << "   " << mostoverlapped->overlaptime << "\n";

}

void writetofile(string filename)
{
	long deptime = mostoverlapped->totaltime - mostoverlapped->overlaptime;
	long funits = (shiftend - shiftstart) - (deptime >=0 ? deptime :0);

	cout << "Final units are:" << funits;
	std::ofstream outfile("outputfiles/" + filename);
	outfile << funits << std::endl;
	outfile.close();
}

int main()
{
	firstnode = NULL;
	// To read the input file and create nodes for the records
	createnodes("1.in");

	//	To calculate the overlap time
	calculateoverlaptime();

	//	Writing output to the file
	writetofile("1.out");

	return 0;
}