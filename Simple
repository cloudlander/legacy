#define maxn 10
typedef int STACK;
STACK stack1[maxn];
STACK stack2[maxn];
int top1;
int top2;
int count;

void toStation()
{
  stack2[--top2];
}

void RtoReady()
{
  int temp;
  temp=stack1[--top1];
  stack2[top2++]=temp;
}

void toStart()
{
  int temp;
  temp=stack2[--top2];
  stack1[top1++]=temp;
}

void toReady(int a)
{
  stack2[top2++]=a;
}

void report()
{
  int i;
  for(i=0;i<top1;i++)
    printf("%3d",stack1[i]);
  printf("\n");
  count++;
}

void train(int ready,int start,int next,int n)
{
  if(start==n){
    report();
    return;
  }
  if(ready==0){
    toReady(next);
    train(ready+1,start,next+1,n);
    toStation();
    return;
  }
  if(next>n){
    toStart();
    train(ready-1,start+1,next,n);
    RtoReady();
    return;
  }
  toReady(next);
  train(ready+1,start,next+1,n);
  toStation();
  toStart();
  train(ready-1,start+1,next,n);
  RtoReady();
  return;
}

void main()
{
  train(0,0,1,4);
  printf("There are %3ds\n\n",count);
}