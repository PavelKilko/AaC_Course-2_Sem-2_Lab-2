#include "src/bptree.h"
#include <complex>



int main() {
	BPTree<std::complex<int>> node;
	node.insert(std::complex<int>(53, 50));
	node.insert(std::complex<int>(56, 58));
	node.insert(std::complex<int>(43, 1));
	node.insert(std::complex<int>(98, 29));
	node.insert(std::complex<int>(48, 47));
	node.insert(std::complex<int>(21, 37));
	node.insert(std::complex<int>(89, 43));
	node.insert(std::complex<int>(36, 31));
	node.insert(std::complex<int>(14, 44));
	node.display(node.get_root());
	std::cout << node.contains(std::complex<int>(89, 43)) << std::endl;
	node.remove(std::complex<int>(89, 43));
	node.display(node.get_root());
	std::cout << node.contains(std::complex<int>(89, 43)) << std::endl;
	return 0;
}