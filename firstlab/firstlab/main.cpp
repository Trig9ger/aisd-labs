#include <iostream>
#include <random>
#include <chrono>

using namespace ::std;

int Max(int Right, int Left) {
	if (Right >= Left) return Right;
	return Left;
};

class BalanceSearch {
private: 
	struct Node {
		int Value;
		int Height;
		Node* Left;
		Node* Right;
		Node(int Number) : Value(Number), Height(1), Left(nullptr), Right(nullptr) {};
	};


	Node* Tree;


	int GetHeight(Node* Root) {
		if (!Root) return 0;
		return Root->Height;
	};
	void UpdateHeight(Node* Root) {
		if (!Root) return;
		Root->Height = Max(GetHeight(Root->Left), GetHeight(Root->Right)) + 1;
	};
	int GetBalance(Node* Root) {
		if (!Root) return 0;
		return GetHeight(Root->Left) - GetHeight(Root->Right);
	};

	Node* SmallLeftRotation(Node* FirstNode) {
		Node* SecondNode = FirstNode->Right;

		FirstNode->Right = SecondNode->Left;
		SecondNode->Left = FirstNode;

		UpdateHeight(FirstNode);
		UpdateHeight(SecondNode);

		return SecondNode;
	}
	Node* SmallRightRotation(Node* FirstNode) {
		Node* SecondNode = FirstNode->Left;

		FirstNode->Left = SecondNode->Right;
		SecondNode->Right = FirstNode;

		UpdateHeight(FirstNode);
		UpdateHeight(SecondNode);

		return SecondNode;
	}	 

	Node* BigLeftRotation(Node* FirstNode) {
		Node* SecondNode = FirstNode->Right;
		Node* ThirdNode = FirstNode->Right->Left;

		SecondNode->Left = ThirdNode->Right;
		ThirdNode->Right = SecondNode;
		FirstNode->Right = ThirdNode->Left;
		ThirdNode->Left = FirstNode;

		UpdateHeight(FirstNode);
		UpdateHeight(SecondNode);
		UpdateHeight(ThirdNode);

		return ThirdNode;
	};
	Node* BigRightRotation(Node* FirstNode) {
		Node* SecondNode = FirstNode->Left;
		Node* ThirdNode = FirstNode->Left->Right;

		SecondNode->Right = ThirdNode->Left;
		ThirdNode->Left = SecondNode;
		FirstNode->Left = ThirdNode->Right;
		ThirdNode->Right = FirstNode;

		UpdateHeight(FirstNode);
		UpdateHeight(SecondNode);
		UpdateHeight(ThirdNode);

		return ThirdNode;
	};
	
	Node* UpdateBalance(Node* Root) {
		int Balance = GetBalance(Root);

		if (!Root) return Root;

		if (Balance == 2) {
			if (GetBalance(Root->Left) == -1) return BigRightRotation(Root);
			else return SmallRightRotation(Root);
		}
		else if (Balance == -2) {
			if (GetBalance(Root->Right) == 1) return BigLeftRotation(Root);
			else return SmallLeftRotation(Root);
		}
		else return Root;
	};

	void CopyNode(const Node* OtherNode, Node*& Root) {
		if (!OtherNode) {
			return;
		};

		Root = new Node(OtherNode->Value);
		Root->Height = OtherNode->Height;
		CopyNode(OtherNode->Left, Root->Left);
		CopyNode(OtherNode->Right, Root->Right);
	};
	
	void Clear(Node* Root) {
		if (!Root) return;
		
		Clear(Root->Left);
		Clear(Root->Right);

		delete Root;
	};

	void Print(Node* Root) {
		if (!Root) return;
		Print(Root->Left);
		cout << Root->Value << " ";
		Print(Root->Right);
	};

	Node* Insert(Node* Root, int Key) {
		if (!Root) return new Node(Key);
		else if (Key < Root->Value) Root->Left = Insert(Root->Left, Key);
		else Root->Right = Insert(Root->Right, Key);
		
		UpdateHeight(Root);

		return UpdateBalance(Root);
	};

	bool Contains(Node* Root, int Key) {
		if (!Root) return false;

		if (Root->Value == Key) return true;

		else if (Key < Root->Value) return Contains(Root->Left, Key);

		return Contains(Root->Right, Key);
	};

	Node* FindMin(Node* Root) {
		while (Root->Left != nullptr) {
			Root = Root->Left;
		}
		return Root;
	};

	Node* Erase(Node* Root, int Key) {

		if (Key == Root->Value) {
			Node* NewRoot = nullptr;
			if (Root->Height == 1) {
				delete Root;
				return nullptr;
			}
			else if (!Root->Right) {
				NewRoot = Root->Left;
				delete Root;
				return NewRoot;
			}
			else if (!Root->Left) {
				NewRoot = Root->Right;
				delete Root;
				return NewRoot;
			}
			else {
				Node* successor = FindMin(Root->Right);

				Root->Value = successor->Value;

				Root->Right = Erase(Root->Right, successor->Value);
				return Root;
			};
		}

		else if (Key < Root->Value) Root->Left = Erase(Root->Left, Key);

		else Root->Right = Erase(Root->Right, Key);

		if (Root != nullptr) {
			UpdateHeight(Root);
			Root = UpdateBalance(Root);
		}

		return Root;
	};

	bool StrictlyBalanced(Node* Root) {
		if (!Root) return true;

		if (StrictlyBalanced(Root->Left) && StrictlyBalanced(Root->Right)) {
			int Difference = GetHeight(Root->Left) - GetHeight(Root->Right);
			return Difference == 0 || Difference == 1 || Difference == -1;
		}
		else {
			return false;
		};
	};

public:
	BalanceSearch() : Tree(nullptr) {};

	BalanceSearch(const BalanceSearch& Other) {
		Node* Root;
		CopyNode(Other.Tree, Root);
		Tree = Root;
	};

	BalanceSearch(Node*& Other) {
		Node* Root;
		CopyNode(Other, Root);
		Tree = Root;
	};

	~BalanceSearch() {
		Clear(Tree);
	};

	BalanceSearch& operator=(const BalanceSearch& Other) {
		if (Other.Tree == Tree) {
			return *this;
		};

		Clear(Tree);

		Node* Root;
		CopyNode(Other.Tree, Root);
		Tree = Root;
		return *this;
	};

	void Print() {
		Print(Tree);
		cout << '\n';
	};

	bool Insert(int Key) {
		if (Contains(Tree, Key)) return false;

		Tree = Insert(Tree, Key);
		return true;
	};

	bool Contains(int Key) {

		return Contains(Tree, Key);
	};

	bool Erase(int Key) {
		if (!Contains(Tree, Key)) return false;

		Tree = Erase(Tree, Key);
		return true;
	};

	bool StrictlyBalanced() {
		return StrictlyBalanced(Tree);
	};
};

size_t lcg() {
	static size_t x = 0;
	x = (1021 * x + 24631) % 116640;
	return x;
};


BalanceSearch RandFill(size_t n) {
	BalanceSearch Tree;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<>  dist(1, n*10);

	for (int i = 0; i < n; ++i) {
		while (true) {
			int Number = dist(gen);
			if (Tree.Insert(Number)) {
				break;
			};
		};
	};

	return Tree;
};

void CheckTime() {
	int N = 1000;

	cout << "Filling the tree\n";

	//заполнение
	double AddTime = 0;
	
	while (true){
		for (int i = 0; i < 100; ++i) {
			auto Begin = chrono::high_resolution_clock::now();

			RandFill(N);

			auto End = chrono::high_resolution_clock::now();
			chrono::duration<double> seconds = End - Begin;
			AddTime += seconds.count();
		};
		AddTime /= 100;

		cout << AddTime << " with " << N << " elements." << '\n';

		AddTime = 0;

		if (N == 100000) {
			N = 1000;
			break;
		};
		N *= 10;
	}

	cout << "\nFind random element\n";

	//нахождение
	double FindTime = 0;

	while (true){
		uniform_int_distribution<>  dist(1, N*10);
		BalanceSearch Tree = RandFill(N);
		
		for(int i = 1; i < 1000; ++i) {
			auto Begin = chrono::high_resolution_clock::now();

			Tree.Contains(lcg());

			auto End = chrono::high_resolution_clock::now();
			chrono::duration<double> seconds = End - Begin;
			FindTime += seconds.count();
		};
		FindTime /= 1000;

		cout << FindTime << " with " << N << " elements.\n";

		FindTime = 0;

		if (N == 100000) {
			N = 1000;
			break;
		};
		N *= 10;
	}
	
	cout << "\nAdd and erase random element\n";

	//добавление и удаление//добавление и удаление
	double ElementTime = 0;

	while (true) {
		BalanceSearch Tree = RandFill(N);
		for (int i = 1; i < 1000; ++i) {
			int Number = lcg();

			auto Begin = chrono::high_resolution_clock::now();

			while (!Tree.Insert(Number)) {
				Number = lcg();
			};
			Tree.Erase(Number);

			auto End = chrono::high_resolution_clock::now();
			chrono::duration<double> seconds = End - Begin;
			ElementTime += seconds.count();
		};
		ElementTime /= 1000;

		cout << ElementTime << " with " << N << " elements.\n";

		ElementTime = 0;

		if (N == 100000) {
			break;
		};
		N *= 10;
	};
};

vector<int> RepeatedVector(vector<int> Arr) {
	BalanceSearch ExampleTree;
	BalanceSearch ResultTree;
	vector<int> ResultArr;

	for (size_t i = 0; i < Arr.size(); ++i) {
		if (!ExampleTree.Insert(Arr[i])) ResultTree.Insert(Arr[i]);
	};

	for (size_t i = 0; i < Arr.size(); ++i) {
		if (ResultTree.Contains(Arr[i])) {
			ResultArr.push_back(Arr[i]);

			ResultTree.Erase(Arr[i]);
		};
	};

	return ResultArr;
}

int main() {
	//CheckTime();



	//vector<int> Arr1 = { 5, 2, 5, 7, 1, 2 };

	//vector<int> Arr2 = RepeatedVector(Arr1);
};