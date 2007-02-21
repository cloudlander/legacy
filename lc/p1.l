program d;
const h=1;
integer: i,j,t;
boolean: sum;
integer: temp;
integer: array g[2,11]
{bubble sort application}
begin
	g[1,10]:=1;
	for i:=0 to 9 do
	begin
		write "Input integer for g[",g[1,10],",",i,"]:";
		read t;
		g[g[1,10],i]:=t
	end;
	for i:=1 to 9 do
		for j:=0 to 9-i do
		begin
			t:=j+1;
			if g[g[1,10],j] > g[g[1,10],t] then
			begin
				temp:=g[h,j];
				g[h,j]:=g[h,t];
				g[h,t]:=temp
			end
		end;
	write "After bubble sorting\n";
	for i:=0 to 9 do
		write "g[",h,",",i,"]=",g[h,i],"\n";
	i:=0;
	j:=0;
	while i<=9 do
	begin
	  j:=j+g[h,i];
	  i:=i+1
	end;
	write "Show summary?";
	read sum;
	if  sum then 
	write "j=",j,"\n"
end 
