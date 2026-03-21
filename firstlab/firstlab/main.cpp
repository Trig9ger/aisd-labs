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

	Node* FoundErase(Node* Root) {
		Node* NewRoot;
		if (Root->Height == 1) {
			delete Root;
			return nullptr;
		}
		else if (!Root->Right) {
			NewRoot = SmallRightRotation(Root);
			delete Root;
			return NewRoot;
		}
		else if (!Root->Left) {
			NewRoot = SmallLeftRotation(Root);
			delete Root;
			return NewRoot;
		}
		else {
			int Balance = GetBalance(Root);

			if (Balance == 1) {
				if (GetBalance(Root->Left) == -1) NewRoot = BigRightRotation(Root);
				else NewRoot = SmallRightRotation(Root);
				
				NewRoot->Right = FoundErase(Root);
			}
			else if (Balance == -1) {
				if (GetBalance(Root->Right) == 1) NewRoot = BigLeftRotation(Root);
				else NewRoot = SmallLeftRotation(Root);

				NewRoot->Left = FoundErase(Root);
			}
			else {
				NewRoot = SmallLeftRotation(Root);
				
				NewRoot->Left = FoundErase(Root);
			};
		};

		UpdateHeight(NewRoot);
		return UpdateBalance(NewRoot);
	};

	Node* Erase(Node* Root, int Key) {

		if (Key == Root->Value) Root = FoundErase(Root);

		else if (Key < Root->Value) Root->Left = Erase(Root->Left, Key);

		else Root->Right = Erase(Root->Right, Key);

		UpdateHeight(Root);
		return UpdateBalance(Root);
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

void RandFill(size_t n) {
	BalanceSearch Tree;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<>  dist(1, n);

	for (int i = 0; i < n; ++i) {
		while (true) {
			int Number = dist(gen);
			if (Tree.Insert(Number)) {
				break;
			};
		};
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
	//vector<int> Arr1 = { 5, 2, 5, 7, 1, 2 };

	auto begin = chrono::high_resolution_clock::now();

	RandFill(100000);

	auto end = chrono::high_resolution_clock::now();

	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - begin);

	cout << microseconds.count() << '\n';
	cout << milliseconds.count() << '\n';
	cout << seconds.count() << '\n';


	//vector<int> Arr2 = RepeatedVector(Arr1);
};