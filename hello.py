

import myModule
myModule.helloworld()
print(myModule.fib(10))
print(myModule.error)
print(myModule.plus(10, 15))
print(myModule.system("ls -lsa"))
print(myModule.system("apt get install sadasdas"))
print(myModule.system("ls -lsa"))

myModule.set_callback(print, ("hi", "hello", 1))
myModule.call_callback()
