#include<iostream>
using namespace std;

class Test
{
public:
	Test()
	:a(2)
	{}
private:
	int a;
};

int main()
{
	Test t();
	cout<<"hello C++"<<endl;
	//cout<<"t.a::"<<t<<endl;
	return 0;
}

