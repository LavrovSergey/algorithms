#include <iostream>
#include <vector>

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


template<typename T>
class Node { // клас вузла дерева
    Node<T>* rightChild = nullptr; // Вказівник на правого сина
    Node<T>* leftChild = nullptr; // Вказівник на лівого сина
    Node<T>* parent = nullptr; // Вказівник на батька
    T data; // Ключ
    bool color; // Колір вузла
    int size; // Розмір
public:
    Node() :data(), color(false) {
    }
    Node(T d) {
        data = d;
        color = true;
        size = 1;
    }
    T getData() { // Отримати ключ вузла
        return data;
    }
    bool isRed() { // Чи є вузол червоний
        return color;
    }
    void setData(T d) { // Змінити ключ у вузлі
        data = d;
    }
    void toBlack() { // Фарбування вузла в чорний
        color = false;
    }
    void toRed() { // Фарбування вузла в червоний
        color = true;
    }
    //Набір функцій для отримання вказівників на потрібні вузли
    Node* getLeftChild() {
        return leftChild;
    }
    Node* getRightChild() {
        return rightChild;
    }
    Node* getBrother() {
        return parent->getLeftChild() == this ? parent->getRightChild() : parent->getLeftChild();
    }
    Node* getParent() {
        return parent;
    }
    Node* getUncle() {
        Node* grandparent = getGrandparent();
        return parent->isLeftChild() ? grandparent->getRightChild() : grandparent->getLeftChild();
    }
    Node* getGrandparent() {
        Node* parent = getParent();
        return parent->getParent();
    }
    bool isLeftChild() {
        return bool(data <= parent->getData());
    }
    void setLeftChild(Node* lChild) {
        leftChild = lChild;
    }
    void setRightChild(Node* rChild) {
        rightChild = rChild;
    }
    void setParent(Node* par) {
        parent = par;
    }
    void setSize(int s) {
        size = s;
    }
    int getSize() {
        return size;
    }
};

template<typename T>
class RedBlackTree { // клас дерева порядкової статистики на основі червоно-чорного дерева

    Node<T>* root = nullptr; // Корінь

    int leaves = 0; // Кількість листків

    void rotateLeft(Node<T>* rotateNode) { // Поворот вліво
        Node<T>* parent = rotateNode->getParent();
        Node<T>* rChild = rotateNode->getRightChild();
        if (parent) {
            if (rotateNode->isLeftChild())
                parent->setLeftChild(rChild);
            else parent->setRightChild(rChild);
        }
        else root = rChild;
        rotateNode->setRightChild(rChild->getLeftChild());
        if (rChild->getLeftChild()) {
            Node<T>* child = rChild->getLeftChild();
            child->setParent(rotateNode);
        }
        rotateNode->setParent(rChild);
        rChild->setLeftChild(rotateNode);
        rChild->setParent(parent);
    }

    void rotateRight(Node<T>* rotateNode) { // Поворот вправо
        Node<T>* parent = rotateNode->getParent();
        Node<T>* lChild = rotateNode->getLeftChild();
        rotateNode->setLeftChild(lChild->getRightChild());
        if (lChild->getRightChild()) {
            Node<T>* child = lChild->getRightChild();
            child->setParent(rotateNode);
        }
        rotateNode->setParent(lChild);
        lChild->setRightChild(rotateNode);
        lChild->setParent(parent);
        if (parent) {
            if (lChild->isLeftChild())
                parent->setLeftChild(lChild);
            else parent->setRightChild(lChild);
        }
        else root = lChild;
    }

    //Вставки

    void insertCase1(Node<T>* insertedNode) {
        Node<T>* parent = insertedNode->getParent();
        if (parent->isRed()) {
            insertCase2(insertedNode);
        }
    }

    void insertCase2(Node<T>* insertedNode) {
        Node<T>* uncle = insertedNode->getUncle();
        Node<T>* parent = insertedNode->getParent();
        if (uncle && uncle->isRed() && parent->isRed()) {
            Node<T>* grandparent = parent->getParent();
            uncle->toBlack();
            parent->toBlack();
            grandparent->toRed();
            if (grandparent->getParent()) {
                insertCase1(grandparent);
            }
            else grandparent->toBlack();
        }
        else insertCase3(insertedNode);
    }

    void insertCase3(Node<T>* insertedNode) {
        Node<T>* parent = insertedNode->getParent();
        if (parent->isLeftChild() && !(insertedNode->isLeftChild())) {
            rotateLeft(parent);
            insertedNode = parent;
        }
        else if (!(parent->isLeftChild()) && insertedNode->isLeftChild()) {
            rotateRight(parent);
            insertedNode = parent;
        }
        insertCase4(insertedNode);
    }

    void insertCase4(Node<T>* insertedNode) {
        Node<T>* grandparent = insertedNode->getGrandparent();
        Node<T>* parent = insertedNode->getParent();
        grandparent->toRed();
        parent->toBlack();
        if (insertedNode->isLeftChild() && parent->isLeftChild()) {
            rotateRight(grandparent);
        }
        else {
            rotateLeft(grandparent);
        }
    }

    //Видалення

    void deleteCase1(Node<T>* deletedNode) {
        if (!deletedNode->getParent()) {
            return;
        }
        Node<T>* brother = deletedNode->getBrother();
        if (brother->isRed()) {
            Node<T>* parent = deletedNode->getParent();
            parent->toRed();
            brother->toBlack();
            if (deletedNode->isLeftChild()) {
                rotateLeft(parent);
            }
            else {
                rotateRight(parent);
            }
        }
        deleteCase2(deletedNode);
    }

    void deleteCase2(Node<T>* deletedNode) {
        Node<T>* parent = deletedNode->getParent();
        Node<T>* brother = deletedNode->getBrother();
        Node<T>* left = brother->getLeftChild();
        Node<T>* right = brother->getRightChild();
        bool lChild = false, rChild = false;

        if (left) {
            lChild = left->isRed();
        }
        if (right) {
            rChild = right->isRed();
        }
        if (!parent->isRed() && !brother->isRed() && !lChild && !rChild) {
            brother->toRed();
            deleteCase1(parent);
        }
        else if (parent->isRed() && !brother->isRed() && !lChild && !rChild) {
            brother->toRed();
            parent->toBlack();
        }
        else {
            if (deletedNode->isLeftChild() && !rChild && lChild) {
                brother->toRed();
                left->toBlack();
                rotateRight(brother);
            }
            else if (!deletedNode->isLeftChild() && !lChild && rChild) {
                brother->toRed();
                right->toBlack();
                rotateLeft(brother);
            }
            if (parent->isRed())
                brother->toRed();
            else
                brother->toBlack();
            parent->toBlack();

            if (deletedNode->isLeftChild()) {
                right->toBlack();
                rotateLeft(parent);
            }
            else {
                left->toBlack();
                rotateRight(parent);
            }
        }
    }

    Node<T>* findMin(Node<T>* ptrToNode) { // Пошук вузла з мінімальним ключем
        while (ptrToNode->getLeftChild() != nullptr)
            ptrToNode = ptrToNode->getLeftChild();
        return ptrToNode;
    }

    Node<T>* findMax(Node<T>* ptrToNode) { // Пошук вузла з максимальним ключем
        while (ptrToNode->getRightChild() != nullptr)
            ptrToNode = ptrToNode->getRightChild();
        leaves -= 1;
        return ptrToNode;
    }

    int getHeight(Node<T>* startNode) { // Отримати висоту
        if (!startNode)
            return 0;
        int heightLeft = getHeight(startNode->getLeftChild()), heightRight = getHeight(startNode->getRightChild());
        return (heightLeft > heightRight ? heightLeft : heightRight) + 1;
    }
    void showRedBlackTree(Node<T>* startNode, int level = 0) { // Рекурсивний обхід для виводу дерева
        std::cout << startNode->getData();
        if (startNode->isRed()) std::cout << "red\n";
        else std::cout << "black\n";
        std::cout << "size: " << startNode->getSize() << std::endl;
        std::cout << "level: " << level << std::endl << std::endl;

        level++;
        if (startNode->getLeftChild())
            showRedBlackTree(startNode->getLeftChild(), level);
        if (startNode->getRightChild())
            showRedBlackTree(startNode->getRightChild(), level);
    }
public:
    RedBlackTree() {
    }
    RedBlackTree(T data) {
        root = new Node<T>(data);
        root->toBlack();
    }
    void deleteRedBlackTree(Node<T>* nodeToDelete) { // Видалення всіх вузлів дерева
        if (nodeToDelete) {
            deleteRedBlackTree(nodeToDelete->getLeftChild());
            deleteRedBlackTree(nodeToDelete->getRightChild());
            delete nodeToDelete;
        }
    }
    ~RedBlackTree() {
        deleteRedBlackTree(root);
    }

    void setAllSize(Node<T>* cur) { // Встановлення розміру
        if (!cur->getLeftChild() && !cur->getRightChild()) {
            cur->setSize(1);
            return;
        }

        int s = 1;
        if (cur->getLeftChild()) {
            setAllSize(cur->getLeftChild());
            s += cur->getLeftChild()->getSize();
        }
        if (cur->getRightChild()) {
            setAllSize(cur->getRightChild());
            s += cur->getRightChild()->getSize();
        }

        cur->setSize(s);
    }

    Node<T>* search(T dataToSearch) { // Пошук вузла в дереві за ключем
        Node<T>* ptrToNode = root;
        while (ptrToNode != nullptr) {
            if (dataToSearch > ptrToNode->getData())
                ptrToNode = ptrToNode->getRightChild();
            else if (dataToSearch < ptrToNode->getData())
                ptrToNode = ptrToNode->getLeftChild();
            else
            {
                if (ptrToNode != nullptr)
                    std::cout << ptrToNode->getSize() << "  " << ptrToNode->getData();
                return ptrToNode;
            }
        }
        if (ptrToNode != nullptr)
            std::cout << ptrToNode->getSize() << "  " << ptrToNode->getData();
        return ptrToNode;
    }

    void insert(T dataToInsert) { // Вставка вузла в дерево
        if (search(dataToInsert)) {
            return;
        }

        if (root == nullptr) {
            root = new Node<T>(dataToInsert);
            root->toBlack();
            leaves++;
            return;
        }

        Node<T>* NodeToInsert = new Node<T>(dataToInsert);
        Node<T>* ptrToNode = root;
        while (ptrToNode != nullptr) {
            NodeToInsert->setParent(ptrToNode);
            if (NodeToInsert->getData() > ptrToNode->getData()) {
                ptrToNode = ptrToNode->getRightChild();
            }
            else {
                ptrToNode = ptrToNode->getLeftChild();
            }
        }
        Node<T>* parent = NodeToInsert->getParent();
        if (NodeToInsert->getData() < parent->getData())
            parent->setLeftChild(NodeToInsert);
        else
            parent->setRightChild(NodeToInsert);
        insertCase1(NodeToInsert);

        setAllSize(root);
        leaves++;
    }

    void erase(T dataToDelete) { // Видалення вузла за ключем
        Node<T>* ptrToDelete = search(dataToDelete);
        if (!ptrToDelete) {
            return;
        }
        if (ptrToDelete->getLeftChild() && ptrToDelete->getRightChild()) {
            Node<T>* predecessor = findMax(ptrToDelete->getLeftChild());
            ptrToDelete = predecessor;
        }
        Node<T>* child = ptrToDelete->getLeftChild() ? ptrToDelete->getLeftChild() : ptrToDelete->getRightChild();
        Node<T>* parent = ptrToDelete->getParent();
        if (parent == nullptr) {
            root = child;
            if (child) {
                child->toBlack();
                child->setParent(nullptr);
            }
        }
        else if (ptrToDelete->isRed()) {
            if (ptrToDelete->isLeftChild())
                parent->setLeftChild(nullptr);
            else
                parent->setRightChild(nullptr);
        }
        else if (child) {
            child->toBlack();
            if (ptrToDelete->isLeftChild())
                parent->setLeftChild(child);
            else
                parent->setRightChild(child);
            child->setParent(parent);
        }
        else {
            deleteCase1(ptrToDelete);
            if (ptrToDelete->isLeftChild())
                parent->setLeftChild(child);
            else
                parent->setRightChild(child);
            if (child) child->setParent(parent);
        }
        delete ptrToDelete;

        setAllSize(root);
        leaves -= 1;
    }

    int getHeight() { // Отримати висоту
        return getHeight(root);
    }

    void print() { // Вивід дерева в консоль
        showRedBlackTree(root);
    }
};

int main()
{
    string operation;

    RedBlackTree<Team> my_tree;
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
            my_tree.insert(new_team);
        }
        else if (operation == "insertplayers")
        {
            string team_name;
            cin >> team_name;
            Team temp(team_name);
            Team finded = my_tree.search(temp)->getData();
            int count;
            cin >> count;
            string players_name;
            for (int i = 0; i < count; i++)
            {
                cin >> players_name;
                finded.add_player(players_name);
            }
        }
        else if (operation == "search")
        {
            string team_name;
            cin >> team_name;
            Team temp(team_name);
            cout << my_tree.search(temp);
        }
        else if (operation == "erase")
        {
            string team_name;
            cin >> team_name;
            Team temp(team_name);
            my_tree.erase(temp);
        }
        else if (operation == "print")
        {
            my_tree.print();
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
    return 0;
}