[https://cloud.tencent.com/developer/article/1493839](https://cloud.tencent.com/developer/article/1493839)

什么是 lvalue, rvalue, xvalue
发布于2019-08-26 20:41:45阅读 3.7K0
直观理解
在 C++11 中，一共有 5 种 value：

lvalue (Left-hand-side value)
rvalue (Right-hand-side value)
xvalue (eXpiring value)
prvalue (Pure rvalue)
glvalue (Generalized lvalue)
它们的关系图：

lvalue 和 xvalue 合称为 glvalue。

prvalue 和 xvalue 合称为 rvalue。

The old saying: in C++ Every value is either an lvalue or an rvalue.

这句话今天仍然正确。不过现在更准确的说法是：

in C++ Every value is either an lvalue or an xvalue or a prvalue.

B神的解释
C++ 之父 Bjarne Stroustrup ：

For all the values, there were only two independent properties:

"has identity" – i.e. an address, a pointer, the user can determine whether two copies are identical, etc.
"can be moved from" – i.e. we are allowed to leave to source of a "copy" in some indeterminate, but valid state.
There are four possible composition:

iM: has identity and cannot be moved from (defined as lvalue)
im: has identity and can be moved from (defined as xvalue)
Im: does not have identity and can be moved from (defined as prvalue)
IM: doesn't have identity and cannot be moved (他认为这种情况在 C++ 中是没有用的)

identity 可以理解为内存地址。因此 lvalue 和 xvalue 都是对应着实实在在的内存地址的，尽管对 xvalue 直接取地址 &xvalue 的语法是非法的。因此只能用间接的方式取得：

struct M { M* a = this;};cout << M().a << " " << M().a << endl;
复制
以上的代码打印出了 xvalue M() 的内存地址。

等等！可能有一定基础的人会问：M() 应该是一个 prvalue，而不是 xvalue，作者是不是在胡扯？

看完后文就知答案。

入门解释
举例：

int a = 1;int& b = a;int&& c = 1;int f() { return 1; }int& g() { return a; }int&& h() { return (int&&)a; }
int main() { a = 1; b = 1; c = 2; f(); g() = 2; h();}
复制
int a = 1;

定义一个 lvalue a，并初始化为 value 1。这里的 = 不是赋值语句，而是表示初始化。在 C++ 中赋值和初始化是两个不同的概念。
括号里面的 1 叫做整型字面量(integer literal)。
注意这个变量是有名字的(named)
这里 named 特指有单独一个 identifier 来标识它。组合的不算(例如 a.m 等)。
进阶推广：
named value 一定是 lvalue
unamed value 一定是 rvalue，除了 unamed lvalue reference。
int& b = a;

定义一个 lvalue reference b，并用 lvalue a 来初始化。这里的 = 不是赋值语句，而是表示初始化。表示让 b 成为 a 的一个 alias(别名)，它们都对应同一个内存地址。
lvalue reference 只能用 lvalue 来初始化，不能用 rvalue 初始化
b 是一个 named lvalue reference，它有名字，所以是 lvalue
进阶推广：
const lvalue reference 也可以用 rvalue 来初始化
lvalue reference(无论是否 named，无论 to objects or to functions) 一定是 lvalue
int&& c = 1;

定义一个 rvalue reference c，并用 rvalue 1 来初始化。这里的 = 不是赋值语句，而是表示初始化。表示让 c 成为 rvalue 1 的一个 alias(别名)，它们都对应同一个内存地址。
(插一句：那 1 的内存地址是什么？如果 c 是在 stack 内定义的，则这个 rvalue 1 的地址就在 stack 中；如果 c 是在 global 内定义的，则这个 rvalue 1 的地址就在 static 中)
rvalue reference 只能用 rvalue 来初始化，不能用 lvalue 初始化
c 是一个 named rvalue reference，它有名字，所以是 lvalue
进阶推广：
对于 const rvalue reference，也只能用 rvalue 来初始化
int f() { return 1; } 和 f();

f 返回值类型是 T，则函数表达式 f() 是一个 rvalue，它代表了一个没有名字的、临时的、 non-reference 变量。
强调一下，"表达式f()是rvalue" 这一事实跟它 return 1(rvalue); 或者 return a(lvalue); 或者 return b(lvalue reference); 或者 return c(rvalue reference); 无关，仅仅与函数原型中的返回值类型 T 有关，即 T f() {...}。
f() = 1; 语法错误，因为 rvalue 不允许写在 built-in = 赋值语句的左边，这也是 rvalue 名字的由来。
int& g() { return a; } 和 g() = 2;

g 返回值类型是 T&，函数表达式 g() 是一个 lvalue，它代表了一个临时的 unamed lvalue reference 变量。
强调一下，"表达式g()是lvalue" 这一事实跟它 return a(lvalue); 或者 return b(lvalue reference); 或者 return c(rvalue reference); 无关，仅仅与函数原型中的返回值类型 T& 有关，即 T& f() {...}。
但是这里有一个限制，g 的 return 语句不能写 return 1;，即只能 return lvalue，不能 return rvalue。这是因为，如果用 ? 来表示这个临时的 unamed lvalue reference，函数返回的过程可以理解为 int& ? = a;，前面提到过，lvalue reference 只能用 lvalue 来初始化。
f() = 1; 合法。lvalue 可以写在 built-in = 赋值语句的左边，这也是 lvalue 名字的由来。
int&& h() { return (int&&)a; } 和 h();

h 返回值类型是 T&&，函数表达式 h() 是一个 xvalue，它代表了一个临时的 unamed rvalue reference 变量。
强调一点："表达式h()是xvalue" 这一事实跟它 return 1; 或者 return f(); 或者 return (int&&)a; 无关，仅仅与函数原型中的返回值类型 T&& 有关，即 T&& f() {...}。
这里也有一个限制，h 的 return 语句不能写 return a 或 b 或 c;，即只能 return rvalue，不能 return lvalue。这是因为，如果用 ? 来表示这个临时的 unamed rvalue reference，可以理解为 int&& ? = (int&&)a;，前面提到过，rvalue reference 只能用 rvalue 来初始化。
补充一点：正常情况下不会选择 return 1 作为 h() 的返回值，因为 1 是存在于 h() 的 stack 的，h() 返回后，其 stack 就失效了，那 h() 这个临时变量的内存对应一个失效的 stack 地址。
(int&&)a 等同于 static_cast<int&&>(a)，表示把 lvalue a 强制转换成一个 rvalue，从而在语法上就可以把 (int&&)a 初始化给 int&& ? 了。在实质上 (int&&)a 和 a 还是对应同一个内存地址。std::move 就是这样实现的。
h() = 1; 不合法。rvalue 不允许写在 built-in = 赋值语句的左边。
详细解释
lvalue
left-hand-side value，左值，其特点是：

可以出现在 C++ built-in = 赋值语句的左边
一般有名字，除了 unamed lvalue reference
如果有单独的一个 identifier 来表示它，它一定是 lvalue
可以用 & 符号取其地址(除了bitfield)
其生命周期为其所在的 scope
在所有 literal 中，只有 string literal 是 lvalue：cout << &"www" << endl;
其他 literal 是 rvalue：cout << &'w' << endl; 非法
rvalue
right-hand-side value，右值。xvalue 和 prvalue 合称 rvalue，他们的共同特点是：

一般只能出现在 C++ built-in = 赋值语句的右边，不能出现在左边
强调是 built-in = 赋值语句：string("hello") 是 rvalue，但可以出现在左边：string("hello") = string("world")。注意，这个等号是 string class 定义的 operator=，不是 C++ built-in
没有名字
不可以用 & 符号取其地址
其生命周期一般仅限于本表达式语句(expression statement)，即本 expression statement 执行后，在执行下一条 statement 前，其内存就被回收。
例外：rvalue 可以用来初始化 rvalue reference 或 const lvalue reference，进而 "该rvalue" 的 lifetime is extended until the scope of the reference ends. (实际上发生了微妙的变化-见后文 Temporary Materialization)
xvalue
eXpiring value，如下四种情况会产生 xvalue：

(1) function calls that return rvalue references to objects

例如上文的表达式 h()，或者更常见的 std::move(t)
(2) casts to rvalue references to objects

例如上文的表达式 (int&&)a，或者更常见的 static_cast<T&&>(t)
(3) subscripting into a array xvalue。

例如 int a[5]; std::move(a)[0];
更准确的说法：必须是 built-in subscript expression，因为 operator[] 是可以重载的
(4) data member access into an xvalue of class type

例如 std::move(x).m 是一个 xvalue
以及 std::move(x).*mp 是一个 xvalue。mp 叫做 member object pointer。
更准确的说法是要分情况讨论：
对于 std::move(x).m ，仅限于 m 是普通 data member(non-static, non-reference)，而不能是 member enumerator 或者 static member 或者 reference member 或者 static member function 或者 普通(non-static) member function。
对于 std::move(x).*mp，仅限于 mp 是 pointer to data member，不能是 pointer to member function
总结一下，(1)和(2)创建的都是 unamed rvalue references to objects (objects 不同于 functions)

unamed rvalue references to objects are treated as xvlaue
unamed rvalue references to functions are treated as lvalue. (named 更不必说了，有名字的一定是 lvalue。lvalue references 也更不必说了。)
(3)和(4)实际上都是取一个 xvalue 结构中的子数据块(data subobjects)，得到的也是一个 xvalue。

prvalue
在 rvalue 中，除了上面提到的 xvalue 的四种情况，其他都是 prvalue。例如:

所有 literals：bool literal (例如 true)，integer literal (例如 42) 等等。
但不包括 string literals (例如 "hello" 实际上是一个 lvalue，其类型是 const char *)
实质上的函数调用：(return non-reference)
f() function call
a.f() static or non-static member function
A() 包括各种构造函数
str1 + str2 重载 operator 也相当于函数调用
functor() (对应 operator() 重载)
[]{}() lambda (对应 operator() 重载)
一些 built-in operator 产生的运算结果：
a++ 自增自减
a+b 加减乘除
a&b 位运算
a&&b 逻辑运算
a<b 关系运算
&a 取地址
a, b comma expression (如果b 是 rvalue)
(int)a 或 static_cast`<int>`(a) 强制类型转换
a ? b : c 在某些情况下是 prvalue
enumerator 枚举值：
enum { yes, no }; 中的 yes, no
a.m 或 p->m，其中 m 是 member enumerator
普通成员函数本身(注意不是函数调用)
a.m 或 p->m 或 a.*pm 或 p->*pm，其中 m 和 mp 都对应普通成员函数(non-static)
this 指针
glvalue
generalized lvalue, xvalue 和 lvalue 合称 glvalue，他们的共同特点是：

have identity
could be polymorphic
could be incomplete type
例如，类似 std::move 的语法：
class unknown;unknown&& func(const unknown& u) { return (unknown&&)u;}

这里 (unknown&&)u 就是一个 incomplete type
这段代码在一个 Translation Unit 内 compile 是合法的。即 gcc -c，只编译，不链接。

回到前面是否在"胡扯"的问题，把 M().a 中的 M() 叫做 xvalue 合理吗？这个现象叫做 Temporary materialization，前方高能预警，可能刷新认知：

Temporary Materialization
在某些情况下，一个 prvalue(必须是 complete type) 会被悄悄转换成 xvalue。也就是为一个本来不实际存在于内存中的对象(prvalue)， 悄悄建立了一个匿名的、临时的、在内存中有地址的对象(xvalue)。这种行为是被 C++ 规范允许且明确定义了的，就叫做 Temporary Materialization。

Temporary Materialization 发生在下面几种情景中：

(1) when binding a reference to a prvalue

该 prvalue 的 lifetime 被延长至和该 reference 一致。
例1(named)：bind to rvalue reference: int&& c = 1;
例2(named)：bind to const lvalue reference: const int& c = 1;
例3(unamed)：string&& k() { return string("hello"); } 这种用法仅限于举例说明，因为它返回了 callee stack 变量的引用，如果把该 unamed reference k() 理解成指针，则 k() 指向已经失效的 callee stack 中的地址。实际中极少用到，编译也会报警。注意不同于 RVO。
(2) when performing a member access on a class prvalue

A().m; pvalue A() 自动变成一个 xvlaue
A().*mp; pvalue A() 自动变成一个 xvlaue
(3) when performing an (prvalue array)-to-pointer conversion or subscripting on an array prvalue

prvalue array to reference: (结合 (1) 和 prvalue array 的例子)
int (&& a)[2] = (int[]){1, 2};或者auto && a = (int[]){1, 2};

prvalue array to pointer implicit conversion: linux gcc 似乎不支持？
[&amp;](http://int%2A/){}((int[]){1,2});

subscript into prvalue array:
((int[]){1, 2})[0];

一个猜想
下面的例子打印了 prvalue M() 的地址？

struct M { operator M*() { return this; }};cout << M() << endl;
复制
这样 M() 的地址是可以打印出来的。不是说 M() 是 prvalue，没有地址的吗？

实际上，这里问题发生在 operator M*()，它看似没有参数，实质上它的参数是 this，且概念上形式为 M().this (1)，符合 Temporary Materialization 的条件。

那如何理解 prvalue没有实际的内存地址 呢？我的理解是：当出现了一个 prvalue 时，编译器如果可以不为它分配内存地址就能实现，那就不为它分配内存地址；如果编译器必须为它分配一个内存地址才能实现，那就是一次 Temporary Materialization。

例如最简单的：

int a = 1; 其中 prvalue 1 就被实现成一个立即数，立即数显然是没有地址的。
T a = T(); 构造函数 T::T() 只被调用了一次，等号右边的 prvalue T() 根本没有存在过(或者理解成 T() 是直接在 a 的地址上构造出来的)，没存在过显然也就没有地址。不过这种初始化的写法一般很少用，常见到的 T(xx) 形式的临时 object 一般都是 xvlaue，例如上例中的 M().a 和 M()。
以上 (1) 属个人猜想，请了解 compiler 的朋友指正。

补充
下面几种表达式在不同情况下拥有的不同 value category：

函数

function 本身是 lvalue，因为有名字
references to functions 是 lvalue，不论是左值引用还是右值引用
member static function 也是 lvalue。
a.f, p->f, a.*pf, p-> *pf, 其中 f 和 pf 都是普通成员函数(non-static)，则它们都是 prvalue，这个我暂时还不理解为什么这样设计。
C++中没有 references to these member funcitons，例如：
struct M { void f() {} void (&r)() = f; }; 语法错误
struct M { void f() {} auto & r= f; }; 语法错误
void (M::* &mp)() = a.f; 语法错误
auto & mp = a.f; 语法错误
以上换成 && 也一样。
function call (例如执行某函数 f(); 就称为 function call):
返回 non-reference 的 function call 是 rvalue
返回 左值引用 的 function call 是 lvalue
返回 右值引用 的 function call 是 xvalue
含义扩展：只要实质上发生了函数调用，包括 a.m(1) 这种成员函数调用，上面的规则都适用。不需要考虑 a.m 本身是 lvalue 还是 prvalue。
a.m

一般 a.m 都是 lvalue。例如：m 是 数据成员，或者 static成员函数。
m 是 member enumerator，或者 普通成员函数(non-static)，则 a.m 是 prvalue
a 是 rvalue(对于 prvalue，在这里自动变成 xvalue)，m 是 普通数据成员(non-static, non-reference)，则 a.m 是 xvlaue
以上的条件，从上往下越来越 specific，越是 specific 越是优先被匹配
p->m

以下讨论的前提都是对于 built-in -> operator
一般 p->m 都是 lvalue。例如：m 是 数据成员，或者 static成员函数。
m 是 member enumerator，或者 普通成员函数(non-static)，则 p->m 是 prvalue
p->m 永远不会产生 xvalue，即使 p 是 xvalue
a.*mp

a 是 lvalue，mp 是 pointer to data member，则 a.*mp 是 lvalue
a 是 rvalue(对于 prvalue，在这里自动变成 xvalue)，mp 是 pointer to data member，则 a.*mp 是 xvalue
mp 是 pointer to member function，则 a.*mp 是 prvalue
注意，mp 不可能指向 member numerator 或者 static member 或者 reference member，这些都是不符合 pointer to member 语法的。因此，只需要讨论普通 data member 和普通 member function 就可以了。
p->*mp

以下讨论的前提都是对于 built-in ->* operator
mp 是 pointer to data member，则 p->*mp 是 lvalue
mp 是 pointer to member function，则 p->*mp 是 prvalue
p->*mp 永远不会产生 xvalue，即使 p 是 xvalue

思考题

std::forward`<T>`(t) 这个表达式是什么类型的 value？

References

本文介绍的比较简略，详细的介绍还是要看官方文档：

[cppreference.com](http://cppreference.com/)：你说的都对
N3055：年久失修，但有些地方讲的很清晰
B神：圣人不仁，以百姓为刍狗
