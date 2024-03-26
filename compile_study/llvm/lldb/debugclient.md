# lldb 连接 debugserver

## 转发 debugserver 监听的端口

iproxy [port] [port]            </br>
如:                             </br>
iproxy 8090 8090                </br>

## 打开 lldb

lldb

### 连接 debugserver

process connect connect://[host]:[port]             </br>
如:         </br>
process connect connect://127.0.0.1:8090            </br>
