# abort-in-thread-examples

为了程序健壮性，解决在 libuv 在多线程环境下 abort() 导致整个程序退出的问题。


- example01 多线程 abort() 处理
- example02 abort() 处理包装调用
- example03 结构化异常处理所有错误