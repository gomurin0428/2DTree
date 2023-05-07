#pragma once

#include <vector>
#include <set>
#include <stack>

// 2DTreeのクラスを作る

namespace KDT {
	class C2DTree
	{
	public:
		C2DTree() {};
		~C2DTree() {

			root.reset();
			points.clear();
			pointsSet.clear();

		};

		// 2DTreeにデータを追加する
		void AddData(int x, int y);

		// 2DTreeを構築する
		void Build();

		// 指定した座標に最も近いデータを返す
		std::pair<int, int> FindNearest(int x, int y);

		void clear();

	private:


		class Node {
		public:

			Node() {
				cord.first = INT_MIN;
				cord.second = INT_MIN;
				axis = -1;
				childIndex = -1;
				parent = nullptr;
				leftmost = INT_MIN;
				rightmost = INT_MAX;
				upmost = INT_MAX;
				downmost = INT_MIN;

			}
			~Node() {
				parent.reset();
				leftChild.reset();
				rightChild.reset();
			}
			// このノードが持つデータの座標(葉ノードでない場合は、片方だけがINT_MINでない。
			// INT_MINでない方がx軸またはy軸との交点)
			std::pair<int, int> cord;
			// このノードの親ノード
			std::shared_ptr<Node> parent;
			// このノードの子ノード
			std::shared_ptr<Node> leftChild;
			std::shared_ptr<Node> rightChild;
			// このノードの分割軸(0:x軸, 1:y軸)
			int axis;

			int leftmost;
			int rightmost;
			int upmost;
			int downmost;

			int childIndex;
		};

		std::shared_ptr<Node> BuildSub(std::shared_ptr<Node> parent, std::vector<std::pair<int, int>> childPoints, int childIndex);

		// 2DTreeの根
		std::shared_ptr<Node> root = nullptr;

		// データを保持するvector
		std::vector<std::pair<int, int>> points;

		// データを保持するset
		std::set<std::pair<int, int>> pointsSet;

		Node recursive(Node node);

		std::pair<int, std::pair<int, int>> getChildrenMinDistance(Node& node, int x, int y, int minDistance);

		void clearSub(std::shared_ptr<Node> node);

	};
}
