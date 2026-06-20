program isprime;
var
  n, i, prime: integer;
begin
  n := argint(1);
  prime := 1;
  if n < 2 then prime := 0;
  i := 2;
  while i * i <= n do
  begin
    if n mod i = 0 then prime := 0;
    i := i + 1;
  end;
  if prime = 1 then writeln("true") else writeln("false");
end.
