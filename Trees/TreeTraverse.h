// Work in Progress

#pragma once

#include <cstdint>

using TraversalEnumeratorType = uint8_t;
enum class TraversalFashion : TraversalEnumeratorType;


#define JOIN(a,b) CONCAT(a,b)

#define CONCAT(a,b) a ## b

#define TRAVERSE_LEFT(ID, ROOT) \
	for(ID = ROOT; !ID; ID = ID->left);

#define TRAVERSE_RIGHT(ID, ROOT) \
		for(ID = ROOT; !ID; ID = ID->right);

using namespace tree_dfs {

//#define TRAVERSE_IN_ORDER(TYPE, ID, ROOT) \

}	