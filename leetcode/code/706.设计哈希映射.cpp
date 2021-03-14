/*
 * @lc app=leetcode.cn id=706 lang=cpp
 *
 * [706] 设计哈希映射
 */

// @lc code=start
class MyHashMap {
private:
    vector<int>bucket;
    const int len=1e6;
public:
    /** Initialize your data structure here. */
    MyHashMap() {
        bucket=vector<int>(len,-1);
    }
    
    /** value will always be non-negative. */
    void put(int key, int value) {
        bucket[key%len]=value;
    }
    
    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int get(int key) {
        return bucket[key%len];
    }
    
    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void remove(int key) {
        bucket[key%len]=-1;
    }
};

/**
 * Your MyHashMap object will be instantiated and called as such:
 * MyHashMap* obj = new MyHashMap();
 * obj->put(key,value);
 * int param_2 = obj->get(key);
 * obj->remove(key);
 */
// @lc code=end

