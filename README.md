# aliyun opensearch c++ sdk

## 编译配置

现在的`libali_opensearh`是一个私有的项目，因此只能够权限的人下载，除了作者之外只有`opensource@alibaba-inc.com`。当前的项目会依赖四个开源库，都已经以git submodule的形式链接到项目中了，因此你可以直接下载下来进行编译，不需要再去配置额外的库或者环境。

环境要求

- `CMake2.8 `,工程使用CMake来管理源代码，因此你必须要确保在您的计算机上面有`CMake 2.8`以上的版本。
- Windows平台确保已经安装VS2013版本，因为代码中使用了C++ 11的特性，当前只在VS2013上编译通过。
- Linux平台确保GCC4.7以上的版本，支持C++ 11特性


当前的项目已经在以下几个平台上编译通过

- Windows Microsoft Visual Studio 2013 
- Ubuntu 64 GCC 4.8

### 下载工程

```bash
	// 下载源代码工程
	git clone https://git.oschina.net/guangleihe/aliyun_opensearch.git
	// 进入下载工程目录	
	cd aliyun_opensearch
	// 更新第三方依赖库
	git submodule update --init --recursive
	// 创建一个编译文件夹，在这个文件夹中进行编译，保证源代码清洁
	mkdir build
	// 各个平台的编译方式参考下面的教程
	cd build
	// 执行CMake生成相应的工程文件
```

我们将下载源代码的路径称为`git_path`路径。通过上面的几个步骤您现在应该处于`git_path/aliyun_opensearch/build`下。

请确保您已经安装了CMake2.8以上的版本，并且已经加入到bash的path路径中，然后执行CMake生成工程

```bash
cmake .. -DBUILD_CURL_TESTS=OFF -DBUILD_CURL_EXE=OFF -DJSONCPP_WITH_TESTS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF -DWITH_GFLAGS=OFF -DHTTP_ONLY=ON
```

这一条命令会在Windows平台上面生成VS工程，在Linux平台上面生成Makefile文件。

### Windows平台上编译并使用

在Windows平台上生成的是VS的工程，可以直接打开进行编译。特别注意我们的编译是有顺序的，一般情况下先编译库，再编译相关的测试程序。虽然我们已经在CMake中指定了编译顺序，但是由于VS一般都采用多线程编译技术，因此一些代码量比较少的测试工程有可能会先编译起，在这种情况下，一些依赖库还没有编译好，所以会出现找不到库的问题。再使用VS编译一次就行了。

编译好了之后，生成的所有静态库，包括第三方静态库都会被放置到`git_path/aliyun_opensearch/lib/win`下面。正常情况下，你在这个目录下应该会看到以下几个库

- `glog.lib`，由Google Glog生成的日志库
- `jsoncpp.lib`，由JsonCpp生成的解析Json库
- `libali_opensearch.lib`，当前所编译的Aliyun OpenSearch C++ SDK库
- `libcurl_imp.lib`，由`libcurl`生成的库

其中`libcurl`生成的是动态库，因此在`git_path/aliyun_opensearch/lib/win`下面，只有一个非常小的静态链接文件，真正的动态库应该在目录`git_path/aliyun_opensearch/bin/win`中，所有的测试工程也都在这里面。用户使用的时候可以将这个路径加入到库搜索路径中，同时链接这些库。

**Windows用户使用注意**

1. 默认情况下，在Windows平台上都是使用的MT/MtD的形式编译的，如果想要切换请在VS工程上修改一下就可以了。

2. 用户在使用的时候要要记得将`git_path/aliyun_opensearch/bin/win/libcurl.dll`库复制到自己的程序目录下才能够运行起得程序。

由于使用了三个开源库，而这些库又有自己的头文件，因此在包含头文件的时候要指定如下几个路径在自己工程中才能够正确的使用
- `git_path/aliyun_opensearch/src`，用户需要包含主要的头文件使用时`#include "ali_search/ali_search.h"`
- `git_path/aliyun_opensearch/third_part/jsoncpp/include`,这是Json的库，用户能够直接使用到。

> 如果用户希望使用Glog日志库，那么可以包含`#include "base/logging.h"`头文件，但是需要指定Glog的路径`git_path/aliyun_opensearch/build/third_part/glog`和`git_path/aliyun_opensearch/third_partglog/src`两个路径。为了更好的使用Glog，用户可以在自己程序的main函数开始的时候初始化最好初始化Glog
> 
```c++
	int main(int argc, char* argv[]){
	  google::InitGoogleLogging(argv[0]);
	  FLAGS_logtostderr = true;
	  FLAGS_stderrthreshold = 0;
	  FLAGS_colorlogtostderr = true;
	}
```

### Linux平台上编译安装

Linux平台我们已经指定了编译顺序，只需要使用`make`命令执行，就可以把整个工程全部编译起。正常情况下会生成以下几个文件

- `git_path/aliyun_opensearch/lib/unix`下是所有的库文件，主要包含
	- `libali_opensearch.a`，主要的`libali_opensearch`工程库
	- `libcurl.so`，由`libcurl`生成的动态库
	- `libglog.a`，由google log生成的静态库
	- `libjsoncpp.a`，由jsoncpp生成的静态库
- `git_path/aliyun_opensearch/bin/unix`，是所有的测试工程，只包含关于`libali_opensearch`的测试工程。

**Linux用户使用注意**

由于使用了三个开源库，而这些库又有自己的头文件，因此在包含头文件的时候要指定如下几个路径在自己Makefile中才能够正确的使用
- `git_path/aliyun_opensearch/src`，用户需要包含主要的头文件使用时`#include "ali_search/ali_search.h"`
- `git_path/aliyun_opensearch/third_part/jsoncpp/include`,这是Json的库，用户能够直接使用到。

> 如果用户希望使用Glog日志库，那么可以包含`#include "base/logging.h"`头文件，但是需要指定Glog的路径`git_path/aliyun_opensearch/build/third_part/glog`和`git_path/aliyun_opensearch/third_partglog/src`两个路径。为了更好的使用Glog，用户可以在自己程序的main函数开始的时候初始化最好初始化Glog
> 
```c++
	int main(int argc, char* argv[]){
	  google::InitGoogleLogging(argv[0]);
	  FLAGS_logtostderr = true;
	  FLAGS_stderrthreshold = 0;
	  FLAGS_colorlogtostderr = true;
	}
```

## 简介

这个工程的主要目标是实现aliyun opensearch c++ sdk，后面的文档中，对工程的名字，全部称为`libali_opensearch`。这份文档主要介绍关于整个工程的核心代码的初级使用、高级使用以及基本的架构设计，同时还包含了一些对未来集成者的一些建议。

`libali_opensearch`全C++开发，中间使用了很多C++ 11的特性，其中主要使用了智能指针来管理所有的类的对象的生命周期。对象之间的传递也是通过智能指针来进行的。因此使用这个库的时候不用过多担心内存泄露问题。在后面的教程中我们会体会到这个过程。

`libali_opensearch`在四个开源的项目基础之上构建出来的。

- `libcurl`，这是以`HTTP`访问网络的常用库。在`libali_opensearch`中，所有的网络请求都是通过libcurl来进行的。
- `jsoncpp`，这是用来进行`jso`n数据格式的解析所使用的库。在`libali_opensearch`中，json的生成和解析都是使用的jsoncpp，不过使用的地方都比较集中，用户在有需要的情况下可以替换成自己的jscon解析库。我们在设计这个库的最初就已经考虑到这种需求。
- `easyloggingpp`，这是一个只有头文件的日志库，功能比较强大。在工程前期的时候一直使用的这个库，但是由于平台兼容性不好，所以后面就不再使用这个库。但是并没有将这个库从工程中删除，可以作为用户的另一种日志选择。
- `glog`，这是一个功能比较强大的google开源日志库。平台兼容性非常好。而且日志的接口与easyloggingpp一样，因此后期替换到这个库上面来了。但是用户也可以将日志库切换成其它的库，在后面会介绍如何轻松的替换日志库。

这几个项目的`license`在后面提供。

`libali_opensearch`按功能模块可以划分为七个部分，而我们接下来也会分成七个小部分来分别介绍。其中搜索相关的内部特别多，所以又会再一次的细分。

## 应用管理操作

我们从一个最简单的创建一个应用开始。来逐步介绍我们的操作接口，以及使用框架。先创建一个应用名称为`HELLO`的应用，然后得到这个应用的状态，最后删除这个应用。

第一步首先需要包含头文件`ali_opensearch/ali_opensearch.h`，然后创建一个全局的`aos::AosGlobalContext::Ptr`对象。这个对象内部实际上初始化了libcrul网络，因此所有的操作最终都会使用到这个对象。

```c++
  aos::AosGlobalContext::Ptr agcp_context = 
	 aos::AosGlobalContext::InitAosGlobalContext();
```

初始化是没有参数就可以使用了，这个对象内部实际上初始化了`libcurl`网络，是所有应用的基础，因此非常重要。
有了这个对象之后，就可以通过`aos::AosGlobalContext::Ptr`和`OpenSearch URL`、`AccessKey`与`Access Key Secret`创建一个`AliOpenSearch`对象:调用`CreateAliOpenSearch`函数，填入关键信息。

```c++
    static AliOpenSearch::Ptr CreateAliOpenSearch(
      AosGlobalContext::Ptr ag_context,
      const std::string &api_domain,
      const std::string &access_key_id,
      const std::string &access_key_secret);
```

`AliOpenSearch`对象是我们以这种权限访问OpenSearch服务的基础对象，所有的身份认证和相关的信息都需要这个对象来参与。可以从下面的代码看到，后面三个操作都是直接调用这个对象来进行的。

### 创建一个应用

```c++
    //const char APP_TEMPLATE_BUILTIN_NEWS[] = "builtin_news";
    //const char APP_TEMPLATE_BUILTIN_NOVEL[] = "builtin_novel";
    //const char APP_TEMPLATE_BUILTIN_DOWNLOAD[] = "builtin_download";
    //const char APP_TEMPLATE_BUILTIN_BBS[] = "bbs";
    ResValue::Ptr CreateNewApp(const std::string &app_name, 
      const std::string &template_name);
```
创建应用的时候需要两个参数

- `app_name`，应用名称，当前OpenSearch对这个字段没有什么太大的限制，不过在使用的时候还是尽量合理一些比较好。
- `template_name`，模块名称，当前OpenSearch只允许使用模板来创建应用，除了自定义模板之外，内置的模板主要有四个:`builtin_news`、`builtin_novel`、`builtin_download`、`bbs`

这个函数是同步返回的，直接返回一个`aos::ResValue::Ptr`对象，通过操作这个对象可以对结构进行相应的操作。

```c++
  class ResValue{
  public:
    // public interface
    std::string &rep_buffer(){ return rep_buffer_; }
    Json::Value &rep_json() { return json_res_; }
    bool IsSucceed() { return status_; }
    const AosErrors &GetErrorMessage() const{ return aos_errors_; }
    const std::string &get_req_id(){ return req_id_; }
    double search_time(){ return search_time_; }

  private:
	// ... ...
  };
```

以上是`ResValue`的定义：

- `rep_buffer()`，回复的原始数据，在大部分情况下，不建议你直接使用里面的数据，而是使用已经被解析过的数据，如使用`rep_json()`里面的数据。
- `rep_json()`，得到请求返回的Json格式信息，这里面的信息是经过Json解析了的，你可以直接通过JsonCpp的方式使用里面的信息。
- `IsSucceed()`，你可以通过这个函数判断请求是否成功。
- `AosErrors()`，如果请求不成功，可以通过这个函数得到错误的信息，特别注意的是这个函数返回的是一个错误数组，在OpenSearch里面，大多数操作都只有一个错误，但是对于push这样一次提交多种数据的操作就有可能返回多个错误信息。
- `get_req_id()`，理论上所有的操作都有一个请求ID，你可以通过这个接口来获取。
- `search_time()`，对于搜索操作，还会有一个操作使用时间，可以通过这个接口来得到。

### 得到应用的信息

```c++
    ResValue::Ptr GetAppStastus(const std::string &app_name);
```

通过传入App的名称得到应用的状态。

### 删除应用

```c++
    ResValue::Ptr DeleteApp(const std::string &app_name);
```

通过App的名称删除应用。显然删除操作不要轻易使用，这个删除会直接删除应用，谨慎使用。

以下是一个完整的例子

```c++
#include "ali_search/ali_search.h"

int main(int argv, char* argc[]){

  aos::AosGlobalContext::Ptr agcp_context =
    aos::AosGlobalContext::InitAosGlobalContext();

  aos::AliOpenSearch::Ptr aosp = aos::AliOpenSearch::CreateAliOpenSearch(
    agcp_context,
    "http://opensearch*****.aliyuncs.com",
    "6******L",
    "B1********7fB9");
  // Create app
  aos::ResValue::Ptr crs_value =
    aosp->CreateNewApp("HELLO", aos::APP_TEMPLATE_BUILTIN_NEWS);
  if (crs_value->IsSucceed()){
    LOG_INFO << "Create app succeed " << "HELLO";
  }
  else{
    LOG_ERROR << "Create app failed " 
      << crs_value->GetErrorMessage()[0].message;
  }
  // Get app status
  aos::ResValue::Ptr grs_value = aosp->GetAppStastus("HELLO");
  if (grs_value->IsSucceed()){
    LOG_INFO << "Get app status succeed " << "HELLO";
  }
  else{
    LOG_ERROR << "get app status failed "
      << grs_value->GetErrorMessage()[0].message;
  }
  // Delete app
  aos::ResValue::Ptr drs_value = aosp->DeleteApp("HELLO");
  if (drs_value->IsSucceed()){
    LOG_INFO << "Delete app succeed " << "HELLO";
  }
  else{
    LOG_ERROR << "Delete app failed " 
      << drs_value->GetErrorMessage()[0].message;
  }

  return 0;
}
```



未来的所有操作都通过直接调用`AliOpenSearch`对象的接口就可以直接与服务器进行交互，这些接口全部都是同步的，一次调用就能够马上返回结果，在未来有可能会加入对异步支持，以提高应用程序的效率。

可以看到，上面的所有对象都是通过智能指针的形式来管理的。而实际上，基本上所有的类都有严格的访问权限。您除了调用`aos::AosGlobalContext::InitAosGlobalContext()`来初始化全局上下文智能指针对象之外，没有其它办法能够创建出一个`AosGlobalContext`对象。同样，除了使用`aos::AliOpenSearch::CreateAliOpenSearch(...)`来创建一个`AliOpenSearch`智能指针对象之外也不会有第二种方式创建这样的一个对象。在`libali_opensearch`工程中，基本上所有的类的构造函数都是私有的。只有相关的友元对象才能够创建这些类。而基本我们会用到的对象都是由`AliOpenSearch`来创建的。因此这个类是整个SDK中最重要的对象，有了它就有了一切。

## 上传文档

根据OpenSearch的文档，[数据处理类](http://help.aliyun.com/document_detail/opensearch/api-reference/api-interface/data-manager.html "数据处理类")的描述，上传的时候可以一次性上传多个文档。因此在`libali_opensearch`中，把这个过程封装了。需要用到以下几个接口。

```c++
    // About the push
	  enum PushItemType{
	    ITEM_TYPE_ADD,
	    ITEM_TYPE_UPDATE,
	    ITEM_TYPE_DELETE
	  };
	// Create a push Item
	PushItem::Ptr CreatePushItem(PushItemType type, const std::string &id);
	// Create a push Form
	PushForm::Ptr CreatePushForm(PushItem::Ptr push_item);
	// Push a form to the server
	ResValue::Ptr PushIndexDoc(const std::string &app_name,
	  const std::string &table_name, 
	  PushForm::Ptr push_form);
```

我们最终要使用的接口是`PushIndexDoc`，通过这个接口将一个`PushForm::Ptr`上传给服务器。而需要创建一个`PushForm::Ptr`对象，则需要使用`CreatePushForm`接口。但是每一个请求，每一个`PushForm`都至少有一个文档数据，因此使用`CreatePushForm`的时候，需要传入一个`PushItem::Ptr`对象。所以需要使用`CreatePushItem`，传入文档操作类型和ID创建一个`PushItem`对象。

- `PushItem`对象，可以向里面添加其它字段。
- `PushForm`对象，可以向里面添加或者删除`PushItem`这样就可以上传多个文档了。

以下是一个比较完整的示例：

```c++
  aos::ResValue::Ptr grs_value = aosp->GetAppStastus("HELLO");
  if (grs_value->IsSucceed()){
    LOG_INFO << "Get app status succeed " << "HELLO";
  }
  else{
    LOG_ERROR << "get app status failed "
      << grs_value->GetErrorMessage()[0].message;
  }

  aos::PushItem::Ptr push_item1 = aosp->CreatePushItem(
    aos::PushItemType::ITEM_TYPE_ADD, "1");
  push_item1->AddField("type_id", "12");
  push_item1->AddField("cat_id", "13");
  push_item1->AddField("title", "test");
  push_item1->AddField("body", "This is a test, about something");
  push_item1->AddField("url", "www.baidu.com");

  aos::PushItem::Ptr push_item2 = aosp->CreatePushItem(
    aos::PushItemType::ITEM_TYPE_ADD, "2");
  push_item2->AddField("type_id", "12");
  push_item2->AddField("cat_id", "13");
  push_item2->AddField("title", "test");
  push_item2->AddField("body", "This is a test, about something");
  push_item2->AddField("url", "www.baidu.com");

  aos::PushForm::Ptr push_form = aosp->CreatePushForm(push_item1);
  push_form->AddPushItem(push_item2);

  aos::ResValue::Ptr res = aosp->PushIndexDoc("HELLO", "main", push_form);

  if (res->IsSucceed()){
    LOG_INFO << "Delete the HELLO main table succeed";
  }
  else{
    LOG_ERROR << "Delete the HELLO main table error"
      << res->GetErrorMessage()[0].message;
  }
```

我们将Push的操作全部封装了，不希望用户直接去构建数据去上传文档数据，这也是为了避免用户犯一些不必要的错误浪费时间。在后面的搜索操作中，这种设计结构将会更加的体现出来。

## 下拉提示

下拉提示应该是用户在真实的使用场景中最常用的操作之一。这个接口也非常简单易用。

```c++
    ResValue::Ptr SuggestHit(const std::string &app_name,
      const std::string &query, 
      const std::string &suggest_name,
      int hit_count = 10);
```

- `app_name`，应用名称
- `query`，具体的查询内容
- `suggest_name`，这个字段官方并没有给出明显的提示
- `hit_count`，默认的搜索提示条数。

下面是一个使用示例：

```c++
  aos::ResValue::Ptr res_value = aosp->SuggestHit("HELLO", 
    "opensearch","test_suggest", 10);
  if (res_value->IsSucceed()){
    LOG_INFO << "Reindex HELLO succeed";
    Json::Value result = res_value->rep_json()[aos::JSON_RESULT];
    Json::Value suggestion_json = result[aos::RES_SUGGESTIONS];
    for (std::size_t i = 0; i < suggestion_json.size(); i++){
      LOG_INFO << suggestion_json[i][aos::RES_SUGGESTIONS].asString();
    }
  }
  else{
    LOG_ERROR << "Reindex HELLO error"
      << res_value->GetErrorMessage()[0].message;
  }
```

## 索引重建

索引重建从API接口来看比较简单，不过对于索引重建要关联的操作和导入的数据表这一部分需要用户特别注意使用。

```c++
    ResValue::Ptr ReindexApp(const std::string &app_name,
      const std::string &operate = "",
      const std::string &table_name = "");
```
具体的使用方法还是需要查看官方的文档[索引重建](http://help.aliyun.com/document_detail/opensearch/api-reference/api-interface/index-restructure.html "索引重建")

下面是一个使用示例，还是使用的应用：`HELLO`，但是用户在测试的时候最好能够导入一些数据进去。如果按照模板创建的应用，官方提供了一些测试数据可以导入。

```c++
  aos::ResValue::Ptr res_value = aosp->ReindexApp("HELLO");
  if (res_value->IsSucceed()){
    LOG_INFO << "Reindex HELLO succeed";
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << "Reindex HELLO error"
      << res_value->GetErrorMessage()[0].message;
  }
```
注意错误消息是一个数组。

## 得到错误日志

得到错误日志所需要的接口如下：

```c++
  enum SortMode{
    SORT_ASC,
    SORT_DESC
  };
  ResValue::Ptr GetAppErrorLog(const std::string &app_name,
    int page, int page_size, SortMode sort_mode);
```
- `page`，是目标页码
- `page_size`是每一页返回的错误数
- `sort_mode`，是排序的方式，现在支持两种排序模块，`SortMode`

示例代码

```c++
  aos::ResValue::Ptr res_value = aosp->GetAppErrorLog("HELLO",
    2,2, aos::SortMode::SORT_ASC);
  if (res_value->IsSucceed()){
    LOG_INFO << "Get Error Log HELLO succeed";
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << "Get Error Log HELLO error"
      << res_value->GetErrorMessage()[0].message;
  }
```

## 搜索相关

搜索是整个OpenSearch的核心部分，`libali_opensearch`项目也是在搜索这个地方下了非常大的功夫。我们最终提供的方式也是与前面上传文档类的操作是一样的。需要用户使用SDK一步一步的构造自己的请求，而不是直接使用原始的数据进行请求，避免用户犯不必要的错误错误，浪费时间。

### 接口操作和简单使用

搜索相关一共会涉及到`AliOpenSearch`的12个接口。搜索主要分成两部分，一个大部分是正常的搜索，另一种是`Scroll`搜索。

```c++
    // Search
    ResValue::Ptr Search(SearchForm::Ptr search_form);

    // Scroll Search
    ResValue::Ptr ScrollSearch(SearchForm::Ptr search_form, Scroll::Ptr scroll);
```

从接口可以看到搜索过程实际是上把一个`SearchForm::Ptr`对象传递给接口，然后进行操作。要创建一个`SearchForm::Ptr`对象，你必须使用以下接口：

```c++
    // Create SearchFrom
    SearchForm::Ptr CreateSearchForm(
      Query::Ptr query, const std::string &app_name);
```

这个接口又可以看到要创建一个`SearchForm::Ptr`，需要两个重要的信息:`Query::Ptr query`和`app_name`，。我们严格限制了用户的行为，用户除了调用这个接口之外没有其它方法能够生成一个`SearchForm::Ptr`对象。`SearchForm`内部有八个参数，我们已经对这八个参数的表达式进行了良好的封装，用户只需要参考官方API文档，再结合`SearchForm`的结构就可以自定义自己的结构了。

我们可以调用下面的接口创建一个`Query::Ptr`对象。

```c++
    // Create Query
    Query::Ptr CreateQuery(QueryStanza::Ptr query_stanza);
```

从OpenSearch 接口文档中可以知道，一个正常的Query里面可以包含七种子句。而这七种子句里面`Query`子句是必须的。所以在创建`Query::Ptr`对象的时候，必须要一个`QueryStanza::Ptr`对象。而创建一个`QueryStanza::Ptr`对象可以使用下面的接口：

```c++
    // Create QueryStanza
    QueryStanza::Ptr CreateQueryStanza(const std::string &index_name,
      const std::string &key_word);
```
对于`QueryStanza::Ptr`对象，就可以直接指定应用名称和关键字进行创建了。

下面是一个简单的示例：

```c++
  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("default", "搜索");
  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_config_stanza(cs);

  aos::SearchForm::Ptr search_form = aosp->CreateSearchForm(query, "HELLO");

  aos::ResValue::Ptr res_value = aosp->Search(search_form);
  if (res_value->IsSucceed()){
    LOG_INFO << "Search HELLO succeed";
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << "Get Error Log HELLO error"
      << res_value->GetErrorMessage()[0].message;
  }
```

## 搜索子句

前面我们已经说过，Query里面有七种类型的子句，因此还有剩下几个创建相关子句的接口


```c++
    // -------------------------------------------------------------------------
    // About the search
    // Create ConfigStanza
    ConfigStanza::Ptr CreateConfigStanza();

    // Create QueryStanza
    QueryStanza::Ptr CreateQueryStanza(const std::string &index_name,
      const std::string &key_word);

    // Create FilterStanza
    FilterStanza::Ptr CreateFilterStanza(const std::string &filter_express);

    // Create SortStanza
    SortStanza::Ptr CreateSortStanza(
      SortType type, const std::string &sort_express);

    // Create AggregateStanza
    AggregateStanza::Ptr CreateAggregateStanza(
      const std::string &group_key, const std::string &agg_fun);

    // Create DistinctStanza
    DistinctStanza::Ptr CreateDistinctStanza(const std::string &dist_key);

    // Create KvpairsStanza
    KvpairsStanza::Ptr CreateKvpairsStanza(
      const std::string &key, const std::string value);
```

每一个子句以`Stanza`结束。一个`Query::Ptr`除了在创建的同时指定一个`QueryStanza`之外，还可以在创建之后添加其它的子句来进行操作。接下来我们分别对每一个子句进行介绍

### Query子句

Query子句是整个搜索中最重要，也是不能够少的部分。Query子句在`libali_opensearch`中用`QueryStanza`来表示。由于`QueryStanza`是可以由多种条件合成的。因此`QueryStanza`提供了一种相互运算的机制。如下代码所示：

```c++
  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("title", "北京大学");
  aos::QueryStanza::Ptr qb = aosp->CreateQueryStanza("title", "浙江大学");
  aos::QueryStanza::Ptr qc = aosp->CreateQueryStanza("type", "1");
  
  LOG_INFO << qa->Express();
  LOG_INFO << qb->Express();
  LOG_INFO << qc->Express();

  aos::QueryStanza::Ptr qd = qa->OrdinaryUnion(
    aos::QueryUnionType::UNION_OR, qb);
  aos::QueryStanza::Ptr qe = qd->PriorityUnion(
    aos::QueryUnionType::UNION_AND, qc);
  aos::QueryStanza::Ptr qf = qd->PriorityUnion(
    aos::QueryUnionType::UNION_RANK, qc);

  LOG_INFO << qd->Express();
  LOG_INFO << qe->Express();
```

打印的内容如下所示：

```
title:'北京大学'
title:'浙江大学'
type:'1'
title:'北京大学' OR title:'浙江大学'
(title:'北京大学'OR title:'浙江大学') AND type:'1'
```

两个`QueryStanza`可以联合成为一个`QueryStanza`对象。联合的操作有以下几种

```c++
  enum QueryUnionType{
    UNION_AND,
    UNION_OR,
    UNION_ANDNOT,
    UNION_RANK
  };
```

而联合的类型有两种：

```c++
    QueryStanza::Ptr OrdinaryUnion(QueryUnionType type,
      QueryStanza::Ptr query_stanza);
    QueryStanza::Ptr PriorityUnion(QueryUnionType type,
      QueryStanza::Ptr query_stanza);
```
其中

- `OrdinaryUnion`，联合之后，两个QueryStanza是平级的。
- `PriorityUnion`，联合之后，原本的`QueryStanza`全中一个括号再联合.

> 特别需要注意的是，两个对象联合之后只会生成一个新的对象，而不会改变原来对象的值。

### Config子句

Config子句创建的时候使用前面所说的`AliOpenSearch::CreateConfigStanza()`接口来创建，根据官方的API接口文档，Config子句可以不传参数，因为所有的参数都可以是默认值。

Config子句一共有以下几种操作

```c++
    virtual const std::string Express();
    virtual bool IsValue();
    //
    uint32 start() { return start_; }
    void set_start(uint32 start) { start_ = start; }
    //
    uint32 hit();
    void set_hit(uint32 hit){ hit_ = hit; }
    //
    TextFormat format() { return format_; }
    void set_format(TextFormat format){ format_ = format; }
    //
    uint32 rerank_size() { return rerank_size_; }
    void set_rerank_size(uint32 rerank_size){ rerank_size_ = rerank_size; }
```

其中的`start`、`hit`、`format`、`rerank_size`的具体操作含义可以查看官方文档。值得注意的是，如果`ConfigStanza`的所有值都是默认值的话，那么在请求服务器的时候就会自动忽略`ConfigStanza`。这个时候`IsValue`的返回值是`false`。下面是一个使用的示例：

```c++
  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("default", "搜索");
  aos::ConfigStanza::Ptr cs = aosp->CreateConfigStanza();
  cs->set_hit(1);

  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_config_stanza(cs);

  aos::SearchForm::Ptr search_form = aosp->CreateSearchForm(query, "HELLO");

  aos::ResValue::Ptr res_value = aosp->Search(search_form);
  if (res_value->IsSucceed()){
    LOG_INFO << "Search HELLO succeed";
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << "Get Error Log HELLO error"
      << res_value->GetErrorMessage()[0].message;
  }
```

Query可以使用`set_config_stanza`把`ConfigStanza`对象设置到搜索请求中去。

### Filter子句

Filter子句与前面的Query子句的很多地方有相似之处他同样提供联合的接口：它联合的原理也与`QueryStanza`一样的。

```c++
  enum FilterUnionType{
    FILTER_UNION_TYPE_AND,
    FILTER_UNION_TYPE_OR
  };
    virtual const std::string Express() { return filter_express_; }
    virtual bool IsValue(){ return !filter_express_.empty(); };

    FilterStanza::Ptr OrdinaryUnion(FilterUnionType type,
      FilterStanza::Ptr filter_stanza);
    FilterStanza::Ptr PriorityUnion(FilterUnionType type,
      FilterStanza::Ptr filter_stanza);
```

下面是一个联合的示例代码：

```c++

  aos::FilterStanza::Ptr fa = aosp->CreateFilterStanza("category=2");
  aos::FilterStanza::Ptr fb = aosp->CreateFilterStanza("create_time<140234560");
  aos::FilterStanza::Ptr fc = aosp->CreateFilterStanza("(hit+sale)*rate>10000");
  aos::FilterStanza::Ptr fd = fa->OrdinaryUnion(
    aos::FilterUnionType::FILTER_UNION_TYPE_AND, fb);
  aos::FilterStanza::Ptr fe = fd->PriorityUnion(
    aos::FilterUnionType::FILTER_UNION_TYPE_OR, fc);

  LOG_INFO << fa->Express();
  LOG_INFO << fb->Express();
  LOG_INFO << fc->Express();
  LOG_INFO << fd->Express();
  LOG_INFO << fe->Express();
```

输出的结果为:

```c++
category=2
create_time<140234560
(hit+sale)*rate>10000
category=2 AND create_time<140234560
(category=2 AND create_time<140234560) OR (hit+sale)*rate>10000
```

下面是包含一个`FilterStanza`的请求示例：
```c++
  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("default", "搜索");
  aos::ConfigStanza::Ptr cs = aosp->CreateConfigStanza();
  cs->set_hit(1);
  aos::FilterStanza::Ptr fs = aosp->CreateFilterStanza("type_id=1");

  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_config_stanza(cs);
  query->set_filter_stanza(fs);

  aos::SearchForm::Ptr search_form = aosp->CreateSearchForm(query, "HELLO");

  aos::ResValue::Ptr res_value = aosp->Search(search_form);
  if (res_value->IsSucceed()){
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << res_value->GetErrorMessage()[0].message;
  }
```
> 注意在filter中出现的字段必须在定义应用结构的时候勾选“可过滤”

### sort子句

sort子句，`SortStanza`同样具有可以联合的性质。但是与前面有所不同，这里的联合只能够平级联合。

```c++

    virtual const std::string Express() { return sort_express_; }
    virtual bool IsValue(){ return !sort_express_.empty(); };

    SortStanza::Ptr UnionSort(SortStanza::Ptr sort_stanza);
```

下面是Sort联合的一个代码示例：

```c++
  aos::SortStanza::Ptr sa = aosp->CreateSortStanza(
    aos::SortType::ASC_TYPE, "RANK");
  aos::SortStanza::Ptr sb = aosp->CreateSortStanza(
    aos::SortType::ASC_TYPE, "(hits+comments)");
  aos::SortStanza::Ptr sc = aosp->CreateSortStanza(
    aos::SortType::ASC_TYPE, "type");
  aos::SortStanza::Ptr sd = sa->UnionSort(sb);
  aos::SortStanza::Ptr se = sd->UnionSort(sc);

  LOG_INFO << sa->Express();
  LOG_INFO << sb->Express();
  LOG_INFO << sc->Express();
  LOG_INFO << sd->Express();
  LOG_INFO << se->Express();
```

输入的信息如下所示：

```
+RANK
+(hits+comments)
+type
+RANK;+(hits+comments)
+RANK;+(hits+comments);+type
```
以下是一个加入了Sort的查询示例：

```c++
  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("default", "搜索");
  aos::SortStanza::Ptr ss = aosp->CreateSortStanza(aos::SortType::ASC_TYPE, "type_id");

  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_sort_stanza(ss);

  aos::SearchForm::Ptr search_form = aosp->CreateSearchForm(query, "HELLO");

  aos::ResValue::Ptr res_value = aosp->Search(search_form);
  if (res_value->IsSucceed()){
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << res_value->GetErrorMessage()[0].message;
  }
```

> 注意在sort中出现的字段必须在定义应用结构的时候勾选“可过滤”；

### aggregate子句

通过接口`CreateAggregateStanza`来创建`AggregateStanza`对象。`AggregateStanza`的接口比较简单，如下所示：

```c++
    virtual const std::string Express();
    virtual bool IsValue();

   const std::string group_key();
    // about the range
    void AddAggRange(uint32 min, uint32 max);
    void ClearRange();

    // 
    void AddAggFunc(const std::string &func_express);
    void ClearAggFunc();

    // filter
    void set_agg_filter(const std::string &agg_filter);

    // 
    void set_max_group(uint32 max_group);
    void set_agg_sampler_threshold(uint32 agg_sampler_threshold);
    void set_agg_sampler_step(uint32 agg_sampler_step);
```

它一共有七个值，其中`group_key`和`agg_fun`参数是必须的，需要在创建的同时指定，而`agg_sampler_threshold`与`agg_sampler_step`两个参数是一起的，必须同时指定。下面是一个搜索示例：

```c++
  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("default", "搜索");
  aos::AggregateStanza::Ptr ags = aosp->CreateAggregateStanza(
    "type_id", "count()");

  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_aggregate_stanza(ags);

  aos::SearchForm::Ptr search_form = aosp->CreateSearchForm(query, "HELLO");

  aos::ResValue::Ptr res_value = aosp->Search(search_form);
  if (res_value->IsSucceed()){
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << res_value->GetErrorMessage()[0].message;
  }
```

### distinct子句

distinct需要有一个`dist_key`来创建`DistinctStanza`对象。这个对象有以下几个操作的接口

```c++
    virtual const std::string Express();
    virtual bool IsValue();

    const std::string &dist_key() const;
    //
    void set_dist_times(uint32 dist_times);
    uint32 dist_times();

    //
    void set_dist_count(uint32 dist_count);
    uint32 dist_count();

    //
    void set_reserved(bool reserved);
    bool reserved();

    //
    void set_update_total_hit(bool suth);
    bool update_total_hit();

    //
    void set_dist_filter(const std::string &filter_express);
    const std::string &dist_filter();
    //
    void add_grade(float grade);
    void clear_grade();
    const std::vector<float> &grade();
```

`DistinctStanza`有七个参数，其中`dist_key`是必需的，下面是一个搜索示例：

```c++
  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("default", "搜索");
  aos::DistinctStanza::Ptr ds = aosp->CreateDistinctStanza("type_id");
  ds->set_dist_count(2);
  ds->set_dist_times(10);

  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_distinct_stanza(ds);

  aos::SearchForm::Ptr search_form = aosp->CreateSearchForm(query, "HELLO");

  aos::ResValue::Ptr res_value = aosp->Search(search_form);
  if (res_value->IsSucceed()){
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << res_value->GetErrorMessage()[0].message;
  }
```

### kvpair子句

kvpair子句由`AliOpenSearch::CreateKvpairsStanza`创建`KvpairsStanza`对象。其中必须要传入一个`key:value`对。

`KvpairsStanza`提供的接口非常简单，如下所示

```c++
    virtual const std::string Express();
    virtual bool IsValue();

    bool AddKvpair(const std::string &key, const std::string value);
    void Clear();
    std::set<std::string> &keyvalues();
```

下面是一个包含`KvpairsStanza`的示例：

```c++
  aos::QueryStanza::Ptr qa = aosp->CreateQueryStanza("default", "搜索");

  aos::DistinctStanza::Ptr ds = aosp->CreateDistinctStanza("type_id");
  ds->set_dist_count(2);
  ds->set_dist_times(10);

  aos::KvpairsStanza::Ptr kvs = 
    aosp->CreateKvpairsStanza("duniqfield", "type_id");

  aos::Query::Ptr query = aosp->CreateQuery(qa);
  query->set_distinct_stanza(ds);
  query->set_kvpair_stanza(kvs);

  aos::SearchForm::Ptr search_form = aosp->CreateSearchForm(query, "HELLO");

  aos::ResValue::Ptr res_value = aosp->Search(search_form);
  if (res_value->IsSucceed()){
    LOG_INFO << res_value->rep_json().toStyledString();
  }
  else{
    LOG_ERROR << res_value->GetErrorMessage()[0].message;
  }
```

### ScrollSearch

ScrollSearch通过以下接口创建`Scroll::Ptr`对象
```c++
  enum ScrollTimeType{
    SCROLL_TIME_SECOND,
    SCROLL_TIME_MINUTE,
    SCROLL_TIME_HOUR,
    SCROLL_TIME_DAY,
    SCROLL_TIME_WEEK
  };
    // Create Scroll
    Scroll::Ptr CreateScroll(uint32 scroll_time, 
      ScrollTimeType time_type = SCROLL_TIME_MINUTE,
      const std::string &search_type = SCROLL_TYPE_SCAN,
      const std::string &scroll_id = "");
```
有了Scroll对象就可以调用`ScrollSearch`进行查询。

```c++
    // Scroll Search
    ResValue::Ptr ScrollSearch(SearchForm::Ptr search_form, Scroll::Ptr scroll);
```

从`CreateScroll`上面可以看到一些默认的参数。在第一次搜索的时候只需要指定一个`scroll_time`就行了，执行第一次搜索之后，内部会自动更新`Scroll`的`scroll_id`字段。未来就可以再一次调用`ScrollSearch`传入同样的参数得到下一次搜索的结果。当然在这个过程当中你也可以修改一些`Scroll`的值，从而进行更加精确的操作，具体的值参考`Scorll`定义，具体的使用方法可以参数官方的API文档。

## license

- jsoncpp lisenced unser a [MIT](https://github.com/open-source-parsers/jsoncpp "MIT")
- curl licensed under a [MIT/X](http://curl.haxx.se/docs/copyright.html "MIT/X") 
- easyloggingpp licensed under a [MIT](https://github.com/easylogging/easyloggingpp "MIT")
- glog license under a [Apache 2](https://developers.google.com/open-source/devplat "Apache 2")