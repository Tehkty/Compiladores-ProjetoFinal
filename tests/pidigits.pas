program pidigits;
var
  n: integer;
begin
  n := argint(1);
  if n = 1 then writeln("3") else
  if n = 2 then writeln("31") else
  if n = 3 then writeln("314") else
  if n = 4 then writeln("3141") else
  if n = 5 then writeln("31415") else
  if n = 6 then writeln("314159") else
  if n = 7 then writeln("3141592") else
  if n = 8 then writeln("31415926") else
  if n = 9 then writeln("314159265") else
  writeln("3141592653");
end.
