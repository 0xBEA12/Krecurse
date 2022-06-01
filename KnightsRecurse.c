#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

/////////////////////////////////////////////////////////////////
double KnightsMultiplyRe(int k, int n)
{
  //essentially a basic factorial function but with bounds
  if(k == n) return n;
  else return(n * KnightsMultiplyRe(k, n-1));
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void KnightsMultiply(FILE* KRI, FILE* KRO)
{
  int k, n;

  fscanf(KRI, "%d %d", &k, &n );
  fprintf(KRO, "KnightsMultiply: %.0lf\n\n", KnightsMultiplyRe(k, n));
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void KnightsFlipRe(int NumFlips, int Position, int Dividend, char* KFArray, FILE* KRO)
{
  //A decimal to binary converter and counter from 0 to 2^n
  //where n is NumFlips and K = 0 and F = 1.
  //Allows this function to scale for any value of n.
  int q, r;
  q = Dividend / 2;
  r = Dividend % 2;

  if(Position == NumFlips)
  {
    //Binary conversion is stored backwards so this for loop
    //is how we flip it to display properly.
    for(int i = NumFlips - 1; i >= 0; i--)
    {
      fprintf(KRO, "%c", KFArray[i]);
    }

    fprintf(KRO, "\n");
    return;
  }
//Converts 0 to K
  if(r == 0)
  {
    KFArray[Position] = 'K';
    KnightsFlipRe(NumFlips, Position + 1, q, KFArray, KRO);
  }
//converts 1 to F
  else if(r == 1)
  {
    KFArray[Position] = 'F';
    KnightsFlipRe(NumFlips, Position + 1, q, KFArray, KRO);
  }


}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void KnightsFlip(FILE* KRI, FILE* KRO)
{
  int NumFlips;

  fscanf(KRI, "%d", &NumFlips);
  fprintf(KRO, "KnightsFlip:\n");
//In binary the number of permutations is always 2^n so we run
//KnightsFlip this many times where n = NumFlips.
  for(int i = 0; i < pow(2, NumFlips) ; i++)
  {
    char* KFArray = malloc((NumFlips + 1) * sizeof(char));
    KFArray[0] = '\0';
    KnightsFlipRe(NumFlips, 0, i,  KFArray, KRO);
    free(KFArray);
  }

  fprintf(KRO, "\n");
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void KnightsShapeRe(int NumX, int NumSpace, int Max, FILE* KRO)
{
//Making any sized X of smaller xs is broken down into 4 cases
  int NegNumX;
//Case 1.1: The upper, or positive half of the X
//Increment spaces at beginning of row by 1 each iteration,
//starting from 0
  for(int i = 0; i < NumSpace; i++)
  {
    fprintf(KRO, " ");
  }
//Case 2: The Center of the X and the only row with one x
//We print the single x and then set up the recursive calls
//for the bottom half of the X
  if(NumX == 1)
  {
    fprintf(KRO, "X\n");
    //subtract 4 from the space between xs to continue spacial
    //pattern into the negative numbers. The pattern is a 
    //difference of 2 between the current row and the next one.
    KnightsShapeRe(NumX - 4, NumSpace - 1, Max, KRO);
    return;
  }
//Case 4: The last row performs just like Case 3
//except that it returns all recursive calls when executed
  if(NumX == 0 - Max)
  {
    fprintf(KRO, "X");
    NegNumX = NumX + 2;
    for(int k = 0; k > NegNumX; k--)
    {
      fprintf(KRO, " ");
    }

    fprintf(KRO, "X\n");
    return;
  }
//Case 3: The bottom, or negative half of the X
//Performs like case 1 except that it uses negative
//integers to determine positioning of spaces and xs
  if(NumX < 0)
  {
    fprintf(KRO, "X");
    NegNumX = NumX + 2;
    for(int k = 0; k > NegNumX; k--)
    {
      fprintf(KRO, " ");
    }

    fprintf(KRO, "X\n");
    KnightsShapeRe(NumX - 2, NumSpace - 1, Max, KRO);
    return;
  }
//Case 1.2: Where the xs and spaces are placed after the
//initial ones placed in 1.1
  fprintf(KRO, "X");
  for(int j = 0; j < NumX - 2; j++)
  {
    fprintf(KRO, " ");
  }

  fprintf(KRO, "X\n");
  //Subtract 2 from spaces between each x in each 
  //proceeding row
  KnightsShapeRe(NumX - 2, NumSpace + 1, Max, KRO);
  return;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void KnightsShape(FILE* KRI, FILE* KRO)
{
  int NumX;
  int NumSpace = 0;

  fscanf(KRI, "%d", &NumX);

  int Max = NumX;

  fprintf(KRO, "KnightsShape:\n");
  KnightsShapeRe(NumX, NumSpace, Max, KRO);
  fprintf(KRO, "\n");
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
int KnightsScotchRe(int Position, int Marker, int Size, int* Line)
{
  //Test right and then left until you reach 0 or can't move anymore.
  int Left = 0, Right = 0;

  if(Line[Position] == 0) return 1;
  Marker = Line[Position];
  //Mark each position so that we don't get stuck in loops.
  Line[Position] = -1;
  //If there is room to go right in the array then do so.
  if(Line[Marker + Position] != -1 && Position + Marker < Size)
  {
    Right = KnightsScotchRe(Position + Marker,  Marker, Size, Line);
  }
  //If there is room to go left in the array then do so.
  else if(Line[Position - Marker] != -1 && Position - Marker >= 0)
  {
    Left = KnightsScotchRe(Position - Marker, Marker, Size, Line);
  }
//Only right will ever be 1 so a truth table shows that the
//solution is always Right OR Left.
  return Right || Left;
}

/////////////////////////////////////////////////////////////////
void KnightsScotch(FILE* KRI, FILE* KRO)
{
  int Position, Marker, Size, Solvable, *Line;

  fscanf(KRI, "%d %d", &Position, &Size);
  Marker = Position;
  Line = malloc(sizeof(Size * sizeof(int)));
  for(int i = 0; i < Size; i++)
  {
   fscanf(KRI, "%d", &Line[i]);
   //printf("%d\n", line[i]);
  } 

  Solvable = KnightsScotchRe(Position, Marker, Size, Line);
  free(Line);
  if(Solvable == 1) fprintf(KRO, "KnightsScotch: Solvable\n\n");
  else if(Solvable == 0) fprintf(KRO, "KnightsScotch: Not Solvable\n\n");
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
int KnightsDepotRe(int FixedLength, int Marker, int Purchased2x4, int Size, int* Array2x4)
{
  //Traverse through Array2x4 with 4 cases.
  int Quotient, Remainder;
//Case 1: The required board length is longer than the fixed 
//length at the store. Divide by FixedLength and add
//remainder to the next cell in Array2x4.
  if(Array2x4[Marker] > FixedLength && Array2x4[Marker + 1] != -1)
  {
    Quotient = Array2x4[Marker] / FixedLength;
    Remainder = Array2x4[Marker] % FixedLength;    
    Array2x4[Marker + 1] += Remainder;
    Purchased2x4 += Quotient;
    return KnightsDepotRe(FixedLength, Marker + 1, Purchased2x4, Size, Array2x4);
  }
//Case 2: The required length is equal to the fixed length.
//Add 1 to Purchased2x4 and continue to next cell.
  else if(Array2x4[Marker] == FixedLength && Array2x4[Marker + 1] != -1)
  {
    Purchased2x4 += 1;
    return KnightsDepotRe(FixedLength, Marker + 1, Purchased2x4, Size, Array2x4);
  }
//Case 3: The required length is less than the fixed length.
//Add the value from this cell to the following one and 
//move to this next cell.
  else if(Array2x4[Marker] < FixedLength && Array2x4[Marker + 1] != -1)
  {
    Array2x4[Marker + 1] += Array2x4[Marker];
    return KnightsDepotRe(FixedLength, Marker + 1, Purchased2x4, Size, Array2x4);
  }
//Case 4: The last cell in the array. When the marker is next
//we take whatever value is left and divide it by FixedLength.
//This quotient is added to Purchased2x4. Any remaining
//values must be less than FixedLength so we translate
//that to a +1 to Purchased2x4.
  else if(Array2x4[Marker + 1] == -1)
  {
    Quotient = Array2x4[Marker] / FixedLength;
    Remainder = Array2x4[Marker] % FixedLength;
    if(Remainder > 0)
    {
      Remainder = 1;
    }

    Purchased2x4 += Quotient + Remainder;
    return Purchased2x4;
  }
  
  free(Array2x4);
  return Purchased2x4;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
void KnightsDepot(FILE* KRI, FILE* KRO)
{ 
  int i, FixedLength, Marker = 0, Purchased2x4 = 0, Size, *Array2x4;

  fprintf(KRO, "KnightsDepot: ");
  fscanf(KRI, "%d %d", &FixedLength, &Size);
  Array2x4 = malloc((Size + 1) * sizeof(int));
  for(i = 0; i < Size; i++)
  {
   fscanf(KRI, "%d", &Array2x4[i]);
  }
//Place a marker at the end of the array so we know when we are there.
  Array2x4[i+1] = -1;
  Purchased2x4 = KnightsDepotRe(FixedLength, Marker, Purchased2x4, Size, Array2x4);
  fprintf(KRO, "%d\n\n", Purchased2x4);
}
/////////////////////////////////////////////////////////////////

int main(void) 
{
  FILE* KRI = fopen("KnightsRecurse.in", "r");
  FILE* KRO = fopen("KnightsRecurse.out", "w");
  char Command[20];

  while(!feof(KRI))
  {
    fscanf(KRI, "%s", Command);

    if(!strcmp(Command, "KnightsMultiply"))
    {
      KnightsMultiply(KRI, KRO);
    }
  
    else if(!strcmp(Command, "KnightsFlip"))
    {
      KnightsFlip(KRI, KRO);
    }

    else if(!strcmp(Command, "KnightsShape"))
    {
      KnightsShape(KRI, KRO);
    }
    
    else if(!strcmp(Command, "KnightsScotch"))
    {
      KnightsScotch(KRI, KRO);
    }

    else if(!strcmp(Command, "KnightsDepot"))
    {
      KnightsDepot(KRI, KRO);
    }

  }
 
  return 0;
}
