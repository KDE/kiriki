#!/bin/sh
$EXTRACTRC *.rc >> rc.cpp || exit 11
$EXTRACTRC $(ls . | grep -E '\.ui') >> rc.cpp || exit 12
$XGETTEXT $(ls . | grep -E '\.cpp$') $(ls . | grep -E '\.h$') -o $podir/kiriki.pot
rm -f rc.cpp
