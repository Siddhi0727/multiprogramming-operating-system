#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <string>
using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

char M[300][4];
char buffer[40];
char IR[4];
char R[5];
int IC;
int C;
int SI;
int PI;
int TI;
int PTR;
bool breakFlag;
vector<int> check_rand;

struct PCB
{
    int job_id;
    int TTL;
    int TLL;
    int TTC;
    int LLC;

    void setPCB(int id, int ttl, int tll)
    {
        job_id = id;
        TTL = ttl;
        TLL = tll;
        TTC = 0;
        LLC = 0;
    }
};

PCB pcb;

string error[9] = {"No Error", "Out of Data", "Line Limit Exceeded", "Time Limit Exceeded",
                   "Operation Code Error", "Operand Error", "Invalid Page Fault", "Time Limit Exceeded with opcode", "Time Limit Exceeded with oprand"};

void init();
void read(int RA);
void write(int RA);
int addressMap(int VA);
void execute_user_program();
void start_execution();
int allocate();
void load();

void init()
{
    memset(M, '\0', 1200);
    memset(IR, '\0', 4);
    memset(R, '\0', 4);
    C = 0;
    SI = 0;
    PI = 0;
    TI = 0;
    breakFlag = false;
}

void Terminate(int EM)
{

    if (EM == 0)
    {
        fout << "Job id " << pcb.job_id << " terminated normally. " << error[EM] << endl;
        fout << "Instruction Counter = " << IC << endl;
        fout << "Instruction Register = " << IR << endl;
        fout << "C= " << C << endl;
        fout << "R= " << R << endl;
        fout << "Total Time Limit= " << pcb.TTL << endl;
        fout << "Total Time Counter = " << pcb.TTC << endl;
        fout << "Total Line Limit= " << pcb.TLL << endl;
        fout << "Lime Limit Counter= " << pcb.LLC << endl
             << endl
             << endl;
    }
    else
    {
        fout << "Job id " << pcb.job_id << " terminated abnormally due to " << error[EM] << endl;
        fout << "Instruction Counter = " << IC << endl;
        fout << "Instruction Register = " << IR << endl;
        fout << "C= " << C << endl;
        fout << "R= " << R << endl;
        fout << "Total Time Limit= " << pcb.TTL << endl;
        fout << "Total Time Counter = " << pcb.TTC << endl;
        fout << "Total Line Limit= " << pcb.TLL << endl;
        fout << "Lime Limit Counter= " << pcb.LLC << endl
             << endl
             << endl;
    }
}

void read(int RA)
{
    fin.getline(buffer, 41);

    char temp[5];
    memset(temp, '\0', 5);
    memcpy(temp, buffer, 4);

    if (!strcmp(temp, "$END"))
    {
        Terminate(1);
        breakFlag = true;
    }
    else
    {
        strcpy(M[RA], buffer);
    }
}

void write(int RA)
{
    if (pcb.LLC + 1 > pcb.TLL)
    {
        Terminate(2);
        breakFlag = true;
    }
    else
    {
        string str;

        int k = 0;
        for (int i = RA; i < (RA + 10); i++)
        {
            for (int j = 0; j < 4; j++)
                str += M[i][j];
        }
        fout << str << endl;
        pcb.LLC++;
    }
}

int mos(int RA = 0)
{
    if (TI == 0)
    {
        if (SI != 0)
        {
            switch (SI)
            {
            case 1:
                read(RA);
                break;
            case 2:
                write(RA);
                break;
            case 3:
                Terminate(0);
                breakFlag = true;
                break;
            default:
                cout << "Error with SI." << endl;
            }
            SI = 0;
        }
        else if (PI != 0)
        {
            switch (PI)
            {
            case 1:
                Terminate(4);
                breakFlag = true;
                break;
            case 2:
                Terminate(5);
                breakFlag = true;
                break;
            case 3:
                PI = 0;

                char temp[3];
                memset(temp, '\0', 3);
                memcpy(temp, IR, 2);

                if (!strcmp(temp, "GD") || !strcmp(temp, "SR"))
                {
                    int m;
                    do
                    {
                        m = allocate();
                    } while (M[m * 10][0] != '\0');

                    int currPTR = PTR;
                    while (M[currPTR][0] != '*')
                        currPTR++;
                    cout << "Current Page Tbale Register " << currPTR << endl;
                    itoa(m, M[currPTR], 10);

                    cout << "Valid Page Fault, page frame = " << m << endl;
                    cout << "Page Table Register = " << PTR << endl;

                    cout << endl;

                    if (pcb.TTC + 1 > pcb.TTL)
                    {
                        TI = 2;
                        PI = 3;
                        mos();
                        break;
                    }
                    return 1;
                }
                else if (!strcmp(temp, "PD") || !strcmp(temp, "LR") || !strcmp(temp, "H") || !strcmp(temp, "CR") || !strcmp(temp, "BT"))
                {
                    Terminate(6);
                    breakFlag = true;

                    if (pcb.TTC + 1 > pcb.TTL)
                    {
                        TI = 2;
                        PI = 3;
                        mos();
                        break;
                    }
                }
                else
                {
                    PI = 1;
                    mos();
                }
                return 0;
            default:
                cout << "Error with PI." << endl;
            }
            PI = 0;
        }
    }
    else
    {
        if (SI != 0)
        {
            switch (SI)
            {
            case 1:
                Terminate(3);
                breakFlag = true;
                break;
            case 2:
                write(RA);
                if (!breakFlag)
                    Terminate(3);
                break;
            case 3:
                Terminate(0);
                breakFlag = true;
                break;
            default:
                cout << "Error with SI." << endl;
            }
            SI = 0;
        }
        else if (PI != 0)
        {
            switch (PI)
            {
            case 1:
                Terminate(7);
                breakFlag = true;
                break;
            case 2:
                Terminate(8);
                breakFlag = true;
                break;
            case 3:
                Terminate(3);
                breakFlag = true;
                break;
            default:
                cout << "Error with PI." << endl;
            }
            PI = 0;
        }
    }

    return 0;
}

void Simulation()
{
    pcb.TTC++;
    cout << "Total Time Counter " << pcb.TTC << endl;
    if (pcb.TTC + 1 > pcb.TTL)
    {
        TI = 2;
    }
}
int addressMap(int VA)
{
    cout << "Virtual Adderess " << VA << endl;
    if (0 <= VA && VA < 100)
    {
        int pte = PTR + VA / 10; // 112
        cout << "Page Table Register " << pte / 10 << endl;
        if (M[pte][0] == '*')
        {
            PI = 3;
            return 0;
        }
        cout << "Virtual Adderess = " << VA << " Page Table Entry = " << pte << endl;
        return atoi(M[pte]) * 10 + VA % 10;
    }
    PI = 2;
    return 0;
}

void execute_user_program()
{
    char temp[3], loca[2];
    int locIR, RA;

    while (true)
    {
        if (breakFlag)
            break;

        RA = addressMap(IC);
        if (PI != 0)
        {
            if (mos())
            {
                continue;
            }
            break;
        }

        memcpy(IR, M[RA], 4);
        IC += 1;
        cout << endl;
        memset(temp, '\0', 3);
        memcpy(temp, IR, 2);
        for (int i = 0; i < 2; i++)
        {
            if (!((47 < IR[i + 2] && IR[i + 2] < 58) || IR[i + 2] == 0))
            {
                PI = 2;
                break;
            }
            loca[i] = IR[i + 2];
        }

        if (PI != 0)
        {
            mos();
            break;
        }

        locIR = atoi(loca);

        RA = addressMap(locIR);
        if (PI != 0)
        {
            if (mos())
            {
                IC--;
                continue;
            }
            break;
        }

        if (!strcmp(temp, "LR"))
        {
            Simulation();
            for (int i = 0; i < 4; i++)
            {
                cout << "m(ra)->" << M[RA][i];
            }
            cout << endl;
            for (int i = 0; i < 4; i++)
            {
                R[i] = M[RA][i];
            }

            cout << "LR R-> " << R << endl;
        }
        else if (!strcmp(temp, "SR"))
        {
            Simulation();
            cout << "SR-> " << R << endl;
            cout << "RAAAAA->" << RA << endl;
            for (int i = 0; i < 4; i++)
            {
                M[RA][i] = R[i];
            }
        }
        else if (!strcmp(temp, "CR"))
        {
            Simulation();
            if (!strcmp(R, M[RA]))
                C = 1;
            else
                C = 0;
        }
        else if (!strcmp(temp, "BT"))
        {
            Simulation();
            if (C == 1)
                IC = RA;
        }
        else if (!strcmp(temp, "GD"))
        {
            pcb.TTC++;
            SI = 1;
            Simulation();
            mos(RA);
        }
        else if (!strcmp(temp, "PD"))
        {
            SI = 2;
            Simulation();
            mos(RA);
        }
        else if (!strcmp(temp, "H"))
        {
            SI = 3;
            Simulation();
            mos();

            break;
        }
        else
        {
            PI = 1;
            mos();
            break;
        }
        memset(IR, '\0', 4);
    }
    for (int i = 0; i < 300; i++)
    {
        cout << i << " ";

        for (int j = 0; j < 4; j++)
        {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

void start_execution()
{
    IC = 0;
    execute_user_program();
}

int allocate()
{
    srand(time(0));
    srand((unsigned)time(NULL));
    int random = rand() % 30;
    if (check_rand.size() == 0)
    {
        check_rand.push_back(random);
        return check_rand[0];
    }
    // for (int i = 0; i < check_rand.size(); i++)
    // {
    //     if (random == check_rand[i])
    //     {
    //         return allocate();
    //     }
    // }
    check_rand.push_back(random);

    return check_rand[check_rand.size() - 1];
}

void load()
{
    int m;
    int currPTR;
    char temp[5];
    memset(buffer, '\0', 40);

    while (!fin.eof())
    {
        fin.getline(buffer, 41);
        memset(temp, '\0', 5);
        memcpy(temp, buffer, 4);

        if (!strcmp(temp, "$AMJ"))
        {
            init();

            int jobId, TTL, TLL;
            memcpy(temp, buffer + 4, 4);
            jobId = abs(atoi(temp));
            memcpy(temp, buffer + 8, 4);
            TTL = abs(atoi(temp));
            cout << "TTL:-   " << TTL << endl;
            memcpy(temp, buffer + 12, 4);
            TLL = abs(atoi(temp));
            pcb.setPCB(jobId, TTL, TLL);

            PTR = allocate() * 10;
            memset(M[PTR], '*', 40);
            currPTR = PTR;
        }
        else if (!strcmp(temp, "$DTA"))
        {
            start_execution();
        }
        else if (!strcmp(temp, "$END"))
        {
            continue;
        }
        else
        {
            if (breakFlag)
                continue;

            do
            {
                m = allocate();

            } while (M[m * 10][0] != '\0');
            itoa(m, M[currPTR], 10);
            currPTR++;

            strcpy(M[m * 10], buffer);

            cout << "PTR = " << PTR << endl;

            cout << endl;
        }
    }
}

int main()
{
    load();
    fin.close();
    fout.close();
    return 0;
}
