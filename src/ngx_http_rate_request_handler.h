#ifndef NGINX_NGX_HTTP_RATE_REQUEST_HANDLER_H
#define NGINX_NGX_HTTP_RATE_REQUEST_HANDLER_H

#include "ngx_http_rate_request_module_ctx.h"
#include "ngx_http_rate_request_ctx.h"
#include "ngx_http_rate_request_module.h"

ngx_int_t ngx_http_rate_request_handler(ngx_http_request_t *r);

#endif //NGINX_NGX_HTTP_RATE_REQUEST_HANDLER_H
