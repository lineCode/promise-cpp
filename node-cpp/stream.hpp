#ifndef INC_STREAM_HPP_
#define INC_STREAM_HPP_

#include <memory>
#include "promise.hpp"
#include "buffer.hpp"

namespace promise {

struct UvReadArg {
    Buffer buf_;
    unsigned int nbufs_;
    uv_stream_t *stream_;
    int ref_count_;

    UvReadArg(const UvReadArg &) = delete;
        
    UvReadArg(Buffer buf, unsigned int nbufs, uv_stream_t *stream)
        : buf_(buf), nbufs_(nbufs), stream_(stream), ref_count_(0) {
        printf("in %s %p\n", __func__, this);
    }
    ~UvReadArg() {
        printf("in %s %p %p\n", __func__, this, stream_);
        uv_read_stop(stream_);
    }

    void* operator new(size_t size){
        return allocator<UvReadArg>::obtain(size);
    }
    void operator delete(void *ptr) {
        allocator<UvReadArg>::release(ptr);
    }
};

struct UvRead {
    shared_ptr<UvReadArg> r_;
    ssize_t nread_;
};

Defer cuv_read(
    uv_stream_t *stream,
    UvRead &r,
    Buffer buf,
    unsigned int nbufs);

Defer cuv_read(
    UvRead &r,
    Buffer buf,
    unsigned int nbufs);

Defer cuv_write(
    uv_stream_t* handle,
    const uv_buf_t bufs[],
    unsigned int nbufs);

}
#endif