//
// Created by Pavel on 10.05.2022.
//

#ifndef SPLAYTREE_SPLAYTREE_H
#define SPLAYTREE_SPLAYTREE_H

#include <complex>

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

template <class T> class SplayTree;

template <class T>
class SplayNode
{
private:
	SplayNode<T>* leftChild;
	SplayNode<T>* rightChild;
	SplayNode<T>* parent;
	T data;
	
	friend class SplayTree<T>;
	
public:
	explicit SplayNode(const T& x);
};

template <class T>
SplayNode<T>::SplayNode(const T& x) {
	leftChild = nullptr;
	rightChild = nullptr;
	parent = nullptr;
	data = x;
}

template<typename T> class SplayTree {
private:
	SplayNode<T>* root;

private:
	SplayNode<T>* _successor(SplayNode<T>* localRoot) const;
	SplayNode<T>* _predecessor(SplayNode<T>* localRoot) const;
	SplayNode<T>* _minimum(SplayNode<T>* localRoot) const;
	SplayNode<T>* _maximum(SplayNode<T>* localRoot) const;
	SplayNode<T>* _search(const T& x);
	void _left_rotate(SplayNode<T>* localRoot);
	void _right_rotate(SplayNode<T>* localRoot);
	void _transplant(SplayNode<T>* localParent, SplayNode<T>* localChild);
	void _splay(SplayNode<T>* pivotNode);
	
public:
	SplayTree();
	virtual ~SplayTree();
	void insert(const T& x);
	void remove(const T& x);
	bool contains(const T& x);
	[[nodiscard]] bool empty() const;
	T successor(const T& x);
	T predecessor(const T& x);
	void display(SplayNode<T>* localRoot, size_t t = 0);
	SplayNode<T>* get_root();
};

template <class T>
SplayNode<T>* SplayTree<T>::_successor(SplayNode<T>* localRoot) const {
	SplayNode<T>* successor = localRoot;
	
	if (!successor)
		return successor;
	else if (successor->rightChild)
		successor = _minimum(successor->rightChild);
	else
		while (successor != root || successor != successor->parent->leftChild)
			successor = successor->parent;
	
	return successor;
}

template <class T>
SplayNode<T>* SplayTree<T>::_predecessor(SplayNode<T>* localRoot) const {
	SplayNode<T>* predecessor = localRoot;
	
	if (!predecessor)
		return nullptr;
	if (predecessor->leftChild)
		predecessor = _maximum(predecessor->leftChild);
	else
		while (predecessor != root
					|| predecessor != predecessor->parent->rightChild)
			predecessor = predecessor->parent;
	
	return predecessor;
}

template <class T>
SplayNode<T>* SplayTree<T>::_minimum(SplayNode<T>* localRoot) const {
	SplayNode<T>* minimum = localRoot;
	
	while (minimum->leftChild)
		minimum = minimum->leftChild;
	
	return minimum;
}

template <class T>
SplayNode<T>* SplayTree<T>::_maximum(SplayNode<T>* localRoot) const {
	SplayNode<T>* maximum = localRoot;
	
	while (maximum->rightChild)
		maximum = maximum->rightChild;
	
	return maximum;
}

template <class T>
SplayNode<T>* SplayTree<T>::_search(const T& x) {
	SplayNode<T>* searchedElement = root;
	
	while (searchedElement)
		if (searchedElement->data < x)
			searchedElement = searchedElement->rightChild;
		else if (x < searchedElement->data)
			searchedElement = searchedElement->leftChild;
		else if (searchedElement->data == x) {
			_splay(searchedElement);
			return searchedElement;
		}
	
	return nullptr;
}

template <class T>
void SplayTree<T>::_left_rotate(SplayNode<T>* localRoot) {
	SplayNode<T>* rightChild = localRoot->rightChild;
	
	localRoot->rightChild = rightChild->leftChild;
	if (rightChild->leftChild)
		rightChild->leftChild->parent = localRoot;
	
	_transplant(localRoot, rightChild);
	
	rightChild->leftChild = localRoot;
	rightChild->leftChild->parent = rightChild;
}

template <class T>
void SplayTree<T>::_right_rotate(SplayNode<T>* localRoot) {
	SplayNode<T>* leftChild = localRoot->leftChild;
	
	localRoot->leftChild = leftChild->rightChild;
	if (leftChild->rightChild)
		leftChild->rightChild->parent = localRoot;
	
	_transplant(localRoot, leftChild);
	
	leftChild->rightChild = localRoot;
	leftChild->rightChild->parent = leftChild;
}

template <class T>
void SplayTree<T>::_transplant(SplayNode<T>* localParent,
															 SplayNode<T>* localChild) {
	if (!localParent->parent)
		root = localChild;
	else if (localParent == localParent->parent->leftChild)
		localParent->parent->leftChild = localChild;
	else if (localParent == localParent->parent->rightChild)
		localParent->parent->rightChild = localChild;
	
	if (localChild)
		localChild->parent = localParent->parent;
}

template <class T>
void SplayTree<T>::_splay(SplayNode<T>* pivotNode) {
	while (pivotNode != root) {
		if (pivotNode->parent == root) {
			if (pivotNode == pivotNode->parent->leftChild)
				_right_rotate(pivotNode->parent);
			else if (pivotNode == pivotNode->parent->rightChild)
				_left_rotate(pivotNode->parent);
		} else {
			// Zig-Zig step.
			if (pivotNode == pivotNode->parent->leftChild
					&& pivotNode->parent == pivotNode->parent->parent->leftChild) {
				_right_rotate(pivotNode->parent->parent);
				_right_rotate(pivotNode->parent);
			} else if (pivotNode == pivotNode->parent->rightChild
								&& pivotNode->parent == pivotNode->parent->parent->rightChild) {
				_left_rotate(pivotNode->parent->parent);
				_left_rotate(pivotNode->parent);
			}
				// Zig-Zag step.
			else if (pivotNode == pivotNode->parent->rightChild
							&& pivotNode->parent == pivotNode->parent->parent->leftChild) {
				_left_rotate(pivotNode->parent);
				_right_rotate(pivotNode->parent);
			} else if (pivotNode == pivotNode->parent->leftChild &&
								 pivotNode->parent == pivotNode->parent->parent->rightChild) {
				_right_rotate(pivotNode->parent);
				_left_rotate(pivotNode->parent);
			}
		}
	}
}

template <class T>
SplayTree<T>::SplayTree() {
	root = nullptr;
}

template <class T>
SplayTree<T>::~SplayTree() {
	delete root;
}

template <class T>
void SplayTree<T>::insert(const T& x) {
	SplayNode<T>* preInsertPlace = nullptr;
	SplayNode<T>* insertPlace = root;
	
	while (insertPlace) {
		preInsertPlace = insertPlace;
		
		if (insertPlace->data < x)
			insertPlace = insertPlace->rightChild;
		else if (x <= insertPlace->data)
			insertPlace = insertPlace->leftChild;
	}
	
	SplayNode<T>* insertElement = new SplayNode(x);
	insertElement->parent = preInsertPlace;
	
	if (!preInsertPlace)
		root = insertElement;
	else if (preInsertPlace->data < insertElement->data)
		preInsertPlace->rightChild = insertElement;
	else if (insertElement->data < preInsertPlace->data)
		preInsertPlace->leftChild = insertElement;
	
	_splay(insertElement);
}

template <class T>
void SplayTree<T>::remove(const T& x) {
	SplayNode<T>* removedNode = _search(x);
	
	if (removedNode) {
		if (removedNode->rightChild == nullptr)
			_transplant(removedNode, removedNode->leftChild);
		else if (removedNode->leftChild == nullptr)
			_transplant(removedNode, removedNode->rightChild);
		else {
			SplayNode<T>* newLocalRoot = _minimum(removedNode->rightChild);
			
			if (newLocalRoot->parent != removedNode) {
				_transplant(newLocalRoot, newLocalRoot->rightChild);
				newLocalRoot->rightChild = removedNode->rightChild;
				newLocalRoot->rightChild->parent = newLocalRoot;
			}
			
			_transplant(removedNode, newLocalRoot);
			
			newLocalRoot->leftChild = removedNode->leftChild;
			newLocalRoot->leftChild->parent = newLocalRoot;
			
			_splay(newLocalRoot);
		}
		
		delete removedNode;
	}
}

template <class T>
bool SplayTree<T>::contains(const T& x) {
	return _search(x);
}

template <class T>
bool SplayTree<T>::empty() const {
	return root;
}

template <class T>
T SplayTree<T>::successor(const T& x) {
	SplayNode<T>* successor = _successor(_search(x));
	
	if (successor)
		return successor->data;
	else
		return 0;
}

template <class T>
T SplayTree<T>::predecessor(const T& x) {
	SplayNode<T>* predecessor = _predecessor(_search(x));
	
	if (predecessor)
		return predecessor->data;
	else
		return 0;
}

template <class T>
void SplayTree<T>::display(SplayNode<T>* localRoot, size_t t) {
	if (localRoot) {
		if (!t)
			std::cout << "root: ";
		
		std::cout << localRoot->data << " {" << std::endl;
		
		for (size_t j = 0; j <= t; j++)
			std::cout << "| ";
		std::cout << "left: ";
		display(localRoot->leftChild, t+1);
		
		for (size_t j = 0; j <= t; j++)
			std::cout << "| ";
		std::cout << "right: ";
		display(localRoot->rightChild, t+1);
		
		for (size_t j = 0; j < t; j++)
			std::cout << "| ";
		std::cout << "}" << std::endl;
	} else
		std::cout << "nullptr" << std::endl;
}

template <class T>
SplayNode<T>* SplayTree<T>::get_root() {
	return root;
}

#endif //SPLAYTREE_SPLAYTREE_H
