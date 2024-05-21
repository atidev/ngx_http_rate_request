#include "ngx_http_rate_request_variables.h"


ngx_int_t ngx_http_rate_request_variable(ngx_http_request_t *r, ngx_http_variable_value_t *v, __attribute__((unused)) uintptr_t data) {
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "rate request variable");

    v->not_found = 1;

    return NGX_OK;
}

ngx_int_t ngx_http_rate_request_set_variables(ngx_http_request_t *r, ngx_http_rate_request_conf_t *rrcf, ngx_http_rate_request_ctx_t *ctx) {
    ngx_str_t val;
    ngx_http_variable_t *v;
    ngx_http_variable_value_t *vv;
    ngx_http_rate_request_variable_t *av, *last;
    ngx_http_core_main_conf_t *cmcf;

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "rate request set variables");

    if (rrcf->vars == NULL) {
        return NGX_OK;
    }

    cmcf = ngx_http_get_module_main_conf(r, ngx_http_core_module);
    v = cmcf->variables.elts;

    av = rrcf->vars->elts;
    last = av + rrcf->vars->nelts;

    while (av < last) {
        vv = &r->variables[av->index];

        if (ngx_http_complex_value(ctx->subrequest, &av->value, &val) != NGX_OK) {
            return NGX_ERROR;
        }

        vv->valid = 1;
        vv->not_found = 0;
        vv->data = val.data;
        vv->len = val.len;

        if (av->set_handler) {
            av->set_handler(r, vv, v[av->index].data);
        }

        av++;
    }

    return NGX_OK;
}