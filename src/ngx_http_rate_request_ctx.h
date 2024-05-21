#ifndef NGINX_NGX_HTTP_RATE_REQUEST_CTX_H
#define NGINX_NGX_HTTP_RATE_REQUEST_CTX_H

#include <ngx_core.h>
#include <ngx_http.h>

typedef struct ngx_http_rate_request_ctx_s {
    ngx_uint_t done;
    ngx_int_t status;
    ngx_http_request_t *subrequest;
} ngx_http_rate_request_ctx_t;

#endif //NGINX_NGX_HTTP_RATE_REQUEST_CTX_H
