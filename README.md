GNU PROLOG with UTF-8 support
=============================

UTF-8 ready variant of [GNU Prolog](http://gprolog.org/).

Description
-----------

Almost same as original [GNU Prolog](http://gprolog.org/), additional features are:
- You can use UTF-8 strings in console.
- You can convert UTF-8 atoms to list of 'Code Point' values (e.g.: atom_codes/2)
- You can use identifiers which start with wide-characters as atoms.
- You can use wide-characters in variables name, and functors name.

Many Japanese writes prolog codes in following rule:
- Identifiers which start wide-character codes are atoms.
- Identifiers which start with '_' + wide-character codes are variables.
So, I modified GNU Prolog parser rule as above.

Background

Most prolog systems (and ISO standards) are case sensitive.
Identifiers that start with uppercase are variables,
identifiers that start with lowercase are atoms.

But East asia languages (also known as 'CJK') characters are caseless caracters.
There are no 'Upper case characters' or 'Lower case characters' in Japanese.

Examples
--------

![hello](https://user-images.githubusercontent.com/28968058/30641273-69acbef8-9e41-11e7-9294-0b9938d61b01.png)

![atom_concat](https://user-images.githubusercontent.com/28968058/30271912-0bf115cc-972d-11e7-8de5-46059d50ea71.png)

Install
-------

    git clone https://github.com/tadashi9e/gprolog-utf8.git
    
    cd src
    autoconf
    ./configure
    make
    (cd doc && make all)
    sudo make install

Requirements
------------

Same as original [GNU Prolog](http://gprolog.org/).

Limitations
----------

I tested this software on x86(32bit/64bit) LINUX boxes only.

Console(Linedit) may be still unstable yet.

License
-------

LGPL (same as original GNU Prolog).
