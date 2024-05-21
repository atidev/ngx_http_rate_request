#ifndef NGINX_NGX_HTTP_RATE_REQUEST_CONF_H
#define NGINX_NGX_HTTP_RATE_REQUEST_CONF_H

#include <ngx_core.h>

typedef struct ngx_http_rate_request_conf_s {
    ngx_str_t uri;
    ngx_array_t *vars;
} ngx_http_rate_request_conf_t;

void *ngx_http_rate_request_create_conf(ngx_conf_t *cf);

char *ngx_http_rate_request_merge_conf(__attribute__((unused)) ngx_conf_t *cf, void *parent, void *child);

#endif
