% -*- coding:utf-8-unix; mode:prolog -*-

test_stream_get_char :-
        write('please type charactoer> '),
        get_char(C),
        write('C="'), write(C), write('"'), nl.

test_stream_get_char_2 :-
        current_input(Ins),
        write('please type charactoer> '),
        get_char(Ins, C),
        write('C="'), write(C), write('"'), nl.

test_stream_get_code :-
        write('please type charactoer> '),
        get_code(C),
        write('C="'), write(C), write('"'), nl.

test_stream_get_code_2 :-
        current_input(Ins),
        write('please type charactoer> '),
        get_code(Ins, C),
        write('C="'), write(C), write('"'), nl.

test_get_key :-
        write('please type key> '),
        get_key(K),
        write('K="'), write(K), write('"'), nl.

test_get_key_2 :-
        current_input(Ins),
        write('please type key> '),
        get_key(Ins, K),
        write('K="'), write(K), write('"'), nl.

test_get_key_no_echo :-
        write('please type key> '),
        get_key_no_echo(K),
        write('K="'), write(K), write('"'), nl.

test_get_key_no_echo_2 :-
        current_input(Ins),
        write('please type key> '),
        get_key_no_echo(Ins, K),
        write('K="'), write(K), write('"'), nl.

test_peek_char :-
        write('please type key> '),
        peek_char(C),
        write('C="'), write(C), write('"'), nl.

test_peek_char_2 :-
        current_input(Ins),
        write('please type key> '),
        peek_char(Ins, C),
        write('C="'), write(C), write('"'), nl.

test_peek_code :-
        write('please type key> '),
        peek_code(C),
        write('C="'), write(C), write('"'), nl.

test_peek_code_2 :-
        current_input(Ins),
        write('please type key> '),
        peek_code(Ins, C),
        write('C="'), write(C), write('"'), nl.

test_unget_char :-
        write('please type key> '),
        get_char(C0),
        unget_char(C0),
        get_char(C1),
        write('C0="'), write(C0), write('"'), nl,
        write('C1="'), write(C1), write('"'), nl.

test_unget_char_2 :-
        current_input(Ins),
        write('please type key> '),
        get_char(Ins, C0),
        unget_char(Ins, C0),
        get_char(Ins, C1),
        write('C0="'), write(C0), write('"'), nl,
        write('C1="'), write(C1), write('"'), nl.

test_unget_code :-
        write('please type key> '),
        get_code(C0),
        unget_code(C0),
        get_code(C1),
        write('C0="'), write(C0), write('"'), nl,
        write('C1="'), write(C1), write('"'), nl.

test_unget_code_2 :-
        current_input(Ins),
        write('please type key> '),
        get_code(Ins, C0),
        unget_code(Ins, C0),
        get_code(C1),
        write('C0="'), write(C0), write('"'), nl,
        write('C1="'), write(C1), write('"'), nl.

