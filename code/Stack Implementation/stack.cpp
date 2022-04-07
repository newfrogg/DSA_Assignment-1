/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>  
using namespace std;

template <class T>

class node {
public:
  T data;
  node<T>* next;
};  

template <class T>

class Stack {
private:
  node<T>* top;
public:
  Stack() {
    top = nullptr;
}
bool isEmpty() {
    if (top == nullptr)
        return false;
    else
        return true;
}
T Top() {
    if (top)
        return top->data;
    else
        cout << "Stack is empty" << endl;
}
void push(const T & val) {
    if (top == nullptr) {
        top = new node<T>;
        top->next = nullptr;
        top->data = val;
    }
    else {
        node<T>* temp = new node<T>;
        temp->data = val;
        temp->next = top;
        top = temp;
    }
}
void pop() {
    if (top == nullptr) {
        cout << "Stack is empty" << endl;
        return;
    }
    else {
        cout << top->data << " is popped" << endl;
        node<T>* temp = top;
        top = top->next;
        delete temp;
    }
}
void print() {
    node<T>* temp = top;
    while (temp != nullptr)
    {
        cout << temp->data << " ";
        temp = temp->next;
    }
   }
   };  
int main()
{
    Stack<int> s;
    s.push(2);
    s.push(6);
    s.print();

    return 0;
}
