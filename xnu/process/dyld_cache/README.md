# 共享缓存机制

iOS 有很多系统库几乎是每个 App 都会用到的（比如，Foundation.framework、UIKit.framework）
与其等 App 需要时，再将这些系统库一个一个加载进内存；不如在一开始时，就先把这些系统库打包好一次加载进内存
从 iOS 3.1 开始，为了提高系统的性能，所有的系统库文件都被打包合并成了一个大的缓存文件，存放在 /System/Library/Caches/com.apple.dyld/ 目录下（并按不同的 CPU 架构类型分别保存）
并且为了减少冗余，/System/Library/Frameworks/（iOS 用于存放系统库的默认目录）下的系统库文件都被删除掉了

iOS 系统共享缓存的路径为：/System/Library/Caches/com.apple.dyld/
macOS 系统共享缓存的路径为：/var/db/dyld/
