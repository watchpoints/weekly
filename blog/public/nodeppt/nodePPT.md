title: NodePPT
speaker: sixStone
url: https://github.com/ksky521/nodePPT
transition: horizontal3d
files: /js/demo.js,/css/demo.css

[slide]

# NodePPT练习
## 演讲者：sixStone

[slide]

### 特点

* 支持markdown语法，简单写ppt

* 支持导出pdf，导出html

* 支持多种转场动画

* 支持手机控制pc翻页

* 支持多窗口同步

* 支持画笔

* 支持备注笔记

* 内嵌了bootstrap;

[slide]


#### 使用

  1. 安装

  　　　　`npm install -g nodeppt`;

  2. 创建网页幻灯片

  　　　　nodeppt create 幻灯片名字;


[slide]

#### 3.基本配置

* title: 这是演讲的题目

* speaker:  演讲者名字

* url: 可以设置链接

* transition: 转场效果，例如：`zoomin/zoomout/cards/horizontal3d/vertical3d`

* files: 引入js和css的地址，如果有的话~自动放在页面底部

[slide data-transition = 'cards' style = 'background-image:url("/img/bg1.png")']

#### 4.分页

* 通过slide作为每页ppt的间隔;

* 添加单页背景; `slide style = 'background-image:url("/img/bg1.png")'`

* 添加单页动画 `slide data-transition = 'cards'`

[slide]

#### 5.插入html代码

* 使用html/css/js创建自己的页面:
　　　　<div class = 'a'>Hello World!</div>

* 页面样式文件引入在 `files`里;

* 在ppt里添加代码块

```javascript
console.log('hello world')
```

[slide data-on-leave = "pptOut" data-on-enter = "pptIn"]

#### 6.出入回调

* 在当前页设置`slide data-on-leave = "pptOut" data-on-enter = "pptIn"`

* 在`/js/demo.js`里写入回调内容;

[slide]

#### 7.导出html

`nodeppt generate `

[slide]

#### 7.导出pdf[未实现]


```javascript
npm install -g phantomjs-prebuilt
nodeppt start
nodeppt pdf 本地地址 -o a.pdf  //导出到a.pdf

```

[slide]

####　谢谢!

###### [我的博客](http://winterzhao.github.io/)

###### 较为详细的nodePPT教程在[这里](Github：https://github.com/ksky521/nodePPT)




