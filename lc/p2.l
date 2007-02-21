program d;
integer: i,j,k;
integer: array g[10,10,10]
{demo for multi array}
begin
	for i:=0 to 9 do
		for j:=0 to 9 do
			for k:=0 to 9 do
			begin
				g[i,j,k]:=i+j*k
			end;

	for i:=0 to 9 do
		for j:=0 to 9 do
			for k:=0 to 9 do
			begin
				if g[i,j,k] = i+j*k then
					write "g[",i,",",j,",",k,"]=",g[i,j,k],"\n"
			end
end 
