#aliyun_opensearch

## 下载源代码

```
	// 1. 下载工程
	git clone https://git.oschina.net/guangleihe/aliyun_opensearch.git
	// 2. 因为项目中引用了第三方库资源，所以也需要更新下来。
	git submodule update --init --recursive
```

## 编译生成

安装`CMake 2.8`以上的版本，然后为了保证源代码的清洁，在源代码目录下新建一个build目录，使用下面的命令生成相应的工程。

``` 
cmake .. -DBUILD_CURL_TESTS=OFF -DBUILD_CURL_EXE=OFF -DJSONCPP_WITH_TESTS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF -DWITH_GFLAGS=OFF
```


## TODO

- 考虑异步系统集成问题
- 能够在编译的时候去掉`curl`编译，因为有一些linux用户可能本机上已经安装了这个库，可以直接使用原生的库
- 项目通过 `travis-ci.org` 或者`drone.io`的编译
- 语法符合 `linthub.io` 的检测
- 单元测试代码不少于整体代码`30%`
- 代码覆盖`coveralls.io`覆盖率`95%`
- 编写英文`Readme.md`
- 至少Windows、Linux、MacOS三个平台可工作
- 不少于1篇英文使用示例文档(Markdown格式)
- 单元测试必须通过valgrind验证，不能有内存泄露或者越界。

## TODO 2015 11 22

当前完成的状态与问题

- 基础的签名流程、应用管理类、数据处理类、索引重建、错误日志功能已经完成。
- 下拉提示的请求结构与其它结构不同，需要修正一下。
- 搜索相关的流程有，但是对规划化请求的封装还不完全。
- 所有的功能都没有经过严格测试，后面需要数据支持。

## 日志库

`libali_opensearch`由于历史原因使用两种日志，前期使用了`easyloggingpp`库，后期使用了`glog`库，`libali_opensearch`提供了非常简单的方法来替换不同的日志库，我们并不会邦架用户，只是希望能够以更好的方式展示给用户有用的信息。
在代码`src/base/baseinclude.h`中，有关于日志库的加载。

```c++
//#define EASY_LOGGING_CPP
#ifdef EASY_LOGGING_CPP
#include "easylogging++.h"
#endif

#define GOOGLE_GLOG_LIBRARY
#ifdef GOOGLE_GLOG_LIBRARY
#include "glog/logging.h"
#endif
// ....

#ifdef GOOGLE_GLOG_LIBRARY || #ifdef EASY_LOGGING_CPP

#define LOG_INFO LOG(INFO)
#define LOG_WARNING LOG(WARNING)
#define LOG_ERROR LOG(ERROR)

#define DLOG_INFO DLOG(INFO)
#define DLOG_WARNING DLOG(WARNING)
#define DLOG_ERROR DLOG(WARNING)
#else

#define LOG_INFO std::cout
#define LOG_WARNING std::cout
#define LOG_ERROR std::cout

#define DLOG_INFO std::cout
#define DLOG_WARNING std::cout
#define DLOG_ERROR std::cout
#endif
```

如上面的代码所示，在库中所有的日志都以`LOG_INFO`、 `LOG_WARNING`、 `LOG_ERROR`、 `DLOG_INFO`、 `DLOG_WARNING`、 `DLOG_ERROR`，您可以将这三个宏替换成你想要的输出方式。

## license

- jsoncpp lisenced unser a [MIT](https://github.com/open-source-parsers/jsoncpp "MIT")
- curl licensed under a [MIT/X](http://curl.haxx.se/docs/copyright.html "MIT/X") 
- easyloggingpp licensed under a [MIT](https://github.com/easylogging/easyloggingpp "MIT")
- glog license under a [Apache 2](https://developers.google.com/open-source/devplat "Apache 2")