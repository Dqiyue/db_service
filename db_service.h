/**
 * 
 * @author:       kylin
 * @email:        kylin.du@outlook.com
 * @dateTime:     2018-06-06 Wed 15:34:58
 * @copyright:    kylin
 * @description:  
 * 
 */

#ifndef _DB_SERVICE_H_
#define _DB_SERVICE_H_

#include <pqxx/connection.hxx>
#include <pqxx/result.hxx>
#include <pqxx/nontransaction.hxx>
#include <pqxx/transaction.hxx>
#include <exception>
#include <string>
#include <stdio.h>

namespace NetCore {

class DBService {
public:
    enum {MAX_SQL_STR_LEN = 4096};
public:
    DBService(const char*& dbinfo) {
        _conn = NULL;
        _dbinfo = dbinfo;
    }
    DBService(const char*&& dbinfo) {
        _conn = NULL;
        _dbinfo = dbinfo;
    }
    DBService(const std::string& dbinfo) {
        _conn = NULL;
        _dbinfo = dbinfo;
    }
    ~DBService() {
        stop();
    }
public:
    inline bool init() {
        if (is_active()) {
            return true;
        }
        if(_conn) {
            delete _conn;
            _conn = 0;
        }

        try {
            _conn = new pqxx::connection(_dbinfo);
            if (is_active()) {
                printf("DBService is ready! dbname is %s\n", _conn->dbname());
                return true;
            }

            if(_conn) {
                delete _conn;
                _conn = 0;
            }
        }catch(const std::exception& e) {
            printf("DBService err : %s\n", e.what());
            stop();
            return false;
        }

        return false;
    }

    inline bool is_active() {
        return (_conn ? _conn->is_open() : false);
    }

    inline void stop() {
        if (is_active()) {
            _conn->disconnect();
            printf("DBService is stopped!\n");
        }
        if(_conn) {
            delete _conn;
            _conn = 0;
        }
    }

    inline void query(pqxx::result& r, const std::stringstream& qstr, const std::string& des=std::string()) {
        try {
            pqxx::nontransaction n_(*_conn);
            r = n_.exec(qstr, des);
        }catch (const std::exception &e) {
            printf("DBService query err >> %s\n", e.what());
        }
    }
    
    inline bool exec(const std::stringstream& cmd, const std::string& des=std::string()) {
        try {
            pqxx::work w_(*_conn);
            pqxx::result r_(w_.exec(cmd,des));
            w_.commit();
            if (r_.affected_rows() == 0) {
                return false;
            }
        }catch (const std::exception &e) {
            printf("DBService exec err >> %s\n", e.what());
            return false;
        }

        return true;
    }
private:
    pqxx::connection* _conn;
    char _buff[MAX_SQL_STR_LEN];
    std::string _dbinfo;
};

};

#endif //_DB_SERVICE_H_