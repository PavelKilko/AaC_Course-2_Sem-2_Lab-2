
#include "src/bptree.h"
#include <complex>

int main() {
	BPTree<std::complex<int>> node;
	node.insert(5);
	node.insert(15);
	node.insert(25);
	node.insert(35);
	node.insert(45);
	node.insert(55);
	node.insert(40);
	node.insert(30);
	node.insert(20);
	node.display(node.get_root());
	node.remove(45);
	node.display(node.get_root());
//	std::cout << node.contains(15);
	return 0;
}
