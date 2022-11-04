#include <iostream>

using namespace std;

struct cool
{
	int a, b;

	cool(int a = 5, int b = 5) : a(a), b(b) {}
};

int main()
{
	cool arr[10] = { };

	for (int i = 0; i < 10; i++) cout << arr[i].a << endl;

	return 0;
}
