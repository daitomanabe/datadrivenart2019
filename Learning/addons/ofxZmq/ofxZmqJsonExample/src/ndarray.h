//
//  ndarray.h
//
//  Created by ISHII 2bit on 2017/08/29.
//
//

#ifndef ndarray_h
#define ndarray_h

#include <array>
#include <vector>

namespace bbb {
    namespace detail {
        template <typename data_type, std::size_t ... shapes>
        struct ndarray;
        
        template <typename data_type, std::size_t shape, std::size_t ... shapes>
        struct ndarray<data_type, shape, shapes ...> {
            using type = std::array<typename ndarray<data_type, shapes ...>::type, shape>;
        };
        
        template <typename data_type>
        struct ndarray<data_type> {
            using type = data_type;
        };
    };
    
    template <typename type, std::size_t ... shapes>
    using ndarray = typename detail::ndarray<type, shapes ...>::type;
    
    namespace detail {
        template <typename data_type, std::size_t dim>
        struct flexarray {
            using type = std::vector<typename flexarray<data_type, dim - 1>::type>;
        };
        template <typename data_type>
        struct flexarray<data_type, 0UL> {
            using type = data_type;
        };
    }
    template <typename data_type, std::size_t dim>
    using flexarray = typename detail::flexarray<data_type, dim>::type;
};

using namespace bbb;

#endif /* ndarray_h */
