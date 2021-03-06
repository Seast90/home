const int oo=(1<<30);
struct LTNode{
	int key,d,s; LTNode *l,*r;
	LTNode(int x=0) : l(NULL),r(NULL),key(x),d(0),s(x!=oo) {}
};

void print(LTNode *t){
	if (t==NULL) return;
	print(t->l);
	printf("%d ",t->key);
	print(t->r);
}

LTNode* merge(LTNode* &A,LTNode* &B){
	if(A==NULL || B==NULL) return A==NULL?B:A;
	int k=A->s+B->s;
	if(A->key > B->key) swap(A,B);
	A->r = merge(A->r,B);
	if(A->l==NULL || A->r->d > A->l->d) swap<LTNode*>(A->l,A->r);
	if(A->r==NULL) A->d = 0; else A->d = A->r->d + 1;
	A->s=k;
	return A;
}

void insert(LTNode *&t,int x){
	LTNode *p = new LTNode(x);
	t = merge(t,p);
}

LTNode* extract(LTNode* &t){  
	LTNode *p = t;
	t = merge(t->l,t->r);
	return p;
}

LTNode* build(int a[],int l,int r){
	queue <LTNode*> Q;
	LTNode *x=NULL,*y=NULL;
	for (int i=l; i<=r; i++) {
		x=new LTNode(a[i]);
		Q.push(x);
	}
	while (!Q.empty()){
		if (Q.size()==1) return Q.front();
		x=Q.front(),Q.pop();
		y=Q.front(),Q.pop();
		Q.push(merge(x,y));
	}
}