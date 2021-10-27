#include <iostream>
#include <queue>
#include <vector>
#include <stack>

using std::vector;
using std::queue;
using std::stack;
using std::cin;
using std::cout;
using std::endl;

const int AMOUNT_NUMBERS = 10000;
const int NONE = 0;
const int INIT = 1;

bool haveZero(int number)
{
    while (number != 0)
    {
        if (number % 10 == 0) return true;

        number /= 10;
    }
    return false;
}

int increaseFirstDigit(int number)
{   
    if (number / 1000 == 9) return number;

    return number + 1000; 
}

int decreaseLastDigit(int number)
{   
    if (number % 10 == 1) return number;

    return number - 1;
}

int decimalShiftRight(int number)
{
    return ((number % 10) * 1000 + number / 10);
}

int decimalShiftLeft(int number)
{
    return ((number % 1000) * 10 + number / 1000);
}

void AddNew(queue<int>& queue, 
            vector<bool>& visited, vector<int>& prev, 
            int parent, int next)
{   

    // cout << "next: " << next << "| parent: " << parent << endl;

    // if (haveZero(next)) cin >> next;

    visited[next] = true;
    prev[next] = parent;
    queue.push(next);
}

void nextStep(queue<int>& queue, 
              vector<bool>& visited, vector<int>& prev, 
              int parent)
{
    int next = increaseFirstDigit(parent);
    if (!visited[next]) AddNew(queue, visited, prev, parent, next);

    next = decreaseLastDigit(parent);
    if (!visited[next]) AddNew(queue, visited, prev, parent, next); 

    next = decimalShiftRight(parent);
    if (!visited[next]) AddNew(queue, visited, prev, parent, next); 

    next = decimalShiftLeft(parent);
    if (!visited[next]) AddNew(queue, visited, prev, parent, next); 
}

void Processing(vector<int>& prev, int from, int to)
{
    queue<int> queue;
    vector<bool> visited(AMOUNT_NUMBERS, false);

    queue.push(from);
    prev[from] = NONE;
    visited[from] = true;

    while (!queue.empty())
    {
        int current = queue.front();
        queue.pop();

        if (current == to) return;

        nextStep(queue, visited, prev, current);
    }
}


void printAnswer(vector<int>& prev, int current, int& path_length)
{
    if (current == NONE) return;

    ++path_length;
    printAnswer(prev, prev[current], path_length);

    if (path_length != -1) 
    {
        cout << path_length << endl;
        path_length = -1;
    }
    cout << current << endl;
}

int main()
{   
    int from;
    int to;
    cin >> from >> to;

    vector<int> prev(AMOUNT_NUMBERS, NONE);

    Processing(prev, from, to);

    int path_length = 0;
    printAnswer(prev, to, path_length);

    return 0;
}