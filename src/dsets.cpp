/* Your code here! */
#include "dsets.h"

using namespace std;

void DisjointSets::addelements(int num)
{
    for (int i = 0; i < num; i++) {
        // root node is index -1
        nodes_.push_back(-1);
    }
}
int DisjointSets::find(int elem)
{
    // return when found root index
    if (nodes_[elem] < 0) return elem;
    int index = find(nodes_[elem]);
    nodes_[elem] = index;
    return index;
}
void DisjointSets::setunion(int a, int b)
{
    // a and b may not be roots, need to find roots

    // moves up the tree to find root of a
    while (nodes_[a] >= 0) {
        a = nodes_[a];
    }
    // moves up the tree to find root of b
    while (nodes_[b] >= 0) {
        b = nodes_[b];
    }
    int newSize = nodes_[a] + nodes_[b];
    // the more negative, the larger the set
    // union smaller set with bigger set or if equal, union second with first
    if (nodes_[a] <= nodes_[b]) {
        nodes_[b] = a;
        nodes_[a] = newSize;
    } else {
        nodes_[a] = b;
        nodes_[b] = newSize;
    }
}
int DisjointSets::size(int elem)
{
    while (nodes_[elem] >= 0) {
        elem = nodes_[elem];
    }
    return -1 * nodes_[elem];
}
