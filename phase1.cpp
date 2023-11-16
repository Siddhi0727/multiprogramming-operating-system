#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<string.h>
#include<string.h>
#include<string>


using namespace std;
int p;
int IC,SI,C=0;              
char IR[4],R[4];
char M[100][4];
char buff[42];
string line;
fstream fin,fout;
void load();

void flush()
{
    int i;
    for(i=0;i<40;i++)
    {
        buff[i]=0;
    }
}

void terminateprogram()
{
	cout<<"\n\n";
	load();
}

void read()                                           // Read

{
    int no,i,j,k,z,x;
    getline(fin,line);
    cout<<"In read line" << line<<endl;
    flush();
            no=((IR[2]-48)*10)+(IR[3]-48);
            //cout<<endl<<no;
            //cout<<"line ="<<line<<endl;
            i=0,k=0;
            while(line[i])
            {
                buff[i]=line[i];
                i++;
            }
            
            if(line[0] =='$' && line[1]=='E' && line[2]=='N' && line[3]=='D')
            {
            cout<<"\n\n*********************Start of new Job*******************\n\n";
            flush();
            }

            cout<<endl<<"buff="<<buff<<endl;
            //cout<<"as "<<buff[1];
            k=0;
            x=no;
            for(i=no+1;i<x+(strlen(buff)/4)+2;i++)
            {
                for(j=0;j<4;j++)
                {
                    //cout<<i<<endl;
                   // cout<<"lll"<<line[k];
                    M[no][j]=buff[k];
                    k++;

                }no++;
            }

             for(i=0;i<99;i++)
            {
                cout<<"Memory location "<<i<<"-->  ";
                for(j=0;j<4;j++)
                {

                    cout<<M[i][j];
                    //cout<<endl<<i<<endl;
                }

                cout<<endl;

            }
          //  IC++;


}

void write()                                                 // Write
{
    int no,i,j,k=0;
    cout<<endl<<"in write"<<endl;

    flush();
     no=((IR[2]-48)*10)+(IR[3]-48);

     for(i=0;i<10;i++)
     {
         for(j=0;j<4;j++)
         {buff[k]=M[no][j];k++;}
         no++;

     }
     //IC++;
     fout.write(buff,strlen(buff));

     fout<<endl;
     cout<<"buff = "<<buff<<endl;


}

void MOS()
{
    cout<<"IN mos"<<endl;
    if(SI==1)
    {
        read();
    }
    else if(SI==2)
    {
        write();

    }
    else if(SI==3)
    {
        terminateprogram();            
        
    }

}



void execute()                                       	    // Execute
{
    cout<<"In execute"<<endl;
    char ch;
    int no,i,k,j,position,cmp=1;

    while(fin)
    {
printf("IN WHILE\n");

        for(int j=0;j<4;j++)
        {
            IR[j]=M[IC][j];

        }
        //IC++;

        ch=IR[0];
        //getline(fin,line);
        cout<<"ch : "<<ch<<endl;
        switch(ch)
        {
        case 'G':          // GD

            cout<<"IN switch"<<endl;
            SI=1;
            MOS();        // read function
            break;                 
            
        case 'P':           // PD 
            SI=2;
            MOS();          // write function
            
            //cout<<"\nffff"<<M[no][1]<<endl;

            break;

        case 'L':            // LR - store data from memory to register
            position=((IR[2]-48)*10)+(IR[3]-48);
            cout<<"pos "<<position<<endl;
            for(i=0;i<4;i++)
            {
                R[i]=M[position][i];
            }

            IC++;
            cout<<"R "<<R<<endl;




            break;

        case 'C':           // CR - for comparison
            cout<<"CMP"<<endl;
            position=((IR[2]-48)*10)+(IR[3]-48);
            cout<<"pos "<<position<<endl;
            for(i=0;i<4;i++)
            {
                if(R[i]!=M[position][i])
                {
                    cmp=0;
                }
            }


            cout<<"CMP  "<<cmp<<endl;
            if(cmp==0){C=0;}
            if(cmp==1){C=1;}
            IC++;
            break;
        case 'B':                // BT - for branch condition

            position=((IR[2]-48)*10)+(IR[3]-48);
            cout<<"pos "<<position<<endl;
            if(C==1)
            {IC=position-1;}
            else
                IC++;
            break;

        case 'S':              // SR - store data from register to memory
            cout<<"\nIN SR\n";
            position=((IR[2]-48)*10)+(IR[3]-48);
            for(i=0;i<4;i++)
            {
                M[position][i]=R[i];

            }
            cout<<"\nSR :"<<M[position]<<"\n";
             for(i=0;i<99;i++)
            {
                cout<<"Memory location "<<i<<"-->  ";
                for(int u=0;u<4;u++)
                {

                    cout<<M[i][u];
                    //cout<<endl<<i<<endl;
                }

                cout<<endl;

            }
            IC++;

            break;


        case 'H':          // Halt 
            SI=3;
            MOS();
            break;
        }

    }


     cout<<"IR"<<IR<<endl;

}


void startExe()
{
    IC=0;
    execute();
}



void load()                             		            // Load

{
printf("IN LOAD\n");
    int i,j,k;
    flush();
    while(fin)
    {
        cout<<"in while"<<endl;

        getline(fin,line);
        cout<<line<<"line1"<<endl;
        //cout<<line[1];
        //break;

        if(line[0] =='$' && line[1]=='A' && line[2]=='M' && line[3]=='J')
        {
            //cout<<j;
            //cout<<"in if";
            getline(fin,line);
            i=0;
            while(line[i] != 'H')
            {
                buff[i]=line[i];
                i++;
            }

            buff[i]='H';
            i++;
            j=0;
            for(j=0;j<3;j++)
            {
                buff[i]='0';i++;
            }
            //i=j;
            buff[i]='\0';
            cout<<endl<<buff<<endl;
            //cout<<buff[4]<<endl;

            k=0;
            for(i=0;i<strlen(buff)/4+(1);i++)
            {
                for(j=0;j<4;j++)
                {
                    //buff[k];
                    M[i][j]=buff[k];
                    k++;
                }

            }
             for(i=0;i<99;i++)
            {
                cout<<"Memory location "<<i<<"-->  ";
                for(j=0;j<4;j++)
                {

                    cout<<M[i][j];
                    //cout<<endl<<i<<endl;
                }

                cout<<endl;

            }

        }


        if(line[0] =='$' && line[1]=='D' && line[2]=='T' && line[3]=='A')
        {
            startExe();
        }
        
         


    }

    p=strlen(buff)/4;


    //execute();

}


int main()
{
    fin.open("ipp1.txt");
    fout.open("opp1.txt");
    load();
    return 1;
}