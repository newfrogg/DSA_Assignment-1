#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>

/*
Node Implementation of Union of int, float and bool
*/
union my_union
{
    int data_int;
    float data_float;
    // decide the data type of following by: 0 -> int, 1 -> float
    bool data_bool;
};

template <class T>
class Node
{
public:
    T data;
    Node<T> *next;
};

/*
Stack Implementation
*/
template <class T>
class Stack
{
private:
    Node<T> *head;
    int count;
    // Stack();
    // ~Stack();
    // bool empty();
    // int size();
    // T top();
    // void push(const T &val);
    // void pop();
    // bool isFull(int size);

public:
    Stack() : head(nullptr), count(0){};
    ~Stack()
    {
        Node<T> *temp;
        while (head)
        {
            temp = head;
            head = head->next;
            delete temp;
        }
        head = nullptr;
        count = 0;
    }
    bool empty()
    {
        if (head == nullptr)
            return true;
        else
            return false;
    }
    int size()
    {
        return count;
    }
    T top()
    {
        if (head)
            return head->data;
        else
            exit(1);
    }
    void push(const T &val)
    {
        if (head == nullptr)
        {
            head = new Node<T>;
            head->next = nullptr;
            head->data = val;
        }
        else
        {
            Node<T> *temp = new Node<T>;
            temp->data = val;
            temp->next = head;
            head = temp;
        }
        ++count;
    }
    void pop()
    {
        if (head == nullptr)
        {
            return;
        }
        else
        {
            Node<T> *temp = head;
            head = head->next;
            delete temp;
            --count;
        }
    }
    bool isFull(int size)
    {
        return count == size;
    }
};

/*
StackFrame declaration
*/
class StackFrame
{

private:
    int opStackMaxSize;  // max size of operand stack
    int localVarArrSize; // size of local variable array

    Stack<my_union> *OperandStack;
    my_union *LocalVariableSpace;

public:
    // Init flag of LocalVariableSpace elements
    bool *IsInit;
    // Program counter
    unsigned int PC;
    // Exception flag
    bool IsExcep;

    /*
    Constructor of StackFrame
    */
    StackFrame();

    /*
    Destruction of StackFrame
    */
    ~StackFrame();
    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(std::string filename);

    /*
    Functions to execute
    and throw exceptions bytecodes
    */

    void runloadstore(std::string instr, my_union val);
    void runLocalVariableManagement(std::string instr, my_union ind);
    void runOperandStackManagement(std::string instr);
    void runTypeConversion(std::string instr);
    void runArithmetic(std::string instr);

    /*
    1. TypeMisMatch
    2. DivideByZero
    3. StackFull
    4. StackEmpty
    5. ArrayOutOfRange
    6. UndefinedVariable
    @param numberOfexception an integer in range of [1, 6]
    @return An instance of your exceptions
    */
    void raiseException(int numberOfexception);
};

#endif // !__STACK_FRAME_H__