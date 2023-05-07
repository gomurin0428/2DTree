// 2DTree.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <algorithm>
#include "C2DTree.h"
#include <string>
#include <cassert>
#include <windows.h>


int main() {



	double time1 = 0.0;
	double time2 = 0.0;
	double minTime1 = DBL_MAX;
	double minTime2 = DBL_MAX;

	int N1 = 100;
	int N2 = 15000;

	std::vector<std::pair<int, int>> points2(N2);

	for (int j = 0; j < N1; j++) {

		for (int i = 0; i < N2; i++) {
			points2[i].first = rand() % 2000;
			points2[i].second = rand() % 2000;
		}

		// 2DTreeを作成
		KDT::C2DTree tree = KDT::C2DTree();

		for (int i = 0; i < N2; i++) {
			tree.AddData(points2[i].first, points2[i].second);
		}

		tree.Build();



		// QueryPerformanceCounter関数の1秒当たりのカウント数を取得する
		LARGE_INTEGER freq1, freq2;
		QueryPerformanceFrequency(&freq1);

		LARGE_INTEGER start1, end1;
		LARGE_INTEGER start2, end2;

		QueryPerformanceCounter(&start1);
		for (int k = 0; k < N2; k++) {
			std::pair<int, int> result = tree.FindNearest(points2[k].first, points2[k].second);
		}
		QueryPerformanceCounter(&end1);
		time1 += static_cast<double>(end1.QuadPart - start1.QuadPart) * 1000.0 / freq1.QuadPart;
		minTime1 = min(minTime1, static_cast<double>(end1.QuadPart - start1.QuadPart) * 1000.0 / freq1.QuadPart);


		int min = INT_MAX;
		std::pair<int, int> result2;

		QueryPerformanceFrequency(&freq2);
		QueryPerformanceCounter(&start2);
		for (int k = 0; k < N2; k++) {
			for (int i = k+1; i < N2; i++) {
				int first, second;
				first = points2[k].first;
				second = points2[k].second;

				int distance = (first - points2[i].first) * (first - points2[i].first) + (second - points2[i].second) * (second - points2[i].second);
				if (distance < min) {
					min = distance;
					result2.first = points2[i].first;
					result2.second = points2[i].second;
				}
			}
		}

		QueryPerformanceCounter(&end2);
		time2 += static_cast<double>(end2.QuadPart - start2.QuadPart) * 1000.0 / freq2.QuadPart;
		minTime2 = min(minTime2, static_cast<double>(end2.QuadPart - start2.QuadPart) * 1000.0 / freq2.QuadPart);

		std::cout << result2.first << min << std::endl;

		for (int k = 0; k < 100; k++) {
			int first, second;
			first = rand() % 2000;
			second = rand() % 2000;
			std::pair<int, int> result = tree.FindNearest(first,second);

			min = INT_MAX;
			for (int i = 0; i < N2; i++) {

				int distance = (first - points2[i].first) * (first - points2[i].first) + (second - points2[i].second) * (second - points2[i].second);
				if (distance == 0) {
					distance = INT_MAX;
				}
				if (distance < min) {
					min = distance;
					result2.first = points2[i].first;
					result2.second = points2[i].second;
				}
			}
			int distance1 = (result.first - first) * (result.first - first) + (result.second - second) * (result.second - second);
			int distance2 = (result2.first - first) * (result2.first - first) + (result2.second - second) * (result2.second - second);
			if (distance1 != distance2) {
				std::cout << "error" << std::endl;
			}
		}

		std::cout << result2.first << std::endl;
		/*
		int distance1 = (result.first - first) * (result.first - first) + (result.second - second) * (result.second - second);
		int distance2 = (result2.first - first) * (result2.first - first) + (result2.second - second) * (result2.second - second);

		if (distance1 != distance2) {
			std::cout << "error" << std::endl;
			return 0;
		}
		else {
			std::cout << "good" << std::endl;
		}
		*/


		tree.clear();

	}

	std::cout << time1 << std::endl;
	std::cout << time2 << std::endl;
	std::cout << minTime1 << std::endl;
	std::cout << minTime2 << std::endl;





	return 0;
}

namespace KDT {
	void C2DTree::AddData(int x, int y) {
		// 重複チェック
		if (pointsSet.count(std::pair<int, int>(x, y)) == 0) {
			points.push_back(std::pair<int, int>(x, y));
			pointsSet.insert(std::pair<int, int>(x, y));
		}
	}

	void C2DTree::Build() {


		// 2DTreeを構築
		root = BuildSub(nullptr, points, -1);

		//recursive(*root);

		return;


	}



	std::shared_ptr<C2DTree::Node> C2DTree::BuildSub(std::shared_ptr<Node> parent, std::vector<std::pair<int, int>> childPoints, int childIndex) {

		if (childPoints.size() == 1) {

			Node node = Node();
			node.parent = parent;
			node.cord.first = childPoints[0].first;
			node.cord.second = childPoints[0].second;
			node.childIndex = childIndex;
			node.leftChild = nullptr;
			node.rightChild = nullptr;

			return std::make_shared<Node>(node);
		}

		int maxX = INT_MIN;
		int minX = INT_MAX;
		int maxY = INT_MIN;
		int minY = INT_MAX;
		for (int i = 0; i < childPoints.size(); i++) {
			if (childPoints[i].first > maxX) {
				maxX = childPoints[i].first;
			}
			if (childPoints[i].first < minX) {
				minX = childPoints[i].first;
			}
			if (childPoints[i].second > maxY) {
				maxY = childPoints[i].second;
			}
			if (childPoints[i].second < minY) {
				minY = childPoints[i].second;
			}
		}

		int dir = (maxX - minX) > (maxY - minY) ? 1 : 0;

		if (dir == 1) {
			std::sort(childPoints.begin(), childPoints.end(), [](std::pair<int, int> a, std::pair<int, int> b) {
				if (a.first == b.first) {
					return a.second < b.second;
				}
				return a.first < b.first;
				});
		}
		else if (dir == 0) {
			std::sort(childPoints.begin(), childPoints.end(), [](std::pair<int, int> a, std::pair<int, int> b) {
				if (a.second == b.second) {
					return a.first < b.first;
				}
				return a.second < b.second;
				});
		}

		int median = childPoints.size() % 2 == 0 ? (childPoints.size() / 2) - 1 : (childPoints.size() / 2);

		std::vector<std::pair<int, int>> leftChildPoints(childPoints.begin(), childPoints.begin() + median + 1);
		std::vector<std::pair<int, int>> rightChildPoints(childPoints.begin() + median + 1, childPoints.end());

		Node node = Node();
		node.parent = parent;
		node.cord.first = dir == 0 ? INT_MIN : childPoints[median].first;
		node.cord.second = dir == 0 ? childPoints[median].second : INT_MIN;
		node.axis = dir;


		if (parent == nullptr) {
			node.upmost = INT_MAX;
			node.downmost = INT_MIN;
			node.leftmost = INT_MIN;
			node.rightmost = INT_MAX;
		}
		else {
			node.upmost = parent->upmost;
			node.downmost = parent->downmost;
			node.leftmost = parent->leftmost;
			node.rightmost = parent->rightmost;
			if (parent->axis == 1) {
				if (childIndex == 0) {
					node.rightmost = min(node.rightmost,parent->cord.first);
				}
				else {
					node.leftmost = max(parent->cord.first,node.leftmost);
				}
			}
			else if (parent->axis == 0) {
				if (childIndex == 0) {
					node.upmost = min(parent->cord.second,node.upmost);
				}
				else {
					node.downmost = max(parent->cord.second,node.downmost);
				}
			}
		}
		auto ptr = std::make_shared<Node>(node);

		ptr->leftChild = BuildSub(ptr, leftChildPoints, 0);
		ptr->rightChild = BuildSub(ptr, rightChildPoints, 1);
		ptr->childIndex = childIndex;


		return ptr;
	}

	// 指定した座標に最も近いデータを返す
	std::pair<int, int> C2DTree::FindNearest(int x, int y) {

		Node* node = root.get();
		while (node->leftChild != nullptr && node->rightChild != nullptr) {
			if (node->axis == 0) {
				if (y <= node->cord.second) {
					node = node->leftChild.get();
				}
				else {
					node = node->rightChild.get();
				}
			}
			else if (node->axis == 1) {
				if (x <= node->cord.first) {
					node = node->leftChild.get();
				}
				else {
					node = node->rightChild.get();
				}
			}
			else {
				assert("axis is invalid");
			}
		}

		int distance = (node->cord.first - x) * (node->cord.first - x) + (node->cord.second - y) * (node->cord.second - y);
		if (distance == 0) {
			distance = INT_MAX;
		}

		std::pair<int, std::pair<int, int>> minResult = std::pair<int, std::pair<int, int>>(distance, node->cord);
		std::shared_ptr<Node> parent = node->parent;
		Node currentNode = *node;

		while (parent != nullptr) {

			Node& leftNode = *parent->leftChild;
			Node& rightNode = *parent->rightChild;

			bool flag = false;

			if (parent->axis == 0) {
				if ((parent->cord.second - y) * (parent->cord.second - y) <= minResult.first) {
					flag = true;
				}
			}
			else if (parent->axis == 1) {
				if ((parent->cord.first - x) * (parent->cord.first - x) <= minResult.first) {
					flag = true;
				}
			}

			if (currentNode.childIndex == 0) {

				if (flag) {
					std::pair<int, std::pair<int, int>> result = getChildrenMinDistance(rightNode, x, y,minResult.first);
					if (result.first < minResult.first) {
						minResult = result;
					}
				}
				currentNode = *parent;
				parent = parent->parent;
				continue;
			}
			else if (currentNode.childIndex == 1) {
				if (flag) {
					std::pair<int, std::pair<int, int>> result = getChildrenMinDistance(leftNode, x, y, minResult.first);
					if (result.first < minResult.first) {
						minResult = result;
					}
				}
				currentNode = *parent;
				parent = parent->parent;
				continue;
			}
		}


		return minResult.second;
	}

	C2DTree::Node C2DTree::recursive(Node node) {
		if (node.leftChild == nullptr && node.rightChild == nullptr) {
			std::cout << node.cord.first << "," << node.cord.second << std::endl;
			return node;
		}
		std::cout << "axis:" + std::to_string(node.axis) << " cord1:" + std::to_string(node.cord.first) << " cord2:" + std::to_string(node.cord.second) << std::endl;
		recursive(*node.leftChild);
		recursive(*node.rightChild);
		return node;
	}

	std::pair<int, std::pair<int, int>> C2DTree::getChildrenMinDistance(Node& node, int x, int y,int minDistance) {

		// std::cout << node.cord.first << "," << node.cord.second << std::endl;

		if (node.leftChild == nullptr && node.rightChild == nullptr) {
			int distance = (x - node.cord.first) * (x - node.cord.first) + (y - node.cord.second) * (y - node.cord.second);

			if (distance == 0) {
				distance = INT_MAX;
			}

			return std::pair<int, std::pair<int, int>>(distance, node.cord);
		}

		int diff = ceil(std::sqrt(minDistance));

		// 長方形の交差判定
		bool flag = (max(node.leftmost, x - diff) <= min(node.rightmost, x + diff)) && (max(node.downmost, y - diff) <= min(node.upmost, y + diff));

		if (!flag) {
			return std::pair<int, std::pair<int, int>>(INT_MAX, std::pair<int, int>(-1, -1));
		}

		std::pair<int, std::pair<int, int>> leftResult = getChildrenMinDistance(*node.leftChild, x, y,minDistance);
		std::pair<int, std::pair<int, int>> rightResult = getChildrenMinDistance(*node.rightChild, x, y,minDistance);

		if (leftResult.first < rightResult.first) {
			return leftResult;
		}

		return rightResult;

	}

	void C2DTree::clear() {
		clearSub(root);
	}

	void C2DTree::clearSub(std::shared_ptr<Node> node) {
		if (node->leftChild == nullptr && node->rightChild == nullptr) {
			node->parent.reset();
			node.reset();
			return;
		}
		if (node->leftChild != nullptr) {
			clearSub(node->leftChild);
		}

		if (node->rightChild != nullptr) {
			clearSub(node->rightChild);
		}
		node->parent.reset();
		node.reset();
		return;
	}

}