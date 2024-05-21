#include "ngx_http_rate_request_module_ctx.h"

ngx_int_t ngx_http_rate_request_init(ngx_conf_t *cf);

ngx_http_module_t ngx_http_rate_request_module_ctx = {NULL,                                  /* preconfiguration */
                                                      ngx_http_rate_request_init,            /* post-configuration */

                                                      NULL,                                  /* create main configuration */
                                                      NULL,                                  /* init main configuration */

                                                      NULL,                                  /* create server configuration */
                                                      NULL,                                  /* merge server configuration */

                                                      ngx_http_rate_request_create_conf,     /* create location configuration */
                                                      ngx_http_rate_request_merge_conf       /* merge location configuration */
};

ngx_int_t ngx_http_rate_request_init(ngx_conf_t *cf) {
    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_rate_request_handler;

    return NGX_OK;
}


