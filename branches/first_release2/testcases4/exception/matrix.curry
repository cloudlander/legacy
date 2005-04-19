class Matrix {
  // these methods are intended to be overridden by subclasses
  // if only we had pure virtual/abstract methods in our language...
  void Init() {}
  void Set(int x, int y, int value) {} 
  int Get(int x, int y) {return 1;}

  void PrintMatrix() {
    int i;
    int j;
    for (i = 0; i < 10; i = i + 1) {
      for (j = 0; j < 10; j = j + 1) 
	   Print(Get(i,j), "\t");
      Print("\n");
    }
  }
  void SeedMatrix() {
    int i;
    int j;
    for (i = 0; i < 5; i = i + 1) 
      for (j = 0; j < 5; j = j + 1) 
	   Set(i,j, i+j);
    Set(2,3,4);
    Set(4,6,2);
    Set(2,3,5);
    Set(0,0,1);
    Set(1,6,3);
    Set(7,7,7);
  }
}


class SparseItem {
  int data;
  int y;
  SparseItem next;
  void Init(int d, int y, SparseItem next) {
    this.data = d;
    this.y = y;
    this.next = next;
  }
  SparseItem GetNext() { return next;}
  int GetY() { return y;}
  int GetData() { return data;}
  void SetData(int val) { data = val;}
  
}

// Matrix as linked lists
class SparseMatrix extends Matrix {
  SparseItem[] m;
  void Init() {
    int i;

    // create mem
    m = NewArray(10, SparseItem);
    for (i = 0; i < 10; i = i + 1)
      m[i] = null;

  }
  
  SparseItem Find(int x, int y) {
    SparseItem elem;
    elem = m[x];
    while (elem != null) {
      if (elem.GetY() == y) {
	return elem;
      }
      elem = elem.GetNext();
    }
    return null;
  }
  
  void Set(int x, int y, int value) {
    SparseItem elem;
	try
	{
    	elem = Find(x,y);
    	if (elem != null)
		{
      		elem.SetData(value);
    	} 
		else 
		{
			throw New(SparseItem);
    	}
	}
	catch(SparseItem newelem)
	{
      	newelem.Init(value, y, m[x]);
      	m[x] = newelem;
	}
  }
  
  int Get(int x, int y) {
    SparseItem elem;
	try
	{
    	elem = Find(x,y);
    	if (elem != null)
		{
      		return elem.GetData();
    	}
		else 
		{
      		throw 0;
    	}
	}
	catch(int a)
	{
		return a;
	}
	return 1;
  }
}

void main()  {

  Matrix m;  
  Print("Sparse Rep \n");
  m = New(SparseMatrix);
  m.Init();
  m.SeedMatrix();
  m.PrintMatrix();
}
