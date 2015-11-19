#ifndef ALI_OPENSEARCH_BASE_NONCOPYABLE_H_
#define ALI_OPENSEARCH_BASE_NONCOPYABLE_H_


namespace aos {

  //  Private copy constructor and copy assignment ensure classes derived from
  //  class noncopyable cannot be copied.

  //  Contributed by Dave Abrahams

  namespace noncopyable_  // protection from unintended ADL
  {
    class noncopyable
    {
    protected:
      noncopyable() {}
      ~noncopyable() {}
    private:  // emphasize the following members are private
      noncopyable(const noncopyable&);
      const noncopyable& operator=(const noncopyable&);
    };
  }

  typedef noncopyable_::noncopyable noncopyable;

} // namespace aos



#endif // ALI_OPENSEARCH_BASE_NONCOPYABLE_H_