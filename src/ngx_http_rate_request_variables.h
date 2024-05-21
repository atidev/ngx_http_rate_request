#ifndef NGINX_NGX_HTTP_RATE_REQUEST_VARIABLES_H
#define NGINX_NGX_HTTP_RATE_REQUEST_VARIABLES_H


#include <ngx_config.h>
#include <ngx_http.h>
#include "ngx_http_rate_request_conf.h"
#include "ngx_http_rate_request_ctx.h"

typedef struct ngx_http_rate_request_variable_s {
    ngx_int_t index;
    ngx_http_complex_value_t value;
    ngx_http_set_variable_pt set_handler;
} ngx_http_rate_request_variable_t;


ngx_int_t
ngx_http_rate_request_variable(ngx_http_request_t *r, ngx_http_variable_value_t *v, __attribute__((unused)) uintptr_t data);

ngx_int_t
ngx_http_rate_request_set_variables(ngx_http_request_t *r, ngx_http_rate_request_conf_t *rrcf,
                                    ngx_http_rate_request_ctx_t *ctx);

#endif //NGINX_NGX_HTTP_RATE_REQUEST_VARIABLES_H
