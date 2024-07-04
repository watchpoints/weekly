#include <cassert>
#include <iostream>
#include <string>
#include <leveldb/db.h>

//g++ -o day1 day1.cpp -pthread -lleveldb -std=c++11
int main() {
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, "./testdb", &db);
  assert(status.ok());

  std::string key = "name";
  std::string value = "wangchuanyi";
  std::string get;

  leveldb::Status s = db->Put(leveldb::WriteOptions(), key, value);
  
  if (s.ok()) s = db->Get(leveldb::ReadOptions(), key, &get);
  if (s.ok()) {
	std::cout << "读取到的与(key=" << key << ")对应的(value=" << get << ")" << std::endl;
  }
  else {
	std::cout << "读取失败!" << std::endl;
  }

  delete db;

  return 0;
}
