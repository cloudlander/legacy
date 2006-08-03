int a;
int b;
int c;
int d;
int e;
int[] f;
int g;
void main() {
 int i;
 a=10000;
 c=2800;
 f=NewArray(2801,int);
/*
 for(;b-c > 0 ;)
 {  
    f[b]=a/5;
    b=b+1;
 }
 d=0;
 g=c*2;
 while(g>0){
    c=c-14;
    Print(e+d/a);
    e=d%a;
 
    b=c;
    d=d+f[b]*a;
    g=g-1;
    f[b]=d%g;
    d=d/g;
    g=g-1;
    b=b-1;
    while(b>0)
    {
       d=d*b;
    d=d+f[b]*a;
    g=g-1;
    f[b]=d%g;
    d=d/g;
    g=g-1;
    b=b-1;
    }
   
    d=0;
    g=c*2;
 }
*/
 for(i=0;i<c;i=i+1)                                                                                                       
     f[i]=a/5;                                                                                                          
 while(c!=0)                                                                                                            
     {                                                                                                                  
         d=0;                                                                                                           
         g=c*2;                                                                                                         
         b=c;                                                                                                           
         while(true)                                                                                                       
            {                                                                                                           
                d=d+f[b]*a;                                                                                             
                g=g-1;                                                                                                    
                f[b]=d%g;                                                                                               
                d=d/g;                                                                                                  
                g=g-1;                                                                                                    
                b=b-1;                                                                                                    
                if(b==0) break;                                                                                         
                d=d*b;                                                                                                  
            }                                                                                                           
         c=c-14;                                                                                                        
//         printf("%.4d",e+d/a);                                                                                          
		Print(e+d/a);
         e=d%a;                                                                                                         
    }                                                                                                                   

/*
 for(;d=0,g=c*2;c -=14,printf("%.4d",e+d/a),e=d%a)


    for(b=c; d+=f[b]*a,f[b]=d%--g,d/=g--,--b; d*=b);
*/
}

