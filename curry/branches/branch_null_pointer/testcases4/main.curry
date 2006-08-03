int main(string[] args)
{
    int i;
    int c;
    bool r;
    string str;
    string prompt;
    str="-p";
    r=false;
    prompt="";
    Print("The command line is:");
    for(i=0;i<args.length();i=i+1)
    {
        Print(args[i]," ");
        if(args[i]==str)
        {
            prompt=args[i+1];
        }
     }
     i=-1;
     c=-2;
     if(prompt!="")
     {
         int i;
         int j;
         c=0;
         Print("\n",prompt,":");
         i=ReadInteger();
         for(j=0;j<i;j=j+1)
         {
            c=c+j;
         }
     }
     else
         Print("\n");
     Print("c=",c,"\t","i=",i,"\n");
     return c;    
}


