# IDA 连接 debugserver 调试

debugserver 启动后,PC端除了可以通过 lldb 连接接调试外,也可以用 IDA 连接调用. 其他步骤都一样

## IDA 配置

IDA 打开 Mach-O 文件(TargetForCrack)

Debugger --> Switch debugserver
             选择 Remote iOS debugserver

Debugger --> Debugger options
             根据需要配置
Debugger --> Debugger options --> Set spectific option
            Symbol path : ~/Library/Developer/Xcode/iOS DeviceSupport/15.7.8 (19H364)/Symbols
            在 iOS DeviceSupport 里找与目标设备匹配的系统

Process option -->
        Application : /private/var/containers/Bundle/Application/1E0A4897-FC9F-4FCA-BD68-AABB6A0F8E4E/TargetForCrack.app/TargetForCrack(Remote Device 里 Mach-O file)
        input flie : /private/var/containers/Bundle/Application/1E0A4897-FC9F-4FCA-BD68-AABB6A0F8E4E/TargetForCrack.app/TargetForCrack(Remote Device 里 Mach-O file)
        host:   127.0.0.1   port: 8090 (这里要和 Remote device 里 debugerser 启动进程时设置的 host:port 一致)

Debugger --> Attach to process
            选择对应的进程,点 OK
