# TestProject 学习索引

这里存放用于源码阅读的第三方项目材料，当前主要是 GuiLite、nginx 和 SQLite。这个目录主要当作阅读索引来用。

## 从哪里开始看

- 想练接口到实现的阅读路径，先看 GuiLite
- 想练单文件纵向阅读，先看 SQLite
- 想看工业级事件驱动服务端结构，再看 nginx

这里不提供统一运行入口。真要编译或运行，还是得回到各项目自己的构建方式和上游说明。

---

当前目录已经清理到以源码学习为主，只保留了 3 组材料：

- GuiLite 核心源码
- nginx 核心源码
- SQLite amalgamation 单文件版源码

这份索引的目标不是介绍项目本身，而是告诉你：从哪里开始看，先看什么，哪些文件最值钱。

---

## 一、目录现状

### 1. GuiLite

保留内容：

- [GuiLite-master/GuiLite.h](GuiLite-master/GuiLite.h)
- [GuiLite-master/src](GuiLite-master/src)
- [GuiLite-master/documents](GuiLite-master/documents)

说明：

- `GuiLite.h` 适合先看对外接口和整体风格
- `src` 是真正的核心实现
- `documents` 里只保留了 6 篇机制说明文档，适合配合源码阅读

### 2. nginx

保留内容：

- [nginx/src](nginx/src)
- [nginx/auto](nginx/auto)
- [nginx/conf](nginx/conf)
- [nginx/.gitignore](nginx/.gitignore)

说明：

- `src` 是学习 nginx 内核的主战场
- `auto` 是 nginx 构建与平台适配系统，理解工程结构时很有价值
- `conf` 是配置样例，适合在读 HTTP 模块与请求处理链时对照看

### 3. SQLite

保留内容：

- [sqlite-amalgamation-3450200/sqlite3.c](sqlite-amalgamation-3450200/sqlite3.c)
- [sqlite-amalgamation-3450200/sqlite3.h](sqlite-amalgamation-3450200/sqlite3.h)
- [sqlite-amalgamation-3450200/sqlite3ext.h](sqlite-amalgamation-3450200/sqlite3ext.h)
- [sqlite-amalgamation-3450200/shell.c](sqlite-amalgamation-3450200/shell.c)

说明：

- 这是最适合做“单文件纵向阅读”的一份 SQLite 源码
- `sqlite3.c` 几乎把核心实现都压在一起，适合建立全局感
- `shell.c` 适合看 SQLite CLI 如何调用库接口

---

## 二、推荐学习顺序

如果你的目标是练源码阅读能力，而不是立即研究最复杂的工程系统，建议顺序如下：

### 第一组：GuiLite

理由：

- 代码量相对小
- UI 框架结构比较直观
- 适合先训练“从接口到实现”的阅读路径

建议顺序：

1. [GuiLite-master/GuiLite.h](GuiLite-master/GuiLite.h)
2. [GuiLite-master/src/core](GuiLite-master/src/core)
3. [GuiLite-master/src/widgets](GuiLite-master/src/widgets)
4. [GuiLite-master/documents/HowToUse.md](GuiLite-master/documents/HowToUse.md)
5. [GuiLite-master/documents/HowMessageWork.md](GuiLite-master/documents/HowMessageWork.md)

### 第二组：SQLite

理由：

- 单文件源码适合建立系统级全局视角
- 很适合练“从 API 入口一路跟到内部实现”的能力
- 比 nginx 更容易在一个文件内来回跳转

建议顺序：

1. [sqlite-amalgamation-3450200/sqlite3.h](sqlite-amalgamation-3450200/sqlite3.h)
2. [sqlite-amalgamation-3450200/sqlite3.c](sqlite-amalgamation-3450200/sqlite3.c)
3. [sqlite-amalgamation-3450200/shell.c](sqlite-amalgamation-3450200/shell.c)

### 第三组：nginx

理由：

- 模块化和事件驱动设计更强
- 工程层次更深，读起来更像工业级服务器源码
- 适合在前两组之后再系统攻坚

建议顺序：

1. [nginx/src/core](nginx/src/core)
2. [nginx/src/event](nginx/src/event)
3. [nginx/src/http](nginx/src/http)
4. [nginx/conf/nginx.conf](nginx/conf/nginx.conf)
5. [nginx/auto](nginx/auto)

---

## 三、每组最值得先看的入口

### GuiLite 入口

如果只想抓主线，先看这些：

- [GuiLite-master/GuiLite.h](GuiLite-master/GuiLite.h)
- [GuiLite-master/src/core/wnd.h](GuiLite-master/src/core/wnd.h)
- [GuiLite-master/src/core/core.cpp](GuiLite-master/src/core/core.cpp)
- [GuiLite-master/src/widgets/widgets.cpp](GuiLite-master/src/widgets/widgets.cpp)
- [GuiLite-master/src/CMakeLists.txt](GuiLite-master/src/CMakeLists.txt)

阅读目标：

- 搞清楚窗口对象怎么组织
- 搞清楚显示、surface、widget 的关系
- 看出这个库如何把平台适配与控件层拆开

### nginx 入口

如果你现在就在看内存池，建议这条路径：

1. [nginx/src/core/ngx_palloc.h](nginx/src/core/ngx_palloc.h)
2. [nginx/src/core/ngx_palloc.c](nginx/src/core/ngx_palloc.c)
3. [nginx/src/core/ngx_array.h](nginx/src/core/ngx_array.h)
4. [nginx/src/core/ngx_list.h](nginx/src/core/ngx_list.h)
5. [nginx/src/core/ngx_queue.h](nginx/src/core/ngx_queue.h)

这条线很适合先理解 nginx 的基础容器与内存管理哲学。

之后再进入：

- [nginx/src/event](nginx/src/event)
- [nginx/src/http](nginx/src/http)

### SQLite 入口

建议不要一上来硬啃整份 `sqlite3.c`，先从接口和高频关键词切入：

1. [sqlite-amalgamation-3450200/sqlite3.h](sqlite-amalgamation-3450200/sqlite3.h)
2. 在 [sqlite-amalgamation-3450200/sqlite3.c](sqlite-amalgamation-3450200/sqlite3.c) 里搜索：
   - `sqlite3_open`
   - `sqlite3_prepare_v2`
   - `sqlite3_step`
   - `sqlite3_finalize`
3. 再回头看 [sqlite-amalgamation-3450200/shell.c](sqlite-amalgamation-3450200/shell.c) 如何驱动这些 API

---

## 四、后续还值不值得再删

当前已经非常接近“只留学习主干”的状态了。

不建议再删的内容：

- [GuiLite-master/documents](GuiLite-master/documents) 里的 6 篇 Markdown
- [nginx/auto](nginx/auto)
- [nginx/conf](nginx/conf)
- [nginx/.gitignore](nginx/.gitignore)

原因很简单：

- 这些东西占空间很小
- 但对理解项目结构、构建过程和机制说明是有帮助的
- 再删下去，目录会更干净，但学习入口反而会变差

---

## 五、最实用的阅读建议

如果你后面真的要开始系统学，建议用下面的方法：

1. 先挑一个问题读，不要按目录机械扫。
2. 每次只盯一条主线，比如“内存池”“窗口树”“SQL 执行入口”。
3. 读完一个点，立刻把入口函数、关键结构体和调用关系记到笔记里。
4. 不要试图一次读完整个 nginx 或 SQLite，先建立局部闭环，再扩展边界。

这样这三个目录才会变成学习材料，而不是新的资料堆。
