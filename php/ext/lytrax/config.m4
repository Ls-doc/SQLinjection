PHP_ARG_ENABLE(lytrax,
    [Whether to enable the "Lytrax" extension],
    [  --enable-lytrax         Enable "Lytrax" extension support])

if test $PHP_LYTRAX != "no"; then
    PHP_REQUIRE_CXX()
    PHP_SUBST(LYTRAX_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, LYTRAX_SHARED_LIBADD)
    PHP_NEW_EXTENSION(lytrax, lytrax.cc test.cc, $ext_shared)
fi