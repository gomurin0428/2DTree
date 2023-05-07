#pragma once

#include <vector>
#include <set>
#include <stack>

// 2DTree�̃N���X�����

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

		// 2DTree�Ƀf�[�^��ǉ�����
		void AddData(int x, int y);

		// 2DTree���\�z����
		void Build();

		// �w�肵�����W�ɍł��߂��f�[�^��Ԃ�
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
			// ���̃m�[�h�����f�[�^�̍��W(�t�m�[�h�łȂ��ꍇ�́A�Е�������INT_MIN�łȂ��B
			// INT_MIN�łȂ�����x���܂���y���Ƃ̌�_)
			std::pair<int, int> cord;
			// ���̃m�[�h�̐e�m�[�h
			std::shared_ptr<Node> parent;
			// ���̃m�[�h�̎q�m�[�h
			std::shared_ptr<Node> leftChild;
			std::shared_ptr<Node> rightChild;
			// ���̃m�[�h�̕�����(0:x��, 1:y��)
			int axis;

			int leftmost;
			int rightmost;
			int upmost;
			int downmost;

			int childIndex;
		};

		std::shared_ptr<Node> BuildSub(std::shared_ptr<Node> parent, std::vector<std::pair<int, int>> childPoints, int childIndex);

		// 2DTree�̍�
		std::shared_ptr<Node> root = nullptr;

		// �f�[�^��ێ�����vector
		std::vector<std::pair<int, int>> points;

		// �f�[�^��ێ�����set
		std::set<std::pair<int, int>> pointsSet;

		Node recursive(Node node);

		std::pair<int, std::pair<int, int>> getChildrenMinDistance(Node& node, int x, int y, int minDistance);

		void clearSub(std::shared_ptr<Node> node);

	};
}
