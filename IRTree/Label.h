#include <utility>


#pragma once
#include "../SymbolTable/Symbol.h"
#include <map>



template<char prefix>
class CThingWithCounter
{

public:
    CThingWithCounter() : label( std::string( {prefix} ) + std::to_string( counter[""]++ ) )
    {
        static int x = 0;
        x++;
    }

    explicit CThingWithCounter( const std::string& _label, bool autolabel = false )
    {
        if( autolabel ) {
            if( counter.find( _label ) == counter.end() ) {
                counter[_label] = 1;
            }
            label = std::string( {prefix} ) + "_" + _label + "_" + std::to_string( counter[_label]++ );
        } else {
            label = std::string( {prefix} ) + "_" + _label;
        }
    }

    CThingWithCounter( const CThingWithCounter& other ) = default;

    std::string String() const
    {
        return label;
    }



    static void ResetCounter()
    {
        counter.clear();
    }

private:
    std::string label;
    static std::map<std::string, int> counter;
};

using Label = CThingWithCounter<'L'>;
using Temp = CThingWithCounter<'T'>;
