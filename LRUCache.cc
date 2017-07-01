#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

template <class K, class V>
struct Node{
	K key;
	V value;
	Node* next;
	Node* pre;
};

template<class K, class V>
class LRUCache{
public:
	LRUCache(size_t num){
		entry = new Node<K, V>[num];
		for(int i = 0; i != num; i++){
			vecFree.push_back(entry + i);
		}
	
		head = new Node<K, V>;
		tail = new Node<K, V>;
		head->pre = NULL;
		head->next = tail;
		tail->pre = head;
		tail->next = NULL;
	}	
	
	~LRUCache(){
		delete head;
		delete tail;
		delete [] entry;
	}
	
	void Set(K key, V value){
		Node<K, V>* node = mapNode[key];
		if(!node){
			if(vecFree.size() != 0){
				node = vecFree.back();
				vecFree.pop_back();
			}
			else{
				node = tail->pre;
				detach(node);
				mapNode.erase(node->key);
			}
			mapNode[key] = node;
			node->key = key;
		}
		else{
			detach(node);
		}
		
		node->value = value;
		attach(node);
	}
	
	V Get(K key){
		Node<K,V>* node = mapNode[key];
		if(!node){
			return V();
		}
		detach(node);
		attach(node);
		return node->value;
	}
	
	void HeadPrint(){
		Node<K, V>* node = head->next;
		while(node){
			if(node == tail){
				break;
			}
			cout<<node->key<<", "<<node->value<<endl;
			node = node->next;
		}
	}
	
	void TailPrint(){
		Node<K, V>* node = tail->pre;
		while(node){
			if(node == head){
				break;
			}
			cout<<node->key<<", "<<node->value<<endl;
			node = node->pre;
		}
	}

private:
	void attach(Node<K, V>* node){
		node->pre = head;
		node->next = head->next;
		head->next->pre = node;
		head->next = node;
	}
	
	void detach(Node<K, V>* node){
		node->pre->next = node->next;
		node->next->pre = node->pre;
		node->next = NULL;
		node->pre = NULL;
	}

	Node<K, V> *head;
	Node<K, V> *tail;
	Node<K, V> *entry;		//初始化时申请的Node指针数组，用于析构
	vector<Node<K, V>*> vecFree;
	unordered_map<K, Node<K, V>*> mapNode;
};

int main(){
	LRUCache<string, int> cache(3);
	cache.Set("111", 1);
	cache.Set("222", 2);
	cache.Set("333", 3);
	cache.Set("444", 4);
	cout<<cache.Get("222")<<endl;
	cache.HeadPrint();
	cache.TailPrint();
	return 0;
}
