% -*- coding:utf-8-unix; mode:prolog -*-
atom_concat_test :-
  Z = おはよう,
  (
   repeat,
   atom_concat(X,Y,Z),
   write(X), nl,
   Y == '',
   !
  ).
