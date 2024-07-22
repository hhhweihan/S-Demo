可以使用pytest让函数测试多组数据参数组合

如果参数集包含多个参数，可以在装饰器中指定参数名，示例代码1：

```python-repl
@pytest.mark.parametrize("x,y", [(1,2),(3,4),(5,5)])
def test_add(x,y):
	assert add(x, y) == x+y
```

如果想要更清晰地看到每个参数集的名称，可以使用参数名，示例代码2：

```python-repl
@pytest.mark.parametrize("x,y", [(1,2),(3,4),(5,5)], ids=["case1", "case2", "case3"])
def test_add(x,y):
	assert add(x, y) == x+y
```

参数动态化，示例代码3：

```python-repl
@pytest.mark.parametrize("x,y", [(i, i*2) for i in range(1,4)])
def test_add(x,y):
	assert(x, y) == x+y
```
