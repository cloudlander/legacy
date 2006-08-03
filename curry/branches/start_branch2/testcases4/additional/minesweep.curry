/* Decaf Minesweeper
 * Written by Warren Shen
 * 3/16/01
 */


int probOfMine; // probability of planting a mine in a given square

/* This random module is copied
 * from the Blackjack program.  The rest is mine.
 */
class rndModule {

  int seed;

  void Init(int seedVal) {
    seed = seedVal;
  }

  int Random() {
    seed = (15625 * (seed % 10000) + 22221) % 65536;
    return seed;
  }

  int RndInt(int max) {
    return (Random() % max);
  }

}

rndModule gRnd;

class Block
{
  bool hasMine;
  int adjMines;
  bool isUncovered;

  void Init() {
     hasMine = false;
     adjMines = 0;
     isUncovered = false;
  }

  void Uncover() {
    isUncovered = true;
  }

  bool IsUncovered() {
    return isUncovered;
  }

  void SetMine(bool m) {
     hasMine = m;
  }

  bool HasMine() {
     return hasMine;
  }

  void IncrementAdjacents(int i) {
    adjMines = adjMines + i;
  }

  void SetAdjacents(int i) {
    adjMines = i;
  }

  int NumAdjacents() {
    return adjMines;
  }

  void PrintOutput(bool printSolution) {
    if(printSolution) {
      if(hasMine) {
        if(!isUncovered) Print("x");
        else Print("%");
      }
      else Print(adjMines);
      return;
    }
    if(isUncovered) {
      if(!hasMine) Print(adjMines);
    } else {
      Print("+");
    }
  }
}

class Field
{
  int height;
  int width;
  Block [][]arr;
  bool hasNotBlownUp;
  int numOpenBlocks;
  int numBlocksCleared;


  void Init(int h, int w) {
    int i;
    int j;

    height = h;
    width = w;
    hasNotBlownUp = true;
    numOpenBlocks = (h * w);
    numBlocksCleared = 0;
    arr = NewArray(width, Block[]);
    for(i = 0; i < width; i = i + 1) {
      arr[i] = NewArray(height, Block);
    }
    for(i = 0; i < width; i = i + 1) {
      for(j = 0; j < height; j = j + 1) {
        arr[i][j] = New(Block);
        arr[i][j].Init();
      }
    }
    PlantMines();
  }

  int GetWidth() { return width; }
  int GetHeight() { return height; }

  void PlantMines() {
    int i;
    int j;
    for(i = 0; i < width; i = i + 1) {
      for(j = 0; j < height; j = j + 1) {
        PlantOneMine(i, j);
      }
    }
  }

  void PlantOneMine(int i, int j) {
    int x;
    int y;
    int rand;
    rand = gRnd.RndInt(100);
    if(rand < probOfMine) {
      arr[i][j].SetMine(true);
      numOpenBlocks = numOpenBlocks - 1;
      for(x = i - 1; x <= i + 1; x = x + 1) {
        for(y = j - 1; y <= j + 1; y = y + 1) {
          if(x >= 0 && x < width && y >= 0 && y < height) {
            if(!arr[x][y].HasMine()) {
              arr[x][y].IncrementAdjacents(1);
            }
          }
        }
      }
    }
  }

  void PrintField(bool printSolution)
  {
    int i;
    int  j;
    Print("   ");
    for(i = 0; i < width; i = i + 1) {
      Print(i, " ");
    }
    Print("\n +");
    for(i = 0; i <  width; i = i + 1) {
      Print("--");
    }
    Print("\n");
    for(j = 0; j < height; j = j + 1) {
      Print(j, "| ");
      for(i = 0; i < width; i = i + 1) {
        arr[i][j].PrintOutput(printSolution);
        Print(" ");
      }
      Print("\n |\n");
    }
  }

  void Expand(int x, int y)
  {
    int i;
    int j;
    if(arr[x][y].IsUncovered()) return;
    arr[x][y].Uncover();
    if(arr[x][y].HasMine()) {
      hasNotBlownUp = false;
      return;
    }
    numBlocksCleared = numBlocksCleared + 1;
    if(arr[x][y].NumAdjacents() != 0) {
     return;
    }
    for(i = x - 1; i <= x + 1; i = i + 1) {
      for(j = y - 1; j <= y + 1; j = j + 1) {
        if(i >= 0 && i < width && j >= 0 && j < height) {
          Expand(i, j);
        }
      }
    }
  }

  bool HasNotBlownUp() {
    return hasNotBlownUp;
  }

  bool HasClearedEverything() {
    return (numBlocksCleared == numOpenBlocks);
  }

}

class Game
{
   Field field;

   void Init(int width, int height) {
     field = New(Field);
     field.Init(width, height);
   }
   
   void PlayGame() {
     int x;
     int y;
     while(field.HasNotBlownUp() && !field.HasClearedEverything()) {
       field.PrintField(false);
       x = PromptForInt(
   "Enter horizontal coordinate, -1 to quit, -2 for help, -3 for grid: "
           , -3, field.GetWidth()-1);
       if(x == -1) break;
       y = PromptForInt(
           "Enter vertical coordinate, -1 to quit, -2 for help, -3 for grid: "
           , -3, field.GetHeight()-1);
       if(y == -1) break;
       Print("Clearing (", x, ", ",y, ")\n");
       field.Expand(x, y);
     }
     if(field.HasNotBlownUp()) AnnounceWin();
     else AnnounceLoss();
   }

   int PromptForInt(string prompt, int min, int max) {
      int x;
      while(true) {
        Print(prompt);
        x = ReadInteger();
        if(x >= min && x <= max) {
          if(x == -1) return -1;
          if(x == -2) PrintHelp();
          else if(x == -3) field.PrintField(false);
          else return x;
        }
      }
   }

   void AnnounceWin() {
     field.PrintField(true);
     if(field.HasClearedEverything()) Print("You win!  Good job.\n");
     else Print("Quitter!!\n");
   }
   
   void AnnounceLoss() {
     field.PrintField(true);
     Print("Ha ha!! You blew up!!  Ha ha!!\n");
   }

}

void PrintHelp()
{
  Print("Welcome to Low-Fat Decaf Minesweeper!\n");
  Print("On the screen you will see a grid that represents your field.\n");
  Print("In each location there may or may not be a mine hidden.  In \n");
  Print("order to clear the field, enter in the coordinates of the \n");
  Print("location you want to uncover.\n\n");
  Print("As you clear mines, the grid will change.  There are two symbols:\n");
  Print("    '+'  - you haven't uncovered this location yet\n");
  Print("a number - there is no mine here, but there are the specified \n");
  Print("           number of mines directly adjacent to this location\n");
  Print("           (including diagonals)\n");
  Print("The field will keep on expanding from the point you specified\n");
  Print("and clear all adjacent points that have no mines.\n\n");
  Print("If you uncover a location with a mine, you die, and the solution\n");
  Print("will be printed.  The solution will show these symbols: \n");
  Print("    'x'  - this location has a mine\n");
  Print("    '%'  - this is where you blew up\n");
  Print("a number - same as before\n\n");
  Print("You win when you have uncovered all locations without a mine.\n");
  
}

void main()
{
  int w;
  int h;
  Game g;

  gRnd = New(rndModule);
  Print("Please enter in a random seed: ");
  gRnd.Init(ReadInteger());
  Print("How much of the field do you want to have mines? (0%-100%) ");
  probOfMine = ReadInteger();
  Print("How wide do you want the field to be? ");
  w = ReadInteger();
  Print("How tall do you want the field to be? ");
  h = ReadInteger();

  g = New(Game);
  g.Init(w, h);
  g.PlayGame();  
}
