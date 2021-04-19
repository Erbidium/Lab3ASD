#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <stack>
#include <queue>

using namespace std;

vector<vector<int>> readDirectDistances(int numberCities);
vector<vector<int>> readRoadDistances(int numberCities);
vector<string> readCities(int numberOfCities, string country);
void printMatrix(vector<vector<int>>);
void bestFirstSearch(vector<vector<int>> directWays, vector<vector<int>> roads, vector<string> cities, int start, int end);
void AStar(vector<vector<int>> directWays, vector<vector<int>> roads, vector<string> cities, int start, int end);
void printResults(int start, int end, vector<int> from, vector<string> cities, int length);

int main()
{
	cout<<"Enter country:\n";
	string country;
	cin>>country;
	int numberOfCities;
	cout<<"Enter number of cities:\n";
	cin>>numberOfCities;
	vector<vector<int>> directDistances=readDirectDistances(numberOfCities);
	vector<vector<int>> roadDistances=readRoadDistances(numberOfCities);
	
	vector<string> cities=readCities(numberOfCities, country);
	cout<<"Enter two cities:\n";
	string city1, city2;
	cin>>city1>>city2;
	int index1=-1, index2=-1;
	for(int i=0;i<15;i++)
	{
		if(cities[i]==city1)
			index1=i;
		if(cities[i]==city2)
			index2=i;
	}
	if((index1!=-1)&&(index2!=-1))
	{
		/*cout<<"Best-first search(0) or A*(1)?"<<endl;
		int choice;
		cin>>choice;
		if(choice==0)
		{
			bestFirstSearch(directDistances, roadDistances, cities, index1, index2);
		}
		else
		{
			AStar(directDistances, roadDistances, cities, index1, index2);
		}*/
		bestFirstSearch(directDistances, roadDistances, cities, index1, index2);
		AStar(directDistances, roadDistances, cities, index1, index2);
	}
	else
	{
		cout<<"No such cities!"<<endl;
	}
}

vector<vector<int>> readDirectDistances(int numberCities)
{
	ifstream inFile("direct.txt");
	vector<vector<int>> directDistances(numberCities, vector<int>(numberCities));
	for(int i=0;i<numberCities;i++)
	{
		for(int j=0;j<numberCities;j++)
			inFile>>directDistances[i][j];
	}
	return directDistances;
}

vector<vector<int>> readRoadDistances(int numberCities)
{
	ifstream inFile("roads.txt");
	vector<vector<int>> roadDistances(numberCities, vector<int>(numberCities));
	for(int i=0;i<numberCities;i++)
	{
		for(int j=0;j<numberCities;j++)
			inFile>>roadDistances[i][j];
	}
	return roadDistances;
}

vector<string> readCities(int numberOfCities, string country)
{
	vector<string> cities(numberOfCities);
	ifstream inFile("cities.txt");
	for(int i=0;i<numberOfCities;i++)
	{
		inFile>>cities[i];
		int position=cities[i].find(","+country);
		if(position!=string::npos) cities[i].erase(position);
	}
	return cities;
}

void printMatrix(vector<vector<int>> matrix)
{
	for(int i=0;i<matrix.size();i++)
	{
		for(int j=0;j<matrix[i].size();j++)
			cout<<setw(4)<<matrix[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
}

void bestFirstSearch(vector<vector<int>> directWays, vector<vector<int>> roads, vector<string> cities, int start, int end)
{
	int numberOfCities=cities.size();
	priority_queue<pair<int,int>> open;
	vector<int> from(numberOfCities, -1), g(numberOfCities, INT_MAX), f(numberOfCities, INT_MAX);
	vector<bool>isOpen(numberOfCities, true);
	g[start]=0;
	f[start]=directWays[start][end];
	auto startVerticle=make_pair(-f[start], start);
	open.push(startVerticle);
	while(!open.empty())
	{
		auto current=open.top();
		open.pop();
		if(isOpen[current.second]==false)
			continue;
		isOpen[current.second]=false;
		if(current.second==end)
		{
			printResults(start, end, from, cities, g[current.second]);
			return;
		}
		for(int i=0;i<numberOfCities;i++)
		{
			if((roads[current.second][i]!=0)&&(isOpen[i]==true)&&(g[current.second]+roads[current.second][i]<g[i]))
			{
				from[i]=current.second;
				g[i]=g[current.second]+roads[current.second][i];
				f[i]=directWays[i][end];
				open.push(make_pair(-f[i], i));
			}
		}
	}
	cout<<"Fail! Way is not found!"<<endl;
}

void AStar(vector<vector<int>> directWays, vector<vector<int>> roads, vector<string> cities, int start, int end)
{
	int numberOfCities=cities.size();
	priority_queue<pair<int,int>> open;
	vector<int> from(numberOfCities, -1), g(numberOfCities, INT_MAX), f(numberOfCities, INT_MAX);
	vector<bool>isOpen(numberOfCities, true);
	g[start]=0;
	f[start]=g[start]+directWays[start][end];
	auto startVerticle=make_pair(-f[start], start);
	open.push(startVerticle);
	while(!open.empty())
	{
		auto current=open.top();
		open.pop();
		if(isOpen[current.second]==false)
			continue;
		isOpen[current.second]=false;
		if(current.second==end)
		{
			printResults(start, end, from, cities, g[current.second]);
			return;
		}
		for(int i=0;i<numberOfCities;i++)
		{
			if((roads[current.second][i]!=0)&&(isOpen[i]==true)&&(g[current.second]+roads[current.second][i]<g[i]))
			{
				from[i]=current.second;
				g[i]=g[current.second]+roads[current.second][i];
				f[i]=g[i]+directWays[i][end];
				open.push(make_pair(-f[i], i));
			}
		}
	}
	cout<<"Fail! Way is not found!"<<endl;
}

void printResults(int start, int end, vector<int> from, vector<string> cities, int length)
{
	cout<<cities[start]<<'-'<<cities[end]<<endl;
	cout<<"Length: "<<length<<endl;
	cout<<"Route: ";
	stack<int>way;
	int verticle=end;
	while(verticle!=start)
	{	
		way.push(verticle);
		verticle=from[verticle];
	}
	way.push(start);
	while(!way.empty())
	{
		cout<<cities[way.top()];
		if(way.top()!=end) cout<<'-';
		way.pop();
	}
	cout<<endl;	
}