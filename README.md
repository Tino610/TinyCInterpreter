# TinyCInterpreter
## 作用
tcc脚本交互速度非常的快，同时完全支持c99。对某些复杂的配置场景，引入tcc作为配置脚本会有事半功倍的效果。所以基于上面的考虑，对tcc做了二次封装。脚本增加**c99_export**语法导出处理，将tcc函数导出到cpp的使用环境中。

## 如何编译
```
git clone https://github.com/Tino610/TinyCInterpreter.git
mkdir build;cd build;cmake ..;make
```

## 如何调用
查看源码中的test.tcc, main.cc了解调用方法
