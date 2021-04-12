/*
 * @lc app=leetcode.cn id=20 lang=cpp
 *
 * [20] 有效的括号
 */

// @lc code=start

/**
 * 青铜：看到题目根本不不怎么做-->题目阅读五遍 发现需要stack
 * 白银：知道for stack如何写一个统一比较规则---> 循环遍历 循环条件2个，然后转换成比较stack 和s 元素大小
 *  类别 比较2个指针是否相同，有三个情况
 *  case1 （）    存在 （left和right都同时存在）
 *  case2  () )  字符串还存在（right）,但是栈里没有记录了（left不你存在了）
 *  case3  ( ()  字符串遍历完毕（right不存在了），但是stack依然存在（left） 不匹配
 *  类别： 判断是否同是否一个对称tree
 * 王者：tidb
 * Lex & Yacc
 * SQL Parser 的功能是把 SQL 语句按照 SQL 语法规则进行解析，将文本转换成抽象语法树（AST）
 */
class Solution
{
public:
    bool isValid(string s)
    {
        //01 定义数据结构
        stack<char> path;
        //存储记录轨迹 why 如何想起来的，后面访问的优先使用符合栈特点。

        map<char, char> ruleMap;
        // 定义规则  判断字符串是否有效 这里记录全部规则
        // 这里放弃 if判断，规则存储起来。可以新增判断。
        //而不是上来固定。
        //不考虑浪费空间问题，让代码更加简洁

        //02 初始化数据结构 why
        ruleMap['('] = ')';
        ruleMap['{'] = '}';
        ruleMap['['] = ']';

        //airbnb | amazon | bloomberg | facebook | google | microsoft | twitter | zenefits

        //03 判断字符串是否有效
        //遍历一次【不变】，比较存储全局遍历的规则【变化】

        //循环：判断条件 有2个 s 和stack 不为null
        for (char cur : s)
        {
            //01 什么情况下入栈
            if (ruleMap.count(cur) == 1)
            {
                path.push(cur);
            }
            else
            {

                //！！！ 难点：之前遍历是tree 非递归，for循环的 stack这里 一样，判断是否empty
                char left = ruleMap[path.top()];
                char right = cur;
                if (path.empty() || left != right)
                {
                    //case 1 case 2 () )

                    //比较2个变量大小 --那2个变量？
                    //01 right 遍历字符串cur for循环判断
                    //02 left：path存储记录 stack判断
                    //03 判断left 和right存在不存在
                    return false;
                }
            }
        }
        //case3  ( ()

        return path.empty();
    }
};
// @lc code=end

//整体有效的括号字符串-->一对有效-->定义2个变量 left，right 指向（） --> stack 做left ，字符串存储right
//--> 如果有一方不存在就肯定不是

#include <iostream>
#include <stack>
#include <map>
using namespace std;

bool isValid(string s)
{
    //01 定义数据结构
    stack<char> path;
    map<char, char> checkMap;

    //02 初始化
    checkMap['('] = ')';
    checkMap['['] = ']';
    checkMap['{'] = '}';

    //03 循环条件 stack for

    for (int i = 0; i < s.size(); i++)
    {
        if (checkMap.count(s[i]) == 1)
        {
            path.push(s[i]);
            //入栈操作
        }
        else if (s[i] == ')')
        {
            char right = s[i];
            if (path.empty())
            {
                return false; //why () )
                //循环条件 stack不空，string不空.其中一个null了，必然不是
            }
            char left = path.top();
            if (left != right)
            {
                return false;
            }

            //忘记  出栈操作
            path.pop();
        }
        else
        {
            return false;
        }
    }
    //!!! 合并链表 和判断 tree是否对称 都使用使用技巧
    //假如 判断 2个指针大小 包括 其中一个不存在情况。
    return path.empty();
}

int main()
{
    string input;
    cin >> input;
    if (isValid(input))
    {
        cout << "YES" << endl;
    }else
    {
        cout << "NO" << endl;
    }
}