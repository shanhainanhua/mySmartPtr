#include"core/myUniquePtr.h"
#include"core/myWeakPtr.h"
#include <iostream>
#include <string>
#include<typeinfo>
using namespace std;
class student
{
public:
    student(string n, int a) : name(n), age(a) { cout << "构造student name" << name << " age:" << age << endl; }
    ~student()
    {
        cout << "析构student name" << name << " age:" << age << endl;
    }
    string getName() const
    {
        return name;
    }
    void older(int n)
    {
        age += n;
    }

private:
    string name;
    int age;
};
int main()
{
    //myUniquePtr<student> autos;
    //// miniunique_ptr<student> autos = miniunique_ptr<student>(new student("稳健",88));
    //myUniquePtr<student> autos2(new student("通过指针构造", 1));
    //myUniquePtr<student> unique1;
    ////unique1 = autos;//禁止左值赋值
    //unique1 = myUniquePtr<student>(new student("右值赋值", 2));
    ////auto au3(autos);//禁止左值复制构造
    //auto au3(myUniquePtr<student>(new student("右值拷贝构造", 3)));
    //cout<<"------------------"<<endl;
    //cout<<autos.get()<<endl;

    mySharedPtr<student> autos2(new student("test", 1));
    myWeakPtr<student> w1(autos2);
    mySharedPtr<student> s2=autos2;
    myWeakPtr<student> w2 = w1;
    cout << autos2.use_count() << endl;

}