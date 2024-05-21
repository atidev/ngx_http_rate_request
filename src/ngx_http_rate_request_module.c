#include "ngx_http_rate_request_module.h"

ngx_module_t ngx_http_rate_request_module = {NGX_MODULE_V1, &ngx_http_rate_request_module_ctx,     /* module context */
                                             ngx_http_rate_request_commands,        /* module directives */
                                             NGX_HTTP_MODULE,                       /* module type */
                                             NULL,                                  /* init master */
                                             NULL,                                  /* init module */
                                             NULL,                                  /* init process */
                                             NULL,                                  /* init thread */
                                             NULL,                                  /* exit thread */
                                             NULL,                                  /* exit process */
                                             NULL,                                  /* exit master */
                                             NGX_MODULE_V1_PADDING};