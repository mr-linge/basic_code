# debugserver 处理

## 导出 debugserver

在一部越狱的iPhone上运行Xcode 项目,运行后 debugserver 会被自动安装到 /Developer/usr/bin 目录.
scp -P 2222 root@localhost:/Developer/usr/bin/debugserver ./
导出debugserver 到电脑上

## 给 debugserver 权限

由于debugserver 缺少 task_for_pid权限,只能调试自己的程序,如若要调试第三方应用就需要添加这个权限,
另外iOS 11 以后运行还需要 platform-application 权限
/usr/bin/codesign -f -s - --entitlements entitlements.plist ./debugserver

## 把 debugserver 放回设备

scp -P 2222 ./debugserver root@localhost:/usr/bin/
添加执行的权限
chmod +x /usr/bin/debugserver
