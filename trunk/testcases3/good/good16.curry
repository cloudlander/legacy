class Cow {
  int height;
  int weight;

  void Init(int w, int h)
  {
    weight = w;
    height = h;
  }

  void Moo() {
    Print ( this.height, " ", this.weight, "\n" );
  }
}


void main()
{
  Cow betsy;
  betsy = New(Cow);
  betsy.Init(100, 122);
  betsy.Moo();
}
