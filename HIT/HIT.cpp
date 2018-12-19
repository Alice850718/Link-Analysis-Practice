#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <time.h>
#include <windows.h>
#include <stdio.h>

using namespace std;


/*timeee*/

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}


clock_t t1, t2;
long double limit = 0.0001;

class Node
{
public:
	Node(int iid)
	{
		id = iid;
		currentAuthority = 0;
		currentHub = 0;
		oldAuthority = 1;
		oldHub = 1;
	}

	vector<int> connect_id;//連接到~
	vector<int> connected_id;//被~連接到

	vector<int> connect_id_arr;//連接到陣列中的哪一個
	vector<int> connected_id_arr;//被陣列中的哪一個連接到

	int id;

	long double currentAuthority;
	long double currentHub;

	long double oldAuthority;
	long double oldHub;
};

vector<Node> node;

void read_data()
{
	node.clear();
	int graphNum;
	ifstream read;
	cout << endl << "Plz enter graph number(1~8): ";
	cin >> graphNum;
	switch (graphNum)
	{
	case 1:
		read.open("graph_1.txt");
		break;
	case 2:
		read.open("graph_2.txt");
		break;
	case 3:
		read.open("graph_3.txt");
		break;
	case 4:
		read.open("graph_4.txt");
		break;
	case 5:
		read.open("graph_5.txt");
		break;
	case 6:
		read.open("graph_6.txt");
		break;
	case 7:
		read.open("data_result.txt");
		break;
	case 8:
		read.open("association_result.txt");
		break;
	default:
		break;
	}

	string read_s;
	while (getline(read, read_s))//紀錄data
	{
		stringstream ss(read_s);
		string sub_str;
		getline(ss, sub_str, ',');//指向別人的node

		int first_id = atoi(sub_str.c_str());
		int first_id_arr;
		bool check = 0;
		for (int i = 0; i < node.size(); i++)
		{
			if (node[i].id == first_id)
			{
				check = 1;
				first_id_arr = i;
				break;
			}
		}
		if (check == 0)
		{
			node.push_back(Node(first_id));
			first_id_arr = node.size() - 1;
		}

		getline(ss, sub_str, ',');//被指向的node
		int second_id = atoi(sub_str.c_str());
		int second_id_arr;
		check = 0;
		for (int i = 0; i < node.size(); i++)
		{
			if (node[i].id == second_id)
			{
				check = 1;
				second_id_arr = i;
				break;
			}
		}
		if (check == 0)
		{
			node.push_back(Node(second_id));
			second_id_arr = node.size() - 1;
		}

		check = 0;
		for (int i = 0; i < node[first_id_arr].connect_id.size(); i++)
		{
			if (node[first_id_arr].connect_id[i] == second_id)
			{
				check = 1;
				break;
			}
		}
		if (check == 0)
		{
			node[first_id_arr].connect_id.push_back(second_id);
			node[first_id_arr].connect_id_arr.push_back(node[first_id_arr].connect_id.size() - 1);
		}

		check = 0;
		for (int i = 0; i < node[second_id_arr].connected_id.size(); i++)
		{
			if (node[second_id_arr].connected_id[i] == first_id)
			{
				check = 1;
				break;
			}
		}
		if (check == 0)
		{
			node[second_id_arr].connected_id.push_back(first_id);
			node[second_id_arr].connected_id_arr.push_back(node[second_id_arr].connected_id.size() - 1);
		}
	}

	for (int i = 0; i < node.size() - 1; i++)
	{
		for (int j = 0; j < node.size() - i - 1; j++)
		{
			if (node[j].id > node[j + 1].id)
			{
				int temp_id = node[j].id;
				node[j].id = node[j + 1].id;
				node[j + 1].id = temp_id;
				node[j].connected_id.swap(node[j + 1].connected_id);
				node[j].connected_id_arr.swap(node[j + 1].connected_id_arr);
				node[j].connect_id.swap(node[j + 1].connect_id);
				node[j].connect_id_arr.swap(node[j + 1].connect_id_arr);
			}
		}
	}



	for (int i = 0; i < node.size(); i++)
	{
		for (int j = 0; j < node[i].connect_id.size(); j++)
		{
			for (int l = 0; l < node.size(); l++)
			{
				if (node[i].connect_id[j] == node[l].id)
				{
					node[i].connect_id_arr[j] = l;
				}
			}
		}

		for (int k = 0; k < node[i].connected_id.size(); k++)
		{
			for (int l = 0; l < node.size(); l++)
			{
				if (node[i].connected_id[k] == node[l].id)
				{
					node[i].connected_id_arr[k] = l;
				}
			}
		}
	}


	read.close();
}

void algorithm()
{
	/*Algorithm*/
	long double maxAuthority;//紀錄最大的
	long double maxAuthorityDiff;//紀錄最新舊最大差別
	long double maxHub;
	long double maxHubDiff;
	long double sumAuthority;
	long double sumHub;

	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	double time;

	long double maxLimit;//用來判斷是否收斂

	int run = 1;

	//t1 = clock();
	//StartCounter();

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);

	while (true)
	{
		maxLimit = 0;
		maxAuthority = -1;
		maxHub = -1;
		maxAuthorityDiff = -1;
		maxHubDiff = -1;
		sumAuthority = 0;
		sumHub = 0;

		/*
		for (int i = 0; i < node.size(); i++)
		{
			node[i].currentAuthority = 0;
			for (int j = 0; j < node[i].connected_id.size(); j++)//計算新的authority
			{
				node[i].currentAuthority = node[i].currentAuthority + node[node[i].connected_id_arr[j]].oldHub;
			}

			node[i].currentHub = 0;
			for (int k = 0; k < node[i].connect_id.size(); k++)//計算新的Hub
			{
				node[i].currentHub = node[i].currentHub + node[node[i].connect_id_arr[k]].oldAuthority;
			}

			sumAuthority = sumAuthority + node[i].currentAuthority*node[i].currentAuthority;
			sumHub = sumHub + node[i].currentHub*node[i].currentHub;

			if (maxAuthority < node[i].currentAuthority)
				maxAuthority = node[i].currentAuthority;
			if (maxHub < node[i].currentHub)
				maxHub = node[i].currentHub;
		}*/
		for (int i = 0; i < node.size(); i++)
		{
			node[i].currentAuthority = 0;
			for (int j = 0; j < node[i].connected_id.size(); j++)//計算新的authority
			{
				node[i].currentAuthority = node[i].currentAuthority + node[node[i].connected_id_arr[j]].oldHub;
			}
			sumAuthority = sumAuthority + node[i].currentAuthority*node[i].currentAuthority;
		}
		sumAuthority = sqrt(sumAuthority);
		for (int i = 0; i < node.size(); i++)
		{
			node[i].currentAuthority = node[i].currentAuthority / sumAuthority;
			if (maxAuthority < abs(node[i].currentAuthority - node[i].oldAuthority))
				maxAuthority = abs(node[i].currentAuthority - node[i].oldAuthority);

			node[i].oldAuthority = node[i].currentAuthority;
		}

		for (int i = 0; i < node.size(); i++)
		{
			node[i].currentHub = 0;
			for (int k = 0; k < node[i].connect_id.size(); k++)//計算新的Hub
			{
				node[i].currentHub = node[i].currentHub + node[node[i].connect_id_arr[k]].currentAuthority;
			}
			sumHub = sumHub + node[i].currentHub*node[i].currentHub;
		}
		sumHub = sqrt(sumHub);
		for (int i = 0; i < node.size(); i++)
		{
			node[i].currentHub= node[i].currentHub / sumHub;
			if (maxHub < abs(node[i].currentHub - node[i].oldHub))
				maxHub = abs(node[i].currentHub - node[i].oldHub);

			node[i].oldHub = node[i].currentHub;
		}

		cout << "run: " << run << endl;
		run++;

		//cout << maxAuthority + maxHub << endl;
		if (maxAuthority + maxHub < limit)
			break;

	}
	//t2 = clock();
	QueryPerformanceCounter(&nEndTime);
	time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
	
	bool bs = false;
	cout << "bs: ";
	cin >> bs;

	if (bs == 1)
	{
		for (int i = 0; i < node.size() - 1; i++)
		{
			for (int j = 0; j < node.size() - 1 - i; j++)
			{
				if (node[j].oldAuthority < node[j + 1].oldAuthority)
				{
					long double iTemp = node[j].id;
					long double aTemp = node[j].oldAuthority;
					long double hTemp = node[j].oldHub;

					node[j].id = node[j + 1].id;
					node[j].oldAuthority = node[j + 1].oldAuthority;
					node[j].oldHub = node[j + 1].oldHub;

					node[j + 1].id = iTemp;
					node[j + 1].oldAuthority = aTemp;
					node[j + 1].oldHub = hTemp;
				}
			}
		}
	}

	/*
	for (int i = 0; i < node.size(); i++)//大到小
	{
		cout << node[i].id << " Authority:";
		cout << node[i].oldAuthority << " Hub:";
		cout << node[i].oldHub << endl;
	}
	*/

	for (int i = node.size() - 1; i >= 0; i--)//小到大
	{
		printf("%d", node[i].id);
		printf(" Authority: %.10llf", node[i].oldAuthority);
		printf(" Hub: %.10llf\n", node[i].oldHub);
		/*
		cout << node[i].id << " Authority:";
		cout << node[i].oldAuthority << " Hub:";
		cout << node[i].oldHub << endl;*/
	}

	printf("%.20lf milisec: ", time * 1000);
	//printf("%.50lf milisec", (t2 - t1) / (long double)(CLOCKS_PER_SEC) * 1000);
	//cout << (t2 - t1) / (long double)(CLOCKS_PER_SEC) << " secs" << endl;
}

int main()
{
	while (true)
	{
		read_data();

		/*
		for (int i = 0; i < node.size(); i++)
		{
		cout << "node: " << node[i].id << endl << "connect to: ";
		for (int j = 0; j < node[i].connect_id.size(); j++)
		cout << node[i].connect_id[j] << " ";
		cout << endl << "connected by: ";
		for (int j = 0; j < node[i].connected_id.size(); j++)
		cout << node[i].connected_id[j] << " ";
		cout << endl;
		}*/

		algorithm();
	}

	system("pause");
	return 0;
}