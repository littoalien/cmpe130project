#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <queue>
#include <stack>
#include <list>
#include <algorithm>
#include <utility>

using namespace std;

struct Vertex
{
  string name;

  bool isVisited;
  list<pair<int, double> > adjacentVertices;
  double cost;
  int previous;
};
struct contain
{
  double cost;
  int vertex;
  int previous;

  bool operator<(const contain& v) const
  {
    return cost > v.cost;
  }
};

pair<stack<int>, double> getRoute(int iStart, int iEnd, vector<Vertex>& storage)
{
  pair<stack<int>, double> result;
  list<pair<int, double> >::iterator it; // to iterate over adjacentVertices

for(int z = 0; z < storage.size(); z++)
  {
    storage[z].isVisited = false;
    storage[z].previous = -1;
    storage[z].cost = 0;
  }

  priority_queue<contain> Cont;
  contain s;
  s.vertex = iStart;
  s.previous = -1;
  s.cost = 0;
  Cont.push(s);
  while(!Cont.empty())
  {
    contain y = Cont.top();
    Cont.pop();

    if(storage[y.vertex].isVisited == true) continue;
    storage[y.vertex].isVisited = true;
    storage[y.vertex].cost = y.cost;
    storage[y.vertex].previous = y.previous;
    if(y.vertex == iEnd) break;
    for(it = storage[y.vertex].adjacentVertices.begin(); it != storage[y.vertex].adjacentVertices.end(); it++)
    {
      if(storage[(*it).first].isVisited == false)
      {
        contain neighbor;
        neighbor.cost = (*it).second + y.cost;
        neighbor.previous = y.vertex;
        neighbor.vertex = (*it).first;
        Cont.push(neighbor);
      }
    }
  }

  result.second = storage[iEnd].cost;
  for (int vertex = iEnd; vertex >= 0; vertex = storage[vertex].previous)
  result.first.push(vertex);
  return result;
}

int main()
{
  ifstream fin;
  fin.open("cities.txt");
  if (!fin.good()) throw "I/O error";


  vector<Vertex> storage;
  while (fin.good())
  {
    string originCity, destCity, cost;


    getline(fin, originCity);
    getline(fin, destCity);
    getline(fin, cost);
    fin.ignore(1000, 10);
    originCity.erase(remove_if(originCity.begin(), originCity.end(), ::isspace), originCity.end());
    destCity.erase(remove_if(destCity.begin(), destCity.end(), ::isspace), destCity.end());

    int ToVertex = -1, FromVertex = -1, i;
    for (i = 0; i < storage.size(); i++)
      if (storage[i].name == originCity)
        break;
    if (i == storage.size())
    {
      Vertex fromVertex;
      fromVertex.name = originCity;
      storage.push_back(fromVertex);
    }
    FromVertex = i;

    for (i = 0; i < storage.size(); i++)
      if (storage[i].name == destCity)
        break;
    if (i == storage.size())
    {
      Vertex toVertex;
      toVertex.name = destCity;
      storage.push_back(toVertex);
    }
    ToVertex = i;

    double edgeCost = atof(cost.c_str());
    storage[FromVertex].adjacentVertices.push_back(pair<int, double>(ToVertex, edgeCost));
    storage[ToVertex].adjacentVertices.push_back(pair<int, double>(FromVertex, edgeCost));
  }
  fin.close();
  cout << "******* Car Shortest Path *******\n";
  cout << "******* Cmpe-130 Project  *******\n\n";

  while (true)
  {
    string originCity, destCity;
    cout << "\nEnter the source city [blank to exit]: ";
    getline(cin, originCity);
    if (originCity.length() == 0) break;

    int From;
    for (From = 0; From < storage.size(); From++)
      if (storage[From].name == originCity)
        break;

    cout << "Enter the destination city [blank to exit]: ";
    getline(cin, destCity);
    if (destCity.length() == 0) break;

    int To;
    for (To = 0; To < storage.size(); To++)
      if (storage[To].name == destCity)
        break;

        if(originCity != storage[From].name && destCity != storage[To].name)
        {
            cout<<"No Matching starting and destination cities found!\n";
        }
        else if(originCity != storage[From].name)
        {
            cout<<"No Matching starting point cities found!\n";
        }
        else if(destCity != storage[To].name)
        {
            cout<<"No Matching destination cities found!\n";
        }

    if(From == storage.size() || To == storage.size())
    continue;
    pair<stack<int>, double> result = getRoute(From, To, storage);
    cout << "Total Miles: " << result.second;
    while (!result.first.empty()){cout << '-' << storage[result.first.top()].name; result.first.pop();}
    cout << endl;
  }
}
