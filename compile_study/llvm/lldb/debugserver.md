# debugserver 附加进程

## 运行可执行文件并附加

debugserver -x backboard [host]:[port] /xxx/executable
options:
    backboard   Launch the executable through BackBoard Services.
    frontboard  Launch the executable through FrontBoard Services.
如:
debugserver -x backboard 127.0.0.1:8090 /var/containers/Bundle/Application/24A5D963-2D45-4BA8-BD01-3532C4AD9AAD/TargetForInjectDyld.app/TargetForInjectDyld

## 附加到已经启动的进程

debugserver host:port --attach [pid]
如:
debugserver 127.0.0.1:8090 --attach 12316
