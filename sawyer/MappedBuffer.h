#ifndef Sawyer_MappedBuffer_H
#define Sawyer_MappedBuffer_H

#include <sawyer/Buffer.h>
#include <sawyer/Sawyer.h>
#include <boost/iostreams/device/mapped_file.hpp>

namespace Sawyer {
namespace Container {

/** Memory mapped file.
 *
 *  This buffer points to a file that is mapped into memory by the operating system.  The API supports a common divisor for
 *  POSIX and Microsoft Windows and is therefore not all that powerful, but it does allow simple maps to be created that have a
 *  file as backing store.  See http://www.boost.org/doc/libs for more information.
 *
 *  Access modes are the following enumerated constants:
 *
 *  @li <code>boost::iostreams::mapped_file::readonly</code>: shared read-only access
 *  @li <code>boost::iostreams::mapped_file::readwrite</code>: shared read/write access
 *  @li <code>boost::iostreams::mapped_file::priv</code>: private read/write access
 *  
 *  When a file is mapped with private access changes written to the buffer are not reflected in the underlying file. */
template<class A, class T>
class MappedBuffer: public Buffer<A, T> {
    boost::iostreams::mapped_file_params params_;
    boost::iostreams::mapped_file device_;
public:
    typedef A Address;
    typedef T Value;
protected:
    MappedBuffer(const boost::iostreams::mapped_file_params &params): params_(params), device_(params) {}

public:
    /** Map a file according to boost parameters.
     *
     *  The parameters describe which file (by name) and part thereof should be mapped into memory. */
    static typename Buffer<A, T>::Ptr instance(const boost::iostreams::mapped_file_params &params) {
        return typename Buffer<A, T>::Ptr(new MappedBuffer(params));
    }

    /** Map a file by name.
     *
     *  The specified file, which must already exist, is mapped into memory and pointed to by this new buffer. */
    static typename Buffer<A, T>::Ptr
    instance(const std::string &path, boost::iostreams::mapped_file::mapmode mode, boost::intmax_t offset=0,
             boost::iostreams::mapped_file::size_type length=boost::iostreams::mapped_file::max_length) {
        boost::iostreams::mapped_file_params params(path);
        params.flags = mode;
        params.length = length;
        params.offset = offset;
        return typename Buffer<A, T>::Ptr(new MappedBuffer(params));
    }

    Address available(Address address) const /*override*/ {
        return address >= device_.size() ? Address(0) : (Address(device_.size()) - address) / sizeof(Value);
    }

    void resize(Address n) /*override*/ {
        if (n != device_.size())
            throw std::runtime_error("resizing not allowed for MappedBuffer");
    }

    Address read(Value *buf, Address address, Address n) const /*override*/ {
        Address nread = std::min(n, available(address));
        memcpy(buf, device_.const_data() + address, nread * sizeof(Value));
        return nread;
    }

    Address write(const Value *buf, Address address, Address n) /*override*/ {
        Address nwritten = std::min(n, available(address));
        memcpy(device_.data() + address, buf, nwritten * sizeof(Value));
        return nwritten;
    }

    const Value* data() const /*override*/ {
        return (Value*)device_.const_data();
    }
};

} // namespace
} // namespace
#endif