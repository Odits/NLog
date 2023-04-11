# NLog

[Example code snippet](NLog.h#L10-L20)


## 简介

`NLog`是一个简单易用的日志记录工具，它支持：

-   3种日志级别`info`、`warn`、`error`
-   16进制输出
-   可选择的
    -   打印时间
    -   调用文件名
    -   调用行号

通过使用RAII（Resource Acquisition Is Initialization）技术，记录对象的开始与结束。

>   RAII是一种管理资源的技术，它利用对象的生命周期来管理资源的获取和释放。~~其实就是对象的构造与析构。~~

## 安装

要安装`NLog`，只需将`NLog`的头文件`NLog.h`和源文件`NLog.cpp`添加到您的项目中即可。

## 使用方法

要使用`NLog`，首先需要创建一个`NLog`对象：

```c++
#include "NLog.h"

NLog nlog(__FILE__);
```

然后，您可以使用`nlog`对象的成员函数来记录日志：

```c++
nlog.info(__LINE__) << "This is an info message.";
nlog.warn(__LINE__) << "This is a warning message.";
nlog.error(__LINE__) << "This is an error message.";
```

## 配置

`NLog`支持以下配置选项：

- ~~`logLevel`：设置日志级别。只有高于或等于当前日志级别的消息才会被记录。~~	待实现
- `_func_name`：函数名。当一个`NLog`对象存在于函数体内时，传入`__func__`，`NLog`将在对象的构造函数和析构函数打印日志。
- `_on_time`：是否打印时间。
- `_on_path`：是否打印文件名。
- `_on_line`：是否打印行号。
- `_log_path`：日志存储的路径及文件名。
- `_rule`：日志将打印到何处。可选：
    - `NLog::rules::none`：不打印日志。
    - `NLog::rules::toConsole`：日志打印到终端。
    - `NLog::rules::toFile`：日志打印到文件。
    - `NLog::rules::toBoth`：日志打印到终端及文件。



```c++
NLog(const char* _func_name, const char* _path, int line_num, bool _on_time = false, bool _on_path = false, bool _on_line = true, const char* _log_path = "", rules _rule = toConsole);
```

要配置`NLog`，只需在创建`NLog`对象时设置相应的选项即可：

```c++
NLog nlog(__func__, __FILE__, __LINE__, "./log.txt", NLog::rules::toBoth, true, true, true);
```

## 常见问题

- 问：...
- 答：...

## 贡献指南

欢迎为NLog做出贡献！您可以通过以下方式为项目做出贡献：

- 提交问题：如果您发现了问题或者有新的想法，请提交一个问题。
- 提交代码：如果您修复了问题或者添加了新功能，请提交一个拉取请求。
- 参与讨论：如果您对项目感兴趣，请参与到讨论中来，分享您的想法和建议。

