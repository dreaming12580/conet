/*
 * =====================================================================================
 *
 *       Filename:  http_server.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年07月26日 23时58分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  piboyeliu
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "svrkit/http_server.h"
#include "thirdparty/glog/logging.h"
#include "thirdparty/gflags/gflags.h"
#include "svrkit/ip_list.h"

DEFINE_string(server_addr, "127.0.0.1:8080", "server address");

using namespace conet;

http_server_t g_server;


int proc_hello(void *arg, http_ctx_t *ctx,
        http_request_t *req, http_response_t *resp)
{
   resp->body = "hello\r\n";
   return 0;
}



int main(int argc, char * argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, false); 
    google::InitGoogleLogging(argv[0]);

    server_t base_server;
    int ret = 0;
    std::vector<ip_port_t> ip_list;
    parse_ip_list(FLAGS_server_addr, &ip_list);
    if (ip_list.empty()) {
        fprintf(stderr, "server_addr:%s, format error!", FLAGS_server_addr.c_str());
        return 1;
    }
    ret = init_server(&base_server, ip_list[0].ip.c_str(), ip_list[0].port);
    if (ret) {
        fprintf(stderr, "listen to %s\n, failed, ret:%d\n", FLAGS_server_addr.c_str(), ret);
        return 1;
    }
    g_server.server = &base_server;

    registry_cmd(&g_server, "/hello", proc_hello, NULL);

    start_server(&g_server);
    while (conet::get_epoll_pend_task_num() >0) {
        conet::dispatch();
    }
    return 0;
}