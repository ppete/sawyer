#ifndef Sawyer_NullBuffer_H
#define Sawyer_NullBuffer_H

#include <sawyer/Buffer.h>

namespace Sawyer {
namespace Container {

/** Buffer that has no data.
 *
 *  This can be useful to reserve areas of of a BufferMap address space without actually storing any data at them.  All
 *  reads return default values and writes using such a buffer will fail (return zero). */
template<class A, class T>
class NullBuffer: public Buffer<A, T> {
public:
    typedef A Address;
    typedef T Value;
private:
    Address size_;
protected:
    NullBuffer(): size_(0) {}
    explicit NullBuffer(Address size): size_(size) {}
public:
    /** Construct a new buffer.
     *
     *  The new buffer will act as if it contains @p size values, although no values will actually be stored. */
    static typename Buffer<A, T>::Ptr instance(Address size) {
        return typename Buffer<A, T>::Ptr(new NullBuffer(size));
    }

    Address available(Address start) const /*override*/ {
        return start < size_ ? size_ - start : 0;
    }

    void resize(Address newSize) /*override*/ {
        size_ = newSize;
    }

    Address read(Value *buf, Address address, Address n) const /*override*/ {
        Address nread = std::min(available(address), n);
        if (buf) {
            for (Address i=0; i<n; ++i)
                buf[i] = Value();
        }
        return nread;
    }

    Address write(const Value *buf, Address address, Address n) /*override*/ {
        return 0;
    }
};

} // namespace
} // namespace

#endif
