

## issue

Score：300

Description：Lock cf use hash index to speedup point lookup. For prewrite(first step of 2PC) stage, the key is not exist in lock cf usually, so we can use bloom filter to speedup this step, for commit(second step of 2PC) stage, the key is exist in lock cf usually, hash index will can improve the lookup speed.

Link: https://github.com/tikv/tikv/issues/6231





Score：3000

Description：Currently we query blob cache in BlobFileReader. It is done after getting file metadata (BlobStorage::FindFile) and getting the file reader from BlobFileCache, both of which require mutex lock. If we move blob cache query to before those two calls, we saves the two mutex lock and other overheads for a cache hit query.

Link: https://github.com/tikv/titan/issues/140





Score：8400

Description：Merge Titan blob cache with RocksDB block cache, and implement PrioritizedCache which caches block content in high pri pool and caches blob content in low pri pool.

Link: https://github.com/tikv/tikv/issues/5742







