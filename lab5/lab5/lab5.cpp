#include <iostream>
#include <iomanip>
#include <stack>
#include <string>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Player // клас гравця
{
public:

	Player(const std::string& name)
	{
		this->name = name;
	}
	string get_name() const
	{
		return this->name;
	}
private:
	string name;
};

class Team // клас команди
{
public:
	Team() = default;
	Team(const string& name)
	{
		this->name = name;
	}
	void add_player(const string& name)
	{
		players.push_back(Player(name));
	}
	string get_name() const
	{
		return this->name;
	}
	friend ostream& operator<< (std::ostream& out, const Team& section);
	friend bool operator==(const Team& first, const Team& second);
	friend bool operator<=(const Team& first, const Team& second);
	friend bool operator<(const Team& first, const Team& second);
	friend bool operator>(const Team& first, const Team& second);
private:
	string name;
	vector<Player> players;
};

std::ostream& operator<<(std::ostream& out, const Team& team)
{
	out << " " << team.name << " { ";
	for (const auto a : team.players)
	{
		out << a.get_name() + " ";
	}
	out << "}";
	return out;
}

bool operator==(const Team& first, const Team& second)
{
	return first.name == second.name;
}

bool operator<=(const Team& first, const Team& second)
{
	return first.name <= second.name;
}

bool operator<(const Team& first, const Team& second)
{
	return first.name < second.name;
}

bool operator>(const Team& first, const Team& second)
{
	return first.name > second.name;
}

enum Color { RED, BLACK };

template <typename T>
struct treeNode {
	int ID;
	static int number;
	T key;
	Color color = BLACK;
	treeNode* parent = nullptr;
	treeNode* left = nullptr;
	treeNode* right = nullptr;
	size_t descendants = 0;
	int generation = 0;

	treeNode() :ID(number++) {};
};

template<typename T>
int treeNode<T>::number = 0;

template <typename T>
class RedBlackTree {
	treeNode<T>* nill;
	treeNode<T>* root;
	vector<treeNode<T>*> persistentRoots;

	treeNode<T>* copy(treeNode<T>* origin);

	void leftRotate(treeNode<T>* x);
	void rightRotate(treeNode<T>* x);

	void insertFixup(treeNode<T>* z);
	void insertFixupPersistent(treeNode<T>* z);

	void removeFixup(treeNode<T>* x);
	void removeFixupPersistent(treeNode<T>* x);

	treeNode<T>* treeSuccessor(treeNode<T>* x);
	treeNode<T>* treeSuccessorPersistent(treeNode<T>* x);

	treeNode<T>* treeMinimum(treeNode<T>* x);
	treeNode<T>* treeMinimumPersistent(treeNode<T>* x, treeNode<T>* pre);

	treeNode<T>* treeMaximum(treeNode<T>* x);
	treeNode<T>* treeSearch(treeNode<T>* x, const T& key);

	string getText(treeNode<T>* x);
	void getGraphInfo(treeNode<T>* x, string& text);
	void getGraphInfoPersistent(treeNode<T>* x, string& text, int gen);

public:

	RedBlackTree();
	RedBlackTree(const initializer_list<T>& list, bool isPersistent = false);

	void skipBack();
	void clear(treeNode<T>* x, int gen);

	void inorderTreeWalk(treeNode<T>* x, int depth = 0);

	template<typename U>
	friend ostream& operator<<(ostream& os, RedBlackTree<U>& tree);

	string getWebGraphviz(string graphName = "G");
	string getWebGraphvizPersistent(string graphName = "G");

	void insert(const T& item);
	void insertPersistent(const T& item);

	size_t size() const;

	treeNode<T>* treeSearch(const T& key);
	treeNode<T>* treeSearchPersistent(const T& key);




	treeNode<T>* remove(const T& item);
	treeNode<T>* removePersistent(const T& item);
};

template<typename T>
RedBlackTree<T>::RedBlackTree() {
	nill = new treeNode<T>();
	root = nill;
	root->parent = nill;
}

template<typename T>
RedBlackTree<T>::RedBlackTree(const initializer_list<T>& list, bool isPersistent) {
	nill = new treeNode<T>();
	root = nill;
	root->parent = nill;

	for (const auto& item : list) {
		if (isPersistent) {
			insertPersistent(item);
		}
		else
			insert(item);
	}

	if (!isPersistent)
		persistentRoots.push_back(root);
}

template <typename T>
treeNode<T>* RedBlackTree<T>::copy(treeNode<T>* origin) {
	if (origin == nill) {
		return origin;
	}

	auto destination = new treeNode<T>();

	destination->key = origin->key;
	destination->color = origin->color;
	destination->parent = origin->parent;
	destination->left = origin->left;
	destination->right = origin->right;
	destination->generation = root->generation;
	destination->descendants = origin->descendants;

	return destination;
}

template<typename T>
void RedBlackTree<T>::leftRotate(treeNode<T>* x) {
	treeNode<T>* y = x->right;
	x->right = y->left;

	int alpha = x->left->descendants + 1, gamma = y->right->descendants + 1;
	if (x->left == nill) {
		alpha--;
	}
	if (y->right == nill) {
		gamma--;
	}

	x->descendants -= (gamma + 1);
	y->descendants += (alpha + 1);

	if (y->left != nill) {
		y->left->parent = x;
	}

	y->parent = x->parent;

	if (x->parent == nill) {
		root = y;
	}
	else {
		if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
	}
	y->left = x;
	x->parent = y;
}

template<typename T>
void RedBlackTree<T>::rightRotate(treeNode<T>* y) {
	treeNode<T>* x = y->left;
	y->left = x->right;

	int alpha = x->left->descendants + 1, gamma = y->right->descendants + 1;
	if (x->left == nill) {
		alpha--;
	}
	if (y->right == nill) {
		gamma--;
	}

	x->descendants += (gamma + 1);
	y->descendants -= (alpha + 1);

	if (x->right != nill) {
		x->right->parent = y;
	}

	x->parent = y->parent;

	if (y->parent == nill) {
		root = x;
	}
	else {
		if (y == y->parent->right) {
			y->parent->right = x;
		}
		else {
			y->parent->left = x;
		}
	}
	x->right = y;
	y->parent = x;
}

template<typename T>
void RedBlackTree<T>::insertFixup(treeNode<T>* z) {
	treeNode<T>* y;

	while (z->parent->color == RED) {
		if (z->parent == z->parent->parent->left) {
			y = z->parent->parent->right;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->right) {
					z = z->parent;
					leftRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rightRotate(z->parent->parent);
			}
		}
		else {
			y = z->parent->parent->left;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {
					z = z->parent;
					rightRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				leftRotate(z->parent->parent);
			}
		}
	}

	root->color = BLACK;
}

template<typename T>
void RedBlackTree<T>::insertFixupPersistent(treeNode<T>* z) {
	treeNode<T>* y;

	while (z->parent->color == RED) {
		if (z->parent == z->parent->parent->left) {

			auto newUncle = copy(z->parent->parent->right);
			newUncle->parent = z->parent->parent;
			z->parent->parent->right = newUncle;

			y = z->parent->parent->right;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->right) {

					auto newLeft = copy(z->left);
					newLeft->parent = z;
					z->left = newLeft;

					z = z->parent;
					leftRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				auto newRight = copy(z->parent->right);
				newRight->parent = z->parent;
				z->parent->right = newRight;

				rightRotate(z->parent->parent);
			}
		}
		else {

			auto newUncle = copy(z->parent->parent->left);
			newUncle->parent = z->parent->parent;
			z->parent->parent->left = newUncle;

			y = z->parent->parent->left;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {

					auto newRight = copy(z->right);
					newRight->parent = z;
					z->right = newRight;

					z = z->parent;
					rightRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				auto newLeft = copy(z->parent->left);
				newLeft->parent = z->parent;
				z->parent->left = newLeft;

				leftRotate(z->parent->parent);
			}
		}
	}

	if (root != persistentRoots[root->generation])
		persistentRoots[root->generation] = root;

	root->color = BLACK;
}

template<typename T>
void RedBlackTree<T>::removeFixup(treeNode<T>* x) {
	while (x != root && x->color == BLACK) {
		treeNode<T>* w;
		if (x == x->parent->left) {
			w = x->parent->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				leftRotate(x->parent);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					rightRotate(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				leftRotate(x->parent);
				x = root;
			}
		}
		else {
			w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				rightRotate(x->parent);
				w = x->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					leftRotate(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rightRotate(x->parent);
				x = root;
			}
		}
	}
	x->color = BLACK;
}

template<typename T>
void RedBlackTree<T>::removeFixupPersistent(treeNode<T>* x) {
	while (x != root && x->color == BLACK) {
		treeNode<T>* w;
		if (x == x->parent->left) {

			auto newW = copy(x->parent->right);
			newW->parent = x->parent;
			x->parent->right = newW;
			w = newW;

			//w = x->parent->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;

				auto newLeft = copy(w->left);
				w->left = newLeft;
				newLeft->parent = w;

				leftRotate(x->parent);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) {

					auto newLeft = copy(w->left);
					w->left = newLeft;
					newLeft->parent = w;

					auto newRight = copy(w->left->right);
					w->left->right = newRight;
					newRight->parent = w->left;

					w->left->color = BLACK;
					w->color = RED;
					rightRotate(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;

				auto newLeft = copy(w->left);
				w->left = newLeft;
				newLeft->parent = w;

				auto newRight = copy(w->right);
				w->right = newRight;
				newRight->parent = w;

				x->parent->color = BLACK;
				w->right->color = BLACK;
				leftRotate(x->parent);
				x = root;
			}
		}
		else {

			auto newW = copy(x->parent->left);
			newW->parent = x->parent;
			x->parent->left = newW;
			w = newW;

			//w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;

				auto newRight = copy(w->right);
				w->right = newRight;
				newRight->parent = w;

				rightRotate(x->parent);
				w = x->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->left->color == BLACK) {

					auto newRight = copy(w->right);
					w->right = newRight;
					newRight->parent = w;

					auto newLeft = copy(w->right->left);
					w->right->left = newLeft;
					newLeft->parent = w->right;

					w->right->color = BLACK;
					w->color = RED;
					leftRotate(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;

				auto newRight = copy(w->right);
				w->right = newRight;
				newRight->parent = w;

				auto newLeft = copy(w->left);
				w->left = newLeft;
				newLeft->parent = w;

				x->parent->color = BLACK;
				w->left->color = BLACK;
				rightRotate(x->parent);
				x = root;
			}
		}
	}
	x->color = BLACK;
}

template<typename T>
treeNode<T>* RedBlackTree<T>::treeSuccessor(treeNode<T>* x) {
	if (x->right != nill) {
		return treeMinimum(x->right);
	}
	treeNode<T>* y = x->parent;
	while (y != nill && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}

template<typename T>
treeNode<T>* RedBlackTree<T>::treeSuccessorPersistent(treeNode<T>* x) {
	if (x->right != nill) {

		return treeMinimumPersistent(x->right, x);
	}
	treeNode<T>* y = x->parent;
	while (y != nill && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}

template<typename T>
treeNode<T>* RedBlackTree<T>::treeMinimum(treeNode<T>* x) {
	while (x->left != nill) {
		x = x->left;
	}
	return x;
}

template<typename T>
treeNode<T>* RedBlackTree<T>::treeMinimumPersistent(treeNode<T>* x, treeNode<T>* pre) {
	auto newPre = new treeNode<T>();

	if (x->left != nill) {
		auto newX = copy(x);
		newX->parent = pre;

		pre->right = newX;

		x = newX;
		newPre = x;
		x = x->left;
	}

	while (x->left != nill) {
		auto newX = copy(x);
		newX->parent = newPre;
		x = newX;
		x = x->left;
	}

	if (x != nill) {
		auto newX = copy(x);
		newX->parent = newPre;
		if (x == newPre->left) {
			newPre->left = newX;
		}
		else {
			newPre->right = newX;
		}
		x = newX;
	}

	return x;
}

template<typename T>
treeNode<T>* RedBlackTree<T>::treeMaximum(treeNode<T>* x) {
	while (x->right != nill) {
		x = x->right;
	}
	return x;
}

template<typename T>
treeNode<T>* RedBlackTree<T>::treeSearch(treeNode<T>* x, const T& key) {
	if (x == nill || key == x->key) {
		return x;
	}
	if (key < x->key)
		return treeSearch(x->left, key);
	else
		return treeSearch(x->right, key);
}

template<typename T>
void RedBlackTree<T>::insert(const T& item) {
	treeNode<T>* z = new treeNode<T>();
	z->key = item;

	treeNode<T>* y = nill;
	treeNode<T>* x = root;

	while (x != nill) {
		y = x;
		x->descendants++;
		if (z->key < x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}

	z->parent = y;
	if (y == nill) {
		root = z;
	}
	else {
		if (z->key < y->key) {
			y->left = z;
		}
		else {
			y->right = z;
		}
	}

	z->left = nill;
	z->right = nill;
	z->color = RED;

	insertFixup(z);
}

template<typename T>
void RedBlackTree<T>::insertPersistent(const T& item) {
	if (root == nill) {
		insert(item);
		persistentRoots.push_back(root);
		return;
	}
	else {
		auto newRoot = copy(root);
		persistentRoots.push_back(newRoot);
		root = newRoot;
		root->generation++;
	}

	treeNode<T>* z = new treeNode<T>();
	z->key = item;

	treeNode<T>* y = nill;
	treeNode<T>* x = root;

	while (x != nill) {
		if (x != root) {
			auto newX = copy(x);
			newX->parent = y;
			if (y->left == x) {
				y->left = newX;
			}
			else {
				y->right = newX;
			}

			x = newX;
		}
		y = x;
		x->descendants++;

		if (z->key <= x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}

	z->parent = y;
	z->generation = y->generation;
	if (y == nill) {
		root = z;
	}
	else {
		if (z->key <= y->key) {
			y->left = z;
		}
		else {
			y->right = z;
		}
	}

	z->left = nill;
	z->right = nill;
	z->color = RED;

	insertFixupPersistent(z);
}

template<typename T>
size_t RedBlackTree<T>::size() const {
	if (root != nill)
		return root->descendants + 1;
	else
		return 0;
}

template<typename T>
treeNode<T>* RedBlackTree<T>::treeSearch(const T& key) {
	return treeSearch(root, key);
}

template<typename T>
treeNode<T>* RedBlackTree<T>::treeSearchPersistent(const T& key) {
	if (root != nill) {
		auto newRoot = copy(root);
		persistentRoots.push_back(newRoot);
		root = newRoot;
		root->generation++;
	}

	treeNode<T>* pre = nill;
	treeNode<T>* iter = root;

	while (iter->key != key && iter != nill) {
		if (iter != root) {
			auto newIter = copy(iter);
			newIter->parent = pre;
			if (pre->left == iter) {
				pre->left = newIter;
			}
			else {
				pre->right = newIter;
			}

			iter = newIter;
		}
		pre = iter;

		if (key < iter->key) {
			iter = iter->left;
		}
		else {
			iter = iter->right;
		}
	}

	if (iter != root) {
		auto newIter = copy(iter);
		newIter->parent = pre;
		if (pre->left == iter) {
			pre->left = newIter;
		}
		else {
			pre->right = newIter;
		}

		iter = newIter;
	}

	return iter;
}



template<typename T>
treeNode<T>* RedBlackTree<T>::remove(const T& item) {
	treeNode<T>* z = treeSearch(root, item);
	treeNode<T>* decreaser;

	if (z == nill)
		throw invalid_argument("There's no element with such parameters.");

	treeNode<T>* x, * y;

	if (z->left == nill || z->right == nill)
		y = z;
	else
		y = treeSuccessor(z);

	decreaser = y->parent;

	if (y->left != nill)
		x = y->left;
	else
		x = y->right;

	x->parent = y->parent;

	if (y->parent == nill) {
		root = x;
	}
	else {
		if (y == y->parent->left) {
			y->parent->left = x;
		}
		else {
			y->parent->right = x;
		}
	}

	if (y != z) {
		z->key = y->key;
	}

	while (decreaser != nill) {
		decreaser->descendants--;
		decreaser = decreaser->parent;
	}

	if (y->color == BLACK) {
		removeFixup(x);
	}

	return y;
}

template<typename T>
treeNode<T>* RedBlackTree<T>::removePersistent(const T& item) {
	treeNode<T>* z = treeSearchPersistent(item);
	treeNode<T>* decreaser;

	if (z == nill)
		throw invalid_argument("There's no element with such parameters.");

	treeNode<T>* x, * y;

	if (z->left == nill || z->right == nill)
		y = z;
	else {
		y = treeSuccessorPersistent(z);
	}

	decreaser = y->parent;

	if (y->left != nill) {

		auto newLeft = copy(y->left);
		y->left = newLeft;

		x = y->left;
	}
	else {

		auto newRight = copy(y->right);
		y->right = newRight;

		x = y->right;
	}

	x->parent = y->parent;

	if (y->parent == nill) {
		root = x;
	}
	else {
		if (y == y->parent->left) {
			y->parent->left = x;
		}
		else {
			y->parent->right = x;
		}
	}

	if (y != z) {
		z->key = y->key;
	}

	while (decreaser != nill) {
		decreaser->descendants--;
		decreaser = decreaser->parent;
	}

	if (y->color == BLACK) {
		removeFixupPersistent(x);
	}

	return y;
}

template<typename T>
void RedBlackTree<T>::clear(treeNode<T>* x, int gen) {
	if (x->left != nill)
		clear(x->left, gen);
	if (x->right != nill)
		clear(x->right, gen);
	if (x->generation == gen)
		delete x;
}

template<typename T>
void RedBlackTree<T>::skipBack() {
	if (persistentRoots.empty())
		return;
	persistentRoots.pop_back();
	clear(root, root->generation);

	root = persistentRoots[persistentRoots.size() - 1];
}

template<typename T>
void RedBlackTree<T>::inorderTreeWalk(treeNode<T>* x, int depth) {
	if (x != nill) {
		inorderTreeWalk(x->left, depth + 1);
		cout << x->key << " Color: " << (x->color ? "BLACK" : "RED") << ";  nodeDepth = " << depth << ";  Descendants: " << x->descendants << endl;
		inorderTreeWalk(x->right, depth + 1);
	}
}

template<typename U>
ostream& operator<<(ostream& os, RedBlackTree<U>& tree) {
	tree.inorderTreeWalk(tree.root);
	return os;
}

template<typename T>
void RedBlackTree<T>::getGraphInfo(treeNode<T>* x, string& text) {
	if (x != nill) {
		if (x->color == RED)
			text += "\"" + to_string(x->key) + "\" [color = \"red\"];\n";
		else if (x == root)
			text += "\"" + to_string(x->key) + "\";\n";
		if (x->left != nill)
			text += "\"" + to_string(x->key) + "\"" + " -> " + "\"" + to_string(x->left->key) + "\";\n";
		if (x->right != nill)
			text += "\"" + to_string(x->key) + "\"" + " -> " + "\"" + to_string(x->right->key) + "\";\n";

		getGraphInfo(x->left, text);
		getGraphInfo(x->right, text);
	}
}

template<typename T>
string RedBlackTree<T>::getText(treeNode<T>* x) {
	return to_string(x->key) + ' ' + to_string(x->generation) + " ID: " + to_string(x->ID);
}



template<>
string RedBlackTree<Team>::getText(treeNode<Team>* x) {
	return x->key.get_name() + ' ' + to_string(x->generation) + " ID: " + to_string(x->ID);
}

template<typename T>
void RedBlackTree<T>::getGraphInfoPersistent(treeNode<T>* x, string& text, int gen) {
	string currentNodeText = getText(x);
	stringstream stream;

	stream << "\"" << currentNodeText << "\";\n";

	if (x != nill) {
		if (x->color == RED && x->generation <= gen)
			stream << "\"" << getText(x) << "\" [color = \"red\"];\n";
		else if (x == root && x->generation <= gen)
			stream << "\"" << getText(x) << "\";\n";
		if (x->left != nill && x->left->generation <= gen)
			stream << "\"" << getText(x) << "\"" << " -> " << "\"" << getText(x->left) << "\";\n";
		if (x->right != nill && x->right->generation <= gen)
			stream << "\"" << getText(x) << "\"" << " -> " << "\"" << getText(x->right) << "\";\n";

		text += stream.str();
		stream.str(string());

		getGraphInfoPersistent(x->left, text, gen);
		getGraphInfoPersistent(x->right, text, gen);
	}
}

template<typename T>
string RedBlackTree<T>::getWebGraphviz(string graphName) {
	string graphText 

	getGraphInfo(root, graphText);

	return graphText += "}";
}

template<typename T>
string RedBlackTree<T>::getWebGraphvizPersistent(string graphName) {
	string graphText;

	if (persistentRoots.size() >= 2)
		getGraphInfoPersistent(persistentRoots[persistentRoots.size() - 2], graphText, root->generation - 1);
	getGraphInfoPersistent(root, graphText, root->generation);

	return graphText += "}";
}

int main()
{
	auto my_tree = new RedBlackTree<Team>({}, true);
	string operation;
	while (true)
	{
		cin >> operation;
		if (operation == "addteam")
		{
			string team_name;
			int players_count;
			cin >> team_name;
			cin >> players_count;
			Team new_team(team_name);

			string players_name;
			for (int i = 0; i < players_count; i++)
			{
				cin >> players_name;
				new_team.add_player(players_name);
			}
			my_tree->insertPersistent(new_team);
		}
		else if (operation == "erase")
		{
			string team_name;
			cin >> team_name;
			my_tree->removePersistent(Team(team_name));
		}
		else if (operation == "back")
		{
			my_tree->skipBack();
		}
		else if (operation == "print")
		{
			cout << my_tree->getWebGraphvizPersistent() << "\n\n";
		}
		else if (operation == "search")
		{
			string team_name;
			cin >> team_name;
			cout << my_tree->treeSearch(Team(team_name))->key;
		}
		else if (operation == "exit")
		{
			return 0;
		}
		else
		{
			cout << "Wrong operation" << endl;
		}
	}
	return 0;
}