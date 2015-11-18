#aliyun_opensearch

## 下载源代码

```
	// 1. 下载下要源代码
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
