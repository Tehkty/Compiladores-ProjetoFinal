program fibonacci;
var
  n, a, b, i, temp: integer;
begin
  n := argint(1);
  a := 0;
  b := 1;
  i := 0;
  while i < n do
  begin
    temp := a + b;
    a := b;
    b := temp;
    i := i + 1;
  end;
  writeln(a);
end.
