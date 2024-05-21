#include "ngx_http_rate_request_handler.h"
#include "ngx_http_rate_request_variables.h"

static ngx_int_t
ngx_http_rate_request_handler_handle_result(ngx_http_request_t *r, ngx_http_rate_request_ctx_t *ctx, ngx_http_rate_request_conf_t *rrcf);

static ngx_int_t ngx_http_rate_request_handler_begin_subrequest(ngx_http_request_t *r, ngx_http_rate_request_conf_t *rrcf);

ngx_int_t ngx_http_rate_request_handler(ngx_http_request_t *r) {
    ngx_http_rate_request_ctx_t *ctx;
    ngx_http_rate_request_conf_t *rrcf;

    rrcf = ngx_http_get_module_loc_conf(r, ngx_http_rate_request_module);

    if (rrcf->uri.len == 0) {
        return NGX_DECLINED;
    }

    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "rate request handler");

    ctx = ngx_http_get_module_ctx(r, ngx_http_rate_request_module);

    if (ctx != NULL) {
        return ngx_http_rate_request_handler_handle_result(r, ctx, rrcf);
    }

    return ngx_http_rate_request_handler_begin_subrequest(r, rrcf);
}

static ngx_int_t
ngx_http_rate_request_handler_handle_result(ngx_http_request_t *r, ngx_http_rate_request_ctx_t *ctx, ngx_http_rate_request_conf_t *rrcf) {
    if (!ctx->done) {
        return NGX_AGAIN;
    }

    if (ngx_http_rate_request_set_variables(r, rrcf, ctx) != NGX_OK) {
        return NGX_ERROR;
    }

    if (ctx->status == NGX_HTTP_TOO_MANY_REQUESTS || ctx->status == NGX_HTTP_FORBIDDEN) {
        return ctx->status;
    }

    if (ctx->status >= NGX_HTTP_OK && ctx->status < NGX_HTTP_SPECIAL_RESPONSE) {
        return NGX_OK;
    }

    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "rate request unexpected status: %ui", ctx->status);

    return NGX_HTTP_INTERNAL_SERVER_ERROR;
}

static ngx_int_t ngx_http_rate_request_done(ngx_http_request_t *r, void *data, ngx_int_t rc);

static ngx_int_t ngx_http_rate_request_handler_begin_subrequest(ngx_http_request_t *r, ngx_http_rate_request_conf_t *rrcf) {
    ngx_http_request_t *sr;
    ngx_http_post_subrequest_t *ps;
    ngx_http_rate_request_ctx_t *ctx;

    ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_rate_request_ctx_t));
    if (ctx == NULL) {
        return NGX_ERROR;
    }

    ps = ngx_palloc(r->pool, sizeof(ngx_http_post_subrequest_t));
    if (ps == NULL) {
        return NGX_ERROR;
    }

    ps->handler = ngx_http_rate_request_done;
    ps->data = ctx;

    if (ngx_http_subrequest(r, &rrcf->uri, NULL, &sr, ps, NGX_HTTP_SUBREQUEST_WAITED) != NGX_OK) {
        return NGX_ERROR;
    }

    sr->request_body = ngx_pcalloc(r->pool, sizeof(ngx_http_request_body_t));
    if (sr->request_body == NULL) {
        return NGX_ERROR;
    }

    sr->header_only = 1;

    ctx->subrequest = sr;

    ngx_http_set_ctx(r, ctx, ngx_http_rate_request_module)

    return NGX_AGAIN;
}

static ngx_int_t ngx_http_rate_request_done(ngx_http_request_t *r, void *data, ngx_int_t rc) {
    ngx_http_rate_request_ctx_t *ctx = data;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "rate request done s:%ui", r->headers_out.status);

    ctx->done = 1;
    ctx->status = (ngx_int_t) r->headers_out.status;

    return rc;
}
