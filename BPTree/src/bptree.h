//
// Created by Pavel on 09.05.2022.
//

#ifndef BPTREE_T_H
#define BPTREE_T_H

#include <iostream>
#include <complex>

const size_t MAX = 3;

template<typename Type>
bool operator > (const std::complex<Type>& l, const std::complex<Type>& r) {
	if(norm(l) > norm(r))
		return true;
	else {
		if (norm(l) == norm(r))
			return l.real() > r.real();
		else
			return false;
	}
}

template<typename Type>
bool operator < (const std::complex<Type>& l, const std::complex<Type>& r) {
	if(norm(r) > norm(l))
		return true;
	else {
		if (norm(r) == norm(l))
			return r.real() > l.real();
		else
			return false;
	}
}

template<typename Type>
bool operator >= (const std::complex<Type>& l, const std::complex<Type>& r) {
	if (l == r || l > r)
		return true;
	else
		return false;
}

template<typename Type>
bool operator <= (const std::complex<Type>& l, const std::complex<Type>& r) {
	if (l == r || l < r)
		return true;
	else
		return false;
}

template <class Type> class BPTree;

template <class Type>
class BPNode {
	bool isLeaf{};
	Type* key;
	size_t size;
	BPNode<Type>** ptr;
	friend class BPTree<Type>;

public:
	BPNode();
};

template <class Type>
BPNode<Type>::BPNode() {
	key = new Type [MAX];
	ptr = new BPNode<Type>* [MAX + 1];
}

template <class Type>
class BPTree {
	BPNode<Type>* root;
	
	void insert_internal(Type x, BPNode<Type>* tmp, BPNode<Type> *child);
	void remove_internal(Type x, BPNode<Type>* tmp, BPNode<Type> *child);
	BPNode<Type>* find_parent(BPNode<Type>* tmp, BPNode<Type>* child);

public:
	BPTree();
	bool contains(Type x);
	void insert(Type x);
	void remove(Type x);
	void display(BPNode<Type>* tmp);
	BPNode<Type>* get_root();
};

template <class Type>
BPTree<Type>::BPTree() {
	root = nullptr;
}

template <class Type>
bool BPTree<Type>::contains(Type x) {
	if (!root)
		return false;
	else {
		BPNode<Type>* tmp = root;
		
		while (!tmp->isLeaf) {
			for (size_t i = 0; i < tmp->size; i++) {
				if (x < tmp->key[i]) {
					tmp = tmp->ptr[i];
					break;
				}
				if (i == tmp->size - 1) {
					tmp = tmp->ptr[i + 1];
					break;
				}
			}
		}
		for (size_t i = 0; i < tmp->size; i++)
			if (tmp->key[i] == x)
				return true;
		return false;
	}
}

template <class Type>
void BPTree<Type>::insert(Type x) {
	if (!root) {
		root = new BPNode<Type>;
		root->key[0] = x;
		root->isLeaf = true;
		root->size = 1;
	} else {
		BPNode<Type>* tmp = root;
		BPNode<Type>* parent;
		
		while (!tmp->isLeaf) {
			parent = tmp;
			for (size_t i = 0; i < tmp->size; i++) {
				if (x < tmp->key[i]) {
					tmp = tmp->ptr[i];
					break;
				}
				if (i == tmp->size - 1) {
					tmp = tmp->ptr[i + 1];
					break;
				}
			}
		}
		if (tmp->size < MAX) {
			size_t i = 0;
			while (x > tmp->key[i] && i < tmp->size)
				i++;
			for (size_t j = tmp->size; j > i; j--)
				tmp->key[j] = tmp->key[j - 1];
			tmp->key[i] = x;
			tmp->size++;
			tmp->ptr[tmp->size] = tmp->ptr[tmp->size - 1];
			tmp->ptr[tmp->size - 1] = nullptr;
		} else {
			BPNode<Type>* newLeaf = new BPNode<Type>;
			Type virtualNode[MAX + 1];
			
			for (size_t i = 0; i < MAX; i++)
				virtualNode[i] = tmp->key[i];
			size_t i = 0, j;
			while (x > virtualNode[i] && i < MAX)
				i++;
			for (size_t k = MAX + 1; k > i; k--)
				virtualNode[k] = virtualNode[k - 1];
			virtualNode[i] = x;
			newLeaf->isLeaf = true;
			tmp->size = (MAX + 1) / 2;
			newLeaf->size = MAX + 1 - (MAX + 1) / 2;
			tmp->ptr[tmp->size] = newLeaf;
			newLeaf->ptr[newLeaf->size] = tmp->ptr[MAX];
			tmp->ptr[MAX] = nullptr;
			for (i = 0; i < tmp->size; i++)
				tmp->key[i] = virtualNode[i];
			for (i = 0, j = tmp->size; i < newLeaf->size; i++, j++)
				newLeaf->key[i] = virtualNode[j];
			if (tmp == root) {
				BPNode<Type>* newRoot = new BPNode<Type>;
				
				newRoot->key[0] = newLeaf->key[0];
				newRoot->ptr[0] = tmp;
				newRoot->ptr[1] = newLeaf;
				newRoot->isLeaf = false;
				newRoot->size = 1;
				root = newRoot;
			} else
				insert_internal(newLeaf->key[0], parent, newLeaf);
		}
	}
}

template <class Type>
void BPTree<Type>::insert_internal(Type x, BPNode<Type> *tmp,
																	 BPNode<Type> *child) {
	if (tmp->size < MAX) {
		size_t i = 0;
		while (x > tmp->key[i] && i < tmp->size)
			i++;
		for (size_t j = tmp->size; j > i; j--)
			tmp->key[j] = tmp->key[j - 1];
		for (size_t j = tmp->size + 1; j > i + 1; j--)
			tmp->ptr[j] = tmp->ptr[j - 1];
		tmp->key[i] = x;
		tmp->size++;
		tmp->ptr[i + 1] = child;
	} else {
		BPNode<Type>* newInternal = new BPNode<Type>;
		Type virtualKey[MAX + 1];
		BPNode<Type>* virtualPtr[MAX + 2];
		
		for (size_t i = 0; i < MAX; i++)
			virtualKey[i] = tmp->key[i];
		for (size_t i = 0; i < MAX + 1; i++)
			virtualPtr[i] = tmp->ptr[i];
		size_t i = 0, j;
		while (x > virtualKey[i] && i < MAX)
			i++;
		for (size_t k = MAX + 1; k > i; k--)
			virtualKey[k] = virtualKey[k - 1];
		virtualKey[i] = x;
		for (size_t k = MAX + 2; k > i + 1; k--)
			virtualPtr[j] = virtualPtr[j - 1];
		virtualPtr[i + 1] = child;
		newInternal->isLeaf = false;
		tmp->size = (MAX + 1) / 2;
		newInternal->size = MAX - (MAX + 1) / 2;
		for (i = 0, j = tmp->size + 1; i < newInternal->size; i++, j++)
			newInternal->key[i] = virtualKey[j];
		for (i = 0, j = tmp->size + 1; i < newInternal->size + 1; i++, j++)
			newInternal->ptr[i] = virtualPtr[j];
		if (tmp == root) {
			BPNode<Type>* newRoot = new BPNode<Type>;
			
			newRoot->key[0] = tmp->key[tmp->size];
			newRoot->ptr[0] = tmp;
			newRoot->ptr[1] = newInternal;
			newRoot->isLeaf = false;
			newRoot->size = 1;
			root = newRoot;
		} else
			insert_internal(tmp->key[tmp->size],
											find_parent(root, tmp),
											newInternal);
	}
}

template <class Type>
BPNode<Type> *BPTree<Type>::find_parent(BPNode<Type>* tmp,
																				BPNode<Type>* child) {
	BPNode<Type>* parent;
	
	if (tmp->isLeaf || tmp->ptr[0]->isLeaf)
		return nullptr;
	for (size_t i = 0; i < tmp->size + 1; i++) {
		if (tmp->ptr[i] == child) {
			parent = tmp;
			return parent;
		} else {
			parent = find_parent(tmp->ptr[i], child);
			if (parent != nullptr)
				return parent;
		}
	}
	return parent;
}

template <class Type>
void BPTree<Type>::remove(Type x) {
	if (root) {
		BPNode<Type>* tmp = root;
		BPNode<Type>* parent;
		size_t leftSibling, rightSibling;
		
		while (!tmp->isLeaf) {
			for (size_t i = 0; i < tmp->size; i++) {
				parent = tmp;
				leftSibling = i - 1;
				rightSibling = i + 1;
				if (x < tmp->key[i]) {
					tmp = tmp->ptr[i];
					break;
				}
				if (i == tmp->size - 1) {
					leftSibling = i;
					rightSibling = i + 2;
					tmp = tmp->ptr[i + 1];
					break;
				}
			}
		}
		bool found = false;
		size_t pos;
		for (pos = 0; pos < tmp->size; pos++) {
			if (tmp->key[pos] == x) {
				found = true;
				break;
			}
		}
		if (!found)
			return;
		for (size_t i = pos; i < tmp->size; i++)
			tmp->key[i] = tmp->key[i + 1];
		tmp->size--;
		if (tmp == root) {
			for (size_t i = 0; i < MAX + 1; i++)
				tmp->ptr[i] = nullptr;
			if (tmp->size == 0) {
				delete[] tmp->key;
				delete[] tmp->ptr;
				delete tmp;
				root = nullptr;
			}
			return;
		}
		tmp->ptr[tmp->size] = tmp->ptr[tmp->size + 1];
		tmp->ptr[tmp->size + 1] = nullptr;
		if (tmp->size >= (MAX + 1) / 2)
			return;
		if (leftSibling >= 0) {
			BPNode<Type>* leftNode = parent->ptr[leftSibling];
			
			if (leftNode->size >= (MAX + 1) / 2 + 1) {
				for (size_t i = tmp->size; i > 0; i--)
					tmp->key[i] = tmp->key[i - 1];
				tmp->size++;
				tmp->ptr[tmp->size] = tmp->ptr[tmp->size - 1];
				tmp->ptr[tmp->size - 1] = nullptr;
				tmp->key[0] = leftNode->key[leftNode->size - 1];
				leftNode->size--;
				leftNode->ptr[leftNode->size] = tmp;
				leftNode->ptr[leftNode->size + 1] = nullptr;
				parent->key[leftSibling] = tmp->key[0];
				return;
			}
		}
		if (rightSibling <= parent->size) {
			BPNode<Type>* rightNode = parent->ptr[rightSibling];
			
			if (rightNode->size >= (MAX + 1) / 2 + 1) {
				tmp->size++;
				tmp->ptr[tmp->size] = tmp->ptr[tmp->size - 1];
				tmp->ptr[tmp->size - 1] = nullptr;
				tmp->key[tmp->size - 1] = rightNode->key[0];
				rightNode->size--;
				rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
				rightNode->ptr[rightNode->size + 1] = nullptr;
				for (size_t i = 0; i < rightNode->size; i++)
					rightNode->key[i] = rightNode->key[i + 1];
				parent->key[rightSibling - 1] = rightNode->key[0];
				return;
			}
		}
		if (leftSibling >= 0) {
			BPNode<Type>* leftNode = parent->ptr[leftSibling];
			
			for (size_t i = leftNode->size, j = 0; j < tmp->size; i++, j++)
				leftNode->key[i] = tmp->key[j];
			leftNode->ptr[leftNode->size] = nullptr;
			leftNode->size += tmp->size;
			leftNode->ptr[leftNode->size] = tmp->ptr[tmp->size];
			remove_internal(parent->key[leftSibling], parent, tmp);
			delete[] tmp->key;
			delete[] tmp->ptr;
			delete tmp;
		} else if (rightSibling <= parent->size) {
			BPNode<Type>* rightNode = parent->ptr[rightSibling];
			
			for (size_t i = tmp->size, j = 0; j < rightNode->size; i++, j++)
				tmp->key[i] = rightNode->key[j];
			tmp->ptr[tmp->size] = nullptr;
			tmp->size += rightNode->size;
			tmp->ptr[tmp->size] = rightNode->ptr[rightNode->size];
			remove_internal(parent->key[rightSibling - 1],
											parent,
											rightNode);
			delete[] rightNode->key;
			delete[] rightNode->ptr;
			delete rightNode;
		}
	}
}

template <class Type>
void BPTree<Type>::remove_internal(Type x,
																	 BPNode<Type> *tmp,
																	 BPNode<Type> *child) {
	if (tmp == root) {
		if (tmp->size == 1) {
			if (tmp->ptr[1] == child) {
				delete[] child->key;
				delete[] child->ptr;
				delete child;
				root = tmp->ptr[0];
				delete[] tmp->key;
				delete[] tmp->ptr;
				delete tmp;
				return;
			} else if (tmp->ptr[0] == child) {
				delete[] child->key;
				delete[] child->ptr;
				delete child;
				root = tmp->ptr[1];
				delete[] tmp->key;
				delete[] tmp->ptr;
				delete tmp;
				return;
			}
		}
	}
	size_t pos;
	for (pos = 0; pos < tmp->size; pos++)
		if (tmp->key[pos] == x)
			break;
	for (size_t i = pos; i < tmp->size; i++)
		tmp->key[i] = tmp->key[i + 1];
	for (pos = 0; pos < tmp->size + 1; pos++)
		if (tmp->ptr[pos] == child)
			break;
	for (size_t i = pos; i < tmp->size + 1; i++)
		tmp->ptr[i] = tmp->ptr[i + 1];
	tmp->size--;
	if (tmp->size >= (MAX + 1) / 2 - 1)
		return;
	if (tmp == root)
		return;
	
	BPNode<Type>* parent = find_parent(root, tmp);
	size_t leftSibling, rightSibling;
	
	for (pos = 0; pos < parent->size + 1; pos++) {
		if (parent->ptr[pos] == tmp) {
			leftSibling = pos - 1;
			rightSibling = pos + 1;
			break;
		}
	}
	if (leftSibling >= 0) {
		BPNode<Type>* leftNode = parent->ptr[leftSibling];
		
		if (leftNode->size >= (MAX + 1) / 2) {
			for (size_t i = tmp->size; i > 0; i--)
				tmp->key[i] = tmp->key[i - 1];
			tmp->key[0] = parent->key[leftSibling];
			parent->key[leftSibling] = leftNode->key[leftNode->size - 1];
			for (size_t i = tmp->size + 1; i > 0; i--)
				tmp->ptr[i] = tmp->ptr[i - 1];
			tmp->ptr[0] = leftNode->ptr[leftNode->size];
			tmp->size++;
			leftNode->size--;
			return;
		}
	}
	if (rightSibling <= parent->size) {
		BPNode<Type>* rightNode = parent->ptr[rightSibling];
		
		if (rightNode->size >= (MAX + 1) / 2) {
			tmp->key[tmp->size] = parent->key[pos];
			parent->key[pos] = rightNode->key[0];
			for (size_t i = 0; i < rightNode->size - 1; i++)
				rightNode->key[i] = rightNode->key[i + 1];
			tmp->ptr[tmp->size + 1] = rightNode->ptr[0];
			for (size_t i = 0; i < rightNode->size; ++i)
				rightNode->ptr[i] = rightNode->ptr[i + 1];
			tmp->size++;
			rightNode->size--;
			return;
		}
	}
	if (leftSibling >= 0) {
		BPNode<Type> *leftNode = parent->ptr[leftSibling];
		leftNode->key[leftNode->size] = parent->key[leftSibling];
		for (size_t i = leftNode->size + 1, j = 0; j < tmp->size; j++)
			leftNode->key[i] = tmp->key[j];
		for (size_t i = leftNode->size + 1, j = 0; j < tmp->size + 1; j++) {
			leftNode->ptr[i] = tmp->ptr[j];
			tmp->ptr[j] = nullptr;
		}
		leftNode->size += tmp->size + 1;
		tmp->size = 0;
		remove_internal(parent->key[leftSibling], parent, tmp);
	} else if (rightSibling <= parent->size) {
		BPNode<Type>* rightNode = parent->ptr[rightSibling];
		tmp->key[tmp->size] = parent->key[rightSibling - 1];
		for (size_t i = tmp->size + 1, j = 0; j < rightNode->size; j++)
			tmp->key[i] = rightNode->key[j];
		for (size_t i = tmp->size + 1, j = 0; j < rightNode->size + 1; j++) {
			tmp->ptr[i] = rightNode->ptr[j];
			rightNode->ptr[j] = nullptr;
		}
		tmp->size += rightNode->size + 1;
		rightNode->size = 0;
		remove_internal(parent->key[rightSibling - 1],
										parent,
										rightNode);
	}
}

template <class Type>
void BPTree<Type>::display(BPNode<Type>* tmp) {
	if (tmp) {
		if (tmp->isLeaf)
			std::cout << "LEAF [";
		for (size_t i = 0; i < tmp->size; i++)
			std::cout << tmp->key[i] << " ";
		if (tmp->isLeaf)
			std::cout << "]";
		std::cout << "\n";
		if (tmp->isLeaf != true)
			for (size_t i = 0; i < tmp->size + 1; i++)
				display(tmp->ptr[i]);
	}
}

template <class Type>
BPNode<Type>* BPTree<Type>::get_root() {
	return root;
}


#endif //BPTREE_T_H
