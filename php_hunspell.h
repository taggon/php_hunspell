/* __header_here__ */

#ifndef PHP_HUNSPELL_H
#define PHP_HUNSPELL_H

extern zend_module_entry hunspell_module_entry;
#define phpext_hunspell_ptr &hunspell_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

#include "hunspell.h"

PHP_MINIT_FUNCTION(hunspell);
PHP_MSHUTDOWN_FUNCTION(hunspell);
PHP_RINIT_FUNCTION(hunspell);
PHP_RSHUTDOWN_FUNCTION(hunspell);
PHP_MINFO_FUNCTION(hunspell);

PHP_FUNCTION(hunspell_create);
PHP_FUNCTION(hunspell_create_key);
PHP_FUNCTION(hunspell_destroy);
PHP_FUNCTION(hunspell_spell);
PHP_FUNCTION(hunspell_get_dic_encoding);
PHP_FUNCTION(hunspell_suggest);
PHP_FUNCTION(hunspell_analyze);
PHP_FUNCTION(hunspell_stem);
//PHP_FUNCTION(hunspell_stem2);
//PHP_FUNCTION(hunspell_generate);
//PHP_FUNCTION(hunspell_generate2);
//PHP_FUNCTION(hunspell_add);
//PHP_FUNCTION(hunspell_add_with_affix);
//PHP_FUNCTION(hunspell_remove);
//PHP_FUNCTION(hunspell_free_list);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(hunspell)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(hunspell)
*/

/* In every utility function you add that needs to use variables 
   in php_hunspell_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as CAIRO_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define HUNSPELL_G(v) TSRMG(hunspell_globals_id, zend_hunspell_globals *, v)
#else
#define HUNSPELL_G(v) (hunspell_globals.v)
#endif

#endif	/* PHP_HUNSPELL_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
