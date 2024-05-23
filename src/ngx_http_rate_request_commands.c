#include "ngx_http_rate_request_commands.h"

char *ngx_http_rate_request(ngx_conf_t *cf, __attribute__((unused)) ngx_command_t *cmd, void *conf);

char *ngx_http_rate_request_set(ngx_conf_t *cf, __attribute__((unused)) ngx_command_t *cmd, void *conf);

ngx_command_t ngx_http_rate_request_commands[] = {{ngx_string("rate_request"),
                                                   NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
                                                   ngx_http_rate_request,
                                                   NGX_HTTP_LOC_CONF_OFFSET,
                                                   0,
                                                   NULL},
                                                  {ngx_string("rate_request_set"),
                                                   NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE2,
                                                   ngx_http_rate_request_set,
                                                   NGX_HTTP_LOC_CONF_OFFSET,
                                                   0,
                                                   NULL},
                                                  ngx_null_command};


char *ngx_http_rate_request(ngx_conf_t *cf, __attribute__((unused)) ngx_command_t *cmd, void *conf) {
    ngx_http_rate_request_conf_t *rrcf = conf;

    ngx_str_t *value;

    if (rrcf->uri.data != NULL) {
        return "is duplicate";
    }

    value = cf->args->elts;

    if (ngx_strcmp(value[1].data, "off") == 0) {
        rrcf->uri.len = 0;
        rrcf->uri.data = (u_char *) "";

        return NGX_CONF_OK;
    }

    rrcf->uri = value[1];

    return NGX_CONF_OK;
}

char *ngx_http_rate_request_set(ngx_conf_t *cf, __attribute__((unused)) ngx_command_t *cmd, void *conf) {
    ngx_http_rate_request_conf_t *rrcf = conf;

    ngx_str_t *value;
    ngx_http_variable_t *v;
    ngx_http_rate_request_variable_t *av;
    ngx_http_compile_complex_value_t ccv;

    value = cf->args->elts;

    if (value[1].data[0] != '$') {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "invalid variable name \"%V\"", &value[1]);
        return NGX_CONF_ERROR;
    }

    value[1].len--;
    value[1].data++;

    if (rrcf->vars == NGX_CONF_UNSET_PTR) {
        rrcf->vars = ngx_array_create(cf->pool, 1, sizeof(ngx_http_rate_request_variable_t));
        if (rrcf->vars == NULL) {
            return NGX_CONF_ERROR;
        }
    }

    av = ngx_array_push(rrcf->vars);
    if (av == NULL) {
        return NGX_CONF_ERROR;
    }

    v = ngx_http_add_variable(cf, &value[1], NGX_HTTP_VAR_CHANGEABLE);
    if (v == NULL) {
        return NGX_CONF_ERROR;
    }

    av->index = ngx_http_get_variable_index(cf, &value[1]);
    if (av->index == NGX_ERROR) {
        return NGX_CONF_ERROR;
    }

    if (v->get_handler == NULL) {
        v->get_handler = ngx_http_rate_request_variable;
        v->data = (uintptr_t) av;
    }

    av->set_handler = v->set_handler;

    ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));

    ccv.cf = cf;
    ccv.value = &value[2];
    ccv.complex_value = &av->value;

    if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}

