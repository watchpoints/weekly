//20. 有效的括号
class Solution {
    bool isValid(string s)
    {

     stack<char> mystack;//保持遍历历史轨迹
     map<char,char>match; //判断是否匹配
     match['(']=')';
     match['[']=']';
     match['{']='}';

     for(char item:s)
     {
         if(match.count(item) == 1)
         {
             mystack.push(item); //如果left
         }else
         {
             if(mystack.empty() == true){
                 return false; //()) 拿不出来多余的了
             }else
             {
                 char left =mystack.top();
                 mystack.pop();

                 //判断是否匹配 "(" ==")"" 永远不相等
                 if（math[left] !=item ）
                 {
                     return false;
                 }
             }

         }
     }
     if (!mystack.empty())
     {
         return false;
     }
     return true;

    }
};

int main()
{

}