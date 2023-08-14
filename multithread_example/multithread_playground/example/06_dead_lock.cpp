#include <iostream>
#include <mutex>
#include <set>
#include <thread>

using namespace std;

class Account {
public:
  Account(string name, double money): mName(name), mMoney(money) {};

public:
  void changeMoney(double amount) {
    mMoney += amount;
  }
  string getName() {
    return mName;
  }
  double getMoney() {
    return mMoney;
  }
  mutex* getLock() {
    return &mMoneyLock;
  }

private:
  string mName;
  double mMoney;
  mutex mMoneyLock;
};

class Bank {
public:
  void addAccount(Account* account) {
    mAccounts.insert(account);
  }

  bool transferMoney(Account* accountA, Account* accountB, double amount) {
    

    
    //为了保证线程安全，在修改每个账号之前，需要获取相应的锁。 下面代码可能会有ABBA死锁
    lock_guard<mutex> guardA(*accountA->getLock());
    lock_guard<mutex> guardB(*accountB->getLock());
    
    //解决死锁方案1:
    // lock(*accountA->getLock(), *accountB->getLock());//通过lock函数来获取两把锁，标准库的实现会保证不会发生死锁。
    // lock_guard<mutex> guardA(*accountA->getLock(),adopt_lock);//adopt_lock表示这个mutex已经被lock了，不需要再次lock。
    // lock_guard<mutex> guardB(*accountB->getLock(),adopt_lock);
    //解决死锁方案2:
    // unique_lock<mutex> lockA(*accountA->getLock(), defer_lock);
    // unique_lock<mutex> lockB(*accountB->getLock(), defer_lock);
    // lock(*accountA->getLock(), *accountB->getLock());
    //解决死锁方案3:
    //scoped_lock lockAll(*accountA->getLock(), *accountB->getLock());
    
    
    //判断转出账户金额是否足够，如果不够此次转账失败。
    if (amount > accountA->getMoney()) {
      return false;
    }
    //进行转账
    accountA->changeMoney(-amount);
    accountB->changeMoney(amount);
    return true;
  }

  double totalMoney() const {
    double sum = 0;
    for (auto a : mAccounts) {
      sum += a->getMoney();
    }
    return sum;
  }

private:
  set<Account*> mAccounts;
};
//解决乱序输出问题
//mutex sCoutLock;
void randomTransfer(Bank* bank, Account* accountA, Account* accountB) {
  while(true) {
    double randomMoney = ((double)rand() / RAND_MAX) * 100;
    if (bank->transferMoney(accountA, accountB, randomMoney)) {
      

      {
        //lock_guard<mutex> guard(sCoutLock);
      cout << "Transfer " << randomMoney << " from " << accountA->getName()
           << " to " << accountB->getName()
           << ", Bank totalMoney: " << bank->totalMoney() << endl;


      }
    
    } else {
      {
        //lock_guard<mutex> guard(sCoutLock);//生存周期就是else里面的花括号，等价于下面的lock和unlock;
      //sCoutLock.lock();

      cout << "Transfer failed, "
           << accountA->getName() << " has only $" << accountA->getMoney() << ", but "
           << randomMoney << " required" << endl;
    
      //sCoutLock.unlock();
      }
    }
  }
}

int main() {
  Account a("Paul", 100);
  Account b("Moira", 100);

  Bank aBank;
  aBank.addAccount(&a);
  aBank.addAccount(&b);

  thread t1(randomTransfer, &aBank, &a, &b);
  thread t2(randomTransfer, &aBank, &b, &a);

  t1.join();
  t2.join();

  return 0;
}