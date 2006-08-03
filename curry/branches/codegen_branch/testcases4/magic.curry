void dllb(int l,int si,int sj,int sn,int[][] d)
{
     int n;
     int i;
     int j;
     i=0;
     j=l/2;                        
     for (n=1;n<=l*l;n=n+1){
        d[i+si][j+sj]=n+sn;
        if ((n%l)>0){
            i=(i>0)?(i-1):(l-1);                                                                                          
            j=(j==l-1)?0:(j+1);                                                                                         
        }                                                                                                               
        else                                                                                                            
            i=(i==l-1)?0:(i+1);                                                                                         
    }                                                                                                                   
}                                                                                                                       
                                                                                                                        
void magic_odd(int l,int[][] d){                                                                                    
    dllb(l,0,0,0,d);                                                                                                    
}                                                                                                                       
                                                                                                                        
void magic_4k(int l,int[][] d){                                                                                     
    int i;
	int j;                                                                                                            
    for (i=0;i<l;i=i+1)                                                                                                   
        for (j=0;j<l;j=j+1)                                                                                               
                                                                                                                        
d[i][j]=((i%4==0||i%4==3)&&(j%4==0||j%4==3)||(i%4==1||i%4==2)&&(j%4==1||j%4==                                           
2))?(l*l-(i*l+j)):(i*l+j+1);                                                                                            
}                                                                                                                       
                                                                                                                        
void magic_other(int l,int[][] d){                                                                                  
    int i;
    int j;
    int t;                                                                                                          
    dllb(l/2,0,0,0,d);                                                                                                  
    dllb(l/2,l/2,l/2,l*l/4,d);                                                                                          
    dllb(l/2,0,l/2,l*l/2,d);                                                                                            
    dllb(l/2,l/2,0,l*l/4*3,d);                                                                                          
    for (i=0;i<l/2;i=i+1)                                                                                                 
        for (j=0;j<l/4;j=j+1)                                                                                             
            if (i!=l/4||j>0)                                                                                              
            {
                 t=d[i][j];
                 d[i][j]=d[i+l/2][j];
                 d[i+l/2][j]=t;                                                            }
    t=d[l/4][l/4];
    d[l/4][l/4]=d[l/4+l/2][l/4];
    d[l/4+l/2][l/4]=t;                                                        
    for (i=0;i<l/2;i=i+1)                                                                                                 
        for (j=l-l/4+1;j<l;j=j+1)                                               {                                          
            t=d[i][j];
            d[i][j]=d[i+l/2][j];
           d[i+l/2][j]=t;                                                                }
}                                                                                                                       
                                                                                                                        
void generate(int l,int[][] d){                                                                                     
    if (l%2)                                                                                                            
        magic_odd(l,d);                                                                                                 
    else if (l%4==0)                                                                                                    
        magic_4k(l,d);                                                                                                  
    else                                                                                                                
        magic_other(l,d);                                                                                               
}

void main()
{
  int l;
  int[][] arr;
  int i;
  int j; 
  Print("ENTER THE DIMENSION:");
  l=ReadInteger();
  arr=NewArray(l,int[]);

  for(i=0;i<arr.length();i=i+1)
     arr[i]=NewArray(l,int);

  generate(l,arr);
  for(i=0;i<arr.length();i=i+1)
  {
    for(j=0;j<arr[i].length();j=j+1)
      Print(arr[i][j],"\t");
    Print("\n");
}
}
