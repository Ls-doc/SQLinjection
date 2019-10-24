#ifndef PHP_LYTRAX_H
#define PHP_LYTRAX_H

#define PHP_LYTRAX_EXTNAME  "lytrax"
#define PHP_LYTRAX_EXTVER   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
}

extern zend_module_entry lytrax_module_entry;
#define lytrax_module_ptr &lytrax_module_entry
#define phpext_lytrax_ptr lytrax_module_ptr

#endif /* PHP_LYTRAX_H */