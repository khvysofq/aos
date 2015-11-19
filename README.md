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
cmake .. -DCMAKE_BUILD_TYPE=DEBUG \
	-DBUILD_CURL_TESTS=OFF -DBUILD_CURL_EXE=OFF \ 
	-DJSONCPP_WITH_TESTS=OFF \
	-DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF \
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


## license

- jsoncpp lisenced unser a [MIT](https://github.com/open-source-parsers/jsoncpp "MIT")
- curl licensed under a [MIT/X](http://curl.haxx.se/docs/copyright.html "MIT/X") 
- easyloggingpp licensed under a [MIT](https://github.com/easylogging/easyloggingpp "MIT")