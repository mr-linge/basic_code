# attribute

__attribute__ 可以设置函数属性(Function Attribute)、变量属性(Variable Attribute)和类型属性(Type Attribute)      </br>
__attribute__ 语法格式为: __attribute__ ((attribute-list))

## 内存对齐 aligned, packed

__attribute__可以对结构体(struct)或共用体(union)进行属性设置        </br>
</br>
需要注意的是,attribute属性的作用与链接器也有关,如果链接器最大只支持16字节对齐,那么你此时定义32字节对齐也不会生效

## 利用 constructor 属性完成 module_init() 设计模式

### 功能

若函数被设定为 constructor 属性,则该函数会在 main() 函数执行之前被自动的执行                             </br>
若函数被设定为 destructor  属性,则该函数会在 main() 函数执行之后或者 exit() 被调用后被自动的执行

### 单线程执行

当存在多个 __attribute__((constructor)) 修饰的函数时,也不用持锁,因为是单线程执行的

### 优先级

使用 __attribute__((constructor(prio))) 指定优先级,从而决定调用次序,优先级prio取值范围是 0--65535,其中 0--100 是预留的不能使用,prio数值越小优先级越高越先被执行,就算是指定的优先级是65535也还是会比没有指定优先级的先调用
