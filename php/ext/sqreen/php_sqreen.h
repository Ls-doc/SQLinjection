/* sqreen extension for PHP */

#ifndef PHP_SQREEN_H
# define PHP_SQREEN_H

extern zend_module_entry sqreen_module_entry;
# define phpext_sqreen_ptr &sqreen_module_entry

# define PHP_SQREEN_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_SQREEN)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_SQREEN_H */
