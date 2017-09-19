#include<iostream>
#include<fstream>
#include<conio.h>
#include<time.h>
#include<string.h>
#include "sudokuFunctions.h"

using namespace std;

int main()
{
    cout<<"\n\n\t\tTHE SUDOKU SOLVER";
    cout<<"\n\n\t\tThis software will solve an entered sudoku puzzle.";
    cout<<"\n\t\tPlease enter the numbers as you see them in the grid,\n";
    cout<<"\t\tand enter 0 wherever blank.";
    cout<<"\n\n\t\tPlease enter 1, if you wish to enter the puzzle manually,\n";
    cout<<"\t\tor 2, if you want to load it from a (txt) file.\n\t\t";
    int ch;
    char puzfile[30];
    char puzzleNumber[4];

    cin>>ch;
    do{
    switch(ch)
    {
    case 2:
        cout<<"\n\n\t\tPlease enter full name of file(EX : puzzle.txt)\n\t\t";
        cin>>puzfile;
        break;
    case 1:
        break;
    default:
        cout<<"\n\t\tInvalid input.";
    }
    }while(ch != 1 && ch != 2);
    cell puzzle[9][9];
    system("cls");
    ifstream fin;
    fin.open(puzfile);
    if(fin == 0 && ch == 2){
        cout<<"\n\t\tThe file does not exist, please enter manually.\n";
        ch = 1;
        getch();
    }
    system("cls");
    for(int i = 0; i<9 ; i++){
        for(int j = 0; j<9 ; ++j){
            int temp;
            if(ch == 1)
                cin>>temp;
            else
                fin>>temp;
            puzzle[i][j].writeValue(temp);
        }
    }
    fin.close();

    solvePuzzle(puzzle);
    assumtionSolve(puzzle);

    time_t present = time(0);
    char* solutionFile = ctime(&present);
    solutionFile[10] = '\0';
    strcat(solutionFile,".txt");

    ofstream fout;
    fout.open(solutionFile,ios::app);

    system("cls");
    cout<<"\n\n\t\tTHE SUDOKU SOLVER";
    cout<<"\n\n\n";
    fout<<"\n\n-------------------------------------------------------------------------\n";
    fout<<ctime(&present);
    for(int i = 0; i<9 ; ++i){
        if(i%3 == 0){
            cout<<"\n\t\t\t-------------------------------\n\t\t\t|";
            fout<<"\n\t\t\t-------------------------------\n\t\t\t|";
        }
        else{
            cout<<"\n\t\t\t|";
            fout<<"\n\t\t\t|";
        }
        for(int j = 0; j<9; ++j){
            if(puzzle[i][j].getValue() == 0){
                cout<<"   ";
                fout<<"   ";
            }
            else{
                fout<<" "<<puzzle[i][j].getValue()<<" ";
                cout<<" "<<puzzle[i][j].getValue()<<" ";
            }
            if((j+1)%3 == 0){
                cout<<"|";
                fout<<"|";
            }
        }
    }
    cout<<"\n\t\t\t-------------------------------\n";
    fout<<"\n\t\t\t-------------------------------\n";
    fout.close();
    cout<<"\n\nThe solution has been written onto a file in the solutions folder.";
    getch();
}
