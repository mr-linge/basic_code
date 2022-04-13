双向认证的关键点在以下几个函数（服务端和客户端都一样），其他就不细说参看代码注释：

SSL_CTX_set_verify----配置启用双向认证

SSL_CTX_load_verify_locations----加载信任的根证书

SSL_CTX_use_certificate_file----加载自己的证书

SSL_CTX_use_PrivateKey_file----加载自己的私钥

SSL_get_verify_result----真正进行验证，一定要调用这个函数不然前面四个光配置而已并不会进行双向验证
