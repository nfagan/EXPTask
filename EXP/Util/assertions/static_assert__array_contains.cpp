//
//  static_assert__array_contains.cpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include <utility>
namespace EXP {
    namespace assertions {
        using Item = std::pair<int, int>;
        constexpr Item map_items[] = {
            { 6, 7 },
            { 10, 12 },
            { 300, 5000 },
        };
        constexpr auto map_size = sizeof map_items/sizeof map_items[0];
        
        static constexpr int find_value(int key, int range = map_size) {
            return
            (range == 0) ? -1 :
            (map_items[range - 1].first == key) ? map_items[range - 1].second:
            find_value(key, range - 1);
        };
        
        const int value = 6;
        
        constexpr int make_const(const int value_)
        {
            return value_;
        }
        
        constexpr bool assert__exists()
        {
//            constexpr int value_ = value;
            bool exists = std::integral_constant<int, find_value(value)>::value != -1;
            return exists;
//            static_assert(std::integral_constant<int, find_value(value_)>::value != -1, "Key does not exist");
        }
        
//        static_assert(std::integral_constant<int, find_value(value_)>::value != -1, "Key does not exist");
        static_assert(assert__exists(), "Key does not exist");
    }
}
