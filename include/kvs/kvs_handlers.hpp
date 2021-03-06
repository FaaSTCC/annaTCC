//  Copyright 2019 U.C. Berkeley RISE Lab
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//  Modifications copyright (C) 2021 Taras Lykhenko, Rafael Soares
#ifndef INCLUDE_KVS_KVS_HANDLERS_HPP_
#define INCLUDE_KVS_KVS_HANDLERS_HPP_

#include "hash_ring.hpp"
#include "metadata.pb.h"
#include "requests.hpp"
#include "server_utils.hpp"
#include "kvs/transactionalHelper.hpp"


void node_join_handler(unsigned thread_id, unsigned &seed, Address public_ip,
                       Address private_ip, logger log, string &serialized,
                       GlobalRingMap &global_hash_rings,
                       LocalRingMap &local_hash_rings,
                       map<Key, KeyProperty> &stored_key_map,
                       map<Key, KeyReplication> &key_replication_map,
                       set<Key> &join_remove_set, SocketCache &pushers,
                       ServerThread &wt, AddressKeysetMap &join_gossip_map,
                       int self_join_count);

void node_depart_handler(unsigned thread_id, Address public_ip,
                         Address private_ip, GlobalRingMap &global_hash_rings,
                         logger log, string &serialized, SocketCache &pushers);

void self_depart_handler(unsigned thread_id, unsigned &seed, Address public_ip,
                         Address private_ip, logger log, string &serialized,
                         GlobalRingMap &global_hash_rings,
                         LocalRingMap &local_hash_rings,
                         map<Key, KeyProperty> &stored_key_map,
                         map<Key, KeyReplication> &key_replication_map,
                         vector<Address> &routing_ips,
                         vector<Address> &monitoring_ips, ServerThread &wt,
                         SocketCache &pushers, SerializerMap &serializers, TransactionalHelper &th);

void user_request_handler(
    unsigned &access_count, unsigned &seed, string &serialized, logger log,
    GlobalRingMap &global_hash_rings, LocalRingMap &local_hash_rings,
    map<Key, vector<PendingRequest>> &pending_requests,
    map<Key, std::multiset<TimePoint>> &key_access_tracker,
    map<Key, KeyProperty> &stored_key_map,
    map<Key, KeyReplication> &key_replication_map, set<Key> &local_changeset,
    ServerThread &wt, SerializerMap &serializers, SocketCache &pushers, TransactionalHelper &th);

void gossip_handler(unsigned &seed, string &serialized,
                    GlobalRingMap &global_hash_rings,
                    LocalRingMap &local_hash_rings,
                    map<Key, vector<PendingGossip>> &pending_gossip,
                    map<Key, KeyProperty> &stored_key_map,
                    map<Key, KeyReplication> &key_replication_map,
                    ServerThread &wt, SerializerMap &serializers,
                    SocketCache &pushers, logger log, TransactionalHelper &th);

void replication_response_handler(
    unsigned &seed, unsigned &access_count, logger log, string &serialized,
    GlobalRingMap &global_hash_rings, LocalRingMap &local_hash_rings,
    map<Key, vector<PendingRequest>> &pending_requests,
    map<Key, vector<PendingGossip>> &pending_gossip,
    map<Key, std::multiset<TimePoint>> &key_access_tracker,
    map<Key, KeyProperty> &stored_key_map,
    map<Key, KeyReplication> &key_replication_map, set<Key> &local_changeset,
    ServerThread &wt, SerializerMap &serializers, SocketCache &pushers);

void replication_change_handler(
    Address public_ip, Address private_ip, unsigned thread_id, unsigned &seed,
    logger log, string &serialized, GlobalRingMap &global_hash_rings,
    LocalRingMap &local_hash_rings, map<Key, KeyProperty> &stored_key_map,
    map<Key, KeyReplication> &key_replication_map, set<Key> &local_changeset,
    ServerThread &wt, SerializerMap &serializers, SocketCache &pushers, TransactionalHelper &th);

// Postcondition:
// cache_ip_to_keys, key_to_cache_ips are both updated
// with the IPs and their fresh list of repsonsible keys
// in the serialized response.
void cache_ip_response_handler(string &serialized,
                               map<Address, set<Key>> &cache_ip_to_keys,
                               map<Key, set<Address>> &key_to_cache_ips,logger log);

void management_node_response_handler(string &serialized,
                                      set<Address> &extant_caches,
                                      map<Address, set<Key>> &cache_ip_to_keys,
                                      map<Key, set<Address>> &key_to_cache_ips,
                                      GlobalRingMap &global_hash_rings,
                                      LocalRingMap &local_hash_rings,
                                      SocketCache &pushers, ServerThread &wt,
                                      unsigned &rid);

void send_gossip(AddressKeysetMap &addr_keyset_map, SocketCache &pushers,
                 SerializerMap &serializers,
                 map<Key, KeyProperty> &stored_key_map, unsigned long long t_high);

std::pair<string, AnnaError> process_get(const Key &key,
                                         Serializer *serializer, unsigned long long t_low = 0 ,unsigned long long readAt = ULLONG_MAX);

void process_put(const Key &key, LatticeType lattice_type,
                 const string &payload, Serializer *serializer,
                 map<Key, KeyProperty> &stored_key_map);

bool is_primary_replica(const Key &key,
                        map<Key, KeyReplication> &key_replication_map,
                        GlobalRingMap &global_hash_rings,
                        LocalRingMap &local_hash_rings, ServerThread &st);


void send_heart_beat(ServerThreadList &threads, SocketCache &pushers,
                     SerializerMap &serializers,
                     map<Key, KeyProperty> &stored_key_map, TransactionalHelper &th);

#endif // INCLUDE_KVS_KVS_HANDLERS_HPP_
