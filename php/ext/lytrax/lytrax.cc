#include "php_lytrax.h"
#include "test.h"

zend_object_handlers test_object_handlers;

typedef struct _test_object {
    Test *test;
    zend_object std;
} test_object;

static inline test_object *php_test_obj_from_obj(zend_object *obj) {
    return (test_object*)((char*)(obj) - XtOffsetOf(test_object, std));
}

#define Z_TSTOBJ_P(zv)  php_test_obj_from_obj(Z_OBJ_P((zv)))

zend_class_entry *test_ce;

PHP_METHOD(Test, __construct)
{
    long maxGear;
    zval *id = getThis();
    test_object *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &maxGear) == FAILURE) {
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if(intern != NULL) {
        intern->test = new Test(maxGear);
    }
}

PHP_METHOD(Test, shift)
{
    long gear;
    zval *id = getThis();
    test_object *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &gear) == FAILURE) {
        RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(id);
    if(intern != NULL) {
        intern->test->shift(gear);
    }
}

PHP_METHOD(Test, doTest)
{
    zval *args = NULL;
    int argc, i;

    zval retval;
    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;

    zval *id = getThis();
    test_object *intern;

    intern = Z_TSTOBJ_P(id);
    if(intern != NULL) {
        memcpy(&fci, &intern->test->fci_onTest, sizeof(fci));
        memcpy(&fci_cache, &intern->test->fcc_onTest, sizeof(fci_cache));
        fci.retval = &retval;

        ZEND_PARSE_PARAMETERS_START(0, -1)
            Z_PARAM_VARIADIC('*', args, argc)
        ZEND_PARSE_PARAMETERS_END();

        if(argc > 0) {
            fci.params = args;
            fci.param_count = argc;
        }

        if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(retval) != IS_UNDEF) {
            if (Z_ISREF(retval)) {
                zend_unwrap_reference(&retval);
            }
            ZVAL_COPY_VALUE(return_value, &retval);
        }
    }
}

PHP_METHOD(Test, onTest)
{
    zval *args = NULL;
    int argc, i;

    zval *id = getThis();
    test_object *intern;

    intern = Z_TSTOBJ_P(id);
    if(intern != NULL) {
        ZEND_PARSE_PARAMETERS_START(1, -1)
            Z_PARAM_FUNC(intern->test->fci_onTest, intern->test->fcc_onTest)
            Z_PARAM_VARIADIC('*', args, argc)
        ZEND_PARSE_PARAMETERS_END();
    }

    intern->test->fci_onTest.param_count = argc;
    if(argc > 0) {
        intern->test->fci_onTest.params = (zval*)safe_emalloc(intern->test->fci_onTest.param_count, sizeof(zval), 0);
        for(i = 0; i < argc; i++) {
            zval *arg = args + i;
            ZVAL_COPY_VALUE(&intern->test->fci_onTest.params[i], arg);
        }
    }
}

PHP_METHOD(Test, testCallback)
{
    zval retval;
    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;

    ZEND_PARSE_PARAMETERS_START(1, -1)
        Z_PARAM_FUNC(fci, fci_cache)
        Z_PARAM_VARIADIC('*', fci.params, fci.param_count)
    ZEND_PARSE_PARAMETERS_END();

    fci.retval = &retval;

    if (zend_call_function(&fci, &fci_cache) == SUCCESS && Z_TYPE(retval) != IS_UNDEF) {
        if (Z_ISREF(retval)) {
            zend_unwrap_reference(&retval);
        }
        ZVAL_COPY_VALUE(return_value, &retval);
    }
}

PHP_METHOD(Test, getCurrentGear)
{
    zval *id = getThis();
    test_object *intern;

    intern = Z_TSTOBJ_P(id);
    if(intern != NULL) {
        RETURN_LONG(intern->test->getCurrentGear());
    }
    RETURN_NULL();
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_testcallback, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, cbfn, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(arginfo_ontest, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, cbfn, 0)
ZEND_END_ARG_INFO();

const zend_function_entry test_methods[] = {
    PHP_ME(Test,  __construct,     NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(Test,  shift,           NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Test,  testCallback,    arginfo_testcallback, ZEND_ACC_PUBLIC)
    PHP_ME(Test,  onTest,          arginfo_ontest, ZEND_ACC_PUBLIC)
    PHP_ME(Test,  doTest,          NULL, ZEND_ACC_PUBLIC)
    PHP_ME(Test,  getCurrentGear,  NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

zend_object *test_object_new(zend_class_entry *ce TSRMLS_DC)
{
    test_object *intern = (test_object*)ecalloc(1,
            sizeof(test_object) +
            zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &test_object_handlers;

    return &intern->std;
}

static void test_object_destroy(zend_object *object)
{
    test_object *my_obj;
    my_obj = (test_object*)((char *)object - XtOffsetOf(test_object, std));

    /* Now we could do something with my_obj->my_custom_buffer, like sending it
       on a socket, or flush it to a file, or whatever, but not free it here */

    zend_objects_destroy_object(object); /* call __destruct() from userland */
}

static void test_object_free(zend_object *object)
{
    test_object *my_obj;
    my_obj = (test_object *)((char *)object - XtOffsetOf(test_object, std));
    delete my_obj->test;
    zend_object_std_dtor(object); /* call Zend's free handler, which will free object properties */
}

PHP_MINIT_FUNCTION(lytrax)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Lytrax\\Test", test_methods);
    test_ce = zend_register_internal_class(&ce TSRMLS_CC);
    test_ce->create_object = test_object_new;

    memcpy(&test_object_handlers, zend_get_std_object_handlers(), sizeof(test_object_handlers));

    test_object_handlers.free_obj = test_object_free; /* This is the free handler */
    test_object_handlers.dtor_obj = test_object_destroy; /* This is the dtor handler */
    test_object_handlers.offset   = XtOffsetOf(test_object, std); /* Here, we declare the offset to the engine */

    return SUCCESS;
}

zend_module_entry lytrax_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_LYTRAX_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(lytrax),
    NULL,                  /* MSHUTDOWN */
    NULL,                  /* RINIT */
    NULL,                  /* RSHUTDOWN */
    NULL,                  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_LYTRAX_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_LYTRAX
extern "C" {
ZEND_GET_MODULE(lytrax)
}
#endif