#ifndef ALI_OPENSEARCH_ALI_OPENSEARCH_REQ_TASK_H_
#define ALI_OPENSEARCH_ALI_OPENSEARCH_REQ_TASK_H_

#include "base/baseinclude.h"

namespace aos{

  class BaseReqTask{
  public:
    BaseReqTask();
    virtual ~BaseReqTask();
  private:
  };

  class CreateAppTask : public BaseReqTask, public noncopyable{
  public:
    CreateAppTask();
    virtual ~CreateAppTask();
  private:
  };

}

#endif // ALI_OPENSEARCH_ALI_OPENSEARCH_REQ_TASK_H_