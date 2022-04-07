#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

enum typeOfinstruction
{
    Arithmetic,
    loadstore,
    typeConversion,
    operandStackManagement,
    localVariableManagement
};

/*
1. TypeMisMatch
2. DivideByZero
3. StackFull
4. StackEmpty
5. ArrayOutOfRange
6. UndefinedVariable
@param numberOfexception Type of Exception
*/
void StackFrame::raiseException(int numberOfexception)
{
    IsExcep = true;
    switch (numberOfexception)
    {
    case 1:
    {
        throw TypeMisMatch(PC);
        break;
    }
    case 2:
    {
        throw DivideByZero(PC);
        break;
    }
    case 3:
    {
        throw StackFull(PC);
        break;
    }
    case 4:
    {
        throw StackEmpty(PC);
        break;
    }
    case 5:
    {
        throw ArrayOutOfRange(PC);
        break;
    }
    case 6:
    {
        throw UndefinedVariable(PC);
        break;
    }
    }
}

/*
StackFrame definition
*/
StackFrame::StackFrame()
{
    opStackMaxSize = OPERAND_STACK_MAX_SIZE;
    localVarArrSize = LOCAL_VARIABLE_ARRAY_SIZE;

    OperandStack = new Stack<my_union>();
    LocalVariableSpace = new my_union[localVarArrSize];

    IsInit = new bool[localVarArrSize]{
        false,
    };
    PC = 0;
    IsExcep = false;
}
StackFrame::~StackFrame()
{
    delete OperandStack;
    delete LocalVariableSpace;
    delete IsInit;
}

void StackFrame::runloadstore(std::string instr, my_union val)
{
    if (instr == "iconst" || instr == "fconst")
    {
        if (OperandStack->isFull(opStackMaxSize))
        {
            raiseException(3);
        }
        else
        {
            OperandStack->push(val);
            my_union i1;
            i1.data_bool = instr[0] == 'f';
            OperandStack->push(i1);
        }
    }
    else if (instr == "iload" || instr == "fload")
    {
        my_union ind = val;
        if (ind.data_int >= localVarArrSize )
        {
            raiseException(5);
        }
        else
        {
            if (IsInit[ind.data_int] == false)
            {
                raiseException(6);
            }
            else
            {
                if ((LocalVariableSpace[ind.data_int].data_bool == false && instr[0] == 'f') ||
                    (LocalVariableSpace[ind.data_int].data_bool == true && instr[0] == 'i'))
                {
                    raiseException(1);
                }
                else
                {
                    if (OperandStack->isFull(opStackMaxSize))
                    {
                        raiseException(3);
                    }
                    else
                    {
                        OperandStack->push(LocalVariableSpace[ind.data_int + 1]);
                        OperandStack->push(LocalVariableSpace[ind.data_int]);
                    }
                }
            }
        }
    }
    else if (instr == "istore" || instr == "fstore")
    {
        my_union ind = val;
        if (OperandStack->empty())
        {
            raiseException(4);
        }
        else
        {
            my_union t1, t2;
            t1 = OperandStack->top();
            OperandStack->pop();
            t2 = OperandStack->top();
            OperandStack->pop();

            if (ind.data_int >= localVarArrSize)
            {
                raiseException(5);
            }
            else
            {
                if ((t1.data_bool == false && instr[0] == 'f') ||
                    (t1.data_bool == true && instr[0] == 'i'))
                {
                    raiseException(1);
                }
                else
                {
                    LocalVariableSpace[ind.data_int] = t1;
                    IsInit[ind.data_int] = true;
                    LocalVariableSpace[ind.data_int + 1] = t2;
                    IsInit[ind.data_int + 1] = true;
                }
            }
        }
    }
}

void StackFrame::runLocalVariableManagement(string instr, my_union ind)
{
    // Only Instruction val<int>
    if (ind.data_int >= localVarArrSize)
    {
        raiseException(5);
    }
    else
    {
        if (IsInit[ind.data_int] == false)
        {
            raiseException(6);
        }
        else
        {
            if (LocalVariableSpace[ind.data_int].data_bool == false)
            {
                cout << LocalVariableSpace[ind.data_int + 1].data_int << '\n';
            }
            else
            {
                cout << LocalVariableSpace[ind.data_int + 1].data_float << '\n';
            }
        }
    }
}

void StackFrame::runOperandStackManagement(string instr)
{
    // Only instruction top
    if (OperandStack->empty())
    {
        raiseException(4);
    }
    else
    {
        my_union temp;
        temp = OperandStack->top();
        OperandStack->pop();
        if (temp.data_bool == false)
        {
            cout << OperandStack->top().data_int << '\n';
        }
        else
        {
            cout << OperandStack->top().data_float << '\n';
        }
        OperandStack->push(temp);
    }
}

void StackFrame::runTypeConversion(string instr)
{
    if (OperandStack->empty())
    {
        raiseException(4);
    }
    else
    {
        my_union temp;
        temp = OperandStack->top();
        OperandStack->pop();
        if ((temp.data_bool == false && instr[0] != 'i') ||
            (temp.data_bool == true && instr[0] != 'f'))
        {
            raiseException(1);
        }
        else
        {
            my_union data;
            data = OperandStack->top();
            OperandStack->pop();
            if (instr[2] == 'f')
            {
                data.data_float = float(data.data_int);
            }
            else
            {
                data.data_int = int(data.data_float);
            }
            OperandStack->push(data);
            temp.data_bool = (instr[2] == 'f');
            OperandStack->push(temp);
        }
    }
}

void StackFrame::runArithmetic(string instr)
{
    if (instr != "ibnot" && instr != "ineg" && instr != "fneg")
    {
        if (OperandStack->size() <= 2)
        {
            raiseException(4);
        }
        else
        {
            my_union t1, t2;
            t1 = OperandStack->top();
            OperandStack->pop();
            t2 = OperandStack->top();
            OperandStack->pop();

            my_union v1, v2;
            v1 = OperandStack->top();
            OperandStack->pop();
            v2 = OperandStack->top();
            OperandStack->pop();

            if (instr[0] == 'i')
            {
                if (t1.data_bool || v1.data_bool)
                {
                    raiseException(1);
                }
                else
                {
                    my_union w1, w2;
                    w2.data_bool = false;
                    if (instr == "iadd")
                    {
                        w1.data_int = v2.data_int + t2.data_int;
                    }
                    else if (instr == "isub")
                    {
                        w1.data_int = v2.data_int - t2.data_int;
                    }
                    else if (instr == "imul")
                    {
                        w1.data_int = v2.data_int * t2.data_int;
                    }
                    else if (instr == "idiv")
                    {
                        if (t2.data_int == 0)
                        {
                            raiseException(2);
                        }
                        else
                        {
                            w1.data_int = v2.data_int / t2.data_int;
                        }
                    }
                    else if (instr == "irem")
                    {
                        if (t2.data_int == 0)
                        {
                            raiseException(2);
                        }
                        else
                        {
                            w1.data_int = v2.data_int - (v2.data_int / t2.data_int) * t2.data_int;
                        }
                    }
                    else if (instr == "iand")
                    {
                        w1.data_int = v2.data_int & t2.data_int;
                    }
                    else if (instr == "ior")
                    {
                        w1.data_int = v2.data_int | t2.data_int;
                    }
                    else if (instr == "ieq")
                    {
                        w1.data_int = v2.data_int == t2.data_int;
                    }
                    else if (instr == "ineq")
                    {
                        w1.data_int = v2.data_int != t2.data_int;
                    }
                    else if (instr == "ilt")
                    {
                        w1.data_int = v2.data_int < t2.data_int;
                    }
                    else if (instr == "igt")
                    {
                        w1.data_int = v2.data_int > t2.data_int;
                    }

                    OperandStack->push(w1);
                    OperandStack->push(w2);
                }
            }
            else if (instr[0] == 'f')
            {
                if (!t1.data_bool && !v1.data_bool)
                {
                    t2.data_float = float(t2.data_int);
                    v2.data_float = float(v2.data_int);
                }
                else if (!t1.data_bool && v1.data_bool)
                {
                    t2.data_float = float(t2.data_int);
                }
                else if (t1.data_bool && !v1.data_bool)
                {
                    v2.data_float = float(v2.data_int);
                }
                my_union w1, w2;
                if (instr == "fadd")
                {
                    w1.data_float = v2.data_float + t2.data_float;
                    w2.data_bool = true;
                }
                else if (instr == "fsub")
                {
                    w1.data_float = v2.data_float - t2.data_float;
                    w2.data_bool = true;
                }
                else if (instr == "fmul")
                {
                    w1.data_float = v2.data_float * t2.data_float;
                    w2.data_bool = true;
                }
                else if (instr == "fdiv")
                {
                    if (t2.data_float == 0.0)
                    {
                        raiseException(2);
                    }
                    else
                    {
                        w1.data_float = v2.data_float / t2.data_float;
                        w2.data_bool = true;
                    }
                }
                else if (instr == "feq")
                {
                    w1.data_int = (v2.data_float == t2.data_float) ? 1 : 0;
                    w2.data_bool = false;
                }
                else if (instr == "fneq")
                {
                    w1.data_int = (v2.data_float != t2.data_float) ? 1 : 0;
                    w2.data_bool = false;
                }
                else if (instr == "flt")
                {
                    w1.data_int = (v2.data_float < t2.data_float) ? 1 : 0;
                    w2.data_bool = false;
                }
                else if (instr == "fgt")
                {
                    w1.data_int = (v2.data_float > t2.data_float) ? 1 : 0;
                    w2.data_bool = false;
                }

                OperandStack->push(w1);
                OperandStack->push(w2);
            }
        }
    }
    else if (instr == "ineg" || instr == "fneg")
    {
        if (OperandStack->empty())
        {
            raiseException(4);
        }
        else
        {
            my_union t1, t2;
            t1 = OperandStack->top();
            OperandStack->pop();
            t2 = OperandStack->top();
            OperandStack->pop();

            if (t1.data_bool == true && instr[0] == 'i')
            {
                raiseException(1);
            }
            else if (t1.data_bool == false && instr[0] == 'f')
            {
                t2.data_float = -float(t2.data_int);
                t1.data_bool = true;
            }
            else
            {
                if (instr[0] == 'i')
                {
                    t2.data_int *= -1;
                    t1.data_bool = false;
                }
                else
                {
                    t2.data_float *= -1.0;
                    t1.data_bool = true;
                }
            }
            OperandStack->push(t2);
            OperandStack->push(t1);
        }
    }
    else if (instr == "ibnot")
    {
        if (OperandStack->empty())
        {
            raiseException(4);
        }
        else
        {
            my_union t1, t2;
            t1 = OperandStack->top();
            OperandStack->pop();
            t2 = OperandStack->top();
            OperandStack->pop();
            if (t1.data_bool == true)
            {
                raiseException(1);
            }
            else
            {
                t2.data_int = (t2.data_int == 0) ? 1 : 0;
                OperandStack->push(t2);

                t1.data_bool = false;
                OperandStack->push(t1);
            }
        }
    }
}

void decodeforProcessing(string line, string &instr, my_union &val, typeOfinstruction &typeOfinstr)
{
    int i = 0, size = line.size();
    while (i < size && line[i] != ' ')
    {
        instr += line[i];
        i++;
    }
    if (i < size - 1)
    {
        typeOfinstr = (instr == "val") ? localVariableManagement : loadstore;
        // Get the constant
        string temp = "";
        ++i;
        while (i < size)
        {
            temp += line[i];
            ++i;
        }
        if (instr == "fconst")
        {
            val.data_float = stof(temp);
        }
        else
        {
            val.data_int = stoi(temp);
        }
    }
    else
    {
        if (instr == "top")
        {
            typeOfinstr = operandStackManagement;
        }
        else if (instr == "i2f" || instr == "f2i")
        {
            typeOfinstr = typeConversion;
        }
        else
        {
            typeOfinstr = Arithmetic;
        }
    }
}
void StackFrame::run(string filename)
{
    ifstream file;
    // Open file
    file.open(filename);

    // Declare parameters for instruction
    string instr;
    my_union val;
    typeOfinstruction typeOfinstr;

    while (!file.eof())
    {
        // Fetching line by line
        instr.clear();
        string line;
        getline(file, line);

        // Pre-processing
        decodeforProcessing(line, instr, val, typeOfinstr);

        //  Processing
        ++PC;
        if (typeOfinstr == loadstore)
        {
            runloadstore(instr, val);
        }
        else if (typeOfinstr == localVariableManagement)
        {
            runLocalVariableManagement(instr, val);
        }
        else if (typeOfinstr == operandStackManagement)
        {
            runOperandStackManagement(instr);
        }
        else if (typeOfinstr == typeConversion)
        {
            runTypeConversion(instr);
        }
        else
        {
            runArithmetic(instr);
        }
    }
    file.close();
}
