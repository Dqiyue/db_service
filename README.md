# db_service
postgresql proxy class imp

# environment
apt install postgresql
apt install libpq-dev
apt install libpqxx-dev
set up postgressql add your db and password and table

# examples
```
#include "../db_service.h"

int main() {
    NetCore::DBService db("dbname=test user=postgres password=postgres hostaddr=127.0.0.1 port=5432");
    if (db.init() == false) {
        printf("start db service failed\n");
        return 0;
    }
    pqxx::result r;
    std::stringstream ss;
    ss << "select * from dev_state where dev_id = " << 1;
    db.query(r, ss);
    if (r.size()) {
        pqxx::result::const_iterator c = r.begin();
        printf("status is %d\n", c[1].as<int>());
    }else {
        printf("no such element!\n");
    }
    ss.str("");
    ss << "INSERT INTO dev_state (dev_id,state) VALUES(" << 1 << ", 0)";
    if (db.exec(ss) == false) {
        printf("insert failed\n");
    }else {
        printf("insert success\n");
    }
    return 0;
}
```

# run
g++ xxxx.cpp  `pkg-config --cflags libpqxx libpq` `pkg-config --libs libpqxx libpq` -std=c++11 && ./a.out

# attention
it is not a thread safe type

# todo
change it to a thread safe type
