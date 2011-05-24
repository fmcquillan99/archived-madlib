/* ----------------------------------------------------------------------- *//**
 *
 * @file PGValue.hpp
 *
 * @brief Header file for automatic conversion of PostgreSQL Datums into values
 *
 *//* ----------------------------------------------------------------------- */

#ifndef MADLIB_POSTGRES_PGVALUE_HPP
#define MADLIB_POSTGRES_PGVALUE_HPP

#include <dbconnector/PGCommon.hpp>
#include <dbconnector/PGAbstractValue.hpp>

extern "C" {
    #include <fmgr.h>           // for FunctionCallInfo
    #include <access/htup.h>    // for HeapTupleHeader
}

namespace madlib {

namespace dbconnector {

using dbal::AbstractValue;
using dbal::AbstractValueSPtr;


template <typename T>
class PGValue;

template <>
class PGValue<FunctionCallInfo> : public PGAbstractValue {
public:
    PGValue<FunctionCallInfo>(const FunctionCallInfo inFCinfo)
        : fcinfo(inFCinfo) { }
    
protected:
    AbstractValueSPtr getValueByID(unsigned int inID) const;
    
    AbstractValueSPtr clone() const {
        return AbstractValueSPtr( new PGValue<FunctionCallInfo>(*this) );
    }
    
private:
    /**
     * The name is chosen so that PostgreSQL macros like PG_NARGS can be
     * used.
     */
    const FunctionCallInfo fcinfo;
};

template <>
class PGValue<HeapTupleHeader> : public PGAbstractValue {
public:    
    PGValue<HeapTupleHeader>(HeapTupleHeader inTuple)
        : mTuple(inTuple) { }

protected:
    AbstractValueSPtr getValueByID(unsigned int inID) const;

    AbstractValueSPtr clone() const {
        return AbstractValueSPtr( new PGValue<HeapTupleHeader>(*this) );
    }

private:    
    const HeapTupleHeader mTuple;
};

} // namespace dbconnector

} // namespace madlib

#endif
