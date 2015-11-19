#ifndef ALI_OPENSEARCH_ALI_OPENSEARCH_RES_VALUE_H_
#define ALI_OPENSEARCH_ALI_OPENSEARCH_RES_VALUE_H_

namespace aos{

  class BaseResValue{
  public:
    BaseResValue();
    virtual ~BaseResValue();
  };

  class CreateNewAppResValue : public BaseResValue{
  public:
    CreateNewAppResValue();
    virtual ~CreateNewAppResValue();
  };

}

#endif // ALI_OPENSEARCH_ALI_OPENSEARCH_RES_VALUE_H_