#ifndef NGINX_NGX_HTTP_RATE_REQUEST_COMMANDS_H
#define NGINX_NGX_HTTP_RATE_REQUEST_COMMANDS_H

#include <ngx_core.h>
#include <ngx_http.h>

#include "ngx_http_rate_request_conf.h"
#include "ngx_http_rate_request_variables.h"


char *ngx_http_rate_request(ngx_conf_t *cf, __attribute__((unused)) ngx_command_t *cmd, void *conf);

char *ngx_http_rate_request_set(ngx_conf_t *cf, __attribute__((unused)) ngx_command_t *cmd, void *conf);

static ngx_command_t ngx_http_rate_request_commands[] = {{ngx_string("rate_request"),
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

#endif //NGINX_NGX_HTTP_RATE_REQUEST_COMMANDS_H
