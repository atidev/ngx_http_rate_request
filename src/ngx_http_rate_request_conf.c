#include "ngx_http_rate_request_conf.h"

void *ngx_http_rate_request_create_conf(ngx_conf_t *cf) {
    ngx_http_rate_request_conf_t *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_rate_request_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    conf->vars = NGX_CONF_UNSET_PTR;

    return conf;
}


char *ngx_http_rate_request_merge_conf(__attribute__((unused)) ngx_conf_t *cf, void *parent, void *child) {
    ngx_http_rate_request_conf_t *prev = parent;
    ngx_http_rate_request_conf_t *conf = child;

    ngx_conf_merge_str_value(conf->uri, prev->uri, "")
    ngx_conf_merge_ptr_value(conf->vars, prev->vars, NULL)

    return NGX_CONF_OK;
}