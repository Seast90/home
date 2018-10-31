#include <vector>
using namespace std;

namespace Scapegoat_Tree {
#define MAXN (100000 + 10)
	const double alpha = 0.75;
	struct Node {
	Node * ch[2];
	int key, size, cover; // sizeΪ��Ч�ڵ��������coverΪ�ڵ������� 
	bool exist;	// �Ƿ���ڣ����Ƿ�ɾ���� 
	void PushUp(void) {
		size = ch[0]->size + ch[1]->size + (int)exist;
		cover = ch[0]->cover + ch[1]->cover + 1;
	}
	bool isBad(void) { // �ж��Ƿ���Ҫ�ع� 
		return ((ch[0]->cover > cover * alpha + 5) || 
				(ch[1]->cover > cover * alpha + 5));
		}
	};
	struct STree {
	protected:
		Node mem_poor[MAXN]; //�ڴ�أ�ֱ�ӷ���ñ��⶯̬�����ڴ�ռ��ʱ�� 
		Node *tail, *root, *null; // ��null��ʾNULL��ָ������㣬tailΪ�ڴ����ָ�룬rootΪ�� 
		Node *bc[MAXN]; int bc_top; // ���汻ɾ���Ľڵ���ڴ��ַ������ʱ������������Щ��ַ 

		Node * NewNode(int key) {
			Node * p = bc_top ? bc[--bc_top] : tail++;
			p->ch[0] = p->ch[1] = null;
			p->size = p->cover = 1; p->exist = true;
			p->key = key;
			return p;
		}
		void Travel(Node * p, vector<Node *>&v) {
			if (p == null) return;
			Travel(p->ch[0], v);
			if (p->exist) v.push_back(p); // �������� 
			else bc[bc_top++] = p; // ���� 
			Travel(p->ch[1], v);
		}
		Node * Divide(vector<Node *>&v, int l, int r) {
			if (l >= r) return null;
			int mid = (l + r) >> 1;
			Node * p = v[mid];
			p->ch[0] = Divide(v, l, mid);
			p->ch[1] = Divide(v, mid + 1, r);
			p->PushUp(); // �Ե�����ά������ά������ 
			return p;
		}
		void Rebuild(Node * &p) {
			static vector<Node *>v; v.clear();
			Travel(p, v); p = Divide(v, 0, v.size());
		}
		Node ** Insert(Node *&p, int val) {
			if (p == null) {
				p = NewNode(val);
				return &null;
			}
			else {
				p->size++; p->cover++;
				
				// ����ֵ������Ҫ�ع���λ�ã�������Ҳ��Ҫ�ع������ڵ㿪ʼҲ��Ҫ�ع����Ա��ڵ�Ϊ���ع� 
				Node ** res = Insert(p->ch[val >= p->key], val);
				if (p->isBad()) res = &p;
				return res;
			}
		}
		void Erase(Node *p, int id) {
			p->size--;
			int offset = p->ch[0]->size + p->exist;
			if (p->exist && id == offset) {
				p->exist = false;
				return;
			}
			else {
				if (id <= offset) Erase(p->ch[0], id);
				else Erase(p->ch[1], id - offset);
			}
		}
	public:
		void Init(void) {
			tail = mem_poor;
			null = tail++;
			null->ch[0] = null->ch[1] = null;
			null->cover = null->size = null->key = 0;
			root = null; bc_top = 0;
		}
		STree(void) { Init(); }

		void Insert(int val) {
			Node ** p = Insert(root, val);
			if (*p != null) Rebuild(*p);
		}
		int Rank(int val) {
			Node * now = root;
			int ans = 1;
			while (now != null) { // �ǵݹ������� 
				if (now->key >= val) now = now->ch[0];
				else {
					ans += now->ch[0]->size + now->exist;
					now = now->ch[1];
				}
			}
			return ans;
		}
		int Kth(int k) {
			Node * now = root;
			while (now != null) { // �ǵݹ����K�� 
				if (now->ch[0]->size + 1 == k && now->exist) return now->key;
				else if (now->ch[0]->size >= k) now = now->ch[0];
				else k -= now->ch[0]->size + now->exist, now = now->ch[1];
			}
		}
		void Erase(int k) {
			Erase(root, Rank(k));
			if (root->size < alpha * root->cover) Rebuild(root);
		}
		void Erase_kth(int k) {
			Erase(root, k);
			if (root->size < alpha * root->cover) Rebuild(root);
		}
	};
#undef MAXN

}
