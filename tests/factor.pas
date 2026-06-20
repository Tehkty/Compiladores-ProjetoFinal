program factor;
var
  n, d: integer;
begin
  n := argint(1);
  d := 2;
  while n > 1 do
  begin
    while n mod d = 0 do
    begin
      write(d);
      write(" ");
      n := n / d;
    end;
    d := d + 1;
  end;
  writeln();
end.
