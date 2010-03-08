/* __header_here__ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include <hunspell.h>
#include <hunspell.h>
#include "php_hunspell.h"

#define HUNSPELL_HANDLE "Hunspell handle"

/* {{{ hunspell_functions[] */
zend_function_entry hunspell_functions[] = {
	PHP_FE(hunspell_create, NULL)
	PHP_FE(hunspell_destroy, NULL)
	PHP_FE(hunspell_spell, NULL)
	PHP_FE(hunspell_get_dic_encoding, NULL)
	PHP_FE(hunspell_suggest, NULL)
	PHP_FE(hunspell_analyze, NULL)
	PHP_FE(hunspell_stem, NULL)
//	PHP_FE(hunspell_stem2, NULL)
//	PHP_FE(hunspell_generate, NULL)
//	PHP_FE(hunspell_generate2, NULL)
//	PHP_FE(hunspell_add, NULL)
//	PHP_FE(hunspell_add_with_affix, NULL)
//	PHP_FE(hunspell_remove, NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ hunspell_module_entry
 */

zend_module_entry hunspell_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"hunspell",
	hunspell_functions,
	PHP_MINIT(hunspell),
	PHP_MSHUTDOWN(hunspell),
	PHP_RINIT(hunspell),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(hunspell),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(hunspell),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_HUNSPELL
ZEND_GET_MODULE(hunspell)
#endif

static int le_handle;

/* {{{ _hunspell_destroy */
static void _hunspell_destroy(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	Hunhandle *handle = (Hunhandle *)rsrc->ptr;

	Hunspell_destroy(handle);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(hunspell)
{
	le_handle = zend_register_list_destructors_ex(_hunspell_destroy, NULL, HUNSPELL_HANDLE, module_number);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(hunspell)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(hunspell)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(hunspell)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(hunspell)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Hunspell Support", "enabled");
	php_info_print_table_end();
}
/* }}} */

PHP_FUNCTION(hunspell_create)
{
	Hunhandle *handle = NULL;
	char *aff_path;
	char *dp_path;
	char *key;
	int aff_len, dp_len, key_len;

	int args = ZEND_NUM_ARGS();
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|s", &aff_path, &aff_len, &dp_path, &dp_len, &key, &key_len) == FAILURE) {
		return;
	}

	if (args == 2) {
		handle = Hunspell_create(aff_path, dp_path);
	} else if (args == 3){
		handle = Hunspell_create_key(aff_path, dp_path, key);
	}


	if (!handle) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Failed to create new Hunspell handle");
		RETURN_FALSE;
	}

	ZEND_REGISTER_RESOURCE(return_value, handle, le_handle)
}

PHP_FUNCTION(hunspell_destroy)
{
	zval *res;
	Hunhandle *handle;

	if (ZEND_NUM_ARGS() < 1) WRONG_PARAM_COUNT;	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &res) == FAILURE) return;

	ZEND_FETCH_RESOURCE(handle, Hunhandle*, &res, -1, HUNSPELL_HANDLE, le_handle);

	zend_list_delete(Z_RESVAL_P(res));

	RETURN_TRUE;
}

PHP_FUNCTION(hunspell_spell)
{
	zval *res;
	char *word;
	int  word_len;
	Hunhandle *handle;

	if (ZEND_NUM_ARGS() < 2) WRONG_PARAM_COUNT;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &word, &word_len) == FAILURE) return;

	ZEND_FETCH_RESOURCE(handle, Hunhandle*, &res, -1, HUNSPELL_HANDLE, le_handle);

	if (Hunspell_spell(handle, word) == 0) {
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}
}

PHP_FUNCTION(hunspell_get_dic_encoding)
{
	zval *res;
	char *enc;
	Hunhandle *handle;

	if (ZEND_NUM_ARGS() < 1) WRONG_PARAM_COUNT;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &res) == FAILURE) return;

	ZEND_FETCH_RESOURCE(handle, Hunhandle*, &res, -1, HUNSPELL_HANDLE, le_handle);

	enc = (char*)Hunspell_get_dic_encoding(handle);

	if (strlen(enc)) {
		RETURN_STRING(enc, 1);
	} else {
		RETURN_FALSE;
	}
}

PHP_FUNCTION(hunspell_suggest)
{
	zval *res;
	char *word, **lst;
	Hunhandle *handle;
	int i, word_len;

	if (ZEND_NUM_ARGS() < 2) WRONG_PARAM_COUNT;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &word, &word_len) == FAILURE) return;

	ZEND_FETCH_RESOURCE(handle, Hunhandle*, &res, -1, HUNSPELL_HANDLE, le_handle);

	int count = Hunspell_suggest(handle, &lst, word);

	array_init(return_value);
	for(i=0; i < count; i++) {
		add_index_string(return_value, i, lst[i], 1);
	}

	Hunspell_free_list(handle, &lst, count);
}

PHP_FUNCTION(hunspell_analyze)
{
	zval *res;
	char *word, **lst;
	Hunhandle *handle;
	int i, word_len;

	if (ZEND_NUM_ARGS() < 2) WRONG_PARAM_COUNT;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &word, &word_len) == FAILURE) return;

	ZEND_FETCH_RESOURCE(handle, Hunhandle*, &res, -1, HUNSPELL_HANDLE, le_handle);

	int count = Hunspell_analyze(handle, &lst, word);

	array_init(return_value);
	for(i=0; i < count; i++) {
		add_index_string(return_value, i, lst[i], 1);
	}

	Hunspell_free_list(handle, &lst, count);
}

PHP_FUNCTION(hunspell_stem)
{
	zval *res;
	char *word, **lst;
	Hunhandle *handle;
	int i, word_len;

	if (ZEND_NUM_ARGS() < 2) WRONG_PARAM_COUNT;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &word, &word_len) == FAILURE) return;

	ZEND_FETCH_RESOURCE(handle, Hunhandle*, &res, -1, HUNSPELL_HANDLE, le_handle);

	int count = Hunspell_stem(handle, &lst, word);

	array_init(return_value);
	for(i=0; i < count; i++) {
		add_index_string(return_value, i, lst[i], 1);
	}

	Hunspell_free_list(handle, lst, count);
}
