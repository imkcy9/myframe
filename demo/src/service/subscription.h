/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   subscription.h
 * Author: kecy
 *
 * Created on 2018年12月9日, 上午1:12
 */

#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H
#include <unordered_map>
#include <set>
#include <bits/unordered_map.h>
typedef kt::user CLIENT;
namespace kt {

    template<typename T>
    class subscription_t {
    public:
        subscription_t<T>(){}
        ~subscription_t<T>() {}
        
        void remove(CLIENT client) {
            auto it = _subscriptions.begin();
            while( it != _subscriptions.end()) {
                it->second.erase(client);
                if(it->second.empty()) {
                    auto o = it;
                    it++;
                    _subscriptions.erase(o);
                    continue;
                }
                it++;
            }
        }
        
        bool remove(CLIENT client, T key) {
            auto it = _subscriptions.find(key);
            if (it != _subscriptions.end()) {
                it->second.erase(client);
                if (it->second.empty()) {
                    _subscriptions.erase(it);
                    return true;
                }
                
            }
            return false;
        }
        
        bool add(CLIENT client, T key) {
            std::set<CLIENT>& clients = _subscriptions[key];
            return clients.insert(client).second;;
        }
        
        const std::set<CLIENT>& get_subscriptions(T key) {
            auto it = _subscriptions.find(key);
            if( it == _subscriptions.end()) {
                return _subscriptions[key];
            }
            return it->second;
        }
    private:
        std::unordered_map<T, std::set<CLIENT>> _subscriptions;
    };
}


#endif /* SUBSCRIPTION_H */

