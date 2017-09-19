#ifndef SUDOKUFUNCTIONS_H_INCLUDED
#define SUDOKUFUNCTIONS_H_INCLUDED


class cell
{
    int Value;
    int possCount;
public:
    int possibilities[9];
    void writeValue(int x){Value = x;possCount = 0;}
    int getValue(){return Value;}
    void writePoss(int x)
    {
        possibilities[possCount++] = x;
    }
    int getPossC(){return possCount;}
    void reset()
    {
        for(int i = 0; i<9; ++i){
            possibilities[i] = 0;
        }
        possCount = 0;
    }
    cell()
    {
        possCount = 0;
    }
};

/// DEFINITIONS

int checkRow(cell y[][9],int x,int z);
int checkColumn(cell y[][9],int x,int z);
int checkBox(cell y[][9],int x, int z, int w);
void fillPoss(cell y[][9]);
int searchArray(int y[],int x, int z);
int solvePuzzle(cell puzzle[][9]);			/// SOLVES THE ENTIRE (ALMOST) PUZZLE USING THE OTHER FUNCTIONS
int checkError(cell a[][9]);
void assumtionSolve(cell a[][9]);		/// DOES THE MAGIC AND SOLVES THE ' EXTRA HARD ' PUZZLES

/// END OF DEFINITIONS

int checkRow(cell y[][9],int x,int z)       /// PARAMETERS : GRID ARRAY, VALUE, ROW NUMBER
{
    for(int i = 0; i<9 ; ++i){
        if(y[z][i].getValue()==x){
            return 1;
        }
    }
    return 0;
}

int checkColumn(cell y[][9],int x,int z)   /// PARAMETERS : GRID ARRAY, VALUE, COLUMN NUMBER
{
    for(int i = 0; i<9 ; ++i){
        if(y[i][z].getValue()==x){
            return 1;
        }
    }
    return 0;
}

int checkBox(cell y[][9],int x, int z, int w)  /// PARAMETERS : GRID ARRAY, VALUE, BOX ROW, BOX COLUMN
{
    for(int i = z*3; i<(z*3 + 3); ++i){
        for(int j = w*3; j<(w*3 + 3); ++j){
            if(y[i][j].getValue() == x)
                return 1;
        }
    }
    return 0;
}

void fillPoss(cell y[][9])
{
    for(int i = 0; i<9 ; ++i){
        for(int j = 0; j<9 ; ++j){
            if(y[i][j].getValue() == 0){
            y[i][j].reset();
            for(int k = 1; k<=9 ; ++k){
                if(!((checkBox(y,k,i/3,j/3))||(checkRow(y,k,i))||(checkColumn(y,k,j)))){
                    y[i][j].writePoss(k);
                }
            }
            }
        }
    }
}

int searchArray(int y[],int x, int z)   /// PARAMETERS : ARRAY, SIZE, SEARCH TARGET.
{
    for(int i = 0; i<x ; ++i){
        if(y[i]==z){
            return 1;
        }
    }
    return 0;
}

int solvePuzzle(cell puzzle[][9])
{
    /**  ------------------------------------------------------------------- **/
    /// CHECKS USING VARIOUS METHODS
    // /////////////////////////////////////////////////////////////////////
    int activity = 1;
    while(activity){

        /// USING POSSIBILITIES

        fillPoss(puzzle);
        activity = 0;
        for(int i = 0; i<9 ; ++i){
            for(int j = 0; j<9 ; ++j){
                if(puzzle[i][j].getPossC()==1){
                    int temp = puzzle[i][j].possibilities[0];
                    puzzle[i][j].writeValue(temp);
                    fillPoss(puzzle);
                    activity = 1;
                }
            }
        }

        /// ONLY CELL FOR 'NUMBER', IN 'BOX'

        for(int Brow = 0; Brow < 3 ; Brow++){
        for(int Bcolumn = 0; Bcolumn < 3 ; Bcolumn++){
        for(int k = 1; k<=9 ; ++k){
            int countP = 0,ipos,jpos;
            for(int i = Brow*3; i<(Brow*3+3) ; ++i){
                for(int j = Bcolumn*3; j<(Bcolumn*3 + 3) ; ++j){
                if(puzzle[i][j].getValue() == 0){
                if(searchArray(puzzle[i][j].possibilities,puzzle[i][j].getPossC(),k) == 1){
                    ++countP;
                    ipos = i;
                    jpos = j;
                }
                }
                }
            }
            if(countP == 1){
                puzzle[ipos][jpos].writeValue(k);
                activity = 1;
                fillPoss(puzzle);
            }
        }
        }
        }



    /// ONLY CELL FOR 'NUMBER' IN ROW

    for(int i = 0; i < 9 ; ++i){
        for(int k = 1; k<=9 ; ++k){
            int countP = 0, ipos, jpos;
        for(int j = 0; j<9 ; ++j){
            if(puzzle[i][j].getValue()==0){
            if(searchArray(puzzle[i][j].possibilities,puzzle[i][j].getPossC(),k)==1){
                ++countP;
                ipos = i;
                jpos = j;
            }
            }
        }
            if(countP == 1){
                puzzle[ipos][jpos].writeValue(k);
                activity = 1;
                fillPoss(puzzle);
            }
        }
    }

    /// ONLY CELL FOR 'NUMBER' IN COLUMN

    for(int j = 0; j < 9 ; ++j){
        for(int k = 1; k<=9 ; ++k){
            int countP = 0, ipos, jpos;
        for(int i = 0; i<9 ; ++i){
            if(puzzle[i][j].getValue()==0){
            if(searchArray(puzzle[i][j].possibilities,puzzle[i][j].getPossC(),k)==1){
                ++countP;
                ipos = i;
                jpos = j;
            }
            }
        }
            if(countP == 1){
                puzzle[ipos][jpos].writeValue(k);
                activity = 1;
                fillPoss(puzzle);
            }
        }

    }
    }
}


int checkError(cell a[][9])
{
    for(int i=0; i<9; ++i){
        for(int j=0; j<9; ++j){
            if(a[i][j].getValue()==0 && a[i][j].getPossC()==0)
                return 1;
        }
    }
    return 0;
}

void assumtionSolve(cell a[][9])
{
    for(int i=0; i<9; ++i){
    for(int j=0; j<9; ++j){
        if(a[i][j].getValue()==0){
            for(int k=0; k< a[i][j].getPossC(); ++k){
                cell temp[9][9];
                for(int n=0; n<9; ++n){
                for(int m=0; m<9; ++m)
                    temp[m][n] = a[m][n];
                }
                temp[i][j].writeValue(temp[i][j].possibilities[k]);
                fillPoss(temp);
                if(checkError(temp))
                    continue;
                solvePuzzle(temp);
                if(checkError(temp))
                    continue;
                assumtionSolve(temp);
                if(checkError(temp))
                    continue;
                for(int n=0; n<9; ++n){
                for(int m=0; m<9; ++m)
                    a[m][n] = temp[m][n];
                }
            }
        }
    }
    }
}



#endif // SUDOKUFUNCTIONS_H_INCLUDED
