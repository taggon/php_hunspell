PHP_ARG_WITH(hunspell, for Hunspell support,[  --with-hunspell[=DIR]   Include Hunspell support])

if test "$PHP_HUNSPELL" != "no"; then
	if test "$PHP_HUNSPELL" != "yes"; then
		HUNSPELL_SEARCH_DIRS=$PHP_HUNSPELL
	else
		HUNSPELL_SEARCH_DIRS="/usr/local /usr"
	fi
	for i in $HUNSPELL_SEARCH_DIRS; do
		if test -f $i/include/hunspell/hunspell.h; then
			HUNSPELL_DIR=$i
			HUNSPELL_INCDIR=$i/include/hunspell
		elif test -f $i/include/hunspell.h; then
			HUNSPELL_DIR=$i
			HUNSPELL_INCDIR=$i/include
		fi
	done

	if test -z "$HUNSPELL_DIR"; then
		AC_MSG_ERROR(Cannot find libhunspell)
	fi

	HUNSPELL_LIBDIR=$HUNSPELL_DIR/lib

	AC_DEFINE(HAVE_HUNSPELL, 1, [Whether you have hunspell])
	PHP_SUBST(HUNSPELL_SHARED_LIBADD)
	PHP_ADD_LIBRARY_WITH_PATH(hunspell, $HUNSPELL_LIBDIR, HUNSPELL_SHARED_LIBADD)
	PHP_ADD_INCLUDE($HUNSPELL_INCDIR)

	PHP_NEW_EXTENSION(hunspell, hunspell.c, $ext_shared)
fi
