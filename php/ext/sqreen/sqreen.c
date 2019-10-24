/* sqreen extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "ext/standard/url.h"
#include "php_sqreen.h"
#include "ext/sqreen/libinjection/libinjection.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ ZEND_RESULT_CODE no_sql_injection(char* data)
 */
ZEND_RESULT_CODE no_sql_injection(char* data)
{
	int result = SUCCESS;
	if (NULL != data) {
		size_t strLen = strlen(data);
		if (strLen > 0) {
			char fingerprint[9] = { '0' };
			size_t resSize = php_url_decode(data, strLen);
			result = libinjection_sqli((const char*)data, strlen, fingerprint);
		}
	}
    return result;
}
/* }}} */


/* {{{ ZEND_RESULT_CODE analysePart(char* data, char what[], char key[], int trackWhat)
 */
ZEND_RESULT_CODE analysePart(char* data, char what[], char key[], int trackWhat)
{
	char* value = NULL;
	if (strncasecmp(data, what, strlen(data)) == 0) {
		if (zend_is_auto_global_str(key, strlen(key)) == 1) {
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(PG(http_globals)[trackWhat]), value) {
				if (no_sql_injection(value) < 0) {
					return FAILURE;
				}
			} ZEND_HASH_FOREACH_END();
		}
	}
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(sqreen)
{
#if defined(ZTS) && defined(COMPILE_DL_SQREEN)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	char* data = NULL;
	char* value = NULL;

	char _server[] = "_SERVER";
	size_t strLen = strlen(_server);
    // Retrieve SERVER data
	if (zend_is_auto_global_str(_server, strLen) == 1) {
		// Retrieve REQUEST_URI
        if ((data = zend_hash_str_find(Z_ARRVAL(PG(http_globals)[TRACK_VARS_SERVER]), ZEND_STRL("REQUEST_URI")))) {
            if (no_sql_injection(data) == FAILURE) {
                return FAILURE;
            }
        }

        // Retrieve METHOD
        if ((data = zend_hash_str_find(Z_ARRVAL(PG(http_globals)[TRACK_VARS_SERVER]), ZEND_STRL("REQUEST_METHOD")))) {
            if (Z_TYPE_P(data) == IS_STRING) {
				ZEND_RESULT_CODE result = analysePart(data, "GET", "_GET", TRACK_VARS_GET);
				if (result == FAILURE) {
					return result;
				}
				else {
					ZEND_RESULT_CODE result = analysePart(data, "POST", "_POST", TRACK_VARS_GET);
					if (result == FAILURE) {
						return result;
					}
				}
            }
        }
    }

    // Retrieve COOKIE data
    if (zend_is_auto_global_str(ZEND_STRL("_COOKIE")) == 1) {
        ZEND_HASH_FOREACH_VAL(Z_ARRVAL(PG(http_globals)[TRACK_VARS_COOKIE]), value) {
            if (no_sql_injection(value) < 0) {
                return FAILURE;
            }
        } ZEND_HASH_FOREACH_END();
    }

	return SUCCESS;
}

/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(sqreen)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "sqreen support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */

/* }}} */

/* {{{ sqreen_functions[]
 */
/* }}} */

/* {{{ sqreen_module_entry
 */
zend_module_entry sqreen_module_entry = {
	STANDARD_MODULE_HEADER,
	"sqreen",					/* Extension name */
	NULL,           			/* zend_function_entry */
	NULL,						/* PHP_MINIT - Module initialization */
	NULL,						/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(sqreen),			/* PHP_RINIT - Request initialization */
	NULL,						/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(sqreen),			/* PHP_MINFO - Module info */
	PHP_SQREEN_VERSION,		    /* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SQREEN
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(sqreen)
#endif
