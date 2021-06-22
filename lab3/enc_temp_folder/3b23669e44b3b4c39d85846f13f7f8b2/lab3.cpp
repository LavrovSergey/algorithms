#include <iostream>
#include <vector>

#define COUNT 20 // змінна вибору відступу при виводі дерева в консоль

using namespace std;

class Player // клас гравця
{
public:

	Player(const std::string& name)
	{
		this->name = name;
	}
	std::string get_name() const
	{
		return this->name;
	}
private:
	std::string name;
};

class Team // клас команди
{
public:
	Team() = default;
	Team(const std::string& name)
	{
		this->name = name;
	}
	void add_player(const std::string& name)
	{
		players.push_back(Player(name));
	}
	std::string get_name() const
	{
		return this->name;
	}
	friend std::ostream& operator<< (std::ostream& out, const Team& section);
	friend bool operator!=(const Team& first, const Team& second);
	friend bool operator==(const Team& first, const Team& second);
	friend bool operator<(const Team& first, const Team& second);
	friend bool operator>(const Team& first, const Team& second);
private:
	std::string name;
	std::vector<Player> players;
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

bool operator!=(const Team& first, const Team& second)
{
	return first.name != second.name;
}

bool operator==(const Team& first, const Team& second)
{
	return first.name == second.name;
}

bool operator<(const Team& first, const Team& second)
{
	return first.name < second.name;
}

bool operator>(const Team& first, const Team& second)
{
	return first.name > second.name;
}


template<typename T>
class Node
{
public:
	T element;
	Node* left;
	Node* right;
	Node* parent;
	Node()
	{
		this->element = T();
		this->left = nullptr;
		this->right = nullptr;
		this->parent = nullptr;
	}
	Node(T element)
	{
		this->element = element;
		this->left = nullptr;
		this->right = nullptr;
		this->parent = nullptr;
	}
	~Node() {}
};

template<typename T>
class SplayTree
{
private:
	size_t size; // розмір дерева
	Node<T>* root; // корінь дерева
	void rotateLeft(Node<T>*& theRoot, Node<T>* x); // поворот вліво
	void rotateRight(Node<T>*& theRoot, Node<T>* x); // поворот вправо
	void splay(Node<T>*& theRoot, Node<T>* theNode);  /*Полягає в переміщенні вершини в корінь за допомогою
														послідовного виконання трьох операцій: Zig, Zig-Zig і Zig-Zag.*/
	Node<T>* getMaxNode(Node<T>* theNode); // Функція для отримання максимальної вершини
	void freeMemory(Node<T>* theNode); // Функція для звільненя динамічної пам'яті
	void printUtil(Node<T>* root, int space); // Допоміжна функція для виводу дерева в консоль
public:
	SplayTree();
	~SplayTree();

	void insert(T element); // Вставка елемента
	void erase(T element); // Видалення елемента
	T* search(T element); //Функція пошуку елемента
	void print(); //Функція для виводу дерева в консоль
};

template<typename T>
void SplayTree<T>::rotateLeft(Node<T>*& theRoot, Node<T>* x)
{
	Node<T>* y = x->right;
	x->right = y->left;
	if (y->left != nullptr)
		y->left->parent = x;

	y->parent = x->parent;
	if (x->parent == nullptr) //x корінь
		theRoot = y;
	else if (x == x->parent->left) //x лівий син
		x->parent->left = y;
	else // х правий син
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}

template<typename T>
void SplayTree<T>::rotateRight(Node<T>*& theRoot, Node<T>* x)
{
	Node<T>* y = x->left;
	x->left = y->right;
	if (y->right != nullptr)
		y->right->parent = x;

	y->parent = x->parent;
	if (x->parent == nullptr) //x корінь
		theRoot = y;
	else if (x == x->parent->right) //x лівий син
		x->parent->right = y;
	else //x правий син
		x->parent->left = y;

	y->right = x;
	x->parent = y;
}

template<typename T>
void SplayTree<T>::splay(Node<T>*& theRoot, Node<T>* theNode)
{
	while (theNode->parent != nullptr)
	{
		//Zig
		if (theNode->parent == theRoot)
		{
			if (theNode == theNode->parent->right)
				rotateLeft(theRoot, theNode->parent);
			else if (theNode == theNode->parent->left)
				rotateRight(theRoot, theNode->parent);
		}
		else
		{
			Node<T>* theParent = theNode->parent;
			Node<T>* theGrandparent = theParent->parent;
			//Zig-zag
			if (theNode->parent->left == theNode && theParent->parent->right == theParent)
			{
				rotateRight(theRoot, theParent);
				rotateLeft(theRoot, theGrandparent);
			}
			else if (theNode->parent->right == theNode && theParent->parent->left == theParent)
			{
				rotateLeft(theRoot, theParent);
				rotateRight(theRoot, theGrandparent);
			}
			//Zig-zig
			else if (theNode->parent->right == theNode && theParent->parent->right == theParent)
			{
				rotateLeft(theRoot, theGrandparent);
				rotateLeft(theRoot, theParent);
			}
			else if (theNode->parent->left == theNode && theParent->parent->left == theParent)
			{
				rotateRight(theRoot, theGrandparent);
				rotateRight(theRoot, theParent);
			}
		}
	}
}

template<typename T>
Node<T>* SplayTree<T>::getMaxNode(Node<T>* theNode)
{
	Node<T>* walker = theNode;
	if (walker != nullptr)
	{
		while (walker->right != nullptr)
			walker = walker->right;
	}
	return walker;
}

template<typename T>
inline void SplayTree<T>::freeMemory(Node<T>* theNode)
{
	if (theNode->left != nullptr)
		freeMemory(theNode->left);
	if (theNode->right != nullptr)
		freeMemory(theNode->right);
	delete theNode;
}

template<typename T>
SplayTree<T>::SplayTree()
{
	this->root = nullptr;
	this->size = 0;
}

template<typename T>
SplayTree<T>::~SplayTree()
{
	if (this->root != nullptr)
		freeMemory(this->root);
}

template<typename T>
void SplayTree<T>::insert(T element)
{
	Node<T>* newNode = new Node<T>(element);
	Node<T>* walker = this->root;
	Node<T>* parent = nullptr;
	while (walker != nullptr)
	{
		parent = walker;
		if (newNode->element == walker->element)
			walker = nullptr;
		else if (newNode->element > walker->element)
			walker = walker->right;
		else if (newNode->element < walker->element)
			walker = walker->left;
	}
	newNode->parent = parent;
	if (parent == nullptr)
	{
		this->root = newNode;
		this->size++;
	}
	else if (newNode->element != parent->element)
	{
		if (newNode->element > parent->element)
			parent->right = newNode;
		else if (newNode->element < parent->element)
			parent->left = newNode;
		this->size++;
		splay(this->root, newNode);
	}
}

template<typename T>
void SplayTree<T>::erase(T element)
{
	Node<T>* walker = this->root;
	while (walker != nullptr && walker->element != element)
	{
		if (walker->element < element)
			walker = walker->right;
		else if (walker->element > element)
			walker = walker->left;
	}
	if (walker != nullptr)
	{
		splay(this->root, walker);
		Node<T>* leftSubTree = this->root->left;
		if (leftSubTree != nullptr)
			leftSubTree->parent = nullptr;
		Node<T>* rightSubTree = this->root->right;
		if (rightSubTree != nullptr)
			rightSubTree->parent = nullptr;

		delete walker;
		size--;
		if (rightSubTree == nullptr)
		{
			this->root = leftSubTree;
		}
		else if (leftSubTree != nullptr)
		{
			Node<T>* maxOfLeft = getMaxNode(leftSubTree);
			splay(leftSubTree, maxOfLeft);
			maxOfLeft->right = rightSubTree;
			rightSubTree->parent = leftSubTree;
			this->root = leftSubTree;
		}
		else if (leftSubTree == nullptr)
		{
			this->root = rightSubTree;
		}
	}
}
template<typename T>
T* SplayTree<T>::search(T element)
{
	T* result = nullptr;
	Node<T>* walker = this->root;
	while (walker != nullptr && walker->element != element)
	{
		if (walker->element == element)
			result = &walker->element;
		else if (walker->element < element)
			walker = walker->right;
		else if (walker->element > element)
			walker = walker->left;
		if (walker != nullptr && walker->element == element)
			result = &walker->element;
	}
	if (walker != nullptr)
		splay(this->root, walker);
	return result;
}

template<typename T>
void SplayTree<T>::printUtil(Node<T>* root, int space)
{
	if (root == NULL)
		return;
	space += COUNT;
	printUtil(root->right, space);
	cout << endl;
	for (int i = COUNT; i < space; i++)
		cout << " ";
	cout << root->element << "\n";
	printUtil(root->left, space);
}

template<typename T>
void SplayTree<T>::print()
{
	printUtil(root, 0);
}


int main()
{
	SplayTree<Team> splay_tree;

	string operation;

	while (true)
	{
		cin >> operation;
		if (operation == "addsection")
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
			splay_tree.insert(new_team);
		}
		else if (operation == "insertgoods")
		{
			string team_name;
			cin >> team_name;
			Team temp(team_name);
			Team* finded = splay_tree.search(temp);
			int count;
			cin >> count;
			string players_name;
			if (finded)
			{
				for (int i = 0; i < count; i++)
				{
					cin >> players_name;
					finded->add_player(players_name);
				}
			}
		}
		else if (operation == "search")
		{
			string team_name;
			cin >> team_name;
			Team temp(team_name);
			cout << *splay_tree.search(temp) << endl;
		}
		else if (operation == "erase")
		{
			string team_name;
			cin >> team_name;
			Team temp(team_name);
			splay_tree.erase(temp);
		}
		else if (operation == "print")
		{
			splay_tree.print();
			cout << endl;
		}
		else if (operation == "exit")
		{
			return 0;
		}
		else
		{
			cout << "Wrong operation!\n";
		}
	}
}

