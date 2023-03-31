# 基于B+树的简易KV数据库案例
1. 该项目是一个使用C++的基于B+树的小型关系型数据库用于理解SQLlite的工作原理，底层使用二进制存储数据表(id,name,age,email)
2. 可以利用shell执行基本的CURD操作:
   1. 新建记录(create):insert db {index} {name} {age} {email};
   2. 更新记录(update):update db {name} {age} {email} where id={index};
   3. 单条读取记录(read):select * from db where id={index};
   4. 范围读取(read in range):select * from db where id in({minIndex},{maxIndex});
   5. 删除记录(delete):delete from db where id ={index};
3. 




## 参考
1. https://cstack.github.io/db_tutorial/