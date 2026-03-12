#include <iostream>

using namespace ::std;

struct Node {
	int Value;
	int Height;
	Node* Left;
	Node* Right;
	Node(int Number) : Value(Number), Height(1), Left(nullptr), Right(nullptr) {};
};

int Max(int Right, int Left) {
	if (Right >= Left) return Right;
	return Left;
};

class BalanceSearch {
private: 
	Node* Tree;

	
	void DoHeight(Node* Root) {
		int Height = 0;
		if (!Root->Right and !Root->Left){
			Height = 0;
		}
		else if (!Root->Left) {
			Height = Root->Right->Height;
		}
		else if (!Root->Right) {
			Height = Root->Left->Height;
		}
		else {
			Height = Max(Root->Left->Height, Root->Right->Height);
		};
		Root->Height = Height;
	}

	void ChangeAfter(Node* Root, int TargetValue) {
		if (TargetValue < Root->Value) {
			ChangeAfter(Root->Left, TargetValue);
		}
		else {
			ChangeAfter(Root->Right, TargetValue);
		};
		DoHeight(Root);
	};
	void ChangeAfterRotation(Node* Root, int TargetValue, Node* ChangeNode) {
		if (Root->Value == TargetValue) Tree = ChangeNode; {}

		if (TargetValue < Root->Value) {
			if (TargetValue == Root->Left->Value) {
				Root->Left = ChangeNode;
			}
			else {
				ChangeAfterRotation(Root->Left, TargetValue, ChangeNode);
			};
		}
		else {
			if (TargetValue == Root->Right->Value) {
				Root->Right = ChangeNode;
			}
			else {
				ChangeAfterRotation(Root->Right, TargetValue, ChangeNode);
			};
		};

		DoHeight(Root);
	};

	void SmallLeftRotation(Node* FirstNode) {
		Node* SecondNode = FirstNode->Right;
		FirstNode->Right = SecondNode->Left;
		SecondNode->Left = FirstNode;

		DoHeight(FirstNode);
		DoHeight(SecondNode);

		ChangeAfterRotation(Tree, FirstNode->Value, SecondNode);
	}
	void SmallRightRotation(Node* FirstNode) {
		Node* SecondNode = FirstNode->Left;
		FirstNode->Left = SecondNode->Right;
		SecondNode->Right = FirstNode;

		DoHeight(FirstNode);
		DoHeight(SecondNode);

		ChangeAfterRotation(Tree, FirstNode->Value, SecondNode);
	}	 

	void BigLeftRotation(Node* CoreNode) {
		SmallRightRotation(CoreNode->Right);
		SmallLeftRotation(CoreNode);
	};
	void BigRightRotation(Node* CoreNode) {
		SmallLeftRotation(CoreNode->Left);
		SmallRightRotation(CoreNode);
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

		delete[] Root;
	};

	void Print(Node* Root) {
		if (!Root) return;
		Print(Root->Left);
		cout << Root->Value << "  ";
		Print(Root->Right);
	};

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	bool Insert(Node* Root, int Key) {
		if (!Root) {
			Tree = new Node(Key);
			return true;
		};
		if (Root->Value == Key) {
			return false;
		}

		//left
		else if (Key < Root->Value) {
			if (!Root->Left) {
				Root->Left = new Node(Key);
				ChangeAfter(Tree, Root->Value);
				return true;
			};

			Node* NewRoot = Root->Left;

			if (Root->Left->Height - Root->Right->Height == 1) {
				if (Key < Root->Left->Value) {
					SmallRightRotation(Root);
				}
				else if (Key > Root->Left->Value) {
					if (!Root->Left->Right) {
						Root->Left->Right = new Node(Key);
						ChangeAfter(Tree, Root->Left->Value);
						BigRightRotation(Root);
						return true;
					};
					NewRoot = NewRoot->Right;
					BigRightRotation(Root);
				};
			};

			return Insert(NewRoot, Key);
		}

		//right
		else {
			if (!Root->Right) {
				Root->Right = new Node(Key);
				ChangeAfter(Tree, Root->Value);
				return true;
			};

			Node* NewRoot = Root->Right;

			if (Root->Left->Height - Root->Right->Height == -1) {
				if (Key > Root->Right->Value) {
					SmallLeftRotation(Root);
				}
				else if (Key < Root->Right->Value) {
					if (!Root->Right->Left) {
						Root->Right->Left = new Node(Key);
						ChangeAfter(Tree, Root->Right->Value);
						BigLeftRotation(Root);
						return true;
					};
					NewRoot = NewRoot->Left;
					BigLeftRotation(Root);
				};
			};

			return Insert(NewRoot, Key);
		};
	};

	bool Contains(Node* Root, int Key) {
		if (!Root) {
			return false;
		};

		if (Root->Value == Key) {
			return true;
		}
		else if (Key < Root->Value) {
			return Contains(Root->Left, Key);
		};
		return Contains(Root->Right, Key);
	};

	void FoundErase(Node* Root) {
		if (!Root->Left) {
			SmallLeftRotation(Root);
			delete[] Root;
		}
		else if (!Root->Right) {
			SmallRightRotation(Root);
			delete[] Root;
		}
		else if (Root->Height == 1) {
			delete[] Root;
		}
		else {
			int Difference = Root->Left->Height - Root->Right->Height;
			if (Difference == 1) {
				int LeftDifference = Root->Left->Left->Height - Root->Left->Right->Height;
				if (LeftDifference == -1) {
					BigRightRotation(Root);
				}
				else {
					SmallRightRotation(Root);
				};
			}
			else {
				int RightDifference = Root->Right->Left->Height - Root->Right->Right->Height;
				if (RightDifference == 1) {
					BigLeftRotation(Root);
				}
				else {
					SmallLeftRotation(Root);
				};
			};
		};
	};

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	bool Erase(Node* Root, int Key) {
		if (Root->Value == Key) {
			FoundErase(Root);
		};

		if (Key > Root->Value) {
			if (Root->Left->Height - Root->Right->Height = 1) {
				SmallRightRotation(Root);
				return Erase(Root);
			};
			return Erase(Root->Right);
		};

		if (Root->Left->Height - Root->Right->Height = -1) {
			SmallRightRotation(Root);
			return Erase(Root);
		};
		return Erase(Root->Left);
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
		return Insert(Tree, Key);
	};

	bool Contains(int Key) {
		return Contains(Tree, Key);
	};

	bool Erase(int Key) {
		if (!Containse(Key)) return false;

		Erase(Tree, Key);
	}
};

int main() {
	Node* Root = new Node(1);
	Root->Right = new Node(2);
	Root->Left = new Node(2);

	Node* RootOne = Root->Right;
	RootOne->Right = new Node(3);
	RootOne->Left = new Node(3);

	Node* RootTwo = RootOne->Right;
	RootTwo->Right = new Node(4);
	RootTwo->Left = new Node(4);
};