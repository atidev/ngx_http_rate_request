# ngx_http_rate_request

`ngx_http_rate_request` — модуль, позволяющий реализовать распределенное лимитирование запросов через подзапросы.
Если подзапрос получает ответ 2XX, лимитирование не применяется. В случае ответов 403 и 429, дальнейшее выполнение
запроса прерывается с соответствующим кодом ответа. Иные коды ответов считаются ошибкой. Модуль подключается
после `ngx_http_auth_request_module`, что позволяет использовать результат авторизации при лимитировании.

## Установка

```shell
git clone --depth 1 git@github.com:atidev/ngx_http_rate_request.git
RATE_REQUEST_MODULE_PATH=${PWD}/ngx_http_rate_request
...
cd nginx-1.x.x
./configure --add-module=${RATE_REQUEST_MODULE_PATH}
make && make install
``` 

Nginx будет собран с модулем.

## Директивы

### `rate_request`

- синтаксис: `rate_request uri | off;`
- по-умолчанию: `rate_request off;`
- контекст: `http`, `server`, `location`

Включает лимитирование, основанное на результате подзапроса, и устанавливает его URI.

### `rate_request_set`

- синтаксис: `rate_request_set $variable value;`
- по-умолчанию: `—`
- контекст: `http`, `server`, `location`

После выполнения подзапроса устанавливает значение `value` переменной `$variable`. Значение может содержать переменные
из подзапроса: `$upstream_http_*`.

## Пример взаимодействия с `ngx_http_auth_request_module`

```nginx configuration
# основной локейшен    
location / {

    # запрос лимитов
    rate_request /rr;

    # запрос авторизации
    auth_request http://localhost:80/ar;
    
    # установка значения заголовка User-Id из ответа auth_request в переменную $user_id
    auth_request_set $user_id $upstream_http_user_id;

    root   html;
    index  index.html index.htm;
}

# локейшен, имитирующий работу сервиса авторизации
location /ar {
    # установка значения заголовка User-Id
    add_header User-Id 10;
    
    return 200 "okay";
}

# внутрениий локейшен, подготавливающий подзапрос к сервису лимитирования
location = /rr {
    internal;

    # установка значения переменной $user_id основоного запроса в заголовок User-Id
    proxy_set_header User-Id $user_id;
    proxy_pass http://localhost:80/outer_rr;
}

# локейшен, имитирующий работу сервиса лимитирования
location /outer_rr {
    if ($http_user_id = 10) {
        return 200 "okay";
    }
    
    return 429 "slow down"; 
}
```